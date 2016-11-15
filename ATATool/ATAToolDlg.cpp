// ATAToolDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ATATool.h"
#include "ATAToolDlg.h"
#include "PasswdInput.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

CATAToolDlg *pATAToolDlg = NULL;;
char version[64];

class CAboutDlg : public CDialog
{
public:
    CAboutDlg();

// Dialog Data
    //{{AFX_DATA(CAboutDlg)
    enum { IDD = IDD_ABOUTBOX };
    //}}AFX_DATA

    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CAboutDlg)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:
    //{{AFX_MSG(CAboutDlg)
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
    //{{AFX_DATA_INIT(CAboutDlg)
    //}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CAboutDlg)
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
    //{{AFX_MSG_MAP(CAboutDlg)
    // No message handlers
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CATAToolDlg dialog

CATAToolDlg::CATAToolDlg(CWnd* pParent /*=NULL*/)
    : CResizableDialog(CATAToolDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CATAToolDlg)
    // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
    // Note that LoadIcon does not require a subsequent DestroyIcon in Win32
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    m_hButtonEvent      =   CreateEvent(NULL, FALSE, TRUE, NULL);

    stopFlag1    = -1;
    stopFlag2    = -1;
    stopFlag3    = -1;
    stopFlag4    = -1;
    memset (&ata_test_items,    0x0,    sizeof(S_ATADLL_TEST_ITEM_T));
    memset (&ata_common_cfg,    0x0,    sizeof(S_ATADLL_COMMON_CFG_T));
    memset (&ata_common_cfg1,   0x0,    sizeof(S_ATADLL_COMMON_CFG_T));
    memset (&ata_common_cfg2,   0x0,    sizeof(S_ATADLL_COMMON_CFG_T));
    memset (&ata_common_cfg3,   0x0,    sizeof(S_ATADLL_COMMON_CFG_T));
    memset (&ata_common_cfg4,   0x0,    sizeof(S_ATADLL_COMMON_CFG_T));
    memset (&ata_test_spec,     0x0,    sizeof(S_ATADLL_TESTITEM_SPEC));
    memset (comport_setting,    0x0,    sizeof(S_COMPORT_ACTIVE_INFO) * MAX_DUT_NUM);

    memset (&loop_test_info,	0x0,	sizeof(S_TESTITEM_LOOP_TEST_INFO));
    loop_test_info.loop_times = 1;

    pATAToolDlg = this;
	
    char SetupDir[512];
    GetCurrentDirectory (512, SetupDir);
    sprintf (SetupDir, "%s\\Setup.ini", SetupDir);
	MTRACE(g_hEBOOT_DEBUG, "Begin to Parse Setup.ini");
    CFile file;
    if (!file.Open (SetupDir, CFile::modeReadWrite))
        return;
    file.SeekToBegin();

    int bufSize = 0;
    char buffer[10240];

    file.Read (&bufSize, sizeof(int));
    file.Read (buffer, bufSize);
    buffer[bufSize] = 0;
    memcpy (&ata_test_items, buffer, bufSize);

    file.Read (&bufSize, sizeof(int));
    file.Read (buffer, bufSize);
    buffer[bufSize] = 0;
    memcpy (&ata_common_cfg, buffer, bufSize);

    file.Read (&bufSize, sizeof(int));
    file.Read (buffer, bufSize);
    buffer[bufSize] = 0;
    memcpy (&ata_test_spec, buffer, bufSize);

    file.Read (&bufSize, sizeof(int));
    file.Read (buffer, bufSize);
    buffer[bufSize] = 0;
    memcpy (&comport_setting, buffer, bufSize);

    file.Close ();
	MTRACE(g_hEBOOT_DEBUG, "Parse Setup.ini end.");

    // Update testitem and common cfg after configuration
    /**
    UpdateTestObj ();
    testObj1.FillListCtrl ();
    testObj2.FillListCtrl ();
    testObj3.FillListCtrl ();
    testObj4.FillListCtrl ();
    **/

}

CATAToolDlg::~CATAToolDlg()
{
    if (m_hButtonEvent != NULL)
    {
        CloseHandle (m_hButtonEvent);
    }

    SaveConfigToSetupFile ();
}

