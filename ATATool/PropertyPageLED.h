#if !defined(AFX_PROPERTYPAGELED_H__4459257C_AB8D_43F3_A60A_F1270172B5D1__INCLUDED_)
#define AFX_PROPERTYPAGELED_H__4459257C_AB8D_43F3_A60A_F1270172B5D1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropertyPageLED.h : header file
//

#include "PropertyPageSuperClass.h"
/////////////////////////////////////////////////////////////////////////////
// CPropertyPageLED dialog

class CPropertyPageLED : public CPropertyPage, public CProperyPageSuperClass
{
    DECLARE_DYNCREATE(CPropertyPageLED)

// Construction
public:
    CPropertyPageLED();
    ~CPropertyPageLED();

// Dialog Data
    //{{AFX_DATA(CPropertyPageLED)
    enum { IDD = IDD_PROPPAGE_MEDIUM_LED };
    CString	m_editLEDState;
    //}}AFX_DATA


// Overrides
    // ClassWizard generate virtual function overrides
    //{{AFX_VIRTUAL(CPropertyPageLED)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:
    // Generated message map functions
    //{{AFX_MSG(CPropertyPageLED)
    // NOTE: the ClassWizard will add member functions here
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()

public:
    virtual void UpdateParameters ();
    virtual void InitParameters ();

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPERTYPAGELED_H__4459257C_AB8D_43F3_A60A_F1270172B5D1__INCLUDED_)
