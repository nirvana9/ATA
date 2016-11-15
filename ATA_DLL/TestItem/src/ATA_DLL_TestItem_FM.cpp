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
 *   ATA_DLL_TestItem_FM.cpp
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
#include "..\header\ATA_DLL_TestItem_FM.h"

CTestItem_FM::CTestItem_FM(ATA_DLL_Handle *current_handle, int timeout, E_ATDLL_TEST_ITEM_COUNT item, char *testName)
    : CTestItem_Interface(current_handle, timeout, item, testName)
{
}

CTestItem_FM::~CTestItem_FM()
{
}

E_ATADLL_RESULT CTestItem_FM::StartTest ()
{
    E_ATADLL_RESULT ata_result = E_ATADLL_RESULT_SUCCESS;

    test_handle->m_commonCFG.cbTestProgess (test_handle->handle_index, E_FM, 10);
    ata_result = TestFM ();
    test_handle->m_commonCFG.cbTestProgess (test_handle->handle_index, E_FM, 100);

    return ata_result;
}

E_ATADLL_RESULT CTestItem_FM::TestFM ()
{
	E_ATADLL_RESULT testResultCode;
    ClearATResPool();
    test_handle->m_testCNF->fm_cnf.cqi = -999;

    char at_buff[128];

	// Set FM frequency
#ifdef __ATA20__
    sprintf (at_buff, "AT+PROPERTY=0,1,\"FMRadio.CH1,%d\"", test_handle->m_commonCFG.fm_freq);
#else
    sprintf (at_buff, "AT+FMRadio.CH1=%d", test_handle->m_commonCFG.fm_freq);
#endif
    string atstr = at_buff;
    atstr += CR;
    ATResult atret;

    test_handle->m_commonCFG.cbTestProgess (test_handle->handle_index, E_FM, 15);
    test_handle->m_comm_handle->WriteCommand(atstr.c_str(), atstr.length(), ATTIMEOUT, false, atret);

//#ifndef __ATA20__mingxue20150709
    if (!GetATResponse(atret))
    {
        AddLog ("[FM ERROR] Set frequency to %d fail due to AT response timeout.", test_handle->m_commonCFG.fm_freq);
        return E_ATADLL_RESULT_AT_RESPONSE_TIMEOUT;
    }
    if (strstr(atret.resultLst[0].eleLst[0].str_value.c_str(), "pass") != NULL)
    {
        AddLog ("[OK] Set frequency to %d success", test_handle->m_commonCFG.fm_freq);
    }
    else
    {
        AddLog ("[FM ERROR] Set frequency to %d fail", test_handle->m_commonCFG.fm_freq);
        return E_ATADLL_RESULT_FORMAT_INCORRECT;
    }
//#endif
    test_handle->m_commonCFG.cbTestProgess (test_handle->handle_index, E_FM, 30);

	// Set FM Play time
#ifdef __ATA30__
	#ifdef __ATA20__
    sprintf (at_buff, "AT+PROPERTY=0,1,\"FMRadio.PlayTime,%d\"", 3);
	#else
    sprintf (at_buff, "AT+BT.DeviceNumber=%d", 3);
	#endif
    atstr = at_buff;
    atstr += CR;
	test_handle->m_comm_handle->WriteCommand(atstr.c_str(), atstr.length(), ATTIMEOUT, false, atret);
	test_handle->m_commonCFG.cbTestProgess (test_handle->handle_index, E_FM, 40);
#endif

    ClearATResPool ();
#ifdef __ATA20__
    sprintf (at_buff, "AT+FM=1,1");
#else
    sprintf (at_buff, "AT+FM");
#endif
    atstr = at_buff;
    atstr += CR;
    test_handle->m_comm_handle->WriteCommand(atstr.c_str(), atstr.length(), ATTIMEOUT, false, atret);
    if (!GetATResponse(atret))
    {
        AddLog ("[FM ERROR] Test FM fail due to AT response timeout.");
        return E_ATADLL_RESULT_AT_RESPONSE_TIMEOUT;
    }
    test_handle->m_commonCFG.cbTestProgess (test_handle->handle_index, E_FM, 80);
    // Ignore test result pass or fail
    if (NULL != strstr(atret.resultLst[0].eleLst[0].str_value.c_str(), "pass"))
    {
    	AddLog ("[OK] FM test ok in target.");
		testResultCode=E_ATADLL_RESULT_SUCCESS;
    }
	else if(NULL != strstr(atret.resultLst[0].eleLst[0].str_value.c_str(), "fail"))
    {
        AddLog ("[FM ERROR] FM test fail in target.");
		testResultCode=E_ATADLL_RESULT_FAILED;
    }
	else if(NULL != strstr(atret.resultLst[0].eleLst[0].str_value.c_str(), "not test"))
	{
		AddLog ("[FM ERROR] FM not test in target.");
		testResultCode=E_ATADLL_RESULT_NOT_TEST;
	}
	else if(NULL != strstr(atret.resultLst[0].eleLst[0].str_value.c_str(), "Cannot find"))
	{
		AddLog ("[FM ERROR] FM not support in target.");
		testResultCode=E_ATADLL_RESULT_NOT_SUPPORT;
	}
	else
    {
        AddLog ("[FM ERROR] FM failed,error code not defined.");
		testResultCode=E_ATADLL_RESULT_NULL;
    }
    ClearATResPool ();

#ifndef __ATA20__
    sprintf (at_buff, "AT+REQUESTDATA");
    atstr = at_buff;
    atstr += CR;
    test_handle->m_comm_handle->WriteCommand(atstr.c_str(), atstr.length(), ATTIMEOUT, false, atret);
    if (!GetATResponse(atret))
    {
        AddLog ("[Error] Test FM fail due to AT response timeout.");
        return E_ATADLL_RESULT_AT_RESPONSE_TIMEOUT;
    }
    sp_ata_data s_data;
    GetATAData (s_data, atret);
    test_handle->m_commonCFG.cbTestProgess (test_handle->handle_index, E_FM, 90);
    if (s_data.fm.fm_rssi == 0)
    {
        test_handle->m_testCNF->fm_cnf.cqi = -500;
    }
    else
    {
        test_handle->m_testCNF->fm_cnf.cqi = s_data.fm.fm_rssi;
    }
    AddLog ("RSSI result: %d", test_handle->m_testCNF->fm_cnf.cqi);
#endif

    test_handle->m_commonCFG.cbTestProgess (test_handle->handle_index, E_FM, 100);
    return testResultCode;
}

