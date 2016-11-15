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
 *   ATA_DLL_TestItem_Camera.cpp
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
#include "..\header\ATA_DLL_TestItem_Camera.h"
#include "..\..\GlobalOBJ.h"

CTestItem_Camera::CTestItem_Camera(ATA_DLL_Handle *current_handle, int timeout, E_ATDLL_TEST_ITEM_COUNT item, char *testName)
    : CTestItem_Interface(current_handle, timeout, item, testName)
{
}

CTestItem_Camera::~CTestItem_Camera()
{
}

E_ATADLL_RESULT CTestItem_Camera::StartTest ()
{
    E_ATADLL_RESULT ata_result = E_ATADLL_RESULT_SUCCESS;
    E_ATADLL_RESULT ata_result_final = E_ATADLL_RESULT_SUCCESS;

    glb_at_resp_flag = true;

    if (E_CAMERA == test_item)
    {
        test_handle->m_commonCFG.cbTestProgess (test_handle->handle_index, E_CAMERA, 20);
        ata_result = TestCamera (0);
        if (ata_result != E_ATADLL_RESULT_SUCCESS)
        {
            ata_result_final = ata_result;
        }
        test_handle->m_testCNF->test_result[E_CAMERA] = ata_result;
        test_handle->m_commonCFG.cbTestProgess (test_handle->handle_index, E_CAMERA, 100);
    }

    if (E_CAMERA_SUB == test_item)
    {
        test_handle->m_commonCFG.cbTestProgess (test_handle->handle_index, E_CAMERA_SUB, 20);
        ata_result = TestCamera (1);
        if (ata_result != E_ATADLL_RESULT_SUCCESS)
        {
            ata_result_final = ata_result;
        }
        test_handle->m_testCNF->test_result[E_CAMERA_SUB] = ata_result;
        test_handle->m_commonCFG.cbTestProgess (test_handle->handle_index, E_CAMERA_SUB, 100);
    }
	if (E_CAMERA_MAIN2== test_item)
    {
        test_handle->m_commonCFG.cbTestProgess (test_handle->handle_index, E_CAMERA_MAIN2, 20);
        ata_result = TestCamera (2);
        if (ata_result != E_ATADLL_RESULT_SUCCESS)
        {
            ata_result_final = ata_result;
        }
        test_handle->m_testCNF->test_result[E_CAMERA_MAIN2] = ata_result;
        test_handle->m_commonCFG.cbTestProgess (test_handle->handle_index, E_CAMERA_MAIN2, 100);
    }
	

    if (*(test_handle->m_commonCFG.stop_flag) == ATA_DLL_STOP_FLAG)
    {
        return E_ATADLL_RESULT_FAILED;
    }

#ifdef __ATA30__
	ClearATResPool ();
    TransferCameraData ();
#endif

    if (*(test_handle->m_commonCFG.stop_flag) == ATA_DLL_STOP_FLAG)
    {
        return E_ATADLL_RESULT_FAILED;
    }

    glb_at_resp_flag = false;

    return ata_result_final;
}

