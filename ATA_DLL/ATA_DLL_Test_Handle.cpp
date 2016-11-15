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
*   ATA_DLL_Test_Handle.cpp
*
* Project:
* --------
*   ATA_DLL
*
* Description:
* ------------
*   This module contains the test flow and schedule algorithm
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
#include "Stdafx.h"
#include "GlobalOBJ.h"
#include "ATA_DLL_Test_Handle.h"

CATA_DLL_Test_Handle	*global_test_handle[ATA_DLL_MAX_CONCURRENT_THREADS];

CATA_DLL_Test_Handle::CATA_DLL_Test_Handle (ATA_DLL_Handle *test_handle)
{
    this->test_handle	= test_handle;
	
    final_event			= ::CreateEvent(NULL, FALSE, FALSE, NULL);
    step_final_event	= ::CreateEvent(NULL, FALSE, FALSE, NULL);
    schedule_event		= ::CreateEvent(NULL, FALSE, TRUE, NULL);
    region_event		= ::CreateEvent(NULL, FALSE, TRUE, NULL);
	
    global_test_handle[test_handle->handle_index]	= this;
}

CATA_DLL_Test_Handle::~CATA_DLL_Test_Handle ()
{
    CloseHandle (final_event);
    CloseHandle (schedule_event);
    CloseHandle (region_event);
    CloseHandle (step_final_event);
}

/*
Description:
check if item is conflicting with current running test items.

  return:
		true: yes, there is ruuning test item conflicted
		false: no
*/

bool CATA_DLL_Test_Handle::CheckItemConflictWithRunningItems (E_ATDLL_TEST_ITEM_COUNT item)
{
    int temp_index = 0;
    int inter_index = 0;
    E_ATDLL_TEST_ITEM_COUNT conflict_item = E_TEST_ITEM_COUNT;
	
    // check any
    for (temp_index = 0; temp_index < test_handle->testcases_index_conflict; temp_index ++)
    {
        if (test_handle->testcases_conflict[temp_index].item[0] == E_TEST_ITEM_COUNT)
        {
            conflict_item = test_handle->testcases_conflict[temp_index].item[1];
        }
        else if (test_handle->testcases_conflict[temp_index].item[1] == E_TEST_ITEM_COUNT)
        {
            conflict_item = test_handle->testcases_conflict[temp_index].item[0];
        }
        else
        {
            continue;
        }
		
        if (conflict_item != item)
        {
            for (inter_index = 0; inter_index < test_handle->testcases_index_bg; inter_index ++)
            {
                if (test_handle->testcases_bg[inter_index].item == conflict_item)
                {
                    if (test_handle->testcases_bg[inter_index].item_state == ITEM_STATE_RUNNING)
                    {
                        return true;
                    }
                }
            }
			
            for (inter_index = 0; inter_index < test_handle->testcases_index_fg; inter_index ++)
            {
                if (test_handle->testcases_fg[inter_index].item == conflict_item)
                {
                    if (test_handle->testcases_fg[inter_index].item_state == ITEM_STATE_RUNNING)
                    {
                        return true;
                    }
                }
            }
        }
        else     // conflict_item == item
        {
            for (inter_index = 0; inter_index < test_handle->testcases_index_bg; inter_index ++)
            {
                if (test_handle->testcases_bg[inter_index].item != item)
                {
                    if (test_handle->testcases_bg[inter_index].item_state == ITEM_STATE_RUNNING)
                    {
                        return true;
                    }
                }
            }
			
            for (inter_index = 0; inter_index < test_handle->testcases_index_fg; inter_index ++)
            {
                if (test_handle->testcases_fg[inter_index].item != item)
                {
                    if (test_handle->testcases_fg[inter_index].item_state == ITEM_STATE_RUNNING)
                    {
                        return true;
                    }
                }
            }
        }
    }
	
    // Not any
    for (temp_index = 0; temp_index < test_handle->testcases_index_conflict; temp_index ++)
    {
        if (test_handle->testcases_conflict[temp_index].item[0] == item)
        {
            conflict_item = test_handle->testcases_conflict[temp_index].item[1];
        }
        else if (test_handle->testcases_conflict[temp_index].item[1] == item)
        {
            conflict_item = test_handle->testcases_conflict[temp_index].item[0];
        }
        else
        {
            conflict_item = E_TEST_ITEM_COUNT;
        }
		
        // if find conflicting items
        if (conflict_item < E_TEST_ITEM_COUNT)
        {
            for (inter_index = 0; inter_index < test_handle->testcases_index_bg; inter_index ++)
            {
                if (test_handle->testcases_bg[inter_index].item == conflict_item)
                {
                    if (test_handle->testcases_bg[inter_index].item_state == ITEM_STATE_RUNNING)
                    {
                        return true;
                    }
                }
            }
			
            for (inter_index = 0; inter_index < test_handle->testcases_index_fg; inter_index ++)
            {
                if (test_handle->testcases_fg[inter_index].item == conflict_item)
                {
                    if (test_handle->testcases_fg[inter_index].item_state == ITEM_STATE_RUNNING)
                    {
                        return true;
                    }
                }
            }
			
            break;
        }
    }
	
    return false;
}

