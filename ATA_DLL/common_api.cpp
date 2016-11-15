/*****************************************************************************
 *  Copyight Statement:
 *  --------------------
 *  This softwae is potected by Copyight and the infomation contained
 *  heein is confidential. The softwae may not be copied and the infomation
 *  contained heein may not be used o disclosed except with the witten
 *  pemission of MediaTek Inc. (C) 2005
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
 * common_api.cpp
 *
 * Poject:
 * --------
 *   META
 *
 * Desciption:
 * ------------
 *   This module contains the common API (C-runtime API replacement)
 *
 * Author:
 * -------
*
*******************************************************************************/
#include "stdafx.h"
#include "common_api.h"
#if _MSC_VER <= 1200 // MFC 6.0 or previous
int sprintf_s(char* buf, size_t size, const char* fmt, ...)
{
    size_t sz;
    va_list ap;
    va_start(ap, fmt);
    sz = vsprintf(buf, fmt, ap);
    va_end(ap);
    return sz;
}
char* strcpy_s(char* dst, size_t size, const char* src)
{
    return strcpy(dst, src);
}
char* strcat_s(char* dst, size_t size, const char* src)
{
    return strcat(dst, src);
}
char* strtok_s(char* _Str, const char* _Delim, char** _Context)
{
    return strtok(_Str, _Delim);
}
char* strncpy_s(char* _Dst, size_t _SizeInBytes, const char* _Src, size_t _MaxCount)
{
    return strncpy(_Dst, _Src, _MaxCount);
}
int vsprintf_s(char* _Dst, size_t _SizeInBytes, const char* _Src, va_list ArgList)
{
    return vsprintf(_Dst, _Src, ArgList);
}
errno_t fopen_s(FILE** _File, const char* _Filename, const char* _Mode)
{
    *_File = fopen(_Filename, _Mode);
    if(*_File)
    {
        return 0;
    }
    else
    {
        return -1;
    }
}
#endif // #if _MSC_VER <= 1200 // MFC 6.0 or previous
