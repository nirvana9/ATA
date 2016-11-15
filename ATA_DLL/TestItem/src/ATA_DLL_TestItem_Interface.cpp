/*****************************************************************************
 *  Copyright Statement:
 *  --------------------
 *  This software is protected by Copyright and the information contained
 *  herein is confidential. The software may not be copied and the information
 *  contained herein may not be used or disclosed except with the written
 *  permission of MediaTek Inc. (C) 2005
 *
 *  BY OPENING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 *  THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
 *  RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON
 *  AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
 *  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
 *  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
 *  NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
 *  SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
 *  SUPPLIED WITH THE MEDIATEK SOFTWARE, AND BUYER AGREES TO LOOK ONLY TO SUCH
 *  THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL ALSO
 *  NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO BUYER'S
 *  SPECIFICATION OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
 *
 *  BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND CUMULATIVE
 *  LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
 *  AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
 *  OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY BUYER TO
 *  MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 *
 *  THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE
 *  WITH THE LAWS OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF
 *  LAWS PRINCIPLES.  ANY DISPUTES, CONTROVERSIES OR CLAIMS ARISING THEREOF AND
 *  RELATED THERETO SHALL BE SETTLED BY ARBITRATION IN SAN FRANCISCO, CA, UNDER
 *  THE RULES OF THE INTERNATIONAL CHAMBER OF COMMERCE (ICC).
 *
*****************************************************************************/
/*******************************************************************************
 * Filename:
 * ---------
 *   ATA_DLL_TestItem_Interface.cpp
 *
 * Project:
 * --------
 *   ATA_DLL
 *
 * Description:
 * ------------
 *   This module contains the calibration handle
 *
 * Author:
 * -------
 *   Yintong Zhang (mtk70681)
 *
 *==============================================================================
 *           HISTORY
 * Below this line, this part is controlled by PVCS VM. DO NOT MODIFY!!
 *------------------------------------------------------------------------------
 * $Revision$
 * $Modtime$
 * $Log$
 *
 *
 *------------------------------------------------------------------------------
 * Upper this line, this part is controlled by PVCS VM. DO NOT MODIFY!!
 *==============================================================================
*******************************************************************************/

#include "StdAfx.h"
#include <stdarg.h>
#include <stdio.h>
//#include <Windows.h>
#include "..\header\ATA_DLL_TestItem_Interface.h"
#include "..\..\GlobalOBJ.h"


#define	QUEUE_MAX	1000
const unsigned char CR = 13;
const unsigned char LF = 10;

typedef unsigned (WINAPI *TESTITEMINTERFACE_THREADFUNC) (LPVOID LpThreadParameter);

CTestItem_Interface::CTestItem_Interface (ATA_DLL_Handle *current_handle, int timeout, E_ATDLL_TEST_ITEM_COUNT item, char *testName)
{
    sprintf (m_TestItemName, "%s", testName);
    test_item				= item;
    timeout_sec			= timeout;
    test_handle 		= current_handle;

    m_hStateMutex		= CreateMutex(NULL, FALSE, NULL);
    m_hInVector			= CreateSemaphore(NULL, 0, QUEUE_MAX, NULL);
    m_iRespNum			= 0;
    m_iReadingInQuene	= 0;

    b_auto				= true;
    cbTestPostprocess	= NULL;

    m_logStr.empty();
    m_sATResponsePool.clear();
}

CTestItem_Interface::~CTestItem_Interface ()
{
    if (m_hStateMutex != NULL)
    {
        CloseHandle (m_hStateMutex);
        m_hStateMutex = NULL;
    }
    if (m_hInVector != NULL)
    {
        CloseHandle (m_hInVector);
        m_hInVector = NULL;
    }

    m_iRespNum			 = 0;
    m_iReadingInQuene	 = 0;
    ClearATResPool ();
}

void CTestItem_Interface::SetAutoMode (bool automode)
{
    b_auto	= automode;
}