/*****************************************
	type : 0 - main camera; 1 - sub camera;

*****************************************/
E_ATADLL_RESULT CTestItem_Camera::TestCamera (int type)
{
	
	E_ATADLL_RESULT testResultCode;
    char at_buff[64];
    char camera_type[64];
    string atstr = "";
    ATResult atret;

    ClearATResPool ();

#ifdef __ATA30__
    sprintf (at_buff, "AT+PROPERTY=0,1,\"FTMCameraTest,1\"");
    atstr = at_buff;
    atstr += CR;
    test_handle->m_comm_handle->WriteCommand(atstr.c_str(), atstr.length(), ATTIMEOUT, false, atret);
    if (!GetATResponse(atret))
    {
        AddLog ("[Camera ERROR]%s response timeout when setting camera test type.",atstr.c_str());
        return E_ATADLL_RESULT_AT_RESPONSE_TIMEOUT;
    }
	if (strstr(atret.resultLst[0].eleLst[0].str_value.c_str(), "pass") != NULL)
    {
        AddLog ("[OK] Set %s TestType to target success", atstr.c_str());
    }
    else
    {
        AddLog ("[Camera ERROR] Set  %s TestType to target fail", atstr.c_str());
        return E_ATADLL_RESULT_FORMAT_INCORRECT;
    }

    ClearATResPool ();
#endif

#ifdef __ATA20__
    if (type == 0)
    {
        if (IsAutoMode())
        {
            sprintf (at_buff, "AT+MAINCAMERA=1,1");
        }
        else
        {
            sprintf (at_buff, "AT+MAINCAMERA=1,2");
        }
        sprintf (camera_type, "Main camera");
    }
    else if (type == 1)
    {
        if (IsAutoMode())
        {
            sprintf (at_buff, "AT+SUBCAMERA=1,1");
        }
        else
        {
            sprintf (at_buff, "AT+SUBCAMERA=1,2");
        }
        sprintf (camera_type, "Sub camera");
    }
	else if (type == 2)
	{
		if (IsAutoMode())
        {
            sprintf (at_buff, "AT+MAINCAMERA2=1,1");
        }
        else
        {
            sprintf (at_buff, "AT+MAINCAMERA2=1,2");
        }
        sprintf (camera_type, "Main2 camera");
		
	}
#else
    if (type == 0)
    {
        sprintf (at_buff, "AT+MAINCAMERA");
        sprintf (camera_type, "Main camera");
    }
    else
    {
        sprintf (at_buff, "AT+SUBCAMERA");
        sprintf (camera_type, "Sub camera");
    }
#endif

    AddLog ("%s test start", camera_type);

    atstr = at_buff;
    atstr += CR;
    test_handle->m_comm_handle->WriteCommand(atstr.c_str(), atstr.length(), ATTIMEOUT, false, atret);
    

	char    camera_str[128];
	int     m_iWholeCmdLen = 0;

	// Get check sum
	if (test_handle->m_commonCFG.camera_cfg.bCheckSum)
	{
		if (!GetATResponse(atret))
	    {
	        AddLog ("[Camera ERROR]AT response timeout when getting check sum.");
	        AddLog ("%s test end\r\n",camera_type);
	        return E_ATADLL_RESULT_AT_RESPONSE_TIMEOUT;
	    }

	    m_iWholeCmdLen  = atret.resultLst[0].eleLst[0].str_value.length();
	    memset (camera_str, 0x0, sizeof(char)*128);
	    memcpy (camera_str, atret.resultLst[0].eleLst[0].str_value.c_str(), m_iWholeCmdLen);
	    camera_str[m_iWholeCmdLen] = 0;

		char *pFlag = strstr (camera_str, ":");
	    if (pFlag == NULL)
	    {
			if (type == 0)
			{
				memcpy (test_handle->m_testCNF->camera_cnf.mainCameraCheckSum, camera_str, m_iWholeCmdLen);
				test_handle->m_testCNF->camera_cnf.mainCameraCheckSum[m_iWholeCmdLen] = 0;
				AddLog ("Main camera check sum: %s", test_handle->m_testCNF->camera_cnf.mainCameraCheckSum);
			}
			else if (type == 1)
			{
				memcpy (test_handle->m_testCNF->camera_cnf.subCameraCheckSum, camera_str, m_iWholeCmdLen);
				test_handle->m_testCNF->camera_cnf.subCameraCheckSum[m_iWholeCmdLen] = 0;
				AddLog ("Sub camera check sum: %s", test_handle->m_testCNF->camera_cnf.subCameraCheckSum);
			}
			else if (type == 2)
			{
				memcpy (test_handle->m_testCNF->camera_cnf.main2CameraCheckSum, camera_str, m_iWholeCmdLen);
				test_handle->m_testCNF->camera_cnf.main2CameraCheckSum[m_iWholeCmdLen] = 0;
				AddLog ("Main2 camera check sum: %s", test_handle->m_testCNF->camera_cnf.main2CameraCheckSum);
			}
	    }
	    else
	    {
			if (type == 0)
			{
				memcpy (test_handle->m_testCNF->camera_cnf.mainCameraCheckSum, pFlag+1, m_iWholeCmdLen-(pFlag-camera_str)-1);
				test_handle->m_testCNF->camera_cnf.mainCameraCheckSum[m_iWholeCmdLen-(pFlag-camera_str)-1] = 0;
				AddLog ("Main camera check sum: %s", test_handle->m_testCNF->camera_cnf.mainCameraCheckSum);
			}
			else if (type == 1)
			{
				memcpy (test_handle->m_testCNF->camera_cnf.subCameraCheckSum, pFlag+1, m_iWholeCmdLen-(pFlag-camera_str)-1);
				test_handle->m_testCNF->camera_cnf.subCameraCheckSum[m_iWholeCmdLen-(pFlag-camera_str)-1] = 0;
				AddLog ("Sub camera check sum: %s", test_handle->m_testCNF->camera_cnf.subCameraCheckSum);
			}
			else if (type == 2)
			{
				memcpy (test_handle->m_testCNF->camera_cnf.main2CameraCheckSum, pFlag+1, m_iWholeCmdLen-(pFlag-camera_str)-1);
				test_handle->m_testCNF->camera_cnf.main2CameraCheckSum[m_iWholeCmdLen-(pFlag-camera_str)-1] = 0;
				AddLog ("Main2 camera check sum: %s", test_handle->m_testCNF->camera_cnf.main2CameraCheckSum);
			}
	    }
	}

	if (!GetATResponse(atret))
    {
        AddLog ("[Camera ERROR]AT response timeout when getting camera sensor result.");
        AddLog ("%s test end\r\n",camera_type);
		if (type == 0)
        {
            	test_handle->m_testCNF->camera_cnf.b_mainCamera	= false;
        }
       	else if (type == 1)
        {
            	test_handle->m_testCNF->camera_cnf.b_subCamera	= false;
        }
		else if (type == 2)
		{
				test_handle->m_testCNF->camera_cnf.b_main2Camera	= false;
		}
        return E_ATADLL_RESULT_AT_RESPONSE_TIMEOUT;
    }
	if (test_handle->m_commonCFG.camera_cfg.bCheckSum)
	{
		return E_ATADLL_RESULT_SUCCESS;
	}
	else
	{
	
	if (NULL != strstr(atret.resultLst[0].eleLst[0].str_value.c_str(), "pass"))
		{
			AddLog ("[OK] %s test ok.",camera_type);
			if (type == 0)
        	{
            	test_handle->m_testCNF->camera_cnf.b_mainCamera	= true;
        	}
        	else if (type == 1)
        	{
            	test_handle->m_testCNF->camera_cnf.b_subCamera	= true;
        	}
			else if (type == 2)
			{
				test_handle->m_testCNF->camera_cnf.b_main2Camera	= true;
			}
			testResultCode=E_ATADLL_RESULT_SUCCESS;
		}
		else if(NULL != strstr(atret.resultLst[0].eleLst[0].str_value.c_str(), "fail"))
		{
			AddLog ("[Camera ERROR] %s test fail.",camera_type);
			if (type == 0)
        	{
            	test_handle->m_testCNF->camera_cnf.b_mainCamera	= false;
        	}
       		else if (type == 1)
        	{
            	test_handle->m_testCNF->camera_cnf.b_subCamera	= false;
        	}
			else if (type == 2)
			{
				test_handle->m_testCNF->camera_cnf.b_main2Camera	= false;
			}
			testResultCode=E_ATADLL_RESULT_FAILED;
		}
		else if(NULL != strstr(atret.resultLst[0].eleLst[0].str_value.c_str(), "not test"))
		{
			AddLog ("[Camera ERROR] %s not test.",camera_type);
			if (type == 0)
        	{
            	test_handle->m_testCNF->camera_cnf.b_mainCamera	= false;
        	}
        	else if (type == 1)
        	{
            	test_handle->m_testCNF->camera_cnf.b_subCamera	= false;
        	}
			else if (type == 2)
			{
				test_handle->m_testCNF->camera_cnf.b_main2Camera= false;
			}
			testResultCode=E_ATADLL_RESULT_NOT_TEST;
		}
		else if(NULL != strstr(atret.resultLst[0].eleLst[0].str_value.c_str(), "Cannot find"))
		{
			AddLog ("[Camera ERROR] %s Not support.",camera_type);
			if (type == 0)
        	{
            	test_handle->m_testCNF->camera_cnf.b_mainCamera	= false;
        	}
        	else if (type == 1)
        	{
            	test_handle->m_testCNF->camera_cnf.b_subCamera	= false;
        	}
			else if (type == 2)
			{
				test_handle->m_testCNF->camera_cnf.b_main2Camera	= false;
			}
			testResultCode=E_ATADLL_RESULT_NOT_SUPPORT;
		}
		else
		{
			AddLog ("[Camera ERROR] %s failed,error code not defined.",camera_type);
			if (type == 0)
        	{
            	test_handle->m_testCNF->camera_cnf.b_mainCamera	= false;
        	}
        	else if (type == 1)
        	{
            	test_handle->m_testCNF->camera_cnf.b_subCamera	= false;
        	}
			else if (type == 2)
			{
				test_handle->m_testCNF->camera_cnf.b_main2Camera	= false;
			}
			testResultCode=E_ATADLL_RESULT_NULL;
		}
		return testResultCode;
	}
	/*m_iWholeCmdLen  = atret.resultLst[0].eleLst[0].str_value.length();
    memset (camera_str, 0x0, sizeof(char)*128);
    memcpy (camera_str, atret.resultLst[0].eleLst[0].str_value.c_str(), m_iWholeCmdLen);
    camera_str[m_iWholeCmdLen] = 0;

    char *camera_flag  = strstr (camera_str, "pass");
    if (camera_flag != NULL)
    {
        AddLog ("%s test ok.", camera_type);
        if (type == 0)
        {
            test_handle->m_testCNF->camera_cnf.b_mainCamera	= true;
        }
        else
        {
            test_handle->m_testCNF->camera_cnf.b_subCamera	= true;
        }
    }
    else
    {
        AddLog ("%s test fail.", camera_type);
        if (type == 0)
        {
            test_handle->m_testCNF->camera_cnf.b_mainCamera	= false;
        }
        else
        {
            test_handle->m_testCNF->camera_cnf.b_subCamera	= false;
        }
    }

    AddLog ("%s test end\r\n", camera_type);
    return E_ATADLL_RESULT_SUCCESS;*/
}

