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
 *   ATA_DLL_TestItem_Interface.h
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

#ifndef __TESTITEM_INTERFACE_H__
#define __TESTITEM_INTERFACE_H__

#pragma warning(disable:4786)
#include <vector>
#include <algorithm>
#include <functional>

#include "..\..\ATA_DLL_Handle.h"
#include "..\..\ATA_DLL_Mutex.h"
#include "..\..\ATA_DLL_Utils.h"
#include "..\..\..\comm\ATResult.h"

using namespace std;

#define ATTIMEOUT 1700
extern const unsigned char CR;

typedef void (__stdcall *CallbackTestItemPostProcess)(int handle_index, E_ATDLL_TEST_ITEM_COUNT item);

class CTestItem_Interface
{
public:
    CTestItem_Interface (ATA_DLL_Handle *current_handle, int timeout = 20, E_ATDLL_TEST_ITEM_COUNT item = E_LCD, char *testName = "");
    virtual ~CTestItem_Interface ();

    virtual void SetAutoMode (bool automode);
    virtual void SetTestPostProcessCB (CallbackTestItemPostProcess cb);
    virtual bool GetATResponse (ATResult &atr);
    virtual bool SetATResponse (ATResult atr);
    virtual void StopTestAfterAll ();
    virtual bool PareStringByCRLF (string &src, vector<string> &desVec);
    inline string&  lTrim(string &ss)
    {
        string::iterator p = find_if(ss.begin(), ss.end(), not1(ptr_fun(isspace)));
        ss.erase(ss.begin(), p);
        return ss;
    }
    inline  string&  rTrim(string &ss)
    {
        string::reverse_iterator p = find_if(ss.rbegin(), ss.rend(), not1(ptr_fun(isspace)));
        ss.erase(p.base(), ss.end());
        return ss;
    }
    inline   string&   Trim(string &st)
    {
        lTrim(rTrim(st));
        return st;
    }

    virtual void AddLog (char* fmt, ...);
    virtual E_ATADLL_RESULT RunTest (void);
    virtual E_ATADLL_RESULT StartTest (void) = 0;

    static E_ATADLL_RESULT HelpThreadFunc (LPVOID param);
    virtual E_ATADLL_RESULT ThreadFunc();

    virtual void GetATAData (sp_ata_data &ata_data, ATResult atret);
    virtual E_ATADLL_RESULT RequestData (sp_ata_data &ata_data);
    virtual bool IsAutoMode ();
    virtual bool IsBackground ();

protected:
    virtual void ClearATResPool (void);
    virtual E_ATADLL_RESULT SendCommandPM (string cmd, string retStr);
    virtual E_ATADLL_RESULT SwitchToCharger ();
    virtual E_ATADLL_RESULT ResetPM ();
    virtual E_ATADLL_RESULT MeasureVoltage (double &vol);
    virtual bool SwitchToMode (int mode);

protected:
    E_ATDLL_TEST_ITEM_COUNT test_item;
    int             timeout_sec;
	
    char			m_TestItemName[256];
    ATA_DLL_Handle	*test_handle;

    HANDLE          m_hStateMutex;
    HANDLE          m_hInVector;
    int             m_iRespNum;
    int             m_iReadingInQuene;
    bool            b_auto;

    std::string		m_logStr;
    vector<ATResult>		m_sATResponsePool;

    CallbackTestItemPostProcess cbTestPostprocess;

public:
		E_ATADLL_RESULT  error_code;
};

#endif
