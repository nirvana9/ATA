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
 *   ATA_DLL_TestItem_Audio.cpp
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
#include "..\header\ATA_DLL_TestItem_Audio.h"
#include "..\..\GlobalOBJ.h"

//#define __QA30_VERSION__

CTestItem_Audio::CTestItem_Audio(ATA_DLL_Handle *current_handle, int timeout, E_ATDLL_TEST_ITEM_COUNT item, char *testName)
    : CTestItem_Interface(current_handle, timeout, item, testName)
{
}

CTestItem_Audio::~CTestItem_Audio()
{
}

E_ATADLL_RESULT CTestItem_Audio::StartTest ()
{
    E_ATADLL_RESULT ata_result = E_ATADLL_RESULT_SUCCESS;
    E_ATADLL_RESULT ata_result_final = E_ATADLL_RESULT_SUCCESS;

    ata_result = SendCommandPM ("Headset=1\r\n", ""/*"HeadsetOn:OK"*/);
    if (E_ATADLL_RESULT_SUCCESS == ata_result)
    {
        AddLog ("[TestBox][OK] Headset on ok.");
    }
    else
    {
        //AddLog ("[Warning] Headset on fail.");
        //return ata_result;
    }

    Sleep (100);

    ata_result = SendCommandPM ("MICSel=0\r\n", ""/*"MICSel0:OK"*/);
    if (E_ATADLL_RESULT_SUCCESS == ata_result)
    {
        AddLog ("[TestBox][OK] Micsel 0 ok.");
    }
    else
    {
        //AddLog ("[Warning] Micsel 0 fail.");
        //return ata_result;
    }

    Sleep (100);

    if (E_SPEAKER == test_item)
    {
        test_handle->m_commonCFG.cbTestProgess (test_handle->handle_index, E_SPEAKER, 20);
#ifdef __ATA30__
		ata_result = TestAudio_ATA30 (0);
#else
        ata_result = TestAudio (0);
#endif
        if (E_ATADLL_RESULT_SUCCESS != ata_result)
        {
            ata_result_final = ata_result;
        }
        test_handle->m_testCNF->test_result[E_SPEAKER] = ata_result;
        test_handle->m_commonCFG.cbTestProgess (test_handle->handle_index, E_SPEAKER, 100);
    }

    if (E_RECEIVER == test_item)
    {
        test_handle->m_commonCFG.cbTestProgess (test_handle->handle_index, E_RECEIVER, 20);
#ifdef __ATA30__
		ata_result = TestAudio_ATA30 (1);
#else
		ata_result = TestAudio (1);
#endif
        if (E_ATADLL_RESULT_SUCCESS != ata_result)
        {
            ata_result_final = ata_result;
        }
        test_handle->m_testCNF->test_result[E_RECEIVER] = ata_result;
        test_handle->m_commonCFG.cbTestProgess (test_handle->handle_index, E_RECEIVER, 100);
    }

    if (E_EARTHPHONE == test_item)
    {
        test_handle->m_commonCFG.cbTestProgess (test_handle->handle_index, E_EARTHPHONE, 20);
#ifdef __ATA30__
		ata_result = TestAudio_ATA30 (2);
#else
		ata_result = TestAudio (2);
#endif
        if (E_ATADLL_RESULT_SUCCESS != ata_result)
        {
            ata_result_final = ata_result;
        }
        test_handle->m_testCNF->test_result[E_EARTHPHONE] = ata_result;
        test_handle->m_commonCFG.cbTestProgess (test_handle->handle_index, E_EARTHPHONE, 100);
    }

#ifndef __ATA20__
    string atstr = "AT+REQUESTDATA";
    ATResult atret;
    atstr += CR;

    ClearATResPool();
    test_handle->m_comm_handle->WriteCommand(atstr.c_str(), atstr.length(), ATTIMEOUT, false, atret);
    if (!GetATResponse(atret))
    {
        AddLog ("[Error] AT response timeout: %d.", timeout_sec);
        return E_ATADLL_RESULT_AT_RESPONSE_TIMEOUT;
    }

    sp_ata_data s_data;
    GetATAData (s_data, atret);

    if (test_item == E_RECEIVER)
    {
        test_handle->m_testCNF->audio_cnf.receiver_data = s_data.receiver;
        AddLog ("Receiver:\r\n\t left freq: %d, right freq: %d\r\n\t left amp: %d, right amp: %d",
                s_data.receiver.freqL, s_data.receiver.freqR, s_data.receiver.amplL, s_data.receiver.amplR);
    }

    if (test_item == E_SPEAKER)
    {
        test_handle->m_testCNF->audio_cnf.speaker_data = s_data.speaker;
        AddLog ("Speaker:\r\n\t left freq: %d, right freq: %d\r\n\t left amp: %d, right amp: %d",
                s_data.speaker.freqL, s_data.speaker.freqR, s_data.speaker.amplL, s_data.speaker.amplR);
    }

    if (test_item == E_EARTHPHONE)
    {
        test_handle->m_testCNF->audio_cnf.headset_data = s_data.headset;
        AddLog ("Headset:\r\n\t left freq: %d, right freq: %d\r\n\t left amp: %d, right amp: %d",
                s_data.headset.freqL, s_data.headset.freqR, s_data.headset.amplL, s_data.headset.amplR);
    }

#endif

    return ata_result_final;
}


