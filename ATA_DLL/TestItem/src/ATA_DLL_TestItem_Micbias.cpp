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
 *   ATA_DLL_TestItem_Micbias.cpp
 *
 * Project:
 * --------
 *   ATA_DLL
 *
 * Description:
 * ------------
 *
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
#include "..\header\ATA_DLL_TestItem_Micbias.h"

CTestItem_Micbias::CTestItem_Micbias(ATA_DLL_Handle *current_handle, int timeout, E_ATDLL_TEST_ITEM_COUNT item, char *testName)
    : CTestItem_Interface(current_handle, timeout, item, testName)
{
}

CTestItem_Micbias::~CTestItem_Micbias()
{
}

E_ATADLL_RESULT CTestItem_Micbias::StartTest ()
{
    E_ATADLL_RESULT ata_result = E_ATADLL_RESULT_SUCCESS;
    ata_result = TestMicbias ();

    return ata_result;
}

E_ATADLL_RESULT CTestItem_Micbias::TestMicbias ()
{
    ClearATResPool();

    test_handle->m_commonCFG.cbTestProgess (test_handle->handle_index, E_MICBIAS, 10);
    if (E_ATADLL_RESULT_SUCCESS != OpenMicbias())
    {
        AddLog ("[Micbias ERROR] Open Micbias fail.");
        goto Error;
    }
    AddLog ("[OK] Open Micbias OK.");
    Sleep (200);

    test_handle->m_commonCFG.cbTestProgess (test_handle->handle_index, E_MICBIAS, 30);

#ifndef INTERNEL_DEBUG_VERSION
    // Test Micbias0
    if (E_ATADLL_RESULT_SUCCESS == SwitchMicbiasTo(0))
    {
        AddLog ("[TestBox OK] Switch to Micbias0 success.");
        Sleep (500);
        if (E_ATADLL_RESULT_SUCCESS == MeasureMicbiasVoltage (test_handle->m_testCNF->micbias_cnf.d_vMicbias0))
        {
            AddLog ("[TestBox OK] Micbias0 voltage = %.3f", test_handle->m_testCNF->micbias_cnf.d_vMicbias0);
        }
        else
        {
            AddLog ("[TestBox ERROR] Micbias0 voltage measure failed.");
            test_handle->m_commonCFG.cbTestProgess (test_handle->handle_index, E_MICBIAS, 100);
    		return E_ATADLL_RESULT_BOX_FAILED;
        }
    }
    else
    {
        AddLog ("[TestBox ERROR] Switch to Micbias0 fail.");
        test_handle->m_commonCFG.cbTestProgess (test_handle->handle_index, E_MICBIAS, 100);
    	return E_ATADLL_RESULT_BOX_FAILED;
    }
	
    test_handle->m_commonCFG.cbTestProgess (test_handle->handle_index, E_MICBIAS, 50);

    // Test Micbias1
    if (E_ATADLL_RESULT_SUCCESS == SwitchMicbiasTo(1))
    {
        AddLog ("[TestBox OK] Switch to Micbias1 success.");
        Sleep (500);
        if (E_ATADLL_RESULT_SUCCESS == MeasureMicbiasVoltage (test_handle->m_testCNF->micbias_cnf.d_vMicbias1))
        {
            AddLog ("[TestBox OK] Micbias1 voltage = %.3f", test_handle->m_testCNF->micbias_cnf.d_vMicbias1);
        }
        else
        {
            AddLog ("[TestBox ERROR] Micbias1 voltage measure failed.");
            SwitchMicbiasBack (1);
            test_handle->m_commonCFG.cbTestProgess (test_handle->handle_index, E_MICBIAS, 100);
    		return E_ATADLL_RESULT_BOX_FAILED;
        }
        SwitchMicbiasBack (1);
    }
    else
    {
        AddLog ("[TestBox ERROR] Switch to Micbias1 fail.");
        test_handle->m_commonCFG.cbTestProgess (test_handle->handle_index, E_MICBIAS, 100);
    	return E_ATADLL_RESULT_BOX_FAILED;
    }
#endif

    test_handle->m_commonCFG.cbTestProgess (test_handle->handle_index, E_MICBIAS, 80);

    if (E_ATADLL_RESULT_SUCCESS != CloseMicbias())
    {
        AddLog ("[Micbias ERROR] Close Micbias fail.");
        goto Error;
    }
    AddLog ("[OK] Close Micbias OK.");

    test_handle->m_commonCFG.cbTestProgess (test_handle->handle_index, E_MICBIAS, 100);
    return E_ATADLL_RESULT_SUCCESS;

Error:

    test_handle->m_commonCFG.cbTestProgess (test_handle->handle_index, E_MICBIAS, 100);
    return E_ATADLL_RESULT_FAILED;
}

