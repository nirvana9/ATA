// TestItemConfig.cpp : implementation file
//

#include "stdafx.h"
#include "ATATool.h"
#include "TestItemConfig.h"
#include "ChangePwd.h"

#include <atlbase.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTestItemConfig dialog
S_TESTITEM_LOOP_TEST_INFO loop_test_info;

CTestItemConfig::CTestItemConfig(CWnd* pParent /*=NULL*/)
    : CDialog(CTestItemConfig::IDD, pParent)
{
    //{{AFX_DATA_INIT(CTestItemConfig)
    m_bBarcode = FALSE;
    m_bBT = FALSE;
    m_bCharger = FALSE;
    m_bEarthphone = FALSE;
    m_bFM = FALSE;
    m_bGPS = FALSE;
    m_bMATV = FALSE;
    m_bReceiver = FALSE;
    m_bSIM = FALSE;
    m_bSpeaker = FALSE;
    m_bSW = FALSE;
    m_bTCard = FALSE;
    m_bVibrator = FALSE;
    m_bWIFI = FALSE;
    m_checkDUT1 = FALSE;
    m_checkDUT2 = FALSE;
    m_checkDUT3 = FALSE;
    m_checkDUT4 = FALSE;
    m_bLCM = FALSE;
    m_bTP = FALSE;
    m_bCamera = FALSE;
    m_iWaitSec = 0;
    m_bHWVer = FALSE;
    m_bRTC = FALSE;
    m_bERState = FALSE;
    m_bKPad = FALSE;
    m_bCheckADC = FALSE;
    m_bSlpmode = FALSE;
    m_iLoopTimes = 0;
    m_iRetryTimes = 0;
    m_bRSSI = FALSE;
    m_bLED = FALSE;
    m_bAutoPoll = FALSE;
    m_iKernelPort1 = 0;
    m_iKernelPort2 = 0;
    m_iKernelPort3 = 0;
    m_iKernelPort4 = 0;
    m_bEMMC = FALSE;
    m_iPreloaderPort1 = 0;
    m_iPreloaderPort2 = 0;
    m_iPreloaderPort3 = 0;
    m_iPreloaderPort4 = 0;
    m_bComposite = FALSE;
    m_bCMMB = FALSE;
    m_iEditPMPort1 = 0;
    m_iEditPMPort2 = 0;
    m_iEditPMPort3 = 0;
    m_iEditPMPort4 = 0;
    m_bGSensor = FALSE;
    m_bGyroscope = FALSE;
    m_bMSensor = FALSE;
    m_bAlsps = FALSE;
    m_bCameraSub = FALSE;
    m_bFlagBarcode = FALSE;
    m_bStopIfFailed = FALSE;
    m_bMicbias = FALSE;
    m_bOffMode = FALSE;
    m_bSelectAll1 = FALSE;
    m_bSelectAll2 = FALSE;
    m_bSelectAll3 = FALSE;
    m_bSelectAll4 = FALSE;
	m_bOTG = FALSE;
	m_bHDMI = FALSE;
	m_iTestItemTimeout = 0;
	m_bMHL = FALSE;
	m_bUsbTypeC = FALSE;
	m_bCameraMain2 = FALSE;
	m_bHumidity = FALSE;
	m_bBarometer = FALSE;
	m_bBTS = FALSE;
	m_bExtBuck = FALSE;
	m_bFingerPrint = FALSE;
	m_bSelectAll4 = FALSE;
	m_bGPSCali = FALSE;		//add by songjian 2016-3-1
	//}}AFX_DATA_INIT

    p_sComportSetting   = NULL;
    comport_setting_len = 0;
}

CTestItemConfig::~CTestItemConfig()
{

}

