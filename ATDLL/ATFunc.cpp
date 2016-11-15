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
*  ATFunc.cpp
*
* Project:
* --------
*   HWTest
*
* Description:
* ------------
*   This file contains the implementation of ATDLL.
*
* Author:
* -------
*  KS (mtk00451)
*
*==============================================================================
*           HISTORY
* Below this line, this part is controlled by PVCS VM. DO NOT MODIFY!!
*------------------------------------------------------------------------------
* $Revision:   1.8  $
* $Modtime:   Dec 07 2005 17:47:20  $
* $Log:   //mtkvs01/VMDATA/HWTest/archives/ATDLL/ATFunc.cpp-arc  $
 *
 * Mar 2 2006 mtk00451
 * [STP100000645] Modify HW Test Tool for customer
 *
 *
 *    Rev 1.8   Dec 08 2005 14:50:58   mtk00451
 * Add PMIC Support
 *
 *    Rev 1.7   Feb 13 2004 09:41:56   mtk00689
 * Add baud rate select feature and fix a few bugs
 *
 *    Rev 1.6   Nov 28 2003 09:20:46   mtk00451
 * fix delete comm twice.
 *
 *    Rev 1.5   Sep 24 2003 14:40:40   mtk00451
 * Add flow control
 *
 *    Rev 1.4   Sep 19 2003 18:14:38   mtk00451
 * Add GPO and sleep mode
 *
 *    Rev 1.3   Sep 12 2003 15:15:26   mtk00451
 * Change timeout: 300->150
 *
 *    Rev 1.2   Sep 12 2003 14:46:40   mtk00451
 * Add ATDisableSleepMode
 *
 *    Rev 1.1   Aug 15 2003 18:14:44   mtk00451
 * Modify timeout from 500 to 1000
 *
 *    Rev 1.0   Aug 14 2003 21:25:52   admin
 * Initial revision.
*
*------------------------------------------------------------------------------
* Upper this line, this part is controlled by PVCS VM. DO NOT MODIFY!!
*==============================================================================
*******************************************************************************/

#include "windows.h"
#include "..\comm\commModule.h"
#include "ATFunc.h"

#define ATTIMEOUT 1700

const unsigned char CR = 13;
const unsigned char LF = 10;
const unsigned char CtrlZ = 26;

//internal use callback
static HWND global_hwnd;
static float global_adc0;
static float global_adc1;
static float global_adc2;
static float global_adc3;
static float global_adc4;

static void callback(ATResult& atret, int handle_index)
{
    if(atret.resultLst.size()==0)
        return;

    if(atret.resultLst[0].eleLst[0].str_value == "+GPIOS")
    {
        if(atret.resultLst[0].eleLst.size()==3)
            PostMessage(global_hwnd, WM_INTEMSG, atret.resultLst[0].eleLst[1].int_value, atret.resultLst[0].eleLst[2].int_value);
    }
    else if(atret.resultLst[0].eleLst[0].str_value == "+BATS")
    {
        if(atret.resultLst[0].eleLst.size()==2)
            PostMessage(global_hwnd, WM_BATTMSG, atret.resultLst[0].eleLst[1].int_value, 0);
    }
    else if(atret.resultLst[0].eleLst[0].str_value == "+EADC")
    {
        if(atret.resultLst[0].eleLst.size()==6)
        {
            global_adc0 = (float) atof(atret.resultLst[0].eleLst[1].str_value.c_str());
            global_adc1 = (float) atof(atret.resultLst[0].eleLst[2].str_value.c_str());
            global_adc2 = (float) atof(atret.resultLst[0].eleLst[3].str_value.c_str());
            global_adc3 = (float) atof(atret.resultLst[0].eleLst[4].str_value.c_str());
            global_adc4 = (float) atof(atret.resultLst[0].eleLst[5].str_value.c_str());

            PostMessage(global_hwnd, WM_ADCMSG, 0, 0);
        }
    }
}

