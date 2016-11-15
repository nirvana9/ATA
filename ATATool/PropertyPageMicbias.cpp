// PropertyPageMicbias.cpp : implementation file
//

#include "stdafx.h"
#include "atatool.h"
#include "PropertyPageMicbias.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropertyPageMicbias property page

IMPLEMENT_DYNCREATE(CPropertyPageMicbias, CPropertyPage)

CPropertyPageMicbias::CPropertyPageMicbias() : CPropertyPage(CPropertyPageMicbias::IDD)
{
    //{{AFX_DATA_INIT(CPropertyPageMicbias)
    m_dMicbias0Max = 0.0;
    m_dMicbias0Min = 0.0;
    m_dMicbias1Max = 0.0;
    m_dMicbias1Min = 0.0;
	m_dOTGMax = 0.0;
	m_dOTGMin = 0.0;
	//}}AFX_DATA_INIT
}

CPropertyPageMicbias::~CPropertyPageMicbias()
{
}

void CPropertyPageMicbias::DoDataExchange(CDataExchange* pDX)
{
    CPropertyPage::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CPropertyPageMicbias)
    DDX_Text(pDX, IDC_EDIT_BIAS0_MAX, m_dMicbias0Max);
    DDX_Text(pDX, IDC_EDIT_BIAS0_MIN, m_dMicbias0Min);
    DDX_Text(pDX, IDC_EDIT_BIAS1_MAX, m_dMicbias1Max);
    DDX_Text(pDX, IDC_EDIT_BIAS1_MIN, m_dMicbias1Min);
	DDX_Text(pDX, IDC_EDIT_OTG_MAX, m_dOTGMax);
	DDX_Text(pDX, IDC_EDIT_OTG_MIN, m_dOTGMin);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropertyPageMicbias, CPropertyPage)
    //{{AFX_MSG_MAP(CPropertyPageMicbias)
    // NOTE: the ClassWizard will add message map macros here
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropertyPageMicbias message handlers

void CPropertyPageMicbias::UpdateParameters()
{
    UpdateData (true);

    // common cfg
    p_sTestSpec->micbias_spec.dBias0Max	= m_dMicbias0Max;
    p_sTestSpec->micbias_spec.dBias0Min	= m_dMicbias0Min;
    p_sTestSpec->micbias_spec.dBias1Max	= m_dMicbias1Max;
    p_sTestSpec->micbias_spec.dBias1Min	= m_dMicbias1Min;

	p_sTestSpec->otg_spec.dMinVol		= m_dOTGMin;
	p_sTestSpec->otg_spec.dMaxVol		= m_dOTGMax;
}

void CPropertyPageMicbias::InitParameters ()
{
    m_dMicbias0Max	= p_sTestSpec->micbias_spec.dBias0Max;
    m_dMicbias0Min	= p_sTestSpec->micbias_spec.dBias0Min;
    m_dMicbias1Max	= p_sTestSpec->micbias_spec.dBias1Max;
    m_dMicbias1Min	= p_sTestSpec->micbias_spec.dBias1Min;

	m_dOTGMin		= p_sTestSpec->otg_spec.dMinVol;
	m_dOTGMax		= p_sTestSpec->otg_spec.dMaxVol;

    UpdateData (false);
}

