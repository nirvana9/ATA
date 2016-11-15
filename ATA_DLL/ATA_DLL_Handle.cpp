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
 *   ATA_DLL_Handle.cpp
 *
 * Project:
 * --------
 *   ATA_DLL
 *
 * Description:
 * ------------
*   This module contains the ATA test flow handle
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
#include "ATA_DLL_Handle.h"
#include "ATA_DLL_Test_Handle.h"
#include "GlobalOBJ.h"
#include "USB_Com_Func.h"
#include "spmeta.h"
#include "sp_brom.h"
#include "auth.h"
#include "SCERT.h"
#include "SLA_Challenge.h"
#include "..\ATDLL\ATFunc.h"
#include "ini.h"

#include "TestItem\header\ATA_DLL_TestItem_SWVersion.h"
#include "TestItem\header\ATA_DLL_TestItem_Barcode.h"
#include "TestItem\header\ATA_DLL_TestItem_SIMCard.h"
#include "TestItem\header\ATA_DLL_TestItem_TCard.h"
#include "TestItem\header\ATA_DLL_TestItem_GPS.h"
#include "TestItem\header\ATA_DLL_TestItem_WIFI.h"
#include "TestItem\header\ATA_DLL_TestItem_BT.h"
#include "TestItem\header\ATA_DLL_TestItem_FM.h"
#include "TestItem\header\ATA_DLL_TestItem_Charger.h"
#include "TestItem\header\ATA_DLL_TestItem_Vibrator.h"
#include "TestItem\header\ATA_DLL_TestItem_MATV.h"
#include "TestItem\header\ATA_DLL_TestItem_Audio.h"
#include "TestItem\header\ATA_DLL_TestItem_Camera.h"
#include "TestItem\header\ATA_DLL_TestItem_LCM.h"
#include "TestItem\header\ATA_DLL_TestItem_TP.h"
#include "TestItem\header\ATA_DLL_TestItem_HWVer.h"
#include "TestItem\header\ATA_DLL_TestItem_RTC.h"
#include "TestItem\header\ATA_DLL_TestItem_ER_State.h"
#include "TestItem\header\ATA_DLL_TestItem_KPad.h"
#include "TestItem\header\ATA_DLL_TestItem_ADC.h"
#include "TestItem\header\ATA_DLL_TestItem_SleepMode.h"
#include "TestItem\header\ATA_DLL_TestItem_RSSI.h"
#include "TestItem\header\ATA_DLL_TestItem_LED.h"
#include "TestItem\header\ATA_DLL_TestItem_EMMC.h"
#include "TestItem\header\ATA_DLL_TestItem_CMMB.h"
#include "TestItem\header\ATA_DLL_TestItem_Sensor.h"
#include "TestItem\header\ATA_DLL_TestItem_Micbias.h"
#include "TestItem\header\ATA_DLL_TestItem_OTG.h"
#include "TestItem\header\ATA_DLL_TestItem_HDMI.h"
#include "TestItem\header\ATA_DLL_TestItem_USBTypeC.h"
#include "TestItem\header\ATA_DLL_TestItem_Barometer.h"
#include "TestItem\header\ATA_DLL_TestItem_BTS.h"
#include "TestItem\header\ATA_DLL_TestItem_ExtBuck.h"
#include "TestItem\header\ATA_DLL_TestItem_FingerPrint.h"
#include "TestItem\header\ATA_DLL_TestItem_Humidity.h"


#define __CONCURRENT_TEST_SUPPORT__

bool g_ReadTargetFlag=false;  //global variable: issue if get flag sequence from target feature is ok in target sw
int g_flag_num=0;  //global variable: total flags number get from target

char test_item_name[E_TEST_ITEM_COUNT+1][64] =
{
    "LCM",
    "Receiver",
    "Speaker",
    "Headset",
    "Mic Bias",
    "Signal Test",
    
    "Main Camera",
    "Sub Camera",
    "Touch Panel",
	
    "Vibrator",
    "Keypad", //10
    
    "CMMB",
    "RTC",
    "ADC",
    "LED",

    "Ring",
    "Earphone State",

    "WIFI",
    "GPS",
    "BT",
    "TV", //20

    "TCard",
    "EMMC",
    "Flash",
    "SIM Card",
    "Barcode",
    "SW Version",
    "LCDBacklight",

    "FM",
    "Charger",

    "G-Sensor",
    "M-Sensor",
    "ALSPS",
    "Gyroscope",

	"OTG",
	"HDMI",
	"MHL",
	"USB Type-C",

	"Idle Current",
	"Off Current",
	"Main2 Camera",
	"FingerPrint",
    "Humidity",
    "BTS",
    "EXT_BUCK",
    "Barometer",

	"GPS_CALI",		//add by songjian 2016-3-1
	
    "End"
};


ATA_DLL_Handle_Array		g_ADHA;
char    *logPath = "log";

typedef enum
{
    ITEM_MAIN_CAMERA = 0,
    ITEM_MAIN2_CAMERA,
    ITEM_SUB_CAMERA,
    ITEM_STROBE,
    ITEM_GPS,
    ITEM_NFC,
    ITEM_FM,
    ITEM_FMTX,
    ITEM_FLASH,
    ITEM_MEMCARD,
    ITEM_RTC,  //10
    ITEM_LCD,
    ITEM_LCM,
    ITEM_BACKLIGHT,
    ITEM_LED,
    ITEM_LOOPBACK,
    ITEM_LOOPBACK1,
    ITEM_LOOPBACK2,
    ITEM_LOOPBACK3,
    ITEM_BT,
    ITEM_WIFI, //20
    ITEM_KEYS,
    ITEM_LOOPBACK_PHONEMICSPK,
    ITEM_WAVEPLAYBACK,
    ITEM_ACOUSTICLOOPBACK,
    ITEM_GSENSOR,
    ITEM_GS_CALI,
    ITEM_MSENSOR,
    ITEM_ALSPS,
    ITEM_HEADSET,
    ITEM_HEADSET_DEBUG, // 30
    ITEM_USB,
    ITEM_OTG,
    ITEM_CLRFLASH,
    ITEM_CHARGER,
    ITEM_TOUCH,
    ITEM_TOUCH_AUTO,
    ITEM_SIM,
    ITEM_VIBRATOR,
    ITEM_RECEIVER,
    ITEM_RECEIVER_DEBUG, //40
    ITEM_SIMCARD,
    ITEM_IDLE,
    ITEM_TVOUT,
    ITEM_JOGBALL,
    ITEM_OFN,
    ITEM_MATV_NORMAL,
    ITEM_MATV_AUTOSCAN,
    ITEM_MUI_TEST,
    ITEM_FULL_TEST,
    ITEM_ITEM_TEST,  //50
    ITEM_AUTO_TEST,
    ITEM_DEBUG_TEST,
    ITEM_VERSION,
    ITEM_REPORT,
    ITEM_UPDATE,
    ITEM_REBOOT,
    ITEM_BAROMETER,
    ITEM_GYROSCOPE,
    ITEM_GYROSCOPE_CALI,
    ITEM_SPK_OC,    //60
    ITEM_SIGNALTEST,
    ITEM_CMMB,
    ITEM_EMMC,
    ITEM_EMI,
    ITEM_CLREMMC,
    ITEM_HDMI,
    ITEM_RECEIVER_PHONE,
    ITEM_HEADSET_PHONE,
    ITEM_LOOPBACK_PHONEMICSPK_PHONE,
    ITEM_VIBRATOR_PHONE,     //70
    ITEM_MICBIAS,

    ITEM_CUSTOM_START,
    ITEM_CUSTOM_STOP,
    ITEM_CUSTOM_REQUESTDATA,
    ITEM_CUSTOM_VERSION,
    ITEM_CUSTOM_READBARCODE,
    ITEM_CUSTOM_WRITEBARCODE,
    ITEM_CUSTOM_CAMERADATA,

	ITEM_CUSTOM_PROPERTY,
    ITEM_RF_TEST,          //80
    ITEM_HOTKNOT,
    ITEM_BTS,
    ITEM_EXT_BUCK,
    ITEM_EXT_VBAT_BOOST,
    ITEM_SPEAKER_MONITOR,
    ITEM_SPEAKER_MONITOR_SET_TMP,
    ITEM_RF_C2K_TEST,
    ITEM_USB_TYPEC,
    ITEM_IRTX_LED_TEST,
    ITEM_HEART_MONITOR,    //90
    ITEM_EFUSE,
    ITEM_MAIN_LENS,
    ITEM_MAIN2_LENS,
    ITEM_SUB_LENS,
    ITEM_FINGERPRINT_TEST,
    ITEM_HUMIDITY,
    ITEM_GPS_CALI,
    ITEM_MAX_IDS
} ITEM_SEQUENCE;

int g_callback_flag[ITEM_MAX_IDS] =
{
    E_CAMERA,
    E_CAMERA_MAIN2,//E_CAMERA,
    E_CAMERA_SUB,
    ITEM_STROBE,
    E_GPS,
    ITEM_NFC,
    E_FM,
    E_FM,
    E_FLASH,
    E_TCARD,
    E_RTC,  //10
    E_LCD,
    E_LCD,
    ITEM_BACKLIGHT,
    E_LED,
    E_SPEAKER,
    E_SPEAKER,
    E_SPEAKER,
    E_SPEAKER,
    E_BT,
    E_WIFI, // 20
    E_KEYPAD,
    E_SPEAKER,
    E_SPEAKER,
    E_SPEAKER,
    E_GSENSOR,
    E_GSENSOR,
    E_MSENSOR,
    E_ALSPS,
    E_EARTHPHONE,
    E_EARTHPHONE, // 30
    ITEM_USB,
    E_OTG,
    ITEM_CLRFLASH,
    E_CHARGING,
    E_TOUCHSCR,
    E_TOUCHSCR,
    E_SIM,
    E_VIBRATOR,
    E_RECEIVER,
    E_RECEIVER, // 40
    E_SIM,
    E_SLPMODE,
    E_TV,
    ITEM_JOGBALL,
    ITEM_OFN,
    E_TV,
    E_TV,
    ITEM_MUI_TEST,
    ITEM_FULL_TEST,
    ITEM_ITEM_TEST, // 50
    ITEM_AUTO_TEST,
    ITEM_DEBUG_TEST,
    E_SWVERSION,
    ITEM_REPORT,
    ITEM_UPDATE,
    ITEM_REBOOT,
    E_BAROMETER,
    E_GYROSCOPE,
    E_GYROSCOPE,
    E_SPEAKER, // 60
    E_RSSI,
    E_CMMB,
    E_EMMC,
    ITEM_EMI,
    ITEM_CLREMMC,
    E_HDMI,
    ITEM_RECEIVER_PHONE,
    ITEM_HEADSET_PHONE,
    ITEM_LOOPBACK_PHONEMICSPK_PHONE,
    ITEM_VIBRATOR_PHONE, //70
    E_MICBIAS,

    ITEM_CUSTOM_START,
    ITEM_CUSTOM_STOP,
    ITEM_CUSTOM_REQUESTDATA,
    E_SWVERSION,
    E_BARCODE,
    E_BARCODE,
    ITEM_CUSTOM_CAMERADATA,

	ITEM_CUSTOM_PROPERTY,
    E_RSSI,	// 80
    ITEM_HOTKNOT,
    E_BTS,
    E_EXT_BUCK,
    ITEM_EXT_VBAT_BOOST,
    ITEM_SPEAKER_MONITOR,
    ITEM_SPEAKER_MONITOR_SET_TMP,
    ITEM_RF_C2K_TEST,
    E_USBTYPEC,
    ITEM_IRTX_LED_TEST,
    ITEM_HEART_MONITOR,  //90
    ITEM_EFUSE,
    ITEM_MAIN_LENS,
    ITEM_MAIN2_LENS,
    ITEM_SUB_LENS,
    E_FINGERPRINT,
    E_HUMIDITY,
    E_GPS_CALI,
};

