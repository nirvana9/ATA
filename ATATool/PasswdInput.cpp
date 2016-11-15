// PasswdInput.cpp : implementation file
//

#include "stdafx.h"
#include "ATATool.h"
#include "PasswdInput.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPasswdInput dialog


CPasswdInput::CPasswdInput(CWnd* pParent /*=NULL*/)
    : CDialog(CPasswdInput::IDD, pParent)
{
    //{{AFX_DATA_INIT(CPasswdInput)
    m_sInputPasswd = _T("");
    //}}AFX_DATA_INIT
}


void CPasswdInput::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CPasswdInput)
    DDX_Text(pDX, IDC_EDIT_PASSWD_INPUT, m_sInputPasswd);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPasswdInput, CDialog)
    //{{AFX_MSG_MAP(CPasswdInput)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPasswdInput message handlers
void CPasswdInput::SetTestItemCFG(S_ATADLL_COMMON_CFG_T *comm_cfg)
{
    this->comm_cfg	= comm_cfg;
}

void CPasswdInput::SetPassFlag(bool *passFlag)
{
    this->passFlag	= passFlag;
}

void CPasswdInput::OnOK()
{
    // TODO: Add extra validation here
    UpdateData(true);
    if (strcmp(comm_cfg->logonPasswrd, "") == 0)
    {
        sprintf (comm_cfg->logonPasswrd, "123456");
    }

    if (strcmp(comm_cfg->logonPasswrd, m_sInputPasswd.GetBuffer(0)) == 0)
    {
        *(this->passFlag)	= true;
    }
    else
    {
        MessageBox ("Invalid password");
        *(this->passFlag)	= false;

        GetDlgItem(IDC_EDIT_PASSWD_INPUT)->SetFocus();
        return;
    }

    CDialog::OnOK();
}

BOOL CPasswdInput::OnInitDialog()
{
    CDialog::OnInitDialog();

    // TODO: Add extra initialization here
    GetDlgItem(IDC_EDIT_PASSWD_INPUT)->SetFocus();
    return FALSE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}
