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
*  PowerFunc.cpp
*
* Project:
* --------
*   HWTest
*
* Description:
* ------------
*   This file contains the implementaion of Power DLL.
*
* Author:
* -------
*  KS (mtk00451)
*
*==============================================================================
*           HISTORY
* Below this line, this part is controlled by PVCS VM. DO NOT MODIFY!!
*------------------------------------------------------------------------------
* $Revision:   1.1  $
* $Modtime:   Sep 24 2003 13:38:38  $
* $Log:   //mtkvs01/VMDATA/HWTest/archives/PowerDLL/PowerFunc.cpp-arc  $
 *
 *    Rev 1.1   Sep 24 2003 14:43:50   mtk00451
 * delete the allocate memory when return false
 *
 *    Rev 1.0   Aug 14 2003 21:29:14   admin
 * Initial revision.
*
*------------------------------------------------------------------------------
* Upper this line, this part is controlled by PVCS VM. DO NOT MODIFY!!
*==============================================================================
*******************************************************************************/
#include "PowerFunc.h"
#include "hp663x2.h"

static ViStatus global_err;

POWER_API long Power_Init(char *resourceName)
{
    ViSession *piHandle = new ViSession;
    global_err=hp663x2_init(resourceName, VI_FALSE, VI_FALSE, piHandle);
    if(global_err!=VI_SUCCESS)
    {
        delete piHandle;
        return 0;
    }

    /**
    global_err = hp663x2_reset(*piHandle);
    if(global_err!=VI_SUCCESS)
    {
        delete piHandle;
        return 0;
    }
    **/
    return (long) piHandle;
}

POWER_API bool Power_DeInit(long handle)
{
    if(handle==0)
        return false;
    ViSession *piHandle = (ViSession *)handle;
    bool ret=false;
    //global_err = hp663x2_reset(*piHandle);
    global_err = hp663x2_close(*piHandle);
    if(global_err==VI_SUCCESS)
        ret=true;
    delete piHandle;
    return ret;
}

POWER_API bool Power_Reset(long handle)
{
    if(handle==0)
        return false;
    ViSession *piHandle = (ViSession *)handle;
    global_err = hp663x2_reset(*piHandle);
    if(global_err==VI_SUCCESS)
        return true;
    else
        return false;
}

POWER_API bool Power_SetOutput(long handle, double volt, double cur)
{
    if(handle==0)
        return false;
    ViSession *piHandle = (ViSession *)handle;

    global_err = hp663x2_outputVoltCurr(*piHandle, volt, cur);
    if(global_err==VI_SUCCESS)
        return true;
    else
        return false;
}

POWER_API bool Power_SetOutput2(long handle, double volt, double cur)
{
    if(handle==0)
        return false;
    ViSession *piHandle = (ViSession *)handle;

    global_err = hp663x2_outputVoltCurr2(*piHandle, volt, cur);
    if(global_err==VI_SUCCESS)
        return true;
    else
        return false;
}

POWER_API bool Power_GetOutput(long handle, double *volt, double *cur)
{
    if(handle==0)
        return false;
    ViSession *piHandle = (ViSession *)handle;

    global_err = hp663x2_getVolt(*piHandle, HP663X2_VOLT_Q, volt);
    if(global_err!=VI_SUCCESS)
        return false;

    global_err = hp663x2_getCurr(*piHandle, HP663X2_CURR_Q, cur);
    if(global_err==VI_SUCCESS)
        return true;
    else
        return false;
}

POWER_API bool Power_Measure(long handle, double *volt, double *cur)
{
    if(handle==0)
        return false;
    ViSession *piHandle = (ViSession *)handle;

    global_err = hp663x2_measureVolt(*piHandle, HP663X2_MEAS_DC, volt);
    if(global_err!=VI_SUCCESS)
        return false;

    global_err = hp663x2_measureCurr(*piHandle, HP663X2_MEAS_DC, cur);
    if(global_err==VI_SUCCESS)
        return true;
    else
        return false;
}


POWER_API void Power_GetLastError(long handle, char *bufstr)
{
    if(handle==0)
        return;
    ViSession *piHandle = (ViSession *)handle;
    hp663x2_error_message(*piHandle, global_err, bufstr);
    return;
}
