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
 * CommModule.cpp
 *
 * Project:
 * --------
 *   Maui_SW - phone suite
 *
 * Description:
 * ------------
 *	 This module implements the Interfaces for upper layer
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
#include "stdafx.h"
#include "commModule.h"
#include "RS232Module.h"

CommModule::CommModule()
{
	m_usc_cb = NULL;
	m_thread = NULL;

	m_baudRate = COMM_RS_115200;
	_Xboot_DebugOn();

}

CommModule::~CommModule()
{

}

bool CommModule::InternalStart(COMM_FLOW_CTRL flowCtrl, 
							   void(*unSolicitedFunc)(ATResult& result, int handle_index), 
							   const bool Dump,
							   COMMTYPE &commtype)
{
	if(m_thread != NULL)
		return false;

	// Create the RS232 Thread
	try
	{
			m_thread = new Rs232Module;
			m_deviceType =  COMM_RS232;		
	}
	catch(...)
	{
		m_thread = NULL;
		return false;
	}


	if(!m_thread->Start(m_commDevice, (int)m_baudRate, (int) m_eFlowCtrl, unSolicitedFunc, commtype))
	{
		delete m_thread;
		m_thread = NULL;
		return false;
	}

	#ifdef SP_LEVEL1
	TRACE("CommModule::InternalStart() successfully!\n");
	#endif
	return true;
}

bool CommModule::Init(string port, 
					  COMM_BAUD baud, 
					  COMM_FLOW_CTRL flowCtrl,
					  void(*unSolicitedFunc)(ATResult& result, int handle_index), 
					  COMMTYPE &commtype)
{
	#ifdef SP_LEVEL1
	TRACE("CommModule::Init\n");
	TRACE("PORT NUMBER: %s\n", port.c_str());
	#endif
	m_commDevice = port;
	m_baudRate = baud;
	m_eFlowCtrl = flowCtrl;

	m_usc_cb = unSolicitedFunc;

	m_hStopEvt   = NULL;
	m_hStopEvt   = ::CreateEvent(NULL, FALSE, FALSE, NULL);



	bool dump = true;

	if(!InternalStart(flowCtrl, unSolicitedFunc, dump, commtype))
	{
		TRACE("**CommModule::InternalStart() Fail! **\n");
		return false;
	}
	return true;
}


bool CommModule::DeInit()
{
	// Stop Raw_Buffer
	#ifdef SP_LEVEL1
	TRACE("CommModule::DeInit()\n");
	#endif

	::CloseHandle(m_hStopEvt);
	m_hStopEvt = NULL;

	if( m_thread!=NULL )
	{
		m_thread->Stop();
		delete m_thread;
		m_thread = NULL;
	}
	return true;
}


string CommModule::GetDevice()
{
	return m_commDevice;
}



commDeviceType CommModule::GetDeviceType()
{
	return m_deviceType;
}