void CTestItem_Interface::SetTestPostProcessCB (CallbackTestItemPostProcess cb)
{
    cbTestPostprocess	= cb;
}

void CTestItem_Interface::ClearATResPool ()
{
    WaitForSingleObject(m_hStateMutex, timeout_sec * 1000);
    m_sATResponsePool.clear();
    m_iRespNum = 0;
    ReleaseMutex(m_hStateMutex);
}

bool CTestItem_Interface::GetATResponse (ATResult &atr)
{
    TRACE ("Get AT Response start: %s\r\n", m_TestItemName);
	MTRACE(g_hEBOOT_DEBUG, "Get AT Response start: %s", m_TestItemName);
    WaitForSingleObject(m_hStateMutex, timeout_sec * 1000);
    TRACE ("Get AT Response: %s Enter\r\n", m_TestItemName);
	//MTRACE (g_hEBOOT_DEBUG,"Get AT Response: %s Enter\r\n", m_TestItemName);

    if (m_iRespNum <= 0)
    {
        TRACE ("Get AT Response: %s num <= 0\r\n", m_TestItemName);
        m_iReadingInQuene ++;
        ReleaseMutex(m_hStateMutex);

        if (WAIT_OBJECT_0 == WaitForSingleObject(m_hInVector, timeout_sec * 1000))
        {
            TRACE ("Get AT Response: %s Notify\r\n", m_TestItemName);
            if (m_iRespNum <= 0)
            {
                return false;
            }

            WaitForSingleObject(m_hStateMutex, timeout_sec * 1000);
            {
                TRACE ("Get AT Response: %s get result - %d\r\n", m_TestItemName, m_iRespNum);
                ATResult atr_temp = m_sATResponsePool[0];
                atr = atr_temp;

                std::vector<ATResult>::iterator it;
                it = m_sATResponsePool.begin();
                m_sATResponsePool.erase(it);

                m_iRespNum --;
                TRACE ("Get AT Response end: %s - %d\r\n", m_TestItemName, atr.resultLst[0].eleLst[0].str_len);
            }
            ReleaseMutex(m_hStateMutex);
        }
        else
        {
            return false;
        }
    }
    else
    {
        ATResult atr_temp = m_sATResponsePool[0];
        atr = atr_temp;

        std::vector<ATResult>::iterator it;
        it = m_sATResponsePool.begin();
        m_sATResponsePool.erase(it);

        m_iRespNum --;
        TRACE ("Get AT Response end: %s - %d\r\n", m_TestItemName, atr.resultLst[0].eleLst[0].str_len);
		MTRACE (g_hEBOOT_DEBUG,"Get AT Response end: %s - %d", m_TestItemName, atr.resultLst[0].eleLst[0].str_len);
        ReleaseMutex(m_hStateMutex);
    }

    return true;
}

bool CTestItem_Interface::SetATResponse (ATResult atr)
{
    TRACE ("Set AT Response start: %s\r\n", m_TestItemName);
    WaitForSingleObject(m_hStateMutex, timeout_sec * 1000);

    if (test_handle->m_fDLLLogFile != NULL)
    {
        char temp_str[89600];
        SYSTEMTIME	cur_time;
        GetLocalTime(&cur_time);
        sprintf (temp_str, "%04d/%02d/%02d %02d:%02d:%02d.%03d [%s] <== %s",
                 cur_time.wYear, cur_time.wMonth, cur_time.wDay,
                 cur_time.wHour, cur_time.wMinute, cur_time.wSecond, cur_time.wMilliseconds,
                 m_TestItemName, atr.resultLst[0].eleLst[0].str_value.c_str());
        fprintf (test_handle->m_fDLLLogFile, "%s\n", temp_str);
        fflush (test_handle->m_fDLLLogFile);
    }

    m_sATResponsePool.push_back(atr);
    m_iRespNum ++;
    if (m_iReadingInQuene > 0)
    {
        m_iReadingInQuene --;
        ReleaseSemaphore(m_hInVector, 1, NULL);
    }

    ReleaseMutex(m_hStateMutex);
    TRACE ("Set AT Response end: %s - %d\r\n", m_TestItemName, atr.resultLst[0].eleLst[0].str_len);

    return true;
}

