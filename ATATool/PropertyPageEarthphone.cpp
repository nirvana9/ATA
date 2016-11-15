// PropertyPageEarthphone.cpp : implementation file
//

#include "stdafx.h"
#include "ATATool.h"
#include "PropertyPageEarthphone.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropertyPageEarthphone property page

IMPLEMENT_DYNCREATE(CPropertyPageEarthphone, CPropertyPage)

CPropertyPageEarthphone::CPropertyPageEarthphone() : CPropertyPage(CPropertyPageEarthphone::IDD)
{
    //{{AFX_DATA_INIT(CPropertyPageEarthphone)
    // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
}

CPropertyPageEarthphone::~CPropertyPageEarthphone()
{
}

void CPropertyPageEarthphone::DoDataExchange(CDataExchange* pDX)
{
    CPropertyPage::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CPropertyPageEarthphone)
    DDX_Control(pDX, IDC_COMBO_STATUS, m_comboERState);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropertyPageEarthphone, CPropertyPage)
    //{{AFX_MSG_MAP(CPropertyPageEarthphone)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropertyPageEarthphone message handlers

void CPropertyPageEarthphone::UpdateParameters ()
{
    UpdateData (true);
    p_sTestSpec->er_state_spec.state = m_comboERState.GetCurSel();
}

void CPropertyPageEarthphone::InitParameters ()
{
    m_comboERState.SetCurSel(p_sTestSpec->er_state_spec.state);
    UpdateData (false);
}

BOOL CPropertyPageEarthphone::OnInitDialog()
{
    CPropertyPage::OnInitDialog();

    // TODO: Add extra initialization here
    m_comboERState.AddString ("Plug out");
    m_comboERState.AddString ("Plug in");
    m_comboERState.AddString ("Mic in");
    m_comboERState.AddString ("Hook switch pressed");

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}
