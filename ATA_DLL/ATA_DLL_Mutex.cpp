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
 *   ATA_DLL_Mutex.cpp
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
#include "ATA_DLL_Mutex.h"

//-----------------------------------------------------------------------------
// mutex class
//-----------------------------------------------------------------------------

/**
 * Descriptions: FIXME
 */
ATA_DLL_Mutex::ATA_DLL_Mutex()
    :m_mutex(NULL)
{
    m_mutex = CreateMutex(NULL, FALSE, NULL);
}
/**
 * Descriptions: FIXME
 */
ATA_DLL_Mutex::~ATA_DLL_Mutex()
{
    if( NULL != m_mutex )
    {
        CloseHandle(m_mutex);
        m_mutex = NULL;
    }
}
/**
 * Descriptions: FIXME
 */
void ATA_DLL_Mutex::Lock()
{
    TRACE ("WaitForSingleObject(m_mutex, INFINITE)\r\n");
    if (m_mutex != NULL)
    {
        WaitForSingleObject(m_mutex, 3000);
    }
    TRACE ("WaitForSingleObject(m_mutex, INFINITE) ok\r\n");
}
/**
 * Descriptions: FIXME
 */
bool ATA_DLL_Mutex::TryLock()
{
    if( WAIT_OBJECT_0 == WaitForSingleObject(m_mutex, 0) )
    {
        return true;
    }
    else
    {
        return false;
    }
}
/**
 * Descriptions: FIXME
 */
void ATA_DLL_Mutex::Unlock()
{
    if (m_mutex != NULL)
    {
        ReleaseMutex(m_mutex);
    }
}
/**
 * Descriptions: FIXME
 */
ATA_DLL_Mutex_Sentry::ATA_DLL_Mutex_Sentry(
    ATA_DLL_Mutex  &mutex,
    std::string enter_hint,
    std::string leave_hint)
    :	m_mutex(mutex), m_enter_hint(enter_hint), m_leave_hint(leave_hint)
{
    m_mutex.Lock();
    TRACE ("m_mutex.Lock ok\r\n");
    if( 0 < m_enter_hint.size() )
    {
        //MTRACE(g_hMTRACE, "%s", m_enter_hint.c_str());
    }
}
/**
 * Descriptions: FIXME
 */
ATA_DLL_Mutex_Sentry::~ATA_DLL_Mutex_Sentry()
{
    if( 0 < m_leave_hint.size() )
    {
        //MTRACE(g_hMTRACE, "%s", m_leave_hint.c_str());
    }
    m_mutex.Unlock();
    TRACE ("m_mutex.unLock ok\r\n");
}