char test_item_flow_name[ITEM_MAX_IDS+1][64] =
{
	"Main Camera",
    "Main2 Camera",
    "Sub Camera",
    "Strobe",	//ITEM_STROBE,
    "GPS",	//E_GPS,
    "NFC",	//ITEM_NFC,
    "FM",	//E_FM,
    "FM",	//E_FM,
    "Flash",		//E_FLASH,
    "TCARD",	//E_TCARD,
    "RTC",	//E_RTC,  //10
    "LCD",	//E_LCD,
    "LCD",	//E_LCD,
    "BackLight",	//ITEM_BACKLIGHT,
    "LED",	//E_LED,
    "Speaker",	//E_SPEAKER,
    "Speaker",	//E_SPEAKER,
    "Speaker",	//E_SPEAKER,
    "Speaker",	//E_SPEAKER,
    "BT",	//E_BT,
    "WIFI",	//E_WIFI, // 20
    "KeyPad",	//E_KEYPAD,
    "Speaker",	//E_SPEAKER,
    "Speaker",	//E_SPEAKER,
    "Speaker",	//E_SPEAKER,
    "Gsensor",	//E_GSENSOR,
    "Gsensor",	//E_GSENSOR,
    "Msensor",	//E_MSENSOR,
    "ALSPS",	//E_ALSPS,
    "Earphone",	//E_EARTHPHONE,
    "Earphone",	//E_EARTHPHONE, // 30
    "USB",	//ITEM_USB,
    "OTG",	//E_OTG,
    "Clear flash",	//ITEM_CLRFLASH,
    "Charger",	//E_CHARGING,
    "TouchPanel",	//E_TOUCHSCR,
    "TouchPanel",	// E_TOUCHSCR,
    "SIM",	//E_SIM,
    "Vibrator",	//E_VIBRATOR,
    "Receiver",	//E_RECEIVER,
    "Receiver",	//E_RECEIVER, // 40
    "SIM",	//E_SIM,
    "SleepMode",	//E_SLPMODE,
    "TV",	//E_TV,
    "Jogball", //ITEM_JOGBALL,
    "OFN",	//ITEM_OFN,
    "TV",	//E_TV,
    "TV", 	//E_TV,
    "MUI_TEST",	//ITEM_MUI_TEST,
    "FULL_TEST",	//ITEM_FULL_TEST,
    "ITEM_TEST",	//ITEM_ITEM_TEST, // 50
    "AUTO_TEST",	//ITEM_AUTO_TEST,
    "DEBUG_TEST",	//ITEM_DEBUG_TEST,
    "SW Version",	//E_SWVERSION,
    "Report",	//ITEM_REPORT,
    "Update",	//ITEM_UPDATE,
    "Reboot",	//ITEM_REBOOT,
    "Barometer",	//E_BAROMETER,
    "Gyroscope",	//E_GYROSCOPE,
    "Gyroscope",	//E_GYROSCOPE,
    "Speaker",	//E_SPEAKER, // 60
    "Rssi",	//E_RSSI,
    "CMMB",	//E_CMMB,
    "EMMC",	//E_EMMC,
    "EMI",	//ITEM_EMI,
    "ClearEmmc",	//ITEM_CLREMMC,
    "HDMI",	//E_HDMI,
    "RECEIVER_PHONE",	//ITEM_RECEIVER_PHONE,
    "HEADSET_PHONE",	//ITEM_HEADSET_PHONE,
    "PHONEMICSPK_PHONE",	//ITEM_LOOPBACK_PHONEMICSPK_PHONE,
    "VIBRATOR_PHONE",	//ITEM_VIBRATOR_PHONE, //70
    "Micbias",	//E_MICBIAS,

    "ITEM_CUSTOM_START",	//ITEM_CUSTOM_START,
    "ITEM_CUSTOM_STOP",	//ITEM_CUSTOM_STOP,
    "ITEM_CUSTOM_REQUESTDATA",	//ITEM_CUSTOM_REQUESTDATA,
    "SW Version",	//E_SWVERSION,
    "Barcode",	//E_BARCODE,
    "Barcode",	//E_BARCODE,
    "ITEM_CUSTOM_CAMERADATA",	//ITEM_CUSTOM_CAMERADATA,

	"ITEM_CUSTOM_PROPERTY",	//ITEM_CUSTOM_PROPERTY,
    "RSSI",	//E_RSSI,	// 80
    "Hotknot",	//ITEM_HOTKNOT,
    "BTS",	//ITEM_BTS,
    "EXT_BUCK",	//ITEM_EXT_BUCK,
    "ITEM_EXT_VBAT_BOOST",	//ITEM_EXT_VBAT_BOOST,
    "ITEM_SPEAKER_MONITOR",	//ITEM_SPEAKER_MONITOR,
    "ITEM_SPEAKER_MONITOR_SET_TMP",	//ITEM_SPEAKER_MONITOR_SET_TMP,
    "C2k RF Test",	//ITEM_RF_C2K_TEST
    "USB Type-C",
    "ITEM_IRTX_LED_TEST",//ITEM_IRTX_LED_TEST,
    "ITEM_HEART_MONITOR",//ITEM_HEART_MONITOR,  //90
    "ITEM_EFUSE",//ITEM_EFUSE,
    "ITEM_MAIN_LENS",//ITEM_MAIN_LENS,
    "ITEM_MAIN2_LENS",//ITEM_MAIN2_LENS,
    "ITEM_SUB_LENS",//ITEM_SUB_LENS,
    "FingerPrint",//E_FINGERPRINT,
    "Humidity",//E_HUMIDITY,
    "GPS_CALI",
    "End"
};

char target_flag_sequence[128][64] ={0};//对应ITEM_SEQUENCE
int g_convert_flag_item[128] ={0};//对应g_callback_flag


static void ATACallBackOfAT(ATResult& atret, int handle_index)
{
    TRACE(" Enter ATACallBackOfAT\r\n");
	MTRACE(g_hEBOOT_DEBUG, "####[Thread%d]Enter ATACallBackOfAT####\r\n",handle_index+1);

    if(atret.resultLst.size() == 0)
        return;

    TRACE("---ATACallBackOfAT: %d\r\n", atret.resultLst[0].eleLst[0].str_len);
	MTRACE(g_hEBOOT_DEBUG, "------[Thread%d]ATACallBackOfAT: result len is: %d",handle_index+1,atret.resultLst[0].eleLst[0].str_len);
    //TRACE(atret.resultLst[0].eleLst[0].str_value.c_str());
	//MTRACE(g_hEBOOT_DEBUG, "------[Thread%d]ATACallBackOfAT: result is: %s\r\n",handle_index+1,atret.resultLst[0].eleLst[0].str_value.c_str());
    //TRACE("\r\n");

    if (atret.resultLst[0].eleLst[0].str_len <= 0)
    {
    	MTRACE_ERR(g_hEBOOT_DEBUG, "------[Thread%d]ATACallBackOfAT: result  len  <=0",handle_index+1);
        return;
    }

#ifdef __ATA20__

	char    result_str[1024*2];
    int     m_iWholeCmdLen = atret.resultLst[0].eleLst[0].str_value.length();
	
    memset (result_str, 0x0, sizeof(char)*1024*2);
    memcpy (result_str, atret.resultLst[0].eleLst[0].str_value.c_str(), m_iWholeCmdLen);
	
    result_str[m_iWholeCmdLen] = '\0';
    //if (glb_at_resp_flag)
	if (result_str[1] != ':' && result_str[2] != ':')
    {
        TRACE ("ATACallBackOfAT: global response data.\r\n");
		
		MTRACE (g_hEBOOT_DEBUG,"------[Thread%d]ATACallBackOfAT: It's global response data.",handle_index+1);
        if (g_TestItemObj[handle_index][glb_test_item[handle_index]] != NULL)
        {
            g_TestItemObj[handle_index][glb_test_item[handle_index]]->SetATResponse (atret);
			
        }
    }
    else
    {
        char *pFlag = strstr (atret.resultLst[0].eleLst[0].str_value.c_str(), ":");
        if (pFlag == NULL) return;

        char flagStr[128];
        memset (flagStr, 0x0, 128);
        memcpy (flagStr, atret.resultLst[0].eleLst[0].str_value.c_str(), pFlag - atret.resultLst[0].eleLst[0].str_value.c_str());
        flagStr[pFlag - atret.resultLst[0].eleLst[0].str_value.c_str()] = 0;

        TRACE ("ATACallBackOfAT: flag = %s\r\n", flagStr);
		MTRACE (g_hEBOOT_DEBUG,"------ATACallBackOfAT: flag = %s\r\n", flagStr);
        int itemSeq = atoi(flagStr);
		if(g_ReadTargetFlag)
		{
			if (itemSeq < 0 || itemSeq >= g_flag_num) 
        		{
        			MTRACE_ERR(g_hEBOOT_DEBUG,"flag exceed the range get from target[0,%d]",g_flag_num);
					return;
        		}
        	if (g_TestItemObj[handle_index][g_convert_flag_item[itemSeq]] != NULL)
        		{
       				MTRACE(g_hEBOOT_DEBUG,"------[Thread%d]ATACallBackOfAT:SetATResponse to %s,  itemSeq=%d",handle_index+1,target_flag_sequence[itemSeq],itemSeq);
            		g_TestItemObj[handle_index][g_convert_flag_item[itemSeq]]->SetATResponse (atret);
        		}
		}
		
		else//get flag sequence from target之前的原有流程
		{
       	 	if (itemSeq < 0 || itemSeq >= ITEM_MAX_IDS) 
        		{
        			MTRACE_ERR(g_hEBOOT_DEBUG,"flag exceed the range[0,%d]",ITEM_MAX_IDS);
					return;
        		}
        	if (g_TestItemObj[handle_index][g_callback_flag[itemSeq]] != NULL)
        		{
       				MTRACE(g_hEBOOT_DEBUG,"------[Thread%d]ATACallBackOfAT:SetATResponse to %s,  itemSeq=%d",handle_index+1,test_item_flow_name[itemSeq],itemSeq);
            		g_TestItemObj[handle_index][g_callback_flag[itemSeq]]->SetATResponse (atret);
        		}
		}

		if (g_callback_flag[itemSeq] == E_CHARGING &&
                g_TestItemObj[handle_index][E_ADC] != NULL)
        {
            g_TestItemObj[handle_index][E_ADC]->SetATResponse (atret);
        }
    }

#else
    if (glb_test_item[handle_index] < E_LCD || glb_test_item[handle_index] >= E_TEST_ITEM_COUNT)
    {
        return;
    }
    if (g_TestItemObj[handle_index][glb_test_item[handle_index]] != NULL)
    {
        TRACE ("ATACallBackOfAT: %d\r\n", glb_test_item[handle_index]);
        g_TestItemObj[handle_index][glb_test_item[handle_index]]->SetATResponse (atret);
    }
#endif
	MTRACE(g_hEBOOT_DEBUG, "####[Thread%d]ATACallBackOfAT  End ####\r\n",handle_index+1);

}



/**
 * Descriptions: FIXME
 */
ATA_DLL_Handle::ATA_DLL_Handle()
    :m_bAvailable(true)
{
    m_listLogStr.clear();
    m_fpLogFile = NULL;
    m_fDLLLogFile = NULL;
	m_fpLogCsvFile = NULL;
    bLogWithDateInfo = true;
//	memset(m_log_buf,0,sizeof(m_log_buf));

    m_hStateMutex		= CreateMutex(NULL, FALSE, NULL);
    for (int i = 0; i < ATA_DLL_MAX_CONCURRENT_THREADS; i ++)
    {
        glb_test_item[i] = E_TEST_ITEM_COUNT;
    }

    // Parse ini file
    ParseIniFile ();
}

/**
 * Descriptions: FIXME
 */
ATA_DLL_Handle::~ATA_DLL_Handle()
{
    if (m_hStateMutex != NULL)
    {
        CloseHandle (m_hStateMutex);
        m_hStateMutex = NULL;
    }

    if (m_fDLLLogFile != NULL)
    {
        fclose (m_fDLLLogFile);
        m_fDLLLogFile = NULL;
    }

    // Close file handle before renameing
    if (m_fpLogFile != NULL)
    {
        fclose (m_fpLogFile);
        m_fpLogFile = NULL;
    }
	 if (m_fpLogCsvFile != NULL)
    {
        fclose (m_fpLogCsvFile);
        m_fpLogCsvFile = NULL;
    }
}

/**
 * Descriptions: FIXME
 */
void ATA_DLL_Handle::Reset()
{
    m_bAvailable = true;
}

E_ATDLL_TEST_ITEM_COUNT ATA_DLL_Handle::GetItemIndex(string item_name)
{
	//MTRACE(g_hEBOOT_DEBUG,"item_name:%s",item_name.c_str());
	int pos= item_name.find(",");
	int str_len= item_name.length();
	item_name=item_name.replace(pos,str_len-pos,"\0");
	//MTRACE(g_hEBOOT_DEBUG,"after replace item_name:%s",item_name.c_str());
    E_ATDLL_TEST_ITEM_COUNT enum_index = E_TEST_ITEM_COUNT;
    for (enum_index = E_LCD; enum_index < E_TEST_ITEM_COUNT; enum_index = (E_ATDLL_TEST_ITEM_COUNT)((int)enum_index + 1))
    {
        //if (strstr(item_name.c_str(), test_item_name[enum_index]) != NULL)
        if (strcmp(item_name.c_str(), test_item_name[enum_index])== 0)
        {
            bool bSelected = false;
            switch (enum_index)
            {
            case E_LCD:
                if (m_testItemCFG.b_LCD) bSelected = true;
                break;
            case E_SLPMODE:
                if (m_testItemCFG.b_slpmode) bSelected = true;
                break;
			case E_OFFMODE:
				if (m_testItemCFG.b_offMode) bSelected = true;
				break;
            case E_CAMERA:
                if (m_testItemCFG.b_Camera) bSelected = true;
                break;
            case E_CAMERA_SUB:
                if (m_testItemCFG.b_Camera_Sub) bSelected = true;
                break;
			case E_CAMERA_MAIN2:
				if (m_testItemCFG.b_Camera_Main2) bSelected = true;
                break;
            case E_VIBRATOR:
                if (m_testItemCFG.b_Vibrator) bSelected = true;
                break;
            case E_KEYPAD:
                if (m_testItemCFG.b_Keypad) bSelected = true;
                break;
            case E_TOUCHSCR:
                if (m_testItemCFG.b_TouchScr) bSelected = true;
                break;
            case E_CMMB:
                if (m_testItemCFG.b_CMMB) bSelected = true;
                break;
            case E_RTC:
                if (m_testItemCFG.b_RTC) bSelected = true;
                break;
            case E_ADC:
                if (m_testItemCFG.b_ADC) bSelected = true;
                break;
            case E_LED:
                if (m_testItemCFG.b_LED) bSelected = true;
                break;

            case E_RING:
                if (m_testItemCFG.b_Ring) bSelected = true;
                break;
            case E_EARTHPHONE_STATE:
                if (m_testItemCFG.b_er_state) bSelected = true;
                break;

            case E_WIFI:
                if (m_testItemCFG.b_WIFI) bSelected = true;
                break;
            case E_GPS:
                if (m_testItemCFG.b_GPS) bSelected = true;
                break;
            case E_BT:
                if (m_testItemCFG.b_BT) bSelected = true;
                break;
            case E_TV:
                if (m_testItemCFG.b_TV) bSelected = true;
                break;

            case E_TCARD:
                if (m_testItemCFG.b_TCard) bSelected = true;
                break;
            case E_EMMC:
                if (m_testItemCFG.b_EMMC) bSelected = true;
                break;
            case E_FLASH:
                if (m_testItemCFG.b_Flash) bSelected = true;
                break;
            case E_SIM:
                if (m_testItemCFG.b_SIM) bSelected = true;
                break;
            case E_BARCODE:
                if (m_testItemCFG.b_Barcode) bSelected = true;
                break;
            case E_SWVERSION:
                if (m_testItemCFG.b_SWVersion) bSelected = true;
                break;
            case E_HWVERSIOM:
                if (m_testItemCFG.b_HWVersion) bSelected = true;
                break;

            case E_SPEAKER:
                if (m_testItemCFG.b_Speaker) bSelected = true;
                break;
            case E_RECEIVER:
                if (m_testItemCFG.b_Receiver) bSelected = true;
                break;
            case E_EARTHPHONE:
                if (m_testItemCFG.b_EarchPhone) bSelected = true;
                break;

            case E_FM:
                if (m_testItemCFG.b_FM) bSelected = true;
                break;
            case E_CHARGING:
                if (m_testItemCFG.b_Charging) bSelected = true;
                break;

            case E_GSENSOR:
                if (m_testItemCFG.b_gsensor) bSelected = true;
                break;
            case E_MSENSOR:
                if (m_testItemCFG.b_msensor) bSelected = true;
                break;
            case E_ALSPS:
                if (m_testItemCFG.b_alsps) bSelected = true;
                break;
            case E_GYROSCOPE:
                if (m_testItemCFG.b_gyroscope) bSelected = true;
                break;

            case E_RSSI:
                if (m_testItemCFG.b_RSSI) bSelected = true;
                break;
			case E_MICBIAS:
				if (m_testItemCFG.b_micbias) bSelected = true;
				break;
			case E_OTG:
				if (m_testItemCFG.b_OTG) bSelected = true;
				break;
			case E_HDMI:
				if (m_testItemCFG.b_HDMI) bSelected = true;
				break;
			case E_MHL:
				if (m_testItemCFG.b_MHL) bSelected = true;
				break;
			case E_USBTYPEC:
				if (m_testItemCFG.b_UsbTypeC) bSelected = true;
				break;
			case E_HUMIDITY:
				if (m_testItemCFG.b_Humidity) bSelected = true;
				break;
			case E_FINGERPRINT:
				if (m_testItemCFG.b_FingerPrint) bSelected = true;
				break;
			case E_BTS:
				if (m_testItemCFG.b_BTS) bSelected = true;
				break;
			case E_EXT_BUCK:
				if (m_testItemCFG.b_ExtBuck) bSelected = true;
				break;
			case E_BAROMETER:
				if (m_testItemCFG.b_Barometer) bSelected = true;
				break;
			case E_GPS_CALI:
				if (m_testItemCFG.b_GPSCali) bSelected = true;
				break;
            }

            if (!bSelected)
            {
                enum_index = E_TEST_ITEM_COUNT;
            }
            break;
        }
    }

    return enum_index;
}