E_ATADLL_RESULT CTestItem_Micbias::OpenMicbias ()
{
    ClearATResPool();

    ATResult atret;
    string atstr = "AT+MICBIAS";
    atstr += CR;

    test_handle->m_comm_handle->WriteCommand(atstr.c_str(), atstr.length(), 1000, false, atret);
    return E_ATADLL_RESULT_SUCCESS;
}

E_ATADLL_RESULT CTestItem_Micbias::CloseMicbias ()
{
    ClearATResPool();

    ATResult atret;
    string atstr = "AT+MICBIAS=CLOSE";

#ifdef __ATA20__
	atstr = "AT+MICBIAS=STOP";
#endif

	atstr += CR;

    test_handle->m_comm_handle->WriteCommand(atstr.c_str(), atstr.length(), ATTIMEOUT, false, atret);
    if (!GetATResponse(atret))
    {
        AddLog ("[Micbias ERROR] Close Micbias fail due to response timeout.");
        return E_ATADLL_RESULT_AT_RESPONSE_TIMEOUT;
    }
	else if(NULL != strstr(atret.resultLst[0].eleLst[0].str_value.c_str(), "Cannot find"))
	{
		AddLog ("[Micbias ERROR] Micbias test Not support in target.");
		return E_ATADLL_RESULT_NOT_SUPPORT;
	}

    return E_ATADLL_RESULT_SUCCESS;
}



// number : 0 or 1
E_ATADLL_RESULT CTestItem_Micbias::SwitchMicbiasTo(int number)
{
    ATResult atret;
    string atstr;

    if (number == 0)   // Main Mic
    {
        if (E_ATADLL_RESULT_SUCCESS != SendCommandPM ("Voltage=1\r\n", ""))
        {

        }
        Sleep (500);

        if (E_ATADLL_RESULT_SUCCESS == SendCommandPM ("MICSel=0\r\n", "OK"))
        {
            AddLog ("[TestBox OK] MICSel=0");
        }
        else
        {
            AddLog ("[TestBox ERROR] set MICSel=0 fail");
            goto Error;
        }
        /**
        if (E_ATADLL_RESULT_SUCCESS == SendCommandPM ("MICSel=1\r\n", "OK")) {
        	AddLog ("[OK] MICSel=1");
        } else {
        	AddLog ("[Fail] MICSel=1");
        	goto Error;
        }
        **/
        Sleep (500);
        if (E_ATADLL_RESULT_SUCCESS == SendCommandPM ("MICBias=0\r\n", "OK"))
        {
            AddLog ("[TestBox OK] MICBias=0");
        }
        else
        {
            AddLog ("[TestBox ERROR] set MICBias=0 fail");
            goto Error;
        }
    }
    else if (number == 1)
    {
		if (E_ATADLL_RESULT_SUCCESS != SendCommandPM ("Voltage=1\r\n", ""))
        {

        }
        Sleep (500);
		
        if (E_ATADLL_RESULT_SUCCESS == SendCommandPM ("Headset=0\r\n", "OK"))
        {
            AddLog ("[TestBox OK] Headset=0");
        }
        else
        {
            AddLog ("[TestBox ERROR] set Headset=0 fail");
            goto Error;
        }

		if (E_ATADLL_RESULT_SUCCESS == SendCommandPM ("MICSel=1\r\n", "OK"))
        {
            AddLog ("[TestBox OK] MICSel=1");
        }
        else
        {
            AddLog ("[TestBox ERROR] set MICSel=1 fail");
            goto Error;
        }

        Sleep (500);
        if (E_ATADLL_RESULT_SUCCESS == SendCommandPM ("MICBias=1\r\n", "OK"))
        {
            AddLog ("[TestBox OK] MICBias=1");
        }
        else
        {
            AddLog ("[TestBox ERROR] set MICBias=1 fail");
            goto Error;
        }
    }

    /**
    char cmdStr[64];

    sprintf (cmdStr, "Voltage=1\r\n");
    atstr = cmdStr;
    test_handle->m_pm_comm_handle->WriteCommand(atstr.c_str(), atstr.length(), 3000, false, atret);

    sprintf (cmdStr, "MICSel=%d\r\n", number);
    atstr = cmdStr;

    char responseStr[64];
    sprintf (responseStr, "OK", number);

    bool result = false;

    for (int i = 0; i < 2; i ++)
    {
    	test_handle->m_pm_comm_handle->SetNeedWithCRLR (true);
    	bool result = test_handle->m_pm_comm_handle->WriteCommand(atstr.c_str(), atstr.length(), 3000, false, atret);
    	test_handle->m_pm_comm_handle->SetNeedWithCRLR (false);

    	if (!result)
    		continue;

    	if (strstr(atret.resultLst[0].eleLst[1].str_value.c_str(), responseStr) != NULL)
    	{
    		AddLog ("%s:%s", atret.resultLst[0].eleLst[0].str_value.c_str(), atret.resultLst[0].eleLst[1].str_value.c_str());
    	    break;
    	}
    	else
    	{
    		//AddLog ("%s:%s", atret.resultLst[0].eleLst[0].str_value.c_str(), atret.resultLst[0].eleLst[1].str_value.c_str());
    	}

    	Sleep (300);
    }

    sprintf (cmdStr, "MICBias=%d\r\n", number);
    atstr = cmdStr;

    for (i = 0; i < 2; i ++)
    {
    	test_handle->m_pm_comm_handle->SetNeedWithCRLR (true);
    	bool result = test_handle->m_pm_comm_handle->WriteCommand(atstr.c_str(), atstr.length(), 3000, false, atret);
    	test_handle->m_pm_comm_handle->SetNeedWithCRLR (false);

    	if (!result)
    		continue;

    	if (strstr(atret.resultLst[0].eleLst[1].str_value.c_str(), responseStr) != NULL)
    	{
    		AddLog ("%s:%s", atret.resultLst[0].eleLst[0].str_value.c_str(), atret.resultLst[0].eleLst[1].str_value.c_str());
    		return E_ATADLL_RESULT_SUCCESS;
    	}
    	else
    	{
    		//AddLog ("%s:%s", atret.resultLst[0].eleLst[0].str_value.c_str(), atret.resultLst[0].eleLst[1].str_value.c_str());
    	}

    	Sleep (300);
    }
    **/

    return E_ATADLL_RESULT_SUCCESS;

Error:
    return E_ATADLL_RESULT_BOX_FAILED;
}