void CTestItem_Interface::StopTestAfterAll ()
{
    TRACE ("[StopTestAfterAll] %s\r\n", m_TestItemName);
    WaitForSingleObject(m_hStateMutex, timeout_sec * 1000);
    while (m_iReadingInQuene > 0)
    {
        m_iReadingInQuene --;
        ReleaseSemaphore(m_hInVector, 1, NULL);
    };
    ReleaseMutex(m_hStateMutex);
}

bool CTestItem_Interface::PareStringByCRLF (string &src, vector<string> &desVec)
{
    char	tmpStr[1024];
    int		i = 0;
    int		j = 0;
    bool    bFindCRLF = false;
    const char *pSrc = src.c_str();
    memset (tmpStr, 0x0, sizeof(char)*1024);

    for (i = 0; i < src.length(); i++)
    {
        if(pSrc[i] == '\r')
        {
            i ++;
            if (i < src.length() && pSrc[i] == '\n')
            {
                if (j != 0)
                {
                    tmpStr[j] = '\0';
                    desVec.push_back(Trim(string(tmpStr)));

                    j = 0;
                }

                bFindCRLF = true;
            }
            else
            {
                return false; // Incorrect format
            }
        }
        else
        {
            tmpStr[j++] = pSrc[i];
        }
    }

    if (!bFindCRLF)
    {
        desVec.push_back(Trim(string(tmpStr)));
    }

    return true;
}

E_ATADLL_RESULT CTestItem_Interface::MeasureVoltage (double &vol)
{
    ATResult atret;
    string atstr = "";
    bool result = false;

    atstr = "Voltage=1\r\n";

    for (int i = 0; i < 3; i++)
    {
        test_handle->m_pm_comm_handle->SetNeedWithCRLR (true);
        result = test_handle->m_pm_comm_handle->WriteCommand(atstr.c_str(), atstr.length(), ATTIMEOUT, false, atret);
        test_handle->m_pm_comm_handle->SetNeedWithCRLR (false);

        if (result && strstr (atret.resultLst[0].eleLst[1].str_value.c_str(), "OK") != NULL)
        {
            break;
        }
        else
        {
            Sleep (100);
        }
    }

    if (i >= 3)
    {
        return E_ATADLL_RESULT_FAILED;
    }

    Sleep (1000);

    atstr = "Voltage?\r\n";

    for (i = 0; i < 5; i ++)
    {
        test_handle->m_pm_comm_handle->SetNeedWithCRLR (true);
        result = test_handle->m_pm_comm_handle->WriteCommand(atstr.c_str(), atstr.length(), ATTIMEOUT, false, atret);
        test_handle->m_pm_comm_handle->SetNeedWithCRLR (false);

        if (result &&
                strstr(atret.resultLst[0].eleLst[1].str_value.c_str(), ":error") == NULL &&
                strstr(atret.resultLst[0].eleLst[1].str_value.c_str(), ":0") == NULL)
        {
            vol = atof(atret.resultLst[0].eleLst[1].str_value.c_str());
            AddLog ("[OK] Power measure voltage sub : %s .", atret.resultLst[0].eleLst[1].str_value.c_str());
            vol = vol * test_handle->m_commonCFG.com_port_info.pm_info.slope + test_handle->m_commonCFG.com_port_info.pm_info.offset;
            return E_ATADLL_RESULT_SUCCESS;
        }
        else
        {
            AddLog ("Measure voltage invalid, retry: %d", i+1);
        }
        Sleep (200);
    }

    atstr = "End\r\n";
    test_handle->m_pm_comm_handle->SetNeedWithCRLR (true);
    result = test_handle->m_pm_comm_handle->WriteCommand(atstr.c_str(), atstr.length(), ATTIMEOUT, false, atret);
    test_handle->m_pm_comm_handle->SetNeedWithCRLR (false);
    Sleep (300);

    return E_ATADLL_RESULT_FAILED;
}