bool ATA_DLL_Handle::CheckIniItemSingle(E_ATDLL_TEST_ITEM_COUNT item)
{
    int loop_index = 0;
    for (loop_index = 0; loop_index < testcases_index_bg; loop_index++)
    {
        if (testcases_bg[loop_index].item == item)
        {
            return false;
        }
    }
    for (loop_index = 0; loop_index < testcases_index_fg; loop_index++)
    {
        if (testcases_fg[loop_index].item == item)
        {
            return false;
        }
    }
    return true;
}

void ATA_DLL_Handle::ParseIniFile()
{
    char currentDir[512];
    GetCurrentDirectory (512, currentDir);
    char iniFilePath[512];
    sprintf (iniFilePath, "%s\\TestItem.ini", currentDir);
    TIniFile iniFile(iniFilePath);

    testcases_index_bg = 0;
    testcases_index_fg = 0;
    testcases_index_conflict = 0;

    int temp_index = 0;
    char section_name[64];
    temp_index = temp_index;
    memset (section_name, 0x0, sizeof(char)*64);
  	
    sprintf (section_name, "Background Items");
    for (temp_index = 0; temp_index < MAX_TEST_CASE_NUM; temp_index ++)
    {
        char key_name[64];
        sprintf (key_name, "TestItem%d", temp_index);
        std::string ini_str = iniFile.ReadString (section_name, key_name, "");
        if (strcmp(ini_str.c_str(), "") == 0) continue;

        E_ATDLL_TEST_ITEM_COUNT item_index = GetItemIndex (ini_str);
        if (item_index >= E_TEST_ITEM_COUNT) continue;
        if (!CheckIniItemSingle(item_index)) continue;

        testcases_bg[testcases_index_bg].item = item_index;
        sprintf (testcases_bg[testcases_index_bg].item_name, "%s", test_item_name[item_index]);
        testcases_bg[testcases_index_bg].item_state = ITEM_STATE_READY;
        if (strstr(ini_str.c_str(), "1") != NULL)
        {
            testcases_bg[testcases_index_bg].b_auto = 0;
        }
        else
        {
            testcases_bg[testcases_index_bg].b_auto = 1;
        }
		MTRACE(g_hEBOOT_DEBUG,"[Background items]item=%d,item_name=%s",testcases_bg[testcases_index_bg].item,testcases_bg[testcases_index_bg].item_name);

        testcases_index_bg ++;
    }

    sprintf (section_name, "Conflicting Items");
    for (temp_index = 0; temp_index < MAX_TEST_CASE_NUM; temp_index ++)
    {
        char key_name[64];
        sprintf (key_name, "Pair%d", temp_index);
        std::string ini_str = iniFile.ReadString (section_name, key_name, "");
        if (strcmp(ini_str.c_str(), "") == 0) continue;

        int conflict_num = 0;
        for (E_ATDLL_TEST_ITEM_COUNT internel_enum_index = E_LCD; internel_enum_index < E_TEST_ITEM_COUNT; internel_enum_index = (E_ATDLL_TEST_ITEM_COUNT)((int)internel_enum_index + 1))
        {
            if (strstr(ini_str.c_str(), test_item_name[internel_enum_index]) != NULL)
            {
                testcases_conflict[testcases_index_conflict].item[conflict_num ++] = internel_enum_index;
                if (conflict_num >= 2)
                {
                    break;
                }
            }
        }

        // Check any, indicated by E_TEST_ITEM_COUNT
        if (conflict_num == 1)
        {
            if (strstr(ini_str.c_str(), "any") != NULL)
            {
                testcases_conflict[testcases_index_conflict].item[conflict_num ++] = E_TEST_ITEM_COUNT;
            }
        }
        testcases_index_conflict ++;
    }

    sprintf (section_name, "Foreground Items");
    for (temp_index = 0; temp_index < MAX_TEST_CASE_NUM; temp_index ++)
    {
        char key_name[64];
        sprintf (key_name, "TestItem%d", temp_index);
        std::string ini_str = iniFile.ReadString (section_name, key_name, "");
        if (strcmp(ini_str.c_str(), "") == 0) continue;

        E_ATDLL_TEST_ITEM_COUNT item_index = GetItemIndex (ini_str);
        if (item_index >= E_TEST_ITEM_COUNT) continue;
        if (!CheckIniItemSingle(item_index)) continue;
		TRACE ("ParseIniFile(): %d, %d, %s\r\n", temp_index, item_index, ini_str.c_str());

        testcases_fg[testcases_index_fg].item = item_index;
        sprintf (testcases_fg[testcases_index_fg].item_name, "%s", test_item_name[item_index]);
        testcases_fg[testcases_index_fg].item_state = ITEM_STATE_READY;
        if (strstr(ini_str.c_str(), "1") != NULL)
        {
            testcases_fg[testcases_index_fg].b_auto = 0;
        }
        else
        {
            testcases_fg[testcases_index_fg].b_auto = 1;
        }
		MTRACE(g_hEBOOT_DEBUG,"[Frontground items]item=%d,item_name=%s",testcases_fg[testcases_index_fg].item,testcases_fg[testcases_index_fg].item_name);

        testcases_index_fg ++;
    }

}

bool ATA_DLL_Handle::IsItemTest (E_ATDLL_TEST_ITEM_COUNT item)
{
    for (int loopIndex = 0; loopIndex < testcases_index_bg + testcases_index_fg; loopIndex ++)
    {
        if (loopIndex < testcases_index_bg)
        {
            if (item == testcases_bg[loopIndex].item)
            {
                return true;
            }
        }
        else
        {
            if (item == testcases_fg[loopIndex - testcases_index_bg].item)
            {
                return true;
            }
        }
    }

    return false;
}

bool ATA_DLL_Handle::EnterFactoryModeByUSB (unsigned int &comport)
{

    SP_BOOT_ARG_S stArg;

    memset (&stArg, 0x0, sizeof(SP_BOOT_ARG_S));
    stArg.m_bbchip_type = SP_AUTO_DETECT_BBCHIP;
    stArg.m_ext_clock = SP_AUTO_DETECT_EXT_CLOCK;
    stArg.m_ms_boot_timeout = SP_BOOT_INFINITE;
    stArg.m_max_start_cmd_retry_count = SP_DEFAULT_BROM_START_CMD_RETRY_COUNT;

    //New parameters
    stArg.m_uTimeout = 20000;
    stArg.m_uRetryTime = 2000;
    stArg.m_uInterval = 10;
    stArg.m_uBaudrate = CBR_115200;

    // Serial Link Authentication
    stArg.m_auth_handle = SPATE_Get_AuthHandle();
    stArg.m_scert_handle = SPATE_Get_ScertHandle();
    stArg.m_cb_sla_challenge = SLA_Challenge;
    stArg.m_cb_sla_challenge_arg = NULL;
    stArg.m_cb_sla_challenge_end = SLA_Challenge_END;
    stArg.m_cb_sla_challenge_end_arg = NULL;

    stArg.m_pStopFlag = m_commonCFG.stop_flag;
    stArg.m_bIsUSBEnable = true;
    stArg.m_bIsSymbolicEnable = false;
    stArg.m_bIsCompositeDeviceEnable = m_commonCFG.com_port_info.bIsComposite ? true : false;
    stArg.m_euBootMode = SP_FACTORY_BOOT;
    stArg.m_uPortNumber = m_commonCFG.com_port_info.preloader_com_port;

    Eboot_DebugOn ();
    SP_META_DebugOn ();
    SP_Brom_DebugOn ();

    CString cstr;
    cstr.Format("\\\\.\\COM%d", m_commonCFG.com_port_info.preloader_com_port);

#if 0
    long lModule = 0;
    for (int i = 0; i < 10; i ++)
    {
        if (*(m_commonCFG.stop_flag) == ATA_DLL_STOP_FLAG)
        {
            return false;
        }

        lModule = ATInitCommPort_Ex ((LPCTSTR)cstr, &m_commonCFG);
        if (lModule <= 0)
        {
            Sleep (100);
        }
        else
        {
            ATDeInitCommPort (lModule);
            break;
        }
    }
    if (lModule <= 0)
    {
        return false;
    }
#endif

    if (SP_S_DONE != SP_Preloader_BootMode(&stArg))
    {
        return false;
    }

    SP_META_DebugOff ();
    Eboot_DebugOff ();
    SP_Brom_DebugOff ();

    comport = m_commonCFG.com_port_info.kernel_com_port;

    return true;
}


bool ATA_DLL_Handle::IsPreloaderComPortOK()
{
    CString cstr;
    cstr.Format("\\\\.\\COM%d", m_commonCFG.com_port_info.preloader_com_port);

    long lModule = 0;
    lModule = ATInitCommPort_Ex ((LPCTSTR)cstr, &m_commonCFG);
    if (lModule <= 0)
    {
        return false;
    }
    else
    {
        ATDeInitCommPort (lModule);
        return true;
    }
}

long ATA_DLL_Handle::IsKernelComPortOK()
{
    CString cstr;
    cstr.Format("\\\\.\\COM%d", m_commonCFG.com_port_info.kernel_com_port);

    long lModule = 0;
    lModule = ATInitCommPort_Ex ((LPCTSTR)cstr, &m_commonCFG);
    if (lModule <= 0)
    {
        //Log_Record ("Search kernel com port fail.\r\n");
        return 0;
    }
    else
    {
        //Log_Record ("Search kernel com port ok.\r\n");
        return lModule;
    }
}

long ATA_DLL_Handle::GetKernelComModule ()
{
    CString cstr;
    cstr.Format("\\\\.\\COM%d", m_commonCFG.com_port_info.kernel_com_port);

    long lModule = 0;
    lModule = ATInitCommPort_Ex ((LPCTSTR)cstr, &m_commonCFG);
    return lModule;
}

// initState  1 - waiting preloader com port, 2 - waiting kernel com port, 3 - kernel com port ok
long ATA_DLL_Handle::GetCommModule (int initState)
{
    int state_index = initState ;
    long lModuleKernel = 0;
    unsigned int kernelCom = 0;
    if (initState == 1)
    {
        Log_Record ("Init state index = %d, wait for preloader com port %d.\r\n", state_index, m_commonCFG.com_port_info.preloader_com_port);
    }
    else
    {
        Log_Record ("Init state index = %d, wait for kernel com port %d.\r\n", state_index, m_commonCFG.com_port_info.kernel_com_port);

    }
    do
    {
        switch (state_index)
        {
        case 1:
            if (IsPreloaderComPortOK())
            {
                Log_Record ("Find preloader com port OK, start booting target into factory mode.\r\n");
                Sleep (100);
                if (EnterFactoryModeByUSB(kernelCom))
                {
                    state_index = 2;
                    Log_Record ("Enter factory mode ok, set state index = %d\r\n", state_index);
                    Log_Record ("Searching kernel com port %d.\r\n", m_commonCFG.com_port_info.kernel_com_port);
                }
                else
                {
                    Log_Record ("Enter factory mode fail, re-scan preloader com port.\r\n");
                }
            }
            Sleep (200); // Not too frequencely, incase each DUT com port affect
            break;
        case 2:
            if ((lModuleKernel = IsKernelComPortOK()) > 0)
            {
                state_index = 3;
                Log_Record ("Find kernel com port ok, set state index = %d\r\n", state_index);
                return lModuleKernel;
            }
            else if (IsPreloaderComPortOK())
            {
                state_index = 1;
                Log_Record ("Find preloader again before kernel com port display, set state index = %d\r\n", state_index);
            }
            Sleep (200);
            break;
#if 0
        case 3:
            if (IsPreloaderComPortOK())
            {
                state_index = 1;
                Log_Record ("Find preloader com port again before opening kernel com port, state index = %d\r\n", state_index);
            }
            else //if (IsKernelComPortOK(comm_cfg))
            {
                lModuleKernel = GetKernelComModule ();
                if (lModuleKernel > 0)
                {
                    Log_Record ("Open kernel com port ok.\r\n");
                    return lModuleKernel;
                }
            }
            break;
#endif

        default:
            break;
        }

        if (*(m_commonCFG.stop_flag) == ATA_DLL_STOP_FLAG)
        {
            Log_Record ("Stopped by user!\r\n");
            return lModuleKernel;
        }

        Sleep (200);

    }
    while (1);

    return lModuleKernel;
}

void ATA_DLL_Handle::ReleaseCommModule (CommModule *cm)
{
    if (cm != NULL)
        ATDeInitCommPort ((long)cm);

    if (m_commonCFG.com_port_info.usb_enable)
    {

    }
    else
    {

    }
}


long ATA_DLL_Handle::GetPMCommModule ()
{
    int com_port = m_commonCFG.com_port_info.pm_info.comport;
    if (com_port <= 0) com_port = 1000;

    CString cstr;
    cstr.Format("\\\\.\\COM%d", com_port);
    return ATInitCommPort_Ex_Ex ((LPCTSTR)cstr, &m_commonCFG);
}

