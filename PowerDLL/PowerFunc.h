/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2003
*
*****************************************************************************/

/*******************************************************************************
* Filename:
* ---------
*  PowerFunc.h
*
* Project:
* --------
*   HWTest
*
* Description:
* ------------
*   This file contains the definition of Power DLL.
*
* Author:
* -------
*  KS (mtk00451)
*
*==============================================================================
*           HISTORY
* Below this line, this part is controlled by PVCS VM. DO NOT MODIFY!!
*------------------------------------------------------------------------------
* $Revision:   1.0  $
* $Modtime:   Aug 11 2003 19:23:26  $
* $Log:   //mtkvs01/VMDATA/HWTest/archives/PowerDLL/PowerFunc.h-arc  $
 *
 *    Rev 1.0   Aug 14 2003 21:29:14   admin
 * Initial revision.
*
*------------------------------------------------------------------------------
* Upper this line, this part is controlled by PVCS VM. DO NOT MODIFY!!
*==============================================================================
*******************************************************************************/
#ifndef MTK_POWERFUNCDLL_H
#define MTK_POWERFUNCDLL_H

#define POWER_API __declspec(dllexport)

#ifdef __cplusplus
extern "C"
{
#endif

    POWER_API long Power_Init(char *resourceName);
    POWER_API bool Power_DeInit(long handle);
    POWER_API bool Power_Reset(long handle);
    POWER_API void Power_GetLastError(long handle, char *bufstr);

    POWER_API bool Power_SetOutput(long handle, double volt, double cur);
    POWER_API bool Power_SetOutput2(long handle, double volt, double cur);
    POWER_API bool Power_GetOutput(long handle, double *volt, double *cur);

    POWER_API bool Power_Measure(long handle, double *volt, double *cur);

#ifdef __cplusplus
}
#endif

#endif
