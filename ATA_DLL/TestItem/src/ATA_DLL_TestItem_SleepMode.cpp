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
*   ATA_DLL_TestItem_SleepMode.cpp
 *
 * Project:
 * --------
 *   ATA_DLL
 *
 * Description:
 * ------------
*   This module contains the idle & off current test flow
 *
 * Author:
 * -------
 *   Yintong Zhang (mtk70681)
 *
 *==============================================================================
 *           HISTORY
 * Below this line, this part is controlled by PVCS VM. DO NOT MOaDIFY!!
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
#include "..\header\ATA_DLL_TestItem_SleepMode.h"
#include "..\..\..\PowerDLL\PowerFunc.h"
#include "..\..\GlobalOBJ.h"

CTestItem_SleepMode::CTestItem_SleepMode(ATA_DLL_Handle *current_handle, int timeout, E_ATDLL_TEST_ITEM_COUNT item, char *testName)
    : CTestItem_Interface(current_handle, timeout, item, testName)
{
    psu_handle = 0;
}

CTestItem_SleepMode::~CTestItem_SleepMode()
{
}

E_ATADLL_RESULT CTestItem_SleepMode::StartTest ()
{
	glb_at_resp_flag = true;
    E_ATADLL_RESULT ata_result = E_ATADLL_RESULT_SUCCESS;

    if (strstr(m_TestItemName, "Off Current") != NULL)
    {
        ata_result = TestOffCurrent ();
    }
    else
    {
        ata_result = TestIdleCurrent ();
    }
    return ata_result;
}

E_ATADLL_RESULT CTestItem_SleepMode::TestIdleCurrent ()
{
    ClearATResPool();
    test_handle->m_commonCFG.cbTestProgess (test_handle->handle_index, E_SLPMODE, 10);

    ATResult atret;
    string atstr = "AT+IDLE";
    atstr += CR;
    AddLog ("[Idle]Control target enter sleep mode.");
    test_handle->m_comm_handle->SetEndNeedWithCRLR(true);
    test_handle->m_comm_handle->WriteCommand(atstr.c_str(), atstr.length(), ATTIMEOUT, false, atret);
    if (!GetATResponse(atret))
    {
        AddLog ("[Idle ERROR] Get AT response timeout.");
        return E_ATADLL_RESULT_AT_RESPONSE_TIMEOUT;
    }

    test_handle->m_commonCFG.cbTestProgess (test_handle->handle_index, E_SLPMODE, 30);
    if (NULL != strstr(atret.resultLst[0].eleLst[0].str_value.c_str(), "close com port"))
    {
        AddLog ("[OK] Enter Idle Mode");
    }
    else
    {
        AddLog ("[Idle ERROR] Enter Idle Mode fail.");
    }

    // Control USB Vbus disconnect
    AddLog ("Close kernel com port handle from Target.");
    test_handle->DisconnectTarget ();
    test_handle->m_commonCFG.cbTestProgess (test_handle->handle_index, E_SLPMODE, 40);
    Sleep (500);

    AddLog ("Disconnect USB Vbus to disable charging.");
    USBConnectEnable (false);
	Sleep (500);

	AddLog ("Pull down OTG ID pin.");
	//OTGIDPinPull (false);
    test_handle->m_commonCFG.cbTestProgess (test_handle->handle_index, E_SLPMODE, 50);

    AddLog ("Wait 5 second for target entering sleep mode.");
    Sleep (1000*10);

    // Control instrument measure idle current
    test_handle->m_commonCFG.cbTestProgess (test_handle->handle_index, E_SLPMODE, 60);
    double currentValue = 0.0;
    MeasureSleepCurrentPreprocess ();
    MeasureCurrent (currentValue);
    AddLog ("Measured current value is %.3f mA.", currentValue);
    test_handle->m_testCNF->slpmode_cnf.current = currentValue;

    // Control USB Vbus re-connect
    AddLog ("Reconnect USB Vbus.");
    test_handle->m_commonCFG.cbTestProgess (test_handle->handle_index, E_SLPMODE, 70);
    Sleep (1000 * 5);
    USBConnectEnable (true);
	Sleep (500);

	AddLog ("Pull up OTG ID pin.");
	//OTGIDPinPull (true);
    AddLog ("Wait and open kernel com port from target.");
    test_handle->ConnectTarget (2);

    Sleep (2000);

    test_handle->m_commonCFG.cbTestProgess (test_handle->handle_index, E_SLPMODE, 80);
    atstr = "AT+START";
    atstr += CR;
    test_handle->m_comm_handle->WriteCommand(atstr.c_str(), atstr.length(), ATTIMEOUT, false, atret);
    GetATResponse(atret); // remove the first return str "pass"
    if (!GetATResponse(atret))
    {
        AddLog ("[Error] Get AT response timeout.");
        return E_ATADLL_RESULT_AT_RESPONSE_TIMEOUT;
    }
    if (NULL != strstr(atret.resultLst[0].eleLst[0].str_value.c_str(), "pass"))
    {
        AddLog ("[OK] Test finished");
    }
    else
    {
        AddLog ("[Fail] Test Fail.");
    }

    test_handle->m_commonCFG.cbTestProgess (test_handle->handle_index, E_SLPMODE, 100);
    return E_ATADLL_RESULT_SUCCESS;
}