//Com Port related Functions
ENGPRETEST_API long ATInitCommPort(const char *portname, int flowctrl,int baud, int *stopFlag)
{
    CommModule *comm = new CommModule;
    COMM_BAUD_TAG Baudrate ;

    if (baud==0) Baudrate = COMM_RS_57600;
    else Baudrate = COMM_RS_115200;

    COMMTYPE t;
    if(comm->Init(portname, Baudrate, (COMM_FLOW_CTRL) flowctrl, callback, t)!=true)
        //if(comm->Init(portname, COMM_RS_57600, (COMM_FLOW_CTRL) flowctrl,callback)!=true)
    {
        delete comm;
        return 0;
    }

    //turn off sleep
    if(ATDisableSleepMode((long)comm, true, stopFlag) == ATSUCCESS)
    {
        Sleep(300);  //re-start
        comm->DeInit();
        delete comm;
        comm = new CommModule;
        //if(comm->Init(portname, COMM_RS_57600, (COMM_FLOW_CTRL) flowctrl, callback)!=true)
        if(comm->Init(portname, Baudrate, (COMM_FLOW_CTRL) flowctrl, callback, t)!=true)
        {
            delete comm;
            return 0;
        }

        //turn off the echo
        ATResult atret;
        string atstr = "ATE0";
        atstr += CR;
        //write twice for sure
        comm->WriteCommand(atstr.c_str(), atstr.length(), ATTIMEOUT, false, atret);
        if(comm->WriteCommand(atstr.c_str(), atstr.length(), ATTIMEOUT, false, atret))
        {
            if(atret.resultLst.size()>0)
                if(atret.resultLst[0].eleLst[0].str_value == "OK")
                {
                    ATDisableLCMSleep((long)comm, true);
                    return (long)comm;
                }
        }
    }
    else
    {
        ATDeInitCommPort((long)comm);  //can't turn off echo
        return -1;
    }

    ATDeInitCommPort((long)comm);  //can't turn off echo
    //delete comm;
    return 0;
}

//Com Port related Functions
ENGPRETEST_API long ATInitCommPort_Ex(const char *portname, const S_ATADLL_COMMON_CFG_T *comm_cfg) // int flowctrl,int baud, int *stopFlag)
{
    CommModule *comm = new CommModule;
    COMM_BAUD_TAG Baudrate ;

    if (comm_cfg->com_port_info.baud_rate == 0) Baudrate = COMM_RS_57600;
    else Baudrate = COMM_RS_115200;

    COMMTYPE t;
    if(comm->Init(portname, Baudrate, (COMM_FLOW_CTRL) comm_cfg->com_port_info.flow_control, callback, t)!=true)
        //if(comm->Init(portname, COMM_RS_57600, (COMM_FLOW_CTRL) flowctrl,callback)!=true)
    {
        delete comm;
        return 0;
    }
    else
    {
        return (long)comm;
    }

    //turn off sleep
    if(ATDisableSleepMode_Ex((long)comm, true, comm_cfg) == ATSUCCESS)
    {
        Sleep(300);  //re-start
        comm->DeInit();
        delete comm;
        comm = new CommModule;
        //if(comm->Init(portname, COMM_RS_57600, (COMM_FLOW_CTRL) flowctrl, callback)!=true)
        if(comm->Init(portname, Baudrate, (COMM_FLOW_CTRL) comm_cfg->com_port_info.flow_control, callback, t)!=true)
        {
            delete comm;
            return 0;
        }

        //turn off the echo
        ATResult atret;
        string atstr = "ATE0";
        atstr += CR;
        //write twice for sure
        comm->WriteCommand(atstr.c_str(), atstr.length(), ATTIMEOUT, false, atret);
        if(comm->WriteCommand(atstr.c_str(), atstr.length(), ATTIMEOUT, false, atret))
        {
            if(atret.resultLst.size()>0)
                if(atret.resultLst[0].eleLst[0].str_value == "OK")
                {
                    ATDisableLCMSleep((long)comm, true);
                    return (long)comm;
                }
        }
    }
    else
    {
        ATDeInitCommPort((long)comm);  //can't turn off echo
        return -1;
    }

    ATDeInitCommPort((long)comm);  //can't turn off echo
    //delete comm;
    return 0;
}


