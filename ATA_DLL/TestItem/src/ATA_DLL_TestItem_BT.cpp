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
 *   ATA_DLL_TestItem_BT.cpp
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
#include "..\header\ATA_DLL_TestItem_BT.h"

CTestItem_BT::CTestItem_BT(ATA_DLL_Handle *current_handle, int timeout, E_ATDLL_TEST_ITEM_COUNT item, char *testName)
    : CTestItem_Interface(current_handle, timeout, item, testName)
{
}

CTestItem_BT::~CTestItem_BT()
{
}

E_ATADLL_RESULT CTestItem_BT::StartTest ()
{
    return TestBT ();
}

E_ATADLL_RESULT CTestItem_BT::OpenBT ()
{
    ClearATResPool();

    //Enter flight mode
    if (!SwitchToMode (4))
    {
        return E_ATADLL_RESULT_FAILED;
    }

    ATResult atret;
    string atstr;

    ClearATResPool();
    atstr = "AT+EBTAT=bt_power_on";
    atstr += CR;
    int timeout = timeout_sec;
    timeout_sec = 5;
    test_handle->m_comm_handle->WriteCommand(atstr.c_str(), atstr.length(), 5000, false, atret);
    if (!GetATResponse(atret))
    {
        AddLog ("[Error] Open BT timeout.");
        return E_ATADLL_RESULT_AT_RESPONSE_TIMEOUT;
    }
    timeout_sec = timeout;

    char    bt_str[1024];
    int     m_iWholeCmdLen  = atret.resultLst[0].eleLst[0].str_value.length();
    memset (bt_str, 0x0, sizeof(char)*1024);
    memcpy (bt_str, atret.resultLst[0].eleLst[0].str_value.c_str(), m_iWholeCmdLen);

    if (strstr (bt_str, "+EBTAT:") != NULL &&
            strstr (bt_str, "power on success") != NULL)
    {
        AddLog ("[OK] Open BT Succussfully.");
    }
    else
    {
        AddLog ("[Error] Open BT fail due to incorrect return format.");
        return E_ATADLL_RESULT_FORMAT_INCORRECT;
    }

    return E_ATADLL_RESULT_SUCCESS;
}

E_ATADLL_RESULT CTestItem_BT::CloseBT ()
{
    ClearATResPool();

    ATResult atret;
    string atstr = "AT+EBTAT=bt_power_off";
    atstr += CR;
    int timeout = timeout_sec;
    timeout_sec = 5;
    test_handle->m_comm_handle->WriteCommand(atstr.c_str(), atstr.length(), 5000, false, atret);
    GetATResponse(atret);
    timeout_sec = timeout;

    AddLog ("[OK] Close BT Succussfully.");

    // enter normal mode
    if (!SwitchToMode (1))
    {
        return E_ATADLL_RESULT_FAILED;
    }
    return E_ATADLL_RESULT_SUCCESS;
}

