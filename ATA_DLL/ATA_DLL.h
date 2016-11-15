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
 *   ATA_DLL.h
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

#ifndef __ATA_DLL_H__
#define __ATA_DLL_H__

#ifdef __cplusplus
extern "C" {
#endif

#define ATA_DLL_STOP_FLAG       0x9876
#define MAX_LED_SUPPORT_NUM     32
#define MAX_TEST_CASE_NUM   64
//#define MAX_LOG_BUF_SIZE        512*30
#define ENUM_TO_CASE_STRING(case_id)\
    case case_id:\
	return #case_id;


#define INTERNEL_DEBUG_VERSION
#define __ATA_CONCURRENT_FLOW__
#define __ATA_LOAD30_TEST20__
#define __ATA20__
//#define __ATA30__
//#define __ATA_MT6795_SMT__

    typedef struct
    {
        bool	b_LCD;
        bool    b_slpmode;
        bool	b_Camera;
		bool    b_Camera_Main2;
        bool    b_Camera_Sub;
        bool	b_Vibrator;
        bool	b_Charging;
        bool	b_Keypad;
        bool	b_TouchScr;
        bool    b_CMMB;
        bool	b_RTC;
        bool    b_ADC;
        bool    b_LED;

        bool	b_Ring;

        bool	b_er_state;

        bool	b_WIFI;
        bool	b_GPS;
        bool	b_BT;
        bool	b_TV;

        bool	b_TCard;
        bool    b_EMMC;
        bool    b_Flash;
        bool	b_SIM;
        bool	b_Barcode;
        bool	b_SWVersion;
        bool	b_HWVersion;

        bool	b_Speaker;
        bool	b_Receiver;
        bool	b_EarchPhone;
        bool	b_FM;

        bool    b_gsensor;
        bool    b_msensor;
        bool    b_alsps;
        bool    b_gyroscope;

        bool    b_micbias;
        bool    b_OTG;
        bool    b_HDMI;
        bool    b_MHL;
        bool    b_RSSI;
        bool    b_offMode;
		bool    b_UsbTypeC;
		bool 	b_Humidity;
		bool	b_Barometer;
		bool 	b_BTS;
		bool	b_ExtBuck;
		bool	b_FingerPrint;
		bool	b_GPSCali;	//add by songjian 2016-3-1
    } S_ATADLL_TEST_ITEM_T;

	//extern E_ATADLL_RESULT glbErrCode;
    typedef enum
{
	E_LCD = 0,

    E_RECEIVER,
    E_SPEAKER,
	E_EARTHPHONE,
	E_MICBIAS,
	E_RSSI,
	
	E_CAMERA,
	E_CAMERA_SUB,
	E_TOUCHSCR,
    
	E_VIBRATOR,
	E_KEYPAD,
	E_CMMB,
	E_RTC,
	E_ADC,
	E_LED,

	E_RING,
	E_EARTHPHONE_STATE,

	E_WIFI,
	E_GPS,
	E_BT,
	E_TV,

	E_TCARD,
	E_EMMC,
	E_FLASH,
	E_SIM,
	E_BARCODE,
	E_SWVERSION,
	E_HWVERSIOM,
	
    E_FM,
    E_CHARGING,

    E_GSENSOR,
    E_MSENSOR,
    E_ALSPS,
    E_GYROSCOPE,

    E_OTG,
    E_HDMI,
    E_MHL,
	E_USBTYPEC,

    E_SLPMODE,
    E_OFFMODE,
	E_CAMERA_MAIN2,
	E_FINGERPRINT,
    E_HUMIDITY,
    E_BTS,
    E_EXT_BUCK,
    E_BAROMETER,

	E_GPS_CALI,

	E_TEST_ITEM_COUNT  
} E_ATDLL_TEST_ITEM_COUNT;

    typedef enum
    {
        ITEM_STATE_READY = 0,
        ITEM_STATE_RUNNING,
        ITEM_STATE_FINISHED,
        ITEM_STATE_UI_UPDATE
    } E_ATADLL_ITEM_STATE;

    typedef struct
    {
        E_ATDLL_TEST_ITEM_COUNT item;
        char    item_name[64];
        bool    b_auto;
        int     testcase_seq;
        E_ATADLL_ITEM_STATE item_state;
    } S_ATADLL_TESTCASE_T;

    typedef struct
    {
        E_ATDLL_TEST_ITEM_COUNT item[2];
    } S_ATADLL_TESTCASE_CONFLICT;

// ftm struct start
    typedef struct
    {
        int fm_rssi;
#ifdef __ATA_LOAD30_TEST20__
        int freq;
        int ampl;
#endif
    } ftm_ata_fm;

    typedef struct
    {
        char wifi_mac[33];
        char wifi_name[32];
        unsigned int wifi_rssi;
        int channel;
        int rate;
    } ftm_ata_wifi;

    typedef struct
    {
        char bt_mac[32];
        char bt_name[32];
        unsigned int bt_rssi;
    } ftm_ata_bt;

    typedef struct
    {
        char modem_ver[128];
        char sw_ver[128];
        //char barcode[128];
    } ftm_ata_version;

    typedef struct
    {
        int num;
        ftm_ata_bt bt[10];
    } ftm_ata_bt_num;

    typedef struct
    {
        float ratio;
        int offset;
        float drift;
        int mean;
        int sigma;
        int update_hz;
        int bitsync;
        int acquision;
        int svid;
    } ftm_ata_gps;


    typedef struct
    {
        int freqL;
        int amplL;
        int freqR;
        int amplR;
    } ftm_ata_speaker;

    typedef struct
    {
        int freqL;
        int amplL;
        int freqR;
        int amplR;
    } ftm_ata_receiver;

    typedef struct
    {
        int freqL;
        int amplL;
        int freqR;
        int amplR;
#ifdef __ATA_LOAD30_TEST20__
        int hds_state;
        int hds_mic_state;
#endif
    } ftm_ata_headset;

    typedef struct
    {
        int current;
        int voltage;
#ifdef __ATA_LOAD30_TEST20__
        int vbattemp;
#endif
    } ftm_ata_battery;

    typedef struct
    {
        int signal_level;
    } ftm_ata_signaltest;

    typedef struct
    {
        float g_sensor_x;
        float g_sensor_y;
        float g_sensor_z;
        int accuracy;
    } ftm_ata_gsensor;

    typedef struct
    {
        int m_sensor_x;
        int m_sensor_y;
        int m_sensor_z;
        int accuracy;
    } ftm_ata_msensor;

    typedef struct
    {
        int als;
        int ps;
    } ftm_ata_alsps;

    typedef struct
    {
        float gyroscope_x;
        float gyroscope_y;
        float gyroscope_z;
        int accuracy;
    } ftm_ata_gyroscope;

    typedef struct
    {
        int total_block;
        int current_block;
        int block_size;
        char camera_data[1024*150];
    } ftm_ata_camera_data;


    typedef struct
    {
        ftm_ata_fm fm;
        ftm_ata_wifi wifi;
        ftm_ata_bt_num bt;
        ftm_ata_version version;
        ftm_ata_gps gps;
        ftm_ata_speaker speaker;
        ftm_ata_receiver receiver;
        ftm_ata_headset headset;
        ftm_ata_battery battery;
        //ftm_ata_signaltest signal;

        ftm_ata_gsensor gsensor;
        ftm_ata_msensor msensor;
        ftm_ata_alsps alsps;
        ftm_ata_gyroscope gyroscope;
    } sp_ata_data_old;

    typedef struct
    {
        int freq;
        int ampl;
    } ftm_ata_vibrator;

    typedef struct
    {
        ftm_ata_fm fm;
        ftm_ata_wifi wifi;
        ftm_ata_bt_num bt;
        ftm_ata_version version;
        ftm_ata_gps gps;
        ftm_ata_speaker speaker;
        ftm_ata_receiver receiver;
        ftm_ata_headset headset;
        ftm_ata_headset headsetL;
        ftm_ata_headset headsetR;
        ftm_ata_battery battery;
        //ftm_ata_signaltest signal;

        ftm_ata_gsensor gsensor;
        ftm_ata_msensor msensor;
        ftm_ata_alsps alsps;
        ftm_ata_gyroscope gyroscope;
        ftm_ata_vibrator vibrator;
    } sp_ata_data_old_ex;

    typedef struct{
    float mean;
    float deviation;
    float max;
    float min;
    } ftm_ata_aud_perfromance;

    typedef struct{
        ftm_ata_aud_perfromance thd;
    } ftm_ata_thd;

    typedef struct{
        ftm_ata_aud_perfromance freqresponse[5];
    } ftm_ata_freq_response;

    typedef struct
    {
        char imei[5][32];
        int number;
    } ftm_ata_imei;

    typedef struct
    {
        unsigned int sd1_total_size;
        unsigned int sd1_free_size;
        unsigned int sd2_total_size;
        unsigned int sd2_free_size;
    } ftm_ata_memcard;

    typedef struct
    {
        float capacity;
    } ftm_ata_emmc;

    typedef struct
    {
        int rf_data;
        int rf_data_lte;
    } ftm_ata_rf;

	typedef struct
	{
	   char  bts_sensor_name[32];
	   float bts_temperature;
	} ftm_ata_bts;
	
	typedef struct
    {
        int num;
        ftm_ata_bts bts[5];
    } ftm_ata_bts_num;

    typedef struct
    {
        ftm_ata_fm fm;
        ftm_ata_wifi wifi;
        ftm_ata_bt_num bt;
        ftm_ata_version version;
        ftm_ata_gps gps;
        ftm_ata_speaker speaker;
        ftm_ata_receiver receiver;
        ftm_ata_headset headset;
        ftm_ata_headset headsetL;
        ftm_ata_headset headsetR;
        ftm_ata_battery battery;
        ftm_ata_gsensor gsensor;
        ftm_ata_msensor msensor;
        ftm_ata_alsps alsps;
        ftm_ata_gyroscope gyroscope;
        ftm_ata_vibrator vibrator;
#ifdef __ATA_LOAD30_TEST20__
        ftm_ata_freq_response rcv_response;
        ftm_ata_freq_response spk_response;
        ftm_ata_thd rcv_thd;
        ftm_ata_thd spk_thd;
        ftm_ata_thd headsetL_thd;
        ftm_ata_thd headsetR_thd;
#endif
        //ftm_ata_imei imei;
        ftm_ata_memcard memcard;
        ftm_ata_emmc emmc;
        
#ifdef __ATA_LOAD30_TEST20__
        ftm_ata_rf rf;
		ftm_ata_bts_num bts;
#endif

    } sp_ata_data;




// ftm struct end

    typedef struct
    {
        int comport;

        double vol1;
        double vol2;
        double adc1;
        double adc2;
        double slope;
        double offset;
    } S_ATADLL_PM_COM_INFO;


    typedef struct
    {
        unsigned int		com_port;
        unsigned int        preloader_com_port;
        unsigned int        kernel_com_port;
        bool    usb_enable;
        bool    bIsComposite;

        char    psu_gpib_addr[128];

        int		flow_control;
        int		baud_rate;

        S_ATADLL_PM_COM_INFO pm_info;
    } S_ATADLL_COMPORT;

    typedef struct
    {
        int row[64];
        int col[64];
        int num;        // test case number

        bool    bExterConnection;
        bool    bElectroMagicSwitch;
    } S_ATADLL_KPAD_CFG;

    typedef struct
    {
        double v1;
        double v2;
    } S_ATADLL_ADC_CFG;

    typedef struct
    {
        bool bX;
        bool bY;
        bool bZ;

        bool bGSensorValueDiff;
    } S_ATADLL_GSENSOR_CFG;

    typedef struct
    {
        bool bAls;
        bool bPs;
    } S_ATADLL_ALSPS_CFG;

    typedef struct
    {
        bool    bSetFlag;
        int     flagIndex;
        char    passFlag[64];
        char    failFlag[64];
    } S_ATADLL_BARCODE_FLAG_CFG;

    typedef struct
    {
        bool    bOnBoard;
    } S_ATADLL_VIBRATOR_CFG;

    typedef struct
    {
        bool    bWIFIMacCheck;
    } S_ATADLL_WIFI_CFG;

	/*zishuo20150727*/
    typedef struct
    {
		bool 	bBtAddrCheck;
    } S_ATADLL_BT_CFG;
/*end*/	
    typedef struct
    {
        bool    bCheckSum;
    } S_ATADLL_CAMERA_CFG;


    typedef void (__stdcall *CallbackTestPregoress)(int handle_index, E_ATDLL_TEST_ITEM_COUNT test_item, int percent);
    typedef void (__stdcall *CallbackPreProcess)(int handle_index);
    typedef void (__stdcall *CallbackQueryTestItemResult)(int handle_index, E_ATDLL_TEST_ITEM_COUNT test_item, bool *result);
    typedef void (__stdcall *CallbackUpdateTestResultToUI)(int handle_index, E_ATDLL_TEST_ITEM_COUNT test_item);

    typedef struct
    {
        char   log_file[256];
		char   log_csv_file[256];
        char    logonPasswrd[64];
		char   tool_version[64];

        S_ATADLL_COMPORT com_port_info;

        int     timeout[E_TEST_ITEM_COUNT];

        int     retry_times;
        bool    auto_polling;
        bool    b_need_polling; // if it is the first time test, no need polling
        bool    b_step_debug;

        // GPS
        int     gpsSVID;

        // FM
        int		fm_freq;

        // MATV
        long	matv_freq;
        int		matv_country;

        // Camera
        char	goldenFilePath[512];


        // SIM Dual ?
        int		iSimNum;
		bool    isStartSend;//mingxue 20150409

        S_ATADLL_KPAD_CFG   kpad_cfg;
        S_ATADLL_ADC_CFG    adc_cfg;
        S_ATADLL_GSENSOR_CFG        gsensor_cfg;
        S_ATADLL_ALSPS_CFG          alsps_cfg;
        S_ATADLL_BARCODE_FLAG_CFG   barcodeFlag_cfg;
        S_ATADLL_VIBRATOR_CFG       vibrator_cfg;
        S_ATADLL_WIFI_CFG           wifi_cfg;
		S_ATADLL_BT_CFG				bt_cfg;    /*zishuo20150727*/
        S_ATADLL_CAMERA_CFG         camera_cfg;

        int     bt_spec_type;  // 0 :  search all bt addr
        // 1 : search any bt addr then return

        int		*stop_flag;
        bool    stop_if_failed;
        int		waitSecBeforeTest;

        CallbackPreProcess          cbTestPreProcess;
        CallbackTestPregoress       cbTestProgess;
        CallbackQueryTestItemResult cbQueryTestItemResult;
        CallbackUpdateTestResultToUI    cbUpdateResult;
    } S_ATADLL_COMMON_CFG_T;


    /*************************
        Return Value
    **************************/
    typedef struct
    {
        float   m_UpdateHz;
        int     m_Acquisition;
        int     m_BitSync;
        float   m_Phase;
        float   m_TCXO_Offset;
        float   m_TCXO_Drift;
        float   m_CNR_Mean;
        float   m_CNR_Sigma;

        bool    bPass;
    } S_ATADLL_GPS_CNF;

    typedef struct
    {
        char	bssid[64];
        char	ssid[64];
        unsigned int	ssid_len;
        int		rssi;
        unsigned int	channel;
        int		max_rate;
    } S_ATADLL_WIFI_AP;

#define	MAX_AP_LIST	20
    typedef struct
    {
        char			main_mac[64];
        unsigned int	main_mac_len;

        S_ATADLL_WIFI_AP	ap_list[MAX_AP_LIST];
        unsigned int		ap_list_num;

        bool            bPass;
        ftm_ata_wifi    ftm_wifi;
    } S_ATADLL_WIFI_CNF;

    typedef struct
    {
        char	addr[32];
        char	device_name[256];
        int		rssi;
    } S_ATADLL_BT_DEVICE;

#define MAX_BT_LIST 20
    typedef struct
    {
		char			bt_addr[64];   /*zishuo20150727*/
        unsigned int	bt_addr_len;  /*zishuo20150727*/
        S_ATADLL_BT_DEVICE	device_list[MAX_BT_LIST];
        unsigned int		device_list_num;

        bool                bPass;
    } S_ATADLL_BT_CNF;

    typedef struct
    {
        char	fm_chip[32];
        int		freq;
        int		cqi;
    } S_ATADLL_FM_CNF;

    typedef struct
    {
        char	matv_chip[64];
        bool	freq_lock_status;
        long	freq;
        int		country;

        bool    bPass;
    } S_ATADLL_MATV_CNF;

    typedef struct
    {
        int		speakerLevel;
        int		receiverLevel;
        int		earthphoneLevel;

        ftm_ata_speaker     speaker_data;
        ftm_ata_headset     headset_data;
        ftm_ata_receiver    receiver_data;
        ftm_ata_headset     headset_data_L;
        ftm_ata_headset     headset_data_R;

        bool    bSpeakerPass;
        bool    bReceiverPass;
        bool    bHeadsetPass;
    } S_ATADLL_AUDIO_CNF;

    typedef struct
    {
        bool    b_mainCamera;
        bool    b_subCamera;
		bool    b_main2Camera;

        char    mainCameraCheckSum[128];
		char    main2CameraCheckSum[128];
        char    subCameraCheckSum[128];
    } S_ATADLL_CAMERA_CNF;

    typedef struct
    {
        double	d_voltage_before;
        double	d_current_before;

        double	d_voltage_after;
        double	d_current_after;

        bool    bPass;
    } S_ATADLL_VIBRATOR_CNF;

    typedef struct
    {
        float	d_voltage;
        int		gpio;
    } S_ATADLL_HWVERSION_CNF;

#define ATA_DLL_SIM_NUM_MAX		4
    typedef struct
    {
        bool	b_detected[ATA_DLL_SIM_NUM_MAX];
        int		sim_num;

        bool    bPass;
    } S_ATADLL_SIM_CNF;

    typedef struct
    {
        bool    result;
    } S_ATADLL_KPAD_CNF;

    typedef struct
    {
        double psu_v1;

        double meas_v1;
        double meas_v2;

        double meas_charger_current;
        bool   result;

        int    vbattemp;
    } S_ATADLL_ADC_CNF;

    typedef struct
    {
        double current;
    } S_ATADLL_SLEEPMODE_CNF;

    typedef struct
    {
        int     dBm;
        int     dBm_lte;
        bool    bPass;
    } S_ATADLL_RSSI_CNF;

    typedef struct
    {
        bool result;
        ftm_ata_emmc emmc;
    } S_ATADLL_EMMC_CNF;;

    typedef struct
    {
        bool result;
    } S_ATADLL_FLASH_CNF;

    typedef struct
    {
        char modem_ver[256];
        char ap_ver[256];
    } S_ATADLL_SWVER_CNF;

    typedef struct
    {
        bool    bPass;
    } S_ATADLL_CMMB_CNF;

    typedef struct
    {
        char state[MAX_LED_SUPPORT_NUM];
    } S_ATADLL_LED_CNF;

    typedef struct
    {
        ftm_ata_gsensor gsensor;
        ftm_ata_msensor msensor;
        ftm_ata_alsps alsps;
        ftm_ata_gyroscope gyroscope;
    } S_ATADLL_SENSOR_STATE;

    typedef struct
    {
        double  d_vMicbias0;
        double  d_vMicbias1;
    } S_ATADLL_MICBIAS_CNF;

    typedef struct
    {
        double current;
    } S_ATADLL_OFFMODE_CNF;

    typedef struct
    {
        ftm_ata_imei imei;
    } S_ATADLL_IMEI_CNF;

    typedef struct
    {
        ftm_ata_memcard memcard;
    } S_ATADLL_MEMCARD_CNF;

    typedef struct
    {
        double d_OTGVol;
    } S_ATADLL_OTG_CNF;

    typedef struct
    {
        bool bResult;
    } S_ATADLL_HDMI_CNF;

	typedef struct
	{
	  char   bts_sensor_name[32];
	  float  bts_temperature;
	} S_ATADLL_BTS_CNF;
	
	typedef struct
	{ 
	  int num; 
	  S_ATADLL_BTS_CNF bts[5];
	}S_ATADLL_BTS_NUM_CNF;

    typedef enum
    {
        E_ATADLL_RESULT_SUCCESS = 0
        ,E_ATADLL_RESULT_FAILED
        ,E_ATADLL_RESULT_BOX_FAILED
        ,E_ATADLL_RESULT_FORMAT_INCORRECT

        ,E_ATADLL_RESULT_NO_ENOUGH_HANDLE
        ,E_ATADLL_RESULT_INVALID_HANDLE_INDEX

        ,E_ATADLL_RESULT_OPEN_COMPORT_FAIL
        ,E_ATADLL_RESULT_WAIT_POWERON_TIMEOUT
        ,E_ATADLL_RESULT_AT_RESPONSE_TIMEOUT

        ,E_ATADLL_RESULT_PSU_INIT_FAIL
        ,E_ATADLL_RESULT_PSU_OPERATE_FAIL
        ,E_ATADLL_RESULT_PSU_DEINIT_FAIL

        ,E_ATADLL_RESULT_GPS_TIMEOUT
        ,E_ATADLL_RESULT_NOT_SUPPORT
        ,E_ATADLL_RESULT_NOT_TEST
        ,E_ATADLL_RESULT_WARNING_IGNORED

		,E_ATADLL_CONNECT_PM_FAIL

		,E_ATADLL_CONNECT_PHONE_FAIL
		,E_ATADLL_START_SYNC_FAIL
		,E_ATADLL_REQUEST_DATA_FAIL
		,E_ATADLL_TESTITEM_INIT_FAIL
		,E_ATADLL_RESULT_SPEC_CHECK_FAIL
		,E_ATADLL_RESULT_HDMI_NO_KEY
		,E_ATADLL_RESULT_BARCODE_FLAG_OK
		,E_ATADLL_RESULT_BARCODE_FLAG_FAIL
		
        ,E_ATADLL_RESULT_NULL

    } E_ATADLL_RESULT;

    typedef struct
    {
        bool		bMSDCOK;  // OK    :	true
        // Error :	false

        bool        bSIMDetected; // Yes :      true
        // No :     false

        bool		bChrgerDetected; // Yes :      true
        // No :     false

        bool		bLCMDetected;

		bool        bLEDDetected;//mingxue 20150711 add

        bool		bTPOK;

        bool		bRTCOK;

		bool        bUSBTypeCOK;  //mingxue 20150827

		bool		bHumidityOK;   

		bool		bBarometerOK;

		bool		bBTSOK;

		bool		bExtBuckOK;

		bool		bFingerPrintOK;

		bool		bGPSCALIOK;	//add by songjian 2016-3-1

        char		SWVersion[128];
        unsigned	swv_len;

        char		barcode[128];
        unsigned	barcode_len;

        int			er_state;

		bool 		bAllPass;

        double      time_cost[E_TEST_ITEM_COUNT];

        S_ATADLL_GPS_CNF    gps_cnf;
        S_ATADLL_WIFI_CNF	wifi_cnf;
        S_ATADLL_BT_CNF		bt_cnf;
        S_ATADLL_FM_CNF		fm_cnf;
        S_ATADLL_MATV_CNF	matv_cnf;
        S_ATADLL_AUDIO_CNF	audio_cnf;
        S_ATADLL_CAMERA_CNF	camera_cnf;
        S_ATADLL_SIM_CNF	sim_cnf;
        S_ATADLL_VIBRATOR_CNF	vibrator_cnf;
        S_ATADLL_HWVERSION_CNF	hwver_cnf;
        S_ATADLL_KPAD_CNF       kpad_cnf;
        S_ATADLL_ADC_CNF        adc_cnf;
        S_ATADLL_SLEEPMODE_CNF  slpmode_cnf;
        S_ATADLL_RSSI_CNF       rssi_cnf;
        S_ATADLL_EMMC_CNF       emmc_cnf;
        S_ATADLL_FLASH_CNF      flash_cnf;
        S_ATADLL_SWVER_CNF      sw_ver_cnf;
        S_ATADLL_CMMB_CNF       cmmb_cnf;
        S_ATADLL_LED_CNF        led_cnf;
        S_ATADLL_SENSOR_STATE   sensor_cnf;
        S_ATADLL_MICBIAS_CNF    micbias_cnf;
        S_ATADLL_OFFMODE_CNF    offmode_cnf;
        S_ATADLL_IMEI_CNF       imei_cnf;
        S_ATADLL_MEMCARD_CNF    memcard_cnf;
        S_ATADLL_OTG_CNF        otg_cnf;
        S_ATADLL_HDMI_CNF       hdmi_cnf;
		S_ATADLL_BTS_NUM_CNF        bts_cnf;

        E_ATADLL_RESULT		test_result[E_TEST_ITEM_COUNT];
    } S_ATADLL_TEST_CNF;

// Spec Definition
    typedef struct
    {
        int		min_speaker_level;
        int		min_receiver_level;
        int		min_earthphone_level;

        ftm_ata_speaker     max_speaker_data;
        ftm_ata_headset     max_headset_data;
        ftm_ata_receiver    max_receiver_data;

        ftm_ata_speaker     min_speaker_data;
        ftm_ata_headset     min_headset_data;
        ftm_ata_receiver    min_receiver_data;
    } S_ATADLL_AUDIO_SPEC;

    typedef struct
    {
        char	local_ssid[64];
        char	local_bssid[128];

        S_ATADLL_WIFI_AP	ap_list[MAX_AP_LIST];
        unsigned int		ap_list_num;
    } S_ATADLL_WIFI_SPEC;

    typedef struct
    {
        char	addr[32];
        int		rssi;
        int     specType;  // 0 - If only limited bt addr is detected  
        char	local_bssid[32];  /*zishuo20150727*/
        // 1 - If any BT addr is detected
    } S_ATADLL_BT_SPEC;

	typedef struct
    {
        
        float		soc_temp_min;
        float       soc_temp_max;
		float		mdpa_temp_min;
		float		mdpa_temp_max;
    } S_ATADLL_BTS_SPEC;
		
    typedef struct
    {
        char	fm_chip[32];
        int		min_cqi;
    } S_ATADLL_FM_SPEC;

    typedef struct
    {
        float	phase_ratio;
        float	tcxo_offset;
        float	tcxo_drift;
        float	cnr_mean;
        float	cnr_sigma;
        float	update_HZ;

        int		bitsync;
        int		acquision;

    } S_ATADLL_GPS_SPEC;

    typedef struct
    {
        char	sensor_id[32];
        float	max_fail_rate;

        char    mainCameraCheckSum[128];
        char    subCameraCheckSum[128];
		char    main2CameraCheckSum[128];
    } S_ATADLL_CAMERA_SPEC;

    typedef struct
    {
        double	max_current;
        double	min_current;
        double	min_diff;  //min difference between before and after vibrator
    } S_ATADLL_VIBRATOR_SPEC;

    typedef struct
    {
        char modem_version[256];
        char ap_version[256];
    } S_ATADLL_SW_SPEC;

    typedef struct
    {
        float	d_voltage_min;
        float	d_voltage_max;

        int		i_gpio;
    } S_ATADLL_HWVER_SPEC;

    typedef struct
    {
        int state; // 0 ~ 3
    } S_ATADLL_ER_STATE_SPEC;

    typedef struct
    {
        double max_vol_diff;

        double charger_current_max;
        double charger_current_min;
    } S_ATADLL_ADC_SPEC;

    typedef struct
    {
        double max_current;
    } S_ATADLL_SLEEPMODE_SPEC;

    typedef struct
    {
        char barcode[84];
    } S_ATADLL_BARCODE_SPEC;

    typedef struct
    {
        int min_dBm;
        int min_dBm_lte;
    } S_ATADLL_RSSI_SPEC;

    typedef struct
    {
        char state[MAX_LED_SUPPORT_NUM];
    } S_ATADLL_LED_SPEC;

    typedef struct
    {
        ftm_ata_gsensor max_gsensor;
        ftm_ata_gsensor min_gsensor;
    } S_ATADLL_GSENSOR_SPEC;

    typedef struct
    {
        ftm_ata_msensor max_msensor;
        ftm_ata_msensor min_msensor;
    } S_ATADLL_MSENSOR_SPEC;

    typedef struct
    {
        ftm_ata_alsps max_alsps;
        ftm_ata_alsps min_alsps;
    } S_ATADLL_ALSPS_SPEC;

    typedef struct
    {
        ftm_ata_gyroscope max_gyroscope;
        ftm_ata_gyroscope min_gyroscope;
    } S_ATADLL_GYROSCOPE_SPEC;

    typedef struct
    {
        double  dBias0Min;
        double  dBias0Max;

        double  dBias1Min;
        double  dBias1Max;
    } S_ATADLL_MICBIAS_SPEC;

    typedef struct
    {
        double  d_maxCurrent;
    } S_ATADLL_OFFMODE_SPEC;

    typedef struct
    {
        double  dMinVol;
        double  dMaxVol;
    } S_ATADLL_OTG_SPEC;

    typedef struct
    {
        char	barcode[64];

        char	atv_chip[32];

        S_ATADLL_BARCODE_SPEC   barcode_spec;
        S_ATADLL_AUDIO_SPEC		audio_spec;
        S_ATADLL_WIFI_SPEC		wifi_spec;
        S_ATADLL_BT_SPEC		bt_spec;
        S_ATADLL_FM_SPEC		fm_spec;
        S_ATADLL_GPS_SPEC		gps_spec;
        S_ATADLL_CAMERA_SPEC	camera_spec;
        S_ATADLL_VIBRATOR_SPEC	vibrator_spec;
        S_ATADLL_SW_SPEC		sw_spec;
        S_ATADLL_HWVER_SPEC		hwver_spec;
        S_ATADLL_ER_STATE_SPEC	er_state_spec;
        S_ATADLL_ADC_SPEC       adc_spec;
        S_ATADLL_SLEEPMODE_SPEC slpmode_spec;
        S_ATADLL_RSSI_SPEC      rssi_spec;
        S_ATADLL_LED_SPEC       led_spec;
        S_ATADLL_GSENSOR_SPEC   gsensor_spec;
        S_ATADLL_MSENSOR_SPEC   msensor_spec;
        S_ATADLL_ALSPS_SPEC     alsps_spec;
        S_ATADLL_GYROSCOPE_SPEC gyroscope_spec;
        S_ATADLL_MICBIAS_SPEC   micbias_spec;
        S_ATADLL_OTG_SPEC       otg_spec;
        S_ATADLL_OFFMODE_SPEC   offmode_spec;
		S_ATADLL_BTS_SPEC       bts_spec;
    } S_ATADLL_TESTITEM_SPEC;



    extern int sp_meta_handle;
    E_ATADLL_RESULT __stdcall ATADLL_GetAvailbleHandle (int *ata_handle);
    E_ATADLL_RESULT __stdcall ATADLL_ReleaseHandle (const int ata_handle);
    E_ATADLL_RESULT __stdcall ATADLL_StartTest (const int ata_handle,
            const S_ATADLL_TEST_ITEM_T		*test_item_cfg,
            S_ATADLL_COMMON_CFG_T		*comm_cfg,
            S_ATADLL_TEST_CNF				*test_cnf
                                               );
    E_ATADLL_RESULT __stdcall ATADLL_StartTestStep (const int ata_handle,
            E_ATDLL_TEST_ITEM_COUNT			test_item,
            const S_ATADLL_TEST_ITEM_T		*test_item_cfg,
            S_ATADLL_COMMON_CFG_T			*comm_cfg,
            S_ATADLL_TEST_CNF				*test_cnf
                                                   );

    E_ATADLL_RESULT __stdcall ATADLL_StopTest (const int ata_handle);
    void __stdcall ATADLL_AddLog (const int ata_handle, char * logstr);
    E_ATADLL_RESULT __stdcall ATADLL_UpdateBarcodeFlag (const int ata_handle, bool bPass);
    void __stdcall ATADLL_UpdateStopFlag (const int ata_handle);
    E_ATADLL_RESULT __stdcall ATADLL_TestOffCurrent (const int ata_handle);
    void __stdcall ATADLL_GetTestCase_BG(const int ata_handle, const S_ATADLL_TEST_ITEM_T	*test_item_cfg, S_ATADLL_TESTCASE_T *testcase, int *testcase_number);
    void __stdcall ATADLL_GetTestCase_FG(const int ata_handle, const S_ATADLL_TEST_ITEM_T	*test_item_cfg, S_ATADLL_TESTCASE_T *testcase, int *testcase_number);
	const char*__stdcall AtaStatusToString(int status);
	void __stdcall ATADLL_CSVLog(const int ata_handle,char* log_buff);


    /*****************************************************
    * exported function (END)
    *****************************************************/
#if _MSC_VER <= 1200 // MFC 6.0 or previous
    int sprintf_s(char* buf, size_t size, const char* fmt, ...);
    char* strcpy_s(char* dst, size_t size, const char* src);
    char* strcat_s(char* dst, size_t size, const char* src);
    char* strtok_s(char* _Str, const char* _Delim, char** _Context);
    char* strncpy_s(char* _Dst, size_t _SizeInBytes, const char* _Src, size_t _MaxCount);
#endif // #if _MSC_VER <= 1200 // MFC 6.0 or previous

#ifdef __cplusplus
}
#endif

#endif
