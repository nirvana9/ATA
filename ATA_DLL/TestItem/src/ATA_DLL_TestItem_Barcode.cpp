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
 *   ATA_DLL_TestItem_Barcode.cpp
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
#include "..\header\ATA_DLL_TestItem_Barcode.h"

CTestItem_Barcode::CTestItem_Barcode(ATA_DLL_Handle *current_handle, int timeout, E_ATDLL_TEST_ITEM_COUNT item, char *testName)
    : CTestItem_Interface(current_handle, timeout, item, testName)
{
}

CTestItem_Barcode::~CTestItem_Barcode()
{
}

E_ATADLL_RESULT CTestItem_Barcode::StartTest ()
{
	E_ATADLL_RESULT item_test_result;

    ClearATResPool();
    test_handle->m_commonCFG.cbTestProgess (test_handle->handle_index, E_BARCODE, 10);
	item_test_result=ReadBarcode (test_handle->m_testCNF->barcode);

    if (E_ATADLL_RESULT_SUCCESS == item_test_result)
    {
        AddLog ("Barcode: %s", test_handle->m_testCNF->barcode);
		//add bug_id:none songjian 2016-3-1 (start)
		if(strstr(test_handle->m_testCNF->barcode,"10p") == NULL &&
			strstr(test_handle->m_testCNF->barcode,"10P") == NULL)
		{
			item_test_result = E_ATADLL_RESULT_FAILED;
			goto Error;
		}
		//add bug_id:none songjian 2016-3-1 (end)
    }
    else
    {
        AddLog ("Read barcode fail!");
        goto Error;
    }
	if (test_handle->m_commonCFG.barcodeFlag_cfg.bSetFlag)
    {
        test_handle->m_commonCFG.cbTestProgess (test_handle->handle_index, E_BARCODE, 70);
    }
    else
	{
		test_handle->m_commonCFG.cbTestProgess (test_handle->handle_index, E_BARCODE, 100);
	}
    return E_ATADLL_RESULT_SUCCESS;

Error:
    test_handle->m_commonCFG.cbTestProgess (test_handle->handle_index, E_BARCODE, 100);
    return item_test_result;
}

E_ATADLL_RESULT CTestItem_Barcode::UpdateBarcodeFlag (bool bPass)
{
    ClearATResPool();

    if (bPass)
    {
        AddLog ("Update ATA test flag (%s) into barcode[%d].",
                test_handle->m_commonCFG.barcodeFlag_cfg.passFlag,
                test_handle->m_commonCFG.barcodeFlag_cfg.flagIndex);
    }
    else
    {
        AddLog ("Update ATA test flag (%s) into barcode[%d].",
                test_handle->m_commonCFG.barcodeFlag_cfg.failFlag,
                test_handle->m_commonCFG.barcodeFlag_cfg.flagIndex);
    }

    // Read original barcode
    char barcode[128];
    E_ATADLL_RESULT ata_result =  ReadBarcode (barcode);
    if (ata_result == E_ATADLL_RESULT_SUCCESS)
    {
        AddLog ("Read original barcode ok: %s", barcode);
    }
    else
    {
        AddLog ("Read original barcode fail.");
        return E_ATADLL_RESULT_FAILED;
    }
	
	int barcodeLen = strlen(barcode);
    for (int index = strlen(barcode); index < 128; index ++)
    {
        barcode[index] = ' ';
    }
    // Update ATA test flag into barcode
    if (bPass)
    {
        barcode[test_handle->m_commonCFG.barcodeFlag_cfg.flagIndex] = test_handle->m_commonCFG.barcodeFlag_cfg.passFlag[0];
    }
    else
    {
        barcode[test_handle->m_commonCFG.barcodeFlag_cfg.flagIndex] = test_handle->m_commonCFG.barcodeFlag_cfg.failFlag[0];
    }
    if (test_handle->m_commonCFG.barcodeFlag_cfg.flagIndex > barcodeLen-1)
    {
        barcode[test_handle->m_commonCFG.barcodeFlag_cfg.flagIndex+1] = 0;
    }
    else
    {
        barcode[barcodeLen] = 0;
    }
    AddLog ("Write new barcode: %s", barcode);

    // Write new updated barcode into target
    ata_result = WriteBarcode (barcode);
    if (ata_result == E_ATADLL_RESULT_SUCCESS)
    {
        AddLog ("Write new barcode ok.");
    }
    else
    {
        AddLog ("Write new barcode fail.");
    }

    Sleep (500);

    // Read barcode again to check if write successfully
    AddLog ("Read barcode again to check if write succesfully.");
    char barcode_new[128];
    ata_result = ReadBarcode (barcode_new);
    if (E_ATADLL_RESULT_SUCCESS == ata_result)
    {
        AddLog ("New readed barcode is: %s", barcode_new);
        if (strcmp (barcode, barcode_new) == 0)
        {
            AddLog ("Check ok!");
        }
        else
        {
            AddLog ("Check fail!");
            return E_ATADLL_RESULT_FAILED;
        }
    }
    else
    {
        AddLog ("Read new barcode fail!");
        return E_ATADLL_RESULT_FAILED;
    }
    memcpy (test_handle->m_testCNF->barcode, barcode_new, strlen(barcode_new));
    test_handle->m_testCNF->barcode[strlen(barcode_new)] = 0;;

    return E_ATADLL_RESULT_SUCCESS;
}


