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
*  Rs232Module.cpp
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
* $Revision:   1.5  $
* $Modtime:   Nov 09 2005 17:54:48  $
* $Log:   //mtkvs01/VMDATA/HWTest/archives/comm/rs232Module.cpp-arc  $
 * 
 *    Rev 1.5   Dec 08 2005 14:46:52   mtk00451
 * Change comm version
 *
 *   Rev 1.3   Apr 29 2004 09:16:50   mtk00264
 *Disable the MFC trace by changing the compile option SP_DEBUG to COMM_DBG_WITH_MFC.
 *
 *   Rev 1.2   Apr 22 2004 20:24:00   mtk00264
 *Fix the bug that phone suite sometimes lost SMS.
 *Resolution for 33: [BugFix] Phone suite would lost some SMS
 *
 *   Rev 1.1   Mar 24 2004 20:35:36   mtk00264
 *Solving the problem of save the second message to phone will cause the phonesuite in a un-recovery state.
 *Resolution for 4489: [PhoneSuite][BugFix]
 *
 *   Rev 1.0   Feb 27 2004 18:28:12   admin
 *Initial revision.
 *
 *   Rev 1.0   Feb 27 2004 16:42:08   admin
 *Initial revision.
*
*
*------------------------------------------------------------------------------
* Upper this line, this part is controlled by PVCS VM. DO NOT MODIFY!!
*==============================================================================
*******************************************************************************/
#include "stdafx.h"
#include "CommThread.h"
#include "rs232Module.h"
#include "commModule.h"
#include "..\ATA_DLL\ATA_DLL.h"

#include <string>
#include <iostream>

using namespace std;


Rs232Module::Rs232Module()
{
	ModuleThread::ModuleThread();

	hCOM = NULL;
	stopEvent = NULL;
	m_idxRespCommand=0;
	m_hResetEvt = NULL;
	m_hWriteEvt = NULL;
	m_hParserEvt = NULL;
	m_hWaitGtAndSpace = NULL;
	m_hInProgressEvt = NULL;
	m_hCleanedEvt = NULL;
	m_hCancelEvt = NULL;

    handle_index = 0;

	m_bReset = false;
	m_waitGtAndSpace = false;
	pduFlag = false;

	m_bNeedStartWithCRLF = false;
	m_bNeedEndWithCRLF = false;
	m_usc_cb = NULL;
	handle_index = -1;

	cr = 0x0d;
	lf = 0x0a;
}

Rs232Module::~Rs232Module()
{
	
}

void Rs232Module::SetDevice( string port, int baud, int flowCtrl)
{
	m_port = port;
	m_baud = baud;
	m_flowCtrl = flowCtrl;
}


bool Rs232Module::Internal_Start(COMMTYPE &commtype)
{
	COMSTAT	comstat;
    DWORD	errors;

	commtype = COMMTYPE_COM;
	m_hWaitGtAndSpace = NULL;
	m_hWaitGtAndSpace = ::CreateEvent(NULL, FALSE, FALSE, NULL);

	hCOM = ::CreateFile(m_port.c_str(),
						GENERIC_READ|GENERIC_WRITE, 
						0, 
						NULL, 
						OPEN_EXISTING, 
						FILE_ATTRIBUTE_NORMAL, 
						NULL
						);


	if( hCOM==INVALID_HANDLE_VALUE )
	{
		hCOM = NULL;
		return false;
	}

	::ClearCommError(hCOM, &errors, &comstat);

	//turn off fAbort first
	DCB dcb;

    if( GetCommState(hCOM, &dcb)==FALSE )
	{
		CloseHandle(hCOM);
		hCOM = NULL;
		return false;
	}

	dcb.DCBlength = sizeof(DCB);
    dcb.Parity = NOPARITY;
    dcb.ByteSize = 8;
    dcb.StopBits = ONESTOPBIT;
    dcb.BaudRate = m_baud;			
    dcb.fBinary = TRUE;
    dcb.fParity = FALSE;

	if(m_flowCtrl == COMM_FC_HW)
		dcb.fOutxCtsFlow = TRUE;
	else
		dcb.fOutxCtsFlow = FALSE;

    dcb.fOutxDsrFlow = FALSE;
    dcb.fDtrControl = DTR_CONTROL_DISABLE;
    dcb.fDsrSensitivity = FALSE;
    dcb.fTXContinueOnXoff = FALSE;
	dcb.fErrorChar = FALSE;
    dcb.fNull = FALSE;
	dcb.fAbortOnError = FALSE;

	if(m_flowCtrl == COMM_FC_SW)
	{
		dcb.fOutX = TRUE;
		dcb.fInX = TRUE;
	}
	else
	{
		dcb.fOutX = FALSE;
		dcb.fInX = FALSE;
	}   
	
	if(m_flowCtrl == COMM_FC_SW
	|| m_flowCtrl == COMM_FC_NONE)
		dcb.fRtsControl = RTS_CONTROL_ENABLE;
	else if(m_flowCtrl == COMM_FC_HW)
		dcb.fRtsControl = RTS_CONTROL_HANDSHAKE;		    

	if(m_flowCtrl == COMM_FC_SW)
	{
		dcb.XonChar = 0x11;
		dcb.XoffChar = 0x13;
	}
	else
	{
		dcb.XonChar = 0;
		dcb.XoffChar = 0;
	}

    dcb.ErrorChar = 0;
    dcb.EofChar = 0;
    dcb.EvtChar = 0;

	::ClearCommError(hCOM, &errors, &comstat);

	if( SetCommState(hCOM, &dcb)==FALSE )
	{
		CloseHandle(hCOM);
		hCOM = NULL;
		return false;
	}

	if( ::SetupComm(hCOM, RESP_BUFFER_LENGTH, 8192)==FALSE )
	{
		CloseHandle(hCOM);
		hCOM = NULL;
		return false;
	}

	if( ::PurgeComm(hCOM, PURGE_TXABORT|PURGE_RXABORT|PURGE_TXCLEAR|PURGE_RXCLEAR)==FALSE )
	{
		CloseHandle(hCOM);
		hCOM = NULL;
		return false;
	}

	// Communications Properties
    COMMPROP cp;
    if(GetCommProperties(hCOM, &cp) == FALSE)
    {
		CloseHandle(hCOM);
		hCOM = NULL;
		return false;
    }
    if(cp.dwProvSubType == PST_MODEM && cp.dwProvCapabilities == 0xcd)
        commtype = COMMTYPE_USB;
	else if(cp.dwProvSubType == PST_RS232 && cp.dwProvCapabilities == 0xc7)
		commtype =  COMMTYPE_IRDA;    			
	
	COMMTIMEOUTS timeouts;
    timeouts.ReadIntervalTimeout = MAXDWORD; //0xFFFFFFFF;
    timeouts.ReadTotalTimeoutMultiplier = 0;
    timeouts.ReadTotalTimeoutConstant = 0;
    timeouts.WriteTotalTimeoutMultiplier = 0;
    timeouts.WriteTotalTimeoutConstant = 2000;
    if( ::SetCommTimeouts(hCOM, &timeouts) == FALSE ) 
	{
		CloseHandle(hCOM);
		hCOM = NULL;
		return false;
	}			
	
	stopEvent = ::CreateEvent(NULL, TRUE, FALSE, NULL);
	if( stopEvent == NULL )
	{
		CloseHandle(hCOM);
		hCOM = NULL;
		return false;
	}

	m_hResetEvt = ::CreateEvent(NULL, true, false, NULL);
	if( m_hResetEvt == NULL)
	{
		CloseHandle(hCOM);
		hCOM = NULL;
		CloseHandle(stopEvent);
		stopEvent = NULL;
		return false;
	}

	m_hWriteEvt = ::CreateEvent(NULL, true, false, NULL);
	if( m_hWriteEvt == NULL)
	{
		CloseHandle(hCOM);
		hCOM = NULL;
		CloseHandle(stopEvent);
		stopEvent = NULL;
		CloseHandle(m_hResetEvt);
		m_hResetEvt = NULL;
		return false;
	}

	m_hParserEvt = ::CreateEvent(NULL, TRUE, FALSE, NULL);
	if( m_hParserEvt == NULL)
	{
		CloseHandle(hCOM);
		hCOM = NULL;
		CloseHandle(stopEvent);
		stopEvent = NULL;
		CloseHandle(m_hResetEvt);
		m_hResetEvt = NULL;
		CloseHandle(m_hWriteEvt);
		m_hWriteEvt = NULL;
		return false;
	}

	m_hInProgressEvt = ::CreateEvent(NULL, TRUE, FALSE, NULL);
	if( m_hInProgressEvt == NULL)
	{
		CloseHandle(hCOM);
		hCOM = NULL;
		CloseHandle(stopEvent);
		stopEvent = NULL;
		CloseHandle(m_hResetEvt);
		m_hResetEvt = NULL;
		CloseHandle(m_hWriteEvt);
		m_hWriteEvt = NULL;
		CloseHandle(m_hParserEvt);
		m_hParserEvt = NULL;
		return false;
	}

	m_hCleanedEvt = ::CreateEvent(NULL, TRUE, FALSE, NULL);
	if( m_hCleanedEvt == NULL)
	{
		CloseHandle(hCOM);
		hCOM = NULL;
		CloseHandle(stopEvent);
		stopEvent = NULL;
		CloseHandle(m_hResetEvt);
		m_hResetEvt = NULL;
		CloseHandle(m_hWriteEvt);
		m_hWriteEvt = NULL;
		CloseHandle(m_hParserEvt);
		m_hParserEvt = NULL;
		CloseHandle(m_hInProgressEvt);
		m_hParserEvt = NULL;

		return false;
	}

	m_hCancelEvt = ::CreateEvent(NULL, TRUE, FALSE, NULL);
	if( m_hCancelEvt == NULL)
	{
		CloseHandle(hCOM);
		hCOM = NULL;
		CloseHandle(stopEvent);
		stopEvent = NULL;
		CloseHandle(m_hResetEvt);
		m_hResetEvt = NULL;
		CloseHandle(m_hWriteEvt);
		m_hWriteEvt = NULL;
		CloseHandle(m_hParserEvt);
		m_hParserEvt = NULL;
		CloseHandle(m_hInProgressEvt);
		m_hParserEvt = NULL;
		CloseHandle(m_hCleanedEvt);
		m_hCleanedEvt = NULL;
		return false;
	}


	if( StartThread()==false )
	{
		CloseHandle(hCOM);
		hCOM = NULL;
		CloseHandle(stopEvent);
		stopEvent = NULL;
		CloseHandle( m_hResetEvt);
		m_hResetEvt = NULL;
		CloseHandle(m_hWriteEvt);
		m_hWriteEvt = NULL;
		CloseHandle(m_hParserEvt);
		m_hParserEvt = NULL;
		CloseHandle(m_hInProgressEvt);
		m_hParserEvt = NULL;
		return false;
	}

	return true;
}