E_ATADLL_RESULT CTestItem_Interface::SwitchToCharger ()
{
    ATResult atret;
    string atstr = "Charge=1\r\n";

    test_handle->m_pm_comm_handle->SetNeedWithCRLR (true);
    bool result = test_handle->m_pm_comm_handle->WriteCommand(atstr.c_str(), atstr.length(), ATTIMEOUT, false, atret);
    test_handle->m_pm_comm_handle->SetNeedWithCRLR (false);

    if (result && (strstr(atret.resultLst[0].eleLst[1].str_value.c_str(), "ok") != NULL ||
                   strstr(atret.resultLst[0].eleLst[1].str_value.c_str(), "OK") != NULL))
    {
        Sleep (500);
        return E_ATADLL_RESULT_SUCCESS;
    }
    return E_ATADLL_RESULT_FAILED;
}


E_ATADLL_RESULT CTestItem_Interface::SendCommandPM (string cmd, string retStr)
{
    ATResult atret;
    string atstr = cmd;

    if ((long)(test_handle->m_pm_comm_handle) <= 0)
    {
    	AddLog("[TestBox]pm handle error");
        return E_ATADLL_RESULT_FAILED;
    }

    if (retStr == "")
    {
        test_handle->m_pm_comm_handle->WriteCommand(atstr.c_str(), atstr.length(), ATTIMEOUT, false, atret);
        return E_ATADLL_RESULT_SUCCESS;
    }

    for (int looptimes = 0; looptimes < 2; looptimes++)
    {
        test_handle->m_pm_comm_handle->SetNeedWithCRLR (true);
        test_handle->m_pm_comm_handle->WriteCommand(atstr.c_str(), atstr.length(), ATTIMEOUT, false, atret);
        test_handle->m_pm_comm_handle->SetNeedWithCRLR (false);
        if (atret.resultLst.empty())
        {
            continue;
        }
        else if (atret.resultLst[0].eleLst.empty())
        {
            continue;
        }

        AddLog ("[TestBox]PM returned string: %s", atret.resultLst[0].eleLst[1].str_value.c_str());

        // Switch to upper
        char refStr[1024];
        for (int i = 0; i < strlen(retStr.c_str()); i ++)
        {
            refStr[i] = toupper(*(retStr.c_str()+i));
        }
        refStr[i] = 0;

        char destStr[1024];
        for (i = 0; i < strlen(atret.resultLst[0].eleLst[1].str_value.c_str()); i ++)
        {
            destStr[i] = toupper(*(atret.resultLst[0].eleLst[1].str_value.c_str()+i));
        }
        destStr[i] = 0;

        if (strstr(destStr, refStr) != NULL)
        {
        	AddLog("[TestBox]Command: %s set success",cmd.c_str());
            return E_ATADLL_RESULT_SUCCESS;
        }

        Sleep (100);
    }
	AddLog("[TestBox]Command: %s set fail",cmd.c_str());
    return E_ATADLL_RESULT_FAILED;
}

E_ATADLL_RESULT CTestItem_Interface::ResetPM ()
{
    ATResult atret;
    string atstr = "End\r\n";


    if ((long)(test_handle->m_pm_comm_handle) <= 0)
    {
        return E_ATADLL_RESULT_FAILED;
    }

    for (int i = 0; i < 4; i++)
    {
        test_handle->m_pm_comm_handle->SetNeedWithCRLR (true);
        bool result = test_handle->m_pm_comm_handle->WriteCommand(atstr.c_str(), atstr.length(), ATTIMEOUT, false, atret);
        test_handle->m_pm_comm_handle->SetNeedWithCRLR (false);
        AddLog ("[TestBox]Reset %d.", i);
        if (result && strstr(atret.resultLst[0].eleLst[1].str_value.c_str(), "OK") != NULL)
        {
            AddLog ("[TestBox]Reset pm return ok.");
            return E_ATADLL_RESULT_SUCCESS;
        }

        Sleep (100);
    }

    return E_ATADLL_RESULT_FAILED;
}


