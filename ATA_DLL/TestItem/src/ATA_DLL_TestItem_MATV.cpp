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
 *   ATA_DLL_TestItem_MATV.cpp
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
#include "..\header\ATA_DLL_TestItem_MATV.h"

CTestItem_MATV::CTestItem_MATV(ATA_DLL_Handle *current_handle, int timeout, E_ATDLL_TEST_ITEM_COUNT item, char *testName)
    : CTestItem_Interface(current_handle, timeout, item, testName)
{
}

CTestItem_MATV::~CTestItem_MATV()
{
}

E_ATADLL_RESULT CTestItem_MATV::StartTest ()
{
    ClearATResPool();
    test_handle->m_commonCFG.cbTestProgess (test_handle->handle_index, E_TV, 10);

    char    matv_str[1024];
    int     m_iWholeCmdLen = 0;
    int     m_iFlagLen = strlen("+EMATV:");

    char    atStr[128];
    sprintf (atStr, "AT+EMATV=%ld,%d", test_handle->m_commonCFG.matv_freq, test_handle->m_commonCFG.matv_country);
    string atstr = atStr;
    atstr += CR;
    ATResult atret;

    test_handle->m_comm_handle->WriteCommand(atstr.c_str(), atstr.length(), ATTIMEOUT, false, atret);
    test_handle->m_commonCFG.cbTestProgess (test_handle->handle_index, E_TV, 30);

    // mATV Chip
    if (!GetATResponse(atret))
    {
        test_handle->m_comm_handle->WriteCommand(atstr.c_str(), atstr.length(), ATTIMEOUT, false, atret);
        if (!GetATResponse(atret))
        {
            AddLog ("[Error] Get AT response timeout when opening mATV.");
            return E_ATADLL_RESULT_AT_RESPONSE_TIMEOUT;
        }
    }
    test_handle->m_commonCFG.cbTestProgess (test_handle->handle_index, E_TV, 60);
    m_iWholeCmdLen  = atret.resultLst[0].eleLst[0].str_value.length();
    memset (matv_str, 0x0, sizeof(char)*1024);
    memcpy (matv_str, atret.resultLst[0].eleLst[0].str_value.c_str(), m_iWholeCmdLen);
    matv_str[m_iWholeCmdLen] = '\0';
    char matv_chip[64];
    memcpy (matv_chip, matv_str + m_iFlagLen, m_iWholeCmdLen - m_iFlagLen);
    matv_chip[m_iWholeCmdLen - m_iFlagLen] = '\0';
    sprintf (test_handle->m_testCNF->matv_cnf.matv_chip, "%s", matv_chip);

    AddLog ("[OK] Open mATV successfully: %s", matv_chip);

    // Lock Status
    if (!GetATResponse(atret))
    {
        AddLog ("[Error] Get AT response timeout when getting lock status.");
        return E_ATADLL_RESULT_AT_RESPONSE_TIMEOUT;
    }
    test_handle->m_commonCFG.cbTestProgess (test_handle->handle_index, E_TV, 80);
    m_iWholeCmdLen  = atret.resultLst[0].eleLst[0].str_value.length();
    memset (matv_str, 0x0, sizeof(char)*1024);
    memcpy (matv_str, atret.resultLst[0].eleLst[0].str_value.c_str(), m_iWholeCmdLen);
    matv_str[m_iWholeCmdLen] = '\0';
    if (NULL != strstr(matv_str, "lock ok"))
    {
        AddLog ("[OK] Lock frequency %d, coutnry %d.",
                test_handle->m_commonCFG.matv_freq,
                test_handle->m_commonCFG.matv_country);
        test_handle->m_testCNF->matv_cnf.freq_lock_status = true;
    }
    else
    {
        //retry
        /**
        ClearATResPool();
        test_handle->m_comm_handle->WriteCommand(atstr.c_str(), atstr.length(), ATTIMEOUT, false, atret);
        if (GetATResponse(atret))
        {
        	if (GetATResponse(atret))
        	{
        		m_iWholeCmdLen  = atret.resultLst[0].eleLst[0].str_value.length();
        	    memset (matv_str, 0x0, sizeof(char)*1024);
        		memcpy (matv_str, atret.resultLst[0].eleLst[0].str_value.c_str(), m_iWholeCmdLen);
        	    matv_str[m_iWholeCmdLen] = '\0';

        		if (NULL != strstr(matv_str, "lock ok"))
        		{
        	        AddLog ("[OK] Lock frequency %d, coutnry %d.",
        	                test_handle->m_commonCFG.matv_freq,
        	                test_handle->m_commonCFG.matv_country);
        	        test_handle->m_testCNF->matv_cnf.freq_lock_status = true;

        			goto Test_End;
        	    }
        	}
        }
        **/

        AddLog ("[Fail] Lock frequency %d, coutnry %d.",
                test_handle->m_commonCFG.matv_freq,
                test_handle->m_commonCFG.matv_country);
        test_handle->m_testCNF->matv_cnf.freq_lock_status = false;
    }

    test_handle->m_testCNF->matv_cnf.freq       = test_handle->m_commonCFG.matv_freq;
    test_handle->m_testCNF->matv_cnf.country    = test_handle->m_commonCFG.matv_country;

    test_handle->m_commonCFG.cbTestProgess (test_handle->handle_index, E_TV, 100);

    if (test_handle->m_testCNF->matv_cnf.freq_lock_status == false)
        return E_ATADLL_RESULT_FAILED;
    else
        return E_ATADLL_RESULT_SUCCESS;
}