void Rs232Module::Internal_Stop()
{

	StopThread();

	
	if( hCOM!=NULL )
	{		
		CloseHandle(hCOM);
		hCOM = NULL;
	}

	if( stopEvent!=NULL )
	{
		CloseHandle(stopEvent);
		stopEvent = NULL;
	}

	if(  m_hResetEvt != NULL)
	{
		::CloseHandle(m_hResetEvt);
		m_hResetEvt = NULL;
	}

	if( m_hWaitGtAndSpace != NULL)
	{
		::CloseHandle(m_hWaitGtAndSpace);
		m_hWaitGtAndSpace = NULL;
	}

	if( m_hParserEvt != NULL)
	{
		::CloseHandle(m_hParserEvt);
		m_hParserEvt = NULL;
	}

	if( m_hWriteEvt != NULL)
	{
		::CloseHandle(m_hWriteEvt);
		m_hWriteEvt = NULL;
	}

	if(m_hInProgressEvt != NULL)
	{
		::CloseHandle(m_hInProgressEvt);
		m_hInProgressEvt = NULL;
	}

	if( m_hCleanedEvt != NULL)
	{
		::CloseHandle(m_hCleanedEvt);
		m_hCleanedEvt = NULL;
	}

	if( m_hCancelEvt != NULL)
	{
		::CloseHandle(m_hCancelEvt);
		m_hCancelEvt = NULL;
	}

}

void Rs232Module::StopThread()
{
	if( GetHandle()==NULL ) 
	{
		return;
	}

	::SetEvent(stopEvent);
	WaitForExit();
}

void Rs232Module::CleanData()
{
	#ifdef SP_LEVEL1
	TRACE("Rs232Module::CleanData() ----3\n");
	#endif
	
	// Clean data in driver
	CleanDriverData();

	// Clean Received data
	CleanReceivedData();

	if(::WaitForSingleObject( m_hCleanedEvt, 3000) == WAIT_TIMEOUT)
	{
		;//		ASSERT(false);
	}

	::ResetEvent(m_hCleanedEvt);

}


void Rs232Module::CleanReceivedData()
{
	#ifdef SP_LEVEL1
	TRACE("Rs232Module::CleanReceivedData(): Set Reset Event ---3.3\n");
	#endif
	// Clean the received data and go 
	// to the initialized state
	m_bReset = true;
	::SetEvent( m_hResetEvt);

	#ifdef SP_LEVEL1
	TRACE("Set the Reset Event  -----3.4\n");
	#endif
}		


