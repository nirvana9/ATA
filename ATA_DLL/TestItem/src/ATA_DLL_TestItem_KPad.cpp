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
 *   ATA_DLL_TestItem_TCard.cpp
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
#include <math.h>
#include "..\header\ATA_DLL_TestItem_KPad.h"
#include<math.h>
CTestItem_KPad::CTestItem_KPad(ATA_DLL_Handle *current_handle, int timeout, E_ATDLL_TEST_ITEM_COUNT item, char *testName)
    : CTestItem_Interface(current_handle, timeout, item, testName)
{
}

CTestItem_KPad::~CTestItem_KPad()
{
}


E_ATADLL_RESULT CTestItem_KPad::StartTest ()
{
	E_ATADLL_RESULT testResultCode;
    ClearATResPool();
	test_handle->m_commonCFG.cbTestProgess (test_handle->handle_index, E_KEYPAD, 10);

    ATResult atret;
    string atstr = "";

#ifdef __ATA30__
	char at_buff[128];
    sprintf (at_buff, "AT+PROPERTY=0,1,\"FtmKeyType,%d\"", 1);
	atstr = at_buff;
    atstr += CR;

	test_handle->m_comm_handle->WriteCommand(atstr.c_str(), atstr.length(), 200, false, atret);
	/*mingxue20150708*/
	if (!GetATResponse(atret))
    {
        AddLog ("[KPD ERROR] %s response timeout.",atstr.c_str());
    }
	if (NULL != strstr(atret.resultLst[0].eleLst[0].str_value.c_str(), "pass"))
	{
		AddLog ("[OK] %s set ok.",atstr.c_str());
	}
	else if (NULL != strstr(atret.resultLst[0].eleLst[0].str_value.c_str(), "fail"))
	{
		AddLog ("[KPD ERROR] %s set fail.",atstr.c_str());
	}

	/*end*/
	Sleep (1000);
#endif

#ifdef __ATA20__
    if (IsAutoMode())
    {
        atstr = "AT+KEY=1,1";
    }
    else
    {
        atstr = "AT+KEY=1,2";
    }
#else
    atstr = "AT+KEY";
#endif
    atstr += CR;

	ClearATResPool ();
    test_handle->m_comm_handle->WriteCommand(atstr.c_str(), atstr.length(), 200, false, atret);
	test_handle->m_commonCFG.cbTestProgess (test_handle->handle_index, E_KEYPAD, 20);

#ifndef INTERNEL_DEBUG_VERSION
    if (test_handle->m_commonCFG.kpad_cfg.bExterConnection)
    {
        test_handle->m_commonCFG.cbTestProgess (test_handle->handle_index, E_KEYPAD, 30);
        if (ConnectExteralPin ())
        {
            AddLog ("[TestBox OK] Connect external pin ok.");
        }
        else
        {
            AddLog ("[TestBox ERROR] Connect external pin fail.");
			return E_ATADLL_RESULT_BOX_FAILED;
        }
        test_handle->m_commonCFG.cbTestProgess (test_handle->handle_index, E_KEYPAD, 70);
		
    }

    if (test_handle->m_commonCFG.kpad_cfg.bElectroMagicSwitch)
    {
        test_handle->m_commonCFG.cbTestProgess (test_handle->handle_index, E_KEYPAD, 30);
        if (ConnectElectroMagSwtich())
        {
            AddLog ("[TestBox OK] Connect electrol magic switch ok.");
        }
        else
        {
            AddLog ("[TestBox ERROR] Connect electrol magic switch fail.");
			return E_ATADLL_RESULT_BOX_FAILED;
        }
        test_handle->m_commonCFG.cbTestProgess (test_handle->handle_index, E_KEYPAD, 70);
		
    }
#endif


#ifdef __ATA20__
	test_handle->m_testCNF->kpad_cnf.result = false;
	do 
	{
		if (!GetATResponse(atret))
	    {
	        AddLog ("[KPD ERROR] %s response timeout.",atstr.c_str());
			test_handle->m_testCNF->kpad_cnf.result = false;
	        return E_ATADLL_RESULT_AT_RESPONSE_TIMEOUT;
	    }

		if (NULL != strstr(atret.resultLst[0].eleLst[0].str_value.c_str(), "pass"))
	    {
	        AddLog ("[OK] Key test ok");
	        test_handle->m_testCNF->kpad_cnf.result = true;
			break;
	    }
	} while (1);
#else
    if (!GetATResponse(atret))
    {
        AddLog ("[Error] AT response timeout.");
        return E_ATADLL_RESULT_AT_RESPONSE_TIMEOUT;
    }

    if (NULL != strstr(atret.resultLst[0].eleLst[0].str_value.c_str(), "pass"))
    {
        AddLog ("[OK] Key test ok");
        test_handle->m_testCNF->kpad_cnf.result = true;
    }
    else
    {
        AddLog ("[ERROR] Key test fail");
        test_handle->m_testCNF->kpad_cnf.result = false;
    }
#endif

    test_handle->m_commonCFG.cbTestProgess (test_handle->handle_index, E_KEYPAD, 100);
    return E_ATADLL_RESULT_SUCCESS;
}

bool CTestItem_KPad::ConnectElectroMagSwtich ()
{
#if 0
    // Power on charger
    SwitchToCharger();
    AddLog ("Switch to charger.");
    Sleep (1000);

    // Reset
    ResetPM ();
    AddLog ("Reset charger.");
#endif
	
	if(E_ATADLL_RESULT_SUCCESS !=SendCommandPM ("GPIO1Dir=1\r\n", "OK"))
	{
			return false;
	}
	Sleep (1000);
	if(E_ATADLL_RESULT_SUCCESS !=SendCommandPM ("GPIO1Level=0\r\n", "OK"))
	{
			return false;
	}
	
	Sleep (1500);
	if(E_ATADLL_RESULT_SUCCESS !=SendCommandPM ("GPIO1Level=1\r\n", "OK"))
	{
			return false;
	}
	Sleep (500);
	if(E_ATADLL_RESULT_SUCCESS !=SendCommandPM ("GPIO1Dir=0\r\n", "OK"))
	{
		return false;
	}
	
    return true;
	
}

bool CTestItem_KPad::ConnectExteralPin ()
{
    // Test keypad
    char cmdStr[64];
    sprintf (cmdStr, "Keypad=1\r\n");

    char responseStr[64];
    sprintf (responseStr, "Keypad:OK");

    ATResult atret;
    string atstr = cmdStr;

    bool result = false;
    for (int i = 0; i < 2; i ++)
    {
        test_handle->m_pm_comm_handle->SetNeedWithCRLR (true);
        bool result = test_handle->m_pm_comm_handle->WriteCommand(atstr.c_str(), atstr.length(), 8000, false, atret);
        test_handle->m_pm_comm_handle->SetNeedWithCRLR (false);

        if (result && strstr(atret.resultLst[0].eleLst[1].str_value.c_str(), "OK") != NULL)
        {
            AddLog ("%s, %s", atret.resultLst[0].eleLst[0].str_value.c_str(), atret.resultLst[0].eleLst[1].str_value.c_str());
            return true;
        }

        Sleep (300);
    }
    AddLog ("[TestBox]Set keypad failed.");

    // Reset
    ResetPM ();
    //AddLog ("[TestBox]Reset charger.");

    return false;
}

