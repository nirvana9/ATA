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
#include "..\header\ATA_DLL_TestItem_Vibrator.h"
#include "..\..\..\PowerDLL\PowerFunc.h"

CTestItem_Vibrator::CTestItem_Vibrator(ATA_DLL_Handle *current_handle, int timeout, E_ATDLL_TEST_ITEM_COUNT item, char *testName)
    : CTestItem_Interface(current_handle, timeout, item, testName)
{
    psu_handle = 0;
}

CTestItem_Vibrator::~CTestItem_Vibrator()
{
}

E_ATADLL_RESULT CTestItem_Vibrator::StartTest ()
{
	E_ATADLL_RESULT ata_result = E_ATADLL_RESULT_SUCCESS;
#ifdef __ATA30__
	ata_result = TestVibratorATA30 ();
#else
	ata_result = TestVibratorATA10 ();
#endif

	return ata_result;
}

E_ATADLL_RESULT CTestItem_Vibrator::TestVibratorATA10 ()
{
    ATResult atret;
    E_ATADLL_RESULT ata_result = E_ATADLL_RESULT_SUCCESS;

    test_handle->m_commonCFG.cbTestProgess (test_handle->handle_index, E_VIBRATOR, 10);

    double  d_curr = 0;
    bool    b_psu_result = false;
    double	current[20];
    double	d_curr_sum = 0;
    double	d_curr_min = 0;
    double	d_curr_max = 0;

    int		count = 3;
    int 	i = 0;;
    char 	tmpStr[512];

    test_handle->m_testCNF->vibrator_cnf.bPass	= true;

    // Measure current before opening vibrator
#ifndef INTERNEL_DEBUG_VERSION

    if (test_handle->m_commonCFG.vibrator_cfg.bOnBoard)
    {
        for (i = 0; i < 3; i ++)
        {
            if (E_ATADLL_RESULT_SUCCESS == SendCommandPM ("VibrateStatus?\r\n", "ok"))
            {
                AddLog ("[Vibrator ERROR] Vibrator state is already ON before opening vibrator.");
                test_handle->m_testCNF->vibrator_cnf.bPass	= false;
                return E_ATADLL_RESULT_BOX_FAILED;
            }
        }

        AddLog ("[OK] Vibrator state is OFF before opening vibrator.");
    }
    else
    {
        d_curr = 0;
        d_curr_min = 9999999;
        d_curr_max = 0;
        MeasureCurrentPreprocess ();
        for (i = 0; i < count; i ++)
        {
            Sleep (50);
            ata_result = MeasureCurrent (current[i]);
            if (ata_result != E_ATADLL_RESULT_SUCCESS)
            {
                AddLog ("[TestBox ERROR]Power measure before vibrator run fail.");
                goto Error;
            }

            if (d_curr_min > current[i])
                d_curr_min = current[i];
            if (d_curr_max < current[i])
                d_curr_max = current[i];

            d_curr_sum += current[i];
        }
        d_curr_sum = d_curr_sum - d_curr_min - d_curr_max;
        d_curr = d_curr_sum / (count - 2);

        memset (tmpStr, 0x0, 512*sizeof(char));
        for (i = 0; i < count; i ++)
        {
            sprintf (tmpStr, "%s %.3f", tmpStr, current[i]);
        }

        AddLog ("[TestBox]Measured list before opening vibrator:\r\n\t\t%s", tmpStr);
        test_handle->m_testCNF->vibrator_cnf.d_current_before    = d_curr;
    }
#endif

    // Open Vibrator
    ata_result = OpenVibrator ();
    if (E_ATADLL_RESULT_SUCCESS != ata_result)
    {
        goto Error;
    }
    test_handle->m_commonCFG.cbTestProgess (test_handle->handle_index, E_VIBRATOR, 30);

    // Measure current after opening vibrator
#ifndef INTERNEL_DEBUG_VERSION

    if (test_handle->m_commonCFG.vibrator_cfg.bOnBoard)
    {
        for (i = 0; i < 3; i ++)
        {
            if (E_ATADLL_RESULT_SUCCESS == SendCommandPM ("VibrateStatus?\r\n", "ok"))
            {
                AddLog ("[TestBox OK] Vibrator state is ON after opening vibrator.");
                break;
            }
        }

        if (i >= 5)
        {
            AddLog ("[TestBox ERROR] Vibrator state is Off after opening vibrator.");
            test_handle->m_testCNF->vibrator_cnf.bPass	= false;
            return E_ATADLL_RESULT_BOX_FAILED;
        }
    }
    else
    {
        d_curr = 0;
        d_curr_min = 9999999;
        d_curr_max = 0;
        MeasureCurrentPreprocess ();
        for (i = 0; i < count; i ++)
        {
            Sleep (50);
            ata_result = MeasureCurrent (current[i]);
            if (ata_result != E_ATADLL_RESULT_SUCCESS)
            {
                AddLog ("[TestBox ERROR]Power measure fail after opening vibrator.");
                goto Error;
            }

            if (d_curr_min > current[i])
                d_curr_min = current[i];
            if (d_curr_max < current[i])
                d_curr_max = current[i];

            d_curr_sum += current[i];
        }
        d_curr_sum = d_curr_sum - d_curr_min - d_curr_max;
        d_curr = d_curr_sum / (count - 2);

        memset (tmpStr, 0x0, 512*sizeof(char));
        for (i = 0; i < count; i ++)
        {
            sprintf (tmpStr, "%s %.3f", tmpStr, current[i]);
        }

        AddLog ("[TestBox]Measured list after opening vibrator:\r\n\t\t%s", tmpStr);
        test_handle->m_testCNF->vibrator_cnf.d_current_after	= d_curr;
        test_handle->m_testCNF->vibrator_cnf.d_current_after	= d_curr_max;
    }
#endif

    // Close Vibrator
    ClearATResPool();
    ata_result = CloseVibrator ();
    if (E_ATADLL_RESULT_SUCCESS != ata_result)
    {
        goto Error;
    }

    test_handle->m_commonCFG.cbTestProgess (test_handle->handle_index, E_VIBRATOR, 100);
    test_handle->m_testCNF->vibrator_cnf.bPass = true;//mingxue add

    // Target will return an extra no test.
    //GetATResponse(atret);

    return E_ATADLL_RESULT_SUCCESS;

Error:
    test_handle->m_commonCFG.cbTestProgess (test_handle->handle_index, E_VIBRATOR, 100);
    test_handle->m_testCNF->vibrator_cnf.bPass = false;
    //GetATResponse(atret);
    return ata_result;
}

