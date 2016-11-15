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
 *  CommModule.h
 *
 * Project:
 * --------
 *   HW TEST
 *
 * Description:
 * ------------
 *   This module is header file for communication module.
 *
 * Author:
 * -------
 *  Spancer
 *
 *==============================================================================
 *           HISTORY
 * Below this line, this part is controlled by PVCS VM. DO NOT MODIFY!!
 *------------------------------------------------------------------------------
 * $Revision:   1.3  $
 * $Modtime:   Aug 12 2005 16:32:12  $
 * $Log:   //mtkvs01/VMDATA/HWTest/archives/comm/CommModule.h-arc  $
 * 
 *    Rev 1.3   Dec 08 2005 14:46:52   mtk00451
 * Change comm version
 * 
 *    Rev 1.0   Feb 27 2004 18:28:10   admin
 * Initial revision.
 * 
 *    Rev 1.0   Feb 27 2004 16:42:06   admin
 * Initial revision.
 *
 *
 *------------------------------------------------------------------------------
 * Upper this line, this part is controlled by PVCS VM. DO NOT MODIFY!!
 *==============================================================================
 *******************************************************************************/
#ifndef _COMM_MODULE_H
#define _COMM_MODULE_H

#include "ATResult.h"
#include "CommThread.h"
#include <string>
#include <iostream>

#define DEFAULT_DUMP_FILE "C:\\EAP.dmp"
#define RAW_BUFFER_SIZE   8192

using namespace std;

typedef enum COMM_BAUD_TAG
{
	COMM_RS_110=CBR_110,
	COMM_RS_300=CBR_300, 
	COMM_RS_600=CBR_600,
	COMM_RS_1200=CBR_1200,
	COMM_RS_2400=CBR_2400,
	COMM_RS_4800=CBR_4800,
	COMM_RS_9600=CBR_9600,
	COMM_RS_14400=CBR_14400, 
	COMM_RS_19200=CBR_19200,
	COMM_RS_38400=CBR_38400,
	COMM_RS_56000=CBR_56000,
	COMM_RS_57600=CBR_57600,
	COMM_RS_115200=CBR_115200,
	COMM_RS_128000=CBR_128000,
	COMM_RS_256000=CBR_256000
}
COMM_BAUD;


typedef enum COMM_FLOW_CTRL_TAG
{
	COMM_FC_NONE = 0,
    COMM_FC_HW,
	COMM_FC_SW	
}
COMM_FLOW_CTRL;

typedef void (*unSolicitedCallBack)(ATResult& result, int handle_index);

typedef enum commDeviceTypeTag
{
	COMM_RS232,
	COMM_IRDA,
	COMM_BLUETOOTH
}
commDeviceType;


class CommModule
{
private:

	commDeviceType			m_deviceType;
	unSolicitedCallBack		m_usc_cb;
	int						handle_index;
    bool                    m_bNeedStartWithCRLF;
    bool                    m_bNeedEndWithCRLF;

	ModuleThread			*m_thread;

	bool					 m_fPdu;
	COMM_BAUD				 m_baudRate;
	COMM_FLOW_CTRL			 m_eFlowCtrl;
	string					 m_commDevice;

	HANDLE					m_hStopEvt;


public:
	CommModule();
	~CommModule();
	bool Init(	string comPort, 
				COMM_BAUD baud, 
				COMM_FLOW_CTRL flowCtrl,
				void(*unSolicitedFunc)(ATResult& result, int handle_index), 
				COMMTYPE &commtype);
	bool DeInit();
	bool WriteCommand(const char *ptr, int len, int timeout, bool pdu, ATResult& result);
	bool WritePDUCommand(const char *ptr, 
							int len,
							const char *pduBuf,
							int pduLen,
							int timeout,
							bool pdu,
							ATResult& result);

	bool CancelCommand(const char *ptr, int len);
	
	void Stop();
	bool Suspend();
	bool Resume();
	void ResetParser();
	commDeviceType GetDeviceType();
	string GetDevice();
	unSolicitedCallBack SetCallBack(void(*unSolicitedFunc)(ATResult& result, int handle_index));
	void SetHandleIndex(int handleIndex);
    void SetNeedWithCRLR (bool bNeed);
    void SetEndNeedWithCRLR (bool bNeed);
    void GetCommandResult (char *result, int &len);

private:
	bool InternalStart(COMM_FLOW_CTRL flowCtrl, void(*unSolicitedFunc)(ATResult& result, int handle_index), const bool Dump, COMMTYPE &commtype);
	

};


#endif