void Rs232Module::CleanDriverData()
{
    unsigned char buf[512];
	COMSTAT	comstat;
    DWORD	errors;

	DWORD dwReadLen;

	#ifdef SP_LEVEL1
	TRACE("Rs232Module::CleanDriverData() ----3.2\n");
	#endif

	while(1)
	{
		if( ::WaitForSingleObject(stopEvent, 0)==WAIT_OBJECT_0 ) 
		{
            ::ClearCommError(hCOM, &errors, &comstat);
			#ifdef SP_LEVEL1
			TRACE("Rs232Module::CleanDriverData()---3.2.1\n");
			#endif
			break;
		}


        ::ClearCommError(hCOM, &errors, &comstat);

		if( !comstat.cbInQue )
        {
			break;
        }
        int dwBytesRead = (DWORD) comstat.cbInQue;
        
		if( 512 <= (int) dwBytesRead ) 
            dwBytesRead = 512;

		if( ::ReadFile(hCOM, buf, 512, &dwReadLen, NULL) )
		{
			#ifdef SP_LEVEL1
			TRACE("Rs232Module::CleanDriverData()---3.2.3\n");
			#endif
			if(dwReadLen!=0)
			{
				continue;
			}
			else
			{
				
				#ifdef SP_LEVEL1
				TRACE("Rs232Module::CleanDriverData()---3.2.4\n");
				#endif
				#if 0
					string tmpStr((const char *)buf, dwReadLen);					
					tmpStr[dwReadLen] = '\0';
					TRACE(">> CleanDriverData(): Cleaned Data %s\n", tmpStr.c_str());
				#endif
			}
		}
		else
			break;
	}
}
		

								
DWORD Rs232Module::ThreadFunc()
{
	COMSTAT	comstat;
    DWORD	errors;
	int     getDataState=0;


	while(1) 
	{
		if( ::WaitForSingleObject(stopEvent, 0)==WAIT_OBJECT_0 ) 
		{
            ::ClearCommError(hCOM, &errors, &comstat);
			
			#ifdef SP_LEVEL1
			TRACE("Rs232Module::ThreadFunc(): Get Stop Event!!\n");
			#endif
			break;
		}
		
		::ClearCommError(hCOM, &errors, &comstat);

		// Reset Event is signaled
		if(::WaitForSingleObject(m_hResetEvt, 0) ==WAIT_OBJECT_0)
		{
			#ifdef SP_LEVEL1
			TRACE("Rs232Module::ThreadFunc(): Get Reset Event!! ----3.2\n");
			#endif
			getDataState = 0;
			writeIndex = 0;
			respDataBufLen = 0;
			m_idxRespCommand = 0;
			result.clear();
			m_bReset = false;
			::ResetEvent( m_hResetEvt);
			::ResetEvent( m_hParserEvt);
			::SetEvent(m_hCleanedEvt);
			#ifdef SP_LEVEL1
			TRACE("Exit the Reset Block!! ----3.3\n");
			#endif
		}

		// Cancel Event is signaled
		if(::WaitForSingleObject(m_hCancelEvt, 0) == WAIT_OBJECT_0)
		{
			#ifdef SP_LEVEL1
			TRACE("Rs232Module::ThreadFunc(): Get Cancel Event\n");
			#endif
			CancelSync();

			::ResetEvent(m_hCancelEvt);
			#ifdef SP_LEVEL1
			TRACE("Rs232Module::ThreadFunc(): Exit Cancel Event\n");
			#endif
		}
        
		if( !comstat.cbInQue )
        {
            Sleep(1);
            continue;
        }

        int dwBytesRead = (DWORD) comstat.cbInQue;
		if( RESP_BUFFER_LENGTH <= (int) dwBytesRead ) 
            dwBytesRead = RESP_BUFFER_LENGTH;

		if( ::ReadFile(hCOM, rawchars, dwBytesRead, &dwRead, NULL) ) 
		{         
			::ClearCommError(hCOM, &errors, &comstat);
			dwBytesRead = (DWORD) comstat.cbInQue;

            if( dwRead==0 )
                continue;

			m_iRead = dwRead;
			DumpData(rawchars, m_iRead);
			
			m_idxReadRawBuffer = 0;

		#ifdef SP_DEBUG
			if(m_iRead < 500)
			{
				string sDebug((const char *)rawchars, m_iRead);		
				sDebug[m_iRead] = '\0';				
				TRACE("Target: %s\n", sDebug.c_str());
			}
		#endif

			SYSTEMTIME  cur_time;
		    GetLocalTime(&cur_time);
            TRACE ("After ReadFile: %04d_%02d_%02d_%02d_%02d_%02d_%03d\r\n",
                     cur_time.wYear, cur_time.wMonth, cur_time.wDay,
                     cur_time.wHour, cur_time.wMinute, cur_time.wSecond, cur_time.wMilliseconds);


#if (defined __ATA30__) || (defined __ATA20__) || (defined __ATA_LOAD30_TEST20__)
			TRACE ("WriteIndex = %d, dwRead = %d\r\n", writeIndex, dwRead);
			
			if (writeIndex + dwRead >= RESP_BUFFER_LENGTH-1)
			{
				TRACE ("[Error] writeIndex + dwRead >= RESP_BUFFER_LENGTH-1\r\n");
				writeIndex = 0;
			}

			if (m_bNeedEndWithCRLF)
			{
				rawchars[dwRead] = '\r';
				rawchars[dwRead+1] = '\n';
				rawchars[dwRead+2] = 0;
				dwRead += 2;
			}

			if (writeIndex > 0 && dwRead == 1 && respDataBuf[writeIndex-1] == '\r' && rawchars[0] == '\n')
			{
				TRACE ("Got the last \\n\r\n");
				respDataBuf[writeIndex-1] = 0;
				respDataBufLen = writeIndex - 1;
				AnalyingRespString(writeIndex-1);
				writeIndex = 0;
			}
			else
			{

				memcpy (respDataBuf+writeIndex, rawchars, dwRead);
				writeIndex += dwRead;
				respDataBuf[writeIndex] = 0;

				if (respDataBuf[writeIndex-2] == '\r' && respDataBuf[writeIndex-1] == '\n')
				{
					TRACE ("Got the last block with CR & LF\r\n");
					respDataBuf[writeIndex-2] = 0;
					respDataBufLen = writeIndex - 2;
					AnalyingRespString(writeIndex-2);
					writeIndex = 0;
				}
			}


#else
            char tempbuff[RESP_BUFFER_LENGTH];
			memcpy (tempbuff, rawchars, m_iRead);
			tempbuff[m_iRead] = 0;
		    TRACE ("\r\n................\r\n");
			TRACE ("Readlength = %d\r\n", m_iRead);

			if (writeIndex == 0 || m_bNeedStartWithCRLF)
			{
				if (rawchars[0] != '\r')
				{
					TRACE ("Add CRLF in the front\r\n");

					rawchars[0] = '\r';
					rawchars[1] = '\n';
					memcpy (rawchars+2, tempbuff, m_iRead);
					
					m_iRead +=2;
					rawchars[m_iRead] = 0;

					m_bNeedStartWithCRLF = false;
				}
			}

			if (m_bNeedEndWithCRLF)
			{
				rawchars[m_iRead] = '\r';
				rawchars[m_iRead+1] = '\n';
				rawchars[m_iRead+2] = 0;
				m_iRead += 2;
				
				m_bNeedEndWithCRLF = false;
			}
  
		    GetLocalTime(&cur_time);
            TRACE ("Before GetOneCrLf: %04d_%02d_%02d_%02d_%02d_%02d_%03d\r\n",
                     cur_time.wYear, cur_time.wMonth, cur_time.wDay,
                     cur_time.wHour, cur_time.wMinute, cur_time.wSecond, cur_time.wMilliseconds);
			
			GetOneCrLf(getDataState, &rawchars[0], m_iRead);

		    GetLocalTime(&cur_time);
            TRACE ("After GetOneCrLf: %04d_%02d_%02d_%02d_%02d_%02d_%03d\r\n",
                     cur_time.wYear, cur_time.wMonth, cur_time.wDay,
                     cur_time.wHour, cur_time.wMinute, cur_time.wSecond, cur_time.wMilliseconds);

#endif
			
        }
	}

	return 0;
}

