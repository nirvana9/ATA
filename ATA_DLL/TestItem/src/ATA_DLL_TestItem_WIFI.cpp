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
 *   ATA_DLL_TestItem_WIFI.cpp
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
#include "..\header\ATA_DLL_TestItem_WIFI.h"

CTestItem_WIFI::CTestItem_WIFI(ATA_DLL_Handle *current_handle, int timeout, E_ATDLL_TEST_ITEM_COUNT item, char *testName)
    : CTestItem_Interface(current_handle, timeout, item, testName)
{
}

CTestItem_WIFI::~CTestItem_WIFI()
{
}

E_ATADLL_RESULT CTestItem_WIFI::StartTest ()
{
    E_ATADLL_RESULT ata_result = E_ATADLL_RESULT_SUCCESS;
    ata_result = TestWIFI ();

    return ata_result;
}

E_ATADLL_RESULT CTestItem_WIFI::TestWIFI ()
{
	E_ATADLL_RESULT testResultCode;

    ClearATResPool();
	E_ATADLL_RESULT ata_result = E_ATADLL_RESULT_SUCCESS;

    ATResult atret;
    string atstr = "";

#ifdef __ATA20__
    if (IsAutoMode())
    {
		if (IsBackground())
		{
        	atstr = "AT+WIFI=1,3";
		}
		else
		{
			atstr = "AT+WIFI=1,1";
		}
    }
    else
    {
        atstr = "AT+WIFI=1,2";
    }
#else
    atstr = "AT+WIFI";
#endif
    atstr += CR;

    test_handle->m_commonCFG.cbTestProgess (test_handle->handle_index, E_WIFI, 10);
    test_handle->m_comm_handle->WriteCommand(atstr.c_str(), atstr.length(), ATTIMEOUT, false, atret);
    if (!GetATResponse(atret))
    {
        AddLog ("[WIFI ERROR] %s response timeout.",atstr.c_str());
		 test_handle->m_testCNF->wifi_cnf.bPass = false;
        return E_ATADLL_RESULT_AT_RESPONSE_TIMEOUT;
    }
    if (NULL != strstr(atret.resultLst[0].eleLst[0].str_value.c_str(), "pass"))
    {
        AddLog ("[OK] WIFI test OK from target.");
        test_handle->m_testCNF->wifi_cnf.bPass = true;
		test_handle->m_commonCFG.cbTestProgess (test_handle->handle_index, E_WIFI, 60);
		testResultCode=E_ATADLL_RESULT_SUCCESS;
    }
    else if(NULL != strstr(atret.resultLst[0].eleLst[0].str_value.c_str(), "fail"))
    {
        AddLog ("[WIFI ERROR] WIFI test fail from target.");
        test_handle->m_testCNF->wifi_cnf.bPass = false;
		
		testResultCode= E_ATADLL_RESULT_FAILED;
		goto Error;
    }
	else if(NULL != strstr(atret.resultLst[0].eleLst[0].str_value.c_str(), "not test"))
	{
		AddLog ("[WIFI ERROR] WIFI not test from target.");
        test_handle->m_testCNF->wifi_cnf.bPass = false;
		testResultCode=E_ATADLL_RESULT_NOT_TEST;
		goto Error;
	}
	else if(NULL != strstr(atret.resultLst[0].eleLst[0].str_value.c_str(), "Cannot find"))
	{
		AddLog ("[WIFI ERROR] WIFI not support in target.");
        test_handle->m_testCNF->wifi_cnf.bPass = false;
		testResultCode=E_ATADLL_RESULT_NOT_SUPPORT;
		goto Error;
	
	}
	else
	{
		AddLog ("[WIFI ERROR] RTC failed,error code not defined.");
		test_handle->m_testCNF->wifi_cnf.bPass = false;
		testResultCode=E_ATADLL_RESULT_NULL;
		goto Error;
	}
    

#ifndef __ATA20__
    atstr = "AT+REQUESTDATA";
    atstr += CR;
    test_handle->m_comm_handle->WriteCommand(atstr.c_str(), atstr.length(), ATTIMEOUT, false, atret);
    if (!GetATResponse(atret))
    {
        AddLog ("[Error] AT response timeout.");
        return E_ATADLL_RESULT_AT_RESPONSE_TIMEOUT;
    }
    test_handle->m_commonCFG.cbTestProgess (test_handle->handle_index, E_WIFI, 70);

    sp_ata_data s_data;
    GetATAData (s_data, atret);
    AddLog ("WIFI MAC: %s, Name: %s, RSSI:%d",
            s_data.wifi.wifi_mac, s_data.wifi.wifi_name, s_data.wifi.wifi_rssi);
    memcpy (&(test_handle->m_testCNF->wifi_cnf.ftm_wifi), &(s_data.wifi), sizeof(ftm_ata_wifi));
	test_handle->m_commonCFG.cbTestProgess (test_handle->handle_index, E_WIFI, 100);
    return E_ATADLL_RESULT_SUCCESS;
#endif

	if (test_handle->m_commonCFG.wifi_cfg.bWIFIMacCheck)
	{
		AddLog ("Start to read WIFI mac address.");
		ata_result = ReadWIFIMac (test_handle->m_testCNF->wifi_cnf.main_mac, test_handle->m_testCNF->wifi_cnf.main_mac_len);
		if (E_ATADLL_RESULT_SUCCESS == ata_result)
		{
			AddLog ("Read WIFI mac address OK: %s", test_handle->m_testCNF->wifi_cnf.main_mac);
		}
		else
		{
			AddLog ("Read WIFI mac address fail.");
			test_handle->m_commonCFG.cbTestProgess (test_handle->handle_index, E_WIFI, 100);
			return ata_result;
		}
		AddLog ("Read WIFI mac address finished.");
	}

    test_handle->m_commonCFG.cbTestProgess (test_handle->handle_index, E_WIFI, 100);
    return E_ATADLL_RESULT_SUCCESS;

	Error:
		test_handle->m_commonCFG.cbTestProgess (test_handle->handle_index, E_WIFI, 100);
		return testResultCode;
}