void CTestItemConfig::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CTestItemConfig)
    DDX_Control(pDX, IDC_COMBO_SIM_NUM, m_comboSimNum);
    DDX_Control(pDX, IDC_COMBO_FC4, m_comboFC4);
    DDX_Control(pDX, IDC_COMBO_FC3, m_comboFC3);
    DDX_Control(pDX, IDC_COMBO_FC2, m_comboFC2);
    DDX_Control(pDX, IDC_COMBO_COM4, m_comboCOM4);
    DDX_Control(pDX, IDC_COMBO_FC1, m_comboFC1);
    DDX_Control(pDX, IDC_COMBO_COM3, m_comboCOM3);
    DDX_Control(pDX, IDC_COMBO_COM2, m_comboCOM2);
    DDX_Control(pDX, IDC_COMBO_COM1, m_comboCOM1);
    DDX_Control(pDX, IDC_COMBO_BAUD4, m_comboBAUD4);
    DDX_Control(pDX, IDC_COMBO_BAUD3, m_comboBAUD3);
    DDX_Control(pDX, IDC_COMBO_BAUD2, m_comboBAUD2);
    DDX_Control(pDX, IDC_COMBO_BAUD1, m_comboBAUD1);
    DDX_Check(pDX, IDC_CHECK_BARCODE, m_bBarcode);
    DDX_Check(pDX, IDC_CHECK_BT, m_bBT);
    DDX_Check(pDX, IDC_CHECK_CHARGER, m_bCharger);
    DDX_Check(pDX, IDC_CHECK_EARTHPHONE, m_bEarthphone);
    DDX_Check(pDX, IDC_CHECK_FM, m_bFM);
    DDX_Check(pDX, IDC_CHECK_GPS, m_bGPS);
    DDX_Check(pDX, IDC_CHECK_MATV, m_bMATV);
    DDX_Check(pDX, IDC_CHECK_RECEIVER, m_bReceiver);
    DDX_Check(pDX, IDC_CHECK_SIM, m_bSIM);
    DDX_Check(pDX, IDC_CHECK_SPEAKER, m_bSpeaker);
    DDX_Check(pDX, IDC_CHECK_SW, m_bSW);
    DDX_Check(pDX, IDC_CHECK_TCARD, m_bTCard);
    DDX_Check(pDX, IDC_CHECK_VIBRATOR, m_bVibrator);
    DDX_Check(pDX, IDC_CHECK_WIFI, m_bWIFI);
    DDX_Check(pDX, IDC_CHECK_ACT1, m_checkDUT1);
    DDX_Check(pDX, IDC_CHECK_ACT2, m_checkDUT2);
    DDX_Check(pDX, IDC_CHECK_ACT3, m_checkDUT3);
    DDX_Check(pDX, IDC_CHECK_ACT4, m_checkDUT4);
    DDX_Check(pDX, IDC_CHECK_LCM, m_bLCM);
    DDX_Check(pDX, IDC_CHECK_TP, m_bTP);
    DDX_Check(pDX, IDC_CHECK_CAMERA, m_bCamera);
    DDX_Text(pDX, IDC_EDIT_WAIT_SEC, m_iWaitSec);
    DDV_MinMaxInt(pDX, m_iWaitSec, 0, 30);
    DDX_Check(pDX, IDC_CHECK_HW, m_bHWVer);
    DDX_Check(pDX, IDC_CHECK_RTC, m_bRTC);
    DDX_Check(pDX, IDC_CHECK_ER_STATE, m_bERState);
    DDX_Check(pDX, IDC_CHECK_KPAD, m_bKPad);
    DDX_Check(pDX, IDC_CHECK_ADC, m_bCheckADC);
    DDX_Check(pDX, IDC_CHECK_SLPMODE, m_bSlpmode);
    DDX_Text(pDX, IDC_EDIT_LOOP_TIMES, m_iLoopTimes);
    DDX_Text(pDX, IDC_EDIT_RETRY_TIMES, m_iRetryTimes);
    DDX_Check(pDX, IDC_CHECK_RSS, m_bRSSI);
    DDX_Check(pDX, IDC_CHECK_LED, m_bLED);
    DDX_Check(pDX, IDC_CHECK_AUTO_POLL, m_bAutoPoll);
    DDX_Text(pDX, IDC_EDIT_KERNEL_PORT1, m_iKernelPort1);
    DDX_Text(pDX, IDC_EDIT_KERNEL_PORT2, m_iKernelPort2);
    DDX_Text(pDX, IDC_EDIT_KERNEL_PORT3, m_iKernelPort3);
    DDX_Text(pDX, IDC_EDIT_KERNEL_PORT4, m_iKernelPort4);
    DDX_Check(pDX, IDC_CHECK_EMMC, m_bEMMC);
    DDX_Text(pDX, IDC_EDIT_PRELOADER_PORT1, m_iPreloaderPort1);
    DDX_Text(pDX, IDC_EDIT_PRELOADER_PORT2, m_iPreloaderPort2);
    DDX_Text(pDX, IDC_EDIT_PRELOADER_PORT3, m_iPreloaderPort3);
    DDX_Text(pDX, IDC_EDIT_PRELOADER_PORT4, m_iPreloaderPort4);
    DDX_Check(pDX, IDC_CHECK_COMPOSITE, m_bComposite);
    DDX_Check(pDX, IDC_CHECK_CMMB, m_bCMMB);
    DDX_Text(pDX, IDC_EDIT_PM_COM1, m_iEditPMPort1);
    DDX_Text(pDX, IDC_EDIT_PM_COM2, m_iEditPMPort2);
    DDX_Text(pDX, IDC_EDIT_PM_COM3, m_iEditPMPort3);
    DDX_Text(pDX, IDC_EDIT_PM_COM4, m_iEditPMPort4);
    DDX_Check(pDX, IDC_CHECK_GSENSOR, m_bGSensor);
    DDX_Check(pDX, IDC_CHECK_GYROSCOPE, m_bGyroscope);
    DDX_Check(pDX, IDC_CHECK_MSENSOR, m_bMSensor);
    DDX_Check(pDX, IDC_CHECK_ALSPS, m_bAlsps);
    DDX_Check(pDX, IDC_CHECK_CAMERA_SUB, m_bCameraSub);
    DDX_Check(pDX, IDC_CHECK_BARCODE_FLAG, m_bFlagBarcode);
    DDX_Check(pDX, IDC_CHECK_STOP_IF_FAIL, m_bStopIfFailed);
    DDX_Check(pDX, IDC_CHECK_MICBIAS, m_bMicbias);
    DDX_Check(pDX, IDC_CHECK_OFFMODE, m_bOffMode);
    DDX_Check(pDX, IDC_CHECK_SELECT_ALL1, m_bSelectAll1);
    DDX_Check(pDX, IDC_CHECK_SELECT_ALL2, m_bSelectAll2);
    DDX_Check(pDX, IDC_CHECK_SELECT_ALL3, m_bSelectAll3);
	DDX_Check(pDX, IDC_CHECK_OTG, m_bOTG);
	DDX_Check(pDX, IDC_CHECK_HDMI, m_bHDMI);
	DDX_Text(pDX, IDC_EDIT_TEST_ITEM_TIMEOUT, m_iTestItemTimeout);
	DDX_Check(pDX, IDC_CHECK_MHL, m_bMHL);
	DDX_Check(pDX, IDC_CHECK_USBTYPEC, m_bUsbTypeC);
	DDX_Check(pDX, IDC_CHECK_CAMERA2, m_bCameraMain2);
	DDX_Check(pDX, IDC_CHECK_HUMIDITY, m_bHumidity);
	DDX_Check(pDX, IDC_CHECK_BAROMETER, m_bBarometer);
	DDX_Check(pDX, IDC_CHECK_BTS, m_bBTS);
	DDX_Check(pDX, IDC_CHECK_EXT_BUCK, m_bExtBuck);
	DDX_Check(pDX, IDC_CHECK_FINGERPRINT, m_bFingerPrint);
	DDX_Check(pDX, IDC_CHECK_SELECT_ALL4, m_bSelectAll4);
	DDX_Check(pDX, IDC_GPS_Cali, m_bGPSCali); 	//add by songjian 2016-3-1
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTestItemConfig, CDialog)
    //{{AFX_MSG_MAP(CTestItemConfig)
    ON_BN_CLICKED(IDSAVE, OnSave)
    ON_BN_CLICKED(IDCHANGEPWD, OnChangepwd)
    ON_BN_CLICKED(IDC_CHECK_SELECT_ALL1, OnCheckSelectAll1)
    ON_BN_CLICKED(IDC_CHECK_SELECT_ALL2, OnCheckSelectAll2)
    ON_BN_CLICKED(IDC_CHECK_SELECT_ALL3, OnCheckSelectAll3)
	ON_BN_CLICKED(IDC_CHECK_SELECT_ALL4, OnCheckSelectAll4)
	ON_BN_CLICKED(IDC_GPS_Cali, OnGPS_Cali) 	//add by songjian 2016-3-1
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestItemConfig message handlers
void CTestItemConfig::SetTestItem (S_ATADLL_TEST_ITEM_T *pTestItem)
{
    this->p_sTestItem = pTestItem;
}

