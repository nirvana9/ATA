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

#include <stdio.h>
#include "StdAfx.h"

#ifndef  _AUTH_H_
#include "AUTH.h"
#endif

#ifndef _SP_BROM_H_
#include "sp_brom.h"
#endif

//#define false 0
//#define true 1

SP_AUTH_HANDLE_T  SPATE_m_sAuthHandle;
//===========================================================================
bool  SPATE_AUTH_Create( void )
{
    int ret = SP_AUTH_Create( &SPATE_m_sAuthHandle );
    if( ret != 0 )
        return false;
    return true;
}

//---------------------------------------------------------------------------
bool  SPATE_AUTH_Destroy( void )
{
    int ret;
    if( NULL == SPATE_m_sAuthHandle )
        return false;

    ret = SP_AUTH_Destroy( &SPATE_m_sAuthHandle );
    if( ret != 0 )
        return false;
    return true;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
bool  SPATE_AUTH_Load( const char *auth_filepath )
{
    int ret;
    if( NULL == SPATE_m_sAuthHandle )
        return false;

    ret = SP_AUTH_Load( SPATE_m_sAuthHandle, auth_filepath );
    if( ret != 0 )
        return false;
    return true;
}

//--------------------------------------------------------------------------
bool  SPATE_AUTH_Unload( void )
{
    int ret;
    if( NULL == SPATE_m_sAuthHandle )
        return false;

    ret = SP_AUTH_Unload( SPATE_m_sAuthHandle );
    if( ret != 0 )
        return false;
    return true;
}

//--------------------------------------------------------------------------
SP_AUTH_HANDLE_T SPATE_Get_AuthHandle( void )
{
    return SPATE_m_sAuthHandle;
}