/**
Description:
Schedule test item, find the next ready item to start testing.

  Return:
  true: find new ready item, or all available items are running
  false: All items is finished.
  **/
  
  bool CATA_DLL_Test_Handle::ScheduleTestItem ()
  {
	  ::WaitForSingleObject(schedule_event, INFINITE);
	  
	  bool bFinished = true;
	  int loopIndex = 0;
	  int availableItemIndexFg = -1;
	  
	  // Scan background ready item
	  for (loopIndex = 0; loopIndex < test_handle->testcases_index_bg; loopIndex ++)
	  {
		  if (*(test_handle->m_commonCFG.stop_flag) == ATA_DLL_STOP_FLAG)
		  {
			  break;
		  }
		  
		  if ((test_handle->testcases_bg[loopIndex].item_state != ITEM_STATE_FINISHED) && (test_handle->testcases_bg[loopIndex].item_state !=ITEM_STATE_UI_UPDATE))
		  {
			  bFinished = false;
		  }
		  else
		  {
			  continue;
		  }
		  
		  if (test_handle->testcases_bg[loopIndex].item_state == ITEM_STATE_READY)
		  {
			  E_ATDLL_TEST_ITEM_COUNT test_item = test_handle->testcases_bg[loopIndex].item;
			  if (!CheckItemConflictWithRunningItems(test_item))
			  {
				  test_handle->testcases_bg[loopIndex].item_state = ITEM_STATE_RUNNING;
				  g_TestItemObj[test_handle->handle_index][test_item]->SetTestPostProcessCB (TestItemFinishedCallback);
				  g_TestItemObj[test_handle->handle_index][test_item]->RunTest();
				  test_handle->m_testCNF->test_result[test_item]=g_TestItemObj[test_handle->handle_index][test_item]->error_code;
			  }
		  }
	  }
	  
	  // Scan foreground ready item --------------------------
	  for (loopIndex = 0; loopIndex < test_handle->testcases_index_fg; loopIndex ++)
	  {
		  if (*(test_handle->m_commonCFG.stop_flag) == ATA_DLL_STOP_FLAG)
		  {
			  break;
		  }
		  
		  if (test_handle->testcases_fg[loopIndex].item_state != ITEM_STATE_FINISHED)
		  {
			  bFinished = false;
			  
			  // If there is one fore ground item running, stop scanning the next fore ground one.
			  // Because fore ground item is tested one after one
			  if (test_handle->testcases_fg[loopIndex].item_state == ITEM_STATE_RUNNING)
			  {
				  availableItemIndexFg = -1;
				  break;
			  }
		  }
		  else
		  {
			  continue;
		  }
		  
		  // Record the first available item index
		  // Fore ground items are tested one after one
		  if (availableItemIndexFg == -1)
		  {
			  if (test_handle->testcases_fg[loopIndex].item_state == ITEM_STATE_READY)
			  {
				  if (!CheckItemConflictWithRunningItems(test_handle->testcases_fg[loopIndex].item))
				  {
					  availableItemIndexFg = loopIndex;
				  }
			  }
		  }
	  }
	  
	  // If found the available fore ground test item, start to test
	  if (E_OFFMODE ==test_handle->testcases_fg[availableItemIndexFg].item)  //mingxue20150806
	  	{
	  	 test_handle->testcases_fg[availableItemIndexFg].item_state = ITEM_STATE_FINISHED;
		 bFinished = true;
	  	}
	  else if (availableItemIndexFg >= 0 && availableItemIndexFg < test_handle->testcases_index_fg && *(test_handle->m_commonCFG.stop_flag) != ATA_DLL_STOP_FLAG)
	  {
		  E_ATDLL_TEST_ITEM_COUNT test_item = test_handle->testcases_fg[availableItemIndexFg].item;
		  glb_test_item[test_handle->handle_index] = test_item;
		  test_handle->testcases_fg[availableItemIndexFg].item_state = ITEM_STATE_RUNNING;
		  g_TestItemObj[test_handle->handle_index][test_item]->SetTestPostProcessCB (TestItemFinishedCallback);
		  g_TestItemObj[test_handle->handle_index][test_item]->RunTest();
		  test_handle->m_testCNF->test_result[test_item]=g_TestItemObj[test_handle->handle_index][test_item]->error_code;
	  }
	  
	  // if stopped by user and there is no item running, return false
	  if (*(test_handle->m_commonCFG.stop_flag) == ATA_DLL_STOP_FLAG)
	  {
		  E_ATDLL_TEST_ITEM_COUNT test_item = E_LCD;
		  bool bRunning = false;
		  for (loopIndex = 0; loopIndex < test_handle->testcases_index_bg + test_handle->testcases_index_fg; loopIndex ++)
		  {
			  if (loopIndex < test_handle->testcases_index_bg)
			  {
				  if (test_handle->testcases_bg[loopIndex].item_state == ITEM_STATE_RUNNING)
				  {
					  bFinished = false;
					  break;
				  }
			  }
			  else
			  {
				  if (test_handle->testcases_fg[loopIndex-test_handle->testcases_index_bg].item_state == ITEM_STATE_RUNNING)
				  {
					  bFinished = false;
					  break;
				  }
			  }
		  }
	  }
	  
	  ::SetEvent (schedule_event);
	  return !bFinished;
}


