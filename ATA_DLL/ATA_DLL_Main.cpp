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
 *   ATA_DLL_Main.cpp
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
#include "ATA_DLL.h"
#include "ATA_DLL_Handle.h"
#include "GlobalOBJ.h"

int sp_meta_handle;

#if _MSC_VER <= 1200 // MFC 6.0 or previous
int sprintf_s(char* buf, size_t size, const char* fmt, ...)
{
    size_t sz;
    va_list ap;
    va_start(ap, fmt);
    sz = vsprintf(buf, fmt, ap);
    va_end(ap);
    return sz;
}
char* strcpy_s(char* dst, size_t size, const char* src)
{
    return strcpy(dst, src);
}
char* strcat_s(char* dst, size_t size, const char* src)
{
    return strcat(dst, src);
}
char* strtok_s(char* _Str, const char* _Delim, char** _Context)
{
    return strtok(_Str, _Delim);
}
char* strncpy_s(char* _Dst, size_t _SizeInBytes, const char* _Src, size_t _MaxCount)
{
    return strncpy(_Dst, _Src, _MaxCount);
}
#endif // #if _MSC_VER <= 1200 // MFC 6.0 or previous

/**
 * Descriptions: Get Availble Handle
 */
E_ATADLL_RESULT __stdcall ATADLL_GetAvailbleHandle (int *ata_handle)
{
    int temp_handle;
    for (temp_handle = 0; temp_handle < ATA_DLL_MAX_CONCURRENT_THREADS; temp_handle ++)
    {
        if (g_ADHA.IsActiveHandle(temp_handle))
        {
            if (0 == g_ADHA.OccupyHandle(temp_handle))
            {
                *ata_handle = temp_handle;
                g_ADHA[temp_handle].handle_index = temp_handle;
                break;
            }
        }
    }
    if (temp_handle >= ATA_DLL_MAX_CONCURRENT_THREADS)
    {
        return E_ATADLL_RESULT_NO_ENOUGH_HANDLE;
    }

    TRACE("ATADLL_GetAvailbleHandle:%d\r\n", temp_handle);
    return E_ATADLL_RESULT_SUCCESS;
}

/**
 * Descriptions: Release handle
 */
E_ATADLL_RESULT __stdcall ATADLL_ReleaseHandle (const int ata_handle)
{
    int result = g_ADHA.ReleaseHandle (ata_handle);
    if (0 != result)
    {
        return E_ATADLL_RESULT_INVALID_HANDLE_INDEX;
    }

    return E_ATADLL_RESULT_SUCCESS;
}

/**
 * Descriptions: GetTestCase
 */
void __stdcall ATADLL_GetTestCase_BG(const int ata_handle, const S_ATADLL_TEST_ITEM_T	*test_item_cfg, S_ATADLL_TESTCASE_T *testcase, int *testcase_number)
{
	MTRACE(g_hEBOOT_DEBUG, "[Thread%d] Entry ATADLL_GetTestCase_BG()",ata_handle+1);
    memcpy (&g_ADHA[ata_handle].m_testItemCFG, test_item_cfg, sizeof(S_ATADLL_TEST_ITEM_T));
    g_ADHA[ata_handle].ParseIniFile ();

    memcpy (testcase, g_ADHA[ata_handle].testcases_bg, MAX_TEST_CASE_NUM*sizeof(S_ATADLL_TESTCASE_T));
    *testcase_number = g_ADHA[ata_handle].testcases_index_bg;
}

void __stdcall ATADLL_GetTestCase_FG(const int ata_handle, const S_ATADLL_TEST_ITEM_T *test_item_cfg, S_ATADLL_TESTCASE_T *testcase, int *testcase_number)
{
	MTRACE(g_hEBOOT_DEBUG, "[Thread%d] Entry ATADLL_GetTestCase_FG()",ata_handle+1);

    memcpy (&g_ADHA[ata_handle].m_testItemCFG, test_item_cfg, sizeof(S_ATADLL_TEST_ITEM_T));
    g_ADHA[ata_handle].ParseIniFile ();

    memcpy (testcase, g_ADHA[ata_handle].testcases_fg, MAX_TEST_CASE_NUM*sizeof(S_ATADLL_TESTCASE_T));
    *testcase_number = g_ADHA[ata_handle].testcases_index_fg;
}


