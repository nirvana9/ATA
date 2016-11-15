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
 *   ATA_DLL_TestItem_ER_State.cpp
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
#include "..\header\ATA_DLL_TestItem_ER_State.h"
//#include <string.h>

CTestItem_ER_State::CTestItem_ER_State(ATA_DLL_Handle *current_handle, int timeout, E_ATDLL_TEST_ITEM_COUNT item, char *testName)
    : CTestItem_Interface(current_handle, timeout, item, testName)
{
}

CTestItem_ER_State::~CTestItem_ER_State()
{
}


E_ATADLL_RESULT CTestItem_ER_State::StartTest ()
{
    ClearATResPool();

    ATResult atret;
    string atstr = "AT+ERPTST";
    atstr += CR;
    test_handle->m_comm_handle->WriteCommand(atstr.c_str(), atstr.length(), ATTIMEOUT, false, atret);
    if (!GetATResponse(atret))
    {
        AddLog ("[Error] Get AT response timeout.");
        return E_ATADLL_RESULT_AT_RESPONSE_TIMEOUT;
    }

    char    er_str[128];
    int     m_iWholeCmdLen  = atret.resultLst[0].eleLst[0].str_value.length();
    int     m_iFlagLen      = strlen("+ERPTST:");
    memset (er_str, 0x0, sizeof(char)*128);
    memcpy (er_str, atret.resultLst[0].eleLst[0].str_value.c_str(), m_iWholeCmdLen);
    er_str[atret.resultLst[0].eleLst[0].str_value.length()] = 0;

    if (m_iFlagLen < m_iWholeCmdLen)
    {
        char *er_str_inter = er_str + m_iFlagLen;
        test_handle->m_testCNF->er_state = atoi(er_str_inter);
        AddLog ("ER State : %d.", atoi(er_str_inter));
    }
    else
    {
        AddLog ("[ERROR] AT response format incorrect.");
        return E_ATADLL_RESULT_FORMAT_INCORRECT;
    }

    return E_ATADLL_RESULT_SUCCESS;
}

// state: 0 - disconnect; 1 - connect
bool CTestItem_ER_State::SetHeadsetState (int state)
{
    ATResult atret;
    char cmdStr[128];
    sprintf (cmdStr, 0x0, sizeof(char)*128);
    sprintf (cmdStr, "Headset=%d\r\n", state);
    string atstr = cmdStr;

    test_handle->m_pm_comm_handle->SetNeedWithCRLR (true);
    bool result = test_handle->m_pm_comm_handle->WriteCommand(atstr.c_str(), atstr.length(), 3000, false, atret);
    test_handle->m_pm_comm_handle->SetNeedWithCRLR (false);
    Sleep (100);

    sprintf (cmdStr, 0x0, sizeof(char)*128);
    if (state == 0)
    {
        sprintf (cmdStr, "HeadsetOff:OK");
    }
    else
    {
        sprintf (cmdStr, "HeadsetOn:OK");
    }

    AddLog ("External status: %s", atret.resultLst[0].eleLst[1].str_value.c_str());
    if (result && strstr (atret.resultLst[0].eleLst[1].str_value.c_str(), cmdStr) != NULL)
    {
        return true;
    }
    else
    {
        return false;
    }

    return true;
}

