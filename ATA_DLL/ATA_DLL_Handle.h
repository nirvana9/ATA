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
 *   ATA_DLL_Handle.h
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

#ifndef __ATA_DLL_HANDLE__
#define __ATA_DLL_HANDLE__

#include "ATA_DLL.h"
#include "ATA_DLL_Mutex.h"
#include "..\comm\CommModule.h"

#include <list>
#include <memory>
#include <map>
#include <string>
#include <time.h>

#pragma warning(disable:4786)

class ATA_DLL_Handle
{
public:
    ATA_DLL_Handle();
    virtual ~ATA_DLL_Handle();
    void Reset();
    inline void Occupy()
    {
        m_bAvailable = false;
    }
    inline void Release()
    {
        m_bAvailable = true;
    }
    inline bool IsAvailable() const
    {
        return m_bAvailable;
    }

    bool InitTestItemObj (void);
    bool DeinitTestItemObj (void);
    E_ATADLL_RESULT RunTestItemObj (void);

    // Test Item
    bool ConnectTarget(int initState);
    bool ConnectPM();
    void DisconnectPM();
    void DisconnectTarget();
    E_ATADLL_RESULT RunTest(int ata_handle);
    E_ATADLL_RESULT StopTest(int ata_handle);
    E_ATADLL_RESULT RunTestStep(E_ATDLL_TEST_ITEM_COUNT test_item);
    E_ATADLL_RESULT UpdateBarcodeFlag(int ata_handle, bool bPass);
    E_ATADLL_RESULT TestOffCurrent ();

    E_ATADLL_RESULT RunLCDTest(void);
    E_ATADLL_RESULT RunCameraTest(void);
    E_ATADLL_RESULT RunVibratorTest(void);
    E_ATADLL_RESULT RunCharingTest(void);
    E_ATADLL_RESULT RunKeypadTest(void);
    E_ATADLL_RESULT RunTouchScrTest(void);

    E_ATADLL_RESULT RunSpeakerTest(void);
    E_ATADLL_RESULT RunReceiverTest(void);
    E_ATADLL_RESULT RunEarthPhoneTest(void);
    E_ATADLL_RESULT RunRingTest(void);

    E_ATADLL_RESULT RunWIFITest(void);
    E_ATADLL_RESULT RunGPSTest(void);
    E_ATADLL_RESULT RunBTTest(void);
    E_ATADLL_RESULT RunTVTest(void);
    E_ATADLL_RESULT RunFMTest(void);

    E_ATADLL_RESULT RunTCardTest(void);
    E_ATADLL_RESULT RunSIMTest(void);
    E_ATADLL_RESULT RunBarcodeTest(void);
    E_ATADLL_RESULT RunSWVersionTest(void);

    void AddLog(const char *fmt, va_list arg);
    void Log_Record(const char *fmt, ...);
	void AddCSVLog(char* log_buff);
    E_ATDLL_TEST_ITEM_COUNT GetItemIndex(string item_name);
    bool CheckIniItemSingle(E_ATDLL_TEST_ITEM_COUNT item);
    void ParseIniFile();

    bool IsItemTest (E_ATDLL_TEST_ITEM_COUNT item);
    bool EnterFactoryModeByUSB (unsigned int &comport);
    bool IsPreloaderComPortOK();
    long IsKernelComPortOK();
    long GetKernelComModule ();
    long GetCommModule (int initState);
    void ReleaseCommModule (CommModule *cm);
    long GetPMCommModule ();
    bool CheckPMOfficial ();
    void ClosePMModule ();
    bool SendStartCommand ();
    bool SendStopCommand ();
    bool QueryResult (E_ATDLL_TEST_ITEM_COUNT test_item);
    void RenameLogFile();
    bool RequestFlagFromTarget(int* number);
	bool ConvertToPCItemSequence(int number);

private:
    ATA_DLL_Mutex			m_logStrMutex;
    ATA_DLL_Mutex			m_logWriteToFileMutex;
    char					logBuffer[1024];
    bool                    bLogWithDateInfo;

protected:
    ATA_DLL_Mutex			m_at_cmd_lock;
    bool					m_bAvailable;
    std::list<std::string>	m_listLogStr;
    FILE					*m_fpLogFile;
	FILE					*m_fpLogCsvFile;
	E_ATADLL_RESULT         test_result_code;

public:
    int						ata_handle;
    int						handle_index;
//	char					m_log_buf[MAX_LOG_BUF_SIZE];
    S_ATADLL_TEST_ITEM_T	m_testItemCFG;
    S_ATADLL_COMMON_CFG_T	m_commonCFG;
    S_ATADLL_TEST_CNF		*m_testCNF;
    FILE					*m_fDLLLogFile;

    S_ATADLL_TESTCASE_T     testcases_bg[MAX_TEST_CASE_NUM];
    S_ATADLL_TESTCASE_T     testcases_fg[MAX_TEST_CASE_NUM];
    S_ATADLL_TESTCASE_CONFLICT  testcases_conflict[MAX_TEST_CASE_NUM];

    int                     testcases_index_bg;
    int                     testcases_index_fg;
    int                     testcases_index_conflict;

    CommModule 				*m_comm_handle;
    CommModule              *m_pm_comm_handle;

    HANDLE                  m_hStateMutex;
	
};

#define ATA_DLL_MAX_CONCURRENT_THREADS		32
class ATA_DLL_Handle_Array
{
public:
    ATA_DLL_Handle_Array();
    virtual ~ATA_DLL_Handle_Array();
    int  OccupyHandle(const int ata_handle);
    int  ReleaseHandle(const int ata_handle);
    bool IsActiveHandle(const int ata_handle);
    ATA_DLL_Handle	&	operator [] (const int ata_handle);

protected:
    bool IsValidIndex(const int index);

private:
    ATA_DLL_Mutex			m_ata_array_Lock;
    ATA_DLL_Handle			mATADLLHandleArray[ATA_DLL_MAX_CONCURRENT_THREADS];
};
extern ATA_DLL_Handle_Array		g_ADHA;
extern const char*__stdcall AtaStatusToString(int status);
#endif