/**
Decription:
This callback function will be called when one item finished.
**/
void __stdcall CATA_DLL_Test_Handle::TestItemFinishedCallback (int handle_index, E_ATDLL_TEST_ITEM_COUNT item)
{
	
    ::WaitForSingleObject(global_test_handle[handle_index]->region_event, INFINITE);
	
    int temp_index = 0;
    for (temp_index = 0; temp_index < global_test_handle[handle_index]->test_handle->testcases_index_bg; temp_index ++)
    {
        if (item == global_test_handle[handle_index]->test_handle->testcases_bg[temp_index].item)
        {
            global_test_handle[handle_index]->test_handle->testcases_bg[temp_index].item_state = ITEM_STATE_FINISHED;
			
			// stop if failed
			if (global_test_handle[handle_index]->test_handle->m_commonCFG.stop_if_failed)
			{
				bool bResult;
				global_test_handle[handle_index]->test_handle->m_commonCFG.cbQueryTestItemResult(handle_index, item, &bResult);
				if (!bResult)
				{
					*(global_test_handle[handle_index]->test_handle->m_commonCFG.stop_flag) = ATA_DLL_STOP_FLAG;
				}
			}
            break;
        }
    }
	
    for (temp_index = 0; temp_index < global_test_handle[handle_index]->test_handle->testcases_index_fg; temp_index ++)
    {
        if (item == global_test_handle[handle_index]->test_handle->testcases_fg[temp_index].item)
        {
            global_test_handle[handle_index]->RequestAndParseData (false);
			global_test_handle[handle_index]->test_handle->testcases_fg[temp_index].item_state = ITEM_STATE_FINISHED;
			
			// Update result onto UI
			for (int temp_index_bg = 0; temp_index_bg < global_test_handle[handle_index]->test_handle->testcases_index_bg; temp_index_bg ++)
			{
				//if (global_test_handle[handle_index]->test_handle->testcases_bg[temp_index_bg].item_state == ITEM_STATE_FINISHED)
				if (global_test_handle[handle_index]->test_handle->testcases_bg[temp_index_bg].item_state == ITEM_STATE_FINISHED 
					&& ITEM_STATE_UI_UPDATE !=global_test_handle[handle_index]->test_handle->testcases_bg[temp_index_bg].item_state)//to avoid update ui many times
				{
					global_test_handle[handle_index]->test_handle->m_commonCFG.cbUpdateResult (handle_index, global_test_handle[handle_index]->test_handle->testcases_bg[temp_index_bg].item);
					global_test_handle[handle_index]->test_handle->testcases_bg[temp_index_bg].item_state = ITEM_STATE_UI_UPDATE;//to avoid update ui many times
					
				}
			}
			
		//	global_test_handle[handle_index]->test_handle->m_commonCFG.cbUpdateResult (handle_index, item);
			
			// stop if failed
			if (global_test_handle[handle_index]->test_handle->m_commonCFG.stop_if_failed)
			{
				bool bResult;
				global_test_handle[handle_index]->test_handle->m_commonCFG.cbQueryTestItemResult(handle_index, item, &bResult);
				if (!bResult)
				{
					*(global_test_handle[handle_index]->test_handle->m_commonCFG.stop_flag) = ATA_DLL_STOP_FLAG;
				}
			}
			global_test_handle[handle_index]->test_handle->m_commonCFG.cbUpdateResult (handle_index, item);
			
					
            break;
        }
    }
	
    if (*(global_test_handle[handle_index]->test_handle->m_commonCFG.stop_flag) == ATA_DLL_STOP_FLAG)
    {
        // If no item is running, exit the test flow (do not continue scheduling new ready items)
        bool bNoRunning = true;
        for (int loopIndex = 0; loopIndex < global_test_handle[handle_index]->test_handle->testcases_index_bg; loopIndex ++)
        {
            if (global_test_handle[handle_index]->test_handle->testcases_bg[loopIndex].item_state == ITEM_STATE_RUNNING)
            {
                bNoRunning = false;
            }
        }
        for (loopIndex = 0; loopIndex < global_test_handle[handle_index]->test_handle->testcases_index_fg; loopIndex ++)
        {
            if (global_test_handle[handle_index]->test_handle->testcases_fg[loopIndex].item_state == ITEM_STATE_RUNNING)
            {
                bNoRunning = false;
            }
        }
        if (bNoRunning)
        {
            ::SetEvent(global_test_handle[handle_index]->final_event);
        }
    }
    else
    {
        if (!global_test_handle[handle_index]->ScheduleTestItem())
        {
            ::SetEvent(global_test_handle[handle_index]->final_event);
        }
    }
	
    ::SetEvent(global_test_handle[handle_index]->region_event);
}