bool Rs232Module::Write(const char *Buf, const int &size)
{
	unsigned long wbytes;

	writeIndex = 0;
	if( WriteFile(hCOM, Buf, size, &wbytes, NULL)==FALSE )
	{
		TRACE("***Rs232Module::Write() Fail--case I\n");
		return false;
	}
	else if( wbytes<size )
	{
		TRACE("***Rs232Module::Write() Fail--case II\n");
		return false;
	}

	return true;
}


bool Rs232Module::Suspend()
{	
	return true;
}

bool Rs232Module::Resume()
{
	return true;
}


bool Rs232Module::GetPduToBuf(char ch)
{
	if(ch == cr)
	{
		return false;
	}
	else if(ch == lf)
	{
		return true;
	}
	else
	{
		m_pduBuf[m_idxPduBuf++]=ch;
	}
	return false;
}


bool Rs232Module::GetPduToBuf()
{
	unsigned char ch;
	unsigned char state=0;

	m_idxPduBuf = 0;

	while(GetRawData(ch))
	{

		switch(state)
		{
		case STATE_WAIT_CR:
			if(ch == cr)
				state = STATE_WAIT_LF;
			else
				m_pduBuf[m_idxPduBuf++]=ch;

			break;

		case STATE_WAIT_LF:
			if(ch == lf)
			{
				m_pduBuf[m_idxPduBuf]='\0';
				#ifdef SP_LEVEL1
				TRACE("GetPduToBuf:%s\n", m_pduBuf);
				#endif
				return true;
			}
			break;

		default:
			break;
		}

	}

	GetDeviceData();

	if(GetPduToBuf(m_idxPduBuf, &state))
	{
		return true;
	}


	return false;
}


bool Rs232Module::GetPduToBuf(int &idx, unsigned char *state)
{
	unsigned char ch;

	while(GetRawData(ch))
	{
		switch((*state))
		{
		case STATE_WAIT_CR:
			if(ch == cr)
				*state = STATE_WAIT_LF;
			else
				m_pduBuf[m_idxPduBuf++]=ch;

			break;

		case STATE_WAIT_LF:
			if(ch == lf)
			{
				m_pduBuf[m_idxPduBuf]='\0';
				#ifdef SP_LEVEL1
				TRACE("GetPduToBuf:%s\n", m_pduBuf);
				#endif
				return true;
			}
			break;

		default:
			break;
		}
 
	}

	GetDeviceData();

	if(GetPduToBuf(idx, state))
	{
		return true;
	}
	return false;
}


bool Rs232Module::GetDeviceData()
{
	COMSTAT	comstat;
	DWORD	errors;


	while(1)
	{
		::ClearCommError(hCOM, &errors, &comstat);
		if( !comstat.cbInQue )
		{
			Sleep(1);
			continue;
		}

		int dwBytesRead = (DWORD) comstat.cbInQue;
        if( 512 <= (int) dwBytesRead ) 
            dwBytesRead = 512;

		if( ::ReadFile(hCOM, rawchars, dwBytesRead, &dwRead, NULL) ) 
		{            
            if( dwRead==0 )
                continue;

			m_iRead = dwRead;
			DumpData(rawchars, m_iRead);
			
			m_idxReadRawBuffer = 0;

		#ifdef D_DEBUG
			string sDebug((const char *)rawchars, m_iRead);	
			sDebug[m_iRead]='\0';
			TRACE("GetDeviceData: %s\n", sDebug.c_str());
		#endif

			return true;
        }
	}
	return true;
}


bool Rs232Module::GetOneCrLf(int &state, unsigned char *data, int dataLength)
{
	unsigned char ch;
	int loopIndex = 0;

#ifdef SP_DEBUG

	char buffer[RESP_BUFFER_LENGTH];

	memcpy(buffer, data, dataLength);
	buffer[dataLength] = '\0';

	TRACE("GetOneCrLf from driver:%s\n", buffer);
#endif

	#ifdef SP_LEVEL1
	TRACE("\tGetOneCrLf()\n");
	#endif

	while(GetRawData(ch))
	{
		if(::WaitForSingleObject(m_hResetEvt, 0) == WAIT_OBJECT_0)
		{
			//#ifdef SP_LEVEL1
			TRACE("\tGetOneCrLf(): exit by reset event\n");
			//#endif
			break;
		}

		FillBufferByState( ch, state);


		loopIndex ++;
		if (state == 0 && loopIndex < dataLength)
		{
			//TRACE ("state == 0 && loopIndex < dataLength continue analyse.\r\n");
			//state = 2;
		}
	}

	return true;
}

void Rs232Module::ResetGetPduState()
{
	m_idxPduBuf=0;
}