bool ATA_DLL_Handle::CheckPMOfficial ()
{
    //Log_Record ("Check if power measure chip is offcial.\r\n");

    char randData[4] = {0};
    for (int i = 0; i < 3; i ++)
    {
        srand(time(0));
        randData[i] = rand()%10 + '0';
        if (randData[i] == 0)
            randData[i] = 0 + '0';
        Sleep (100);
    }
    //Log_Record ("send random string is %s.\r\n", randData);

    char *key = "MTK";
    int sum = 0;
    for (i = 0; i < 3; i ++)
    {
        sum = sum + randData[i] * key[i];
    }
    char encryedStr[256];
    sprintf (encryedStr, "%x", sum);
    //Log_Record ("Expected response is %s.\r\n", encryedStr);

    string atstr = "End\r\n";
    ATResult atret;

    // Reset
    m_pm_comm_handle->WriteCommand(atstr.c_str(), atstr.length(), ATTIMEOUT, false, atret);
    Sleep (500);

    char cmd_str[256];
    sprintf (cmd_str, "ActiveCode=%s\r\n", randData);
    atstr = cmd_str;
    bool result = m_pm_comm_handle->WriteCommand(atstr.c_str(), atstr.length(), ATTIMEOUT, false, atret);
    //Log_Record ("Final response is %s.\r\n", atret.resultLst[0].eleLst[0].str_value.c_str());
    if (result && strstr(atret.resultLst[0].eleLst[0].str_value.c_str(), encryedStr) != NULL)
    {
        //Log_Record ("check ok.\r\n");
        return true;
    }

    //Log_Record ("check failed, not MTK official power measure chip.\r\n");
    return false;

}

void ATA_DLL_Handle::ClosePMModule ()
{
    ATResult	atret;
    string	atstr;

    atstr = "End\r\n";
    m_pm_comm_handle->SetNeedWithCRLR (true);
    m_pm_comm_handle->WriteCommand(atstr.c_str(), atstr.length(), ATTIMEOUT, false, atret);
    m_pm_comm_handle->SetNeedWithCRLR (false);
}

bool ATA_DLL_Handle::SendStartCommand ()
{
    ATResult atret;
    string atstr = "AT+START";
    atstr += CR;
	m_testCNF->bAllPass = true;
    int count = 50;
    for (int loopIdx = 0; loopIdx < count; loopIdx ++)
    {
        Log_Record ("Send start command %d.\r\n", loopIdx+1);
        m_comm_handle->SetNeedWithCRLR(true);
        if (m_comm_handle->WriteCommand (atstr.c_str(), atstr.length(), 500, false, atret))
        {
            char    result_str[128];
            int     m_iWholeCmdLen = atret.resultLst[0].eleLst[0].str_value.length();
            m_iWholeCmdLen = m_iWholeCmdLen > 128 ? 128 : m_iWholeCmdLen;
            memset (result_str, 0x0, 128 * sizeof(char));
            memcpy (result_str, atret.resultLst[0].eleLst[0].str_value.c_str(), m_iWholeCmdLen);
            result_str[m_iWholeCmdLen] = 0;
            if (strstr(result_str, "pass") != NULL)
            {
                Log_Record ("Get response pass, OK!\r\n");
                return true;
            }
        }

        Sleep (100);
    }

    if (loopIdx >= count)
    {
        Log_Record ("Send start command fail after %d times\r\n", loopIdx+1);
        return false;
    }

    return true;

}
/****************************************************************************************/
//从Target端获取flag sequence，读取到的是char*对应ITEM_SEQUENCE
//
/****************************************************************************************/
bool ATA_DLL_Handle::RequestFlagFromTarget (int* number)
{

   ATResult atret;
   string atstr = "AT+REQUESTFLAG";
   atstr += CR;
   char    result_str[1024*2];
   int len = 0;
   Log_Record ("Send request flag command.\r\n");
   m_comm_handle->SetNeedWithCRLR(true);
   if (m_comm_handle->WriteCommand (atstr.c_str(), atstr.length(), 500, false, atret))
   {	
        /* int     m_iWholeCmdLen = atret.resultLst[0].eleLst[0].str_value.length();
         memset (result_str, 0x0, 8196 * sizeof(char));
         memcpy (result_str, atret.resultLst[0].eleLst[0].str_value.c_str(), m_iWholeCmdLen);
         result_str[m_iWholeCmdLen] = 0;*/
        m_comm_handle->GetCommandResult (result_str, len);
		result_str[len]=0;
		
		MTRACE(g_hEBOOT_DEBUG, "Get flag result is:%s",result_str);
		
         if (strstr(result_str, "ITEM_MAX_IDS") != NULL)
         {
                Log_Record ("Get flag from target OK!\r\n");
				Log_Record ("flag sequence from target are:%s\r\n",result_str);
				Log_Record ("Start parsing...\r\n");
				//parse flag
				int k=0,j=0;
				for(int i=0;i<len;i++)
					{
						if(result_str[i]==',')
						{
						  j++;
						  k=0;
						  i++;//取','后的值
						}
					    target_flag_sequence[j][k]=result_str[i];
					    k++;
					}

				*number=j;
				Log_Record ("Parse flag success,total number is:%d\r\n",&number);
                
				//parse flag end
				
                return true;
         }
		 else
		 	  return false;
   }
   else 
   	      return false;

}

bool ATA_DLL_Handle::SendStopCommand ()
{
    ATResult atret;
    string atstr = "AT+STOP";
	//add songjian 2016-4-26
	if(m_testCNF->bAllPass){
		atstr = "AT+ATAPASS";
	} else {
		atstr = "AT+ATAFAIL";
	}
	//add songjian 2016-4-26
    atstr += CR;

    int count = 1;
    for (int loopIdx = 0; loopIdx < count; loopIdx ++)
    {
        m_comm_handle->SetNeedWithCRLR(true);
        if (m_comm_handle->WriteCommand (atstr.c_str(), atstr.length(), 1000, false, atret))
        {
            char    result_str[128];
            int     m_iWholeCmdLen = atret.resultLst[0].eleLst[0].str_value.length();
            m_iWholeCmdLen = m_iWholeCmdLen > 128 ? 128 : m_iWholeCmdLen;
            memset (result_str, 0x0, 128 * sizeof(char));
            memcpy (result_str, atret.resultLst[0].eleLst[0].str_value.c_str(), m_iWholeCmdLen);
            result_str[m_iWholeCmdLen] = 0;
            if (strstr(result_str, "pass") != NULL)
            {
                return true;
            }
        }

        Sleep (100);
    }

    if (loopIdx >= count)
    {
        return false;
    }

    return true;
}

bool ATA_DLL_Handle::QueryResult (E_ATDLL_TEST_ITEM_COUNT test_item)
{
    bool result = true;
    bool result_temp = true;

    switch (test_item)
    {
    case E_CAMERA:
        if (m_testItemCFG.b_Camera)
        {
            m_commonCFG.cbQueryTestItemResult(handle_index, E_CAMERA, &result_temp);
            result = result & result_temp;
        }
        break;

    case E_CAMERA_SUB:
        if (m_testItemCFG.b_Camera_Sub)
        {
            m_commonCFG.cbQueryTestItemResult(handle_index, E_CAMERA_SUB, &result_temp);
            result = result & result_temp;
        }
        break;

    case E_SPEAKER:
        if (m_testItemCFG.b_Speaker)
        {
            m_commonCFG.cbQueryTestItemResult(handle_index, E_SPEAKER, &result_temp);
            result = result & result_temp;
        }
        break;
    case E_RECEIVER:
        if (m_testItemCFG.b_Receiver)
        {
            m_commonCFG.cbQueryTestItemResult(handle_index, E_RECEIVER, &result_temp);
            result = result & result_temp;
        }
        break;
    case E_EARTHPHONE:
        if (m_testItemCFG.b_EarchPhone)
        {
            m_commonCFG.cbQueryTestItemResult(handle_index, E_EARTHPHONE, &result_temp);
            result = result & result_temp;
        }
        break;
    case E_GSENSOR:
        if (m_testItemCFG.b_gsensor)
        {
            m_commonCFG.cbQueryTestItemResult(handle_index, E_GSENSOR, &result_temp);
            result = result & result_temp;
        }
        break;
    case E_MSENSOR:
        if (m_testItemCFG.b_msensor)
        {
            m_commonCFG.cbQueryTestItemResult(handle_index, E_MSENSOR, &result_temp);
            result = result & result_temp;
        }
        break;
    case E_ALSPS:
        if (m_testItemCFG.b_alsps)
        {
            m_commonCFG.cbQueryTestItemResult(handle_index, E_ALSPS, &result_temp);
            result = result & result_temp;
        }
        break;
    case E_GYROSCOPE:
        if (m_testItemCFG.b_gyroscope)
        {
            m_commonCFG.cbQueryTestItemResult(handle_index, E_GYROSCOPE, &result_temp);
            result = result & result_temp;
        }
        break;
    default:
        m_commonCFG.cbQueryTestItemResult(handle_index, test_item, &result);

    }

    return result;
}

/**
 * Descriptions: FIXME
 */
