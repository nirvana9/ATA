#if !defined(AFX_PROPERTYPAGEMATV_H__7AD089CE_D206_40E2_845B_041AE3E68381__INCLUDED_)
#define AFX_PROPERTYPAGEMATV_H__7AD089CE_D206_40E2_845B_041AE3E68381__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropertyPageMATV.h : header file
//

#include "PropertyPageSuperClass.h"
/////////////////////////////////////////////////////////////////////////////
// CPropertyPageMATV dialog

class CPropertyPageMATV : public CPropertyPage, public CProperyPageSuperClass
{
    DECLARE_DYNCREATE(CPropertyPageMATV)

// Construction
public:
    CPropertyPageMATV();
    ~CPropertyPageMATV();

// Dialog Data
    //{{AFX_DATA(CPropertyPageMATV)
    enum { IDD = IDD_PROPPAGE_MEDIUM_MATV };
    CComboBox	m_comboCountry;
    CString	m_MATVChip;
    long	m_MATVFreq;
    //}}AFX_DATA


// Overrides
    // ClassWizard generate virtual function overrides
    //{{AFX_VIRTUAL(CPropertyPageMATV)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
public:
    virtual void UpdateParameters ();
    virtual void InitParameters ();

protected:
    // Generated message map functions
    //{{AFX_MSG(CPropertyPageMATV)
    virtual BOOL OnInitDialog();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()

protected:

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPERTYPAGEMATV_H__7AD089CE_D206_40E2_845B_041AE3E68381__INCLUDED_)