E_ATADLL_RESULT CTestItem_Vibrator::TestVibratorATA30 ()
{
	test_handle->m_commonCFG.cbTestProgess (test_handle->handle_index, E_VIBRATOR, 10);
	ClearATResPool ();

	ATResult atret;
    string atstr = "";

	char at_buff[256];
	sprintf (at_buff, "AT+PROPERTY=0,1,\"Vibrator.PhoneAutoTest,%d\"", 1);
	atstr = at_buff;
	atstr += CR;
	test_handle->m_comm_handle->WriteCommand(atstr.c_str(), atstr.length(), 100, false, atret);
	Sleep (1000);
	test_handle->m_commonCFG.cbTestProgess (test_handle->handle_index, E_VIBRATOR, 40);
	//GetATResponse(atret);
	ClearATResPool ();
	
    atstr = "AT+VIBRATOR";
	atstr += CR;
	test_handle->m_comm_handle->WriteCommand(atstr.c_str(), atstr.length(), 100, false, atret);
    if (!GetATResponse (atret))
	{
		AddLog ("AT Response timeout.");
		goto Error;
	}

	test_handle->m_commonCFG.cbTestProgess (test_handle->handle_index, E_VIBRATOR, 100);
	return E_ATADLL_RESULT_SUCCESS;

Error:

	test_handle->m_commonCFG.cbTestProgess (test_handle->handle_index, E_VIBRATOR, 100);
	return E_ATADLL_RESULT_FAILED;
	
}

E_ATADLL_RESULT CTestItem_Vibrator::OpenVibrator ()
{
    ClearATResPool();

    ATResult atret;
    string atstr = "";
#ifdef __ATA20__
    if (IsAutoMode())
    {
        atstr = "AT+VIBRATOR=1,1";
    }
    else
    {
        atstr = "AT+VIBRATOR=1,2";
    }
#else
    atstr = "AT+VIBRATOR";
#endif
    atstr += CR;
    test_handle->m_comm_handle->WriteCommand(atstr.c_str(), atstr.length(), 100, false, atret);
    Sleep (300);
	
    return E_ATADLL_RESULT_SUCCESS;
}

E_ATADLL_RESULT CTestItem_Vibrator::CloseVibrator ()
{
    ClearATResPool();

    ATResult atret;
    string atstr = "";
#ifdef __ATA20__
    if (IsAutoMode())
    {
       //atstr ="AT+VIBRATOR=STOP"; //atstr = "AT+VIBRATOR=1,0";mingxue 20150710
       atstr = "AT+VIBRATOR=1,0";
    }
#else
    atstr = "AT+VIBRATOR=CLOSE";
#endif
    atstr += CR;
    test_handle->m_comm_handle->WriteCommand(atstr.c_str(), atstr.length(), 100, false, atret);
    Sleep (100);
	// Target will return  not test.
	if (!GetATResponse(atret))
		{
			AddLog ("[VIBRATOR ERROR] %s response timeout.",atstr.c_str());
			return E_ATADLL_RESULT_AT_RESPONSE_TIMEOUT;
		}

    return E_ATADLL_RESULT_SUCCESS;
}

E_ATADLL_RESULT CTestItem_Vibrator::MeasureCurrentPreprocess ()
{
    ATResult atret;
    bool result = false;
    string atstr = "Vibrate=1\r\n";

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
    Sleep (1500);
    return E_ATADLL_RESULT_SUCCESS;;
}


E_ATADLL_RESULT CTestItem_Vibrator::MeasureCurrent (double &current)
{
    ATResult atret;
    string atstr = "Current?\r\n";

    for (int i = 0; i < 5; i ++)
    {
        test_handle->m_pm_comm_handle->SetNeedWithCRLR (true);
        bool result = test_handle->m_pm_comm_handle->WriteCommand(atstr.c_str(), atstr.length(), ATTIMEOUT, false, atret);
        test_handle->m_pm_comm_handle->SetNeedWithCRLR (false);
        if (result)
        {
            current = atof(atret.resultLst[0].eleLst[1].str_value.c_str());
            current = (current / 1000.0);
            return E_ATADLL_RESULT_SUCCESS;
        }
        Sleep (300);
    }
	AddLog("[TestBox]measure current fail");
    return E_ATADLL_RESULT_BOX_FAILED;
}