bool CTestItem_Interface::SwitchToMode (int mode)
{
    return true;


    ClearATResPool ();

    char tmpStr[128];
    sprintf (tmpStr, "AT+CFUN=%d", mode);

    ATResult atret;
    string atstr = tmpStr;
    atstr += CR;
    test_handle->m_comm_handle->WriteCommand(atstr.c_str(), atstr.length(), ATTIMEOUT, false, atret);
    Sleep (200);

    int loop_times = 0;
    while (1)
    {
        Sleep (200);
        loop_times ++;

        ClearATResPool ();
        atstr = "AT+CFUN?";
        atstr += CR;
        test_handle->m_comm_handle->WriteCommand(atstr.c_str(), atstr.length(), ATTIMEOUT, false, atret);
        if (!GetATResponse(atret))
        {
            if (mode == 1)
            {
                AddLog ("[Error] Get AT response timeout when switching to normal mode");
            }
            else
            {
                AddLog ("[Error] Get AT response timeout when switching to flight mode");
            }
            return false;
        }

        char    mode_str[128];
        int     m_iWholeCmdLen  = atret.resultLst[0].eleLst[0].str_value.length();
        memset (mode_str, 0x0, sizeof(char)*128);
        memcpy (mode_str, atret.resultLst[0].eleLst[0].str_value.c_str(), m_iWholeCmdLen);
        mode_str[m_iWholeCmdLen] = 0;

        char sub_str[32];
        sprintf (sub_str, "%d", mode);
        if (NULL != strstr(mode_str, sub_str))
        {
            if (mode == 1)
            {
                AddLog ("[OK] Switch to normal mode ok after %d times retry", loop_times);
            }
            else
            {
                AddLog ("[OK] Switch to flight mode ok after %d times retry", loop_times);
            }
            return true;
        }

        if (loop_times > 30)
        {
            if (mode == 1)
            {
                AddLog ("[Fail] Switch to normal mode fail after %d times retry", loop_times);
            }
            else
            {
                AddLog ("[Fail] Switch to flight mode fail after %d times retry", loop_times);
            }
            return false;
        }
    }

    return false;

}

void CTestItem_Interface::AddLog (char* fmt, ...)
{
    va_list ap;
    va_start (ap, fmt);

    char time_str[512];
    SYSTEMTIME	cur_time;
    GetLocalTime(&cur_time);
    sprintf (time_str, "%04d/%02d/%02d %02d:%02d:%02d.%03d ",
             cur_time.wYear, cur_time.wMonth, cur_time.wDay,
             cur_time.wHour, cur_time.wMinute, cur_time.wSecond, cur_time.wMilliseconds);

    char tempStr[1024];
    char logStr[1024];
    vsprintf (tempStr, fmt, ap);
    sprintf (logStr, "%s%s\r\n", time_str, tempStr);
    //m_logStr = m_logStr + time_str + tempStr + "\r\n";
    va_end (ap);

    ATA_LOG (test_handle, logStr);
}

