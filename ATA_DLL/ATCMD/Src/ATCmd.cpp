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
 *   ATCmd.cpp
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

#include "Stdafx.h"
#include "..\header\ATCmd.h"

CATCmd::CATCmd()
{
    m_hThread = NULL;
    m_ThreadId = 0;
}

CATCmd::~CATCmd()
{
    if (m_hThread != NULL)
    {
        CloseHandle (m_hThread);
        m_hThread = NULL;
    }
}

bool CATCmd::Init(int com_port, int flow_control, int baud_rate)
{
    COMSTAT	comstat;
    DWORD	errors;

    char com_port_str[128];
    sprintf_s (com_port_str, 1024, "\\\\.\\COM%d", com_port);

    hCOM = ::CreateFile(com_port_str,
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
    dcb.BaudRate = band_rate;
    dcb.fBinary = TRUE;
    dcb.fParity = FALSE;

    if(flow_control == COMM_FC_HW)
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

    if(flow_control == COMM_FC_SW)
    {
        dcb.fOutX = TRUE;
        dcb.fInX = TRUE;
    }
    else
    {
        dcb.fOutX = FALSE;
        dcb.fInX = FALSE;
    }

    if(flow_control == COMM_FC_SW
            || flow_control == COMM_FC_NONE)
        dcb.fRtsControl = RTS_CONTROL_ENABLE;
    else if(m_flowCtrl == COMM_FC_HW)
        dcb.fRtsControl = RTS_CONTROL_HANDSHAKE;

    if(flow_control == COMM_FC_SW)
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

    if( ::SetupComm(hCOM, 8192, 8192)==FALSE )
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

    COMMTYPE commtype;
    if(cp.dwProvSubType == PST_MODEM && cp.dwProvCapabilities == 0xcd)
        commtype = COMMTYPE_USB;
    else if(cp.dwProvSubType == PST_RS232 && cp.dwProvCapabilities == 0xc7)
        commtype =  COMMTYPE_IRDA;

    COMMTIMEOUTS timeouts;
    timeouts.ReadIntervalTimeout = 0xFFFFFFFF;
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

bool CATCmd::StartThread()
{
    m_hThread = (HANDLE)_beginthreadex(
                    NULL,
                    0,
                    (PBEGINTHREADEX_THREADFUNC)CATCmd::HelpThreadFunc,
                    (LPVOID)this,
                    0,
                    (PBEGINTHREADEX_THREADID) &m_ThreadId
                );

    if( m_hThread )
        return true;
    else
        return false;
}

void CATCmd::StopThread()
{
    if (m_hThread != NULL)
    {
        CloseHandle (m_hThread);
        m_hThread = NULL;
    }
}

// Help tread function
DWORD CATCmd::HelpThreadFunc(LPVOID param)
{
    CATCmd *pto = (CATCmd *)param;

    return pto->ThreadFunc();
}


DWORD CATCmd::ThreadFunc()
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
            respDataBufLen = 0
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
        if( 512 <= (int) dwBytesRead )
            dwBytesRead = 512;



        if( ::ReadFile(hCOM, rawchars, dwBytesRead, &dwRead, NULL) )
        {
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
            GetOneCrLf(getDataState, &rawchars[0], m_iRead);
        }
    }

    return 0;
}
