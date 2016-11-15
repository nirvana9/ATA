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
*  ATFunc.h
*
* Project:
* --------
*   HWTest
*
* Description:
* ------------
*   This file contains the definition of ATDLL.
*   To Use ATDLL, users have to include this file.
*
* Author:
* -------
*  KS (mtk00451)
*
*==============================================================================
*           HISTORY
* Below this line, this part is controlled by PVCS VM. DO NOT MODIFY!!
*------------------------------------------------------------------------------
* $Revision:   1.5  $
* $Modtime:   Dec 07 2005 17:46:04  $
* $Log:   //mtkvs01/VMDATA/HWTest/archives/ATDLL/ATFunc.h-arc  $
 *
 *    Rev 1.5   Dec 08 2005 14:50:58   mtk00451
 * Add PMIC Support
 *
 *    Rev 1.4   Feb 13 2004 09:41:56   mtk00689
 * Add baud rate select feature and fix a few bugs
 *
 *    Rev 1.3   Sep 24 2003 14:40:40   mtk00451
 * Add flow control
 *
 *    Rev 1.2   Sep 19 2003 18:14:38   mtk00451
 * Add GPO and sleep mode
 *
 *    Rev 1.1   Sep 12 2003 14:46:40   mtk00451
 * Add ATDisableSleepMode
 *
 *    Rev 1.0   Aug 14 2003 21:25:52   admin
 * Initial revision.
*
*------------------------------------------------------------------------------
* Upper this line, this part is controlled by PVCS VM. DO NOT MODIFY!!
*==============================================================================
*******************************************************************************/
#ifndef MTK_ATFUNCDLL_H
#define MTK_ATFUNCDLL_H

#include "..\ATA_DLL\ATA_DLL.h"


#define ENGPRETEST_API __declspec(dllexport)

#ifndef WM_APP
#define WM_APP 0x8000
#endif

#define WM_ADCMSG  WM_APP+1
#define WM_INTEMSG WM_APP+2
#define WM_BATTMSG WM_APP+3


typedef enum
{
    ATCONN_FAIL = -1,
    ATERROR = 0,
    ATSUCCESS = 1
} EResult;


#ifdef __cplusplus
extern "C"
{
#endif

//Com Port related Functions
    ENGPRETEST_API long ATInitCommPort(const char *portname, int flowctrl,int baud, int *stopFlag);
    ENGPRETEST_API long ATInitCommPort_Ex(const char *portname, const S_ATADLL_COMMON_CFG_T *comm_cfg);
//ENGPRETEST_API long ATInitCommPort(const char *portname, int flowctrl);
    ENGPRETEST_API long ATInitCommPort_Ex_Ex(const char *portname, const S_ATADLL_COMMON_CFG_T *comm_cfg);
    ENGPRETEST_API void ATDeInitCommPort(long handle);
    ENGPRETEST_API void ATWriteCommand(long handle, char* atcommand);
    ENGPRETEST_API EResult ATDisableEcho(long handle);

//AT+ESLP turn on/off sleep mode
    ENGPRETEST_API EResult ATDisableSleepMode(long handle, bool isdisable, int *stopFlag);
    ENGPRETEST_API EResult ATDisableSleepMode_Ex (long handle, bool isdisable, const S_ATADLL_COMMON_CFG_T *comm_cfg);
//AT+ELSM turn  on/off LCM sleep mode
    ENGPRETEST_API EResult ATDisableLCMSleep(long handle, bool isdisable);

//Get Version
    ENGPRETEST_API EResult ATCGSN_GETIMEI(long handle, char *imei);
    ENGPRETEST_API EResult ATEGMR_GetSetVersion(long handle, bool isget, int type, char *verstr);
    ENGPRETEST_API EResult ATCGMR_GetSWVersion(long handle, char *verstr);

//AT+CEMS
    ENGPRETEST_API EResult ATCEMS_TurnOnUnsolicitedEmMode(long handle, bool isOn, HWND hwnd);

//AT+EADP
    ENGPRETEST_API EResult ATEADP_SetAudio(long handle, bool isSet, int mode, int type, int level, unsigned char *gain);

//AT+EGPIO
    ENGPRETEST_API EResult ATEGPIO_SetGPIO(long handle, int type, bool isOn);

//AT+EGPO
    ENGPRETEST_API EResult ATEGPO_SetGPO(long handle, int type, bool isOn);

//AT+EADC
    ENGPRETEST_API EResult ATEADC_TurnOnUnsolicitedADC(long handle, bool isOn, HWND hwnd);
    ENGPRETEST_API void ATGetADCValue(float *ADC0, float *ADC1, float *ADC2, float *ADC3, float *ADC4);

//AT+ELCD
    ENGPRETEST_API EResult ATELCD_GetLCDParamNum(long handle, int lcdtype, int function, int *param_num);
    ENGPRETEST_API EResult ATELCD_TestLCD(long handle, int lcdtype, int function, const char *str);
    ENGPRETEST_API EResult ATELCD_SetLCD(long handle, bool isSet, int lcdtype, char *bias_str, char *contrast_str, char *line_str, char *temp_str);

//AT+EPWM
    ENGPRETEST_API EResult ATEPWM_SetPWM(long handle, bool isSet, int num, int level, int *freq, int *duty);
    ENGPRETEST_API EResult ATEPWM_StartPWM(long handle, bool isStart, int type, int freq1, int duty);

//AT+EREG
    ENGPRETEST_API EResult ATEREG_RWPMIC(long handle, bool bwrite, int index, int &value);

//AT+EPMIC
    ENGPRETEST_API EResult ATEREG_EPMICStatus(long handle, int category, int value, int &param1, int &param2, int &param3);

#ifdef __cplusplus
}
#endif

#endif
