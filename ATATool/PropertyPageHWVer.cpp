// PropertyPageHWVer.cpp : implementation file
//

#include "stdafx.h"
#include "ATATool.h"
#include "PropertyPageHWVer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropertyPageHWVer property page

IMPLEMENT_DYNCREATE(CPropertyPageHWVer, CPropertyPage)

CPropertyPageHWVer::CPropertyPageHWVer() : CPropertyPage(CPropertyPageHWVer::IDD)
{
    //{{AFX_DATA_INIT(CPropertyPageHWVer)
    m_dVolMax = 0.0f;
    m_dVolMin = 0.0f;
    m_iGPIO = 0;
    m_swSubStr = _T("");
    m_Barcode = _T("");
    m_sAPSW = _T("");
    m_iBarcodeFlagIndex = 0;
    m_sPassFlag = _T("");
    m_sFailFlag = _T("");
    //}}AFX_DATA_INIT
}

CPropertyPageHWVer::~CPropertyPageHWVer()
{
}

void CPropertyPageHWVer::DoDataExchange(CDataExchange* pDX)
{
    CPropertyPage::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CPropertyPageHWVer)
    DDX_Text(pDX, IDC_EDIT_VOL_MAX, m_dVolMax);
    DDX_Text(pDX, IDC_EDIT_VOL_MIN, m_dVolMin);
    DDX_Text(pDX, IDC_EDIT_GPIO, m_iGPIO);
    DDX_Text(pDX, IDC_EDIT_SW_SUBSTR, m_swSubStr);
    DDX_Text(pDX, IDC_EDIT_BARCODE, m_Barcode);
    DDV_MaxChars(pDX, m_Barcode, 84);
    DDX_Text(pDX, IDC_EDIT_AP_SW, m_sAPSW);
    DDX_Text(pDX, IDC_EDIT_BARCODE_FLAG_INDEX, m_iBarcodeFlagIndex);
    DDX_Text(pDX, IDC_EDIT_PASS_FLAG, m_sPassFlag);
    DDX_Text(pDX, IDC_EDIT_FAIL_FLAG, m_sFailFlag);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropertyPageHWVer, CPropertyPage)
    //{{AFX_MSG_MAP(CPropertyPageHWVer)
    // NOTE: the ClassWizard will add message map macros here
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropertyPageHWVer message handlers

void CPropertyPageHWVer::UpdateParameters ()
{

    UpdateData (true);
    memcpy (p_sTestSpec->sw_spec.modem_version, m_swSubStr.GetBuffer(0), m_swSubStr.GetLength());
    p_sTestSpec->sw_spec.modem_version[m_swSubStr.GetLength()] = 0;
    memcpy (p_sTestSpec->sw_spec.ap_version, m_sAPSW.GetBuffer(0), m_sAPSW.GetLength());
    p_sTestSpec->sw_spec.ap_version[m_sAPSW.GetLength()] = 0;
    p_sTestSpec->hwver_spec.d_voltage_min = m_dVolMin;
    p_sTestSpec->hwver_spec.d_voltage_max = m_dVolMax;
    p_sTestSpec->hwver_spec.i_gpio        = m_iGPIO;

    memcpy (p_sTestSpec->barcode_spec.barcode, m_Barcode.GetBuffer(0), m_Barcode.GetLength());
    p_sTestSpec->barcode_spec.barcode[m_Barcode.GetLength()] = 0;

    p_sCommonCFG->barcodeFlag_cfg.flagIndex	= m_iBarcodeFlagIndex;
    memcpy (p_sCommonCFG->barcodeFlag_cfg.passFlag, m_sPassFlag.GetBuffer(0), m_sPassFlag.GetLength());
    p_sCommonCFG->barcodeFlag_cfg.passFlag[m_sPassFlag.GetLength()] = 0;
    memcpy (p_sCommonCFG->barcodeFlag_cfg.failFlag, m_sFailFlag.GetBuffer(0), m_sFailFlag.GetLength());
    p_sCommonCFG->barcodeFlag_cfg.passFlag[m_sFailFlag.GetLength()] = 0;
}

void CPropertyPageHWVer::InitParameters ()
{
    m_swSubStr = p_sTestSpec->sw_spec.modem_version;
    m_sAPSW	= p_sTestSpec->sw_spec.ap_version;
    m_Barcode = p_sTestSpec->barcode_spec.barcode;
    m_dVolMax = p_sTestSpec->hwver_spec.d_voltage_max;
    m_dVolMin = p_sTestSpec->hwver_spec.d_voltage_min;
    m_iGPIO   = p_sTestSpec->hwver_spec.i_gpio;

    m_iBarcodeFlagIndex	= p_sCommonCFG->barcodeFlag_cfg.flagIndex;
    m_sPassFlag			= p_sCommonCFG->barcodeFlag_cfg.passFlag;
    m_sFailFlag			= p_sCommonCFG->barcodeFlag_cfg.failFlag;
    UpdateData (false);

}
