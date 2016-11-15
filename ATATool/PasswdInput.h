#if !defined(AFX_PASSWDINPUT_H__EAAE3EF6_E5BA_4C11_90AB_3DF674187474__INCLUDED_)
#define AFX_PASSWDINPUT_H__EAAE3EF6_E5BA_4C11_90AB_3DF674187474__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PasswdInput.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPasswdInput dialog
#include "..\ATA_DLL\ATA_DLL.h"

class CPasswdInput : public CDialog
{
// Construction
public:
    CPasswdInput(CWnd* pParent = NULL);   // standard constructor
    void SetTestItemCFG(S_ATADLL_COMMON_CFG_T *comm_cfg);
    void SetPassFlag(bool *passFlag);

// Dialog Data
    //{{AFX_DATA(CPasswdInput)
    enum { IDD = IDD_DIALOG_INPUT_PASSWD };
    CString	m_sInputPasswd;
    //}}AFX_DATA


// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CPasswdInput)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CPasswdInput)
    virtual void OnOK();
    virtual BOOL OnInitDialog();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()

private:
    S_ATADLL_COMMON_CFG_T *comm_cfg;
    bool                  *passFlag;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PASSWDINPUT_H__EAAE3EF6_E5BA_4C11_90AB_3DF674187474__INCLUDED_)