void CTestItem_Interface::GetATAData (sp_ata_data &ata_data, ATResult atret)
{
    int len = atret.resultLst[0].eleLst[0].str_len;
    AddLog ("GetATAData data size: %d.", len);
    AddLog ("GetATAData sp_ata_data size: %d.", sizeof(sp_ata_data));
    AddLog ("GetATAData sp_ata_data_old size: %d.", sizeof(sp_ata_data_old));
	AddLog ("GetATAData sp_ata_data_old_ex size: %d.", sizeof(sp_ata_data_old_ex));

    if (sizeof(sp_ata_data) == len)
    {
        memcpy (&ata_data, atret.resultLst[0].eleLst[0].str_char, len);
    }
    else if (sizeof(sp_ata_data_old) == len)
    {
        sp_ata_data_old ata_data_old;
        memcpy (&ata_data_old, atret.resultLst[0].eleLst[0].str_char, len);

        memcpy (&ata_data.fm, &ata_data_old.fm, sizeof(ftm_ata_fm));
        memcpy (&ata_data.wifi, &ata_data_old.wifi, sizeof(ftm_ata_wifi));
        memcpy (&ata_data.bt, &ata_data_old.bt, sizeof(ftm_ata_bt_num));
        memcpy (&ata_data.version, &ata_data_old.version, sizeof(ftm_ata_version));
        memcpy (&ata_data.gps, &ata_data_old.gps, sizeof(ftm_ata_gps));
        memcpy (&ata_data.speaker, &ata_data_old.speaker, sizeof(ftm_ata_speaker));
        memcpy (&ata_data.receiver, &ata_data_old.receiver, sizeof(ftm_ata_receiver));
        memcpy (&ata_data.headset, &ata_data_old.headset, sizeof(ftm_ata_headset));
        memcpy (&ata_data.battery, &ata_data_old.battery, sizeof(ftm_ata_battery));
        memcpy (&ata_data.gsensor, &ata_data_old.gsensor, sizeof(ftm_ata_gsensor));
        memcpy (&ata_data.msensor, &ata_data_old.msensor, sizeof(ftm_ata_msensor));
        memcpy (&ata_data.alsps, &ata_data_old.alsps, sizeof(ftm_ata_alsps));
        memcpy (&ata_data.gyroscope, &ata_data_old.gyroscope, sizeof(ftm_ata_gyroscope));
    }
    else if (sizeof(sp_ata_data_old_ex) == len)
    {
        sp_ata_data_old_ex ata_data_old;
        memcpy (&ata_data_old, atret.resultLst[0].eleLst[0].str_char, len);

        memcpy (&ata_data.fm, &ata_data_old.fm, sizeof(ftm_ata_fm));
        memcpy (&ata_data.wifi, &ata_data_old.wifi, sizeof(ftm_ata_wifi));
        memcpy (&ata_data.bt, &ata_data_old.bt, sizeof(ftm_ata_bt_num));
        memcpy (&ata_data.version, &ata_data_old.version, sizeof(ftm_ata_version));
        memcpy (&ata_data.gps, &ata_data_old.gps, sizeof(ftm_ata_gps));
        memcpy (&ata_data.speaker, &ata_data_old.speaker, sizeof(ftm_ata_speaker));
        memcpy (&ata_data.receiver, &ata_data_old.receiver, sizeof(ftm_ata_receiver));
        memcpy (&ata_data.headset, &ata_data_old.headset, sizeof(ftm_ata_headset));
        memcpy (&ata_data.battery, &ata_data_old.battery, sizeof(ftm_ata_battery));
        memcpy (&ata_data.gsensor, &ata_data_old.gsensor, sizeof(ftm_ata_gsensor));
        memcpy (&ata_data.msensor, &ata_data_old.msensor, sizeof(ftm_ata_msensor));
        memcpy (&ata_data.alsps, &ata_data_old.alsps, sizeof(ftm_ata_alsps));
        memcpy (&ata_data.gyroscope, &ata_data_old.gyroscope, sizeof(ftm_ata_gyroscope));

        memcpy (&ata_data.headsetL, &ata_data_old.headsetL, sizeof(ftm_ata_headset));
        memcpy (&ata_data.headsetR, &ata_data_old.headsetR, sizeof(ftm_ata_headset));
        memcpy (&ata_data.vibrator, &ata_data_old.vibrator, sizeof(ftm_ata_vibrator));
    }
    else
    {
        memcpy (&ata_data, atret.resultLst[0].eleLst[0].str_char, sizeof(sp_ata_data)<len?sizeof(sp_ata_data):len);
    }
}

