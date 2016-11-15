// ChangePwd.cpp : implementation file
//

#include "stdafx.h"
#include "ATATool.h"
#include "ChangePwd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChangePwd dialog


CChangePwd::CChangePwd(CWnd* pParent /*=NULL*/)
    : CDialog(CChangePwd::IDD, pParent)
{
    //{{AFX_DATA_INIT(CChangePwd)
    m_sNewPwd1 = _T("");
    m_sNewPwd2 = _T("");
    m_sOldPwd = _T("");
    //}}AFX_DATA_INIT
}


void CChangePwd::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CChangePwd)
    DDX_Text(pDX, IDC_EDIT_NEW_PWD1, m_sNewPwd1);
    DDX_Text(pDX, IDC_EDIT_NEW_PWD2, m_sNewPwd2);
    DDX_Text(pDX, IDC_EDIT_OLD_PWD, m_sOldPwd);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CChangePwd, CDialog)
    //{{AFX_MSG_MAP(CChangePwd)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChangePwd message handlers
void CChangePwd::SetTestItemCFG(S_ATADLL_COMMON_CFG_T *comm_cfg)
{
    this->comm_cfg = comm_cfg;
}

void CChangePwd::OnOK()
{
    // TODO: Add extra validation here
    UpdateData (TRUE);

    if (strcmp(comm_cfg->logonPasswrd, m_sOldPwd.GetBuffer(0)) != 0)
    {
        MessageBox ("Old password not correct!");
        return;
    }

    if (strcmp(m_sNewPwd1.GetBuffer(0), m_sNewPwd2.GetBuffer(0)) != 0)
    {
        MessageBox ("New password is not equal!");
        return;
    }

    sprintf (comm_cfg->logonPasswrd, "%s", m_sNewPwd1.GetBuffer(0));
    MessageBox ("Change password OK!");

    CDialog::OnOK();
}

BOOL CChangePwd::OnInitDialog()
{
    CDialog::OnInitDialog();

    // TODO: Add extra initialization here
    GetDlgItem(IDC_EDIT_OLD_PWD)->SetFocus();
    return FALSE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}