E_ATADLL_RESULT CTestItem_Audio::TestAudio (char index)
{
    char testItemName[64];
    string atstr = "";
    ATResult atret;


#if 0
	char at_buff[512];
    sprintf (at_buff, "AT+PROPERTY=0,1,\"Audio.ManualTest,0\"");
	atstr = at_buff;
	atstr += CR;
	test_handle->m_comm_handle->WriteCommand(atstr.c_str(), atstr.length(), 100, false, atret);
	GetATResponse(atret);
#endif


    if (index == 0)
    {
        sprintf (testItemName, "speaker");
	#ifdef __ATA20__
        if (IsAutoMode())
        {
            atstr = "AT+RINGTONE=1,1";
        }
        else
        {
            atstr = "AT+RINGTONE=1,2";
        }
	#else
        atstr = "AT+RINGTONE";
	#endif
    }
    else if (index == 1)
    {
        sprintf (testItemName, "receiver");

	#ifdef __ATA20__
        if (IsAutoMode())
        {
            atstr = "AT+RECEIVER=1,1";
        }
        else
        {
            atstr = "AT+RECEIVER=1,2";
        }
	#else
        atstr = "AT+RECEIVER";
	#endif
    }
    else if (index == 2)
    {
        sprintf (testItemName, "headset");
		
	#ifdef __ATA20__
        if (IsAutoMode())
        {
            atstr = "AT+HEADSET=1,1";
        }
        else
        {
            atstr = "AT+HEADSET=1,2";
        }
	#else
        atstr = "AT+HEADSET";
	#endif
    }
    else
    {
        sprintf (testItemName, "Audio");
    }
    atstr += CR;

    ClearATResPool();
    test_handle->m_comm_handle->WriteCommand(atstr.c_str(), atstr.length(), ATTIMEOUT, false, atret);
    if (!GetATResponse(atret))
    {
        AddLog ("[%s ERROR] AT response timeout: %d.", testItemName,timeout_sec);
        return E_ATADLL_RESULT_AT_RESPONSE_TIMEOUT;
    }

	AddLog ("[%s] test finished.", testItemName);

	/**
    bool result = false;
    if (NULL != strstr(atret.resultLst[0].eleLst[0].str_value.c_str(), "pass"))
    {
        result = true;
        AddLog ("[OK] %s test pass.", testItemName);
    }
    else
    {
        AddLog ("[Error] %s test fail.", testItemName);
    }

    if (index == 0)
    {
        test_handle->m_testCNF->audio_cnf.bSpeakerPass = result;
    }
    else if (index == 1)
    {
        test_handle->m_testCNF->audio_cnf.bReceiverPass = result;
    }
    else if (index == 2)
    {
        test_handle->m_testCNF->audio_cnf.bHeadsetPass = result;
    }
    **/

    return E_ATADLL_RESULT_SUCCESS;
}