E_ATADLL_RESULT CTestItem_SleepMode::TestOffCurrent ()
{
	E_ATADLL_RESULT testResultCode = E_ATADLL_RESULT_SUCCESS;

    ClearATResPool ();
    test_handle->m_commonCFG.cbTestProgess (test_handle->handle_index, E_OFFMODE, 10);

    // Power off target
    testResultCode=PowerOffTarget();
    if (E_ATADLL_RESULT_SUCCESS != testResultCode)
    {
        AddLog ("[OFF ERROR] Power off target fail!");
        goto Error;
    }
    else
    {
        AddLog ("[OK] Power off target ok!");
    }
    test_handle->m_commonCFG.cbTestProgess (test_handle->handle_index, E_OFFMODE, 30);

    // Disconnect USB Vbus
    AddLog ("Disconnect USB Vbus to disable charging.");
    USBConnectEnable (false);
	Sleep (500);

	AddLog ("Pull down OTG ID pin.");
	OTGIDPinPull (false);
    test_handle->m_commonCFG.cbTestProgess (test_handle->handle_index, E_OFFMODE, 50);

    Sleep (3000);

	//SendCommandPM ("SetUA=1\r\n", "");
    testResultCode=MeasureCurrent (test_handle->m_testCNF->offmode_cnf.current);
	if(E_ATADLL_RESULT_SUCCESS!=testResultCode)
		{
		goto Error;
		}
		
	//SendCommandPM ("SetMA=1\r\n", "");
    AddLog ("Off mode current is %.3f uA.", test_handle->m_testCNF->offmode_cnf.current);

    AddLog ("Connect USB Vbus to enable charging.");
    USBConnectEnable (true);
	Sleep (500);

	AddLog ("Pull up OTG ID pin.");
	OTGIDPinPull (true);
    test_handle->m_commonCFG.cbTestProgess (test_handle->handle_index, E_OFFMODE, 70);

    test_handle->m_commonCFG.cbTestProgess (test_handle->handle_index, E_OFFMODE, 100);
    return E_ATADLL_RESULT_SUCCESS;

Error:
    test_handle->m_commonCFG.cbTestProgess (test_handle->handle_index, E_OFFMODE, 100);
    return testResultCode;
}


E_ATADLL_RESULT CTestItem_SleepMode::MeasureSleepCurrentPreprocess ()
{
    ATResult atret;
    bool result = false;
    string atstr = "Sleep=1\r\n";

    for (int i = 0; i < 3; i ++)
    {
        test_handle->m_pm_comm_handle->SetNeedWithCRLR (true);
        result = test_handle->m_pm_comm_handle->WriteCommand(atstr.c_str(), atstr.length(), ATTIMEOUT, false, atret);
        test_handle->m_pm_comm_handle->SetNeedWithCRLR (false);
        if (result && strstr(atret.resultLst[0].eleLst[1].str_value.c_str(), "OK") != NULL)
        {
            break;
        }
        else
        {
            Sleep (200);
        }
    }
    Sleep (1000);
    return E_ATADLL_RESULT_SUCCESS;;
}


