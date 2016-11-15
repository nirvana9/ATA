// PropertyPageFM.cpp : implementation file
//

#include "stdafx.h"
#include "ATATool.h"
#include "PropertyPageFM.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropertyPageFM property page

IMPLEMENT_DYNCREATE(CPropertyPageFM, CPropertyPage)

CPropertyPageFM::CPropertyPageFM() : CPropertyPage(CPropertyPageFM::IDD)
{
    //{{AFX_DATA_INIT(CPropertyPageFM)
    m_FMFreq = 0;
    m_FMCQI = 0;
    m_FMChip = _T("");
    m_rssiDBM = 0;
	m_rssiDBMLTE = 0;
	//}}AFX_DATA_INIT
}

CPropertyPageFM::~CPropertyPageFM()
{
}

void CPropertyPageFM::DoDataExchange(CDataExchange* pDX)
{
    CPropertyPage::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CPropertyPageFM)
    DDX_Text(pDX, IDC_EDIT_FM_FREQ, m_FMFreq);
    DDX_Text(pDX, IDC_EDIT_FM_CQI, m_FMCQI);
    DDV_MinMaxInt(pDX, m_FMCQI, -1000, 1000);
    DDX_Text(pDX, IDC_EDIT_FM_CHIP, m_FMChip);
    DDX_Text(pDX, IDC_EDIT_RSSI, m_rssiDBM);
	DDV_MinMaxInt(pDX, m_rssiDBM, -1000, 1000);
	DDX_Text(pDX, IDC_EDIT_RSSI_LTE, m_rssiDBMLTE);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPropertyPageFM, CPropertyPage)
    //{{AFX_MSG_MAP(CPropertyPageFM)
    // NOTE: the ClassWizard will add message map macros here
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropertyPageFM message handlers
void CPropertyPageFM::UpdateParameters()
{
    UpdateData (true);

    //FM common cfg
    p_sCommonCFG->fm_freq   = m_FMFreq;

    //FM spec
    memcpy (p_sTestSpec->fm_spec.fm_chip, m_FMChip.GetBuffer(0), m_FMChip.GetLength());
    p_sTestSpec->fm_spec.fm_chip[m_FMChip.GetLength()] =   '\0';
    p_sTestSpec->fm_spec.min_cqi    = m_FMCQI;

    //RSSI spec
    p_sTestSpec->rssi_spec.min_dBm = m_rssiDBM;
	p_sTestSpec->rssi_spec.min_dBm_lte = m_rssiDBMLTE;

}

void CPropertyPageFM::InitParameters ()
{

    m_FMFreq    = p_sCommonCFG->fm_freq;
    m_FMChip    = p_sTestSpec->fm_spec.fm_chip;
    m_FMCQI     = p_sTestSpec->fm_spec.min_cqi;

    m_rssiDBM	= p_sTestSpec->rssi_spec.min_dBm;
	m_rssiDBMLTE	= p_sTestSpec->rssi_spec.min_dBm_lte;
    UpdateData (false);
}