bool CATA_DLL_Test_Handle::StartTest()
{
    int loopIndex = 0;
	
    // Reset all items state to ready.
    for (loopIndex = 0; loopIndex < test_handle->testcases_index_bg + test_handle->testcases_index_fg; loopIndex ++)
    {
        if (loopIndex < test_handle->testcases_index_bg)
        {
            test_handle->testcases_bg[loopIndex].item_state = ITEM_STATE_READY;
        }
        else
        {
            test_handle->testcases_fg[loopIndex - test_handle->testcases_index_bg].item_state = ITEM_STATE_READY;
			
			if (test_handle->testcases_fg[loopIndex - test_handle->testcases_index_bg].item == E_MHL)
			{
				test_handle->testcases_fg[loopIndex - test_handle->testcases_index_bg].item_state = ITEM_STATE_FINISHED;
			}
        }
    }
	
    // Schedule item to start testing
    if (ScheduleTestItem())
    {
        test_handle->Log_Record ("Start test items.\r\n");
    }
    else
    {
        test_handle->Log_Record ("There is no test items.\r\n");
        ::SetEvent(final_event);
    }
	
    // if stopped by user
    if (*(test_handle->m_commonCFG.stop_flag) == ATA_DLL_STOP_FLAG)
    {
        test_handle->Log_Record ("Stopped by user!\r\n");
    }
	
    // Wait both background and foreground items finished
    // timeout = 10 miniutes
    ::WaitForSingleObject(final_event, 1000*60*10);
    return true;
	
}

/**
Decription:
This callback function will be called when one item finished.
used for step debug
**/
void __stdcall CATA_DLL_Test_Handle::TestItemStepFinishedCallback (int handle_index, E_ATDLL_TEST_ITEM_COUNT item)
{
    ::WaitForSingleObject(global_test_handle[handle_index]->region_event, INFINITE);
	
    int temp_index = 0;
    for (temp_index = 0; temp_index < global_test_handle[handle_index]->test_handle->testcases_index_fg; temp_index ++)
    {
        if (item == global_test_handle[handle_index]->test_handle->testcases_fg[temp_index].item)
        {
            global_test_handle[handle_index]->test_handle->testcases_fg[temp_index].item_state = ITEM_STATE_FINISHED;
            break;
        }
    }
    ::SetEvent(global_test_handle[handle_index]->step_final_event);
	
    ::SetEvent(global_test_handle[handle_index]->region_event);
}