E_ATADLL_RESULT CTestItem_SleepMode::USBConnectEnable (bool bEnable)
{
    ATResult atret;
    string atstr = "Current?\r\n";

    if (!bEnable)
    {
        atstr = "End\r\n";
    }
    else
    {
        atstr = "Charge=1\r\n";
    }

    for (int i = 0; i < 3; i ++)
    {
        test_handle->m_pm_comm_handle->SetNeedWithCRLR (true);
        bool result = test_handle->m_pm_comm_handle->WriteCommand(atstr.c_str(), atstr.length(), ATTIMEOUT, false, atret);
        test_handle->m_pm_comm_handle->SetNeedWithCRLR (false);
        if (result) // if has "OK"
        {
            return E_ATADLL_RESULT_SUCCESS;
        }
        Sleep (50);
    }

    return E_ATADLL_RESULT_BOX_FAILED;
}

E_ATADLL_RESULT CTestItem_SleepMode::MeasureCurrent (double &current)
{
    ATResult atret;
    string atstr = "Current?\r\n";

	atstr = "Current?\r\n";
    for (int i = 0; i < 3; i ++)
    {
        test_handle->m_pm_comm_handle->SetNeedWithCRLR (true);
        bool result = test_handle->m_pm_comm_handle->WriteCommand(atstr.c_str(), atstr.length(), ATTIMEOUT, false, atret);
        test_handle->m_pm_comm_handle->SetNeedWithCRLR (false);
        if (result)
        {
            AddLog ("PM returned value: %s - %s", atret.resultLst[0].eleLst[0].str_value.c_str(), atret.resultLst[0].eleLst[1].str_value.c_str());
            current = atof(atret.resultLst[0].eleLst[1].str_value.c_str());
            //current = (current / 1000.0);
            return E_ATADLL_RESULT_SUCCESS;
        }
        Sleep (50);
    }
	
    return E_ATADLL_RESULT_BOX_FAILED;
}

E_ATADLL_RESULT CTestItem_SleepMode::PowerOffTarget()
{
    ATResult atret;
    string atstr = "OffCurrent=1\r\n";

    for (int i = 0; i < 3; i ++)
    {
        test_handle->m_pm_comm_handle->SetNeedWithCRLR (true);
        bool result = test_handle->m_pm_comm_handle->WriteCommand(atstr.c_str(), atstr.length(), ATTIMEOUT, false, atret);
        test_handle->m_pm_comm_handle->SetNeedWithCRLR (false);
        if (!result)
            continue;

        if (strstr(atret.resultLst[0].eleLst[1].str_value.c_str(), "OK")!=NULL) // if has "OK"
        {
            return E_ATADLL_RESULT_SUCCESS;
        }
        Sleep (50);
    }

    return E_ATADLL_RESULT_BOX_FAILED;
}

E_ATADLL_RESULT CTestItem_SleepMode::OTGIDPinPull (bool bEnable)
{
    ATResult atret;
    string atstr = "";

    if (bEnable)
    {
        atstr = "GPIO2Level=0\r\n";
    }
    else
    {
        atstr = "GPIO2Level=1\r\n";
    }

    for (int i = 0; i < 3; i ++)
    {
        test_handle->m_pm_comm_handle->SetNeedWithCRLR (true);
        bool result = test_handle->m_pm_comm_handle->WriteCommand(atstr.c_str(), atstr.length(), ATTIMEOUT, false, atret);
        test_handle->m_pm_comm_handle->SetNeedWithCRLR (false);
        if (result) // if has "OK"
        {
            return E_ATADLL_RESULT_SUCCESS;
        }
        Sleep (50);
    }

    return E_ATADLL_RESULT_BOX_FAILED;
}


