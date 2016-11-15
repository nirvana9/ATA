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
*   ATA_DLL_TestItem_OTG.cpp
 *
 * Project:
 * --------
 *   ATA_DLL
 *
 * Description:
 * ------------
*   This module is for OTG auto test.
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
#include "..\header\ATA_DLL_TestItem_OTG.h"

CTestItem_OTG::CTestItem_OTG(ATA_DLL_Handle *current_handle, int timeout, E_ATDLL_TEST_ITEM_COUNT item, char *testName)
    : CTestItem_Interface(current_handle, timeout, item, testName)
{
    
}

CTestItem_OTG::~CTestItem_OTG()
{
}

E_ATADLL_RESULT CTestItem_OTG::StartTest ()
{
    E_ATADLL_RESULT ata_result = E_ATADLL_RESULT_SUCCESS;
	
	ata_result = TestOTG ();
    return ata_result;
}

E_ATADLL_RESULT CTestItem_OTG::TestOTG ()
{
    ClearATResPool();
    test_handle->m_commonCFG.cbTestProgess (test_handle->handle_index, E_OTG, 10);
	OTGInit ();

    ATResult atret;
    string atstr = "AT+OTG";
    atstr += CR;
	
    AddLog ("Control target enter OTG mode....");
    //test_handle->m_comm_handle->SetEndNeedWithCRLR(true);
    test_handle->m_comm_handle->WriteCommand(atstr.c_str(), atstr.length(), ATTIMEOUT, false, atret);

    // Control USB Vbus disconnect
    AddLog ("Close kernel com port handle of Target....");
    test_handle->DisconnectTarget ();
    test_handle->m_commonCFG.cbTestProgess (test_handle->handle_index, E_OTG, 40);
    Sleep (100);

	// Disconnect USB cable, then connect OTG
#ifdef INTERNEL_DEBUG_VERSION
	char popupInfo[256];
    sprintf (popupInfo, "[DUT%d] Please disconnect USB Cable, then connect OTG!", test_handle->handle_index+1);
    ::AfxMessageBox (popupInfo, 0, 0);
	test_handle->m_commonCFG.cbTestProgess (test_handle->handle_index, E_OTG, 50);
#else
    AddLog ("Disconnect USB Vbus to disable charging.");
    USBConnectEnable (false);
    test_handle->m_commonCFG.cbTestProgess (test_handle->handle_index, E_OTG, 50);

	AddLog ("Pull down OTG Pin to enable OTG.");
	OTGIDPinPull (true);
#endif

	Sleep (3000);

	// Control instrument measure voltage
#ifdef INTERNEL_DEBUG_VERSION
	sprintf (popupInfo, "[DUT%d] Please measure OTG pin voltage.", test_handle->handle_index+1);
	::AfxMessageBox (popupInfo, 0, 0);
	test_handle->m_commonCFG.cbTestProgess (test_handle->handle_index, E_OTG, 60);
#else
    test_handle->m_commonCFG.cbTestProgess (test_handle->handle_index, E_OTG, 60);
	double vol = 0.0;
	double vol2 = 0.0;
	double vol3 = 0.0;
	MeasureVoltage (vol);
	AddLog ("[1] Measure voltage result: %.3f v", vol);
	Sleep (500);
	MeasureVoltage (vol2);
	AddLog ("[2] Measure voltage result: %.3f v", vol2);
	Sleep (500);
	MeasureVoltage (vol3);
	AddLog ("[3] Measure voltage result: %.3f v", vol3);
	if (vol > vol2) {
		test_handle->m_testCNF->otg_cnf.d_OTGVol = vol;
	} else {
		test_handle->m_testCNF->otg_cnf.d_OTGVol = vol2;
	}

	if (test_handle->m_testCNF->otg_cnf.d_OTGVol < vol3){
		test_handle->m_testCNF->otg_cnf.d_OTGVol = vol3;
	}
	AddLog ("[OTG]Measure voltage final result: %.3f v", test_handle->m_testCNF->otg_cnf.d_OTGVol);
	
	//test_handle->m_testCNF->otg_cnf.d_OTGVol = vol;
	//AddLog ("Measure voltage result: %.3f v", vol);
#endif

	Sleep (3000);

	// Control USB Vbus re-connect
#ifdef INTERNEL_DEBUG_VERSION
	sprintf (popupInfo, "[DUT%d] Please disconnect OTG, then connect USB cable.", test_handle->handle_index+1);
	::AfxMessageBox (popupInfo, 0, 0);
	test_handle->m_commonCFG.cbTestProgess (test_handle->handle_index, E_OTG, 70);
#else
	AddLog ("Pull up OTG Pin to disable OTG.");
	OTGIDPinPull (false);
	Sleep (2000);
	
    AddLog ("Reconnect USB Vbus.");
    test_handle->m_commonCFG.cbTestProgess (test_handle->handle_index, E_OTG, 70);
    USBConnectEnable (true);
#endif

	Sleep (3000);

    AddLog ("Wait and open kernel com port of target.");
    test_handle->ConnectTarget (2);

	Sleep (2000);

    test_handle->m_commonCFG.cbTestProgess (test_handle->handle_index, E_OTG, 80);
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

    test_handle->m_commonCFG.cbTestProgess (test_handle->handle_index, E_OTG, 100);
    return E_ATADLL_RESULT_SUCCESS;
}

E_ATADLL_RESULT CTestItem_OTG::OTGInit ()
{
	SendCommandPM ("GPIO2Dir=1\r\n", "");
	Sleep (300);
	return E_ATADLL_RESULT_SUCCESS;
}

E_ATADLL_RESULT CTestItem_OTG::USBConnectEnable (bool bEnable)
{
    ATResult atret;
    string atstr = "";

    if (bEnable)
    {
        atstr = "Charge=1\r\n";
    }
    else
    {
        atstr = "Charge=0\r\n";
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

E_ATADLL_RESULT CTestItem_OTG::OTGIDPinPull (bool bEnable)
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

E_ATADLL_RESULT CTestItem_OTG::MeasureVoltage (double &vol)
{
    ATResult atret;
    string atstr = "";
    bool result = false;

    atstr = "LEDVoltage=1\r\n";

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

    return E_ATADLL_RESULT_BOX_FAILED;
}


