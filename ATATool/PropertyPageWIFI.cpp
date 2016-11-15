// PropertyPageWIFI.cpp : implementation file
//

#include "stdafx.h"
#include "ATATool.h"
#include "PropertyPageWIFI.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropertyPageWIFI property page

IMPLEMENT_DYNCREATE(CPropertyPageWIFI, CPropertyPage)

CPropertyPageWIFI::CPropertyPageWIFI() : CPropertyPage(CPropertyPageWIFI::IDD)
{
    //{{AFX_DATA_INIT(CPropertyPageWIFI)
    m_WIFIMac = _T("");
    m_WIFIRssi = 0;
	m_bCheckWIFIMAC = FALSE;
	//}}AFX_DATA_INIT
}

CPropertyPageWIFI::~CPropertyPageWIFI()
{
}

void CPropertyPageWIFI::DoDataExchange(CDataExchange* pDX)
{
    CPropertyPage::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CPropertyPageWIFI)
    DDX_Text(pDX, IDC_EDIT_WIFI_MAC, m_WIFIMac);
    DDX_Text(pDX, IDC_EDIT_WIFI_RSSI, m_WIFIRssi);
    DDV_MinMaxInt(pDX, m_WIFIRssi, -100, 100);
	DDX_Check(pDX, IDC_CHECK_WIFI_MAC, m_bCheckWIFIMAC);
	//}}AFX_DATA_MAP
}

void CPropertyPageWIFI::UpdateParameters()
{
    UpdateData (true);

    memcpy (p_sTestSpec->wifi_spec.local_bssid, m_WIFIMac.GetBuffer(0), m_WIFIMac.GetLength());
    p_sTestSpec->wifi_spec.local_bssid[m_WIFIMac.GetLength()] = '\0';
	
    p_sTestSpec->wifi_spec.ap_list[0].rssi      = m_WIFIRssi;
    p_sTestSpec->wifi_spec.ap_list_num = 1;

	p_sCommonCFG->wifi_cfg.bWIFIMacCheck		= m_bCheckWIFIMAC ? true : false;
}

void CPropertyPageWIFI::InitParameters ()
{
    m_WIFIMac   = p_sTestSpec->wifi_spec.local_bssid;
    m_WIFIRssi  = p_sTestSpec->wifi_spec.ap_list[0].rssi;
	m_bCheckWIFIMAC	= p_sCommonCFG->wifi_cfg.bWIFIMacCheck;

    UpdateData (false);
}


BEGIN_MESSAGE_MAP(CPropertyPageWIFI, CPropertyPage)
    //{{AFX_MSG_MAP(CPropertyPageWIFI)
    // NOTE: the ClassWizard will add message map macros here
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropertyPageWIFI message handlers