ENGPRETEST_API long ATInitCommPort_Ex_Ex(const char *portname, const S_ATADLL_COMMON_CFG_T *comm_cfg) // int flowctrl,int baud, int *stopFlag)
{
    CommModule *comm = new CommModule;
    COMM_BAUD_TAG Baudrate ;

    Baudrate = COMM_RS_115200;

    COMMTYPE t;
    if(comm->Init(portname, Baudrate, COMM_FC_NONE, callback, t)!=true)
    {
        delete comm;
    }
    else
    {
        return (long)comm;
    }

    return 0;
}


ENGPRETEST_API void ATDeInitCommPort(long handle)
{
    if(handle==0)
        return;
    CommModule *comm = (CommModule *)handle;

    //ATResult atret;
    //string atstr = "ATE1";  //reset echo
    //atstr += CR;
    //comm->WriteCommand(atstr.c_str(), atstr.length(), ATTIMEOUT, false, atret);
    //enable sleep
    //ATDisableLCMSleep((long)comm, false);
    //ATDisableSleepMode((long)comm, false);
    comm->DeInit();
    delete comm;
}

//Write AT Command
ENGPRETEST_API void ATWriteCommand(long handle, char* atcommand)
{
    if(handle==0)
        return;
    CommModule *comm = (CommModule *)handle;

    ATResult atret;
    string atstr = atcommand;
    atstr += CR;
    comm->WriteCommand(atstr.c_str(), atstr.length(), ATTIMEOUT, false, atret);
}

//ATE0
ENGPRETEST_API EResult ATDisableEcho(long handle)
{
    if(handle==0)
        return ATERROR;
    CommModule *comm = (CommModule *)handle;
    ATResult atret;
    string atstr = "ATE0";
    atstr += CR;
    if(comm->WriteCommand(atstr.c_str(), atstr.length(), ATTIMEOUT, false, atret))
    {
        if(atret.resultLst.size()>0)
            if(atret.resultLst[0].eleLst[0].str_value == "OK")
                return ATSUCCESS;
    }
    return ATERROR;
}

//AT+ESLP
ENGPRETEST_API EResult ATDisableSleepMode(long handle, bool isdisable, int *stopFlag)
{
    if(handle==0)
        return ATERROR;
    CommModule *comm = (CommModule *)handle;
    ATResult atret;
    string atstr;

    atstr = "AT";
    atstr += CR;
    int countNum = 0;
    do
    {
        if(comm->WriteCommand(atstr.c_str(), atstr.length(),40, false, atret))
        {
            const char *temp = atret.resultLst[0].eleLst[0].str_value.c_str();
            int	size = atret.resultLst.size();
            if (atret.result == AT_SUCCESS)
            {
                if(atret.resultLst.size()>0)
                {
                    if(atret.resultLst[0].eleLst[0].str_value == "OK")
                        break;
                }
            }
        }

        countNum ++;
        if (*stopFlag == 0xFF || countNum > 180)
        {
            return ATERROR;
        }

        Sleep (500);
    }
    while (1);

    if(isdisable)
        atstr = "AT+ESLP=0";
    else
        atstr = "AT+ESLP=1";
    atstr += CR;

    if(isdisable)
    {
        int i=0;  //try until success
        while(i<100)
        {
            i++;
            DWORD tickc = GetTickCount();
            if(comm->WriteCommand(atstr.c_str(), atstr.length(),40, false, atret))
            {
                if(atret.resultLst.size()>0)
                    if(atret.resultLst[0].eleLst[0].str_value == "OK")
                        return ATSUCCESS;
            }
            if(GetTickCount()-tickc > 500)  //no target connected
                break;
        }
    }
    else
    {
        if(comm->WriteCommand(atstr.c_str(), atstr.length(), ATTIMEOUT, false, atret))
        {
            if(atret.resultLst.size()>0)
                if(atret.resultLst[0].eleLst[0].str_value == "OK")
                    return ATSUCCESS;
        }
    }
    return ATERROR;
}

