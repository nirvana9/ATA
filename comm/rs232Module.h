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
*  Rs232Module.h
*
* Project:
* --------
*   Maui SW - Phone Suite
*
* Description:
* ------------
*   This module is for UART read/write.
*
* Author:
* -------
*  Spancer
*
*==============================================================================
*           HISTORY
* Below this line, this part is controlled by PVCS VM. DO NOT MODIFY!!
*------------------------------------------------------------------------------
* $Revision:   1.4  $
* $Modtime:   Nov 24 2005 15:39:44  $
* $Log:   //mtkvs01/VMDATA/HWTest/archives/comm/rs232Module.h-arc  $
 * 
 *    Rev 1.4   Dec 08 2005 14:46:52   mtk00451
 * Change comm version
 * 
 *    Rev 1.2   Apr 22 2004 20:24:00   mtk00264
 * Fix the bug that phone suite sometimes lost SMS.
 * Resolution for 33: [BugFix] Phone suite would lost some SMS
 * 
 *    Rev 1.1   Mar 24 2004 20:36:24   mtk00264
 * Solving the bug of checking <CR> <LF> <GT> <Space>
 * Resolution for 4489: [PhoneSuite][BugFix]
 * 
 *    Rev 1.0   Feb 27 2004 18:28:12   admin
 * Initial revision.
 * 
 *    Rev 1.0   Feb 27 2004 16:42:08   admin
 * Initial revision.
*
*
*------------------------------------------------------------------------------
* Upper this line, this part is controlled by PVCS VM. DO NOT MODIFY!!
*==============================================================================
*******************************************************************************/
#ifndef _RS232MODULE_H_
#define _RS232MODULE_H_

#include "ATResult.h"

#define RESP_BUFFER_LENGTH	1024*200

#define STATE_WAIT_CR    0
#define STATE_WAIT_LF    1


typedef void (*unSolicitedCallback)(ATResult& result, int handle_index);


class Rs232Module : public ModuleThread
{
	HANDLE	hCOM;
	HANDLE	stopEvent;
	string  m_port;
	int     m_baud;
	int     m_flowCtrl;
	int     m_getPduState;  // 0: init
							// 1: GetCR
							// 2: GetLF

	// Revising for one thread
	bool	pduFlag;
	bool	m_waitGtAndSpace;
	HANDLE  m_hWaitGtAndSpace;
	int  readIndex;
	int  writeIndex;
    int  respDataBufLen;
	char cr;
	char lf;
	char respDataBuf[RESP_BUFFER_LENGTH];
	char m_pduBuf[RESP_BUFFER_LENGTH];
	int  m_idxPduBuf;
	ATResult	result;
	ATResult	unsolicitedRt;
	unSolicitedCallback		m_usc_cb;
	int						handle_index;
    bool                    m_bNeedStartWithCRLF;
    bool                    m_bNeedEndWithCRLF;
	HANDLE					m_hWriteEvt;
	HANDLE					m_hParserEvt;
	HANDLE					m_hResetEvt;
	HANDLE					m_hInProgressEvt;
	HANDLE					m_hCleanedEvt;
	HANDLE					m_hCancelEvt;

	// Additional for merage
	int    m_idxReadRawBuffer;

	// Move from the thread
	unsigned char	rawchars[RESP_BUFFER_LENGTH];
	DWORD	dwRead;
	int		m_iRead;

	unsigned char   m_pduData[RESP_BUFFER_LENGTH];
	int		m_iPduRead;
	DWORD   m_dwRead;

	unsigned char  m_curCommand[4096];

	int			   m_idxRespCommand;
	unsigned char  m_respCommand[1024];

	bool m_bReset;


public:
	// Revising for one thread
	bool GetOneCrLf(int &state, unsigned char *data, int dataLength);
	void FillBufferByState(unsigned char ch, int &state);
	void SetWaitGtAndSpace(bool flag);
	void SetPduFlag(bool flag);
	bool CheckIfGetGtandSpace(char *data, int idx);
	HANDLE GetWaitGtAndSpaceEvent();
	char GetCrByte();
	char GetLfByte();
	bool AnalyingRespString(int len);
	void Sync();
	bool IsUnsolicited(string cmd, ATParamLst& lst);
	bool GetParam(char *ptr, ATParamElem& elem, bool isString);
	bool GetParam(char *ptr, ATParamLst& lst, bool isString);
	bool UnsolicitedGetPdu(char *buf, const int bufLen, ATParamLst& lst);
	void CleanResult(ATResult& rt);
	bool GetPdu(char *buf, const int maxLen);
	bool GetList(char *ptr, int& wi, ATParamElem& elem);
	bool GetList(char *ptr, int& wi, ATParamLst& lst);
	bool SkipComma(char *buf, int& ri, const int size);
	bool IsNumber(char *ptr);
	bool GetString(char *ptr, int& wi, ATParamElem& elem);
	bool GetString(char *ptr, int& wi, ATParamLst& lst);
	bool GetLstString(char *ptr, int& wi, ATParamElem& elem);
	bool GetStringToBuf(char *ptr, int& wi, const int size);
	bool GetRawData(unsigned char &ch);
	void GetResult(ATResult& rt);
	void SetCallBack(void(*unSolicitedFunc)(ATResult& result, int handle_index));
	void SetHandleIndex(int handleIndex);
    void SetStartWithCRLF(bool bNeed);
    void SetEndWithCRLF(bool bNeed);
    void GetCommandResult (char *result, int &len);
	void SetExpectString(string command);
    bool GetPduToBuf(char ch);
	bool GetPduToBuf();
	void ResetGetPduState();
	bool GetDeviceData();
	bool GetPduToBuf(int &idx, unsigned char *state);
	bool CheckIfGetGtandSpace(char ch);

	void SetCurCommand(unsigned char *cmd, int len);
	HANDLE GetWriteEvent();
	HANDLE GetParserEvent();
	HANDLE GetInProgressEvent();
	HANDLE GetCleanedEvent();
	HANDLE GetCancelEvent();
	HANDLE GetResetEvent();
	void CleanData();

	// The orignal part
	Rs232Module();
	virtual ~Rs232Module();

	virtual bool Write(const char *Buf, const int &size);
	virtual bool Suspend();
	virtual bool Resume();
private:
	virtual bool Internal_Start(COMMTYPE &commtype);
	virtual void Internal_Stop();
	virtual DWORD ThreadFunc();
	virtual void StopThread();
	virtual void SetDevice( string port, int baud, int flowCtrl);

	
	void CleanReceivedData();
	void CleanDriverData();
	void CancelSync();
	
};


#endif