void Rs232Module::FillBufferByState(unsigned char ch, int &state)
{
//	unsigned char ch2;

	#ifdef SP_LEVEL1
	TRACE("\t\tFillBufferByState(): Enter\n");
	#endif

	switch(state)
	{
	case 0:
		// Get the first tag of CR
		#ifdef SP_LEVEL1
		TRACE("\t\t\tstate = 0\n");
		#endif
		if(ch == GetCrByte())
		{
			state = 1;
			m_respCommand[m_idxRespCommand]='\0';
		}
	break;

	case 1:
		// Get the first tag of Lf
		#ifdef SP_LEVEL1
		TRACE("\t\t\tstate = 1\n");
		#endif
		if(ch == GetLfByte())
		{
			state = 2;
			writeIndex = 0;
		}
	break;

	case 2:
		// Save the Data to respDatabuf if not the second CR
		#ifdef SP_LEVEL1
		TRACE("\t\t\tstate = 2\n");
		#endif
		if(m_waitGtAndSpace)
		{
			#ifdef SP_LEVEL1
			TRACE("\t\t\tstate = 2 in wait GT spac\n");
			#endif
			if(CheckIfGetGtandSpace(ch))
			{
				::SetEvent(m_hWaitGtAndSpace);
				m_waitGtAndSpace = false;				
				state = 0;
			}
		}
		else
		{
			if(ch!=GetCrByte())
			{
				if(writeIndex<RESP_BUFFER_LENGTH)
					respDataBuf[writeIndex++]=ch;
				if (writeIndex >= 20490)
				{
					TRACE ("Get the last char [%d] = %c", writeIndex, ch);
				}
			}
			else
			{
#if 0
				// 0xd is in the middle of the string, ignore it
				if (writeIndex + 2 < m_iRead) 
				{
					if(writeIndex<RESP_BUFFER_LENGTH)
						respDataBuf[writeIndex++]=ch;
				}
				else
				{
					state = 3;
				}
#endif

				if(writeIndex!=0)  //1)
				{
					// respDataBuf not an empty string, CR indicates end
					//if (m_idxReadRawBuffer + 2 >= m_iRead)
						state = 3;
					TRACE ("Find the 2nd CR, Set state = 3\r\n");
				}
				else
				{
					// respDataBuf an empty string
					// It must get into the condition of
					//    0x0d 0x0a 0x0d 0x0A
					state = 1;
					writeIndex = 0;
				}
			}
		}

	break;

	case 3:
		// Get the 2nd Lf
		#ifdef SP_LEVEL1
		TRACE("\t\t\tstate = 3\n");
		#endif
		if(ch == GetLfByte())
		{
			TRACE ("Find the 2nd LF, writeIndex = %d\r\n", writeIndex);
			if(!m_waitGtAndSpace)
			{
				respDataBuf[writeIndex]='\0';

			#ifdef SP_DEBUG
				TRACE("FillBufferByState for parsing: %s\n", respDataBuf);
			#endif

				TRACE ("start to analyse.\r\n");
				AnalyingRespString(writeIndex);
				state = 0;
				writeIndex = 0;

				m_idxRespCommand=0;
			}

		}
		else // if it is not LF after CR, set state = 2
		{
			TRACE ("Do not find the 2nd LF, writeIndex = %d\r\n", writeIndex);
			respDataBuf[writeIndex++] = GetCrByte();
			respDataBuf[writeIndex++] = ch;
			state = 2;
		}
	break;

	default:
	break;

	}
}


void Rs232Module::SetWaitGtAndSpace(bool flag)
{
	m_waitGtAndSpace = flag;
}


void Rs232Module::SetPduFlag(bool flag)
{
	pduFlag = flag;
}



bool Rs232Module::CheckIfGetGtandSpace(char *data, int index)
{

	if(data[index]=='>' && data[index+1]==' ')
		return true;
	else
		return false;
}


bool Rs232Module::CheckIfGetGtandSpace(char a)
{
	unsigned char ch;

//	while(GetRawData(ch))
//	{
		if(a == '>')
		{
			while(GetRawData(ch))
			{
				if( ch == ' ')
					return true;
				else
					return false;
			}

			GetDeviceData();
			if(GetRawData(ch))
			{
				if(ch == ' ')
					return true;
				else
					return false;
			}
		}
		else
		{
			return false;
		}
//	}

	GetDeviceData();

//	return CheckIfGetGtandSpace();
	return true;

}


HANDLE Rs232Module::GetWaitGtAndSpaceEvent()
{
	return m_hWaitGtAndSpace;
}


char Rs232Module::GetCrByte()
{
	return cr;
}


char Rs232Module::GetLfByte()
{
	return lf;
}

void Rs232Module::CleanResult(ATResult& rt)
{
	
	rt.resultLst.clear();
	
	#ifdef SP_LEVEL1
	TRACE(">>>>>Rs232Module::CleanResult()<<<<<\n");
	#endif
}


/*
bool Rs232Module::GetPdu(char *buf, const int maxLen)
{
    int wi = 0;
	unsigned char ch;
	unsigned char ch2;

	while(1)
	{
		if(ch == cr)
		{
			if (ch2 == lf)
			{
				buf[wi] = '\0';
				return true;
			}
			else
				return false;
		}
		else
		{
			if(wi < maxLen)
				buf[wi++] = ch;
		}
		
	}

}
*/

bool Rs232Module::GetPdu(char *buf, const int maxLen)
{
    int wi = 0;

	COMSTAT	comstat;
    DWORD	errors;
	
	int getPduState=0;


	while(1)
	{
		//Get the PDU DATA from UART
		if( ::WaitForSingleObject(stopEvent, 0)==WAIT_OBJECT_0 ) 
		{
            ::ClearCommError(hCOM, &errors, &comstat);
			break;
		}


        ::ClearCommError(hCOM, &errors, &comstat);

		if( !comstat.cbInQue )
        {
            Sleep(1);
            continue;
        }

        int dwBytesRead = (DWORD) comstat.cbInQue;
        if( 512 <= (int) dwBytesRead ) 
            dwBytesRead = 512;


		if( ::ReadFile(hCOM, m_pduData, dwBytesRead, &m_dwRead, NULL) ) 
		{            
            if( m_dwRead==0 )
                continue;

			m_iPduRead = m_dwRead;


			// Fill the data into m_pduDATA
			for(int i=0; i<m_iPduRead && !stopEvent; i++)
			{
				switch(getPduState)
				{
				case 0:
					if(m_pduData[i]==cr)
						getPduState=1;
					else
						buf[wi++]=m_pduData[i];

				break;

				case 1:
					if(m_pduData[i]==lf)
					{
						buf[wi]='\0';
						return true;
					}
					else
					{
						return false;
					}
				break;
				}
			}
        }
	}

	return true;
}


bool Rs232Module::GetList(char* ptr, int& wi, ATParamLst& lst)
{
	ATParamElem elems;
	bool		intervalFlag=false;

	bool stag = false;

	wi = 0;
	readIndex++;  // Skip (
	
	while(respDataBuf[readIndex]!=')')
	{
		switch(respDataBuf[readIndex])
		{
		case ',':
			
			GetParam(ptr, elems, stag);
			stag = false;
			break;

		case '"':
			GetString(ptr, wi, elems);
			stag = true;
			wi = 0;
			break;

		case '-':
			// Case of interval
			intervalFlag = true;
			
			ptr[wi]='\0';
			if(IsNumber(ptr))
				elems.int_range_begin = atoi(ptr);
			else
				return false;

			wi = 0;
			readIndex++;
			while(respDataBuf[readIndex]!=')')
			{
				ptr[wi++] = respDataBuf[readIndex++];
			}

			ptr[wi]='\0';
			if(IsNumber(ptr))
				elems.int_range_end = atoi(ptr);
			else
				return false;
			

			break;

		case '\0':
			return false;
			break;

		case '(':
			GetList(ptr, wi, elems);
			break;

		default:
			ptr[wi++] = respDataBuf[readIndex++];
			break;

		}
	}


	if(intervalFlag)
	{
		elems.type = AT_INTERVAL;
	}
	else
	{
		elems.type = AT_PARA_LIST;
	
	}
	lst.eleLst.push_back(elems);
	
	
	return true;
}