ENGPRETEST_API EResult ATDisableSleepMode_Ex (long handle, bool isdisable, const S_ATADLL_COMMON_CFG_T *comm_cfg)
{
    if(handle==0)
        return ATERROR;
    CommModule *comm = (CommModule *)handle;
    ATResult atret;
    string atstr;

    atstr = "AT";
    atstr += CR;
    int countNum = 0;
    int	okNum = 0;

    double last_tick = 0;
    double current_tick = 0;
    do
    {
        if(comm->WriteCommand(atstr.c_str(), atstr.length(),40, false, atret))
        {
            const char *temp = atret.resultLst[0].eleLst[0].str_value.c_str();
            int	size = atret.resultLst.size();
            if (atret.result == AT_SUCCESS)
            {
                if(atret.resultLst.size()>0)
                {
                    if(atret.resultLst[0].eleLst[0].str_value == "OK")
                    {
                        okNum ++;

                        // Only need polling from the second time
                        if (!comm_cfg->auto_polling || !comm_cfg->b_need_polling)
                        {
                            break;
                        }

                        if (okNum <= 1)
                        {
                            last_tick = current_tick = GetTickCount();
                        }
                        else
                        {
                            current_tick = GetTickCount ();
                            if ((current_tick - last_tick)/1000 > 3.0)
                            {
                                break;
                            }
                            last_tick = current_tick;
                        }
                    }
                }
            }
        }

        countNum ++;
        if (*(comm_cfg->stop_flag) == 0xFF || countNum > 180)
        {
            return ATERROR;
        }

        Sleep (500);
    }
    while (1);

    if(isdisable)
        atstr = "AT+ESLP=0";
    else
        atstr = "AT+ESLP=1";
    atstr += CR;

    if(isdisable)
    {
        int i=0;  //try until success
        while(i<100)
        {
            i++;
            DWORD tickc = GetTickCount();
            if(comm->WriteCommand(atstr.c_str(), atstr.length(),40, false, atret))
            {
                if(atret.resultLst.size()>0)
                    if(atret.resultLst[0].eleLst[0].str_value == "OK")
                        return ATSUCCESS;
            }
            if(GetTickCount()-tickc > 500)  //no target connected
                break;
        }
    }
    else
    {
        if(comm->WriteCommand(atstr.c_str(), atstr.length(), ATTIMEOUT, false, atret))
        {
            if(atret.resultLst.size()>0)
                if(atret.resultLst[0].eleLst[0].str_value == "OK")
                    return ATSUCCESS;
        }
    }
    return ATERROR;
}


//AT+ELSM
ENGPRETEST_API EResult ATDisableLCMSleep(long handle, bool isdisable)
{
    if(handle==0)
        return ATERROR;
    CommModule *comm = (CommModule *)handle;
    ATResult atret;
    string atstr;
    if(isdisable)
        atstr = "AT+ELSM=0";
    else
        atstr = "AT+ELSM=1";
    atstr += CR;
    if(comm->WriteCommand(atstr.c_str(), atstr.length(), ATTIMEOUT, false, atret))
    {
        if(atret.resultLst.size()>0)
            if(atret.resultLst[0].eleLst[0].str_value == "OK")
                return ATSUCCESS;
    }
    return ATERROR;
}