E_ATADLL_RESULT CTestItem_Interface::RequestData (sp_ata_data &ata_data)
{
    ClearATResPool ();

    ATResult atret;
    string atstr = "AT+REQUESTDATA";
    atstr += CR;

    test_handle->m_comm_handle->WriteCommand(atstr.c_str(), atstr.length(), ATTIMEOUT, false, atret);
    if (!GetATResponse(atret))
    {
        AddLog ("[Requestdata ERROR] Request result data timeout.");
        return E_ATADLL_RESULT_AT_RESPONSE_TIMEOUT;
    }

    GetATAData (ata_data, atret);
    return E_ATADLL_RESULT_SUCCESS;
}

bool CTestItem_Interface::IsAutoMode ()
{
    int loopIndex = 0;
	for (loopIndex = 0; loopIndex < test_handle->testcases_index_bg + test_handle->testcases_index_fg; loopIndex ++)
    {
        if (loopIndex < test_handle->testcases_index_bg)
        {
			if (test_handle->testcases_bg[loopIndex].item == test_item)
	        {
	            return test_handle->testcases_bg[loopIndex].b_auto;
	        }
        }
        else
        {
			if (test_handle->testcases_fg[loopIndex - test_handle->testcases_index_bg].item == test_item)
	        {
	            return test_handle->testcases_fg[loopIndex - test_handle->testcases_index_bg].b_auto;
	        }
		}
    }

    return true;
}

bool CTestItem_Interface::IsBackground ()
{
	int loopIndex = 0;
	for (loopIndex = 0; loopIndex < test_handle->testcases_index_bg; loopIndex ++)
    {
		if (test_handle->testcases_bg[loopIndex].item == test_item)
		{
			return true;
		}
    }
	return false;
}

E_ATADLL_RESULT CTestItem_Interface::RunTest ()
{
    HANDLE	m_hThread;
    DWORD	m_ThreadId;

    m_hThread = (HANDLE)_beginthreadex(
                    NULL,
                    0,
                    (TESTITEMINTERFACE_THREADFUNC)CTestItem_Interface::HelpThreadFunc,
                    (LPVOID)this,
                    0,
                    (unsigned *) &m_ThreadId
                );

    if(m_hThread)
        CloseHandle (m_hThread);

    return E_ATADLL_RESULT_SUCCESS;;
}

E_ATADLL_RESULT CTestItem_Interface::HelpThreadFunc (LPVOID param)
{
    CTestItem_Interface *tic = (CTestItem_Interface *)param;
    return tic->ThreadFunc();
}

E_ATADLL_RESULT CTestItem_Interface::ThreadFunc()
{
    clock_t start_clock = clock();
	glb_at_resp_flag = false;

    AddLog ("====== [%s] Start111111111111111111111111 Test ======", m_TestItemName);
    E_ATADLL_RESULT ata_result = StartTest ();

    clock_t end_clock = clock();
    double time_cost_mill = difftime (end_clock, start_clock);
    test_handle->m_testCNF->time_cost[glb_test_item[test_handle->handle_index]] = time_cost_mill/1000.0;
    test_handle->m_testCNF->test_result[test_item] = ata_result;
	if(ata_result!=E_ATADLL_RESULT_SUCCESS)
		{
			error_code=ata_result;
			AddLog("Test Error:%s,Error code is %d\r\n",AtaStatusToString(error_code),error_code);
			
		}
    AddLog ("Time cost: %.3f second", test_handle->m_testCNF->time_cost[glb_test_item[test_handle->handle_index]]);

    AddLog ("====== [%s] End Test ======\r\n", m_TestItemName);

    if (cbTestPostprocess != NULL)
    {
            cbTestPostprocess (test_handle->handle_index, test_item);
    }

    return ata_result;
}


