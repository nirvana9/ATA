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

/*****************************************************************************
 *
 * Filename:
 * ---------
 * CommThread.h
 *
 * Project:
 * --------
 *   Maui_SW - phone suite
 *
 * Description:
 * ------------
 *	 This module implements the communication thread
 *	
 * Author:
 * -------
 * Spancer 	(mtk00264)
 *
 *==============================================================================
 *             HISTORY
 * Below this line, this part is controlled by PVCS VM. DO NOT MODIFY!!
 *------------------------------------------------------------------------------
 *
 *------------------------------------------------------------------------------
 * Upper this line, this part is controlled by PVCS VM. DO NOT MODIFY!!
 *============================================================================
 ****************************************************************************/
#ifndef _COMMTHREAD_H
#define _COMMTHREAD_H

#include "ATResult.h"

#include <string>

using namespace std;

#include "thread.h"

enum COMMTYPE
{
	COMMTYPE_COM = 0,
	COMMTYPE_USB = 1,
	COMMTYPE_IRDA = 2
};

class ModuleThread : public AThread
{
protected:
	FILE *FilePtr;

public:

	ModuleThread();
	bool OpenDump(const string &FileName);
	void CloseDump();
	void DumpData(void *Buf, int &size);
	virtual ~ModuleThread()
	{
	}

	bool Start(string device, int baud, int flowCtrl, void(*unSolicitedFunc)(ATResult& result, int handle_index), COMMTYPE &commtype);
	void Stop();

	virtual bool Write(const char *Buf, const int &size) = 0;
	virtual bool Suspend()=0;
	virtual bool Resume()=0;
	virtual void SetPduFlag(bool flag) = 0;
	virtual void SetWaitGtAndSpace(bool flag) = 0;
	virtual HANDLE GetWaitGtAndSpaceEvent() = 0;
	virtual void SetCurCommand(unsigned char *cmd, int len) = 0;
	virtual HANDLE GetWriteEvent() = 0;
	virtual HANDLE GetParserEvent() = 0;
	virtual HANDLE GetInProgressEvent() = 0;
	virtual HANDLE GetCleanedEvent() = 0;
	virtual HANDLE GetCancelEvent() = 0;
	virtual HANDLE GetResetEvent() = 0;
	virtual void CleanData()=0;
	virtual void GetResult(ATResult& rt)=0;
	virtual void SetCallBack(void(*unSolicitedFunc)(ATResult& result, int handle_index))=0;
	virtual void SetHandleIndex(int handleIndex) = 0;
    virtual void SetStartWithCRLF(bool bNeed) = 0;
    virtual void SetEndWithCRLF(bool bNeed) = 0;
	virtual void SetExpectString(string command) = 0;
    virtual void GetCommandResult (char *result, int &len) = 0;
		

private:
	
	virtual bool Internal_Start(COMMTYPE &commtype) = 0;
	virtual void Internal_Stop() = 0;
	virtual DWORD ThreadFunc() = 0;
	virtual void StopThread() = 0;
	virtual void SetDevice(string port, int baud, int flowCtrl) = 0;
};


#endif