E_ATADLL_RESULT CTestItem_Barcode::ReadBarcode (char *barcode)
{
    ClearATResPool ();

    ATResult atret;
    string atstr = "";

#ifdef __ATA20__
    atstr = "AT+READBARCODE";
#else
    atstr = "AT+READBARCODE";
#endif
    atstr += CR;

    test_handle->m_comm_handle->WriteCommand(atstr.c_str(), atstr.length(), ATTIMEOUT, false, atret);
    if (!GetATResponse(atret))
    {
        AddLog ("[ERROR] Get AT response timeout when reading barcode.");
        return E_ATADLL_RESULT_AT_RESPONSE_TIMEOUT;
    }

    char    barcode_str[1024];
    int     m_iWholeCmdLen = atret.resultLst[0].eleLst[0].str_value.length();
    memset (barcode_str, 0x0, sizeof(char)*1024);
    memcpy (barcode_str, atret.resultLst[0].eleLst[0].str_value.c_str(), m_iWholeCmdLen);
    barcode_str[m_iWholeCmdLen] = '\0';

    char *pFlag = strstr (barcode_str, ":");
    if (pFlag == NULL)
    {
        memcpy (barcode, barcode_str, m_iWholeCmdLen);
        barcode[m_iWholeCmdLen] = 0;
    }
    else
    {
        memcpy (barcode, pFlag+1, m_iWholeCmdLen-(pFlag-barcode_str)-1);
        barcode[m_iWholeCmdLen-(pFlag-barcode_str)-1] = 0;
    }

    return E_ATADLL_RESULT_SUCCESS;
}

E_ATADLL_RESULT CTestItem_Barcode::WriteBarcode (char *barcode)
{
    ClearATResPool ();

    ATResult atret;
    string atstr = "";

#ifdef __ATA20__
    atstr = "AT+BARCODE=0,1,\"";
#else
    atstr = "AT+WRITEBARCODE=";
#endif

    atstr += barcode;

#ifdef __ATA20__
	atstr += "\"";
#endif

    atstr += CR;

    AddLog ("Wirte command to target: %s", atstr.c_str());
    test_handle->m_comm_handle->WriteCommand(atstr.c_str(), atstr.length(), ATTIMEOUT, false, atret);
    if (!GetATResponse(atret))
    {
        AddLog ("[Error] Get AT response timeout.");
        return E_ATADLL_RESULT_AT_RESPONSE_TIMEOUT;
    }

    if (NULL != strstr(atret.resultLst[0].eleLst[0].str_value.c_str(), "pass"))
    {
        return E_ATADLL_RESULT_SUCCESS;
    }
    else
    {
        return E_ATADLL_RESULT_FAILED;
    }

}
