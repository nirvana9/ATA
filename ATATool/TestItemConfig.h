#if !defined(AFX_TESTITEMCONFIG_H__F749A7E6_7EF1_4457_B3B7_651C065D54E1__INCLUDED_)
#define AFX_TESTITEMCONFIG_H__F749A7E6_7EF1_4457_B3B7_651C065D54E1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TestItemConfig.h : header file
//

#include "..\ATA_DLL\ATA_DLL.h"
#include "PropertyPageWIFI.h"
#include "PropertyPageBT.h"
#include "PropertyPageFM.h"
#include "PropertyPageMATV.h"
#include "PropertyPageGPS.h"
#include "PropertyPageAudio.h"
#include "PropertyPageVibrator.h"
#include "PropertyPageHWVer.h"
//#include "PropertyPageEarthphone.h"
#include "PropertyPageBTS.h"
#include "PropertyPageCamera.h"
#include "PropertyPageKPad.h"
#include "PropertyPagePMCal.h"
#include "PropertyPageLED.h"
#include "PropertyPageSensor.h"
#include "PropertyPageMicbias.h"


#include ".\GlobleSetting.h"
#include "ChangePwd.h"

/////////////////////////////////////////////////////////////////////////////
// CTestItemConfig dialog
extern S_TESTITEM_LOOP_TEST_INFO loop_test_info;

#define USB_PORT_NUM    9999

class CTestItemConfig : public CDialog
{
// Construction
public:
    CTestItemConfig(CWnd* pParent = NULL);   // standard constructor
    virtual ~CTestItemConfig ();

// Dialog Data
    //{{AFX_DATA(CTestItemConfig)
	enum { IDD = IDD_DIALOG_CONFIG };
    CComboBox	m_comboSimNum;
    CComboBox	m_comboFC4;
    CComboBox	m_comboFC3;
    CComboBox	m_comboFC2;
    CComboBox	m_comboCOM4;
    CComboBox	m_comboFC1;
    CComboBox	m_comboCOM3;
    CComboBox	m_comboCOM2;
    CComboBox	m_comboCOM1;
    CComboBox	m_comboBAUD4;
    CComboBox	m_comboBAUD3;
    CComboBox	m_comboBAUD2;
    CComboBox	m_comboBAUD1;
    BOOL	m_bBarcode;
    BOOL	m_bBT;
    BOOL	m_bCharger;
    BOOL	m_bEarthphone;
    BOOL	m_bFM;
    BOOL	m_bGPS;
    BOOL	m_bMATV;
    BOOL	m_bReceiver;
    BOOL	m_bSIM;
    BOOL	m_bSpeaker;
    BOOL	m_bSW;
    BOOL	m_bTCard;
    BOOL	m_bVibrator;
    BOOL	m_bWIFI;
    BOOL	m_checkDUT1;
    BOOL	m_checkDUT2;
    BOOL	m_checkDUT3;
    BOOL	m_checkDUT4;
    BOOL	m_bLCM;
    BOOL	m_bTP;
    BOOL	m_bCamera;
    int		m_iWaitSec;
    BOOL	m_bHWVer;
    BOOL	m_bRTC;
    BOOL	m_bERState;
    BOOL	m_bKPad;
    BOOL	m_bCheckADC;
    BOOL	m_bSlpmode;
    int		m_iLoopTimes;
    int		m_iRetryTimes;
    BOOL	m_bRSSI;
    BOOL	m_bLED;
    BOOL	m_bAutoPoll;
    int		m_iKernelPort1;
    int		m_iKernelPort2;
    int		m_iKernelPort3;
    int		m_iKernelPort4;
    BOOL	m_bEMMC;
    int		m_iPreloaderPort1;
    int		m_iPreloaderPort2;
    int		m_iPreloaderPort3;
    int		m_iPreloaderPort4;
    BOOL	m_bComposite;
    BOOL	m_bCMMB;
    int		m_iEditPMPort1;
    int		m_iEditPMPort2;
    int		m_iEditPMPort3;
    int		m_iEditPMPort4;
    BOOL	m_bGSensor;
    BOOL	m_bGyroscope;
    BOOL	m_bMSensor;
    BOOL	m_bAlsps;
    BOOL	m_bCameraSub;
    BOOL	m_bFlagBarcode;
    BOOL	m_bStopIfFailed;
    BOOL	m_bMicbias;
    BOOL	m_bOffMode;
    BOOL	m_bSelectAll1;
    BOOL	m_bSelectAll2;
    BOOL	m_bSelectAll3;
	BOOL	m_bOTG;
	BOOL	m_bHDMI;
	int		m_iTestItemTimeout;
	BOOL	m_bMHL;
	BOOL	m_bUsbTypeC;
	BOOL	m_bCameraMain2;
	BOOL	m_bHumidity;
	BOOL	m_bBarometer;
	BOOL	m_bBTS;
	BOOL	m_bExtBuck;
	BOOL	m_bFingerPrint;
	BOOL	m_bSelectAll4;
	BOOL	m_bGPSCali;  //add by songjian 2016-3-1
	//}}AFX_DATA


// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CTestItemConfig)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
    //}}AFX_VIRTUAL

// Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CTestItemConfig)
    virtual BOOL OnInitDialog();
    afx_msg void OnSave();
    afx_msg void OnChangepwd();
    virtual void OnCancel();
    afx_msg void OnCheckSelectAll1();
    afx_msg void OnCheckSelectAll2();
    afx_msg void OnCheckSelectAll3();
	afx_msg void OnCheckSelectAll4();
	afx_msg void OnGPS_Cali();  //add by songjian 2016-3-1
	//}}AFX_MSG
    DECLARE_MESSAGE_MAP()

private:

    S_ATADLL_TEST_ITEM_T	*p_sTestItem;
    S_ATADLL_COMMON_CFG_T	*p_sCommonCFG;
    S_ATADLL_TESTITEM_SPEC	*p_sTestItemSpec;

    S_COMPORT_ACTIVE_INFO	*p_sComportSetting;
    int						comport_setting_len;

    CPropertySheet			m_configsheet;
    CPropertyPageFM			m_pageFM;
    CPropertyPageWIFI		m_pageWIFI;
    CPropertyPageBT			m_pageBT;
    CPropertyPageGPS		m_pageGPS;
    CPropertyPageMATV		m_pageMATV;
    CPropertyPageAudio		m_pageAudio;
    CPropertyPageVibrator	m_pageVibrator;
    CPropertyPageHWVer		m_pageHWVer;
    //CPropertyPageEarthphone	m_pageERState;
    CPropertyPageBTS        m_pageBTS;
    CPropertyPageCamera		m_pageCamera;
    CPropertyPageKPad       m_pageKPad;
    CPropertyPagePMCal      m_pagePMCal;
    CPropertyPageLED        m_pageLED;
    CPropertyPageSensor     m_pageSensor;
    CPropertyPageMicbias    m_pageMicbias;

public:
    void SetTestItem (S_ATADLL_TEST_ITEM_T *pTestItem);
    void SetCommonCFG (S_ATADLL_COMMON_CFG_T *pCommonCFG);
    void SetTestItemSpec (S_ATADLL_TESTITEM_SPEC *testItemSpec);
    void SetComportSetting (S_COMPORT_ACTIVE_INFO *comport_setting, int length);
    void UpdateTestItem();

    void EnumerateSerialPorts(CUIntArray& ports);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTITEMCONFIG_H__F749A7E6_7EF1_4457_B3B7_651C065D54E1__INCLUDED_)
