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
 *   ATA_DLL_TestItem_GPS.cpp
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
#include "..\header\ATA_DLL_TestItem_GPS.h"
#include <string.h>

CTestItem_GPS::CTestItem_GPS(ATA_DLL_Handle *current_handle, int timeout, E_ATDLL_TEST_ITEM_COUNT item, char *testName)
    : CTestItem_Interface(current_handle, timeout, item, testName)
{
}

CTestItem_GPS::~CTestItem_GPS()
{
}


E_ATADLL_RESULT CTestItem_GPS::StartTest ()
{
	E_ATADLL_RESULT testResultCode;
    string atstr = "";
#ifdef __ATA20__
    if (IsAutoMode())
    {
		if (IsBackground())
		{
        	atstr = "AT+GPS=1,3";
		}
		else
		{
			atstr = "AT+GPS=1,1";
		}
    }
    else
    {
        atstr = "AT+GPS=1,2";
    }
#else
    atstr = "AT+GPS";
#endif
    atstr += CR;

    ATResult atret;
    memset (&atret, 0x0, sizeof(ATResult));
    test_handle->m_commonCFG.cbTestProgess (test_handle->handle_index, E_GPS, 10);

    ClearATResPool();
    test_handle->m_comm_handle->WriteCommand(atstr.c_str(), atstr.length(), ATTIMEOUT, false, atret);
    test_handle->m_commonCFG.cbTestProgess (test_handle->handle_index, E_GPS, 30);
    if (!GetATResponse(atret))
    {
        AddLog ("[GPS ERROR] %s response timeout.",atstr.c_str());
		test_handle->m_testCNF->gps_cnf.bPass = false;
        return E_ATADLL_RESULT_AT_RESPONSE_TIMEOUT;
    }
    test_handle->m_commonCFG.cbTestProgess (test_handle->handle_index, E_GPS, 70);

    if (NULL != strstr(atret.resultLst[0].eleLst[0].str_value.c_str(), "pass"))
    {
        test_handle->m_testCNF->gps_cnf.bPass = true;
        AddLog ("[OK] GPS test pass.");
		testResultCode=E_ATADLL_RESULT_SUCCESS;
    }
    else if(NULL != strstr(atret.resultLst[0].eleLst[0].str_value.c_str(), "fail"))
    {
        test_handle->m_testCNF->gps_cnf.bPass = false;
        AddLog ("[GPS ERROR] GPS test fail from target.");
		testResultCode=E_ATADLL_RESULT_FAILED;
    }
	else if(NULL != strstr(atret.resultLst[0].eleLst[0].str_value.c_str(), "not test"))
	{
		AddLog ("[GPS ERROR] GPS not test in target.");
		test_handle->m_testCNF->gps_cnf.bPass = false;
		testResultCode=E_ATADLL_RESULT_NOT_TEST;
	}
	else if(NULL != strstr(atret.resultLst[0].eleLst[0].str_value.c_str(), "Cannot find"))
	{
		AddLog ("[GPS ERROR] GPS Not support in target.");
		test_handle->m_testCNF->gps_cnf.bPass = false;
		testResultCode=E_ATADLL_RESULT_NOT_SUPPORT;
	}
	else
    {
        AddLog ("[GPS ERROR] GPS failed,error code not defined.");
        test_handle->m_testCNF->gps_cnf.bPass = false;
		testResultCode=E_ATADLL_RESULT_NULL;
    }
    test_handle->m_commonCFG.cbTestProgess (test_handle->handle_index, E_GPS, 100);

    return testResultCode;
}


//add bug_id:none songjian 2016-3-1 (start)
CTestItem_GPSCali::CTestItem_GPSCali(ATA_DLL_Handle *current_handle, int timeout, E_ATDLL_TEST_ITEM_COUNT item, char *testName)
    : CTestItem_Interface(current_handle, timeout, item, testName)
{
}

CTestItem_GPSCali::~CTestItem_GPSCali()
{
}

E_ATADLL_RESULT CTestItem_GPSCali::StartTest ()
{
    return TestGPSCali ();
}

E_ATADLL_RESULT CTestItem_GPSCali::TestGPSCali ()
{
    ClearATResPool();

    ATResult atret;
    string atstr = "";

#ifdef __ATA20__
    if (IsAutoMode())
    {
        atstr = "AT+GPSCALI";
    }
    else
    {
        atstr = "AT+GPSCALI";
    }
#else
    atstr = "AT+GPSCALI";
#endif
    atstr += CR;

    test_handle->m_commonCFG.cbTestProgess (test_handle->handle_index, E_GPS_CALI, 10);
    test_handle->m_comm_handle->WriteCommand(atstr.c_str(), atstr.length(), 5000, false, atret);

#ifndef __ATA20__
    if (!GetATResponse(atret))
    {
        AddLog ("[GPSCali ERROR] AT response timeout when getting the GPSCali.");
        return E_ATADLL_RESULT_AT_RESPONSE_TIMEOUT;
    }
    if (NULL != strstr(atret.resultLst[0].eleLst[0].str_value.c_str(), "pass"))
    {
        AddLog ("[OK] GPSCali test ok.");			
		return E_ATADLL_RESULT_SUCCESS;
    }
    else
    {
        AddLog ("[ERROR] GPSCali test fail.");
		return E_ATADLL_RESULT_FAILED;
    }
#endif
    if (!GetATResponse(atret))
    {
        AddLog ("[GPSCali ERROR] AT response timeout when getting the GPSCali.");
		test_handle->m_testCNF->bGPSCALIOK = false;
        return E_ATADLL_RESULT_AT_RESPONSE_TIMEOUT;
    }
	AddLog ("[GPSCali ERROR] sss==== %s\n", atret.resultLst[0].eleLst[0].str_value.c_str());
    if (NULL != strstr(atret.resultLst[0].eleLst[0].str_value.c_str(), "pass"))
    {
        AddLog ("[OK] GPSCali test ok.");
		test_handle->m_testCNF->bGPSCALIOK = true;
		return E_ATADLL_RESULT_SUCCESS;
    }
	else if (NULL != strstr(atret.resultLst[0].eleLst[0].str_value.c_str(), "not support"))
	{
		AddLog ("[ERROR] GPSCali not support.");
		test_handle->m_testCNF->bGPSCALIOK = false;
		return E_ATADLL_RESULT_NOT_SUPPORT;
	}
    else
    {
        AddLog ("[ERROR] GPSCali test fail.");
		test_handle->m_testCNF->bGPSCALIOK = false;
		return E_ATADLL_RESULT_FAILED;
    }

#ifndef __ATA20__
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
	test_handle->m_commonCFG.cbTestProgess (test_handle->handle_index, E_GPS_CALI, 100);
    return E_ATADLL_RESULT_SUCCESS;
#endif
}
//add bug_id:none songjian 2016-3-1 (end)

