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
 *   ATA_DLL_TestItem_HWVer.cpp
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
#include "..\header\ATA_DLL_TestItem_HWVer.h"

CTestItem_HWVer::CTestItem_HWVer(ATA_DLL_Handle *current_handle, int timeout, E_ATDLL_TEST_ITEM_COUNT item, char *testName)
    : CTestItem_Interface(current_handle, timeout, item, testName)
{
}

CTestItem_HWVer::~CTestItem_HWVer()
{
}


E_ATADLL_RESULT CTestItem_HWVer::StartTest ()
{
    ClearATResPool();

    test_handle->m_commonCFG.cbTestProgess (test_handle->handle_index, E_HWVERSIOM, 10);

    if (E_ATADLL_RESULT_SUCCESS != TestLCD ())
    {
        return E_ATADLL_RESULT_FAILED;
    }

    if (E_ATADLL_RESULT_SUCCESS != TestBacklight ())
    {
        return E_ATADLL_RESULT_FAILED;
    }

    test_handle->m_commonCFG.cbTestProgess (test_handle->handle_index, E_HWVERSIOM, 100);

    return E_ATADLL_RESULT_SUCCESS;
}

E_ATADLL_RESULT CTestItem_HWVer::TestLCD ()
{
    ClearATResPool ();

    ATResult atret;
    string atstr = "AT+LCD";
    atstr += CR;

    bool result = true;

    AddLog ("[LCD Test]");

    // Enter LCD test item
    test_handle->m_comm_handle->WriteCommand(atstr.c_str(), atstr.length(), 1000, false, atret);
    test_handle->m_commonCFG.cbTestProgess (test_handle->handle_index, E_HWVERSIOM, 20);
    Sleep (1000);
    AddLog ("Enter LCD test item.");

    // switch LCD color in sequence ( red, green, blue, white, black)
    for (int i = 0; i < 5; i ++)
    {
        result = test_handle->m_comm_handle->WriteCommand(atstr.c_str(), atstr.length(), ATTIMEOUT, false, atret);
        if (!result)
        {
            AddLog ("Switch to color %d fail.", i+1);
            goto Error;
        }
        test_handle->m_commonCFG.cbTestProgess (test_handle->handle_index, E_HWVERSIOM, 25+i*5);
        Sleep (1500);
    }

    atstr = "AT+LCD=STOP";
    atstr += CR;
    test_handle->m_comm_handle->WriteCommand(atstr.c_str(), atstr.length(), 1000, false, atret);
    test_handle->m_commonCFG.cbTestProgess (test_handle->handle_index, E_HWVERSIOM, 50);

    return E_ATADLL_RESULT_SUCCESS;

Error:
    atstr = "AT+LCD=STOP";
    atstr += CR;
    test_handle->m_comm_handle->WriteCommand(atstr.c_str(), atstr.length(), 1000, false, atret);
    return E_ATADLL_RESULT_FAILED;
}

E_ATADLL_RESULT CTestItem_HWVer::TestBacklight()
{
    ClearATResPool ();

    ATResult atret;
    string atstr = "AT+BACKLIGHT";
    atstr += CR;

    bool result = true;

    AddLog ("[Backlight test]");

    // Enter LCD test item
    test_handle->m_comm_handle->WriteCommand(atstr.c_str(), atstr.length(), 1000, false, atret);
    test_handle->m_commonCFG.cbTestProgess (test_handle->handle_index, E_HWVERSIOM, 60);
    Sleep (1000);

    // switch LCD color in sequence ( red, green, blue, white, black)
    for (int i = 0; i < 5; i ++)
    {
        result = test_handle->m_comm_handle->WriteCommand(atstr.c_str(), atstr.length(), ATTIMEOUT, false, atret);
        if (!result)
        {
            AddLog ("Switch to backlight level %d fail.", i+1);
            goto Error;
        }
        Sleep (1000);

        test_handle->m_commonCFG.cbTestProgess (test_handle->handle_index, E_HWVERSIOM, 65+i*5);
    }

    atstr = "AT+BACKLIGHT=STOP";
    atstr += CR;
    test_handle->m_comm_handle->WriteCommand(atstr.c_str(), atstr.length(), 1000, false, atret);

    return E_ATADLL_RESULT_SUCCESS;

Error:
    atstr = "AT+BACKLIGHT=STOP";
    atstr += CR;
    test_handle->m_comm_handle->WriteCommand(atstr.c_str(), atstr.length(), 1000, false, atret);
    return E_ATADLL_RESULT_FAILED;
}