E_ATADLL_RESULT CTestItem_Micbias::SwitchMicbiasBack(int number)
{
    if (number == 1)
    {
        if (E_ATADLL_RESULT_SUCCESS == SendCommandPM("Headset=1\r\n", "OK"))
        {
            AddLog ("[TestBox OK] Headset=1");
        }
        else
        {
            AddLog ("[TestBox ERROR] set Headset=1 fail");
            goto Error;
        }
    }

    return E_ATADLL_RESULT_SUCCESS;

Error:
    return E_ATADLL_RESULT_BOX_FAILED;
}


E_ATADLL_RESULT CTestItem_Micbias::MeasureMicbiasVoltage (double &vol)
{
    ATResult atret;
    string atstr = "";
    bool result = false;

    atstr = "Voltage?\r\n";
    for (int i = 0; i < 5; i ++)
    {
        test_handle->m_pm_comm_handle->SetNeedWithCRLR (true);
        result = test_handle->m_pm_comm_handle->WriteCommand(atstr.c_str(), atstr.length(), ATTIMEOUT, false, atret);
        test_handle->m_pm_comm_handle->SetNeedWithCRLR (false);

        if (result &&
                strstr(atret.resultLst[0].eleLst[1].str_value.c_str(), ":error") == NULL &&
                strstr(atret.resultLst[0].eleLst[1].str_value.c_str(), ":0") == NULL)
        {
            vol = atof(atret.resultLst[0].eleLst[1].str_value.c_str());
            AddLog ("[TestBox OK] Power measure voltage sub : %s .", atret.resultLst[0].eleLst[1].str_value.c_str());
            vol = vol * test_handle->m_commonCFG.com_port_info.pm_info.slope + test_handle->m_commonCFG.com_port_info.pm_info.offset;
            return E_ATADLL_RESULT_SUCCESS;
        }
        else
        {
            AddLog ("[TestBox ERROR]Measure voltage invalid, retry: %d", i+1);
        }
        Sleep (200);
    }

    atstr = "End\r\n";
    test_handle->m_pm_comm_handle->SetNeedWithCRLR (true);
    result = test_handle->m_pm_comm_handle->WriteCommand(atstr.c_str(), atstr.length(), ATTIMEOUT, false, atret);
    test_handle->m_pm_comm_handle->SetNeedWithCRLR (false);
    Sleep (300);

    return E_ATADLL_RESULT_BOX_FAILED;
}