E_ATADLL_RESULT CTestItem_BT::TestBT ()
{
	E_ATADLL_RESULT testResultCode;
    ClearATResPool();
    test_handle->m_commonCFG.cbTestProgess (test_handle->handle_index, E_BT, 30);

    ATResult atret;
    string atstr = "";

#ifdef __ATA30__
	char at_buff[128];
	memset (at_buff, 0x0, sizeof(char)*128);
    sprintf (at_buff, "AT+PROPERTY=0,1,\"BT.DeviceNumber,3\"");
    atstr = at_buff;
    atstr += CR;
    test_handle->m_comm_handle->WriteCommand(atstr.c_str(), atstr.length(), ATTIMEOUT, false, atret);

	/*mingxue20150708*/
	if (!GetATResponse(atret))
    {
        AddLog ("[BT ERROR] %s response timeout.",atstr.c_str());
    }
	if (NULL != strstr(atret.resultLst[0].eleLst[0].str_value.c_str(), "pass"))
	{
		AddLog ("[OK] %s set ok.",atstr.c_str());
	}
	else if (NULL != strstr(atret.resultLst[0].eleLst[0].str_value.c_str(), "fail"))
	{
		AddLog ("[BT ERROR] %s set fail.",atstr.c_str());
	}

	/*end*/

    ClearATResPool ();
	test_handle->m_commonCFG.cbTestProgess (test_handle->handle_index, E_BT, 40);
#endif
	

#ifdef __ATA20__
    if (IsAutoMode())
    {
		if (IsBackground())
		{
        	atstr = "AT+BT=1,3";
		}
		else
		{
			atstr = "AT+BT=1,1";
		}
    }
    else
    {
        atstr = "AT+BT=1,2";
    }
#else
    atstr = "AT+BT";
#endif

    atstr += CR;
    test_handle->m_comm_handle->WriteCommand(atstr.c_str(), atstr.length(), ATTIMEOUT, false, atret);
    if (!GetATResponse(atret))
    {
         AddLog ("[BT ERROR] AT response timeout: %d.", timeout_sec);
		 test_handle->m_testCNF->bt_cnf.bPass = false;
        return E_ATADLL_RESULT_AT_RESPONSE_TIMEOUT;
    }
    test_handle->m_commonCFG.cbTestProgess (test_handle->handle_index, E_BT, 60);
    if (NULL != strstr(atret.resultLst[0].eleLst[0].str_value.c_str(), "pass"))
    {
        test_handle->m_testCNF->bt_cnf.bPass = true;
        AddLog ("[OK] BT test pass from target.");
		testResultCode=E_ATADLL_RESULT_SUCCESS;
    }
	else if(NULL != strstr(atret.resultLst[0].eleLst[0].str_value.c_str(), "fail"))
	{
		 test_handle->m_testCNF->bt_cnf.bPass = false;
         AddLog ("[BT ERROR] BT test fail from target.");
		 testResultCode=E_ATADLL_RESULT_FAILED;	
	}
	else if(NULL != strstr(atret.resultLst[0].eleLst[0].str_value.c_str(), "not test"))
	{
		 test_handle->m_testCNF->bt_cnf.bPass = false;
         AddLog ("[BT ERROR] BT not test in target.");
		 testResultCode=E_ATADLL_RESULT_NOT_TEST;
	}
	else if(NULL != strstr(atret.resultLst[0].eleLst[0].str_value.c_str(), "Cannot find"))
	{
		 test_handle->m_testCNF->bt_cnf.bPass = false;
         AddLog ("[BT ERROR] BT not support in target.");
		 testResultCode=E_ATADLL_RESULT_NOT_SUPPORT;
	}
    else
    {
   		AddLog ("[BT ERROR] BT failed,error code not defined.");
        test_handle->m_testCNF->bt_cnf.bPass = false;
		testResultCode=E_ATADLL_RESULT_NULL;    
    }
    test_handle->m_commonCFG.cbTestProgess (test_handle->handle_index, E_BT, 80);

#ifndef __ATA20__
    atstr = "AT+REQUESTDATA";
    atstr += CR;
    test_handle->m_comm_handle->WriteCommand(atstr.c_str(), atstr.length(), ATTIMEOUT, false, atret);
    if (!GetATResponse(atret))
    {
        AddLog ("[Error] AT response timeout.");
        return E_ATADLL_RESULT_AT_RESPONSE_TIMEOUT;
    }
    test_handle->m_commonCFG.cbTestProgess (test_handle->handle_index, E_BT, 90);

    sp_ata_data s_data;
    GetATAData (s_data, atret);
    for (int index = 0; index < s_data.bt.num; index ++)
    {
        AddLog ("%s, %s, %d", s_data.bt.bt[index].bt_mac, s_data.bt.bt[index].bt_name, s_data.bt.bt[index].bt_rssi);
    }
	testResultCode=E_ATADLL_RESULT_SUCCESS;
#endif

/*zishuo20150727*/
	if (test_handle->m_commonCFG.bt_cfg.bBtAddrCheck)
	{
		E_ATADLL_RESULT ata_result;
		AddLog ("Start to read bt address.");
		ata_result = ReadBTaddr (test_handle->m_testCNF->bt_cnf.bt_addr, test_handle->m_testCNF->bt_cnf.bt_addr_len);
		if (E_ATADLL_RESULT_SUCCESS == ata_result)
		{
			AddLog ("Read BT  address OK: %s", test_handle->m_testCNF->bt_cnf.bt_addr);
		}
		else
		{
			AddLog ("[BT ERROR]Read BT address fail.");
			test_handle->m_commonCFG.cbTestProgess (test_handle->handle_index, E_BT, 100);
			return ata_result;
		}
		AddLog ("[OK]Read BT address finished.");
	}
/*end*/

    test_handle->m_commonCFG.cbTestProgess (test_handle->handle_index, E_BT, 100);

    return testResultCode;
}