bool ATA_DLL_Handle::InitTestItemObj ()
{

#ifdef __ATA_CONCURRENT_FLOW__
    int index = 0;
    E_ATDLL_TEST_ITEM_COUNT test_item = E_LCD;
    for (index = 0; index < testcases_index_bg + testcases_index_fg; index ++)
    {
        if (index < testcases_index_bg)
        {
            test_item = testcases_bg[index].item;
        }
        else
        {
            test_item = testcases_fg[index-testcases_index_bg].item;
        }

        switch (test_item)
        {
        case E_SWVERSION:
            g_TestItemObj[handle_index][E_SWVERSION] =
                new CTestItem_SWVersion(this, m_commonCFG.timeout[E_SWVERSION], E_SWVERSION, "SWVersion");

            if (g_TestItemObj[handle_index][E_SWVERSION] == NULL)
            {
                return false;
            }
            break;

        case E_HWVERSIOM:
            g_TestItemObj[handle_index][E_HWVERSIOM] =
                new CTestItem_HWVer (this, m_commonCFG.timeout[E_HWVERSIOM], E_HWVERSIOM, "LCD&Backlight");

            if (g_TestItemObj[handle_index][E_HWVERSIOM] == NULL)
            {
                return false;
            }
            break;

        case E_BARCODE:
            g_TestItemObj[handle_index][E_BARCODE] =
                new CTestItem_Barcode(this, m_commonCFG.timeout[E_BARCODE], E_BARCODE, "Barcode");

            if (g_TestItemObj[handle_index][E_BARCODE] == NULL)
            {
                return false;
            }
            break;

        case E_TCARD:
            g_TestItemObj[handle_index][E_TCARD] =
                new CTestItem_TCard(this, m_commonCFG.timeout[E_TCARD], E_TCARD, "TCard");

            if (g_TestItemObj[handle_index][E_TCARD] == NULL)
            {
                return false;
            }
            break;

        case E_SIM:
            g_TestItemObj[handle_index][E_SIM] =
                new CTestItem_SIMCard(this, m_commonCFG.timeout[E_SIM], E_SIM, "SIMCard");

            if (g_TestItemObj[handle_index][E_SIM] == NULL)
            {
                return false;
            }
            break;

        case E_GPS:
            g_TestItemObj[handle_index][E_GPS] =
                new CTestItem_GPS(this, m_commonCFG.timeout[E_GPS], E_GPS, "GPS");

            if (g_TestItemObj[handle_index][E_GPS] == NULL)
            {
                return false;
            }
            break;

        case E_WIFI:
            g_TestItemObj[handle_index][E_WIFI] =
                new CTestItem_WIFI(this, m_commonCFG.timeout[E_WIFI], E_WIFI, "WIFI");

            if (g_TestItemObj[handle_index][E_WIFI] == NULL)
            {
                return false;
            }
            break;

        case E_BT:
            g_TestItemObj[handle_index][E_BT] =
                new CTestItem_BT(this, m_commonCFG.timeout[E_BT], E_BT, "BT");

            if (g_TestItemObj[handle_index][E_BT] == NULL)
            {
                return false;
            }
            break;

        case E_FM:
            g_TestItemObj[handle_index][E_FM] =
                new CTestItem_FM(this, m_commonCFG.timeout[E_FM], E_FM, "FM");

            if (g_TestItemObj[handle_index][E_FM] == NULL)
            {
                return false;
            }
            break;

        case E_CHARGING:
            g_TestItemObj[handle_index][E_CHARGING] =
                new CTestItem_Charger(this, m_commonCFG.timeout[E_CHARGING], E_CHARGING, "Charger");

            if (g_TestItemObj[handle_index][E_CHARGING] == NULL)
            {
                return false;
            }
            break;

        case E_VIBRATOR:
            g_TestItemObj[handle_index][E_VIBRATOR] =
                new CTestItem_Vibrator(this, m_commonCFG.timeout[E_VIBRATOR], E_VIBRATOR, "Vibrator");

            if (g_TestItemObj[handle_index][E_VIBRATOR] == NULL)
            {
                return false;
            }
            break;

        case E_TV:
            g_TestItemObj[handle_index][E_TV] =
                new CTestItem_MATV(this, m_commonCFG.timeout[E_TV], E_TV, "MATV");

            if (g_TestItemObj[handle_index][E_TV] == NULL)
            {
                return false;
            }
            break;

        case E_SPEAKER:
            g_TestItemObj[handle_index][E_SPEAKER] =
                new CTestItem_Audio (this, m_commonCFG.timeout[E_SPEAKER], E_SPEAKER, "Speaker");

            if (g_TestItemObj[handle_index][E_SPEAKER] == NULL)
            {
                return false;
            }
            break;

        case E_RECEIVER:
            g_TestItemObj[handle_index][E_RECEIVER] =
                new CTestItem_Audio (this, m_commonCFG.timeout[E_RECEIVER], E_RECEIVER, "Receiver");

            if (g_TestItemObj[handle_index][E_RECEIVER] == NULL)
            {
                return false;
            }
            break;

        case E_EARTHPHONE:
            g_TestItemObj[handle_index][E_EARTHPHONE] =
                new CTestItem_Audio (this, m_commonCFG.timeout[E_EARTHPHONE], E_EARTHPHONE, "Headset");

            if (g_TestItemObj[handle_index][E_EARTHPHONE] == NULL)
            {
                return false;
            }
            break;

        case E_CAMERA:
            g_TestItemObj[handle_index][E_CAMERA] =
                new CTestItem_Camera (this, m_commonCFG.timeout[E_CAMERA], E_CAMERA, "Main Camera");

            if (g_TestItemObj[handle_index][E_CAMERA] == NULL)
            {
                return false;
            }
            break;

        case E_CAMERA_SUB:
            g_TestItemObj[handle_index][E_CAMERA_SUB] =
                new CTestItem_Camera (this, m_commonCFG.timeout[E_CAMERA_SUB], E_CAMERA_SUB, "Sub Camera");

            if (g_TestItemObj[handle_index][E_CAMERA_SUB] == NULL)
            {
                return false;
            }
            break;
		case E_CAMERA_MAIN2:
			
			g_TestItemObj[handle_index][E_CAMERA_MAIN2] =
				new CTestItem_Camera (this, m_commonCFG.timeout[E_CAMERA_MAIN2], E_CAMERA_MAIN2, "Main2 Camera");
			
			if (g_TestItemObj[handle_index][E_CAMERA_MAIN2] == NULL)
			{
				return false;
			}
			break;

        case E_LCD:
            g_TestItemObj[handle_index][E_LCD] =
                new CTestItem_LCM (this, m_commonCFG.timeout[E_LCD], E_LCD, "LCD");

            if (g_TestItemObj[handle_index][E_LCD] == NULL)
            {
                return false;
            }
            break;

        case E_TOUCHSCR:
            g_TestItemObj[handle_index][E_TOUCHSCR] =
                new CTestItem_TP (this, m_commonCFG.timeout[E_TOUCHSCR], E_TOUCHSCR, "Touch Panel");

            if (g_TestItemObj[handle_index][E_TOUCHSCR] == NULL)
            {
                return false;
            }
            break;

        case E_RTC:
            g_TestItemObj[handle_index][E_RTC] =
                new CTestItem_RTC (this, m_commonCFG.timeout[E_RTC], E_RTC, "RTC");

            if (g_TestItemObj[handle_index][E_RTC] == NULL)
            {
                return false;
            }
            break;

        case E_KEYPAD:
            g_TestItemObj[handle_index][E_KEYPAD] =
                new CTestItem_KPad(this, m_commonCFG.timeout[E_KEYPAD], E_KEYPAD, "Key Pad");

            if (g_TestItemObj[handle_index][E_KEYPAD] == NULL)
            {
                return false;
            }
            break;

        case E_ADC:
            g_TestItemObj[handle_index][E_ADC] =
                new CTestItem_ADC(this, m_commonCFG.timeout[E_ADC], E_ADC, "ADC");

            if (g_TestItemObj[handle_index][E_ADC] == NULL)
            {
                return false;
            }
            break;

        case E_SLPMODE:
            g_TestItemObj[handle_index][E_SLPMODE] =
                new CTestItem_SleepMode(this, m_commonCFG.timeout[E_SLPMODE], E_SLPMODE, "Sleep Mode");

            if (g_TestItemObj[handle_index][E_SLPMODE] == NULL)
            {
                return false;
            }
            break;
			/*mingxue 20150803*/
		case E_OFFMODE:
            g_TestItemObj[handle_index][E_OFFMODE] =
                new CTestItem_SleepMode(this, m_commonCFG.timeout[E_OFFMODE], E_OFFMODE, "Off Current");

            if (g_TestItemObj[handle_index][E_OFFMODE] == NULL)
            {
                return false;
            }
            break;

        case E_RSSI:
            g_TestItemObj[handle_index][E_RSSI] =
                new CTestItem_RSSI(this, m_commonCFG.timeout[E_RSSI], E_RSSI, "Signal Strength");

            if (g_TestItemObj[handle_index][E_RSSI] == NULL)
            {
                return false;
            }
            break;

        case E_LED:
            g_TestItemObj[handle_index][E_LED] =
                new CTestItem_LED(this, m_commonCFG.timeout[E_LED], E_LED, "LED");

            if (g_TestItemObj[handle_index][E_LED] == NULL)
            {
                return false;
            }
            break;

        case E_EMMC:
            g_TestItemObj[handle_index][E_EMMC] =
                new CTestItem_EMMC(this, m_commonCFG.timeout[E_EMMC], E_EMMC, "EMMC");

            if (g_TestItemObj[handle_index][E_EMMC] == NULL)
            {
                return false;
            }
            break;

        case E_GSENSOR:
            g_TestItemObj[handle_index][E_GSENSOR] =
                new CTestItem_Sensor(this, m_commonCFG.timeout[E_GSENSOR], E_GSENSOR, "G-Sensor");

            if (g_TestItemObj[handle_index][E_GSENSOR] == NULL)
            {
                return false;
            }
            break;

        case E_MSENSOR:
            g_TestItemObj[handle_index][E_MSENSOR] =
                new CTestItem_Sensor(this, m_commonCFG.timeout[E_MSENSOR], E_MSENSOR, "M-Sensor");

            if (g_TestItemObj[handle_index][E_MSENSOR] == NULL)
            {
                return false;
            }
            break;

        case E_ALSPS:
            g_TestItemObj[handle_index][E_ALSPS] =
                new CTestItem_Sensor(this, m_commonCFG.timeout[E_ALSPS], E_ALSPS, "ALSPS");

            if (g_TestItemObj[handle_index][E_ALSPS] == NULL)
            {
                return false;
            }
            break;

        case E_GYROSCOPE:
            g_TestItemObj[handle_index][E_GYROSCOPE] =
                new CTestItem_Sensor(this, m_commonCFG.timeout[E_GYROSCOPE], E_GYROSCOPE, "GYROSCOPE");

            if (g_TestItemObj[handle_index][E_GYROSCOPE] == NULL)
            {
                return false;
            }
            break;
        case E_MICBIAS:
            g_TestItemObj[handle_index][E_MICBIAS] =
                new CTestItem_Micbias(this, m_commonCFG.timeout[E_MICBIAS], E_MICBIAS, "Micbias");

            if (g_TestItemObj[handle_index][E_MICBIAS] == NULL)
            {
                return false;
            }
            break;

		case E_OTG:
            g_TestItemObj[handle_index][E_OTG] =
                new CTestItem_OTG (this, m_commonCFG.timeout[E_OTG], E_OTG, "OTG");

            if (g_TestItemObj[handle_index][E_OTG] == NULL)
            {
                return false;
            }
            break;

		case E_HDMI:
            g_TestItemObj[handle_index][E_HDMI] =
                new CTestItem_HDMI (this, m_commonCFG.timeout[E_HDMI], E_HDMI, "HDMI");

            if (g_TestItemObj[handle_index][E_HDMI] == NULL)
            {
                return false;
            }
            break;

		case E_MHL:
            g_TestItemObj[handle_index][E_MHL] =
                new CTestItem_HDMI (this, m_commonCFG.timeout[E_MHL], E_MHL, "MHL");

            if (g_TestItemObj[handle_index][E_MHL] == NULL)
            {
                return false;
            }
            break;
		case E_USBTYPEC:
			g_TestItemObj[handle_index][E_USBTYPEC] =
                new CTestItem_USB (this, m_commonCFG.timeout[E_USBTYPEC], E_USBTYPEC, "USB Type-C");

            if (g_TestItemObj[handle_index][E_USBTYPEC] == NULL)
            {
                return false;
            }
            break;
		case E_HUMIDITY:
			g_TestItemObj[handle_index][E_HUMIDITY] =
                new CTestItem_Humidity (this, m_commonCFG.timeout[E_HUMIDITY], E_HUMIDITY, "Humidity");

            if (g_TestItemObj[handle_index][E_HUMIDITY] == NULL)
            {
                return false;
            }
            break;
		case E_FINGERPRINT:
			g_TestItemObj[handle_index][E_FINGERPRINT] =
                new CTestItem_FingerPrint (this, m_commonCFG.timeout[E_FINGERPRINT], E_FINGERPRINT, "FingerPrint");

            if (g_TestItemObj[handle_index][E_FINGERPRINT] == NULL)
            {
                return false;
            }
            break;
		case E_BTS:
			g_TestItemObj[handle_index][E_BTS] =
                new CTestItem_BTS (this, m_commonCFG.timeout[E_BTS], E_BTS, "BTS");

            if (g_TestItemObj[handle_index][E_BTS] == NULL)
            {
                return false;
            }
            break;
		case E_EXT_BUCK:
			g_TestItemObj[handle_index][E_EXT_BUCK] =
                new CTestItem_ExtBuck (this, m_commonCFG.timeout[E_EXT_BUCK], E_EXT_BUCK, "Ext-Buck");

            if (g_TestItemObj[handle_index][E_EXT_BUCK] == NULL)
            {
                return false;
            }
            break;
		case E_BAROMETER:
			g_TestItemObj[handle_index][E_BAROMETER] =
                new CTestItem_Barometer (this, m_commonCFG.timeout[E_BAROMETER], E_BAROMETER, "Barometer");

            if (g_TestItemObj[handle_index][E_BAROMETER] == NULL)
            {
                return false;
            }
            break;

		case E_GPS_CALI:
			g_TestItemObj[handle_index][E_GPS_CALI] =
                new CTestItem_GPSCali(this, m_commonCFG.timeout[E_GPS_CALI], E_GPS_CALI, "GPSCali");

            if (g_TestItemObj[handle_index][E_GPS_CALI] == NULL)
            {
                return false;
            }
            break;
			

        default:
            break;
        }
    }

#else

    if (m_testItemCFG.b_SWVersion)
    {
        g_TestItemObj[handle_index][E_SWVERSION] =
            new CTestItem_SWVersion(this, m_commonCFG.timeout[E_SWVERSION], E_SWVERSION, "SWVersion");

        if (g_TestItemObj[handle_index][E_SWVERSION] == NULL)
        {
            return false;
        }
    }
    if (m_testItemCFG.b_Barcode)
    {
        g_TestItemObj[handle_index][E_BARCODE] =
            new CTestItem_Barcode(this, m_commonCFG.timeout[E_BARCODE], E_BARCODE, "Barcode");

        if (g_TestItemObj[handle_index][E_BARCODE] == NULL)
        {
            return false;
        }
    }
    if (m_testItemCFG.b_TCard)
    {
        g_TestItemObj[handle_index][E_TCARD] =
            new CTestItem_TCard(this, m_commonCFG.timeout[E_TCARD], E_TCARD, "TCard");

        if (g_TestItemObj[handle_index][E_TCARD] == NULL)
        {
            return false;
        }
    }
    if (m_testItemCFG.b_SIM)
    {
        g_TestItemObj[handle_index][E_SIM] =
            new CTestItem_SIMCard(this, m_commonCFG.timeout[E_SIM], E_SIM, "SIMCard");

        if (g_TestItemObj[handle_index][E_SIM] == NULL)
        {
            return false;
        }
    }
    if (m_testItemCFG.b_GPS)
    {
        g_TestItemObj[handle_index][E_GPS] =
            new CTestItem_GPS(this, m_commonCFG.timeout[E_GPS], E_GPS, "GPS");

        if (g_TestItemObj[handle_index][E_GPS] == NULL)
        {
            return false;
        }
    }

    if (m_testItemCFG.b_WIFI)
    {
        g_TestItemObj[handle_index][E_WIFI] =
            new CTestItem_WIFI(this, m_commonCFG.timeout[E_WIFI], E_WIFI, "WIFI");

        if (g_TestItemObj[handle_index][E_WIFI] == NULL)
        {
            return false;
        }
    }

    if (m_testItemCFG.b_BT)
    {
        g_TestItemObj[handle_index][E_BT] =
            new CTestItem_BT(this, m_commonCFG.timeout[E_BT], E_BT, "BT");

        if (g_TestItemObj[handle_index][E_BT] == NULL)
        {
            return false;
        }
    }

    if (m_testItemCFG.b_FM)
    {
        g_TestItemObj[handle_index][E_FM] =
            new CTestItem_FM(this, m_commonCFG.timeout[E_FM], E_FM, "FM");

        if (g_TestItemObj[handle_index][E_FM] == NULL)
        {
            return false;
        }
    }
    if (m_testItemCFG.b_Charging)
    {
        g_TestItemObj[handle_index][E_CHARGING] =
            new CTestItem_Charger(this, m_commonCFG.timeout[E_CHARGING], E_CHARGING, "Charger");

        if (g_TestItemObj[handle_index][E_CHARGING] == NULL)
        {
            return false;
        }
    }
    if (m_testItemCFG.b_Vibrator)
    {
        g_TestItemObj[handle_index][E_VIBRATOR] =
            new CTestItem_Vibrator(this, m_commonCFG.timeout[E_VIBRATOR], E_VIBRATOR, "Vibrator");

        if (g_TestItemObj[handle_index][E_VIBRATOR] == NULL)
        {
            return false;
        }
    }
    if (m_testItemCFG.b_TV)
    {
        g_TestItemObj[handle_index][E_TV] =
            new CTestItem_MATV(this, m_commonCFG.timeout[E_TV], E_TV, "MATV");

        if (g_TestItemObj[handle_index][E_TV] == NULL)
        {
            return false;
        }
    }
    // Audio : Speark, receiver, earthphone
    if (m_testItemCFG.b_Speaker)
    {
        g_TestItemObj[handle_index][E_SPEAKER] =
            new CTestItem_Audio (this, m_commonCFG.timeout[E_SPEAKER], E_SPEAKER, "Speaker");

        if (g_TestItemObj[handle_index][E_SPEAKER] == NULL)
        {
            return false;
        }
    }

    if (m_testItemCFG.b_Receiver)
    {
        g_TestItemObj[handle_index][E_RECEIVER] =
            new CTestItem_Audio (this, m_commonCFG.timeout[E_RECEIVER], E_RECEIVER, "Receiver");

        if (g_TestItemObj[handle_index][E_RECEIVER] == NULL)
        {
            return false;
        }
    }

    if (m_testItemCFG.b_EarchPhone)
    {
        g_TestItemObj[handle_index][E_EARTHPHONE] =
            new CTestItem_Audio (this, m_commonCFG.timeout[E_EARTHPHONE], E_EARTHPHONE, "Headset");

        if (g_TestItemObj[handle_index][E_EARTHPHONE] == NULL)
        {
            return false;
        }
    }

    if (m_testItemCFG.b_Camera)
    {
        g_TestItemObj[handle_index][E_CAMERA] =
            new CTestItem_Camera (this, m_commonCFG.timeout[E_CAMERA], E_CAMERA, "Main Camera");

        if (g_TestItemObj[handle_index][E_CAMERA] == NULL)
        {
            return false;
        }
    }

    if (m_testItemCFG.b_Camera_Sub)
    {
        g_TestItemObj[handle_index][E_CAMERA_SUB] =
            new CTestItem_Camera (this, m_commonCFG.timeout[E_CAMERA_SUB], E_CAMERA_SUB, "Sub Camera");

        if (g_TestItemObj[handle_index][E_CAMERA_SUB] == NULL)
        {
            return false;
        }
    }
    if (m_testItemCFG.b_LCD)
    {
        g_TestItemObj[handle_index][E_LCD] =
            new CTestItem_LCM (this, m_commonCFG.timeout[E_LCD], E_LCD, "LCD");

        if (g_TestItemObj[handle_index][E_LCD] == NULL)
        {
            return false;
        }
    }
    if (m_testItemCFG.b_TouchScr)
    {
        g_TestItemObj[handle_index][E_TOUCHSCR] =
            new CTestItem_TP (this, m_commonCFG.timeout[E_TOUCHSCR], E_TOUCHSCR, "Touch Panel");

        if (g_TestItemObj[handle_index][E_TOUCHSCR] == NULL)
        {
            return false;
        }
    }
    if (m_testItemCFG.b_RTC)
    {
        g_TestItemObj[handle_index][E_RTC] =
            new CTestItem_RTC (this, m_commonCFG.timeout[E_RTC], E_RTC, "RTC");

        if (g_TestItemObj[handle_index][E_RTC] == NULL)
        {
            return false;
        }
    }
    if (m_testItemCFG.b_HWVersion)
    {
        g_TestItemObj[handle_index][E_HWVERSIOM] =
            new CTestItem_HWVer (this, m_commonCFG.timeout[E_HWVERSIOM], E_HWVERSIOM, "HW Version");

        if (g_TestItemObj[handle_index][E_HWVERSIOM] == NULL)
        {
            return false;
        }
    }
    if (m_testItemCFG.b_er_state)
    {
        g_TestItemObj[handle_index][E_EARTHPHONE_STATE] =
            new CTestItem_ER_State(this, m_commonCFG.timeout[E_EARTHPHONE_STATE], E_EARTHPHONE_STATE, "Earthphone state");

        if (g_TestItemObj[handle_index][E_EARTHPHONE_STATE] == NULL)
        {
            return false;
        }
    }
    if (m_testItemCFG.b_Keypad)
    {
        g_TestItemObj[handle_index][E_KEYPAD] =
            new CTestItem_KPad(this, m_commonCFG.timeout[E_KEYPAD], E_KEYPAD, "Key Pad");

        if (g_TestItemObj[handle_index][E_KEYPAD] == NULL)
        {
            return false;
        }
    }
    if (m_testItemCFG.b_ADC)
    {
        g_TestItemObj[handle_index][E_ADC] =
            new CTestItem_ADC(this, m_commonCFG.timeout[E_ADC], E_ADC, "ADC");

        if (g_TestItemObj[handle_index][E_ADC] == NULL)
        {
            return false;
        }
    }
    if (m_testItemCFG.b_slpmode)
    {
        g_TestItemObj[handle_index][E_SLPMODE] =
            new CTestItem_SleepMode(this, m_commonCFG.timeout[E_SLPMODE], E_SLPMODE, "Sleep Mode");

        if (g_TestItemObj[handle_index][E_SLPMODE] == NULL)
        {
            return false;
        }
    }
    if (m_testItemCFG.b_RSSI)
    {
        g_TestItemObj[handle_index][E_RSSI] =
            new CTestItem_RSSI(this, m_commonCFG.timeout[E_RSSI], E_RSSI, "Signal Strength");

        if (g_TestItemObj[handle_index][E_RSSI] == NULL)
        {
            return false;
        }
    }
    if (m_testItemCFG.b_LED)
    {
        g_TestItemObj[handle_index][E_LED] =
            new CTestItem_LED(this, m_commonCFG.timeout[E_LED], E_LED, "LED");

        if (g_TestItemObj[handle_index][E_LED] == NULL)
        {
            return false;
        }
    }
    if (m_testItemCFG.b_EMMC)
    {
        g_TestItemObj[handle_index][E_EMMC] =
            new CTestItem_EMMC(this, m_commonCFG.timeout[E_EMMC], E_EMMC, "EMMC");

        if (g_TestItemObj[handle_index][E_EMMC] == NULL)
        {
            return false;
        }
    }
    if (m_testItemCFG.b_CMMB)
    {
        g_TestItemObj[handle_index][E_CMMB] =
            new CTestItem_CMMB(this, m_commonCFG.timeout[E_CMMB], E_CMMB, "CMMB");

        if (g_TestItemObj[handle_index][E_CMMB] == NULL)
        {
            return false;
        }
    }

    if (m_testItemCFG.b_gsensor)
    {
        g_TestItemObj[handle_index][E_GSENSOR] =
            new CTestItem_Sensor(this, m_commonCFG.timeout[E_GSENSOR], E_GSENSOR, "GSensor");

        if (g_TestItemObj[handle_index][E_GSENSOR] == NULL)
        {
            return false;
        }
    }

    if (m_testItemCFG.b_msensor)
    {
        g_TestItemObj[handle_index][E_MSENSOR] =
            new CTestItem_Sensor(this, m_commonCFG.timeout[E_MSENSOR], E_MSENSOR, "MSensor");

        if (g_TestItemObj[handle_index][E_MSENSOR] == NULL)
        {
            return false;
        }
    }

    if (m_testItemCFG.b_alsps)
    {
        g_TestItemObj[handle_index][E_ALSPS] =
            new CTestItem_Sensor(this, m_commonCFG.timeout[E_ALSPS], E_ALSPS, "ALSPS");

        if (g_TestItemObj[handle_index][E_ALSPS] == NULL)
        {
            return false;
        }
    }

    if (m_testItemCFG.b_gyroscope)
    {
        g_TestItemObj[handle_index][E_GYROSCOPE] =
            new CTestItem_Sensor(this, m_commonCFG.timeout[E_GYROSCOPE], E_GYROSCOPE, "Gyroscope");

        if (g_TestItemObj[handle_index][E_GYROSCOPE] == NULL)
        {
            return false;
        }
    }

    if (m_testItemCFG.b_micbias)
    {
        g_TestItemObj[handle_index][E_MICBIAS] =
            new CTestItem_Micbias(this, m_commonCFG.timeout[E_MICBIAS], E_MICBIAS, "Micbias");

        if (g_TestItemObj[handle_index][E_MICBIAS] == NULL)
        {
            return false;
        }
    }

	if (m_testItemCFG.b_OTG)
    {
        g_TestItemObj[handle_index][E_OTG] =
            new CTestItem_OTG(this, m_commonCFG.timeout[E_OTG], E_OTG, "OTG");

        if (g_TestItemObj[handle_index][E_OTG] == NULL)
        {
            return false;
        }
    }

	if (m_testItemCFG.b_HDMI)
    {
        g_TestItemObj[handle_index][E_HDMI] =
            new CTestItem_HDMI(this, m_commonCFG.timeout[E_HDMI], E_HDMI, "HDMI");

        if (g_TestItemObj[handle_index][E_HDMI] == NULL)
        {
            return false;
        }
    }

	if (m_testItemCFG.b_MHL)
    {
        g_TestItemObj[handle_index][E_MHL] =
            new CTestItem_HDMI(this, m_commonCFG.timeout[E_MHL], E_MHL, "MHL");

        if (g_TestItemObj[handle_index][E_MHL] == NULL)
        {
            return false;
        }
    }

	if (m_testItemCFG.b_GPSCali)
    {
        g_TestItemObj[handle_index][E_GPS_CALI] =
            new CTestItem_GPSCali (this, m_commonCFG.timeout[E_GPS_CALI], E_GPS_CALI, "GPS_CALI");

        if (g_TestItemObj[handle_index][E_GPS_CALI] == NULL)
        {
            return false;
        }
    }
#endif

    return true;
}

