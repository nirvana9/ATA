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
 *   ATA_DLL_TestItem_SIMCard.cpp
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
#include "..\header\ATA_DLL_TestItem_SIMCard.h"

CTestItem_SIMCard::CTestItem_SIMCard(ATA_DLL_Handle *current_handle, int timeout, E_ATDLL_TEST_ITEM_COUNT item, char *testName)
    : CTestItem_Interface(current_handle, timeout, item, testName)
{
}

CTestItem_SIMCard::~CTestItem_SIMCard()
{
}

E_ATADLL_RESULT CTestItem_SIMCard::StartTest ()
{
    return TestSIM (4);
}

void CTestItem_SIMCard::SwitchToSIM (int sim_num)
{
    char tmp_char[128];
    sprintf (tmp_char, "AT+ESUO=%d", sim_num + 3);

    string atstr = tmp_char;
    atstr += CR;
    ATResult atret;
    test_handle->m_comm_handle->WriteCommand(atstr.c_str(), atstr.length(), 500, false, atret);
}


// sim_num : 1 ~ 4
E_ATADLL_RESULT CTestItem_SIMCard::TestSIM (int sim_num)
{
	E_ATADLL_RESULT testResultCode;
    ClearATResPool();
    test_handle->m_commonCFG.cbTestProgess (test_handle->handle_index, E_SIM, 10);

    ATResult atret;
    string atstr = "";

#ifdef __ATA20__
    if (IsAutoMode())
    {
        atstr = "AT+SIM=1,1";
    }
    else
    {
        atstr = "AT+SIM=1,2";
    }
#else
    atstr = "AT+SIM";
#endif
    atstr += CR;
    test_handle->m_comm_handle->WriteCommand(atstr.c_str(), atstr.length(), ATTIMEOUT, false, atret);
    if (!GetATResponse(atret))
    {
        AddLog ("[SIM ERROR] %s response timeout.",atstr.c_str());
		test_handle->m_testCNF->sim_cnf.bPass = false;
        return E_ATADLL_RESULT_AT_RESPONSE_TIMEOUT;
    }
    test_handle->m_commonCFG.cbTestProgess (test_handle->handle_index, E_SIM, 70);

    if (NULL != strstr(atret.resultLst[0].eleLst[0].str_value.c_str(), "pass"))
    {
        AddLog ("[OK] SIM detected in target.");
        test_handle->m_testCNF->sim_cnf.bPass = true;
		testResultCode=E_ATADLL_RESULT_SUCCESS;
    }
	else if(NULL != strstr(atret.resultLst[0].eleLst[0].str_value.c_str(), "fail"))
	{
		AddLog ("[SIM ERROR] SIM test fail in target.");
        test_handle->m_testCNF->sim_cnf.bPass = false;
		testResultCode=E_ATADLL_RESULT_FAILED;
	}
	else if(NULL != strstr(atret.resultLst[0].eleLst[0].str_value.c_str(), "not test"))
	{
		AddLog ("[SIM ERROR] SIM not test in target.");
        test_handle->m_testCNF->sim_cnf.bPass = false;
		testResultCode=E_ATADLL_RESULT_NOT_TEST;
	}
	else if(NULL != strstr(atret.resultLst[0].eleLst[0].str_value.c_str(), "Cannot find"))
	{
		AddLog ("[SIM ERROR] SIM Test not support in target.");
		test_handle->m_testCNF->sim_cnf.bPass = false;
		testResultCode=E_ATADLL_RESULT_NOT_SUPPORT;
	}
    else
    {
    	AddLog ("[ERROR] SIM Test failed,error code not defined.");
        test_handle->m_testCNF->sim_cnf.bPass = false;
		testResultCode=E_ATADLL_RESULT_NULL;
    }

    test_handle->m_commonCFG.cbTestProgess (test_handle->handle_index, E_SIM, 100);
    return testResultCode;
}