bool Rs232Module::GetList(char *ptr, int& wi, ATParamElem& elem)
{

	ATParamElem elemLocal;

	elem.type = AT_PARA_LIST;

	bool stag = false;

	wi = 0;
	readIndex++;  // Skip (
	
	while(respDataBuf[readIndex]!=')')
	{
		switch(respDataBuf[readIndex])
		{
		case ',':
			GetParam(ptr, elem, stag);
			ptr[0]='\0';;
			wi = 0;
			stag = false;
			break;

		case '"':
			GetLstString(ptr, wi, elem);

			stag = true;
			
			break;

		case '-':
			// Case of interval
			elem.type = AT_INTERVAL;
			ptr[wi]='\0';
			
			if(IsNumber(ptr))
				elem.int_range_begin = atoi(ptr);
			else
				return false;

			wi = 0;
			readIndex++;
			while(respDataBuf[readIndex]!=')')
			{
				ptr[wi++] = respDataBuf[readIndex++];
			}
			readIndex--;
			ptr[wi]='\0';
			if(IsNumber(ptr))
				elem.int_range_end = atoi(ptr);
			else
				return false;

			break;

		case '\0':
			return false;
			break;

		case '(':
			GetList(ptr, wi, elemLocal);
			break;

		default:
			ptr[wi++] = respDataBuf[readIndex];
			break;

		}
		readIndex++;
	}
	ptr[wi]='\0';

	
	if(sizeof(ptr)!=0)
		GetParam(ptr, elem, stag);


	return true;
}


bool Rs232Module::SkipComma( char *ptr, int& ri, const int size)
{
	while(ri < size && *(ptr+ri)!=',')
		ri++;

	if (ri == size)
	{
		ri--;
		return false;
	}
	else
		return true;
}


bool Rs232Module::GetString(char *ptr, int& wi, ATParamLst& lst)
{
	
	ATParamElem  elem;

	readIndex++; // Skip "

	wi = 0;

	while(respDataBuf[readIndex]!='"')
	{
		switch(respDataBuf[readIndex])
		{
		case '\0':
			return false;

			break;

		default:
			ptr[wi++] = respDataBuf[readIndex++];
			break;
		}
	}


	ptr[wi]='\0';
	elem.type = AT_STRING;
	elem.str_value = string(ptr);
	lst.eleLst.push_back(elem);


	while(respDataBuf[readIndex]!='\0'
	&& respDataBuf[readIndex]!=',')
	{
		readIndex++;
	}
	
	if(respDataBuf[readIndex]==',')
		readIndex++;

	if(respDataBuf[readIndex]=='\0')
		readIndex--;

	return true;
}

bool Rs232Module::GetString(char *ptr, int& wi, ATParamElem& elem)
{
	ATParamElem  elemLocal;

	readIndex++; // Skip "

	wi = 0;

	while(respDataBuf[readIndex]!='"')
	{
		switch(respDataBuf[readIndex])
		{
		case '\0':
			return false;

			break;

		default:
			ptr[wi++] = respDataBuf[readIndex++];
			break;
		}
	}


	ptr[wi]='\0';
	elemLocal.type = AT_STRING;
	elemLocal.str_value = string(ptr);
	

	while(respDataBuf[readIndex]!='\0'
	&& respDataBuf[readIndex]!=')'
	&& respDataBuf[readIndex]!=',')
	{
		readIndex++;
	}
	
	if(respDataBuf[readIndex]==',')
		readIndex++;

	if(respDataBuf[readIndex]=='\0'
	|| respDataBuf[readIndex]==')')
		readIndex--;

	return true;
}


bool Rs232Module::IsNumber(char *ptr)
{
	int len = strlen(ptr);

	for(int i =0; i<len; i++)
	{
		if( (*(ptr+i) < '0')
		||  (*(ptr+i) > '9')
		)
		{
			return false;
		}
	}
	return true;
}


bool Rs232Module::GetLstString(char *ptr, int& wi, ATParamElem& elem)
{
	ATParamElem  elemLocal;

	readIndex++; // Skip "

	wi = 0;

	while(respDataBuf[readIndex]!='"')
	{
		switch(respDataBuf[readIndex])
		{
		case '\0':
			return false;

			break;

		default:
			ptr[wi++] = respDataBuf[readIndex++];
			break;
		}
	}

	

	ptr[wi]='\0';


	return true;
}


bool Rs232Module::GetStringToBuf(char *ptr, int& wi, const int size)
{
	wi = 0;

	readIndex++; // Skip one "

	while(respDataBuf[readIndex]!='"')
	{
		switch(respDataBuf[readIndex])
		{
		case '\0':
			return false;

			break;

		default:
			ptr[wi++] = respDataBuf[readIndex++];
			break;
		}

		if(readIndex>=size)
			return false;
	}

	ptr[wi]='\0';

	#ifdef SP_DEBUG
		string tmpString((const char *)ptr, wi);
		tmpString[wi]='\0';	
		TRACE("Rs232Module::GetStringToBuf():  %s\n", tmpString.c_str());
	#endif

	return true;
}


bool Rs232Module::UnsolicitedGetPdu(char *buf, const int bufLen, ATParamLst& lst)
{

	ATParamElem elem;

	if(lst.eleLst[0].str_value == "+CMT"
	|| lst.eleLst[0].str_value == "+CBM"
	|| lst.eleLst[0].str_value == "+CDS")
	{
		// Get pdu data to buffer
		GetPdu(buf, bufLen);

		// Set up elem
		elem.type = AT_STRING;
		elem.str_value = string(buf);

		// Push to the elem to elemlst
		lst.eleLst.push_back(elem);
	}

	return true;
}


bool Rs232Module::GetRawData(unsigned char &ch)
{
	if( m_idxReadRawBuffer == m_iRead )
		return false;
	else
		ch = rawchars[m_idxReadRawBuffer++];

	return true;
}

void Rs232Module::SetCurCommand(unsigned char *ptr, int len)
{
	memcpy(m_curCommand, ptr, len);
	m_curCommand[len]='\0';
}


HANDLE Rs232Module::GetWriteEvent()
{
	return m_hWriteEvt;
}


HANDLE Rs232Module::GetParserEvent()
{
	return m_hParserEvt;
}

void Rs232Module::GetResult(ATResult& rt)
{
	#ifdef SP_DEBUG
	TRACE(" ----AP COPY DATA FROM PARSER \n");
	#endif
	rt = result;
}


void Rs232Module::SetCallBack(void(*unSolicitedFunc)(ATResult& result, int handle_index))
{
	m_usc_cb = unSolicitedFunc;	
}

void Rs232Module::SetHandleIndex(int handleIndex)
{
	handle_index = handleIndex;	
}

void Rs232Module::SetStartWithCRLF(bool bNeed)
{
	m_bNeedStartWithCRLF = bNeed;
}

void Rs232Module::GetCommandResult (char *result, int &len) 
{
	memcpy (result, respDataBuf, respDataBufLen);
	result[respDataBufLen] = 0;
	len = respDataBufLen;
}

void Rs232Module::SetEndWithCRLF(bool bNeed)
{
	m_bNeedEndWithCRLF = bNeed;
}


void Rs232Module::SetExpectString(string command)
{
	result.expectedCommand=command;
	#ifdef SP_LEVEL1
	TRACE("Expected Command: %s\n", command.c_str());
	#endif
}