void CTestItemConfig::SetCommonCFG (S_ATADLL_COMMON_CFG_T *pCommonCFG)
{
    this->p_sCommonCFG = pCommonCFG;
}

void CTestItemConfig::SetTestItemSpec (S_ATADLL_TESTITEM_SPEC *testItemSpec)
{
    this->p_sTestItemSpec = testItemSpec;
}

void CTestItemConfig::SetComportSetting (S_COMPORT_ACTIVE_INFO *comport_setting, int length)
{
    this->p_sComportSetting     = comport_setting;
    this->comport_setting_len   =   length;
}

BOOL CTestItemConfig::OnInitDialog()
{
    CDialog::OnInitDialog();

    // TODO: Add extra initialization here
    UpdateTestItem ();

    m_pageWIFI.SetCommonCFG (p_sCommonCFG);
    m_pageWIFI.SetTestItemSpec (p_sTestItemSpec);
    m_pageBT.SetCommonCFG (p_sCommonCFG);
    m_pageBT.SetTestItemSpec (p_sTestItemSpec);
    m_pageFM.SetCommonCFG (p_sCommonCFG);
    m_pageFM.SetTestItemSpec (p_sTestItemSpec);
    //m_pageGPS.SetCommonCFG (p_sCommonCFG);
    //m_pageGPS.SetTestItemSpec (p_sTestItemSpec);
    //m_pageMATV.SetCommonCFG (p_sCommonCFG);
    //m_pageMATV.SetTestItemSpec (p_sTestItemSpec);
    m_pageAudio.SetCommonCFG (p_sCommonCFG);
    m_pageAudio.SetTestItemSpec (p_sTestItemSpec);
    m_pageVibrator.SetCommonCFG (p_sCommonCFG);
    m_pageVibrator.SetTestItemSpec (p_sTestItemSpec);
    m_pageHWVer.SetCommonCFG (p_sCommonCFG);
    m_pageHWVer.SetTestItemSpec (p_sTestItemSpec);	
	m_pageBTS.SetCommonCFG(p_sCommonCFG);
	m_pageBTS.SetTestItemSpec(p_sTestItemSpec);
//    m_pageERState.SetCommonCFG (p_sCommonCFG);
 //   m_pageERState.SetTestItemSpec (p_sTestItemSpec);
    m_pageCamera.SetCommonCFG (p_sCommonCFG);
    m_pageCamera.SetTestItemSpec (p_sTestItemSpec);
    m_pageKPad.SetCommonCFG (p_sCommonCFG);
    m_pageKPad.SetTestItemSpec (p_sTestItemSpec);
    m_pagePMCal.SetCommonCFG (p_sCommonCFG);
    m_pagePMCal.SetTestItemSpec (p_sTestItemSpec);
    m_pagePMCal.SetComportSetting (p_sComportSetting, comport_setting_len);
    m_pageLED.SetCommonCFG (p_sCommonCFG);
    m_pageLED.SetTestItemSpec (p_sTestItemSpec);
    m_pageSensor.SetCommonCFG (p_sCommonCFG);
    m_pageSensor.SetTestItemSpec (p_sTestItemSpec);
    m_pageMicbias.SetCommonCFG (p_sCommonCFG);
    m_pageMicbias.SetTestItemSpec (p_sTestItemSpec);

    m_configsheet.AddPage (&m_pageWIFI);
    m_configsheet.AddPage (&m_pageBT);
    m_configsheet.AddPage (&m_pageFM);
    //m_configsheet.AddPage (&m_pageGPS);
    //m_configsheet.AddPage (&m_pageMATV);
    m_configsheet.AddPage (&m_pageAudio);
    m_configsheet.AddPage (&m_pageSensor);
    m_configsheet.AddPage (&m_pageVibrator);
    m_configsheet.AddPage (&m_pageHWVer);
//    m_configsheet.AddPage (&m_pageERState);
	m_configsheet.AddPage (&m_pageBTS);
    m_configsheet.AddPage (&m_pageCamera);
    m_configsheet.AddPage (&m_pageKPad);
    m_configsheet.AddPage (&m_pagePMCal);
    m_configsheet.AddPage (&m_pageLED);
    m_configsheet.AddPage (&m_pageMicbias);

    m_configsheet.Create(this, WS_CHILD|WS_VISIBLE, 0);
    m_configsheet.ModifyStyleEx(0, WS_EX_CONTROLPARENT);
    m_configsheet.ModifyStyle(0, WS_TABSTOP);

    CRect rect;
    GetDlgItem(IDC_STATIC_SPEC)->GetWindowRect(&rect);
    ScreenToClient(rect);
    m_configsheet.SetWindowPos(NULL, rect.left+7, rect.top+20, 0, 0, SWP_NOZORDER|SWP_NOSIZE|SWP_NOACTIVATE);

    // Refresh page
    m_configsheet.SetActivePage (&m_pageFM);
    //m_configsheet.SetActivePage (&m_pageGPS);
    //m_configsheet.SetActivePage (&m_pageMATV);
    m_configsheet.SetActivePage (&m_pageAudio);
    m_configsheet.SetActivePage (&m_pageVibrator);
    m_configsheet.SetActivePage (&m_pageBT);
    m_configsheet.SetActivePage (&m_pageHWVer);
//    m_configsheet.SetActivePage (&m_pageERState);
	 m_configsheet.SetActivePage (&m_pageBTS);
    m_configsheet.SetActivePage (&m_pageCamera);
    m_configsheet.SetActivePage (&m_pageKPad);
    m_configsheet.SetActivePage (&m_pagePMCal);
    m_configsheet.SetActivePage (&m_pageLED);
    m_configsheet.SetActivePage (&m_pageSensor);
    m_configsheet.SetActivePage (&m_pageMicbias);
    m_configsheet.SetActivePage (&m_pageWIFI);

    // Init parameters
    m_pageWIFI.InitParameters ();
    m_pageBT.InitParameters ();
    //m_pageGPS.InitParameters ();
    m_pageAudio.InitParameters ();
    m_pageFM.InitParameters ();
    //m_pageMATV.InitParameters ();
    m_pageVibrator.InitParameters ();
    m_pageHWVer.InitParameters ();
//    m_pageERState.InitParameters ();
	 m_pageBTS.InitParameters ();
    m_pageCamera.InitParameters ();
    m_pageKPad.InitParameters();
    m_pagePMCal.InitParameters ();
    m_pageLED.InitParameters ();
    m_pageSensor.InitParameters ();
    m_pageMicbias.InitParameters ();

    // Init com port
    CUIntArray  ports;
    EnumerateSerialPorts (ports);
    CString str;
    bool bDUT1 = false;     // if find the com port which is selected last time
    bool bDUT2 = false;
    bool bDUT3 = false;
    bool bDUT4 = false;

    /**
    for (int i = 0; i < ports.GetSize(); i ++)
    {
        str.Format ("COM%d", ports[i]);

        m_comboCOM1.AddString (str);
        m_comboCOM1.SetItemData (i, ports[i]);
        if (comport_setting_len > 0)
        {
            if (p_sComportSetting[0].com_port_info.com_port == ports[i])
            {
                m_comboCOM1.SetCurSel (i);
                bDUT1 = true;
            }
        }

        m_comboCOM2.AddString (str);
        m_comboCOM2.SetItemData (i, ports[i]);
        if (comport_setting_len > 0)
        {
            if (p_sComportSetting[1].com_port_info.com_port == ports[i])
            {
                m_comboCOM2.SetCurSel (i);
                bDUT2 = true;
            }
        }

        m_comboCOM3.AddString (str);
        m_comboCOM3.SetItemData (i, ports[i]);
        if (comport_setting_len > 0)
        {
            if (p_sComportSetting[2].com_port_info.com_port == ports[i])
            {
                m_comboCOM3.SetCurSel (i);
                bDUT3 = true;
            }
        }

        m_comboCOM4.AddString (str);
        m_comboCOM4.SetItemData (i, ports[i]);
        if (comport_setting_len > 0)
        {
            if (p_sComportSetting[3].com_port_info.com_port == ports[i])
            {
                m_comboCOM4.SetCurSel (i);
                bDUT4 = true;
            }
        }
    }
    **/
    int i = 0;
    m_comboCOM1.AddString ("USB");
    m_comboCOM1.SetItemData (i, USB_PORT_NUM);
    m_comboCOM2.AddString ("USB");
    m_comboCOM2.SetItemData (i, USB_PORT_NUM);
    m_comboCOM3.AddString ("USB");
    m_comboCOM3.SetItemData (i, USB_PORT_NUM);
    m_comboCOM4.AddString ("USB");
    m_comboCOM4.SetItemData (i, USB_PORT_NUM);

    if (!bDUT1)
        m_comboCOM1.SetCurSel (0);
    if (!bDUT2)
        m_comboCOM2.SetCurSel (0);
    if (!bDUT3)
        m_comboCOM3.SetCurSel (0);
    if (!bDUT4)
        m_comboCOM4.SetCurSel (0);

    /**
    m_comboFC1.AddString ("NO");
    m_comboFC1.AddString ("HW");
    m_comboFC1.AddString ("SW");
    m_comboFC2.AddString ("NO");
    m_comboFC2.AddString ("HW");
    m_comboFC2.AddString ("SW");
    m_comboFC3.AddString ("NO");
    m_comboFC3.AddString ("HW");
    m_comboFC3.AddString ("SW");
    m_comboFC4.AddString ("NO");
    m_comboFC4.AddString ("HW");
    m_comboFC4.AddString ("SW");
    **/

    m_comboFC1.AddString ("NO");
    m_comboFC2.AddString ("NO");
    m_comboFC3.AddString ("NO");
    m_comboFC4.AddString ("NO");

    if (comport_setting_len > 0)
    {
        m_comboFC1.SetCurSel (p_sComportSetting[0].com_port_info.flow_control);
        m_comboFC2.SetCurSel (p_sComportSetting[1].com_port_info.flow_control);
        m_comboFC3.SetCurSel (p_sComportSetting[2].com_port_info.flow_control);
        m_comboFC4.SetCurSel (p_sComportSetting[3].com_port_info.flow_control);
    }
    else
    {
        m_comboFC1.SetCurSel (0);
        m_comboFC2.SetCurSel (0);
        m_comboFC3.SetCurSel (0);
        m_comboFC4.SetCurSel (0);
    }

    /**
    m_comboBAUD1.AddString ("57600");
    m_comboBAUD1.AddString ("115200");
    m_comboBAUD2.AddString ("57600");
    m_comboBAUD2.AddString ("115200");
    m_comboBAUD3.AddString ("57600");
    m_comboBAUD3.AddString ("115200");
    m_comboBAUD4.AddString ("57600");
    m_comboBAUD4.AddString ("115200");
    **/

    m_comboBAUD1.AddString ("115200");
    m_comboBAUD2.AddString ("115200");
    m_comboBAUD3.AddString ("115200");
    m_comboBAUD4.AddString ("115200");

    if (comport_setting_len > 0)
    {
        m_comboBAUD1.SetCurSel (p_sComportSetting[0].com_port_info.baud_rate);
        m_comboBAUD2.SetCurSel (p_sComportSetting[1].com_port_info.baud_rate);
        m_comboBAUD3.SetCurSel (p_sComportSetting[2].com_port_info.baud_rate);
        m_comboBAUD4.SetCurSel (p_sComportSetting[3].com_port_info.baud_rate);
    }
    else
    {
        m_comboBAUD1.SetCurSel (1);
        m_comboBAUD2.SetCurSel (1);
        m_comboBAUD3.SetCurSel (1);
        m_comboBAUD4.SetCurSel (1);
    }

    if (comport_setting_len > 0)
    {
        m_checkDUT1 =   p_sComportSetting[0].bActive;
        m_checkDUT2 =   p_sComportSetting[1].bActive;
        m_checkDUT3 =   p_sComportSetting[2].bActive;
        m_checkDUT4 =   p_sComportSetting[3].bActive;
    }

    if (comport_setting_len > 0)
    {
        m_iPreloaderPort1	= p_sComportSetting[0].com_port_info.preloader_com_port;
        m_iPreloaderPort2	= p_sComportSetting[1].com_port_info.preloader_com_port;
        m_iPreloaderPort3	= p_sComportSetting[2].com_port_info.preloader_com_port;
        m_iPreloaderPort4	= p_sComportSetting[3].com_port_info.preloader_com_port;

        m_iKernelPort1	= p_sComportSetting[0].com_port_info.kernel_com_port;
        m_iKernelPort2	= p_sComportSetting[1].com_port_info.kernel_com_port;
        m_iKernelPort3	= p_sComportSetting[2].com_port_info.kernel_com_port;
        m_iKernelPort4	= p_sComportSetting[3].com_port_info.kernel_com_port;

        m_iEditPMPort1	= p_sComportSetting[0].com_port_info.pm_info.comport;
        m_iEditPMPort2	= p_sComportSetting[1].com_port_info.pm_info.comport;
        m_iEditPMPort3	= p_sComportSetting[2].com_port_info.pm_info.comport;
        m_iEditPMPort4	= p_sComportSetting[3].com_port_info.pm_info.comport;
    }

    m_comboSimNum.SetCurSel (p_sCommonCFG->iSimNum - 1);
    m_iWaitSec  = p_sCommonCFG->waitSecBeforeTest;
    m_iRetryTimes	= p_sCommonCFG->retry_times;
    m_iLoopTimes	= loop_test_info.loop_times;
    m_bAutoPoll		= p_sCommonCFG->auto_polling ? true : false;
    m_bFlagBarcode	= p_sCommonCFG->barcodeFlag_cfg.bSetFlag ? true : false;
    m_bComposite	= p_sCommonCFG->com_port_info.bIsComposite ? true : false;
    m_bStopIfFailed	= p_sCommonCFG->stop_if_failed ? true : false;

	m_iTestItemTimeout	= p_sCommonCFG->timeout[0];

    UpdateData (false);

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CTestItemConfig::UpdateTestItem()
{
    m_bBarcode  = p_sTestItem->b_Barcode ? 1 : 0;
    m_bSW      = p_sTestItem->b_SWVersion ? 1 : 0;
    m_bTCard    = p_sTestItem->b_TCard ? 1 : 0;
    m_bSIM      = p_sTestItem->b_SIM ? 1 : 0;
    m_bGPS      = p_sTestItem->b_GPS ? 1 : 0;
    m_bWIFI		= p_sTestItem->b_WIFI ? 1 : 0;
    m_bFM		= p_sTestItem->b_FM ? 1 : 0;
    m_bVibrator	= p_sTestItem->b_Vibrator ? 1 : 0;
    m_bMATV		= p_sTestItem->b_TV ? 1 : 0;
    m_bCharger	= p_sTestItem->b_Charging ? 1 : 0;
    m_bSpeaker  = p_sTestItem->b_Speaker ? 1 : 0;
    m_bReceiver = p_sTestItem->b_Receiver ? 1 : 0;
    m_bEarthphone   = p_sTestItem->b_EarchPhone ? 1 : 0;
    m_bLCM		= p_sTestItem->b_LCD ? 1 : 0;
    m_bBT       = p_sTestItem->b_BT ? 1 : 0;
    m_bTP       = p_sTestItem->b_TouchScr ? 1 : 0;
    m_bRTC      = p_sTestItem->b_RTC ? 1 : 0;
    m_bCamera   = p_sTestItem->b_Camera ? 1 : 0;
	m_bCameraMain2 = p_sTestItem->b_Camera_Main2 ? 1 : 0;
    m_bCameraSub	= p_sTestItem->b_Camera_Sub ? 1 : 0;
    m_bHWVer    = p_sTestItem->b_HWVersion ? 1 : 0;
    m_bERState  = p_sTestItem->b_er_state ? 1 : 0;
    m_bKPad     = p_sTestItem->b_Keypad ? 1 : 0;
    m_bSlpmode	= p_sTestItem->b_slpmode ? 1 : 0;
    m_bCheckADC	= p_sTestItem->b_ADC ? 1 : 0;
    m_bRSSI		= p_sTestItem->b_RSSI ? 1 : 0;
    m_bLED		= p_sTestItem->b_LED ? 1 : 0;
    m_bEMMC		= p_sTestItem->b_EMMC ? 1 : 0;
    m_bCMMB		= p_sTestItem->b_CMMB ? 1 : 0;
    m_bGSensor	= p_sTestItem->b_gsensor ? 1 : 0;
    m_bMSensor	= p_sTestItem->b_msensor ? 1 : 0;
    m_bAlsps	= p_sTestItem->b_alsps ? 1 : 0;
    m_bGyroscope = p_sTestItem->b_gyroscope ? 1 : 0;
	m_bOTG		= p_sTestItem->b_OTG ? 1 : 0;
	m_bHDMI		= p_sTestItem->b_HDMI ? 1 : 0;
	m_bMHL		= p_sTestItem->b_MHL ? 1 : 0;
    m_bMicbias	= p_sTestItem->b_micbias ? 1 : 0;
    m_bOffMode	= p_sTestItem->b_offMode ? 1 : 0;
	m_bUsbTypeC = p_sTestItem->b_UsbTypeC ? 1 : 0;
	m_bHumidity = p_sTestItem->b_Humidity ? 1 : 0;
	m_bBarometer= p_sTestItem->b_Barometer ? 1 : 0;
	m_bBTS		= p_sTestItem->b_BTS ? 1 : 0;
	m_bExtBuck	= p_sTestItem->b_ExtBuck? 1 : 0;
	m_bFingerPrint = p_sTestItem->b_FingerPrint? 1 : 0;
	m_bGPSCali = p_sTestItem->b_GPSCali? 1 : 0; 	//add by songjian 2016-3-1

#ifdef INTERNEL_DEBUG_VERSION
	GetDlgItem(IDC_CHECK_SLPMODE)->EnableWindow(FALSE);
	GetDlgItem(IDC_CHECK_OFFMODE)->EnableWindow(FALSE);
	GetDlgItem(IDC_CHECK_MICBIAS)->EnableWindow(FALSE);
	GetDlgItem(IDC_CHECK_OTG)->EnableWindow(FALSE);
#endif

#ifndef INTERNEL_DEBUG_VERSION
	GetDlgItem(IDC_CHECK_USBTYPEC)->EnableWindow(FALSE);
	GetDlgItem(IDC_CHECK_USBTYPEC)->ShowWindow(SW_HIDE);
#endif

    UpdateData (false);
}

void CTestItemConfig::EnumerateSerialPorts(CUIntArray& ports)
{
    // Clear
    ports.RemoveAll();

    // Get OS Version
    OSVERSIONINFO osvi;
    osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
    BOOL bRet = GetVersionEx(&osvi);

    // NT: QueryDosDevice
    if(bRet && (osvi.dwPlatformId == VER_PLATFORM_WIN32_NT))
    {
        CRegKey regKey;
        if (ERROR_SUCCESS != regKey.Open(HKEY_LOCAL_MACHINE, "HARDWARE\\DEVICEMAP\\SERIALCOMM", KEY_QUERY_VALUE|KEY_ENUMERATE_SUB_KEYS))
            return;

        TCHAR tstrName[256];
        BYTE  bData[256];
        DWORD dwNameLen = 256;
        DWORD dwDataLen = 256;
        DWORD dwType = 0;

        memset(bData, 0, sizeof(bData));

        int nValueIndex = 0;
        while (TRUE)
        {
            LONG res;
            res = RegEnumValue(
                      regKey.m_hKey,
                      nValueIndex,
                      tstrName,
                      &dwNameLen,
                      NULL,
                      &dwType,
                      bData,
                      &dwDataLen);

            if (ERROR_SUCCESS != res && ERROR_MORE_DATA != res) break;

            if (REG_SZ != dwType) continue;

            int nPort = _ttoi((char *)&bData[3]);
            ports.Add(nPort);
            nValueIndex++;
            dwNameLen = 256;
            dwDataLen = 256;
        }

        regKey.Close();
    }
    else
    {
        // 95/98
        for (UINT i=1; i<256; i++)
        {
            // COM Name
            CString strPort;
            strPort.Format(_T("\\\\.\\COM%d"), i);

            // Try to Open Port
            BOOL bSuccess = FALSE;
            HANDLE hPort = ::CreateFile(strPort, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, 0, 0);
            if(hPort == INVALID_HANDLE_VALUE)
            {
                DWORD dwError = GetLastError();
                if(dwError == ERROR_ACCESS_DENIED || dwError == ERROR_GEN_FAILURE)
                    bSuccess = TRUE;
            }
            else
            {
                bSuccess = TRUE;
                CloseHandle(hPort);
            }

            if(bSuccess)
                ports.Add(i);
        }
    }
}

void CTestItemConfig::OnSave()
{
    // TODO: Add your control notification handler code here
    this->UpdateData(true);

    // Test Item
    p_sTestItem->b_Barcode      = m_bBarcode ? true : false;
    p_sTestItem->b_SWVersion    = m_bSW ? true : false;
    p_sTestItem->b_GPS          = m_bGPS ? true : false;
    p_sTestItem->b_TCard        = m_bTCard ? true : false;
    p_sTestItem->b_SIM          = m_bSIM ? true : false;
    p_sTestItem->b_WIFI			= m_bWIFI ? true : false;
    p_sTestItem->b_FM			= m_bFM ? true : false;
    p_sTestItem->b_Vibrator		= m_bVibrator ? true : false;
    p_sTestItem->b_TV			= m_bMATV ? true :false;
    p_sTestItem->b_Charging		= m_bCharger ? true : false;
    p_sTestItem->b_Receiver     = m_bReceiver ? true : false;
    p_sTestItem->b_EarchPhone   = m_bEarthphone ? true : false;
    p_sTestItem->b_Speaker      = m_bSpeaker ? true : false;
    p_sTestItem->b_LCD			= m_bLCM ? true : false;
    p_sTestItem->b_BT           = m_bBT ? true : false;
    p_sTestItem->b_TouchScr     = m_bTP ? true : false;
    p_sTestItem->b_RTC          = m_bRTC ? true : false;
    p_sTestItem->b_Camera       = m_bCamera ? true : false;
	p_sTestItem->b_Camera_Main2 = m_bCameraMain2 ? true : false;
    p_sTestItem->b_Camera_Sub	= m_bCameraSub ? true : false;
    p_sTestItem->b_HWVersion    = m_bHWVer ? true : false;
    p_sTestItem->b_er_state     = m_bERState ? true : false;
    p_sTestItem->b_Keypad       = m_bKPad ? true : false;
    p_sTestItem->b_slpmode		= m_bSlpmode ? true : false;
    p_sTestItem->b_ADC			= m_bCheckADC ? true : false;
    p_sTestItem->b_RSSI			= m_bRSSI ? true : false;
    p_sTestItem->b_LED			= m_bLED ? true : false;
    p_sTestItem->b_EMMC			= m_bEMMC ? true : false;
    p_sTestItem->b_CMMB			= m_bCMMB ? true : false;
    p_sTestItem->b_gsensor		= m_bGSensor ? true : false;
    p_sTestItem->b_msensor		= m_bMSensor ? true : false;
    p_sTestItem->b_alsps		= m_bAlsps ? true : false;
    p_sTestItem->b_gyroscope	= m_bGyroscope ? true : false;
	p_sTestItem->b_OTG			= m_bOTG ? true : false;
	p_sTestItem->b_HDMI			= m_bHDMI ? true : false;
	p_sTestItem->b_MHL			= m_bMHL ? true : false;
    p_sTestItem->b_micbias		= m_bMicbias ? true : false;
    p_sTestItem->b_offMode		= m_bOffMode ? true : false;
	p_sTestItem->b_UsbTypeC		= m_bUsbTypeC ? true : false;
	p_sTestItem->b_Humidity		= m_bHumidity ? true : false;
	p_sTestItem->b_Barometer	= m_bBarometer ? true : false;
	p_sTestItem->b_BTS			= m_bBTS ? true : false;
	p_sTestItem->b_ExtBuck		= m_bExtBuck? true : false;
	p_sTestItem->b_FingerPrint	= m_bFingerPrint? true : false;
	p_sTestItem->b_GPSCali 		= m_bGPSCali? true : false;  	//add by songjian 2016-3-1
	

    p_sCommonCFG->iSimNum       = m_comboSimNum.GetCurSel () + 1;
    p_sCommonCFG->waitSecBeforeTest = m_iWaitSec;
    p_sCommonCFG->retry_times	= m_iRetryTimes;
    loop_test_info.loop_times	= m_iLoopTimes;
    p_sCommonCFG->auto_polling	= m_bAutoPoll ? true : false;
    p_sCommonCFG->barcodeFlag_cfg.bSetFlag	= m_bFlagBarcode ? true : false;
    p_sCommonCFG->com_port_info.bIsComposite = m_bComposite ? true : false;
    p_sCommonCFG->stop_if_failed	= m_bStopIfFailed ? true : false;

	for (E_ATDLL_TEST_ITEM_COUNT index = E_LCD; index < E_TEST_ITEM_COUNT; index = (E_ATDLL_TEST_ITEM_COUNT)((int)index + 1))
    {
		p_sCommonCFG->timeout[index]	= m_iTestItemTimeout;
    }

    // Com port setting
    if (comport_setting_len > 0)
    {
        int sel = 0;

        p_sComportSetting[0].bActive    = m_checkDUT1;
        p_sComportSetting[0].com_port_info.com_port     = (sel = m_comboCOM1.GetCurSel()) != CB_ERR ? m_comboCOM1.GetItemData (sel) : 0;
        p_sComportSetting[0].com_port_info.flow_control = m_comboFC1.GetCurSel ()!= CB_ERR ? m_comboFC1.GetCurSel () : 0;
        p_sComportSetting[0].com_port_info.baud_rate    = m_comboBAUD1.GetCurSel () != CB_ERR ? m_comboBAUD1.GetCurSel () : 0;
        p_sComportSetting[0].com_port_info.preloader_com_port	= m_iPreloaderPort1;
        p_sComportSetting[0].com_port_info.kernel_com_port	= m_iKernelPort1;
        p_sComportSetting[0].com_port_info.pm_info.comport	= m_iEditPMPort1;
        p_sComportSetting[0].com_port_info.bIsComposite	= m_bComposite ? true : false;

        p_sComportSetting[1].bActive    = m_checkDUT2;
        p_sComportSetting[1].com_port_info.com_port     = (sel = m_comboCOM2.GetCurSel()) != CB_ERR ? m_comboCOM2.GetItemData (sel) : 0;
        p_sComportSetting[1].com_port_info.flow_control = m_comboFC2.GetCurSel ()!= CB_ERR ? m_comboFC2.GetCurSel () : 0;
        p_sComportSetting[1].com_port_info.baud_rate    = m_comboBAUD2.GetCurSel () != CB_ERR ? m_comboBAUD2.GetCurSel () : 0;
        p_sComportSetting[1].com_port_info.preloader_com_port	= m_iPreloaderPort2;
        p_sComportSetting[1].com_port_info.kernel_com_port	= m_iKernelPort2;
        p_sComportSetting[1].com_port_info.pm_info.comport	= m_iEditPMPort2;
        p_sComportSetting[1].com_port_info.bIsComposite	= m_bComposite ? true : false;

        p_sComportSetting[2].bActive    = m_checkDUT3;
        p_sComportSetting[2].com_port_info.com_port     = (sel = m_comboCOM3.GetCurSel()) != CB_ERR ? m_comboCOM3.GetItemData (sel) : 0;
        p_sComportSetting[2].com_port_info.flow_control = m_comboFC3.GetCurSel ()!= CB_ERR ? m_comboFC3.GetCurSel () : 0;
        p_sComportSetting[2].com_port_info.baud_rate    = m_comboBAUD3.GetCurSel () != CB_ERR ? m_comboBAUD3.GetCurSel () : 0;
        p_sComportSetting[2].com_port_info.preloader_com_port	= m_iPreloaderPort3;
        p_sComportSetting[2].com_port_info.kernel_com_port	= m_iKernelPort3;
        p_sComportSetting[2].com_port_info.pm_info.comport	= m_iEditPMPort3;
        p_sComportSetting[2].com_port_info.bIsComposite	= m_bComposite ? true : false;

        p_sComportSetting[3].bActive    = m_checkDUT4;
        p_sComportSetting[3].com_port_info.com_port     = (sel = m_comboCOM4.GetCurSel()) != CB_ERR ? m_comboCOM4.GetItemData (sel) : 0;
        p_sComportSetting[3].com_port_info.flow_control = m_comboFC4.GetCurSel ()!= CB_ERR ? m_comboFC4.GetCurSel () : 0;
        p_sComportSetting[3].com_port_info.baud_rate    = m_comboBAUD4.GetCurSel () != CB_ERR ? m_comboBAUD4.GetCurSel () : 0;
        p_sComportSetting[3].com_port_info.preloader_com_port	= m_iPreloaderPort4;
        p_sComportSetting[3].com_port_info.kernel_com_port	= m_iKernelPort4;
        p_sComportSetting[3].com_port_info.pm_info.comport	= m_iEditPMPort4;
        p_sComportSetting[3].com_port_info.bIsComposite	= m_bComposite ? true : false;
    }

    // Spec setting
    m_pageWIFI.UpdateParameters ();
    m_pageBT.UpdateParameters ();
    //m_pageGPS.UpdateParameters ();
    m_pageAudio.UpdateParameters ();
    m_pageFM.UpdateParameters ();
    //m_pageMATV.UpdateParameters ();
    m_pageVibrator.UpdateParameters ();
    m_pageHWVer.UpdateParameters ();
//    m_pageERState.UpdateParameters ();
	m_pageBTS.UpdateParameters ();
    m_pageCamera.UpdateParameters ();
    m_pageKPad.UpdateParameters();
    m_pagePMCal.UpdateParameters ();
    m_pageLED.UpdateParameters ();
    m_pageSensor.UpdateParameters ();
    m_pageMicbias.UpdateParameters ();

    this->OnOK();
}

void CTestItemConfig::OnCancel()
{
    // TODO: Add extra cleanup here
    CDialog::OnCancel();
}

void CTestItemConfig::OnChangepwd()
{
    // TODO: Add your control notification handler code here
    CChangePwd changePWD;
    changePWD.SetTestItemCFG (p_sCommonCFG);
    changePWD.DoModal();

}

BOOL CTestItemConfig::PreCreateWindow(CREATESTRUCT& cs)
{
    // TODO: Add your specialized code here and/or call the base class
    /**
    cs.style=WS_OVERLAPPEDWINDOW;
    cs.cx=::GetSystemMetrics(SM_CXSCREEN);
    cs.cy=::GetSystemMetrics(SM_CYSCREEN);
    **/

    return CDialog::PreCreateWindow(cs);
}

void CTestItemConfig::OnCheckSelectAll1()
{
    // TODO: Add your control notification handler code here
    UpdateData (true);

	m_bSW = m_bBarcode= m_bWIFI = m_bBT= m_bFM= m_bGPS= m_bRSSI=
          	m_bGSensor = m_bMSensor = m_bGyroscope = m_bAlsps= m_bSelectAll1;

    UpdateData (false);
}

void CTestItemConfig::OnCheckSelectAll2()
{
    // TODO: Add your control notification handler code here
    UpdateData (true);

	m_bLCM = m_bTP = m_bCamera = m_bCameraSub =m_bCameraMain2= m_bEMMC= m_bTCard=
                                     m_bSIM= m_bKPad= m_bRTC = m_bMHL = m_bHDMI= m_bSelectAll2;

    UpdateData (false);
}

void CTestItemConfig::OnCheckSelectAll3()
{
    // TODO: Add your control notification handler code here
    UpdateData (true);

	m_bSpeaker = m_bReceiver= m_bVibrator= m_bCharger= m_bCheckADC= m_bLED
	     = m_bEarthphone = m_bSelectAll3;

#ifndef INTERNEL_DEBUG_VERSION
	m_bSlpmode = m_bOffMode= m_bMicbias= m_bOTG= m_bSelectAll3;
#else
		m_bUsbTypeC= m_bSelectAll3;

#endif

    UpdateData (false);
}

void CTestItemConfig::OnCheckSelectAll4() 
{
	// TODO: Add your control notification handler code here
	UpdateData (true);
	m_bHumidity = m_bBarometer = m_bExtBuck = m_bFingerPrint = m_bBTS = m_bGPSCali = m_bSelectAll4; 	//modify by songjian 2016-3-1

	UpdateData (false);
	
}

void CTestItemConfig::OnGPS_Cali() 
{
	// TODO: Add your control notification handler code here
	
}
