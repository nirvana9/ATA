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
*  Thread.cpp
*
* Project:
* --------
*   Maui sw - Phone Suite
*
* Description:
* ------------
*   This module contains the implementation of AThread.
*
* Author:
* -------
*  Angus Chen (mtk00306)
*
*==============================================================================
*           HISTORY
* Below this line, this part is controlled by PVCS VM. DO NOT MODIFY!!
*------------------------------------------------------------------------------
* $Revision:   1.1  $
* $Modtime:   Aug 12 2005 16:32:16  $
* $Log:   //mtkvs01/VMDATA/HWTest/archives/comm/Thread.cpp-arc  $
 * 
 *    Rev 1.1   Dec 08 2005 14:46:52   mtk00451
 * Change comm version
 *
 *   Rev 1.0   Feb 27 2004 18:28:12   admin
 *Initial revision.
 *
 *   Rev 1.0   Feb 27 2004 16:42:08   admin
 *Initial revision.
*
*------------------------------------------------------------------------------
* Upper this line, this part is controlled by PVCS VM. DO NOT MODIFY!!
*==============================================================================
*******************************************************************************/

#include "Thread.h"

// Constructor
AThread::AThread()
{
	m_hThread = NULL;
	m_ThreadId = 0;
}

// Destructor
AThread::~AThread()
{
}

// Start thread
bool AThread::StartThread()
{
	m_hThread = (HANDLE)_beginthreadex(
					NULL, 
					0, 
					(PBEGINTHREADEX_THREADFUNC)AThread::HelpThreadFunc, 
					(LPVOID)this, 
					0, 
					(PBEGINTHREADEX_THREADID) &m_ThreadId
				);

	if( m_hThread )
		return true;
	else
		return false;
}

// Wait for thread
void AThread::WaitForExit()
{
	if( m_hThread==NULL )
        return;
	WaitForSingleObject(m_hThread, INFINITE);
	CloseHandle(m_hThread);
    m_hThread=NULL;
}

// Help tread function
DWORD AThread::HelpThreadFunc(LPVOID param)
{
	AThread *pto = (AThread *)param;

	return pto->ThreadFunc();
}