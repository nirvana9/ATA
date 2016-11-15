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

CTestItem_FM::CTestItem_FM(ATA_DLL_Handle *current_handle, int timeout, char *testName)
    : CTestItem_Interface(current_handle, timeout, testName)
{
}

CTestItem_FM::~CTestItem_FM()
{
}

E_ATADLL_RESULT CTestItem_FM::StartTest ()
{
    E_ATADLL_RESULT ata_result = E_ATADLL_RESULT_SUCCESS;
    test_handle->m_commonCFG.cbTestProgess (test_handle->handle_index, E_FM, 10);

    ata_result = OpenFM();
    if (E_ATADLL_RESULT_SUCCESS != ata_result)
    {
        goto Error;
    }
    Sleep (500);
    test_handle->m_commonCFG.cbTestProgess (test_handle->handle_index, E_FM, 30);

    ata_result = TestFM();
    if (E_ATADLL_RESULT_SUCCESS != ata_result)
    {
        goto Error;
    }
    test_handle->m_commonCFG.cbTestProgess (test_handle->handle_index, E_FM, 80);

    ata_result = CloseFM();
    if (E_ATADLL_RESULT_SUCCESS != ata_result)
    {
        goto Error;
    }
    Sleep (500);
    test_handle->m_commonCFG.cbTestProgess (test_handle->handle_index, E_FM, 100);

    return E_ATADLL_RESULT_SUCCESS;

Error:
    CloseFM();
    return ata_result;
}

E_ATADLL_RESULT CTestItem_FM::OpenFM ()
{
    ClearATResPool();
    glb_at_resp_flag = true;

    ATResult atret;
    string atstr = "AT+EGCMD=7";
    atstr += CR;
    test_handle->m_comm_handle->WriteCommand(atstr.c_str(), atstr.length(), ATTIMEOUT, false, atret);

    /**
    if (!GetATResponse(atret))
    {
        AddLog ("[Error] Get at response timeout when opening FM.");
        glb_at_resp_flag = false;
    	return E_ATADLL_RESULT_AT_RESPONSE_TIMEOUT;
    }

    char    fm_str[128];
    int     m_iWholeCmdLen  = atret.resultLst[0].eleLst[0].str_value.length();
    memset (fm_str, 0x0, sizeof(char)*128);
    memcpy (fm_str, atret.resultLst[0].eleLst[0].str_value.c_str(), m_iWholeCmdLen);
    fm_str[m_iWholeCmdLen] = 0;
    if (strcmp (fm_str, "OK") == 0) {
        AddLog ("[OK] Open FM.");
    } else {
        AddLog ("[Fail] Open FM.");
        glb_at_resp_flag = false;
        return E_ATADLL_RESULT_FORMAT_INCORRECT;
    }
    **/

    AddLog ("Open FM.");
    glb_at_resp_flag = false;
    return E_ATADLL_RESULT_SUCCESS;
}

E_ATADLL_RESULT CTestItem_FM::CloseFM ()
{
    ClearATResPool();
    glb_at_resp_flag = true;

    ATResult atret;
    string atstr = "AT+EGCMD=8";
    atstr += CR;
    test_handle->m_comm_handle->WriteCommand(atstr.c_str(), atstr.length(), ATTIMEOUT, false, atret);
    /**
    if (!GetATResponse(atret))
    {
        AddLog ("[Error] Get at response timeout when closing FM.");
        glb_at_resp_flag = false;
    	return E_ATADLL_RESULT_AT_RESPONSE_TIMEOUT;
    }

    char    fm_str[128];
    int     m_iWholeCmdLen  = atret.resultLst[0].eleLst[0].str_value.length();
    memset (fm_str, 0x0, sizeof(char)*128);
    memcpy (fm_str, atret.resultLst[0].eleLst[0].str_value.c_str(), m_iWholeCmdLen);
    fm_str[m_iWholeCmdLen] = 0;
    if (strcmp (fm_str, "OK") == 0) {
        AddLog ("[OK] Close FM.");
    } else {
        AddLog ("[Fail] Close FM.");
        glb_at_resp_flag = false;
        return E_ATADLL_RESULT_FORMAT_INCORRECT;
    }
    **/

    AddLog ("Close FM.");
    glb_at_resp_flag = false;
    return E_ATADLL_RESULT_SUCCESS;
}

E_ATADLL_RESULT CTestItem_FM::TestFM ()
{
    ClearATResPool();

    char at_buff[32];
    sprintf (at_buff, "AT+EGCMD=9,2,\"%04x\"", test_handle->m_commonCFG.fm_freq);
    string atstr = at_buff;
    atstr += CR;
    ATResult atret;
    test_handle->m_comm_handle->WriteCommand(atstr.c_str(), atstr.length(), ATTIMEOUT, false, atret);
    AddLog ("Set Freq to %d.", test_handle->m_commonCFG.fm_freq);

    Sleep (500);

    sprintf (at_buff, "AT+EGCMD=10,3,\"03%04x\"", test_handle->m_commonCFG.fm_freq);
    atstr = at_buff;
    atstr += CR;
    test_handle->m_comm_handle->WriteCommand(atstr.c_str(), atstr.length(), ATTIMEOUT, false, atret);
    if (!GetATResponse(atret))
    {
        AddLog ("[Error] Test FM fail due to AT response timeout.");
        return E_ATADLL_RESULT_AT_RESPONSE_TIMEOUT;
    }

    char    fm_str[1024];
    int     m_iWholeCmdLen  = atret.resultLst[0].eleLst[0].str_value.length();
    int     m_iFlagLen      = strlen("CSQ:");
    memset (fm_str, 0x0, sizeof(char)*1024);
    memcpy (fm_str, atret.resultLst[0].eleLst[0].str_value.c_str(), m_iWholeCmdLen);
    fm_str[m_iWholeCmdLen] = 0;

    char *fm_flag = strstr(fm_str, "CSQ:");
    if (fm_flag != NULL)
    {
        char csq_str[32];
        memcpy (csq_str, fm_str + m_iFlagLen, m_iWholeCmdLen - m_iFlagLen);
        csq_str[m_iWholeCmdLen - m_iFlagLen] = '\0';

        // Remove blank space
        char    csq_str_ex[32];
        int     csq_index = 0;
        for (int index = 0; index < m_iWholeCmdLen - m_iFlagLen; index ++)
        {
            if (csq_str[index] != ' ')
            {
                csq_str_ex[csq_index++] = csq_str[index];
            }
        }
        csq_str_ex[csq_index] = '\0';
        test_handle->m_testCNF->fm_cnf.cqi = atoi(csq_str_ex);
        test_handle->m_testCNF->fm_cnf.freq = test_handle->m_commonCFG.fm_freq;

        AddLog ("[OK] Test FM ok. Frequency(%d), CSQ(%s)", test_handle->m_commonCFG.fm_freq, csq_str_ex);
    }
    else
    {
        AddLog ("[Error] Test FM fail due to response format incorrect.");
        return E_ATADLL_RESULT_FORMAT_INCORRECT;
    }

    return E_ATADLL_RESULT_SUCCESS;
}