//AT+CGSN_GET
ENGPRETEST_API EResult ATCGSN_GETIMEI(long handle, char *imei)
{
    if(handle==0)
        return ATERROR;
    CommModule *comm = (CommModule *)handle;
    ATResult atret;
    string atstr="AT+CGSN";
    atstr+=CR;
    atret.expectedCommand="+CGSN";
    if(comm->WriteCommand(atstr.c_str(), atstr.length(), ATTIMEOUT, false, atret))
    {
        if(atret.resultLst.size()>1)
            if(atret.resultLst[0].eleLst[0].str_value == "+CGSN")
            {
                strcpy(imei, atret.resultLst[0].eleLst[1].str_value.c_str());
                return ATSUCCESS;
            }
    }
    return ATERROR;
}


//AT+EGMR
ENGPRETEST_API EResult ATEGMR_GetSetVersion(long handle, bool isget, int type, char *verstr)
{
    if(handle==0)
        return ATERROR;
    CommModule *comm = (CommModule *)handle;
    ATResult atret;
    char tempbuf[512];
    if(isget)
    {
        sprintf(tempbuf, "AT+EGMR=0,%d",type);
        atret.expectedCommand="+EGMR";
    }
    else
        sprintf(tempbuf, "AT+EGMR=1, %d, %s",type, verstr);
    string atstr=tempbuf;
    atstr+=CR;
    if(comm->WriteCommand(atstr.c_str(), atstr.length(), ATTIMEOUT, false, atret))
    {
        if(atret.resultLst.size()==1)   //set
        {
            if(atret.resultLst[0].eleLst[0].str_value == "OK")
                return ATSUCCESS;
        }
        else if(atret.resultLst.size()==2)  //get
        {
            if(atret.resultLst[0].eleLst[0].str_value == "+EGMR")
            {
                strcpy(verstr, atret.resultLst[0].eleLst[1].str_value.c_str());
                return ATSUCCESS;
            }
        }
    }
    return ATERROR;
}


//AT+CGMR
ENGPRETEST_API EResult ATCGMR_GetSWVersion(long handle, char *verstr)
{
    if(handle==0)
        return ATERROR;
    CommModule *comm = (CommModule *)handle;
    ATResult atret;
    string atstr="AT+CGMR";
    atstr+=CR;
    atret.expectedCommand="+CGMR";
    if(comm->WriteCommand(atstr.c_str(), atstr.length(), ATTIMEOUT, false, atret))
    {
        if(atret.resultLst.size()>1)
            if(atret.resultLst[0].eleLst[0].str_value == "+CGMR")
            {
                strcpy(verstr, atret.resultLst[0].eleLst[1].str_value.c_str());
                return ATSUCCESS;
            }
    }
    return ATERROR;
}


//AT+CEMS
ENGPRETEST_API EResult ATCEMS_TurnOnUnsolicitedEmMode(long handle, bool isOn, HWND hwnd)
{
    if(handle==0)
        return ATERROR;
    CommModule *comm = (CommModule *)handle;
    ATResult atret;
    string atstr;
    if(isOn)
    {
        atstr = "AT+CEMS=1";
        global_hwnd = hwnd;
    }
    else
        atstr = "AT+CEMS=0";
    atstr += CR;
    if(comm->WriteCommand(atstr.c_str(), atstr.length(), ATTIMEOUT, false, atret))
    {
        if(atret.resultLst.size()>0)
            if(atret.resultLst[0].eleLst[0].str_value == "OK")
                return ATSUCCESS;
    }
    return ATERROR;
}

//AT+EADP
ENGPRETEST_API EResult ATEADP_SetAudio(long handle, bool isSet, int mode, int type, int level, unsigned char *gain)
{
    if(handle==0 || gain==NULL)
        return ATERROR;
    CommModule *comm = (CommModule *)handle;
    ATResult atret;
    char tempbuf[512];
    if(isSet)
        sprintf(tempbuf, "AT+EADP=1,%d,%d,%d,%d", mode, type, level, *gain);
    else
    {
        sprintf(tempbuf, "AT+EADP=0,%d,%d,%d", mode, type, level);
        atret.expectedCommand="+EADP";
    }
    string atstr = tempbuf;
    atstr+=CR;
    if(comm->WriteCommand(atstr.c_str(), atstr.length(), ATTIMEOUT, false, atret))
    {
        if(atret.resultLst.size()==1)
        {
            if(atret.resultLst[0].eleLst[0].str_value == "OK")
                return ATSUCCESS;
        }
        else if(atret.resultLst.size()==2)  //return value
        {
            if(atret.resultLst[0].eleLst[0].str_value == "+EADP")
            {
                *gain = atret.resultLst[0].eleLst[1].int_value;
                return ATSUCCESS;
            }
        }
    }
    return ATERROR;
}