E_ATADLL_RESULT CTestItem_WIFI::ReadWIFIMac (char *wifiMacStr, unsigned int &len)
{
	ClearATResPool ();

	ATResult atret;
	string atstr = "AT+READWIFIMAC";
    atstr += CR;

    test_handle->m_comm_handle->WriteCommand(atstr.c_str(), atstr.length(), ATTIMEOUT, false, atret);
    if (!GetATResponse(atret))
    {
        AddLog ("[WIFI ERROR] %s response timeout.",atstr.c_str());
        return E_ATADLL_RESULT_AT_RESPONSE_TIMEOUT;
    }

	char    wifiMac_str[1024];
    int     m_iWholeCmdLen = atret.resultLst[0].eleLst[0].str_value.length();
    memset (wifiMac_str, 0x0, sizeof(char)*1024);
    memcpy (wifiMac_str, atret.resultLst[0].eleLst[0].str_value.c_str(), m_iWholeCmdLen);
    wifiMac_str[m_iWholeCmdLen] = '\0';

    char *pFlag = strstr (wifiMac_str, ":");
    if (pFlag == NULL)
    {
        memcpy (wifiMacStr, wifiMac_str, m_iWholeCmdLen);
        wifiMacStr[m_iWholeCmdLen] = 0;
		len = m_iWholeCmdLen;
    }
    else
    {
        memcpy (wifiMacStr, pFlag+1, m_iWholeCmdLen-(pFlag-wifiMac_str)-1);
        wifiMacStr[m_iWholeCmdLen-(pFlag-wifiMac_str)-1] = 0;
		len = m_iWholeCmdLen-(pFlag-wifiMac_str)-1;
    }
	if(NULL!= strstr(wifiMacStr,"fail"))
		{
			 AddLog ("[WIFI ERROR] Get the response from target is : fail.");
			return E_ATADLL_RESULT_FAILED;
		}

	return E_ATADLL_RESULT_SUCCESS;
}