bool CTestItem_BT::ParseBTDeviceList (char *bt_str, unsigned int len)
{
    char *bt_str_inter = bt_str;
    char *bt_flag = NULL;
    char temp_str[128];
    unsigned int total_len = 0;
    S_ATADLL_BT_DEVICE bt;
    memset (&bt, 0x0, sizeof(S_ATADLL_BT_DEVICE));

    //+EBTAT:Addr:78dd,08,c15474,name:MTK-95C5CD662CF,rssi:aa,170

    //Address
    bt_flag = strstr (bt_str_inter, ",name");
    if (NULL != bt_flag)
    {
        memcpy (temp_str, bt_str_inter, bt_flag - bt_str_inter);
        temp_str[bt_flag - bt_str_inter] = '\0';
        if (strstr(temp_str, "Addr:") != NULL)
        {
            char *temp_p = bt_str_inter + strlen("Addr:");
            memcpy (bt.addr, temp_p, bt_flag - bt_str_inter - strlen("Addr:"));
            bt.addr[bt_flag - bt_str_inter - strlen("Addr:")] = '\0';
        }
        else
        {
            return false;
        }

        total_len = total_len + (bt_flag - bt_str_inter);
        if (total_len < len)
            bt_str_inter = bt_flag + 1;
        else
            return false;
    }
    else
    {
        return false;
    }

    //name
    bt_flag = strstr (bt_str_inter, ",rssi");
    if (NULL != bt_flag)
    {
        memcpy (temp_str, bt_str_inter, bt_flag - bt_str_inter);
        temp_str[bt_flag - bt_str_inter] = '\0';
        if (strstr(temp_str, "name:") != NULL)
        {
            char *temp_p = bt_str_inter + strlen("name:");
            memcpy (bt.device_name, temp_p, bt_flag - bt_str_inter - strlen("name:"));
            bt.device_name[bt_flag - bt_str_inter - strlen("name:")] = '\0';
        }
        else
        {
            return false;
        }

        total_len = total_len + (bt_flag - bt_str_inter);
        if (total_len < len)
            bt_str_inter = bt_flag + 1;
        else
            return false;
    }
    else
    {
        return false;
    }

    //rssi
    memcpy (temp_str, bt_str_inter, strlen(bt_str_inter));
    {
        char *temp_p = bt_str_inter + strlen("rssi:");
        char rssi[16];
        memcpy (rssi, temp_p, strlen(bt_str_inter) - strlen("rssi:"));
        rssi[strlen(bt_str_inter) - strlen("rssi:")] = '\0';
        bt.rssi = atoi(rssi);
    }


    // Add bt into bt device list
    test_handle->m_testCNF->bt_cnf.device_list[test_handle->m_testCNF->bt_cnf.device_list_num] = bt;
    test_handle->m_testCNF->bt_cnf.device_list_num ++;

    return true;

}

/*zishuo20150727*/
E_ATADLL_RESULT CTestItem_BT::ReadBTaddr (char *btAddrStr, unsigned int &len)
{
	ClearATResPool ();

	ATResult atret;
	string atstr = "AT+READBTADDR";
    atstr += CR;

    test_handle->m_comm_handle->WriteCommand(atstr.c_str(), atstr.length(), ATTIMEOUT, false, atret);
    if (!GetATResponse(atret))
    {
        AddLog ("[BT ERROR] AT response timeout while reading bt address from target.");
        return E_ATADLL_RESULT_AT_RESPONSE_TIMEOUT;
    }

	char    btAddr_str[1024];
    int     m_iWholeCmdLen = atret.resultLst[0].eleLst[0].str_value.length();
    memset (btAddr_str, 0x0, sizeof(char)*1024);
    memcpy (btAddr_str, atret.resultLst[0].eleLst[0].str_value.c_str(), m_iWholeCmdLen);
    btAddr_str[m_iWholeCmdLen] = '\0';

    char *pFlag = strstr (btAddr_str, ":");
    if (pFlag == NULL)
    {
        memcpy (btAddrStr, btAddr_str, m_iWholeCmdLen);
        btAddrStr[m_iWholeCmdLen] = 0;
		len = m_iWholeCmdLen;
    }
    else
    {
        memcpy (btAddrStr, pFlag+1, m_iWholeCmdLen-(pFlag-btAddr_str)-1);
        btAddrStr[m_iWholeCmdLen-(pFlag-btAddr_str)-1] = 0;
		len = m_iWholeCmdLen-(pFlag-btAddr_str)-1;
    }
	
	if(NULL!= strstr(btAddrStr,"fail"))
	{
			 AddLog ("[BT ERROR] Get the response from target is : fail while reading bt address from target.");
			return E_ATADLL_RESULT_FAILED;
	}
	
	return E_ATADLL_RESULT_SUCCESS;
}
/*end*/
