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
 *   ATA_DLL_TestItem_Sensor.cpp
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
#include "..\header\ATA_DLL_TestItem_Sensor.h"
#include "..\..\GlobalOBJ.h"
//#include <string.h>

CTestItem_Sensor::CTestItem_Sensor(ATA_DLL_Handle *current_handle, int timeout, E_ATDLL_TEST_ITEM_COUNT item, char *testName)
    : CTestItem_Interface(current_handle, timeout, item, testName)
{
}

CTestItem_Sensor::~CTestItem_Sensor()
{
}


E_ATADLL_RESULT CTestItem_Sensor::StartTest ()
{

    ClearATResPool();

    ATResult atret;
    string atstr = "";
    atstr += CR;

	glb_at_resp_flag = true;

    if (E_GSENSOR == test_item)
    {
#ifdef __ATA20__
        if (IsAutoMode())
        {
            atstr = "AT+GSENSOR=1,1";
        }
        else
        {
            atstr = "AT+GSENSOR=1,2";
        }
#else
        atstr = "AT+GSENSOR";
#endif
        atstr += CR;

        test_handle->m_testCNF->test_result[E_GSENSOR] = E_ATADLL_RESULT_SUCCESS;

        sp_ata_data s_data_temp[3];
        int temp_index = 0;
        int loopCount = 1;
        if (test_handle->m_commonCFG.gsensor_cfg.bGSensorValueDiff)
        {
            loopCount = 3;
        }

        test_handle->m_commonCFG.cbTestProgess (test_handle->handle_index, E_GSENSOR, 10);
        for (temp_index = 0; temp_index < loopCount; temp_index ++)
        {
            Sleep (300);
            atstr = "AT+GSENSOR";
            atstr += CR;

            ClearATResPool ();
			
#if (defined __ATA20__) || (defined __ATA_LOAD30_TEST20__)
			test_handle->m_comm_handle->WriteCommand(atstr.c_str(), atstr.length(), 500, false, atret);
#else
            test_handle->m_comm_handle->WriteCommand(atstr.c_str(), atstr.length(), ATTIMEOUT, false, atret);
#endif

#if (defined __ATA20__) || (defined __ATA_LOAD30_TEST20__)
			Sleep (3000);
	        atstr = "AT+GSENSOR=STOP";
	        atstr += CR;
	        test_handle->m_comm_handle->WriteCommand(atstr.c_str(), atstr.length(), ATTIMEOUT, false, atret);
#endif

            if (!GetATResponse(atret))
            {
                AddLog ("[GSensor ERROR] %s response timeout.",atstr.c_str());
                test_handle->m_testCNF->test_result[E_GSENSOR] = E_ATADLL_RESULT_AT_RESPONSE_TIMEOUT;
				
                return E_ATADLL_RESULT_AT_RESPONSE_TIMEOUT;
            }

            if (strstr (atret.resultLst[0].eleLst[0].str_value.c_str(), "Cannot find") != NULL)
            {
                AddLog ("[GSensor ERROR] GSensor Not support !");
                test_handle->m_testCNF->test_result[E_GSENSOR] = E_ATADLL_RESULT_NOT_SUPPORT;
				return E_ATADLL_RESULT_NOT_SUPPORT;
            }

            atstr = "AT+REQUESTDATA";
            atstr += CR;

            ClearATResPool();
            test_handle->m_comm_handle->WriteCommand(atstr.c_str(), atstr.length(), ATTIMEOUT, false, atret);
            if (!GetATResponse(atret))
            {
                AddLog ("[GSensor ERROR] %s response timeout: %d.",atstr.c_str(), timeout_sec);
                test_handle->m_testCNF->test_result[E_GSENSOR] = E_ATADLL_RESULT_AT_RESPONSE_TIMEOUT;
                return E_ATADLL_RESULT_AT_RESPONSE_TIMEOUT;
            }

            GetATAData (s_data_temp[temp_index], atret);
            AddLog ("%d: G-Sensor: x - (%.7f), y - (%.7f), z - (%.7f)", temp_index+1, s_data_temp[temp_index].gsensor.g_sensor_x,
                    s_data_temp[temp_index].gsensor.g_sensor_y, s_data_temp[temp_index].gsensor.g_sensor_z);

            test_handle->m_commonCFG.cbTestProgess (test_handle->handle_index, E_GSENSOR, 10+30*temp_index);
        }

        if (test_handle->m_commonCFG.gsensor_cfg.bGSensorValueDiff)
        {
            if (test_handle->m_commonCFG.gsensor_cfg.bX &&
                    (s_data_temp[0].gsensor.g_sensor_x == s_data_temp[1].gsensor.g_sensor_x && s_data_temp[1].gsensor.g_sensor_x == s_data_temp[2].gsensor.g_sensor_x))
            {
                AddLog ("[X Failed] There is no difference among the 3 times value.");
                test_handle->m_testCNF->test_result[E_GSENSOR] = E_ATADLL_RESULT_FAILED;
            }

            if (test_handle->m_commonCFG.gsensor_cfg.bY &&
                    (s_data_temp[0].gsensor.g_sensor_y == s_data_temp[1].gsensor.g_sensor_y && s_data_temp[1].gsensor.g_sensor_y == s_data_temp[2].gsensor.g_sensor_y))
            {
                AddLog ("[Y Failed] There is no difference among the 3 times value.");
                test_handle->m_testCNF->test_result[E_GSENSOR] = E_ATADLL_RESULT_FAILED;
            }

            if (test_handle->m_commonCFG.gsensor_cfg.bZ &&
                    (s_data_temp[0].gsensor.g_sensor_z == s_data_temp[1].gsensor.g_sensor_z && s_data_temp[1].gsensor.g_sensor_z == s_data_temp[2].gsensor.g_sensor_z))
            {
                AddLog ("[Z Failed] There is no difference among the 3 times value.");
                test_handle->m_testCNF->test_result[E_GSENSOR] = E_ATADLL_RESULT_FAILED;
            }
        }

        test_handle->m_commonCFG.cbTestProgess (test_handle->handle_index, E_GSENSOR, 100);
    }

    if (E_MSENSOR == test_item)
    {
#ifdef __ATA20__
        if (IsAutoMode())
        {
            atstr = "AT+MSENSOR=1,1";
        }
        else
        {
            atstr = "AT+MSENSOR=1,2";
        }
#else
        atstr = "AT+MSENSOR";
#endif
        atstr += CR;

		ClearATResPool ();
        test_handle->m_commonCFG.cbTestProgess (test_handle->handle_index, E_MSENSOR, 10);
#if (defined __ATA20__) || (defined __ATA_LOAD30_TEST20__)
		test_handle->m_comm_handle->WriteCommand(atstr.c_str(), atstr.length(), 500, false, atret);
#else
        test_handle->m_comm_handle->WriteCommand(atstr.c_str(), atstr.length(), ATTIMEOUT, false, atret);
#endif
        test_handle->m_commonCFG.cbTestProgess (test_handle->handle_index, E_MSENSOR, 40);

#if (defined __ATA20__) || (defined __ATA_LOAD30_TEST20__)
		Sleep (300);
		atstr = "AT+MSENSOR=STOP";
		atstr += CR;
		test_handle->m_comm_handle->WriteCommand(atstr.c_str(), atstr.length(), ATTIMEOUT, false, atret);
#endif

        if (!GetATResponse(atret))
        {
            AddLog ("[M-Sensor ERROR] %s response timeout.",atstr.c_str());
            test_handle->m_testCNF->test_result[E_MSENSOR] = E_ATADLL_RESULT_AT_RESPONSE_TIMEOUT;
            return E_ATADLL_RESULT_AT_RESPONSE_TIMEOUT;
        }

        if (strstr (atret.resultLst[0].eleLst[0].str_value.c_str(), "Cannot find") != NULL)
        {
            AddLog ("[M-Sensor ERROR] M-Sensor Not support !");
            test_handle->m_testCNF->test_result[E_MSENSOR] = E_ATADLL_RESULT_NOT_SUPPORT;
			return E_ATADLL_RESULT_NOT_SUPPORT;
        }
        else
        {
            test_handle->m_testCNF->test_result[E_MSENSOR] = E_ATADLL_RESULT_SUCCESS;
        }

        test_handle->m_commonCFG.cbTestProgess (test_handle->handle_index, E_MSENSOR, 100);
        Sleep (200);
    }

    if (E_ALSPS == test_item)
    {
#ifdef __ATA20__
        if (IsAutoMode())
        {
            atstr = "AT+ALSPS=1,1";
        }
        else
        {
            atstr = "AT+ALSPS=1,2";
        }
#else
        atstr = "AT+ALSPS";
#endif
        atstr += CR;

        ClearATResPool ();
		
        test_handle->m_commonCFG.cbTestProgess (test_handle->handle_index, E_ALSPS, 10);
#if (defined __ATA20__) || (defined __ATA_LOAD30_TEST20__)
		test_handle->m_comm_handle->WriteCommand(atstr.c_str(), atstr.length(), 500, false, atret);
#else
        test_handle->m_comm_handle->WriteCommand(atstr.c_str(), atstr.length(), ATTIMEOUT, false, atret);
#endif
        test_handle->m_commonCFG.cbTestProgess (test_handle->handle_index, E_ALSPS, 50);


#if (defined __ATA20__) || (defined __ATA_LOAD30_TEST20__)
		Sleep (2000);
        atstr = "AT+ALSPS=STOP";
        atstr += CR;
        test_handle->m_comm_handle->WriteCommand(atstr.c_str(), atstr.length(), ATTIMEOUT, false, atret);
#endif

		if (!GetATResponse(atret))
        {
            AddLog ("[ALS/PS ERROR] %s response timeout.",atstr.c_str());
            test_handle->m_testCNF->test_result[E_ALSPS] = E_ATADLL_RESULT_AT_RESPONSE_TIMEOUT;
            return E_ATADLL_RESULT_AT_RESPONSE_TIMEOUT;
        }

        if (strstr (atret.resultLst[0].eleLst[0].str_value.c_str(), "Cannot find") != NULL)
        {
            AddLog ("[ALS/PS ERROR] ALS/PS Not support !");
            test_handle->m_testCNF->test_result[E_ALSPS] = E_ATADLL_RESULT_NOT_SUPPORT;
			return E_ATADLL_RESULT_NOT_SUPPORT;
        }
        else
        {
            test_handle->m_testCNF->test_result[E_ALSPS] = E_ATADLL_RESULT_SUCCESS;
        }

        test_handle->m_commonCFG.cbTestProgess (test_handle->handle_index, E_ALSPS, 100);
        Sleep (200);
    }

    if (E_GYROSCOPE == test_item)
    {

#ifdef __ATA20__
        if (IsAutoMode())
        {
            atstr = "AT+GYROSCOPE=1,1";
        }
        else
        {
            atstr = "AT+GYROSCOPE=1,2";
        }
#else
        atstr = "AT+GYROSCOPE";
#endif
        atstr += CR;

        ClearATResPool ();
        test_handle->m_commonCFG.cbTestProgess (test_handle->handle_index, E_GYROSCOPE, 10);
#if (defined __ATA20__) || (defined __ATA_LOAD30_TEST20__)
		test_handle->m_comm_handle->WriteCommand(atstr.c_str(), atstr.length(), 500, false, atret);
#else
        test_handle->m_comm_handle->WriteCommand(atstr.c_str(), atstr.length(), ATTIMEOUT, false, atret);
#endif
        test_handle->m_commonCFG.cbTestProgess (test_handle->handle_index, E_GYROSCOPE, 40);

#if (defined __ATA20__) || (defined __ATA_LOAD30_TEST20__)
		Sleep (300);
		atstr = "AT+GYROSCOPE=STOP";
		atstr += CR;
		test_handle->m_comm_handle->WriteCommand(atstr.c_str(), atstr.length(), ATTIMEOUT, false, atret);
#endif

        if (!GetATResponse(atret))
        {
            AddLog ("[GYROSCOPE ERROR] %s response timeout.",atstr.c_str());
            test_handle->m_testCNF->test_result[E_GYROSCOPE] = E_ATADLL_RESULT_AT_RESPONSE_TIMEOUT;
            return E_ATADLL_RESULT_AT_RESPONSE_TIMEOUT;
        }

        if (strstr (atret.resultLst[0].eleLst[0].str_value.c_str(), "Cannot find") != NULL)
        {
            AddLog ("[GYROSCOPE ERROR] GYROSCOPE Not support !");
            test_handle->m_testCNF->test_result[E_GYROSCOPE] = E_ATADLL_RESULT_NOT_SUPPORT;
			return E_ATADLL_RESULT_NOT_SUPPORT;
        }
        else
        {
            test_handle->m_testCNF->test_result[E_GYROSCOPE] = E_ATADLL_RESULT_SUCCESS;
        }

        test_handle->m_commonCFG.cbTestProgess (test_handle->handle_index, E_GYROSCOPE, 100);
        Sleep (200);
    }

#ifndef __ATA20__
    // Get data
    atstr = "AT+REQUESTDATA";
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
    memcpy (&test_handle->m_testCNF->sensor_cnf.gsensor, &s_data.gsensor, sizeof(ftm_ata_gsensor));
    memcpy (&test_handle->m_testCNF->sensor_cnf.msensor, &s_data.msensor, sizeof(ftm_ata_msensor));
    memcpy (&test_handle->m_testCNF->sensor_cnf.alsps, &s_data.alsps, sizeof(ftm_ata_alsps));
    memcpy (&test_handle->m_testCNF->sensor_cnf.gyroscope, &s_data.gyroscope, sizeof(ftm_ata_gyroscope));

    if (test_handle->m_testItemCFG.b_gsensor)
    {
        AddLog ("G-Sensor: x - (%f), y - (%f), z - (%f), accuracy - (%d)",
                s_data.gsensor.g_sensor_x, s_data.gsensor.g_sensor_y, s_data.gsensor.g_sensor_z, s_data.gsensor.accuracy);
    }
    if (test_handle->m_testItemCFG.b_msensor)
    {
        AddLog ("M-Sensor: x - (%d), y - (%d), z - (%d), accurary - (%d)",
                s_data.msensor.m_sensor_x, s_data.msensor.m_sensor_y, s_data.msensor.m_sensor_z, s_data.msensor.accuracy);
    }
    if (test_handle->m_testItemCFG.b_alsps)
    {
        AddLog ("ALS/PS: ALS - (%d), PS - (%d)", s_data.alsps.als, s_data.alsps.ps);
    }
    if (test_handle->m_testItemCFG.b_gyroscope)
    {
        AddLog ("GYROSCOPE: x - (%f), y - (%f), z - (%f), accuracy - (%d)",
                s_data.gyroscope.gyroscope_x, s_data.gyroscope.gyroscope_y, s_data.gyroscope.gyroscope_z, s_data.gyroscope.accuracy);
    }
#endif

	glb_at_resp_flag = false;

    return test_handle->m_testCNF->test_result[test_item];
}


