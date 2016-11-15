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
 *   ATA_DLL_TestItem_BTS.cpp
 *
 * Project:
 * --------
 *   ATA_DLL
 *
 * Description:
 * ------------
 *   BTS Test
 *
 * Author:
 * -------
 *   Mingxue Sun
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
#include "..\header\ATA_DLL_TestItem_BTS.h"
//#include <string.h>

CTestItem_BTS::CTestItem_BTS(ATA_DLL_Handle *current_handle, int timeout, E_ATDLL_TEST_ITEM_COUNT item, char *testName)
    : CTestItem_Interface(current_handle, timeout, item, testName)
{
}

CTestItem_BTS::~CTestItem_BTS()
{
}


E_ATADLL_RESULT CTestItem_BTS::StartTest ()
{
	E_ATADLL_RESULT testResultCode;
    ClearATResPool();

    ATResult atret;
    string atstr = "";

#ifdef __ATA30__
	atstr = "AT+BTS";
#else
	#ifdef __ATA20__
	    if (IsAutoMode())
	    {
	        atstr = "AT+BTS=1,1";
	    }
	    else
	    {
	        atstr = "AT+BTS=1,2";
	    }
	#else
	    atstr = "AT+BTS";
	#endif
#endif
    atstr += CR;

    test_handle->m_commonCFG.cbTestProgess (test_handle->handle_index, E_BTS, 10);
    test_handle->m_comm_handle->WriteCommand(atstr.c_str(), atstr.length(), 500, false, atret);

	Sleep (3000);

	atstr = "AT+BTS=STOP";
	atstr += CR;
	test_handle->m_comm_handle->WriteCommand(atstr.c_str(), atstr.length(), ATTIMEOUT, false, atret);

    if (!GetATResponse(atret))
    {
        AddLog ("[BTS ERROR] %s response timeout.",atstr.c_str());
		test_handle->m_testCNF->bBTSOK = false;
		test_handle->m_commonCFG.cbTestProgess (test_handle->handle_index, E_BTS, 100);
        return E_ATADLL_RESULT_AT_RESPONSE_TIMEOUT;
    }
    /*if (NULL != strstr(atret.resultLst[0].eleLst[0].str_value.c_str(), "pass"))
    {
        AddLog ("[OK] BTS test ok.");
        test_handle->m_testCNF->bBTSOK = true;
		testResultCode=E_ATADLL_RESULT_SUCCESS;
    }
    else if(NULL != strstr(atret.resultLst[0].eleLst[0].str_value.c_str(), "fail"))
    {
        AddLog ("[BTS ERROR] BTS test fail.");
        test_handle->m_testCNF->bBTSOK = false;
		testResultCode=E_ATADLL_RESULT_FAILED;
    }
	else if(NULL != strstr(atret.resultLst[0].eleLst[0].str_value.c_str(), "not test"))
	{
		AddLog ("[BTS ERROR] BTS not test.");
		test_handle->m_testCNF->bBTSOK = false;
		testResultCode=E_ATADLL_RESULT_NOT_TEST;
	}*/
	else if(NULL != strstr(atret.resultLst[0].eleLst[0].str_value.c_str(), "Cannot find"))
	{
		AddLog ("[BTS ERROR] BTS Not support.");
		test_handle->m_testCNF->bBTSOK = false;
		testResultCode=E_ATADLL_RESULT_NOT_SUPPORT;
	}
/*	else
	{
        AddLog ("[BTS ERROR] BTS failed,error code not defined.");
        test_handle->m_testCNF->bBTSOK = false;
		testResultCode=E_ATADLL_RESULT_NULL;
    }*/
	else
	{
		test_handle->m_testCNF->bBTSOK=true;
		testResultCode= E_ATADLL_RESULT_SUCCESS;
		AddLog ("[OK] BTS test stop measure temperature.");
	}

    test_handle->m_commonCFG.cbTestProgess (test_handle->handle_index, E_BTS, 100);
    return testResultCode;
}