#ifdef __ATA30__
E_ATADLL_RESULT CTestItem_Camera::TransferCameraData ()
{
    char at_buff[64];
    char capturedFileName[64];
    string atstr = "";
    ATResult atret;
    int	loopIndex = 0;

    if (E_CAMERA == test_item)
    {
        sprintf (capturedFileName, "MainCameraCapture.jpg");
    }
    else if (E_CAMERA_SUB == test_item)
    {

        sprintf (capturedFileName, "SubCameraCapture.jpg");
    }
	else if (E_CAMERA_MAIN2 == test_item)
	{
	    sprintf (capturedFileName, "Main2CameraCapture.jpg");   //just add not test now
	}

    ::DeleteFile (capturedFileName);	// delete file with the same name
    FILE *m_fpCameraDataFile = fopen(capturedFileName, "ab+");

    ftm_ata_camera_data camera_data_last;
    ftm_ata_camera_data camera_data_next;
    memset (&camera_data_last, 0x0, sizeof(ftm_ata_camera_data));
    memset (&camera_data_next, 0x0, sizeof(ftm_ata_camera_data));

    AddLog ("Start to transfer camera data.");
    test_handle->m_commonCFG.cbTestProgess (test_handle->handle_index, test_item, 10);

    ClearATResPool ();
    sprintf (at_buff, "AT+CAMERADATA");
    atstr = at_buff;
    atstr += CR;

    AddLog ("Start to receiver camera data block by block.");
    do
    {
        if (*(test_handle->m_commonCFG.stop_flag) == ATA_DLL_STOP_FLAG)
        {
            AddLog ("Stopped by user.");
            return E_ATADLL_RESULT_FAILED;
        }

        AddLog ("Send at command to start transfering.");
        test_handle->m_comm_handle->WriteCommand(atstr.c_str(), atstr.length(), 20*1000, false, atret);

        if (!GetATResponse(atret))
        {
            AddLog ("AT response timeout when getting camera data.");
            return E_ATADLL_RESULT_AT_RESPONSE_TIMEOUT;
        }

        char p_result[1024*160];
        int len = 0;
        test_handle->m_comm_handle->GetCommandResult (p_result, len);
        AddLog ("Received block.");

        /**
        if (loopIndex <= 0)
        {
        	memcpy (&camera_data_next, atret.resultLst[0].eleLst[0].str_char, atret.resultLst[0].eleLst[0].str_len);
        }
        else
        {
        	memcpy (&camera_data_next, atret.resultLst[0].eleLst[0].str_char, atret.resultLst[0].eleLst[0].str_len);
        }
        **/
        memcpy (&camera_data_next, p_result, len);

        AddLog ("Got block %d, Total block number is %d, data length is %d, Str len is %d",
                camera_data_next.current_block,
                camera_data_next.total_block,
                camera_data_next.block_size,
                atret.resultLst[0].eleLst[0].str_len);

        // Current block data will be written to file when receiving the next block
        if (camera_data_next.current_block > 0)
        {
            if (camera_data_last.current_block + 1 == camera_data_next.current_block)
            {
                // Write camera_data_last data into file
                if (fwrite (camera_data_last.camera_data, camera_data_last.block_size, 1, m_fpCameraDataFile) != 1)
                {
                    AddLog ("Write data %d to file %s fail.(%d)", camera_data_last.current_block, capturedFileName, camera_data_last.block_size);
                }
                else
                {
                    AddLog ("Write data %d to file %s successfully.(%d)", camera_data_last.current_block, capturedFileName, camera_data_last.block_size);
                }
                fflush (m_fpCameraDataFile);
            }
            else
            {
                AddLog ("Block number is not in sequence (%d -> %d), receive this block gain.",
                        camera_data_last.current_block,
                        camera_data_next.current_block);

                if (camera_data_next.current_block == camera_data_last.current_block)
                {
                    memcpy (&camera_data_last, &camera_data_next, sizeof(ftm_ata_camera_data));
                }
                continue;
            }
        }
        else
        {
            if (camera_data_next.total_block == 1)
            {
                if (fwrite (camera_data_next.camera_data, camera_data_next.block_size, 1, m_fpCameraDataFile) != 1)
                {
                    AddLog ("Write data to file fail.(%d)", camera_data_next.block_size);
                }
                else
                {
                    AddLog ("Write data to file successfully.(%d)", camera_data_next.block_size);
                }
                fflush (m_fpCameraDataFile);
            }
        }

        if (camera_data_next.current_block >= camera_data_next.total_block - 1)
        {
            AddLog ("Got the last block, receive finished!");
            if (fwrite (camera_data_next.camera_data, camera_data_next.block_size, 1, m_fpCameraDataFile) != 1)
            {
                AddLog ("Write data %d to file %s fail.", camera_data_next.current_block, capturedFileName);
            }
            else
            {
                AddLog ("Write data %d to file %s successfully.(%d)", camera_data_next.current_block, capturedFileName, camera_data_next.block_size);
            }
            fflush (m_fpCameraDataFile);
            break;
        }

        memcpy (&camera_data_last, &camera_data_next, sizeof(ftm_ata_camera_data));

        loopIndex ++;
        //AddLog ("Loopindex: %d", loopIndex);
        AddLog ("--------------------------------------------------------------------");

        // update progress bar every 20 times
        if (loopIndex % 1 == 0)
        {
            int progress = 10 + (int)(90.0 * (float)camera_data_last.current_block/(float)camera_data_last.total_block);
            test_handle->m_commonCFG.cbTestProgess (test_handle->handle_index, test_item, progress>100?100:progress);
        }

        if (loopIndex % 50 == 0)
            Sleep (1000);
    }
    while (1);

    fclose (m_fpCameraDataFile);
    m_fpCameraDataFile = NULL;
    test_handle->m_commonCFG.cbTestProgess (test_handle->handle_index, test_item, 100);
    return E_ATADLL_RESULT_SUCCESS;
}
#endif