/**
 * Descriptions: FIXME
 */
bool ATA_DLL_Handle::DeinitTestItemObj ()
{
    for ( E_ATDLL_TEST_ITEM_COUNT test_item = E_LCD;
            test_item < E_TEST_ITEM_COUNT;
            test_item = (E_ATDLL_TEST_ITEM_COUNT)((int)test_item + 1) )
    {
        TRACE ("%d\r\n", (int)test_item);
        if (g_TestItemObj[handle_index][test_item] != NULL)
        {
            delete g_TestItemObj[handle_index][test_item];
            g_TestItemObj[handle_index][test_item] = NULL;
        }
    }

    return true;
}

/**
 * Descriptions: FIXME
 */
E_ATADLL_RESULT ATA_DLL_Handle::RunTestItemObj ()
{
#ifdef __ATA_CONCURRENT_FLOW__

    CATA_DLL_Test_Handle dll_test_handle(this);

    dll_test_handle.StartTest ();

    if (testcases_index_bg + testcases_index_fg > 0 && *(this->m_commonCFG.stop_flag) != ATA_DLL_STOP_FLAG)
    {
        Log_Record ("Start to request test result data.\r\n");
        dll_test_handle.RequestAndParseData ();
    }
	
    return E_ATADLL_RESULT_SUCCESS;

#else
    E_ATADLL_RESULT final_result = E_ATADLL_RESULT_SUCCESS;
    int timeout_times = 0;

    glb_test_item[handle_index] = E_LCD;
    for ( E_ATDLL_TEST_ITEM_COUNT test_item = E_LCD;
            test_item < E_TEST_ITEM_COUNT;
            test_item = (E_ATDLL_TEST_ITEM_COUNT)((int)test_item + 1) )
    {
        if (*(this->m_commonCFG.stop_flag) == ATA_DLL_STOP_FLAG)
        {
            break;
        }

        if (g_TestItemObj[handle_index][test_item] != NULL)
        {
            glb_test_item[handle_index] = test_item;

            E_ATADLL_RESULT test_result;
            // If need retry, loop test until retry times done.
            for (int retry_index = 1; retry_index <= m_commonCFG.retry_times+1; retry_index ++)
            {
                if (*(this->m_commonCFG.stop_flag) == ATA_DLL_STOP_FLAG)
                {
                    break;
                }

                if (retry_index > 1)
                {
                    m_commonCFG.cbTestProgess (handle_index, test_item, 0);
                    Sleep (200);
                }

                test_result = g_TestItemObj[handle_index][test_item]->ThreadFunc ();
                m_testCNF->test_result[test_item] = test_result;
				
                if (*(this->m_commonCFG.stop_flag) == ATA_DLL_STOP_FLAG)
                {
                    break;
                }

                if (test_result == E_ATADLL_RESULT_SUCCESS)
                {
                    // if test result fail, do retry ...
                    bool result = QueryResult (test_item);
                    if (result)
                    {
                        break;
                    }
                }

                if (*(this->m_commonCFG.stop_flag) == ATA_DLL_STOP_FLAG)
                {
                    break;
                }

                // if response timeout, break out.
                // because phone maybe hang.
                if (test_result == E_ATADLL_RESULT_AT_RESPONSE_TIMEOUT)
                {
                    break;
                }
            }

            if (E_ATADLL_RESULT_SUCCESS != test_result)
            {
                final_result = E_ATADLL_RESULT_FAILED;
            }

            // If timeout during consecutive 3 test items, maybe DUT break down
            if (test_result == E_ATADLL_RESULT_AT_RESPONSE_TIMEOUT)
            {
                timeout_times ++;
                if (timeout_times >= 2)
                {
                    break;
                }
            }

            if (m_commonCFG.stop_if_failed)
            {
                if (!QueryResult(test_item))
                {
                    break;
                }
            }

            Sleep (100);
        }
    }

    glb_test_item[handle_index] = E_TEST_ITEM_COUNT;
    return final_result;

#endif
}

/**
 * Descriptions: FIXME
 */
bool ATA_DLL_Handle::ConnectTarget(int initState)
{
	MTRACE(g_hEBOOT_DEBUG,"[Thread%d] Entry ConnectTarget()", ata_handle+1);
    long lCommMod = 0;
    CommModule *cm = NULL;

    lCommMod = GetCommModule(initState);
    if (lCommMod <= 0)
    {
        Log_Record ("GetCommModule failed.\r\n");
        return false;
    }
    cm = (CommModule *)lCommMod;
    cm->SetCallBack (ATACallBackOfAT);
    cm->SetHandleIndex (ata_handle);
    m_comm_handle = cm;
    Log_Record ("GetCommModule ok.\r\n");

    return true;
}

/**
* Descriptions: FIXME
*/
bool ATA_DLL_Handle::ConnectPM()
{	
	MTRACE(g_hEBOOT_DEBUG,"[Thread%d] Entry ConnectPM()", ata_handle+1);
    long lpmCommMod = 0;
    CommModule *pmcm = NULL;

    lpmCommMod = GetPMCommModule ();
    if (lpmCommMod <= 0)
    {
		Log_Record ("Get PM CommModule failed.\r\n"); 
		MTRACE_ERR(g_hEBOOT_DEBUG,"[Thread%d]Get PM CommModule failed",ata_handle+1);
		return false;
    }

    pmcm = (CommModule *)lpmCommMod;
    pmcm->SetCallBack (NULL);
    m_pm_comm_handle = pmcm;
    Log_Record ("Get PM CommModule ok.\r\n");
	MTRACE(g_hEBOOT_DEBUG,"[Thread%d]Get PM CommModule ok.",ata_handle+1);

    ATResult atret;
    string atstr = "End\r\n";
    m_pm_comm_handle->WriteCommand(atstr.c_str(), atstr.length(), 100, false, atret);

    //if (m_testItemCFG.b_slpmode || m_testItemCFG.b_OTG)
	if (1)
    {
		
		atstr = "SetMA=1\r\n";
		
        atstr = "Charge=1\r\n";
        m_pm_comm_handle->WriteCommand(atstr.c_str(), atstr.length(), 100, false, atret);
		
		/*if( strstr (atret.resultLst[0].eleLst[1].str_value.c_str(), "OK") != NULL)
			{
				MTRACE(g_hEBOOT_DEBUG,"[Thread%d]send test box charge on  success.",ata_handle+1);
			
			}*/
    }

    return true;
}

/**
* Descriptions: FIXME
*/
void ATA_DLL_Handle::DisconnectPM()
{
    if ((long)m_pm_comm_handle > 0)
    {
        Log_Record ("Release PM comm handle.\r\n");
        ClosePMModule();
        ATDeInitCommPort ((long)m_pm_comm_handle);
        m_pm_comm_handle = 0;
    }
}

/**
* Descriptions: FIXME
*/
void ATA_DLL_Handle::DisconnectTarget()
{
    Log_Record ("Release target comm handle.\r\n");
    ReleaseCommModule (m_comm_handle);
    m_comm_handle = 0;
}

/**
 * Descriptions: FIXME
 */
