// PropertyPageBTS.cpp : implementation file
//

#include "stdafx.h"
#include "atatool.h"
#include "PropertyPageBTS.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropertyPageBTS property page

IMPLEMENT_DYNCREATE(CPropertyPageBTS, CPropertyPage)

CPropertyPageBTS::CPropertyPageBTS() : CPropertyPage(CPropertyPageBTS::IDD)
{
	//{{AFX_DATA_INIT(CPropertyPageBTS)
	m_iMDPATempMax = 0.0f;
	m_iMDPATempMin = 0.0f;
	m_iSOCTempMax = 0.0f;
	m_iSOCTempMin = 0.0f;
	//}}AFX_DATA_INIT
}

CPropertyPageBTS::~CPropertyPageBTS()
{
}

void CPropertyPageBTS::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropertyPageBTS)
	DDX_Text(pDX, IDC_EDIT_MDPA_TEMP_MAX, m_iMDPATempMax);
	DDX_Text(pDX, IDC_EDIT_MDPA_TEMP_MIN, m_iMDPATempMin);
	DDX_Text(pDX, IDC_EDIT_SOC_TEMP_MAX, m_iSOCTempMax);
	DDX_Text(pDX, IDC_EDIT_SOC_TEMP_MIN, m_iSOCTempMin);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropertyPageBTS, CPropertyPage)
	//{{AFX_MSG_MAP(CPropertyPageBTS)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropertyPageBTS message handlers
void CPropertyPageBTS::UpdateParameters()
{
    UpdateData (true);
    p_sTestSpec->bts_spec.soc_temp_min = m_iSOCTempMin;
	p_sTestSpec->bts_spec.soc_temp_max = m_iSOCTempMax;
	p_sTestSpec->bts_spec.mdpa_temp_min = m_iMDPATempMin;
	p_sTestSpec->bts_spec.mdpa_temp_max = m_iMDPATempMax;

}

void CPropertyPageBTS::InitParameters ()
{
    m_iSOCTempMin = p_sTestSpec->bts_spec.soc_temp_min;
	m_iSOCTempMax = p_sTestSpec->bts_spec.soc_temp_max;
	m_iMDPATempMin = p_sTestSpec->bts_spec.mdpa_temp_min;
	m_iMDPATempMax = p_sTestSpec->bts_spec.mdpa_temp_max;
    UpdateData (false);
}