#ifdef __ATA30__
E_ATADLL_RESULT CTestItem_Audio::TestAudio_ATA30 (char index)
{
	E_ATADLL_RESULT ata_result = E_ATADLL_RESULT_SUCCESS;

#ifdef __QA30_VERSION__
	ata_result = TestAudio_Manual_ATA30 (index);
#else
	if (IsAutoMode())
	{
		ata_result = TestAudio_Auto_ATA30 (index);
	}
	else
	{
		ata_result = TestAudio_Manual_ATA30 (index);
	}
#endif
	
	return ata_result;
}

E_ATADLL_RESULT CTestItem_Audio::TestAudio_Auto_ATA30 (char index)
{
    char testItemName[64];
    string atstr = "";
	string atstr_prop = "";
    ATResult atret;

	char popupInfo[256] = {0};

    if (index == 0)
    {
        sprintf (testItemName, "speaker");
        atstr = "AT+RINGTONE=1,1";

    }
    else if (index == 1)
    {
        sprintf (testItemName, "receiver");
        atstr = "AT+RECEIVER=1,1";
    }
    else if (index == 2)
    {
        sprintf (testItemName, "headset");
        atstr = "AT+HEADSET=1,1";

    }
    else
    {
        sprintf (testItemName, "Audio");
		atstr = "AT+RINGTONE=1,1";
    }
    atstr += CR;
	ClearATResPool ();

	sprintf (popupInfo, "[DUT%d][%s] Press to continue to test common part.", test_handle->handle_index+1, testItemName);
	if (IDYES != ::AfxMessageBox (popupInfo, 0, 1))

	// Phone level common test
	AddLog ("[%s][Common test] start.", testItemName);
	atstr_prop = "AT+PROPERTY=0,1,\"Audio.Auto.OutputType,1\"";
	atstr_prop += CR;
	test_handle->m_comm_handle->WriteCommand(atstr_prop.c_str(), atstr_prop.length(), ATTIMEOUT, false, atret);
	Sleep (300);
	ClearATResPool ();
	test_handle->m_comm_handle->WriteCommand(atstr.c_str(), atstr.length(), ATTIMEOUT, false, atret);
	if (!GetATResponse(atret))
	{
        AddLog ("[%s][Common Test] AT response timeout: %d.", testItemName, timeout_sec);
        return E_ATADLL_RESULT_AT_RESPONSE_TIMEOUT;
    }
	if (NULL != strstr(atret.resultLst[0].eleLst[0].str_value.c_str(), "pass") ||
		NULL != strstr(atret.resultLst[0].eleLst[0].str_value.c_str(), "fail"))
    {
        AddLog ("[%s][Common Test] Finished.", testItemName);
    }
    else
    {
        AddLog ("[%s][Common Test] Response not corrent.", testItemName);
		return E_ATADLL_RESULT_FAILED;
    }
	test_handle->m_commonCFG.cbTestProgess (test_handle->handle_index, test_item, 40);


if (index != 2)  // Headset not support Freq response & THD
{
	sprintf (popupInfo, "[DUT%d][%s] Press to continue test Freq Response.", test_handle->handle_index+1, testItemName);
	if (IDYES != ::AfxMessageBox (popupInfo, 0, 1))

	// Phone level freq response
	AddLog ("[%s][Freq Response] start.", testItemName);
	atstr_prop = "AT+PROPERTY=0,1,\"Audio.Auto.OutputType,2\"";
	atstr_prop += CR;
	test_handle->m_comm_handle->WriteCommand(atstr_prop.c_str(), atstr_prop.length(), ATTIMEOUT, false, atret);
	Sleep (300);
	ClearATResPool ();
	test_handle->m_comm_handle->WriteCommand(atstr.c_str(), atstr.length(), ATTIMEOUT, false, atret);
	if (!GetATResponse(atret))
	{
        AddLog ("[%s][Freq Response] AT response timeout: %d.", testItemName, timeout_sec);
        return E_ATADLL_RESULT_AT_RESPONSE_TIMEOUT;
    }
	if (NULL != strstr(atret.resultLst[0].eleLst[0].str_value.c_str(), "pass") ||
		NULL != strstr(atret.resultLst[0].eleLst[0].str_value.c_str(), "fail"))
    {
        AddLog ("[%s][Freq Response] Finished.", testItemName);
    }
    else
    {
        AddLog ("[%s][Freq Response] Response not corrent.", testItemName);
		return E_ATADLL_RESULT_FAILED;
    }
	test_handle->m_commonCFG.cbTestProgess (test_handle->handle_index, test_item, 80);
}

	sprintf (popupInfo, "[DUT%d][%s] Press to continue test THD.", test_handle->handle_index+1, testItemName);
	if (IDYES != ::AfxMessageBox (popupInfo, 0, 1))

	// Phone level thd
	AddLog ("[%s][THD] start.", testItemName);
	if (index == 2)
	{
		atstr_prop = "AT+PROPERTY=0,1,\"Audio.Auto.OutputType,2\"";
	}
	else
	{
		atstr_prop = "AT+PROPERTY=0,1,\"Audio.Auto.OutputType,3\"";
	}
	atstr_prop += CR;
	test_handle->m_comm_handle->WriteCommand(atstr_prop.c_str(), atstr_prop.length(), ATTIMEOUT, false, atret);
	Sleep (300);
	ClearATResPool ();
	test_handle->m_comm_handle->WriteCommand(atstr.c_str(), atstr.length(), ATTIMEOUT, false, atret);
	if (!GetATResponse(atret))
	{
        AddLog ("[%s][THD] AT response timeout: %d.", testItemName, timeout_sec);
        return E_ATADLL_RESULT_AT_RESPONSE_TIMEOUT;
    }
	if (NULL != strstr(atret.resultLst[0].eleLst[0].str_value.c_str(), "pass") ||
		NULL != strstr(atret.resultLst[0].eleLst[0].str_value.c_str(), "fail"))
    {
        AddLog ("[%s][THD] Finished.", testItemName);
    }
    else
    {
        AddLog ("[%s][THD] Response not corrent.", testItemName);
		return E_ATADLL_RESULT_FAILED;
    }

	if (index == 0)
    {
        test_handle->m_testCNF->audio_cnf.bSpeakerPass = true;
    }
    else if (index == 1)
    {
        test_handle->m_testCNF->audio_cnf.bReceiverPass = true;
    }
    else if (index == 2)
    {
        test_handle->m_testCNF->audio_cnf.bHeadsetPass = true;
    }

	glb_at_resp_flag = true;
	sp_ata_data s_data;
	RequestData (s_data);
	glb_at_resp_flag = false;

	test_handle->m_commonCFG.cbTestProgess (test_handle->handle_index, test_item, 100);

	char audioStr[2048];
	memset (audioStr, 0x0, 2048);

	int temp_index = 0;
	if (index == 0)
	{
					for (temp_index = 0; temp_index < 5; temp_index ++)
					{
						sprintf (audioStr, "%s[Audio][Speaker] Freq response: mean(%.5f), deviation(%.5f), min(%.5f), max(%.5f)\r\n",
										audioStr,
										s_data.spk_response.freqresponse[temp_index].mean,
										s_data.spk_response.freqresponse[temp_index].deviation,
										s_data.spk_response.freqresponse[temp_index].min,
										s_data.spk_response.freqresponse[temp_index].max);
							
					}
					
					sprintf (audioStr, "%s[Audio][Speaker] THD: mean(%.5f), deviation(%.5f), min(%.5f), max(%.5f)\r\n",
										audioStr,
										s_data.spk_thd.thd.mean,
										s_data.spk_thd.thd.deviation,
										s_data.spk_thd.thd.min,
										s_data.spk_thd.thd.max );
	}
	else if (index == 1)
	{
					for (temp_index = 0; temp_index < 5; temp_index ++)
					{
						sprintf (audioStr, "%s[Audio][Receiver] Freq response: mean(%.5f), deviation(%.5f), min(%.5f), max(%.5f)\r\n",
										audioStr,
										s_data.rcv_response.freqresponse[temp_index].mean,
										s_data.rcv_response.freqresponse[temp_index].deviation,
										s_data.rcv_response.freqresponse[temp_index].min,
										s_data.rcv_response.freqresponse[temp_index].max );
												
					}

					sprintf (audioStr, "%s[Audio][Receiver] THD: mean(%.5f), deviation(%.5f), min(%.5f), max(%.5f)\r\n",
										audioStr,
										s_data.rcv_thd.thd.mean,
										s_data.rcv_thd.thd.deviation,
										s_data.rcv_thd.thd.min,
										s_data.rcv_thd.thd.max );
	}
	else if (index == 2)
	{	
					sprintf (audioStr, "%s[Audio][HeadsetL][Common]:\r\n\t left freq: %d, right freq: %d\r\n\t left amp: %d, right amp: %d\r\n",
                                             audioStr, s_data.headsetL.freqL, s_data.headsetL.freqR, s_data.headsetL.amplL, s_data.headsetL.amplR);

					sprintf (audioStr, "%s[Audio][HeadsetR][Common]:\r\n\t left freq: %d, right freq: %d\r\n\t left amp: %d, right amp: %d\r\n",
                                             audioStr, s_data.headsetR.freqL, s_data.headsetR.freqR, s_data.headsetR.amplL, s_data.headsetR.amplR);

					sprintf (audioStr, "%s[Audio][HeadsetL] THD: mean(%.5f), deviation(%.5f), min(%.5f), max(%.5f)\r\n",
										audioStr,
										s_data.headsetL_thd.thd.mean,
										s_data.headsetL_thd.thd.deviation,
										s_data.headsetL_thd.thd.min,
										s_data.headsetL_thd.thd.max );


					sprintf (audioStr, "%s[Audio][HeadsetR] THD: mean(%.5f), deviation(%.5f), min(%.5f), max(%.5f)\r\n",
										audioStr,
										s_data.headsetR_thd.thd.mean,
										s_data.headsetR_thd.thd.deviation,
										s_data.headsetR_thd.thd.min,
										s_data.headsetR_thd.thd.max );
	}

	::AfxMessageBox (audioStr, 0, 0);

    return E_ATADLL_RESULT_SUCCESS;
}