E_ATADLL_RESULT ATA_DLL_Handle::RunTest(int ata_handle)
{
    E_ATADLL_RESULT test_result = E_ATADLL_RESULT_SUCCESS;
    CommModule *cm = NULL;
    CommModule *pmcm = NULL;
    long lCommMod = 0;
    long lpmCommMod = 0;

    E_ATDLL_TEST_ITEM_COUNT test_item = E_LCD;
    double totalTimeCost = 0.0;
    this->ata_handle = ata_handle;

    // Create AT command log file handle
    if (m_fDLLLogFile == NULL)
    {
        char dllLog[128];
        sprintf (dllLog, "C:\\ATA_DLL_DUT%d.log", ata_handle+1);
        this->m_fDLLLogFile = fopen(dllLog, "at+");
    }
    if(!m_commonCFG.isStartSend)
    {
	// Get communication module handle for PM
#ifndef INTERNEL_DEBUG_VERSION
    if (m_testItemCFG.b_Vibrator || m_testItemCFG.b_LED || m_testItemCFG.b_ADC ||
            (m_testItemCFG.b_Keypad && m_commonCFG.kpad_cfg.bExterConnection) ||
            (m_testItemCFG.b_Keypad && m_commonCFG.kpad_cfg.bElectroMagicSwitch) ||
            m_testItemCFG.b_micbias || m_testItemCFG.b_slpmode || m_testItemCFG.b_offMode || 
            m_testItemCFG.b_OTG)
    {
        if (!ConnectPM()) 
        	{
        		test_result_code=E_ATADLL_CONNECT_PM_FAIL;
				goto Error;
        	}

    }
#endif

    // Get communication module handle for target
    if (!ConnectTarget(1))
    	{
    		test_result_code=E_ATADLL_CONNECT_PHONE_FAIL;
        	goto Error;
    	}

    // Send start command to start test
    Log_Record ("Send Start Command (AT+START) start.\r\n");
    if (!SendStartCommand ())
    {
        //ReleaseCommModule (m_comm_handle);
        //m_comm_handle = 0;
        Log_Record ("Send Start Command (AT+START) fail.\r\n");
        return E_ATADLL_START_SYNC_FAIL;//E_ATADLL_RESULT_FAILED;
    }
    Log_Record ("Send Start Command (AT+START) ok.\r\n");

//[NEW] Mingxue add Get flag sequency from target
    
	if(!RequestFlagFromTarget(&g_flag_num))
	{
	  Log_Record ("Request flag from targt fail.Maybe not support in this sw.Using default one...\r\n");
	  g_ReadTargetFlag=false;
	}
	else
	{
		if(ConvertToPCItemSequence(g_flag_num))
			{
				g_ReadTargetFlag=true;
				Log_Record("Get and parse target side's flag sequence successful!Using flag sequence get from target\r\n");
			}
		else
			{
				g_ReadTargetFlag=false;
				Log_Record ("Covert flag sequence failed.Using the default one\r\n");
			}
			
	}

//	[NEW] Mingxue add Get flag sequency from target--end

    // update preprocess call back function
    m_commonCFG.cbTestPreProcess(ata_handle);

    // Sleep before start test
    Log_Record ("Sleep %d second before test.\r\n", m_commonCFG.waitSecBeforeTest);
    Sleep (m_commonCFG.waitSecBeforeTest * 1000);

    // Init test object
    Log_Record ("Init test item object.\r\n");
    if (!InitTestItemObj())
    {
        Log_Record ("Init test item object fail.\r\n");
		test_result_code=E_ATADLL_TESTITEM_INIT_FAIL;
        goto Error;
    }
    }

    // run test
    if (!m_commonCFG.b_step_debug)
    {
        Log_Record ("Start Testing items:\r\n\r\n");
        test_result_code = RunTestItemObj ();
        if (E_ATADLL_RESULT_SUCCESS != test_result_code)
        {
            goto Error;
        }
    }

    // Wait 300 mill second for stability
    Sleep (300);

    // Deinit test object
    /**
    Log_Record ("DeInit test item object.\r\n");
    DeinitTestItemObj ();
    **/

    return E_ATADLL_RESULT_SUCCESS;

Error:
	
	Log_Record("Test Error:%s,Error code is %d\r\n",AtaStatusToString(test_result_code),test_result_code);
    return test_result_code;
}

/**
 * Descriptions: FIXME
 */
E_ATADLL_RESULT ATA_DLL_Handle::RunTestStep(E_ATDLL_TEST_ITEM_COUNT test_item)
{
    CATA_DLL_Test_Handle dll_test_handle(this);
    dll_test_handle.StartTestStep (test_item);

    if (testcases_index_bg + testcases_index_fg > 0)
    {
        Log_Record ("Start to request test result data.\r\n");
        dll_test_handle.RequestAndParseData ();
    }
    return E_ATADLL_RESULT_SUCCESS;
}


/**
 * Descriptions: FIXME
 */
E_ATADLL_RESULT ATA_DLL_Handle::StopTest(int ata_handle)
{
    // Deinit test object
    Log_Record ("DeInit test item object.\r\n");
    DeinitTestItemObj ();

    // compute total time cost
    double totalTimeCost = 0.0;
    E_ATDLL_TEST_ITEM_COUNT test_item = E_LCD;
    for (test_item = E_LCD; test_item < E_TEST_ITEM_COUNT; test_item = (E_ATDLL_TEST_ITEM_COUNT)((int)test_item + 1) )
    {
        totalTimeCost += m_testCNF->time_cost[(int)test_item];
    }
    Log_Record ("Total time cost: %.2f second. \r\n\r\n", totalTimeCost);

	if (g_TestItemObj[handle_index][E_MHL] != NULL)
	{
		ATResult atret;
	    string atstr = "AT+HDMI";
	    atstr += CR;
		m_comm_handle->WriteCommand (atstr.c_str(), atstr.length(), 1000, false, atret);
	}
	else
	{
	    // send stop command, switch to normal factory mode
	    if ((long)m_comm_handle > 0 && !m_testItemCFG.b_offMode)
	    {
	        Log_Record ("Send stop command(AT+STOP) to target.\r\n");
	        SendStopCommand ();
	    }
	}

    // close target com port handle
    if ((long)m_comm_handle > 0)
    {
        DisconnectTarget ();
    }

    // close PM com port handle
#ifndef INTERNEL_DEBUG_VERSION
    if (m_testItemCFG.b_Vibrator || m_testItemCFG.b_LED || m_testItemCFG.b_ADC ||
            (m_testItemCFG.b_Keypad && m_commonCFG.kpad_cfg.bExterConnection) ||
            (m_testItemCFG.b_Keypad && m_commonCFG.kpad_cfg.bElectroMagicSwitch) ||
            m_testItemCFG.b_micbias || m_testItemCFG.b_slpmode || m_testItemCFG.b_offMode ||
            m_testItemCFG.b_OTG)
    {
        DisconnectPM ();
    }
#endif

    if (*(m_commonCFG.stop_flag) == ATA_DLL_STOP_FLAG)
    {
        Log_Record ("Stopped by user.\r\n");
    }

	//update test result to .csv log file
    //int number = fprintf (m_fpLogCsvFile, "%s", log_buff);

    fflush (m_fpLogCsvFile);
	fclose(m_fpLogCsvFile);
	
	

	//update .csv end

    // rename log file name by barcode or date info
    RenameLogFile ();
    return E_ATADLL_RESULT_SUCCESS;
}

/**
 * Descriptions: FIXME
 */
E_ATADLL_RESULT ATA_DLL_Handle::UpdateBarcodeFlag(int ata_handle, bool bPass)
{
    bool bNeedCreate = false;
    E_ATADLL_RESULT ata_result = E_ATADLL_RESULT_SUCCESS;
    Log_Record ("Update ATA test flag into Barcode start.\r\n");

    if (m_comm_handle <= 0)
    {
        Log_Record ("Target do not enter facotry mode, Skipped!\r\n");
        return E_ATADLL_RESULT_FAILED;//E_ATADLL_RESULT_WARNING_IGNORED;
    }

    if (g_TestItemObj[handle_index][E_BARCODE] == NULL)
    {
        g_TestItemObj[handle_index][E_BARCODE] =
            new CTestItem_Barcode(this, m_commonCFG.timeout[E_BARCODE], E_BARCODE, "Barcode");

        if (g_TestItemObj[handle_index][E_BARCODE] == NULL)
        {
            return E_ATADLL_RESULT_FAILED;
        }

        glb_test_item[handle_index] = E_BARCODE;
        bNeedCreate = true;
    }

    CTestItem_Barcode *testItem_barcode = (CTestItem_Barcode *)g_TestItemObj[handle_index][E_BARCODE];
	glb_test_item[handle_index] = E_BARCODE;
    ata_result = testItem_barcode->UpdateBarcodeFlag (bPass);
    if (E_ATADLL_RESULT_SUCCESS == ata_result)
    {
        Log_Record ("Update ATA test flag into Barcode OK.\r\n\r\n");
    }
    else
    {
        Log_Record ("Update ATA test flag into Barcode Fail.\r\n\r\n");
    }

    if (bNeedCreate)
    {
        delete g_TestItemObj[handle_index][E_BARCODE];
        g_TestItemObj[handle_index][E_BARCODE] = NULL;
    }
	m_commonCFG.cbTestProgess (ata_handle, E_BARCODE, 100);	
    m_commonCFG.cbTestProgess (ata_handle, E_BARCODE, 100);	
    return ata_result;
}

E_ATADLL_RESULT ATA_DLL_Handle::TestOffCurrent ()
{
    if ((long)m_pm_comm_handle <= 0)
    {
        return E_ATADLL_RESULT_FAILED;
    }

    bool bNeedCreate = false;

    if (g_TestItemObj[handle_index][E_OFFMODE] == NULL)
    {
        g_TestItemObj[handle_index][E_OFFMODE] =
            new CTestItem_SleepMode(this, m_commonCFG.timeout[E_OFFMODE], E_OFFMODE, "Off Current");

        if (g_TestItemObj[handle_index][E_OFFMODE] == NULL)
        {
            return E_ATADLL_RESULT_FAILED;
        }
        bNeedCreate = true;
    }

    CTestItem_SleepMode *testItem_sleepmode = (CTestItem_SleepMode *)g_TestItemObj[handle_index][E_OFFMODE];
/*#ifdef __ATA_CONCURRENT_FLOW__
    m_testCNF->test_result[E_OFFMODE] = testItem_sleepmode->RunTest();
#else*/
	m_testCNF->test_result[E_OFFMODE] = testItem_sleepmode->ThreadFunc ();
//#endif

    /*if (bNeedCreate)
    {
        delete g_TestItemObj[handle_index][E_OFFMODE];
        g_TestItemObj[handle_index][E_OFFMODE] = NULL;
    }*/

    return m_testCNF->test_result[E_OFFMODE];
}

void ATA_DLL_Handle::AddLog(const char *fmt, va_list arg)
{
    //WaitForSingleObject(m_hStateMutex, 2000);
    bLogWithDateInfo = false;
    Log_Record (fmt, arg);
    bLogWithDateInfo = true;

    //ReleaseMutex(m_hStateMutex);
}

void ATA_DLL_Handle::AddCSVLog(char* log_buff)
{
	fprintf (m_fpLogCsvFile, "%s", log_buff);
	fflush (m_fpLogCsvFile);
}