/**
 * Descriptions: Start Test
 */
E_ATADLL_RESULT __stdcall ATADLL_StartTest (const int ata_handle,
        const S_ATADLL_TEST_ITEM_T		*test_item_cfg,
        S_ATADLL_COMMON_CFG_T		*comm_cfg,
        S_ATADLL_TEST_CNF				*test_cnf
                                           )
{
    TRACE("%d: Start ATADLL_StartTest.\r\n", ata_handle);
	_Xboot_DebugOn();
	MTRACE(g_hEBOOT_DEBUG, "[Thread%d] ATADLL_StartTest",ata_handle+1);

    memcpy (&g_ADHA[ata_handle].m_testItemCFG, test_item_cfg, sizeof(S_ATADLL_TEST_ITEM_T));
    memcpy (&g_ADHA[ata_handle].m_commonCFG, comm_cfg, sizeof(S_ATADLL_COMMON_CFG_T));
    g_ADHA[ata_handle].m_testCNF = test_cnf;

    // Firstly, Save log to tempx.txt (x = 1, 2, 3 ,4)
    // then rename log file by barcode or data after test finished.
    sprintf (g_ADHA[ata_handle].m_commonCFG.log_file, "temp%d.txt", ata_handle+1);
	sprintf (g_ADHA[ata_handle].m_commonCFG.log_csv_file, "temp%d.csv", ata_handle+1);

    E_ATADLL_RESULT ata_result = g_ADHA[ata_handle].RunTest(ata_handle);
    return ata_result;
}

E_ATADLL_RESULT __stdcall ATADLL_StartTestStep (const int ata_handle,
        E_ATDLL_TEST_ITEM_COUNT			test_item,
        const S_ATADLL_TEST_ITEM_T		*test_item_cfg,
        S_ATADLL_COMMON_CFG_T			*comm_cfg,
        S_ATADLL_TEST_CNF				*test_cnf
                                               )
{
    TRACE("%d: Start ATADLL_StartTest.\r\n", ata_handle);

    memcpy (&g_ADHA[ata_handle].m_testItemCFG, test_item_cfg, sizeof(S_ATADLL_TEST_ITEM_T));
    memcpy (&g_ADHA[ata_handle].m_commonCFG, comm_cfg, sizeof(S_ATADLL_COMMON_CFG_T));
    g_ADHA[ata_handle].m_testCNF = test_cnf;

    // Firstly, Save log to tempx.txt (x = 1, 2, 3 ,4)
    // then rename log file by barcode or data after test finished.
    sprintf (g_ADHA[ata_handle].m_commonCFG.log_file, "temp%d.txt", ata_handle+1);

    E_ATADLL_RESULT ata_result = g_ADHA[ata_handle].RunTestStep (test_item);
    return ata_result;
}


E_ATADLL_RESULT __stdcall ATADLL_StopTest (const int ata_handle)
{
    return g_ADHA[ata_handle].StopTest(ata_handle);
}

void __stdcall ATADLL_AddLog (const int ata_handle,  char* logstr)
{
	strcat(logstr,"\r\n");
	//g_ADHA[ata_handle].bLogWithDateInfo = false;
    g_ADHA[ata_handle].Log_Record (logstr);
	//g_ADHA[ata_handle].bLogWithDateInfo = true;
   
}
void __stdcall ATADLL_CSVLog(const int ata_handle,char* log_buff)
{
	g_ADHA[ata_handle].AddCSVLog(log_buff);
}


E_ATADLL_RESULT __stdcall ATADLL_UpdateBarcodeFlag (const int ata_handle, bool bPass)
{
    return g_ADHA[ata_handle].UpdateBarcodeFlag (ata_handle, bPass);
}

E_ATADLL_RESULT __stdcall ATADLL_TestOffCurrent (const int ata_handle)
{
    return g_ADHA[ata_handle].TestOffCurrent();
}