// Only for forground items
bool CATA_DLL_Test_Handle::StartTestStep(E_ATDLL_TEST_ITEM_COUNT item)
{
    int loopIndex = 0;
	
    // Start to test
    if (!CheckItemConflictWithRunningItems(item))
    {
        for (loopIndex = 0; loopIndex < test_handle->testcases_index_fg; loopIndex ++)
        {
            if (item == test_handle->testcases_fg[loopIndex].item)
            {
                glb_test_item[test_handle->handle_index] = item;
                test_handle->testcases_fg[loopIndex].item_state = ITEM_STATE_RUNNING;
                g_TestItemObj[test_handle->handle_index][item]->SetTestPostProcessCB (TestItemStepFinishedCallback);
                g_TestItemObj[test_handle->handle_index][item]->RunTest();
				
                // Wait foreground items finished
                // timeout = 10 miniutes
                ::WaitForSingleObject(step_final_event, 1000*60*10);
                return true;
            }
        }
    }
	
    return false;
}


/**
Decription:
Request test result data from target
**/
bool CATA_DLL_Test_Handle::RequestAndParseData (bool bLogPrint)
{
    sp_ata_data s_data;
    memset (&s_data, 0x0, sizeof(sp_ata_data));
	
    int loopIndex = 0;
    E_ATDLL_TEST_ITEM_COUNT test_item = E_LCD;
    for (loopIndex = 0; loopIndex < test_handle->testcases_index_bg+test_handle->testcases_index_fg; loopIndex ++)
    {
        if (loopIndex < test_handle->testcases_index_bg)
        {
            test_item = test_handle->testcases_bg[loopIndex].item;
        }
        else
        {
            test_item = test_handle->testcases_fg[loopIndex-test_handle->testcases_index_bg].item;
        }
		
        if (g_TestItemObj[test_handle->handle_index][test_item] != NULL)
        {
            glb_at_resp_flag = true;
            glb_test_item[test_handle->handle_index] = test_item;
            E_ATADLL_RESULT ata_result = g_TestItemObj[test_handle->handle_index][test_item]->RequestData (s_data);
            glb_at_resp_flag = false;
			
            if (E_ATADLL_RESULT_SUCCESS != ata_result)
            {
                test_handle->Log_Record ("[Error] Request data fail!!\r\n");
                return false;
            }
			
            test_handle->Log_Record ("[OK] Request data successfully.\r\n");
            break;
        }
    }
	
    int index = 0;
    for (loopIndex = 0; loopIndex < test_handle->testcases_index_bg+test_handle->testcases_index_fg; loopIndex ++)
    {
        if (loopIndex < test_handle->testcases_index_bg)
        {
            test_item = test_handle->testcases_bg[loopIndex].item;
        }
        else
        {
            test_item = test_handle->testcases_fg[loopIndex-test_handle->testcases_index_bg].item;
        }
		
        //if (test_handle->testcases[loopIndex].testcase_seq == testcase_seq)
        {
            //if (g_TestItemObj[test_handle->handle_index][test_handle->testcases[loopIndex].item] != NULL)
            {
                switch (test_item)
                {
                case E_WIFI:
					if (bLogPrint) {
						test_handle->Log_Record ("[WIFI] WIFI MAC: %s, Name: %s, RSSI:%d\r\n",
							s_data.wifi.wifi_mac, s_data.wifi.wifi_name, s_data.wifi.wifi_rssi);
					}
                    memcpy (&(test_handle->m_testCNF->wifi_cnf.ftm_wifi), &(s_data.wifi), sizeof(ftm_ata_wifi));
                    break;
					
                case E_BT:
                    for (index = 0; index < s_data.bt.num; index ++)
                    {
						if (bLogPrint) {
							test_handle->Log_Record ("[BT] %s, %s, %d\r\n", s_data.bt.bt[index].bt_mac, s_data.bt.bt[index].bt_name, s_data.bt.bt[index].bt_rssi);
						}
                    }
                    break;
					
                case E_SWVERSION:
                    sprintf (test_handle->m_testCNF->sw_ver_cnf.modem_ver, s_data.version.modem_ver);
                    sprintf (test_handle->m_testCNF->sw_ver_cnf.ap_ver, s_data.version.sw_ver);
#if 0
                    memcpy (&test_handle->m_testCNF->imei_cnf, &s_data.imei, sizeof(ftm_ata_imei));
					for (index = 0; index < s_data.imei.number; index ++)
                    {
                        test_handle->Log_Record ("IMEI%d:%s\r\n", index+1, s_data.imei.imei[index]);
                    }
#endif
					if (bLogPrint) {
						test_handle->Log_Record ("[Modem SW Version] %s\r\n", test_handle->m_testCNF->sw_ver_cnf.modem_ver);
						test_handle->Log_Record ("[AP SW Version] %s\r\n", test_handle->m_testCNF->sw_ver_cnf.ap_ver);
					}
                    break;
                case E_CHARGING:
                    //if (s_data.battery.current < 1000)
                   // {
                        test_handle->m_testCNF->adc_cnf.meas_charger_current = s_data.battery.current;
                    //}
                   /* else
                    {
                        test_handle->m_testCNF->adc_cnf.meas_charger_current = s_data.battery.voltage;
                    }*/
					
					if (bLogPrint) {
						test_handle->Log_Record ("[Charger] Current: %.3f mA\r\n", test_handle->m_testCNF->adc_cnf.meas_charger_current);
					}
					
#if (defined __ATA30__) || (defined __ATA_LOAD30_TEST20__)
                    test_handle->m_testCNF->adc_cnf.vbattemp = s_data.battery.vbattemp;
					if (bLogPrint) {
						test_handle->Log_Record ("Battery temparature: %d\r\n", s_data.battery.vbattemp);
					}
#endif
                    break;
                case E_ADC:
                    if (s_data.battery.current < 1000.0)
                    {
                        test_handle->m_testCNF->adc_cnf.meas_v1	= ((double)s_data.battery.voltage)/1000.0;
                    }
                    else
                    {
                        test_handle->m_testCNF->adc_cnf.meas_v1	= ((double)s_data.battery.current)/1000.0;
                    }
					if (bLogPrint) {
						test_handle->Log_Record ("[ADC] Voltage: %.3f V\r\n", test_handle->m_testCNF->adc_cnf.meas_v1);
					}
                    break;
                case E_TCARD:
#if (defined __ATA30__) || (defined __ATA_LOAD30_TEST20__)
                    memcpy (&test_handle->m_testCNF->memcard_cnf.memcard, &s_data.memcard, sizeof(ftm_ata_memcard));
					if (bLogPrint) {
						test_handle->Log_Record("Memory Card SD1 total size: %d MB\r\n", test_handle->m_testCNF->memcard_cnf.memcard.sd1_total_size);
						test_handle->Log_Record("Memory Card SD1 free size: %d MB\r\n", test_handle->m_testCNF->memcard_cnf.memcard.sd1_free_size);
						test_handle->Log_Record("Memory Card SD2 total size: %d MB\r\n", test_handle->m_testCNF->memcard_cnf.memcard.sd2_total_size);
						test_handle->Log_Record("Memory Card SD2 free size: %d MB\r\n", test_handle->m_testCNF->memcard_cnf.memcard.sd2_free_size);
					}
#endif
                    break;
                case E_EMMC:
#if (defined __ATA30__) || (defined __ATA_LOAD30_TEST20__)
                    memcpy (&test_handle->m_testCNF->emmc_cnf.emmc, &s_data.emmc, sizeof(ftm_ata_emmc));
					if (bLogPrint) {
						test_handle->Log_Record ("EMMC total size: %.3f GB\r\n", test_handle->m_testCNF->emmc_cnf.emmc.capacity);
					}
#endif
                    break;
                case E_FM:
                    if (s_data.fm.fm_rssi == 0)
                    {
                        test_handle->m_testCNF->fm_cnf.cqi = -500;
                    }
                    else
                    {
                        test_handle->m_testCNF->fm_cnf.cqi = s_data.fm.fm_rssi;
                    }
					if (bLogPrint) {
						test_handle->Log_Record ("[FM] RSSI result: %d\r\n", test_handle->m_testCNF->fm_cnf.cqi);
					}
					
#if (defined __ATA30__) || (defined __ATA_LOAD30_TEST20__)
					//test_handle->Log_Record ("FM Freq: %d, Ampl: %d\r\n", s_data.fm.freq, s_data.fm.ampl);
#endif
					
                    break;
                case E_SPEAKER:
                    test_handle->m_testCNF->audio_cnf.speaker_data = s_data.speaker;
					if (bLogPrint) {
						test_handle->Log_Record ("[Audio] Speaker: left freq: %d, right freq: %d \t left amp: %d, right amp: %d\r\n",
							s_data.speaker.freqL, s_data.speaker.freqR, s_data.speaker.amplL, s_data.speaker.amplR);
					}
					
#if (defined __ATA30__) || (defined __ATA_LOAD30_TEST20__)
					if (bLogPrint) {
						for (index = 0; index < 5; index ++)
						{
							test_handle->Log_Record ("[Audio][Speaker] Freq response: mean(%.5f), deviation(%.5f), min(%.5f), max(%.5f)\r\n",
								s_data.spk_response.freqresponse[index].mean,
								s_data.spk_response.freqresponse[index].deviation,
								s_data.spk_response.freqresponse[index].min,
								s_data.spk_response.freqresponse[index].max );
							
						}
						
						test_handle->Log_Record ("[Audio][Speaker] THD: mean(%.5f), deviation(%.5f), min(%.5f), max(%.5f)\r\n",
							s_data.spk_thd.thd.mean,
							s_data.spk_thd.thd.deviation,
							s_data.spk_thd.thd.min,
							s_data.spk_thd.thd.max );
					}
					
#endif
					
                    break;
                case E_RECEIVER:
                    test_handle->m_testCNF->audio_cnf.receiver_data = s_data.receiver;
					if (bLogPrint) {
						test_handle->Log_Record ("[Audio] Receiver:\r\n\t left freq: %d, right freq: %d \t left amp: %d, right amp: %d\r\n",
							s_data.receiver.freqL, s_data.receiver.freqR, s_data.receiver.amplL, s_data.receiver.amplR);
					}
					
					
#if (defined __ATA30__) || (defined __ATA_LOAD30_TEST20__)
					if (bLogPrint) {
						for (index = 0; index < 5; index ++)
						{
							test_handle->Log_Record ("[Audio][Receiver] Freq response: mean(%.5f), deviation(%.5f), min(%.5f), max(%.5f)\r\n",
								s_data.rcv_response.freqresponse[index].mean,
								s_data.rcv_response.freqresponse[index].deviation,
								s_data.rcv_response.freqresponse[index].min,
								s_data.rcv_response.freqresponse[index].max );
							
						}
						
						test_handle->Log_Record ("[Audio][Receiver] THD: mean(%.5f), deviation(%.5f), min(%.5f), max(%.5f)\r\n",
							s_data.rcv_thd.thd.mean,
							s_data.rcv_thd.thd.deviation,
							s_data.rcv_thd.thd.min,
							s_data.rcv_thd.thd.max );
					}
					
#endif
					
                    break;
                case E_EARTHPHONE:
                    test_handle->m_testCNF->audio_cnf.headset_data = s_data.headset;
					test_handle->m_testCNF->audio_cnf.headset_data_L = s_data.headsetL;
					test_handle->m_testCNF->audio_cnf.headset_data_R = s_data.headsetR;
					
					if (bLogPrint) {
						test_handle->Log_Record ("[Audio] Headset:\r\n\t left freq: %d, right freq: %d\r\n\t left amp: %d, right amp: %d\r\n",
							s_data.headset.freqL, s_data.headset.freqR, s_data.headset.amplL, s_data.headset.amplR);
					}
					
#if (defined __ATA30__) || (defined __ATA_LOAD30_TEST20__)
					
					if (bLogPrint) {
						test_handle->Log_Record ("[Audio][HeadsetL][Common]:\r\n\t left freq: %d, right freq: %d\r\n\t left amp: %d, right amp: %d\r\n",
							s_data.headsetL.freqL, s_data.headsetL.freqR, s_data.headsetL.amplL, s_data.headsetL.amplR);
						
						test_handle->Log_Record ("[Audio][HeadsetR][Common]:\r\n\t left freq: %d, right freq: %d\r\n\t left amp: %d, right amp: %d\r\n",
							s_data.headsetR.freqL, s_data.headsetR.freqR, s_data.headsetR.amplL, s_data.headsetR.amplR);
						
						test_handle->Log_Record ("[Audio][HeadsetL] THD: mean(%.5f), deviation(%.5f), min(%.5f), max(%.5f)\r\n",
							s_data.headsetL_thd.thd.mean,
							s_data.headsetL_thd.thd.deviation,
							s_data.headsetL_thd.thd.min,
							s_data.headsetL_thd.thd.max );
						
						
						test_handle->Log_Record ("[Audio][HeadsetR] THD: mean(%.5f), deviation(%.5f), min(%.5f), max(%.5f)\r\n",
							s_data.headsetR_thd.thd.mean,
							s_data.headsetR_thd.thd.deviation,
							s_data.headsetR_thd.thd.min,
							s_data.headsetR_thd.thd.max );
					}
					
#endif
                    break;
                case E_GSENSOR:
                    memcpy (&test_handle->m_testCNF->sensor_cnf.gsensor, &s_data.gsensor, sizeof(ftm_ata_gsensor));
					if (bLogPrint) {
						test_handle->Log_Record ("G-Sensor: x - (%f), y - (%f), z - (%f), accuracy - (%d)\r\n",
							s_data.gsensor.g_sensor_x, s_data.gsensor.g_sensor_y, s_data.gsensor.g_sensor_z, s_data.gsensor.accuracy);
					}
                    break;
					
                case E_MSENSOR:
                    memcpy (&test_handle->m_testCNF->sensor_cnf.msensor, &s_data.msensor, sizeof(ftm_ata_msensor));
                    if (bLogPrint) {
                        test_handle->Log_Record ("M-Sensor: x - (%d), y - (%d), z - (%d), accurary - (%d)\r\n",
							s_data.msensor.m_sensor_x, s_data.msensor.m_sensor_y, s_data.msensor.m_sensor_z, s_data.msensor.accuracy);
                    }
                    break;
                case E_ALSPS:
                    memcpy (&test_handle->m_testCNF->sensor_cnf.alsps, &s_data.alsps, sizeof(ftm_ata_alsps));
                    if (bLogPrint) {
                        test_handle->Log_Record ("ALS/PS: ALS - (%d), PS - (%d)\r\n", s_data.alsps.als, s_data.alsps.ps);
                    }
                    break;
                case E_GYROSCOPE:
                    memcpy (&test_handle->m_testCNF->sensor_cnf.gyroscope, &s_data.gyroscope, sizeof(ftm_ata_gyroscope));
                    if (bLogPrint) {
                        test_handle->Log_Record ("GYROSCOPE: x - (%f), y - (%f), z - (%f), accuracy - (%d)\r\n",
							s_data.gyroscope.gyroscope_x, s_data.gyroscope.gyroscope_y, s_data.gyroscope.gyroscope_z, s_data.gyroscope.accuracy);
                    }
                    break;
					
#if (defined __ATA30__) || (defined __ATA_LOAD30_TEST20__)
				case E_VIBRATOR:
					if (bLogPrint) {
						test_handle->Log_Record ("Vibrator Freq: %d, Ampl: %d\r\n", s_data.vibrator.freq, s_data.vibrator.ampl);
					}
					break;
#endif
					
#if (defined __ATA30__) || (defined __ATA_LOAD30_TEST20__)
				case E_RSSI:
					test_handle->m_testCNF->rssi_cnf.dBm = s_data.rf.rf_data;
					test_handle->m_testCNF->rssi_cnf.dBm_lte = s_data.rf.rf_data_lte;
					if (bLogPrint) {
						test_handle->Log_Record ("2G Rssi level: %d\r\n", s_data.rf.rf_data);
						test_handle->Log_Record ("LTE Rssi level: %d\r\n", s_data.rf.rf_data_lte);
					}
					break;
				case E_BTS:
					
					for (index = 0; index < s_data.bts.num; index ++)
                    {
						if (bLogPrint) {
							test_handle->Log_Record ("[BTS] sensor_name: %s, temperature: %.1f\r\n", s_data.bts.bts[index].bts_sensor_name, s_data.bts.bts[index].bts_temperature);
						}
						memcpy (&(test_handle->m_testCNF->bts_cnf.bts[index]), &(s_data.bts.bts), sizeof(ftm_ata_bts));
                    }
					
					break;
					
#endif
				//add bug_id:none songjian 2016-3-1 (start)
				//case E_GPS_CALI:
                    //sprintf (test_handle->m_testCNF->sw_ver_cnf.modem_ver, s_data.version.modem_ver);
                    //sprintf (test_handle->m_testCNF->sw_ver_cnf.ap_ver, s_data.version.sw_ver);
#if 0
                    memcpy (&test_handle->m_testCNF->imei_cnf, &s_data.imei, sizeof(ftm_ata_imei));
					for (index = 0; index < s_data.imei.number; index ++)
                    {
                        test_handle->Log_Record ("IMEI%d:%s\r\n", index+1, s_data.imei.imei[index]);
                    }
#endif
					//if (bLogPrint) {
					//	test_handle->Log_Record ("[Modem SW Version] %s\r\n", test_handle->m_testCNF->sw_ver_cnf.modem_ver);
					//	test_handle->Log_Record ("[AP SW Version] %s\r\n", test_handle->m_testCNF->sw_ver_cnf.ap_ver);
					//}
                 //   break;
					//add bug_id:none songjian 2016-3-1 (end)
					
                }
				
				if (bLogPrint) {
					test_handle->Log_Record ("\r\n");
				}
        }
    }
    }
	
    return true;
}



