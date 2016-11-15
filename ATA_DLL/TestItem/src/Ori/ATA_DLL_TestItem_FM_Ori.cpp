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

    ata_result = OpenFM();
    if (E_ATADLL_RESULT_SUCCESS != ata_result)
    {
        goto Error;
    }

    ata_result = TestFM();
    if (E_ATADLL_RESULT_SUCCESS != ata_result)
    {
        goto Error;
    }

    ata_result = CloseFM();
    if (E_ATADLL_RESULT_SUCCESS != ata_result)
    {
        goto Error;
    }

    return E_ATADLL_RESULT_SUCCESS;

Error:
    CloseFM();
    return ata_result;
}

E_ATADLL_RESULT CTestItem_FM::OpenFM ()
{
    ClearATResPool();

    ATResult atret;
    string atstr = "AT+EMFM=1";
    atstr += CR;
    test_handle->m_comm_handle->WriteCommand(atstr.c_str(), atstr.length(), ATTIMEOUT, false, atret);
    if (!GetATResponse(atret))
    {
        AddLog ("[Error] Open FM timeout.");
        return E_ATADLL_RESULT_AT_RESPONSE_TIMEOUT;
    }

    char    fm_str[1024];
    int     m_iWholeCmdLen  = atret.resultLst[0].eleLst[0].str_value.length();
    int     m_iFlagLen      = strlen("+EMFM:");
    memset (fm_str, 0x0, sizeof(char)*1024);
    memcpy (fm_str, atret.resultLst[0].eleLst[0].str_value.c_str(), m_iWholeCmdLen);

    char *fm_flag  = strstr (fm_str, "+EMFM:");
    if (fm_flag != NULL)
    {
        memcpy (test_handle->m_testCNF->fm_cnf.fm_chip, fm_str + m_iFlagLen, m_iWholeCmdLen - m_iFlagLen);
        test_handle->m_testCNF->fm_cnf.fm_chip[m_iWholeCmdLen - m_iFlagLen] = '\0';

        AddLog ("[OK] Open FM successfully: %s", test_handle->m_testCNF->fm_cnf.fm_chip);
    }
    else
    {
        AddLog ("[Error] Open FM fail due to response format incorrect.");
        return E_ATADLL_RESULT_FORMAT_INCORRECT;
    }

    return E_ATADLL_RESULT_SUCCESS;
}

E_ATADLL_RESULT CTestItem_FM::CloseFM ()
{
    ClearATResPool();

    ATResult atret;
    string atstr = "AT+EMFM=0";
    atstr += CR;
    test_handle->m_comm_handle->WriteCommand(atstr.c_str(), atstr.length(), ATTIMEOUT, false, atret);

    AddLog ("[OK] Close FM successfully.");
    return E_ATADLL_RESULT_SUCCESS;
}

E_ATADLL_RESULT CTestItem_FM::TestFM ()
{
    ClearATResPool();

    char at_buff[32];
    sprintf (at_buff, "AT+EMFM=2,%d", test_handle->m_commonCFG.fm_freq);
    string atstr = at_buff;
    atstr += CR;
    ATResult atret;
    test_handle->m_comm_handle->WriteCommand(atstr.c_str(), atstr.length(), ATTIMEOUT, false, atret);
    if (!GetATResponse(atret))
    {
        AddLog ("[Error] Test FM fail due to AT response timeout.");
        return E_ATADLL_RESULT_AT_RESPONSE_TIMEOUT;
    }

    char    fm_str[1024];
    int     m_iWholeCmdLen  = atret.resultLst[0].eleLst[0].str_value.length();
    int     m_iFlagLen      = strlen("+EMFM:");
    memset (fm_str, 0x0, sizeof(char)*1024);
    memcpy (fm_str, atret.resultLst[0].eleLst[0].str_value.c_str(), m_iWholeCmdLen);

    char *fm_flag = strstr(fm_str, "+EMFM:");
    if (fm_flag != NULL)
    {
        char *freq_flag = strstr(fm_str+m_iFlagLen, ",");
        char freq_str[16];
        memcpy (freq_str, fm_str + m_iFlagLen, freq_flag - fm_str - m_iFlagLen);
        freq_str[freq_flag - fm_str - m_iFlagLen] = '\0';
        test_handle->m_testCNF->fm_cnf.freq = atoi(freq_str);

        char cqi_str[16];
        memcpy (cqi_str, freq_flag + 1, m_iWholeCmdLen - (freq_flag - fm_str - 1));
        cqi_str[m_iWholeCmdLen - (freq_flag - fm_str - 1)] = '\0';
        test_handle->m_testCNF->fm_cnf.cqi = atoi(cqi_str);

        AddLog ("[OK] Test FM ok. Frequency(%s), CQI(%s)", freq_str, cqi_str);
    }
    else
    {
        AddLog ("[Error] Test FM fail due to response format incorrect.");
        return E_ATADLL_RESULT_FORMAT_INCORRECT;
    }

    return E_ATADLL_RESULT_SUCCESS;
}