void CATAToolDlg::SaveConfigToSetupFile()
{
	MTRACE(g_hEBOOT_DEBUG, "Entry SaveConfigToSetupFile()");

    char SetupDir[512];
    GetCurrentDirectory (512, SetupDir);
    sprintf (SetupDir, "%s\\Setup.ini", SetupDir);

    CFile file;
    if (!file.Open (SetupDir, CFile::modeReadWrite | CFile::modeCreate))
        return;
    file.SeekToBegin();

    // ata_test_items
    int size = sizeof(ata_test_items);
    file.Write ((const char*)&size, sizeof(int));
    file.Write ((const char*)&ata_test_items, size);

    // ata_common_cfg
    size = sizeof (ata_common_cfg);
    file.Write ((const char*)&size, sizeof(int));
    file.Write ((const char*)&ata_common_cfg, size);

    // ata_test_spec
    size = sizeof (ata_test_spec);
    file.Write ((const char*)&size, sizeof(int));
    file.Write ((const char*)&ata_test_spec, size);

    // comport_setting
    size = sizeof (comport_setting);
    file.Write ((const char*)&size, sizeof(int));
    file.Write ((const char*)&comport_setting, size);

    file.Close ();
	MTRACE(g_hEBOOT_DEBUG, "Exit SaveConfigToSetupFile()");
}

void CATAToolDlg::DoDataExchange(CDataExchange* pDX)
{
    CResizableDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CATAToolDlg)
    DDX_Control(pDX, IDC_LIST_DUT4, m_ListCtrlDUT4);
    DDX_Control(pDX, IDC_LIST_DUT3, m_ListCtrlDUT3);
    DDX_Control(pDX, IDC_LIST_DUT2, m_ListCtrlDUT2);
    DDX_Control(pDX, IDC_LIST_DUT1, m_ListCtrlDUT1);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CATAToolDlg, CResizableDialog)
    //{{AFX_MSG_MAP(CATAToolDlg)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_BUTTON_CONFIG, OnButtonConfig)
    ON_BN_CLICKED(IDC_BUTTON_START_ALL, OnButtonStartAll)
    ON_BN_CLICKED(IDC_BUTTON_STOP_ALL, OnButtonStopAll)
    ON_WM_SIZE()
    ON_BN_CLICKED(IDC_BUTTON_1, OnButton1)
    ON_BN_CLICKED(IDC_BUTTON_2, OnButton2)
    ON_BN_CLICKED(IDC_BUTTON_3, OnButton3)
    ON_BN_CLICKED(IDC_BUTTON_4, OnButton4)
    ON_NOTIFY(NM_DBLCLK, IDC_LIST_DUT1, OnDblclkListDut1)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CATAToolDlg message handlers

