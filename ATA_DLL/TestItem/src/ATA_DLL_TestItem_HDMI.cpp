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
 *   ATA_DLL_TestItem_HDMI.cpp
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
#include "..\header\ATA_DLL_TestItem_HDMI.h"

CTestItem_HDMI::CTestItem_HDMI(ATA_DLL_Handle *current_handle, int timeout, E_ATDLL_TEST_ITEM_COUNT item, char *testName)
    : CTestItem_Interface(current_handle, timeout, item, testName)
{
}

CTestItem_HDMI::~CTestItem_HDMI()
{
}

E_ATADLL_RESULT CTestItem_HDMI::StartTest ()
{
    E_ATADLL_RESULT ata_result = E_ATADLL_RESULT_SUCCESS;
    ata_result = TestHDMI ();

    return ata_result;
}

E_ATADLL_RESULT CTestItem_HDMI::TestHDMI ()
{
	E_ATADLL_RESULT testResultCode=E_ATADLL_RESULT_SUCCESS;
    ClearATResPool();
	E_ATADLL_RESULT ata_result = E_ATADLL_RESULT_SUCCESS;

    ATResult atret;
    string atstr = "";

#ifdef __ATA20__
    if (IsAutoMode())
    {
		if (IsBackground())
		{
        	atstr = "AT+HDMI=1,3";
		}
		else
		{
			atstr = "AT+HDMI=1,1";
		}
    }
    else
    {
        atstr = "AT+HDMI=1,2";
    }
#else
    atstr = "AT+HDMI";
#endif
    atstr += CR;

    test_handle->m_commonCFG.cbTestProgess (test_handle->handle_index, test_item, 10);
    test_handle->m_comm_handle->WriteCommand(atstr.c_str(), atstr.length(), ATTIMEOUT, false, atret);

	if (test_item == E_MHL)
	{
		Sleep(3000);
	}
	else
	{
		int respTime = 0;
		test_handle->m_testCNF->hdmi_cnf.bResult = true;
		do 
		{
			if (!GetATResponse(atret))
		    {
		        AddLog ("[HDMI ERROR] AT response timeout when getting HDMI status.");
				test_handle->m_commonCFG.cbTestProgess (test_handle->handle_index, test_item, 100);
				test_handle->m_testCNF->hdmi_cnf.bResult = false;
		        return E_ATADLL_RESULT_AT_RESPONSE_TIMEOUT;
		    }

			if (strstr(atret.resultLst[0].eleLst[0].str_value.c_str(), "plug in") != NULL)
			{
				AddLog ("HDMI Cable plug in.");
			}
			else if (strstr(atret.resultLst[0].eleLst[0].str_value.c_str(), "plug out") != NULL)
			{
				AddLog ("HDMI Cable plug out.");
			}
			else if (strstr(atret.resultLst[0].eleLst[0].str_value.c_str(), "EDID check fail") != NULL)
			{
				AddLog ("[HDMI ERROR]HDMI EDID check fail.");

				test_handle->m_testCNF->hdmi_cnf.bResult = false;

				testResultCode=E_ATADLL_RESULT_FAILED;
				break;
			}
			else if (strstr(atret.resultLst[0].eleLst[0].str_value.c_str(), "HDCP KEY NOT FOUND") != NULL)
			{
				AddLog ("[HDMI ERROR] HDCP KEY not found.");
				test_handle->m_testCNF->hdmi_cnf.bResult = false;
				testResultCode=E_ATADLL_RESULT_HDMI_NO_KEY;
			}
			else if (strstr(atret.resultLst[0].eleLst[0].str_value.c_str(), "HDCP KEY FOUND") != NULL)
			{
				AddLog ("HDCP KEY found.");
			}
			else if (strstr(atret.resultLst[0].eleLst[0].str_value.c_str(), "pass") != NULL)
			{
				AddLog ("HDMI Test finished.");
				test_handle->m_testCNF->hdmi_cnf.bResult = true;
				testResultCode=E_ATADLL_RESULT_SUCCESS;
				break;
			}
			else if (strstr(atret.resultLst[0].eleLst[0].str_value.c_str(), "fail") != NULL)
			{
				AddLog ("[ERROR]HDMI Test fail.");
				test_handle->m_testCNF->hdmi_cnf.bResult = false;
				testResultCode=E_ATADLL_RESULT_FAILED;
				break;
			}
			else
			{
				AddLog ("[ERROR]Unrecognized return!!");
				test_handle->m_testCNF->hdmi_cnf.bResult = false;
				testResultCode=E_ATADLL_RESULT_NULL;
				break;
			}

			respTime ++;
			test_handle->m_commonCFG.cbTestProgess (test_handle->handle_index, test_item, (respTime*25+10)>90?90:(respTime*25+10));
		} while (1);
	}
    
    test_handle->m_commonCFG.cbTestProgess (test_handle->handle_index, test_item, 100);

    return testResultCode;
}