//AT+EGPIO
ENGPRETEST_API EResult ATEGPIO_SetGPIO(long handle, int type, bool isOn)
{
    if(handle==0)
        return ATERROR;
    CommModule *comm = (CommModule *)handle;
    ATResult atret;
    char tempbuf[512];
    string atstr;
    if(isOn)
        sprintf(tempbuf, "AT+EGPIO=%d,1", type);
    else
        sprintf(tempbuf, "AT+EGPIO=%d,0", type);
    atstr = tempbuf;
    atstr += CR;
    if(comm->WriteCommand(atstr.c_str(), atstr.length(), ATTIMEOUT, false, atret))
    {
        if(atret.resultLst.size()>0)
            if(atret.resultLst[0].eleLst[0].str_value == "OK")
                return ATSUCCESS;
    }
    return ATERROR;
}

//AT+EGPO
ENGPRETEST_API EResult ATEGPO_SetGPO(long handle, int type, bool isOn)
{
    if(handle==0)
        return ATERROR;
    CommModule *comm = (CommModule *)handle;
    ATResult atret;
    char tempbuf[512];
    string atstr;
    if(isOn)
        sprintf(tempbuf, "AT+EGPO=%d,1", type);
    else
        sprintf(tempbuf, "AT+EGPO=%d,0", type);
    atstr = tempbuf;
    atstr += CR;
    if(comm->WriteCommand(atstr.c_str(), atstr.length(), ATTIMEOUT, false, atret))
    {
        if(atret.resultLst.size()>0)
            if(atret.resultLst[0].eleLst[0].str_value == "OK")
                return ATSUCCESS;
    }
    return ATERROR;
}

//AT+EADC
ENGPRETEST_API EResult ATEADC_TurnOnUnsolicitedADC(long handle, bool isOn, HWND hwnd)
{
    if(handle==0)
        return ATERROR;
    CommModule *comm = (CommModule *)handle;
    ATResult atret;
    string atstr;
    if(isOn)
    {
        atstr = "AT+EADC=1";
        global_hwnd = hwnd;
    }
    else
        atstr = "AT+EADC=0";
    atstr += CR;
    if(comm->WriteCommand(atstr.c_str(), atstr.length(), ATTIMEOUT, false, atret))
    {
        if(atret.resultLst.size()>0)
            if(atret.resultLst[0].eleLst[0].str_value == "OK")
                return ATSUCCESS;
    }
    return ATERROR;
}

ENGPRETEST_API void ATGetADCValue(float *ADC0, float *ADC1, float *ADC2, float *ADC3, float *ADC4)
{
    if(ADC0==NULL || ADC1==NULL || ADC2==NULL || ADC3==NULL || ADC4==NULL)
        return;
    *ADC0=global_adc0;
    *ADC1=global_adc1;
    *ADC2=global_adc2;
    *ADC3=global_adc3;
    *ADC4=global_adc4;
}

//AT+ELCD
ENGPRETEST_API EResult ATELCD_GetLCDParamNum(long handle, int lcdtype, int function, int *param_num)
{
    if(handle==0 || param_num == NULL)
        return ATERROR;
    CommModule *comm = (CommModule *)handle;
    ATResult atret;
    char tempbuf[512];
    string atstr;
    sprintf(tempbuf, "AT+ELCD=0,%d,%d", lcdtype, function);
    atstr = tempbuf;
    atstr += CR;
    atret.expectedCommand="+ELCD";

    if(comm->WriteCommand(atstr.c_str(), atstr.length(), ATTIMEOUT, false, atret))
    {
        if(atret.resultLst.size()==2)
            if(atret.resultLst[0].eleLst[0].str_value == "+ELCD")
            {
                *param_num = atret.resultLst[0].eleLst[1].int_value;
                return ATSUCCESS;
            }
    }
    return ATERROR;
}