BOOL CATAToolDlg::OnInitDialog()
{
    CResizableDialog::OnInitDialog();

    // Add "About..." menu item to system menu.

    // IDM_ABOUTBOX must be in the system command range.
    ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
    ASSERT(IDM_ABOUTBOX < 0xF000);

    CMenu* pSysMenu = GetSystemMenu(FALSE);
    if (pSysMenu != NULL)
    {
        CString strAboutMenu;
        strAboutMenu.LoadString(IDS_ABOUTBOX);
        if (!strAboutMenu.IsEmpty())
        {
            pSysMenu->AppendMenu(MF_SEPARATOR);
            pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
        }
    }

    // Set the icon for this dialog.  The framework does this automatically
    //  when the application's main window is not a dialog
    SetIcon(m_hIcon, TRUE);			// Set big icon
    SetIcon(m_hIcon, FALSE);		// Set small icon

    // TODO: Add extra initialization here
    AddAnchor(IDC_STATIC_MAIN_RECT, TOP_LEFT, BOTTOM_RIGHT);
    AddAnchor(IDC_LIST_DUT1, TOP_LEFT, ANCHOR(25, 100));
    AddAnchor(IDC_LIST_DUT2, ANCHOR(25, 0), ANCHOR(50, 100));
    AddAnchor(IDC_LIST_DUT3, ANCHOR(50, 0), ANCHOR(75, 100));
    AddAnchor(IDC_LIST_DUT4, ANCHOR(75, 0), BOTTOM_RIGHT);

    AddAnchor(IDC_STATIC_LINE, BOTTOM_LEFT, BOTTOM_RIGHT);
    AddAnchor(IDC_BUTTON_1, BOTTOM_LEFT, ANCHOR(25, 100));
    AddAnchor(IDC_BUTTON_2, ANCHOR(25, 100), ANCHOR(50, 100));
    AddAnchor(IDC_BUTTON_3, ANCHOR(50, 100), ANCHOR(75, 100));
    AddAnchor(IDC_BUTTON_4, ANCHOR(75, 100), BOTTOM_RIGHT);

    AddAnchor(IDC_STATIC_LINE2, BOTTOM_LEFT, BOTTOM_RIGHT);
    AddAnchor(IDC_BUTTON_CONFIG, BOTTOM_LEFT);
    AddAnchor(IDC_BUTTON_START_ALL, BOTTOM_RIGHT);
    AddAnchor(IDC_BUTTON_STOP_ALL, BOTTOM_RIGHT);

    // allow any size
    ResetMinTrackSize();

	CString title;//mingxue20150807
	GetWindowText(title);//mingxue 20150807
	MTRACE(g_hEBOOT_DEBUG, "//*******************************************************************//");//mingxue 20150807
	MTRACE(g_hEBOOT_DEBUG, "//             Tool version:%s",LPCSTR(title));//mingxue 20150807
	MTRACE(g_hEBOOT_DEBUG, "//*******************************************************************//");//mingxue20150807
	strcpy(version,LPCSTR(title));
	UpdateTestObj ();

    // Init Test Object (1 ~ 4)
    testObj1.SetXListCtrl (&m_ListCtrlDUT1);    // Set XList ctrl
    testObj1.InitializeListCtrl();              // Init Column & title
    testObj1.FillListCtrl();
    testObj2.SetXListCtrl (&m_ListCtrlDUT2);
    testObj2.InitializeListCtrl();
    testObj2.FillListCtrl();
    testObj3.SetXListCtrl (&m_ListCtrlDUT3);
    testObj3.InitializeListCtrl();
    testObj3.FillListCtrl();
    testObj4.SetXListCtrl (&m_ListCtrlDUT4);
    testObj4.InitializeListCtrl();
    testObj4.FillListCtrl();

    return TRUE;  // return TRUE  unless you set the focus to a control
}

void CATAToolDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
    if ((nID & 0xFFF0) == IDM_ABOUTBOX)
    {
        CAboutDlg dlgAbout;
        dlgAbout.DoModal();
    }
    else
    {
        CResizableDialog::OnSysCommand(nID, lParam);
    }
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CATAToolDlg::OnPaint()
{
    if (IsIconic())
    {
        CPaintDC dc(this); // device context for painting

        SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

        // Center icon in client rectangle
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;

        // Draw the icon
        dc.DrawIcon(x, y, m_hIcon);
    }
    else
    {
        CResizableDialog::OnPaint();
    }
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CATAToolDlg::OnQueryDragIcon()
{
    return (HCURSOR) m_hIcon;
}

void CATAToolDlg::OnButtonConfig()
{
	MTRACE(g_hEBOOT_DEBUG, "Entry OnButtonConfig()");

    // TODO: Add your control notification handler code here

#ifndef INTERNEL_DEBUG_VERSION
    bool passFlag = false;
    CPasswdInput passwrdDlg;
    passwrdDlg.SetTestItemCFG (&ata_common_cfg);
    passwrdDlg.SetPassFlag (&passFlag);
    passwrdDlg.DoModal();
    if (!passFlag)
    {
        return;
    }
#endif

    CTestItemConfig ticonfig;
    ticonfig.SetTestItem (&ata_test_items);
    ticonfig.SetCommonCFG (&ata_common_cfg);
    ticonfig.SetTestItemSpec (&ata_test_spec);
    ticonfig.SetComportSetting (comport_setting, MAX_DUT_NUM);
    ticonfig.DoModal();

    // Update testitem and common cfg after configuration
    UpdateTestObj ();
    testObj1.FillListCtrl ();
    testObj2.FillListCtrl ();
    testObj3.FillListCtrl ();
    testObj4.FillListCtrl ();

    SaveConfigToSetupFile ();
}

// Update testitems, common cfg
void CATAToolDlg::UpdateTestObj ()
{
#ifndef __ATA_MT6795_SMT__
    for (E_ATDLL_TEST_ITEM_COUNT index = E_LCD; index < E_TEST_ITEM_COUNT; index = (E_ATDLL_TEST_ITEM_COUNT)((int)index + 1))
    {
#ifdef __ATA30__
		ata_common_cfg.timeout[index] = 120;
#else
        ata_common_cfg.timeout[index] = 60;
        if (index == E_BT)
            ata_common_cfg.timeout[index] = 90;
        if (index == E_GPS)
            ata_common_cfg.timeout[index] = 90;
        if (index == E_KEYPAD || index == E_TOUCHSCR || index == E_CMMB )
            ata_common_cfg.timeout[index] = 20;
        if (index == E_CHARGING)
            ata_common_cfg.timeout[index] = 60;
        if (index == E_RSSI)
            ata_common_cfg.timeout[index] = 120;
        if (index == E_CAMERA)
            ata_common_cfg.timeout[index] = 60;
        if (index == E_SIM)
            ata_common_cfg.timeout[index] = 40;
		if (index == E_HDMI)
			ata_common_cfg.timeout[index] = 60;
#endif

    }
#endif

    m_hTestDUTNum = 0;
    GetDlgItem(IDC_BUTTON_1)->EnableWindow(false);
    GetDlgItem(IDC_BUTTON_2)->EnableWindow(false);
    GetDlgItem(IDC_BUTTON_3)->EnableWindow(false);
    GetDlgItem(IDC_BUTTON_4)->EnableWindow(false);
    if (comport_setting[0].bActive)
    {
        memcpy (&ata_common_cfg1, &ata_common_cfg, sizeof(S_ATADLL_COMMON_CFG_T));
        memcpy (&ata_common_cfg1.com_port_info, &comport_setting[0].com_port_info, sizeof(S_ATADLL_COMPORT));
        ata_common_cfg1.stop_flag   = &stopFlag1;
        m_hTestDUTNum ++;
        GetDlgItem(IDC_BUTTON_1)->EnableWindow(TRUE);
    }
    if (comport_setting[1].bActive)
    {
        memcpy (&ata_common_cfg2, &ata_common_cfg, sizeof(S_ATADLL_COMMON_CFG_T));
        memcpy (&ata_common_cfg2.com_port_info, &comport_setting[1].com_port_info, sizeof(S_ATADLL_COMPORT));
        ata_common_cfg2.stop_flag   = &stopFlag2;
        m_hTestDUTNum ++;
        GetDlgItem(IDC_BUTTON_2)->EnableWindow(TRUE);
    }
    if (comport_setting[2].bActive)
    {
        memcpy (&ata_common_cfg3, &ata_common_cfg, sizeof(S_ATADLL_COMMON_CFG_T));
        memcpy (&ata_common_cfg3.com_port_info, &comport_setting[2].com_port_info, sizeof(S_ATADLL_COMPORT));
        ata_common_cfg3.stop_flag   = &stopFlag3;
        m_hTestDUTNum ++;
        GetDlgItem(IDC_BUTTON_3)->EnableWindow(TRUE);
    }
    if (comport_setting[3].bActive)
    {
        memcpy (&ata_common_cfg4, &ata_common_cfg, sizeof(S_ATADLL_COMMON_CFG_T));
        memcpy (&ata_common_cfg4.com_port_info, &comport_setting[3].com_port_info, sizeof(S_ATADLL_COMPORT));
        ata_common_cfg4.stop_flag   = &stopFlag4;
        m_hTestDUTNum ++;
        GetDlgItem(IDC_BUTTON_4)->EnableWindow(TRUE);
    }

    // Use one common test items
    testObj1.SetTestItem (&ata_test_items);
    testObj2.SetTestItem (&ata_test_items);
    testObj3.SetTestItem (&ata_test_items);
    testObj4.SetTestItem (&ata_test_items);

    // Use separate common cfg
    testObj1.SetCommCFG (&ata_common_cfg1);
    testObj2.SetCommCFG (&ata_common_cfg2);
    testObj3.SetCommCFG (&ata_common_cfg3);
    testObj4.SetCommCFG (&ata_common_cfg4);

    // Use one common test spec
    testObj1.SetTestSpec (&ata_test_spec);
    testObj2.SetTestSpec (&ata_test_spec);
    testObj3.SetTestSpec (&ata_test_spec);
    testObj4.SetTestSpec (&ata_test_spec);

    // Set callback func
    testObj1.SetTestFinishCallBack (UpdateButtonStatus);
    testObj2.SetTestFinishCallBack (UpdateButtonStatus);
    testObj3.SetTestFinishCallBack (UpdateButtonStatus);
    testObj4.SetTestFinishCallBack (UpdateButtonStatus);

    // Set button ID
    testObj1.SetButtonID (IDC_BUTTON_1);
    testObj2.SetButtonID (IDC_BUTTON_2);
    testObj3.SetButtonID (IDC_BUTTON_3);
    testObj4.SetButtonID (IDC_BUTTON_4);
}

void CATAToolDlg::OnButtonStartAll()
{
	
	MTRACE(g_hEBOOT_DEBUG, "Entry OnButtonStartAll()");

    // TODO: Add your control notification handler code here
    WaitForSingleObject(m_hButtonEvent, 3000);

    for (int i = 0; i < MAX_DUT_NUM; i ++)
    {
        if (comport_setting[i].bActive)
            break;

        if (i == MAX_DUT_NUM - 1)
        {
            MessageBox ("--- No Test DUT! ---", "Warning");
            return;
        }
    }

    if (ata_test_items.b_Barcode || ata_test_items.b_BT || ata_test_items.b_Camera || ata_test_items.b_Charging ||
            ata_test_items.b_EarchPhone || ata_test_items.b_FM || ata_test_items.b_GPS || ata_test_items.b_Keypad ||
            ata_test_items.b_LCD || ata_test_items.b_Receiver || ata_test_items.b_Ring || ata_test_items.b_SIM ||
            ata_test_items.b_Speaker || ata_test_items.b_SWVersion || ata_test_items.b_TCard || ata_test_items.b_TouchScr ||
            ata_test_items.b_TV || ata_test_items.b_Vibrator || ata_test_items.b_WIFI || ata_test_items.b_HWVersion ||
            ata_test_items.b_RTC || ata_test_items.b_ADC || ata_test_items.b_slpmode || ata_test_items.b_RSSI ||
            ata_test_items.b_EMMC || ata_test_items.b_Flash || ata_test_items.b_CMMB||
            ata_test_items.b_msensor || ata_test_items.b_gsensor || ata_test_items.b_gyroscope || ata_test_items.b_alsps || 
            ata_test_items.b_Camera_Main2 ||ata_test_items.b_UsbTypeC || ata_test_items.b_GPSCali || ata_test_items.b_FingerPrint)		// add b_GPSCali
    {
    }
    else
    {
        MessageBox ("--- No Test Item! ---", "Warning");
        return;
    }


    stopFlag1    = -1;
    stopFlag2    = -1;
    stopFlag3    = -1;
    stopFlag4    = -1;

    GetDlgItem(IDC_BUTTON_CONFIG)->EnableWindow(FALSE);
    GetDlgItem(IDC_BUTTON_START_ALL)->EnableWindow(FALSE);
    GetDlgItem(IDC_BUTTON_STOP_ALL)->EnableWindow(TRUE);

    ata_common_cfg1.b_need_polling = false;
    ata_common_cfg2.b_need_polling = false;
    ata_common_cfg3.b_need_polling = false;
    ata_common_cfg4.b_need_polling = false;

    m_hFinishedDUTNum = 0;
    if (comport_setting[0].bActive && !testObj1.isRunning)
    {
        GetDlgItem(IDC_BUTTON_1)->SetWindowText ("Stop");
        testObj1.FillListCtrl ();
        testObj1.StartTestThread ();
    }
    if (comport_setting[1].bActive && !testObj2.isRunning)
    {
        GetDlgItem(IDC_BUTTON_2)->SetWindowText ("Stop");
        testObj2.FillListCtrl ();
        testObj2.StartTestThread ();
    }
    if (comport_setting[2].bActive && !testObj3.isRunning)
    {
        GetDlgItem(IDC_BUTTON_3)->SetWindowText ("Stop");
        testObj3.FillListCtrl ();
        testObj3.StartTestThread ();
    }
    if (comport_setting[3].bActive && !testObj4.isRunning)
    {
        GetDlgItem(IDC_BUTTON_4)->SetWindowText ("Stop");
        testObj4.FillListCtrl ();
        testObj4.StartTestThread ();
    }

    SetEvent (m_hButtonEvent);
}

void CATAToolDlg::OnButtonStopAll()
{
    // TODO: Add your control notification handler code here
    stopFlag1    = ATA_DLL_STOP_FLAG;
    stopFlag2    = ATA_DLL_STOP_FLAG;
    stopFlag3    = ATA_DLL_STOP_FLAG;
    stopFlag4    = ATA_DLL_STOP_FLAG;

    GetDlgItem(IDC_BUTTON_STOP_ALL)->SetWindowText ("Stoping");
    if (comport_setting[0].bActive && testObj1.isRunning)
    {
        testObj1.StopTestThread();
        GetDlgItem(IDC_BUTTON_1)->SetWindowText ("Stopping");
    }
    if (comport_setting[1].bActive && testObj2.isRunning)
    {
        testObj2.StopTestThread ();
        GetDlgItem(IDC_BUTTON_2)->SetWindowText ("Stopping");
    }
    if (comport_setting[2].bActive && testObj3.isRunning)
    {
        testObj3.StopTestThread ();
        GetDlgItem(IDC_BUTTON_3)->SetWindowText ("Stopping");
    }
    if (comport_setting[3].bActive && testObj4.isRunning)
    {
        testObj4.StopTestThread ();
        GetDlgItem(IDC_BUTTON_4)->SetWindowText ("Stopping");
    }
}

void CATAToolDlg::OnSize(UINT nType, int cx, int cy)
{
    CResizableDialog::OnSize(nType, cx, cy);

    // TODO: Add your message handler code here
    // Update ctrl list column width whenever window side change
    testObj1.UpdateListCtrlWidth ();
    testObj2.UpdateListCtrlWidth ();
    testObj3.UpdateListCtrlWidth ();
    testObj4.UpdateListCtrlWidth ();

}

void __stdcall CATAToolDlg::UpdateButtonStatus(int buttonID)
{
    pATAToolDlg->UpdateButtonStatus_Internal (buttonID);
}

void CATAToolDlg::UpdateButtonStatus_Internal(int buttonID)
{
    WaitForSingleObject(m_hButtonEvent, 3000);

    GetDlgItem(buttonID)->SetWindowText ("Start");

    int m_hTestingNum = 0;
    if (testObj1.isRunning)
    {
        m_hTestingNum ++;
    }
    if (testObj2.isRunning)
    {
        m_hTestingNum ++;
    }
    if (testObj3.isRunning)
    {
        m_hTestingNum ++;
    }
    if (testObj4.isRunning)
    {
        m_hTestingNum ++;
    }
    if (m_hTestingNum < 1)
    {
        GetDlgItem(IDC_BUTTON_START_ALL)->EnableWindow(TRUE);
        GetDlgItem(IDC_BUTTON_STOP_ALL)->EnableWindow(FALSE);
        GetDlgItem(IDC_BUTTON_CONFIG)->EnableWindow(TRUE);

        GetDlgItem(IDC_BUTTON_STOP_ALL)->SetWindowText ("Stop All");
    }

    /**
    m_hFinishedDUTNum ++;
    if (m_hFinishedDUTNum >= m_hTestDUTNum)
    {
        GetDlgItem(IDC_BUTTON_START_ALL)->EnableWindow(TRUE);
        GetDlgItem(IDC_BUTTON_STOP_ALL)->EnableWindow(FALSE);
        GetDlgItem(IDC_BUTTON_CONFIG)->EnableWindow(TRUE);

        GetDlgItem(IDC_BUTTON_STOP_ALL)->SetWindowText ("Stop All");
    }
    **/

    SetEvent (m_hButtonEvent);
}

void CATAToolDlg::OnButton1()
{
    // TODO: Add your control notification handler code here
    WaitForSingleObject(m_hButtonEvent, 3000);

    //CString str;
    //GetDlgItemText(IDC_BUTTON_1, str);

    if (!testObj1.isRunning)
    {
        stopFlag1	= -1;
        GetDlgItem(IDC_BUTTON_1)->SetWindowText ("Stop");
        GetDlgItem(IDC_BUTTON_CONFIG)->EnableWindow(FALSE);
        GetDlgItem(IDC_BUTTON_START_ALL)->EnableWindow(FALSE);
        GetDlgItem(IDC_BUTTON_STOP_ALL)->EnableWindow(TRUE);

        testObj1.FillListCtrl ();
        testObj1.StartTestThread ();
    }
    else
    {
        stopFlag1 = ATA_DLL_STOP_FLAG;
        GetDlgItem(IDC_BUTTON_1)->SetWindowText ("Stopping");
        testObj1.StopTestThread();
    }

    SetEvent (m_hButtonEvent);
}

void CATAToolDlg::OnButton2()
{
    // TODO: Add your control notification handler code here
    WaitForSingleObject(m_hButtonEvent, 3000);

    if (!testObj2.isRunning)
    {
        stopFlag2	= -1;
        GetDlgItem(IDC_BUTTON_2)->SetWindowText ("Stop");
        GetDlgItem(IDC_BUTTON_CONFIG)->EnableWindow(FALSE);
        GetDlgItem(IDC_BUTTON_START_ALL)->EnableWindow(FALSE);
        GetDlgItem(IDC_BUTTON_STOP_ALL)->EnableWindow(TRUE);

        testObj2.FillListCtrl ();
        testObj2.StartTestThread ();
    }
    else
    {
        stopFlag2 = ATA_DLL_STOP_FLAG;
        GetDlgItem(IDC_BUTTON_2)->SetWindowText ("Stopping");
        testObj2.StopTestThread();
    }

    SetEvent (m_hButtonEvent);

}

void CATAToolDlg::OnButton3()
{
    // TODO: Add your control notification handler code here
    WaitForSingleObject(m_hButtonEvent, 3000);

    if (!testObj3.isRunning)
    {
        stopFlag3	= -1;
        GetDlgItem(IDC_BUTTON_3)->SetWindowText ("Stop");
        GetDlgItem(IDC_BUTTON_CONFIG)->EnableWindow(FALSE);
        GetDlgItem(IDC_BUTTON_START_ALL)->EnableWindow(FALSE);
        GetDlgItem(IDC_BUTTON_STOP_ALL)->EnableWindow(TRUE);

        testObj3.FillListCtrl ();
        testObj3.StartTestThread ();
    }
    else
    {
        stopFlag3 = ATA_DLL_STOP_FLAG;
        GetDlgItem(IDC_BUTTON_3)->SetWindowText ("Stopping");
        testObj3.StopTestThread();
    }

    SetEvent (m_hButtonEvent);
}

void CATAToolDlg::OnButton4()
{
    // TODO: Add your control notification handler code here
    WaitForSingleObject(m_hButtonEvent, 3000);

    if (!testObj4.isRunning)
    {
        stopFlag4	= -1;
        GetDlgItem(IDC_BUTTON_4)->SetWindowText ("Stop");
        GetDlgItem(IDC_BUTTON_CONFIG)->EnableWindow(FALSE);
        GetDlgItem(IDC_BUTTON_START_ALL)->EnableWindow(FALSE);
        GetDlgItem(IDC_BUTTON_STOP_ALL)->EnableWindow(TRUE);

        testObj4.FillListCtrl ();
        testObj4.StartTestThread ();
    }
    else
    {
        stopFlag4 = ATA_DLL_STOP_FLAG;
        GetDlgItem(IDC_BUTTON_4)->SetWindowText ("Stopping");
        testObj4.StopTestThread();
    }

    SetEvent (m_hButtonEvent);
}

void CATAToolDlg::OnDblclkListDut1(NMHDR* pNMHDR, LRESULT* pResult)
{
    // TODO: Add your control notification handler code here
    LPNMITEMACTIVATE pNMIA = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

    if (testObj1.isRunning && !testObj1.isStepDebugRunning)
    {
        int nItem = -1;
        int nSubItem = -1;
        if (pNMIA)
        {
            nItem = pNMIA->iItem;
            nSubItem = pNMIA->iSubItem;
        }

        if (nItem >= 0 && nSubItem >= 0)
        {
            testObj1.SetStepDebugRow (nItem);
            testObj1.StartTestThread_StepDebug();
        }
    }

    *pResult = 0;
}