bool CommModule::WriteCommand(const char *ptr, int len, int timeout, bool pdu, ATResult& result)
{
	DWORD	state;
	int     t_out;
	#ifdef SP_LEVEL1
	TRACE("CommModule::WriteCommand()-------1\n");
	#endif

#ifdef T_DEBUG
	CTime t = CTime::GetCurrentTime();
	
	TRACE("++Time start in Wrtie command: %d sec\n", t.GetSecond());

	string tempCommand(ptr, len);
	tempCommand[len]='\0';
	TRACE("\tWriteCommand:  %s \n", tempCommand.c_str());
	TRACE("\tTimeOut: %d\n", timeout);
	TRACE("\tPDU: %d\n", pdu);
#endif

    string tempCommand(ptr, len);
	tempCommand[len]='\0';
	TRACE("\tWriteCommand:  %s \n", tempCommand.c_str());
	MTRACE(g_hEBOOT_DEBUG, "------WriteCommand: %s------",tempCommand.c_str());
	// Help the cancel command, Parser's sync function will check if this event is
	// signaled, if NOt. It will give up some parsed result
	::ResetEvent( m_thread->GetCleanedEvent());
	::SetEvent( m_thread->GetInProgressEvent());

#ifdef T_DEBUG
	t = CTime::GetCurrentTime();
	
	TRACE("++Time Before Clean Data: %d sec\n", t.GetSecond());
#endif

	#ifdef SP_LEVEL1
	TRACE("Write Command before Clear Data-----2\n");
	#endif
	m_thread->CleanData();

	#ifdef SP_LEVEL1
	TRACE("Write Command after Clear Data-----4\n");
	#endif
#ifdef T_DEBUG
	t = CTime::GetCurrentTime();

	TRACE("++Time After Clean Data: %d sec\n", t.GetSecond());
#endif

	// Fill the request data to the ATRespParser module
	#ifdef SP_LEVEL1
	TRACE("Fill the expect command: %s    ------5\n", result.expectedCommand.c_str());
	#endif
    
	m_thread->SetExpectString(result.expectedCommand);
	m_thread->SetPduFlag(pdu);
	m_thread->SetCurCommand((unsigned char *)ptr, len);

	if(timeout == 0)
		t_out = INFINITE;
	else
		t_out = timeout;

    // Write the command characters to the COMM port.
	#ifdef SP_LEVEL1
	TRACE("Write Command to COM port: -----6\n");
	#endif

    SYSTEMTIME  cur_time;
	GetLocalTime(&cur_time);
    TRACE ("Before Write command: %04d_%02d_%02d_%02d_%02d_%02d_%03d\r\n",
                 cur_time.wYear, cur_time.wMonth, cur_time.wDay,
                 cur_time.wHour, cur_time.wMinute, cur_time.wSecond, cur_time.wMilliseconds);

	if(!m_thread->Write(ptr, len))
	{
		result.result = AT_WRITE_FAIL;
		::SetEvent(m_thread->GetParserEvent());
		::ResetEvent( m_thread->GetInProgressEvent());
		::SetEvent(m_thread->GetResetEvent());

		MTRACE_ERR(g_hEBOOT_DEBUG, "------WriteCommand: %s fail !------",tempCommand.c_str());
		return false;
	}

	GetLocalTime(&cur_time);
    TRACE ("After Write command: %04d_%02d_%02d_%02d_%02d_%02d_%03d\r\n",
                 cur_time.wYear, cur_time.wMonth, cur_time.wDay,
                 cur_time.wHour, cur_time.wMinute, cur_time.wSecond, cur_time.wMilliseconds);

	
	state = ::WaitForSingleObject( m_thread->GetWriteEvent(), t_out);

	// Handle the timeout mechanism.
	if(state == WAIT_TIMEOUT)
	{
		result.result = AT_TIMEOUT;
		::ResetEvent( m_thread->GetInProgressEvent());
		::SetEvent(m_thread->GetParserEvent());
		TRACE("*** Wait Responds Time Out ***!\n");
		::SetEvent(m_thread->GetResetEvent());
		//MTRACE_ERR(g_hEBOOT_DEBUG, "------Command: %s Wait Responds Time Out !------",tempCommand.c_str());
		return false;
	}

	// Get the data from ATRespParser
	m_thread->GetResult(result);
	MTRACE(g_hEBOOT_DEBUG,"------Get the result from the parser------");
	
	#ifdef SP_LEVEL1
	TRACE("Get the result from the parser: -----8\n");
	#endif
#ifdef T_DEBUG
	t = CTime::GetCurrentTime();
	TRACE("++Time End in Wrtie command: %d sec\n", t.GetSecond());
#endif

	::ResetEvent( m_thread->GetInProgressEvent());
	::SetEvent(m_thread->GetParserEvent());
	::ResetEvent( m_thread->GetWriteEvent());	

    /**
	if(result.resultLst.size()==0)
	{	
//		ASSERT(false);
		return false;
	}

	if(result.resultLst.size()>0)
	{
        if(result.resultLst[result.resultLst.size()-1].eleLst.size()==0)
		{
			ASSERT(false);
			return false;
		}
	}
	**/

	return true;
}