bool Rs232Module::AnalyingRespString(int len)
{

	//int size1 = strlen(respDataBuf);
	int size = len;
	char	tmp[1024];
	int		tmpWI =0;
	bool	ignoreWhiteSpace = false;
	string endString;
	string command = "";
	bool    stag = false;
	bool    getDbQEmptyString=false;


	ATParamElem elem;
	ATParamLst	paraLst;

    ATResult atr_temp;
    ATParamElem elem_temp;
    ATParamLst	paraLst_temp;

	readIndex =0;

	#ifdef T_DEBUG
		string tmpStr(respDataBuf, size);	
		tmpStr[size]='\0';
		TRACE("PARSER: [Entire resp] %s\n", tmpStr.c_str());
	#endif
	
	
	
    sprintf (tmp, "\r\nsize:%d", size);
    //TRACE("###############\r\n");
    //TRACE(respDataBuf);
    //TRACE(tmp);
    //TRACE("\r\n###############\r\n\r\n");
    MTRACE(g_hEBOOT_DEBUG,"###############");
	MTRACE(g_hEBOOT_DEBUG, "%s",respDataBuf);
	  MTRACE(g_hEBOOT_DEBUG,"###############");
    
	
    if (size <= 0)
    {
        return true;
    }
	else if(size > 500)
	{
		goto ATA_CALLBACK;
	}

	while(readIndex!=(size+1) && !m_bReset)
	{
		if(::WaitForSingleObject( m_hResetEvt, 0) == WAIT_OBJECT_0)
		{
			#ifdef SP_LEVEL1
			TRACE("\t\t\t\tAnalyingRespString(): exit by reset event\n");
			#endif
			break;
		}

		switch(respDataBuf[readIndex])
		{
		case '\0':
			tmp[tmpWI] = '\0';
			if (command.empty())
				command = tmp;
			else
				endString = tmp;

            if(command == "> ")
			{
				tmp[0]='\0';
			}
			else if(command == "OK"
			|| command == "ERROR"
			|| command == "CONNECT"
			)
			{

				// Just fill the data to the result structure
				result.result = AT_SUCCESS;
				
				elem.type = AT_STRING;
				elem.str_value = command;
				paraLst.eleLst.push_back(elem);

				
				result.resultLst.push_back(paraLst);

				Sync();

				tmp[0]='\0';


			}
			else if(command == "+CME ERROR"
				 || command == "+CMS ERROR")
			{
				ATParamElem telem;
				telem.type = AT_INTEGER;
				telem.int_value = atoi(tmp);
				paraLst.eleLst.push_back(telem);

				result.result = AT_SUCCESS;
				result.resultLst.push_back(paraLst);

#ifdef SPSP_DEBUG
				cout << "result in parser: \n";
				cout << result << endl;
#endif
				tmp[0] = '\0';
				Sync();
				
			}
			/**
			else if(IsUnsolicited(command, paraLst))
			{
				//Check if current paraLst  is a unsolicited command
				// Call the unsolicited call back function to notify
				// upper layer.

				if(sizeof(tmp)!=0)
					GetParam(tmp, paraLst, false);

				UnsolicitedGetPdu(tmp, 2047, paraLst);

				unsolicitedRt.result = AT_SUCCESS;
				unsolicitedRt.resultLst.push_back(paraLst);

#ifdef SPSP_DEBUG
				cout << "result in parser: \n";
				cout << result << endl;
#endif

				//m_usc_cb(unsolicitedRt, handle_index);
				CleanResult(unsolicitedRt);
			}
			**/
			else if(command == result.expectedCommand )
			{
				// Get the pdu block and push it the the parameter list

				if((strlen(tmp)!=0 )
				|| (getDbQEmptyString == true))
					GetParam( tmp, paraLst, stag);

				if(pduFlag)
				{
					// Get PDU
					TRACE("PARSER: Get PDU\n");
					GetPduToBuf();

					// Put to parameter list
					elem.type = AT_STRING;
					elem.str_value = string(m_pduBuf);

					paraLst.eleLst.push_back(elem);

					pduFlag = false;
				}
				result.resultLst.push_back(paraLst);
			}
			else
			{
				result.result = AT_SUCCESS;
				
				elem.type = AT_STRING;
				elem.str_value = tmp;
				paraLst.eleLst.push_back(elem);

				
				result.resultLst.push_back(paraLst);
				TRACE(elem.str_value.c_str());

				tmp[0]='\0';
				TRACE("~~~~~~ONE COMMAND IS SKIPPED!\n");
                Sync();
			}

			break;

		case ',':
			tmp[tmpWI]='\0';
			GetParam( tmp, paraLst, stag);
			tmpWI = 0;
			stag = false;
			ignoreWhiteSpace = true;

			break;

		case ':':
			#ifdef SP_LEVEL1
			TRACE("+EFSL: 'XXXXXX' STEP 1\n");
			#endif
			tmp[tmpWI]='\0';
			ignoreWhiteSpace = true;

			//readIndex++;	// skip ':'

			command = tmp;

			elem.type = AT_STRING;
			elem.str_value = string(command);
			paraLst.eleLst.push_back(elem);

			tmpWI = 0; // clear tmp
			
			break;

		case '(':
			GetList(tmp, tmpWI, elem);
			paraLst.eleLst.push_back(elem);
			elem.paramLst.clear();

			SkipComma(respDataBuf, readIndex, size);

			tmp[0]='\0';
			break;

		case '"':	
			#ifdef SP_LEVEL1
			TRACE("+EFSL: 'XXXXXX' STEP 2\n");
			#endif
			if(!GetStringToBuf(tmp, tmpWI, (size+1)))
			{
				TRACE("**********+EFSL: 'XXXXXX' STEP 3\n");
				return false;
			}

			if(strlen(tmp)==0)
				getDbQEmptyString = true;

			stag = true;

			break;

		default:
			if(ignoreWhiteSpace == false)
			{
				tmp[tmpWI++] = respDataBuf[readIndex];
			}
			else
			{
				if(respDataBuf[readIndex]!=' ')
				{
					ignoreWhiteSpace = false;
					tmpWI = 0;
					tmp[tmpWI++] = respDataBuf[readIndex];
				}
			}
			
			break;
		}
		readIndex++;
	}	

ATA_CALLBACK:
    // Put after Sync(), after writecommand ready
    // Print out the whole result

	TRACE ("Pepare for ata callback.\r\n");
	MTRACE(g_hEBOOT_DEBUG,"Pepare for ata callback.\r\n" );
    memset (&atr_temp, 0x0, sizeof(ATResult));
    memset (&elem_temp, 0x0, sizeof(ATParamElem));
    memset (&paraLst_temp, 0x0, sizeof(ATParamLst));

	TRACE ("Put data into elem_temp.\r\n");
    elem_temp.type = AT_STRING;

	if (size < 10240) {
    	elem_temp.str_value = respDataBuf;
		memcpy (elem_temp.str_char, respDataBuf, size);
		elem_temp.str_len = size;
	} else {
		elem_temp.str_value = "Notify";
		memcpy (elem_temp.str_char, "Notify", strlen("Notify"));
		elem_temp.str_len = strlen("Notify");
	}

	TRACE ("Put data into paraLst_temp.\r\n");
    paraLst_temp.eleLst.push_back(elem_temp);

	TRACE ("Put data into atr_temp.\r\n");
	MTRACE (g_hEBOOT_DEBUG, "Put data into result.");
	atr_temp.result = AT_SUCCESS;
    atr_temp.resultLst.push_back(paraLst_temp);

    TRACE ("Before if (m_usc_cb != NULL)\r\n");
    if (m_usc_cb != NULL)
    {
		TRACE ("\r\nBefore call back\r\n");
        m_usc_cb(atr_temp, handle_index);
		TRACE ("\r\nAfter call back.\r\n");
    }

	if(size > 500)
	{
		Sync ();
	}

	return true;
}