void ATA_DLL_Handle::Log_Record(const char *fmt, ...)
{
    SYSTEMTIME  cur_time;
	char datelogfile[256];

    //ATA_DLL_Mutex_Sentry    sentry(m_logWriteToFileMutex);
    if (m_fpLogFile == NULL)
    {
        char logDir[512];
		char logDirCsv[512];
        GetCurrentDirectory (512, logDir);
        sprintf (logDir, "%s\\log", logDir);

        // make sure the dir "log" exist
        WIN32_FIND_DATA wfd;
        HANDLE hFind = FindFirstFile (logDir, &wfd);
        if ((hFind != INVALID_HANDLE_VALUE) && (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
        {
        }
        else
        {
            CreateDirectory (logDir, NULL);
        }

        // make sure log file is not null
        if (strcmp(m_commonCFG.log_file, "") == 0)
        {
            GetLocalTime(&cur_time);
            sprintf (datelogfile, "%04d_%02d_%02d_%02d_%02d_%02d_%03d",
                     cur_time.wYear, cur_time.wMonth, cur_time.wDay,
                     cur_time.wHour, cur_time.wMinute, cur_time.wSecond, cur_time.wMilliseconds);
			sprintf(m_commonCFG.log_file,"%s.txt",datelogfile);
        }
		if (strcmp(m_commonCFG.log_csv_file, "") == 0)
		{
			strcpy(m_commonCFG.log_csv_file,datelogfile);
			sprintf(m_commonCFG.log_csv_file,"%s.csv",datelogfile);
		}
			

        // create file handle
        strcpy(logDirCsv,logDir);
        sprintf (logDir, "%s\\%s", logDir, m_commonCFG.log_file);
		
        ::DeleteFile (logDir);	// delete file with the same name
        this->m_fpLogFile = fopen(logDir, "at+");
		
		sprintf (logDirCsv, "%s\\%s", logDirCsv,m_commonCFG.log_csv_file);
		::DeleteFile (logDirCsv);
		this->m_fpLogCsvFile = fopen(logDirCsv, "a+");

        // make sure file handle created ok
        if (m_fpLogFile == NULL)
        {
            return;
        }
		
       	 // make sure file handle created ok
        if (m_fpLogCsvFile == NULL)
        {
            return;
        }

		//ItemName, TestTimes, TestResult, ErrorCode
		fprintf(m_fpLogFile,"//*************************************************************//\r\n",m_commonCFG.tool_version);
		fprintf(m_fpLogFile,"//           ATA Tool Version: %s\r\n",m_commonCFG.tool_version);
		fprintf(m_fpLogFile,"//*************************************************************//\r\n",m_commonCFG.tool_version);
		int number = fprintf (m_fpLogCsvFile, "ItemName, TestTimes, TestResult, ErrorCode\r\n");

    }
	

    // Pick out log string
    char strLog[89600];
    memset (strLog, 0x0, sizeof(char)*89600);
    va_list ap;
    va_start (ap, fmt);
    vsprintf (strLog, fmt, ap);
    va_end (ap);

    // if need add date info
    if (bLogWithDateInfo)
    
    {
        char dateinfo[256];
        memset (dateinfo, 0x0, sizeof(char)*256);
        GetLocalTime(&cur_time);
        sprintf (dateinfo, "%04d/%02d/%02d/%02d/%02d/%02d/%03d",
                 cur_time.wYear, cur_time.wMonth, cur_time.wDay,
                 cur_time.wHour, cur_time.wMinute, cur_time.wSecond, cur_time.wMilliseconds);

        char tempStr[89600];
        sprintf (tempStr, "%s %s", dateinfo, strLog);
        sprintf (strLog, "%s", tempStr);
    }

    // flush log to file
    int number = fprintf (m_fpLogFile, "%s", strLog);
    fflush (m_fpLogFile);
}

void ATA_DLL_Handle::RenameLogFile()
{
    if (strcmp(m_testCNF->barcode, "") != 0)
    {
        Log_Record ("Rename log file (%s) by barcode: %s", m_commonCFG.log_file, m_testCNF->barcode);
    }
    else
    {
        Log_Record ("Rename log file (%s) by date info.", m_commonCFG.log_file);
    }

    // Close file handle before renameing
    if (m_fpLogFile != NULL)
    {
        fclose (m_fpLogFile);
        m_fpLogFile = NULL;
    }
	  if (m_fpLogCsvFile != NULL)
    {
        fclose (m_fpLogCsvFile);
        m_fpLogCsvFile = NULL;
    }

    // Get local dir path
    char logDir[512];
    GetCurrentDirectory (512, logDir);
    sprintf (logDir, "%s\\log", logDir);

    // Old file name
    char oldFileName[512];
	char oldFileName1[512];
    sprintf (oldFileName, "%s\\%s", logDir, m_commonCFG.log_file);
	
	sprintf (oldFileName1, "%s\\%s", logDir, m_commonCFG.log_csv_file);
    // new file name
    char newFileName[512];
	 char newFileName1[512];
    if (strcmp(m_testCNF->barcode, "") != 0 /*&& strcmp(m_testCNF->barcode, "unknown") != 0*/)   // if barcode is not null, use barcode as log file name
    {
        sprintf (newFileName, "%s\\%s.txt", logDir, m_testCNF->barcode);
		sprintf (newFileName1, "%s\\%s.csv", logDir, m_testCNF->barcode);
    }
    else  	// else use date info as log file name
    {
        SYSTEMTIME  cur_time;
        GetLocalTime(&cur_time);
        char filename[256];
        sprintf (filename, "%04d_%02d_%02d_%02d_%02d_%02d_%03d",
                 cur_time.wYear, cur_time.wMonth, cur_time.wDay,
                 cur_time.wHour, cur_time.wMinute, cur_time.wSecond, cur_time.wMilliseconds);
        sprintf (newFileName, "%s\\%s.txt", logDir, filename);

        sprintf (newFileName1, "%s\\%s.csv", logDir, filename);
    }
	
    // Delete file with the same name
    ::DeleteFile (newFileName);
	::DeleteFile (newFileName1);
    // Rename
    rename (oldFileName, newFileName);
	rename (oldFileName1, newFileName1);
	//Log_Record("end of RenameLogFile()");
}


// Following is ATA_DLL_Handle_Array related

/**
 * Descriptions: FIXME
 */
ATA_DLL_Handle_Array::ATA_DLL_Handle_Array()
{
}

/**
 * Descriptions: FIXME
 */
ATA_DLL_Handle_Array::~ATA_DLL_Handle_Array()
{
}

/**
 * Descriptions: FIXME
 */
int ATA_DLL_Handle_Array::OccupyHandle(const int ata_handle)
{
    ATA_DLL_Mutex_Sentry ata_lock_sentry(m_ata_array_Lock);

    if(!IsValidIndex(ata_handle))
    {
        return 1;
    }
    if(!mATADLLHandleArray[ata_handle].IsAvailable())
    {
        return 2;
    }
    // reset
    mATADLLHandleArray[ata_handle].Reset();
    // occupy
    mATADLLHandleArray[ata_handle].Occupy();
    return 0;
}

/**
 * Descriptions: FIXME
 */
int ATA_DLL_Handle_Array::ReleaseHandle(const int ata_handle)
{
    ATA_DLL_Mutex_Sentry ata_lock_sentry(m_ata_array_Lock);

    if(!IsValidIndex(ata_handle))
    {
        return 1;
    }
    // reset
    mATADLLHandleArray[ata_handle].Reset();
    // release
    mATADLLHandleArray[ata_handle].Release();
    return 0;
}

/**
 * Descriptions: FIXME
 */
bool ATA_DLL_Handle_Array::IsActiveHandle(const int ata_handle)
{
    ATA_DLL_Mutex_Sentry ata_lock_sentry(m_ata_array_Lock);

    if(!IsValidIndex(ata_handle))
    {
        return false;
    }
    if(!mATADLLHandleArray[ata_handle].IsAvailable())
    {
        return false;
    }
    return true;
}

/**
 * Descriptions: FIXME
 */
ATA_DLL_Handle	& ATA_DLL_Handle_Array::operator [] (const int ata_handle)
{
    // you must ensure the index is correct before use this operator
    return mATADLLHandleArray[ata_handle];
}

/**
 * Descriptions: FIXME
 */
bool ATA_DLL_Handle_Array::IsValidIndex(const int index)
{
    if( 0>index || ATA_DLL_MAX_CONCURRENT_THREADS<=index )
    {
        return false;
    }
    return true;
}
/******************************************************************************/
//ConvertToPCItemSequence
//用于将从target端获取到的flag sequence(char*型)转换为enum
//对应原本的对应g_callback_flag
/*******************************************************************************/

bool ATA_DLL_Handle::ConvertToPCItemSequence (int number)
{
	Log_Record("In function ConvertToPCItemSequence(),convert flag sequence to g_convert_flag_item,total number is:%d\r\n",number);

int i=0;
while(i<number)//while(target_flag_sequence[i]!=0)
{
	Log_Record("target_flag_sequence[%d]= %s\r\n",i,target_flag_sequence[i]);
	if(strstr(target_flag_sequence[i],"ITEM_MAIN_CAMERA")!=NULL)
		{
   			g_convert_flag_item[i]=E_CAMERA;
			i++;
			continue;
		}
	else if(strstr(target_flag_sequence[i],"ITEM_MAIN2_CAMERA")!=NULL)
		{
			g_convert_flag_item[i]=E_CAMERA_MAIN2;
			i++;
			continue;
		}
	else if(strstr(target_flag_sequence[i],"ITEM_SUB_CAMERA")!=NULL)
		{
			g_convert_flag_item[i]=E_CAMERA_SUB;
			i++;
			continue;
		}
	else if(strstr(target_flag_sequence[i],"ITEM_GPS")!=NULL)
		{
			g_convert_flag_item[i]=E_GPS;
			i++;
			continue;
		}
	else if(strstr(target_flag_sequence[i],"ITEM_FM")!=NULL)
		{
			g_convert_flag_item[i]=E_FM;
			i++;
			continue;
		}
	else if(strstr(target_flag_sequence[i],"ITEM_FLASH")!=NULL)
		{
			g_convert_flag_item[i]=E_FLASH;
			i++;
			continue;
		}
	else if(strstr(target_flag_sequence[i],"ITEM_MEMCARD")!=NULL)
		{
			g_convert_flag_item[i]=E_TCARD;
			i++;
			continue;
		}
	else if(strstr(target_flag_sequence[i],"ITEM_RTC")!=NULL)
		{
			g_convert_flag_item[i]=E_RTC;
			i++;
			continue;
		}
	else if(strstr(target_flag_sequence[i],"ITEM_LCD")!=NULL || strstr(target_flag_sequence[i],"ITEM_LCM")!=NULL)
		{
			g_convert_flag_item[i]=E_LCD;
			i++;
			continue;
		}
	else if(strstr(target_flag_sequence[i],"ITEM_LED")!=NULL)
		{
			g_convert_flag_item[i]=E_LED;
			i++;
			continue;
		}
	else if(strstr(target_flag_sequence[i],"ITEM_LOOPBACK")!=NULL || strstr(target_flag_sequence[i],"ITEM_LOOPBACK_PHONEMICSPK")!=NULL || strstr(target_flag_sequence[i],"ITEM_WAVEPLAYBACK")!=NULL 
				|| strstr(target_flag_sequence[i],"ITEM_ACOUSTICLOOPBACK")!=NULL || strstr(target_flag_sequence[i],"ITEM_SPK_OC")!=NULL)
		{
			g_convert_flag_item[i]=E_SPEAKER;
			i++;
			continue;
		}
	else if(strcmp(target_flag_sequence[i],"ITEM_BT")==NULL)//避免与ITEM錩BTS判断错误
		{
			g_convert_flag_item[i]=E_BT;
			i++;
			continue;
		}
	else if(strstr(target_flag_sequence[i],"ITEM_WIFI")!=NULL)
		{
			g_convert_flag_item[i]=E_WIFI;
			i++;
			continue;
		}
	else if(strstr(target_flag_sequence[i],"ITEM_KEYS")!=NULL)
		{
			g_convert_flag_item[i]=E_KEYPAD;
			i++;
			continue;
		}
	else if(strstr(target_flag_sequence[i],"ITEM_GSENSOR")!=NULL || strstr(target_flag_sequence[i],"ITEM_GS_CALI")!=NULL)
		{
			g_convert_flag_item[i]=E_GSENSOR;
			i++;
			continue;
		}
	else if(strstr(target_flag_sequence[i],"ITEM_MSENSOR")!=NULL)
		{
			g_convert_flag_item[i]=E_MSENSOR;
			i++;
			continue;
		}
	
	else if(strstr(target_flag_sequence[i],"ITEM_ALSPS")!=NULL)
			{
				g_convert_flag_item[i]=E_ALSPS;
				i++;
				continue;
			}
	else if(strstr(target_flag_sequence[i],"ITEM_HEADSET")!=NULL)
			{
				g_convert_flag_item[i]=E_EARTHPHONE;
				i++;
				continue;
			}
	else if(strstr(target_flag_sequence[i],"ITEM_OTG")!=NULL)
			{
				g_convert_flag_item[i]=E_OTG;
				i++;
				continue;
			}
	else if(strstr(target_flag_sequence[i],"ITEM_CHARGER")!=NULL)
			{
				g_convert_flag_item[i]=E_CHARGING;
				i++;
				continue;
			}
	else if(strstr(target_flag_sequence[i],"ITEM_TOUCH")!=NULL)
			{
				g_convert_flag_item[i]=E_TOUCHSCR;
				i++;
				continue;
			}
	else if(strstr(target_flag_sequence[i],"ITEM_SIM")!=NULL)//ITEM_SIM,ITEM_SIMCARD
			{
				g_convert_flag_item[i]=E_SIM;
				i++;
				continue;
			}
	else if(strstr(target_flag_sequence[i],"ITEM_VIBRATOR")!=NULL)
			{
				g_convert_flag_item[i]=E_VIBRATOR;
				i++;
				continue;
			}
	else if(strstr(target_flag_sequence[i],"ITEM_RECEIVER")!=NULL)//ITEM_RECEIVER,ITEM_RECEIVER_DEBUG
			{
				g_convert_flag_item[i]=E_RECEIVER;
				i++;
				continue;
			}
	else if(strstr(target_flag_sequence[i],"ITEM_IDLE")!=NULL)
			{
				g_convert_flag_item[i]=E_SLPMODE;
				i++;
				continue;
			}
	
	else if(strstr(target_flag_sequence[i],"ITEM_TVOUT")!=NULL || strstr(target_flag_sequence[i],"ITEM_MATV")!=NULL)//ITEM_MATV_NORMAL,ITEM_MATV_AUTOSCAN
				{
					g_convert_flag_item[i]=E_TV;
					i++;
					continue;
				}
	else if(strstr(target_flag_sequence[i],"ITEM_VERSION")!=NULL || strstr(target_flag_sequence[i],"ITEM_CUSTOM_VERSION")!=NULL)
			{
				g_convert_flag_item[i]=E_SWVERSION;
				i++;
				continue;
			}
	else if(strstr(target_flag_sequence[i],"ITEM_BAROMETER")!=NULL)
			{
				g_convert_flag_item[i]=E_BAROMETER;
				i++;
				continue;
			}
	else if(strstr(target_flag_sequence[i],"ITEM_GYROSCOPE")!=NULL)//ITEM_GYROSCOPE,ITEM_GYROSCOPE_CALI
			{
				g_convert_flag_item[i]=E_GYROSCOPE;
				i++;
				continue;
			}
	else if(strstr(target_flag_sequence[i],"ITEM_SIGNALTEST")!=NULL ||strstr(target_flag_sequence[i],"ITEM_RF_TEST")!=NULL)
			{
				g_convert_flag_item[i]=E_RSSI;
				i++;
				continue;
			}
	else if(strstr(target_flag_sequence[i],"ITEM_CMMB")!=NULL)
			{
				g_convert_flag_item[i]=E_CMMB;
				i++;
				continue;
			}
	else if(strstr(target_flag_sequence[i],"ITEM_EMMC")!=NULL)
			{
				g_convert_flag_item[i]=E_EMMC;
				i++;
				continue;
			}
	else if(strstr(target_flag_sequence[i],"ITEM_HDMI")!=NULL)
			{
				g_convert_flag_item[i]=E_HDMI;
				i++;
				continue;
			}
	else if(strstr(target_flag_sequence[i],"ITEM_MICBIAS")!=NULL)
			{
				g_convert_flag_item[i]=E_MICBIAS;
				i++;
				continue;
			}
	else if(strstr(target_flag_sequence[i],"ITEM_CUSTOM_READBARCODE")!=NULL || strstr(target_flag_sequence[i],"ITEM_CUSTOM_WRITEBARCODE")!=NULL)
			{
				g_convert_flag_item[i]=E_BARCODE;
				i++;
				continue;
			}
	else if(strstr(target_flag_sequence[i],"ITEM_BTS")!=NULL)
			{
				g_convert_flag_item[i]=E_BTS;
				i++;
				continue;
			}
	else if(strstr(target_flag_sequence[i],"ITEM_EXT_BUCK")!=NULL)
			{
				g_convert_flag_item[i]=E_EXT_BUCK;
				i++;
				continue;
			}
	else if(strstr(target_flag_sequence[i],"ITEM_USB_TYPEC")!=NULL)
			{
				g_convert_flag_item[i]=E_USBTYPEC;
				i++;
				continue;
			}
	else if(strstr(target_flag_sequence[i],"ITEM_FINGERPRINT_TEST")!=NULL)
			{
				g_convert_flag_item[i]=E_FINGERPRINT;
				i++;
				continue;
			}
	else if(strstr(target_flag_sequence[i],"ITEM_HUMIDITY")!=NULL)
			{
				g_convert_flag_item[i]=E_HUMIDITY;
				i++;
				continue;
			}
	else if(strstr(target_flag_sequence[i],"ITEM_GPS_CALI")!=NULL)
			{
				g_convert_flag_item[i]=E_GPS_CALI;
				i++;
				continue;
			}
	else//如果没有定义E_XXXX代表没有加入此测试项
		{
			g_convert_flag_item[i]=E_TEST_ITEM_COUNT;
			Log_Record("%s not defined test item in this tool,convert to E_TEST_ITEM_COUNT.\r\n",target_flag_sequence[i]);
				i++;
				continue;
		}
	Log_Record("g_convert_flag_item is : \n");
	for(int j=0;j<i;j++)
		{
		Log_Record("%d,",g_convert_flag_item[j]);
		}
}	
return true;
	
}