ENGPRETEST_API EResult ATELCD_TestLCD(long handle, int lcdtype, int function, const char *str)
{
    if(handle==0 || str == NULL)
        return ATERROR;
    CommModule *comm = (CommModule *)handle;
    ATResult atret;
    char tempbuf[512];
    string atstr;
    sprintf(tempbuf, "AT+ELCD=1,%d,%d,\"%s\"", lcdtype, function, str);
    atstr = tempbuf;
    atstr += CR;
    if(comm->WriteCommand(atstr.c_str(), atstr.length(), ATTIMEOUT, false, atret))
    {
        if(atret.resultLst.size()>0)
            if(atret.resultLst[0].eleLst[0].str_value == "OK")
                return ATSUCCESS;
    }
    return ATERROR;
}

ENGPRETEST_API EResult ATELCD_SetLCD(long handle, bool isSet, int lcdtype, char *bias_str, char *contrast_str, char *line_str, char *temp_str)
{
    if(handle==0 || bias_str==NULL || contrast_str==NULL || line_str==NULL || temp_str==NULL)
        return ATERROR;
    CommModule *comm = (CommModule *)handle;
    ATResult atret;
    char tempbuf[1024];
    string atstr;

    if(isSet)   //set all
        sprintf(tempbuf, "AT+ELCD=3,%d,\"%s\",\"%s\",\"%s\",\"%s\"", lcdtype,bias_str,contrast_str,line_str,temp_str);
    else      //get all
    {
        sprintf(tempbuf, "AT+ELCD=2,%d", lcdtype);
        atret.expectedCommand="+ELCD";
    }

    atstr = tempbuf;
    atstr += CR;
    if(comm->WriteCommand(atstr.c_str(), atstr.length(), ATTIMEOUT, false, atret))
    {
        if(atret.resultLst.size()==1)
        {
            if(atret.resultLst[0].eleLst[0].str_value == "OK")  //save all
                return ATSUCCESS;
        }
        else if(atret.resultLst.size()==2)
        {
            if(atret.resultLst[0].eleLst.size()==5)
            {
                strcpy(bias_str,     atret.resultLst[0].eleLst[1].str_value.c_str());
                strcpy(contrast_str, atret.resultLst[0].eleLst[2].str_value.c_str());
                strcpy(line_str,     atret.resultLst[0].eleLst[3].str_value.c_str());
                strcpy(temp_str,     atret.resultLst[0].eleLst[4].str_value.c_str());
                return ATSUCCESS;
            }
        }
    }
    return ATERROR;
}

//AT+EPWM
ENGPRETEST_API EResult ATEPWM_SetPWM(long handle, bool isSet, int num, int level, int *freq, int *duty)
{
    if(handle==0 || freq==NULL || duty==NULL)
        return ATERROR;
    CommModule *comm = (CommModule *)handle;
    ATResult atret;
    char tempbuf[512];
    string atstr;
    if(isSet)   //set
        sprintf(tempbuf, "AT+EPWM=1,%d,%d,%d,%d", num, level, *freq, *duty);
    else // get
    {
        sprintf(tempbuf, "AT+EPWM=0,%d,%d", num, level);
        atret.expectedCommand="+EPWM";
    }
    atstr = tempbuf;
    atstr += CR;
    if(comm->WriteCommand(atstr.c_str(), atstr.length(), ATTIMEOUT, false, atret))
    {
        if(atret.resultLst.size()==1)
        {
            if(atret.resultLst[0].eleLst[0].str_value == "OK")  //set all
                return ATSUCCESS;
        }
        else if(atret.resultLst.size()==2)
        {
            if(atret.resultLst[0].eleLst.size()==3)
            {
                *freq=atret.resultLst[0].eleLst[1].int_value;
                *duty=atret.resultLst[0].eleLst[2].int_value;

                return ATSUCCESS;
            }
        }
    }
    return ATERROR;
}

