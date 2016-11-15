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
 *   ATA_DLL_TestItem_LED.cpp
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
#include "..\header\ATA_DLL_TestItem_LED.h"
#include "..\..\..\PowerDLL\PowerFunc.h"

CTestItem_LED::CTestItem_LED(ATA_DLL_Handle *current_handle, int timeout, E_ATDLL_TEST_ITEM_COUNT item, char *testName)
    : CTestItem_Interface(current_handle, timeout, item, testName)
{
    psu_handle = 0;
}

CTestItem_LED::~CTestItem_LED()
{
}

#define LED_TEST_TIMES 2
E_ATADLL_RESULT CTestItem_LED::StartTest ()
{
    char led_state[LED_TEST_TIMES][MAX_LED_SUPPORT_NUM];
    ATResult atr;
	E_ATADLL_RESULT testResultCode;

    ClearATResPool();
    test_handle->m_commonCFG.cbTestProgess (test_handle->handle_index, E_LED, 10);
    OpenLED ();
    AddLog ("Open LED.");
    test_handle->m_commonCFG.cbTestProgess (test_handle->handle_index, E_LED, 30);
    Sleep (2000);

#ifndef INTERNEL_DEBUG_VERSION
    // Measure and record LED state
    /**
    if (E_ATADLL_RESULT_SUCCESS == MeasureLEDState (led_state)) {
    	sprintf (test_handle->m_testCNF->led_cnf.state, led_state);
    }
    **/

    // Measure and record LED state
    int index = 0;
    AddLog ("[TestBox]Measure LED state %d times for stability.", LED_TEST_TIMES);
    for (index = 0; index < LED_TEST_TIMES; index ++)  	// Measure LED_TEST_TIMES times
    {
        if (E_ATADLL_RESULT_SUCCESS == MeasureLEDState (led_state[index]))
        {
            AddLog ("[%d] LED State: %s", index+1, led_state[index]);
        }
		else
		{
			 AddLog ("[ERROR][%d] measure LED State fail", index+1);
		}
    }

    int ledNumber[LED_TEST_TIMES] = {0};
    for (index = 0; index < LED_TEST_TIMES; index ++)  	// Compute LED number for each test time
    {
        ledNumber[index] = 0;
        for (int ledIndex = 0; ledIndex < strlen(led_state[index]); ledIndex ++)
        {
            if (ledIndex >= MAX_LED_SUPPORT_NUM) break;
            if (led_state[index][ledIndex] == '0')
            {
                ledNumber[index] ++;
            }
        }
    }

    int choosedIndex = 0;
    int maxLedNum = -1;
    for (index = 0; index < LED_TEST_TIMES; index ++)  	// choose the most proper test result ( with more led number )
    {
        if (maxLedNum < ledNumber[index])
        {
            maxLedNum = ledNumber[index];
            choosedIndex = index;
        }
    }

    sprintf (test_handle->m_testCNF->led_cnf.state, led_state[choosedIndex]);
    AddLog ("[Time%d] is the most proper test result: %s", choosedIndex+1, led_state[choosedIndex]);
#endif
    test_handle->m_commonCFG.cbTestProgess (test_handle->handle_index, E_LED, 50);

    // Close LED
    testResultCode=CloseLED ();
	if(E_ATADLL_RESULT_SUCCESS != testResultCode)
		{
			test_handle->m_commonCFG.cbTestProgess (test_handle->handle_index, E_LED, 100);
			return testResultCode;
		}
    AddLog ("Close LED successful.");
    test_handle->m_commonCFG.cbTestProgess (test_handle->handle_index, E_LED, 80);
    Sleep (1500);

#ifndef INTERNEL_DEBUG_VERSION
    // Measure LED state ( LED state should be all closed
    if (E_ATADLL_RESULT_SUCCESS == MeasureLEDState (led_state[0]))
    {
        if (strstr(led_state[0], "0") == NULL)
        {
            AddLog ("[OK] LED state ok after cloing LED: %s.", led_state[0]);
        }
        else
        {
            AddLog ("[TestBox ERROR] LED state error after cloing LED: %s.", led_state[0]);
            return E_ATADLL_RESULT_BOX_FAILED;
        }
    }
    else
    {
        return E_ATADLL_RESULT_BOX_FAILED;
    }
#endif
    test_handle->m_commonCFG.cbTestProgess (test_handle->handle_index, E_LED, 100);

    // process addional return string "not test"
    //GetATResponse (atr);mingxue remove

    return E_ATADLL_RESULT_SUCCESS;
}