void Rs232Module::Sync()
{

	#ifdef SP_LEVEL1
	TRACE("Rs232Module::Sync()     -----7\n");
	#endif
	// Check InProgressEvt to check if the result should be given up.
	
    TRACE("\tWaitForSingleObject(m_hInProgressEvt, 0)\n");
	if(::WaitForSingleObject(m_hInProgressEvt, 0) == WAIT_TIMEOUT)
	{
		// No command is in progressing
		#ifdef SP_LEVEL1
		TRACE("\tNo Command In progressing, Skip the result of cancel.\n");
		#endif
		CleanResult(result);
		return;
	}
	
	if( ::WaitForSingleObject( m_hResetEvt, 0)== WAIT_OBJECT_0)
	{
		#ifdef SP_LEVEL1
		TRACE(">>>>>ResetInSync: so Clean Result<<<\n");
		#endif
		CleanResult(result);
		return;
	}

	
	::SetEvent(m_hWriteEvt);
	
	// Write command copy data in this time period
#ifdef T_DEBUG
		if(result.resultLst.size()==0)	
		{	
				ASSERT(false);	
		}
#endif
	::WaitForSingleObject(m_hParserEvt, INFINITE);
	::ResetEvent(m_hParserEvt);

	#ifdef T_DEBUG
	if(result.resultLst.size()==0)
	{	
		ASSERT(false);
	}
#endif

	CleanResult(result);
}


bool Rs232Module::IsUnsolicited(string cmd, ATParamLst& lst)
{
    ATParamElem elem;

    TRACE ("IsUnsolicited:");
	TRACE(lst.eleLst[0].str_value.c_str());
	TRACE ("\r\n");

	if( cmd == "RING" )
	{
		return true;
	}

	if(lst.eleLst.size()==0)
		return false;

	if(lst.eleLst[0].type != AT_STRING)
		return false;

	// Then handled unsolicited command is listed here
	// This should take care of all the unsolicited command
	// listed in spec.
	if(
	   lst.eleLst[0].str_value == "RING"
	|| lst.eleLst[0].str_value == "NO CARRIER"
	|| lst.eleLst[0].str_value == "+CMT"
	|| lst.eleLst[0].str_value == "+CBM"
	|| lst.eleLst[0].str_value == "+CDS"
	|| lst.eleLst[0].str_value == "+CMTI"
	|| lst.eleLst[0].str_value == "+CBMI"
	|| lst.eleLst[0].str_value == "+CDSI"
	|| lst.eleLst[0].str_value == "+CLIP"
	|| lst.eleLst[0].str_value == "+EADC"
	|| lst.eleLst[0].str_value == "+GPIOS"
	|| lst.eleLst[0].str_value == "+EKPDS"
	|| lst.eleLst[0].str_value == "+BATS"
	|| lst.eleLst[0].str_value == "+CREG"
	|| lst.eleLst[0].str_value == "+CTEV"

	|| lst.eleLst[0].str_value == "+CGMR"
	|| lst.eleLst[0].str_value == "+EGMR"
	|| lst.eleLst[0].str_value == "+EGCMD"
	|| lst.eleLst[0].str_value == "+MSDC"
	)
		return true;

	else
		return false;
}



bool Rs232Module::GetParam(char *ptr, ATParamElem& elem, bool isString)
{

	ATParamElem elemLocal;

	if(isString)
	{
		elemLocal.type = AT_STRING;
		elemLocal.str_value = string(ptr);
	}
	else
	{
		if(strlen(ptr)==0)
			elemLocal.type = AT_OMIT;

		
		if(IsNumber(ptr))
		{
			elemLocal.type = AT_INTEGER;
			elemLocal.int_value = atoi(ptr);
		}
		else
		{
			elemLocal.type = AT_STRING;
			elemLocal.str_value = string(ptr);
		}
	}

	elem.paramLst.push_back(elemLocal);


	return true;
}

bool Rs232Module::GetParam(char *ptr, ATParamLst& lst, bool isString)
{

	ATParamElem elem;

	if(isString)
	{
		elem.type = AT_STRING;
		elem.str_value = string(ptr);
	}
	else
	{
		if(strlen(ptr)==0)
		{
			elem.type = AT_OMIT;

		}
		else
		{
			if(IsNumber(ptr))
			{
				elem.type = AT_INTEGER;
				elem.int_value = atoi(ptr);
			}
			else
			{
				elem.type = AT_STRING;
				elem.str_value = string(ptr);
			}
		}
	}
	lst.eleLst.push_back(elem);


	return true;
}


HANDLE Rs232Module::GetInProgressEvent()
{	
	return m_hInProgressEvt;
}


HANDLE Rs232Module::GetCleanedEvent()
{	
	return m_hCleanedEvt;
}

HANDLE Rs232Module::GetCancelEvent()
{
	return m_hCancelEvt;
}

void Rs232Module::CancelSync()
{
	DWORD state;

	#ifdef SP_LEVEL1
	TRACE("Rs232Module::CancelSync() -- Enter\n");
	TRACE("Rs232Module::CancelSync() -- m_ParserEvent HANDLE = %d\n", m_hParserEvt);
	#endif

	// Check InProgressEvt to check if the result should be given up.
	state = ::WaitForSingleObject(m_hInProgressEvt, 0);

	if(state == WAIT_TIMEOUT)
	{
		// No command is in progressing
		#ifdef SP_LEVEL1
		TRACE("\tNo Command In progressing, Skip the result of cancel.\n");
		#endif
		CleanResult(result);
		return;
	}

	CleanResult(result);
	
	//Setting the result to be AT_TIMEOUT
	result.result = AT_TIMEOUT;
	
	::SetEvent(m_hWriteEvt);
	
	// Write command copy data in this time period

	state = ::WaitForSingleObject(m_hParserEvt, INFINITE);
	::ResetEvent(m_hParserEvt);

	#ifdef SP_LEVEL1
	TRACE("Rs232Module::CancelSync() -- Exit\n");
	#endif

}


HANDLE Rs232Module::GetResetEvent()
{
	return m_hResetEvt;
}





