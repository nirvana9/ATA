// PropertyPageBT.cpp : implementation file
//

#include "stdafx.h"
#include "ATATool.h"
#include "PropertyPageBT.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropertyPageBT property page

IMPLEMENT_DYNCREATE(CPropertyPageBT, CPropertyPage)

CPropertyPageBT::CPropertyPageBT() : CPropertyPage(CPropertyPageBT::IDD)
{
    //{{AFX_DATA_INIT(CPropertyPageBT)
    m_BTAddr = _T("");
	/*zishuo20150727*/
	m_BTHostAddr = _T("");
	m_bCheckBTAddr = FALSE;
	/*end*/
    //}}AFX_DATA_INIT
}

CPropertyPageBT::~CPropertyPageBT()
{
}

void CPropertyPageBT::DoDataExchange(CDataExchange* pDX)
{
    CPropertyPage::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CPropertyPageBT)
    DDX_Control(pDX, IDC_COMBO_SPEC_TYPE, m_comboSpecType);
    DDX_Text(pDX, IDC_EDIT_BT_ADDR, m_BTAddr);
	/*zishuo20150727*/
	DDX_Check(pDX, IDC_CHECK_BT_ADDR, m_bCheckBTAddr);
	DDX_Text(pDX, IDC_EDIT_BT_HOSTADDR, m_BTHostAddr);
	/*end*/
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropertyPageBT, CPropertyPage)
    //{{AFX_MSG_MAP(CPropertyPageBT)
    // NOTE: the ClassWizard will add message map macros here
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropertyPageBT message handlers

void CPropertyPageBT::UpdateParameters()
{
    UpdateData (true);
    sprintf (p_sTestSpec->bt_spec.addr, m_BTAddr.GetBuffer(0), m_BTAddr.GetLength());
    p_sTestSpec->bt_spec.addr[m_BTAddr.GetLength()] = '\0';
    p_sCommonCFG->bt_spec_type = p_sTestSpec->bt_spec.specType = m_comboSpecType.GetCurSel();
	/*zishuo20150727*/
	p_sCommonCFG->bt_cfg.bBtAddrCheck = m_bCheckBTAddr ? true : false;
    memcpy (p_sTestSpec->bt_spec.local_bssid, m_BTHostAddr.GetBuffer(0), m_BTHostAddr.GetLength());
	p_sTestSpec->bt_spec.local_bssid[m_BTHostAddr.GetLength()] = '\0';
	/*end*/
}

void CPropertyPageBT::InitParameters ()
{
    m_BTAddr = p_sTestSpec->bt_spec.addr;
    m_comboSpecType.SetCurSel (p_sTestSpec->bt_spec.specType);
	/*zishuo20150727*/
	m_bCheckBTAddr =p_sCommonCFG->bt_cfg.bBtAddrCheck;
	m_BTHostAddr=p_sTestSpec->bt_spec.local_bssid;
	/*end*/
    UpdateData (false);
}

