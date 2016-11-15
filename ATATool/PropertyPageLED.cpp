// PropertyPageLED.cpp : implementation file
//

#include "stdafx.h"
#include "ATATool.h"
#include "PropertyPageLED.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropertyPageLED property page

IMPLEMENT_DYNCREATE(CPropertyPageLED, CPropertyPage)

CPropertyPageLED::CPropertyPageLED() : CPropertyPage(CPropertyPageLED::IDD)
{
    //{{AFX_DATA_INIT(CPropertyPageLED)
    m_editLEDState = _T("");
    //}}AFX_DATA_INIT
}

CPropertyPageLED::~CPropertyPageLED()
{
}

void CPropertyPageLED::DoDataExchange(CDataExchange* pDX)
{
    CPropertyPage::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CPropertyPageLED)
    DDX_Text(pDX, IDC_EDIT_LED_STATE, m_editLEDState);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropertyPageLED, CPropertyPage)
    //{{AFX_MSG_MAP(CPropertyPageLED)
    // NOTE: the ClassWizard will add message map macros here
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropertyPageLED message handlers

void CPropertyPageLED::UpdateParameters ()
{
    UpdateData (true);

    memcpy (p_sTestSpec->led_spec.state, m_editLEDState.GetBuffer(0), m_editLEDState.GetLength());
    p_sTestSpec->led_spec.state[m_editLEDState.GetLength()] = '\0';
}

void CPropertyPageLED::InitParameters ()
{
    m_editLEDState = p_sTestSpec->led_spec.state;
    UpdateData (false);
}