void __stdcall ATADLL_UpdateStopFlag (const int ata_handle)
{
	TRACE ("Enter ATADLL_UpdateStopFlag()\r\n");
	MTRACE(g_hEBOOT_DEBUG,"Enter ATADLL_UpdateStopFlag()");
    //ATA_DLL_Handle handle = g_ADHA[ata_handle];
    if (*(g_ADHA[ata_handle].m_commonCFG.stop_flag) == ATA_DLL_STOP_FLAG)
    {
     
        for ( E_ATDLL_TEST_ITEM_COUNT test_item = E_LCD;
                test_item < E_TEST_ITEM_COUNT;
                test_item = (E_ATDLL_TEST_ITEM_COUNT)((int)test_item + 1) )
        {
			TRACE ("Stop %d\r\n", test_item);
            if (g_TestItemObj[g_ADHA[ata_handle].handle_index][test_item] != NULL)
            {
				TRACE ("	Stop %d OK\r\n", test_item);
                g_TestItemObj[g_ADHA[ata_handle].handle_index][test_item]->StopTestAfterAll();
            }
        }
    }

	TRACE ("Leave ATADLL_UpdateStopFlag()\r\n");
	MTRACE(g_hEBOOT_DEBUG,"Leave ATADLL_UpdateStopFlag()");
}

const char*__stdcall AtaStatusToString(int status)
{

	switch(status) 
	{
	ENUM_TO_CASE_STRING(E_ATADLL_RESULT_SUCCESS)
	ENUM_TO_CASE_STRING(E_ATADLL_RESULT_FAILED)
	ENUM_TO_CASE_STRING(E_ATADLL_RESULT_BOX_FAILED)
	ENUM_TO_CASE_STRING(E_ATADLL_RESULT_FORMAT_INCORRECT)
	ENUM_TO_CASE_STRING(E_ATADLL_RESULT_NO_ENOUGH_HANDLE)
    ENUM_TO_CASE_STRING(E_ATADLL_RESULT_INVALID_HANDLE_INDEX)

    ENUM_TO_CASE_STRING(E_ATADLL_RESULT_OPEN_COMPORT_FAIL)
    ENUM_TO_CASE_STRING(E_ATADLL_RESULT_WAIT_POWERON_TIMEOUT)
    ENUM_TO_CASE_STRING(E_ATADLL_RESULT_AT_RESPONSE_TIMEOUT)

    ENUM_TO_CASE_STRING(E_ATADLL_RESULT_PSU_INIT_FAIL)
    ENUM_TO_CASE_STRING(E_ATADLL_RESULT_PSU_OPERATE_FAIL)
    ENUM_TO_CASE_STRING(E_ATADLL_RESULT_PSU_DEINIT_FAIL)

    ENUM_TO_CASE_STRING(E_ATADLL_RESULT_GPS_TIMEOUT)
    ENUM_TO_CASE_STRING(E_ATADLL_RESULT_NOT_SUPPORT)
    ENUM_TO_CASE_STRING(E_ATADLL_RESULT_NOT_TEST)
    ENUM_TO_CASE_STRING(E_ATADLL_RESULT_WARNING_IGNORED)
    ENUM_TO_CASE_STRING(E_ATADLL_CONNECT_PM_FAIL)

    ENUM_TO_CASE_STRING(E_ATADLL_CONNECT_PHONE_FAIL)
	ENUM_TO_CASE_STRING(E_ATADLL_START_SYNC_FAIL)
	ENUM_TO_CASE_STRING(E_ATADLL_REQUEST_DATA_FAIL)
	ENUM_TO_CASE_STRING(E_ATADLL_TESTITEM_INIT_FAIL)
	ENUM_TO_CASE_STRING(E_ATADLL_RESULT_SPEC_CHECK_FAIL)
	ENUM_TO_CASE_STRING(E_ATADLL_RESULT_HDMI_NO_KEY)
	ENUM_TO_CASE_STRING(E_ATADLL_RESULT_BARCODE_FLAG_OK)
	ENUM_TO_CASE_STRING(E_ATADLL_RESULT_BARCODE_FLAG_FAIL)
	ENUM_TO_CASE_STRING(E_ATADLL_RESULT_NULL)

	default:
		return "??";

	}
}


