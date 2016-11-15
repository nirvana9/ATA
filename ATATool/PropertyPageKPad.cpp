// PropertyPageKPad.cpp : implementation file
//

#include "stdafx.h"
#include "ATATool.h"
#include "PropertyPageKPad.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropertyPageKPad property page

IMPLEMENT_DYNCREATE(CPropertyPageKPad, CPropertyPage)

CPropertyPageKPad::CPropertyPageKPad() : CPropertyPage(CPropertyPageKPad::IDD)
{
    //{{AFX_DATA_INIT(CPropertyPageKPad)
    m_bExterConnection = FALSE;
    m_bElectroMagSwitch = FALSE;
    //}}AFX_DATA_INIT
}

CPropertyPageKPad::~CPropertyPageKPad()
{
}

void CPropertyPageKPad::DoDataExchange(CDataExchange* pDX)
{
    CPropertyPage::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CPropertyPageKPad)
    DDX_Check(pDX, IDC_CHECK_KEY_EXTER_CONN, m_bExterConnection);
    DDX_Check(pDX, IDC_CHECK_ELEC_MAG_SWITCH, m_bElectroMagSwitch);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropertyPageKPad, CPropertyPage)
    //{{AFX_MSG_MAP(CPropertyPageKPad)
    // NOTE: the ClassWizard will add message map macros here
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropertyPageKPad message handlers
void CPropertyPageKPad::UpdateParameters()
{
    UpdateData(true);
    p_sCommonCFG->kpad_cfg.bExterConnection = m_bExterConnection ? true: false;
    p_sCommonCFG->kpad_cfg.bElectroMagicSwitch = m_bElectroMagSwitch ? true: false;
}

void CPropertyPageKPad::InitParameters()
{
    m_bExterConnection	= p_sCommonCFG->kpad_cfg.bExterConnection;
    m_bElectroMagSwitch = p_sCommonCFG->kpad_cfg.bElectroMagicSwitch;
    UpdateData (false);
}