E_ATADLL_RESULT CTestItem_Audio::TestAudio_Manual_ATA30 (char index)
{
    char testItemName[64];
    string atstr = "";
	string atstr_prop = "";
    ATResult atret;
	
	char popupInfo[256] = {0};
	bool result = true;

    if (index == 0)
    {
        sprintf (testItemName, "speaker");
        atstr = "AT+RINGTONE=1,2";

    }
    else if (index == 1)
    {
        sprintf (testItemName, "receiver");
        atstr = "AT+RECEIVER=1,2";
    }
    else if (index == 2)
    {
        sprintf (testItemName, "headset receiver");
        atstr = "AT+HEADSET=1,2";
    }
    else
    {
        sprintf (testItemName, "Audio");
		atstr = "AT+RINGTONE=1,2";
    }
    atstr += CR;
	ClearATResPool ();

#ifdef __QA30_VERSION__

	test_handle->m_comm_handle->WriteCommand(atstr.c_str(), atstr.length(), ATTIMEOUT, false, atret);
	if (!GetATResponse(atret))
	{
        AddLog ("[Loopback][%s] Wait AT response timeout: %d.", testItemName, timeout_sec);
        return E_ATADLL_RESULT_AT_RESPONSE_TIMEOUT;
    }

	if (NULL != strstr(atret.resultLst[0].eleLst[0].str_value.c_str(), "pass"))
    {
        AddLog ("[Loopback][%s] Test pass.", testItemName);
		result = true;
    }
	else
	{
		AddLog ("[Loopback][%s] Test fail.", testItemName);
		result = false;
	}

#else

	// Phone level Input type = Main Mic
	AddLog ("[Loopback][main mic -> %s]", testItemName);
	atstr_prop = "AT+PROPERTY=0,1,\"Audio.Manual.InputType,1\"";
	atstr_prop += CR;
	test_handle->m_comm_handle->WriteCommand(atstr_prop.c_str(), atstr_prop.length(), ATTIMEOUT, false, atret);
	test_handle->m_comm_handle->WriteCommand(atstr.c_str(), atstr.length(), ATTIMEOUT, false, atret);
	if (!GetATResponse(atret))
	{
        AddLog ("[Loopback][main mic -> %s] Open loopback timeout: %d.", testItemName, timeout_sec);
        return E_ATADLL_RESULT_AT_RESPONSE_TIMEOUT;
    }
	AddLog ("[Loopback][main mic -> %s] Open loopback OK.", testItemName);
	test_handle->m_commonCFG.cbTestProgess (test_handle->handle_index, test_item, 30);

	sprintf (popupInfo, "[DUT%d][Loopback][main mic -> %s] Opened, please check if OK.", test_handle->handle_index+1, testItemName);
	if (IDYES != ::AfxMessageBox (popupInfo, MB_YESNO, 0))
	{
		result = false;
		AddLog ("[Loopback][main mic -> %s] Check fail.", testItemName);
	}
	AddLog ("[Loopback][main mic -> %s] Check OK.", testItemName);
	
	atstr_prop = "AT+PROPERTY=0,1,\"Audio.Manual.InputType,0\"";
	atstr_prop += CR;
	test_handle->m_comm_handle->WriteCommand(atstr_prop.c_str(), atstr_prop.length(), ATTIMEOUT, false, atret);
	test_handle->m_comm_handle->WriteCommand(atstr.c_str(), atstr.length(), ATTIMEOUT, false, atret);
	if (!GetATResponse(atret))
	{
        AddLog ("[Loopback][main mic -> %s] Close loopback timeout: %d.", testItemName, timeout_sec);
        return E_ATADLL_RESULT_AT_RESPONSE_TIMEOUT;
    }
	AddLog ("[Loopback][main mic -> %s] Close loopback OK.", testItemName);
	test_handle->m_commonCFG.cbTestProgess (test_handle->handle_index, test_item, 40);

	// Phone level Input type = Sub Mic
	AddLog ("\r\n[Loopback][sub mic -> %s]", testItemName);
	atstr_prop = "AT+PROPERTY=0,1,\"Audio.Manual.InputType,2\"";
	atstr_prop += CR;
	test_handle->m_comm_handle->WriteCommand(atstr_prop.c_str(), atstr_prop.length(), ATTIMEOUT, false, atret);
	test_handle->m_comm_handle->WriteCommand(atstr.c_str(), atstr.length(), ATTIMEOUT, false, atret);
	if (!GetATResponse(atret))
	{
        AddLog ("[Loopback][sub mic -> %s] Open loopback timeout: %d.", testItemName, timeout_sec);
        return E_ATADLL_RESULT_AT_RESPONSE_TIMEOUT;
    }
	AddLog ("[Loopback][sub mic -> %s] Open loopback OK.", testItemName);
	test_handle->m_commonCFG.cbTestProgess (test_handle->handle_index, test_item, 50);

	sprintf (popupInfo, "[DUT%d][Loopback][sub mic -> %s] Opened, please check if OK.", test_handle->handle_index+1, testItemName);
	if (IDYES != ::AfxMessageBox (popupInfo, MB_YESNO, 0))
	{
		result = false;
		AddLog ("[Loopback][sub mic -> %s] Check fail.", testItemName);
	}
	AddLog ("[Loopback][sub mic -> %s] Check OK.", testItemName);
	
	atstr_prop = "AT+PROPERTY=0,1,\"Audio.Manual.InputType,0\"";
	atstr_prop += CR;
	test_handle->m_comm_handle->WriteCommand(atstr_prop.c_str(), atstr_prop.length(), ATTIMEOUT, false, atret);
	test_handle->m_comm_handle->WriteCommand(atstr.c_str(), atstr.length(), ATTIMEOUT, false, atret);
	if (!GetATResponse(atret))
	{
        AddLog ("[Loopback][sub mic -> %s] Close loopback timeout: %d.", testItemName, timeout_sec);
        return E_ATADLL_RESULT_AT_RESPONSE_TIMEOUT;
    }
	AddLog ("[Loopback][sub mic -> %s] Close loopback OK.", testItemName);
	test_handle->m_commonCFG.cbTestProgess (test_handle->handle_index, test_item, 60);

	// Phone level Input type = Headset Mic
	AddLog ("\r\n[Loopback][headset mic -> %s]", testItemName);
	atstr_prop = "AT+PROPERTY=0,1,\"Audio.Manual.InputType,3\"";
	atstr_prop += CR;
	test_handle->m_comm_handle->WriteCommand(atstr_prop.c_str(), atstr_prop.length(), ATTIMEOUT, false, atret);
	test_handle->m_comm_handle->WriteCommand(atstr.c_str(), atstr.length(), ATTIMEOUT, false, atret);
	if (!GetATResponse(atret))
	{
        AddLog ("[Loopback][headset mic -> %s] Open loopback timeout: %d.", testItemName, timeout_sec);
        return E_ATADLL_RESULT_AT_RESPONSE_TIMEOUT;
    }
	AddLog ("[Loopback][headset mic -> %s] Open loopback OK.", testItemName);
	test_handle->m_commonCFG.cbTestProgess (test_handle->handle_index, test_item, 70);

	sprintf (popupInfo, "[DUT%d][Loopback][headset mic -> %s] Opened, please check if OK.", test_handle->handle_index+1, testItemName);
	if (IDYES != ::AfxMessageBox (popupInfo, MB_YESNO, 0))
	{
		result = false;
		AddLog ("[Loopback][headset mic -> %s] Check fail.", testItemName);
	}
	AddLog ("[Loopback][headset mic -> %s] Check OK.", testItemName);
	
	atstr_prop = "AT+PROPERTY=0,1,\"Audio.Manual.InputType,0\"";
	atstr_prop += CR;
	test_handle->m_comm_handle->WriteCommand(atstr_prop.c_str(), atstr_prop.length(), ATTIMEOUT, false, atret);
	test_handle->m_comm_handle->WriteCommand(atstr.c_str(), atstr.length(), ATTIMEOUT, false, atret);
	if (!GetATResponse(atret))
	{
        AddLog ("[Loopback][headset mic -> %s] Close loopback timeout: %d.", testItemName, timeout_sec);
        return E_ATADLL_RESULT_AT_RESPONSE_TIMEOUT;
    }
	AddLog ("[Loopback][Headset mic -> %s] Close loopback OK.", testItemName);
	test_handle->m_commonCFG.cbTestProgess (test_handle->handle_index, test_item, 80);

#endif

	if (index == 0)
    {
        test_handle->m_testCNF->audio_cnf.bSpeakerPass = result;
    }
    else if (index == 1)
    {
        test_handle->m_testCNF->audio_cnf.bReceiverPass = result;
    }
    else if (index == 2)
    {
        test_handle->m_testCNF->audio_cnf.bHeadsetPass = result;
    }

	if (result == true)
	{
    	return E_ATADLL_RESULT_SUCCESS;
	}
	else
	{
		return E_ATADLL_RESULT_FAILED;
	}
}
#endif