ENGPRETEST_API EResult ATEPWM_StartPWM(long handle, bool isStart, int type, int freq, int duty)
{
    if(handle==0)
        return ATERROR;
    CommModule *comm = (CommModule *)handle;
    ATResult atret;
    char tempbuf[512];
    string atstr;
    if(isStart)   //start
        sprintf(tempbuf, "AT+EPWM=2,%d,%d,%d",type, freq, duty);
    else // stop
        sprintf(tempbuf, "AT+EPWM=3,%d,%d,%d",type, freq, duty);
    atstr = tempbuf;
    atstr += CR;
    if(comm->WriteCommand(atstr.c_str(), atstr.length(), ATTIMEOUT, false, atret))
    {
        if(atret.resultLst.size()>0)
        {
            if(atret.resultLst[0].eleLst[0].str_value == "OK")  //set all
                return ATSUCCESS;
        }
    }
    return ATERROR;
}


ENGPRETEST_API EResult ATEREG_RWPMIC(long handle, bool bwrite, int index, int &value)
{
    if(handle==0)
        return ATERROR;
    CommModule *comm = (CommModule *)handle;
    ATResult atret;
    char tempbuf[512];
    string atstr;
    if(bwrite)   //set
        sprintf(tempbuf, "AT+EREG=\"0x%x\",%d", index, value);
    else // get
    {
        sprintf(tempbuf, "AT+EREG=\"0x%x\"", index);
        atret.expectedCommand="+EREG";
    }
    atstr = tempbuf;
    atstr += CR;
    if(comm->WriteCommand(atstr.c_str(), atstr.length(), ATTIMEOUT, false, atret))
    {
        if(atret.resultLst.size()==1)
        {
            if(atret.resultLst[0].eleLst[0].str_value == "OK")  //write
                return ATSUCCESS;
        }
        else if(atret.resultLst.size()==2)
        {
            if(atret.resultLst[0].eleLst.size()==3)  //read
            {
                value=atret.resultLst[0].eleLst[2].int_value;
                return ATSUCCESS;
            }
        }
    }
    return ATERROR;
}


ENGPRETEST_API EResult ATEREG_EPMICStatus(long handle, int category, int value, int &param1, int &param2, int &param3)
{
    if(handle==0)
        return ATERROR;
    CommModule *comm = (CommModule *)handle;
    ATResult atret;
    char tempbuf[512];
    string atstr;
    if(value < 4)
        sprintf(tempbuf, "AT+EPMIC=%d,%d", category, value);
    else
        sprintf(tempbuf, "AT+EPMIC=%d", category);

    atret.expectedCommand="+EPMIC";

    atstr = tempbuf;
    atstr += CR;
    if(comm->WriteCommand(atstr.c_str(), atstr.length(), ATTIMEOUT, false, atret))
    {
        if(atret.resultLst.size()==2)
        {
            if(atret.resultLst[0].eleLst.size()==3)
            {
                param1=atret.resultLst[0].eleLst[2].int_value;
                return ATSUCCESS;
            }
            else if(atret.resultLst[0].eleLst.size()==4)
            {
                param1=atret.resultLst[0].eleLst[2].int_value;
                param2=atret.resultLst[0].eleLst[3].int_value;
                return ATSUCCESS;
            }
            else if(atret.resultLst[0].eleLst.size()==5)
            {
                param1=atret.resultLst[0].eleLst[2].int_value;
                param2=atret.resultLst[0].eleLst[3].int_value;
                param3=atret.resultLst[0].eleLst[4].int_value;
                return ATSUCCESS;
            }
        }
    }
    return ATERROR;
}
