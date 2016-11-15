#if !defined(AFX_CHANGEPWD_H__06FA7AF6_A4E5_40B1_BEC0_05E14CE7D13D__INCLUDED_)
#define AFX_CHANGEPWD_H__06FA7AF6_A4E5_40B1_BEC0_05E14CE7D13D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ChangePwd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CChangePwd dialog
#include "..\ATA_DLL\ATA_DLL.h"

class CChangePwd : public CDialog
{
// Construction
public:
    CChangePwd(CWnd* pParent = NULL);   // standard constructor
    void SetTestItemCFG(S_ATADLL_COMMON_CFG_T *comm_cfg);

// Dialog Data
    //{{AFX_DATA(CChangePwd)
    enum { IDD = IDD_DIALOG_CHANGE_PWD };
    CString	m_sNewPwd1;
    CString	m_sNewPwd2;
    CString	m_sOldPwd;
    //}}AFX_DATA


// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CChangePwd)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CChangePwd)
    virtual void OnOK();
    virtual BOOL OnInitDialog();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()

private:
    S_ATADLL_COMMON_CFG_T *comm_cfg;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHANGEPWD_H__06FA7AF6_A4E5_40B1_BEC0_05E14CE7D13D__INCLUDED_)