// This command is almost the same as the WriteCommand, it just has the pdu part
bool CommModule::WritePDUCommand(const char *ptr, int len, 
								 const char *pduBuf,  int pduLen,
								 int timeout, bool pdu, ATResult& result)
{
	DWORD	state;
	int     t_out;
	
	#ifdef SP_LEVEL1
	TRACE("CommModule::WritePDUCommand()\n");
	#endif

#ifdef SP_DEBUG
	string tempCommand(ptr, len);
	tempCommand[len]='\0';
	TRACE("\tCommand: %s \n", tempCommand.c_str());

	string tempPdu(pduBuf, pduLen);
	tempPdu[pduLen]='\0';
	TRACE("\tPdu Buffer: %s \n", tempPdu.c_str());
#endif
	
	// Help the cancel command, Parser's sync function will check if this event is
	// signaled, if NOt. It will give up some parsed reslt
	::ResetEvent( m_thread->GetCleanedEvent());
	::SetEvent( m_thread->GetInProgressEvent());


	m_thread->CleanData();
	
	// Set the parser to wait '>' and ' '
	m_thread->SetWaitGtAndSpace(true);

	// Fill the request data to the ATRespParser module
	m_thread->SetExpectString(result.expectedCommand);
    m_thread->SetPduFlag(pdu);
	m_thread->SetCurCommand((unsigned char *)ptr, len);

	if(timeout == 0)
		t_out = INFINITE;
	else
		t_out = timeout;

    // Write the command characters to the COMM port.
	if(!m_thread->Write(ptr, len))
	{
		TRACE("\t***Write Command Fail*** \n");
		result.result = AT_WRITE_FAIL;
		m_thread->SetWaitGtAndSpace(false);
		::SetEvent(m_thread->GetParserEvent());
		return false;
	}

	// Wait for the  <CR> <LF> <GREATER THAN> <SPACE>
	state = ::WaitForSingleObject( m_thread->GetWaitGtAndSpaceEvent(), t_out);

	if(state == WAIT_TIMEOUT)
	{
		result.result = AT_TIMEOUT;
		::SetEvent(m_thread->GetParserEvent());
		TRACE("\t*** Wait Respond Timeout ****\n");
		return false;
	}

	::ResetEvent( m_thread->GetWaitGtAndSpaceEvent());


	// Write the Second buffer to the COMM port.
	if(!m_thread->Write(pduBuf, pduLen))
	{
		result.result = AT_WRITE_FAIL;
		m_thread->SetWaitGtAndSpace(false);
		::SetEvent(m_thread->GetParserEvent());
		TRACE("\t*** Write PDU buffer Error ****\n");
		return false;
	}

	// Wait for result

	state = ::WaitForSingleObject( m_thread->GetWriteEvent(), t_out);

	// Handle the timeout mechanism.
	if(state == WAIT_TIMEOUT)
	{
		result.result = AT_TIMEOUT;
		::SetEvent(m_thread->GetParserEvent());
		return false;
	}

	// Get the data from ATRespParser
	m_thread->GetResult(result);	

	::ResetEvent( m_thread->GetInProgressEvent());
	::SetEvent(m_thread->GetParserEvent());
	::ResetEvent( m_thread->GetWriteEvent());
	
	return true;
}



void CommModule::Stop()
{
	
}


bool CommModule::Suspend()
{
	m_thread->Suspend();
	return true;
}

bool CommModule::Resume()
{
	m_thread->Resume();
	return true;
}

unSolicitedCallBack CommModule::SetCallBack(void(*unSolicitedFunc)(ATResult& result, int handle_index))
{
	unSolicitedCallBack oldTmpCallBack = m_usc_cb;
	
	if(unSolicitedFunc!=NULL)
	{
		m_usc_cb = unSolicitedFunc;

		//Set the call back to rs232 module
		m_thread->SetCallBack(unSolicitedFunc);

		return oldTmpCallBack;
	}

	return m_usc_cb;
}

void CommModule::SetHandleIndex(int handleIndex)
{
	handle_index = handleIndex;
    m_thread->SetHandleIndex (handleIndex);
}

void CommModule::SetNeedWithCRLR (bool bNeed)
{
	m_bNeedStartWithCRLF = bNeed;
	m_thread->SetStartWithCRLF (bNeed);
}

void CommModule::GetCommandResult (char *result, int &len)
{
	m_thread->GetCommandResult (result, len);
}

void CommModule::SetEndNeedWithCRLR (bool bNeed)
{
	m_bNeedEndWithCRLF = bNeed;
	m_thread->SetEndWithCRLF (bNeed);
}


void CommModule::ResetParser()
{
	m_thread->CleanData();
}


