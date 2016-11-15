#if !defined(AFX_PROPERTYPAGEKPAD_H__AF774C77_09E3_496A_A1A0_3F805588DD9A__INCLUDED_)
#define AFX_PROPERTYPAGEKPAD_H__AF774C77_09E3_496A_A1A0_3F805588DD9A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropertyPageKPad.h : header file
//

#include "PropertyPageSuperClass.h"

/////////////////////////////////////////////////////////////////////////////
// CPropertyPageKPad dialog

class CPropertyPageKPad : public CPropertyPage, public CProperyPageSuperClass
{
    DECLARE_DYNCREATE(CPropertyPageKPad)

// Construction
public:
    CPropertyPageKPad();
    ~CPropertyPageKPad();

    virtual void UpdateParameters ();
    virtual void InitParameters ();

// Dialog Data
    //{{AFX_DATA(CPropertyPageKPad)
    enum { IDD = IDD_PROPPAGE_MEDIUM_KPAD };
    BOOL	m_bExterConnection;
    BOOL	m_bElectroMagSwitch;
    //}}AFX_DATA


// Overrides
    // ClassWizard generate virtual function overrides
    //{{AFX_VIRTUAL(CPropertyPageKPad)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:
    // Generated message map functions
    //{{AFX_MSG(CPropertyPageKPad)
    // NOTE: the ClassWizard will add member functions here
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPERTYPAGEKPAD_H__AF774C77_09E3_496A_A1A0_3F805588DD9A__INCLUDED_)
