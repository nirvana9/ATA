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
 *   ATA_DLL_TestItem_SWVersion.cpp
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
#include "..\header\ATA_DLL_TestItem_SWVersion.h"

CTestItem_SWVersion::CTestItem_SWVersion(ATA_DLL_Handle *current_handle, int timeout, E_ATDLL_TEST_ITEM_COUNT item, char *testName)
    : CTestItem_Interface(current_handle, timeout, item, testName)
{
}

CTestItem_SWVersion::~CTestItem_SWVersion()
{
}


E_ATADLL_RESULT CTestItem_SWVersion::StartTest ()
{
    ClearATResPool();

    ATResult atret;
    string atstr = "";

#ifdef __ATA20__
    if (IsAutoMode())
    {
		//modify bug_id:none songjian 2016-3-1 (start)
        //atstr = "AT+VERSION=0,0";
        atstr = "AT+VERSION";
		//modify bug_id:none songjian 2016-3-1 (end)
    }
    else
    {
        atstr = "AT+VERSION=1,2";
    }
#else
    atstr = "AT+VERSION";
#endif
    atstr += CR;

    test_handle->m_commonCFG.cbTestProgess (test_handle->handle_index, E_SWVERSION, 10);
    test_handle->m_comm_handle->WriteCommand(atstr.c_str(), atstr.length(), 5000, false, atret);

#ifndef __ATA20__
    if (!GetATResponse(atret))
    {
        AddLog ("[VERSION ERROR] AT response timeout when getting the version.");
        return E_ATADLL_RESULT_AT_RESPONSE_TIMEOUT;
    }
    if (NULL != strstr(atret.resultLst[0].eleLst[0].str_value.c_str(), "pass"))
    {
        AddLog ("[OK] Version test ok.");
		//add bug_id:none songjian 2016-3-1 (start)			
		return E_ATADLL_RESULT_SUCCESS;
		//add bug_id:none songjian 2016-3-1 (end)
    }
    else
    {
        AddLog ("[ERROR] Version test fail.");
		return E_ATADLL_RESULT_FAILED;
    }
#endif
	//modify bug_id:none songjian 2016-3-1 (start)
    if (!GetATResponse(atret))
    {
        AddLog ("[VERSION ERROR] AT response timeout when getting the version.");
        return E_ATADLL_RESULT_AT_RESPONSE_TIMEOUT;
    }
    if (NULL != strstr(atret.resultLst[0].eleLst[0].str_value.c_str(), "pass"))
    {
        AddLog ("[OK] Version test ok.");
    	//test_handle->m_commonCFG.cbTestProgess (test_handle->handle_index, E_SWVERSION, 60);
		//test_handle->m_commonCFG.cbTestProgess (test_handle->handle_index, E_SWVERSION, 100);
		return E_ATADLL_RESULT_SUCCESS;
    }
    else
    {
        //AddLog ("[ERROR] Version test fail.");
		return E_ATADLL_RESULT_SUCCESS;
    }
	//modify bug_id:none songjian 2016-3-1 (end)

#ifndef __ATA20__
    // Get version number
    atstr = "AT+REQUESTDATA";
    atstr += CR;
    test_handle->m_comm_handle->WriteCommand(atstr.c_str(), atstr.length(), ATTIMEOUT, false, atret);
	Sleep (2000);
	test_handle->m_comm_handle->WriteCommand(atstr.c_str(), atstr.length(), ATTIMEOUT, false, atret);
	Sleep (2000);
	test_handle->m_comm_handle->WriteCommand(atstr.c_str(), atstr.length(), ATTIMEOUT, false, atret);
	Sleep (2000);

	ClearATResPool();
	test_handle->m_comm_handle->WriteCommand(atstr.c_str(), atstr.length(), ATTIMEOUT, false, atret);
    if (!GetATResponse(atret))
    {
        AddLog ("[Error] AT response timeout.");
        return E_ATADLL_RESULT_AT_RESPONSE_TIMEOUT;
    }

    sp_ata_data s_data;
    GetATAData (s_data, atret);

    char str1[62];
    sprintf (str1, "%d", sizeof(sp_ata_data));


    sprintf (test_handle->m_testCNF->sw_ver_cnf.modem_ver, s_data.version.modem_ver);
    sprintf (test_handle->m_testCNF->sw_ver_cnf.ap_ver, s_data.version.sw_ver);

    AddLog ("[Modem SW Version] %s", test_handle->m_testCNF->sw_ver_cnf.modem_ver);
    AddLog ("[AP SW Version] %s", test_handle->m_testCNF->sw_ver_cnf.ap_ver);

    char    swv_str[1024];
    int     m_iWholeCmdLen  = atret.resultLst[0].eleLst[0].str_value.length();
    memset (swv_str, 0x0, sizeof(char)*1024);
    memcpy (swv_str, atret.resultLst[0].eleLst[0].str_value.c_str(), m_iWholeCmdLen);
    swv_str[m_iWholeCmdLen] = 0;
	test_handle->m_commonCFG.cbTestProgess (test_handle->handle_index, E_SWVERSION, 100);
    return E_ATADLL_RESULT_SUCCESS;
#endif

    
}