bool CommModule::CancelCommand(const char *ptr, int len)
{
	TRACE("CommModule::CancelCommand()\n");
    // Write the command characters to the COMM port.
	// This command won't clean the Parser, it will just
	// try to stop the previous command.

	
	if(!m_thread->Write(ptr, len))
	{
		TRACE("*** Cancel Copmmand Fail ***!\n");

		// TODO: Set the cancel event to stop the current receving command
		::SetEvent(m_thread->GetResetEvent());
		::SetEvent(m_thread->GetCancelEvent());
		return false;
	}
	return true;
}



#if 0
void UncallBack(ATResult& rt)
{
	ATResult result;

	result = rt;
	cout << "Out put the AT Result\n";
	cout << "result: "<< result << endl;
}




void main()
{

    char buffer[128];

	string comport = "COM1";
	ATResult atr;
	CommModule com;


	if(!com.Init( comport, UncallBack))
	{
		cout << "COMM Initialize fail!\n";
		exit(1);
	}

	// ATE0
    string command= "ATE0";
	command += 0x0d;
	strcpy(buffer, command.c_str());

    com.WriteCommand(buffer, strlen(buffer), 0, false, atr);

	//AT+CSMS=?
	command= "AT+CSMS=?";
	command += 0x0d;
	strcpy(buffer, command.c_str());
	atr.expectedCommand= "+CSMS";
	com.WriteCommand(buffer, strlen(buffer), 0, false, atr);


	//AT+CSMS?
	command= "AT+CSMS?";
	command += 0x0d;
	strcpy(buffer, command.c_str());
	atr.expectedCommand= "+CSMS";
	com.WriteCommand(buffer, strlen(buffer), 0, false, atr);


	//AT+CMGF=?
	command= "AT+CMGF=?";
	command += 0x0d;
	strcpy(buffer, command.c_str());
	atr.expectedCommand= "+CMGF";
	com.WriteCommand(buffer, strlen(buffer), 0, false, atr);


	//AT+CMGF?
	command= "AT+CMGF?";
	command += 0x0d;
	strcpy(buffer, command.c_str());
	atr.expectedCommand= "+CMGF";
	com.WriteCommand(buffer, strlen(buffer), 0, false, atr);



	//AT+CPMS?
	command= "AT+CPMS?";
	command += 0x0d;
	strcpy(buffer, command.c_str());
	atr.expectedCommand= "+CPMS";
	com.WriteCommand(buffer, strlen(buffer), 0, false, atr);

	//AT+CPMS=?
	command= "AT+CPMS=?";
	command += 0x0d;
	strcpy(buffer, command.c_str());
	atr.expectedCommand= "+CPMS";
	com.WriteCommand(buffer, strlen(buffer), 0, false, atr);


	//AT+CSCA?
	command= "AT+CSCA?";
	command += 0x0d;
	strcpy(buffer, command.c_str());
	atr.expectedCommand= "+CSCA";
	com.WriteCommand(buffer, strlen(buffer), 0, false, atr);


	//AT+CSCA?
	command= "AT+CNMI?";
	command += 0x0d;
	strcpy(buffer, command.c_str());
	atr.expectedCommand= "+CNMI";
	com.WriteCommand(buffer, strlen(buffer), 0, false, atr);

	command= "AT+CNMI=?";
	command += 0x0d;
	strcpy(buffer, command.c_str());
	atr.expectedCommand= "+CNMI";
	com.WriteCommand(buffer, strlen(buffer), 0, false, atr);


	// AT+CMGL=?
	command= "AT+CMGL=?";
	command += 0x0d;
	strcpy(buffer, command.c_str());
	atr.expectedCommand= "+CMGL";
	com.WriteCommand(buffer, strlen(buffer), 0, false, atr);

	// AT+CMGL=1
	command= "AT+CMGL=1";
	command += 0x0d;
	strcpy(buffer, command.c_str());
	atr.expectedCommand= "+CMGL";
	com.WriteCommand(buffer, strlen(buffer), 0, true, atr);



	// AT+CMGR=?
	command= "AT+CMGR=?";
	command += 0x0d;
	strcpy(buffer, command.c_str());
	atr.expectedCommand= "+CMGR";
	com.WriteCommand(buffer, strlen(buffer), 0, false, atr);


	while(1)
	{
		com.WriteCommand( buffer,strlen(buffer), 3000, 0, atr);
		::Sleep(500);
	}



	// Test Read Commands


}
#endif

