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
#include "..\header\ATA_DLL_TestItem_ADC.h"
#include "..\..\..\PowerDLL\PowerFunc.h"

CTestItem_ADC::CTestItem_ADC(ATA_DLL_Handle *current_handle, int timeout, E_ATDLL_TEST_ITEM_COUNT item, char *testName)
    : CTestItem_Interface(current_handle, timeout, item, testName)
{
    psu_handle = 0;
}

CTestItem_ADC::~CTestItem_ADC()
{
}

E_ATADLL_RESULT CTestItem_ADC::StartTest ()
{
    return Test();
}

E_ATADLL_RESULT CTestItem_ADC::Test()
{
	E_ATADLL_RESULT testResultCode;
    ClearATResPool();
    test_handle->m_commonCFG.cbTestProgess (test_handle->handle_index, E_ADC, 10);

    ATResult atret;
    string atstr = "";
#ifdef __ATA20__
    if (IsAutoMode())
    {
        atstr = "AT+CHARGER=1,1";
    }
    else
    {
        atstr = "AT+CHARGER=1,2";
    }
#else
    atstr = "AT+CHARGER";
#endif
    atstr += CR;

    E_ATADLL_RESULT ata_result = E_ATADLL_RESULT_SUCCESS;

#ifndef INTERNEL_DEBUG_VERSION
    ata_result = MeasureVoltage (test_handle->m_testCNF->adc_cnf.psu_v1);
    if (ata_result != E_ATADLL_RESULT_SUCCESS)
    {
        AddLog ("[TestBox ERROR] Power Measure measures voltage fail.");
        goto Error;
    }
    AddLog ("[OK] Switch sample to voltage is : %.3f V.", test_handle->m_testCNF->adc_cnf.psu_v1);
    test_handle->m_commonCFG.cbTestProgess (test_handle->handle_index, E_ADC, 40);
#endif

    test_handle->m_comm_handle->WriteCommand(atstr.c_str(), atstr.length(), ATTIMEOUT, false, atret);
    if (!GetATResponse(atret))
    {
        AddLog ("[ADC ERROR] %s response timeout.",atstr.c_str());
        return E_ATADLL_RESULT_AT_RESPONSE_TIMEOUT;
    }
    if (NULL != strstr(atret.resultLst[0].eleLst[0].str_value.c_str(), "pass"))
    {
        test_handle->m_testCNF->bChrgerDetected = true;
        AddLog ("[OK]Target ADC test ok.");
		testResultCode=E_ATADLL_RESULT_SUCCESS;
    }
    else if(NULL != strstr(atret.resultLst[0].eleLst[0].str_value.c_str(), "fail"))
    {
        test_handle->m_testCNF->bChrgerDetected = false;
        AddLog ("[ADC ERROR]Target ADC test fail.");
		testResultCode=E_ATADLL_RESULT_FAILED;
    }
	
	else if(NULL != strstr(atret.resultLst[0].eleLst[0].str_value.c_str(), "not test"))
	{
		AddLog ("[ADC ERROR] ADC not test in target.");
		test_handle->m_testCNF->bChrgerDetected = false;
		testResultCode=E_ATADLL_RESULT_NOT_TEST;
	}
	else if(NULL != strstr(atret.resultLst[0].eleLst[0].str_value.c_str(), "Cannot find"))
	{
		AddLog ("[ADC ERROR] ADC not support in target.");
		test_handle->m_testCNF->bChrgerDetected = false;
		testResultCode=E_ATADLL_RESULT_NOT_SUPPORT;
	}
	else
    {
        AddLog ("[ADC ERROR] ADC test failed,error code not defined.");
        test_handle->m_testCNF->bChrgerDetected = false;
		testResultCode=E_ATADLL_RESULT_NULL;
    }
    test_handle->m_commonCFG.cbTestProgess (test_handle->handle_index, E_ADC, 70);


#ifndef __ATA20__
    atstr = "AT+REQUESTDATA";
    atstr += CR;
    test_handle->m_comm_handle->WriteCommand(atstr.c_str(), atstr.length(), ATTIMEOUT, false, atret);
    if (!GetATResponse(atret))
    {
        AddLog ("[Error] AT response timeout.");
        return E_ATADLL_RESULT_AT_RESPONSE_TIMEOUT;
    }
    test_handle->m_commonCFG.cbTestProgess (test_handle->handle_index, E_ADC, 90);

    sp_ata_data s_data;
    GetATAData (s_data, atret);

    if (s_data.battery.current < 3000.0)
    {
        test_handle->m_testCNF->adc_cnf.meas_v1	= ((double)s_data.battery.voltage)/1000.0;
    }
    else
    {
        test_handle->m_testCNF->adc_cnf.meas_v1	= ((double)s_data.battery.current)/1000.0;
    }
    AddLog ("Target battery voltage is: %.3f mV", test_handle->m_testCNF->adc_cnf.meas_v1*1000);
	testResultCode=E_ATADLL_RESULT_SUCCESS;
#endif

    test_handle->m_commonCFG.cbTestProgess (test_handle->handle_index, E_ADC, 100);
    return testResultCode;

Error:
    return ata_result;

}

E_ATADLL_RESULT CTestItem_ADC::MeasureVoltage (double &vol)
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
    	AddLog("[TestBox ERROR] measure voltage fail.");
        return E_ATADLL_RESULT_BOX_FAILED;
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
            AddLog ("[ADC OK] Power measure Samples : %s .", atret.resultLst[0].eleLst[1].str_value.c_str());
            vol = vol * test_handle->m_commonCFG.com_port_info.pm_info.slope + test_handle->m_commonCFG.com_port_info.pm_info.offset;
            return E_ATADLL_RESULT_SUCCESS;
        }
        else
        {
            AddLog ("[TestBox warning]Measure voltage invalid, retry: %d", i+1);
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

E_ATADLL_RESULT CTestItem_ADC::SwitchToCharger ()
{
    ATResult atret;
    string atstr = "Charge=1\r\n";

    test_handle->m_pm_comm_handle->SetNeedWithCRLR (true);
    test_handle->m_pm_comm_handle->WriteCommand(atstr.c_str(), atstr.length(), ATTIMEOUT, false, atret);
    test_handle->m_pm_comm_handle->SetNeedWithCRLR (false);
    Sleep (500);

    return E_ATADLL_RESULT_FAILED;
}


E_ATADLL_RESULT CTestItem_ADC::ResetPM ()
{
    ATResult atret;
    string atstr = "End\r\n";

    for (int i = 0; i < 5; i++)
    {
        test_handle->m_pm_comm_handle->SetNeedWithCRLR (true);
        test_handle->m_pm_comm_handle->WriteCommand(atstr.c_str(), atstr.length(), ATTIMEOUT, false, atret);
        test_handle->m_pm_comm_handle->SetNeedWithCRLR (false);
        if (strstr(atret.resultLst[0].eleLst[1].str_value.c_str(), "OK") != NULL)
        {
            return E_ATADLL_RESULT_SUCCESS;
        }

        Sleep (100);
    }

    return E_ATADLL_RESULT_FAILED;
}