E_ATADLL_RESULT CTestItem_LED::OpenLED ()
{
    ClearATResPool();

    ATResult atret;
    string atstr = "";
#ifdef __ATA20__
    if (IsAutoMode())
    {
        atstr = "AT+LED=1,1";
    }
    else
    {
        atstr = "AT+LED=1,2";
    }
#else
    atstr = "AT+LED";
#endif
    atstr += CR;
    test_handle->m_comm_handle->WriteCommand(atstr.c_str(), atstr.length(), 100, false, atret);
    Sleep (100);
	return E_ATADLL_RESULT_SUCCESS;
}

E_ATADLL_RESULT CTestItem_LED::CloseLED ()
{
    ClearATResPool();

    ATResult atret;
    string atstr = "";
#ifdef __ATA20__
    if (IsAutoMode())
    {
       //atstr = "AT+LED=STOP"; //atstr = "AT+LED=1,0";
       atstr = "AT+LED=1,0";
    }
#else
    atstr = "AT+LED=CLOSE";
#endif
    atstr += CR;
    test_handle->m_comm_handle->WriteCommand(atstr.c_str(), atstr.length(), 100, false, atret);
    Sleep (100);
	// Target will return  not test.
	if (!GetATResponse(atret))
		{
			AddLog ("[LED ERROR] %s response timeout.",atstr.c_str());
			test_handle->m_testCNF->bLEDDetected=false;
			return E_ATADLL_RESULT_AT_RESPONSE_TIMEOUT;
		}
	else if (NULL != strstr(atret.resultLst[0].eleLst[0].str_value.c_str(), "Cannot find"))
	{
		AddLog ("[LED ERROR] LED not support in target.");
		test_handle->m_testCNF->bLEDDetected=false;
		return E_ATADLL_RESULT_NOT_SUPPORT;
	}
	test_handle->m_testCNF->bLEDDetected=true;
	return E_ATADLL_RESULT_SUCCESS;
}

E_ATADLL_RESULT CTestItem_LED::MeasureLEDState (char *led_state)
{
    ATResult atret;
    string atstr = "LED?\r\n";
    bool result = false;

    for (int i = 0; i < 2 ; i ++)
    {
        test_handle->m_pm_comm_handle->SetNeedWithCRLR (true);
        bool result = test_handle->m_pm_comm_handle->WriteCommand(atstr.c_str(), atstr.length(), ATTIMEOUT, false, atret);
        test_handle->m_pm_comm_handle->SetNeedWithCRLR (false);
        Sleep (300);
    }
    Sleep (2000);

    test_handle->m_pm_comm_handle->SetNeedWithCRLR (true);
    result = test_handle->m_pm_comm_handle->WriteCommand(atstr.c_str(), atstr.length(), ATTIMEOUT, false, atret);
    test_handle->m_pm_comm_handle->SetNeedWithCRLR (false);

    if (result)
    {
        memcpy (led_state, atret.resultLst[0].eleLst[1].str_value.c_str(), atret.resultLst[0].eleLst[1].str_value.length());
        led_state[atret.resultLst[0].eleLst[1].str_value.length()] = '\0';

        AddLog ("[OK] LED State : %s .", led_state);
        return E_ATADLL_RESULT_SUCCESS;
    }

    AddLog ("[TestBox ERROR] Measure LED state error.");
    return E_ATADLL_RESULT_BOX_FAILED;
}



