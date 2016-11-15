#if !defined(AFX_PROPERTYPAGEWIFI_H__FA47C6C3_C4F0_49AB_A63A_9C9699EA4A7B__INCLUDED_)
#define AFX_PROPERTYPAGEWIFI_H__FA47C6C3_C4F0_49AB_A63A_9C9699EA4A7B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropertyPageWIFI.h : header file
//

#include "PropertyPageSuperClass.h"
/////////////////////////////////////////////////////////////////////////////
// CPropertyPageWIFI dialog

class CPropertyPageWIFI : public CPropertyPage, public CProperyPageSuperClass
{
    DECLARE_DYNCREATE(CPropertyPageWIFI)

// Construction
public:
    CPropertyPageWIFI();
    ~CPropertyPageWIFI();

    virtual void UpdateParameters ();
    virtual void InitParameters ();

// Dialog Data
    //{{AFX_DATA(CPropertyPageWIFI)
	enum { IDD = IDD_PROPPAGE_MEDIUM_WIFI };
    CString	m_WIFIMac;
    int		m_WIFIRssi;
	BOOL	m_bCheckWIFIMAC;
	//}}AFX_DATA


// Overrides
    // ClassWizard generate virtual function overrides
    //{{AFX_VIRTUAL(CPropertyPageWIFI)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:
    // Generated message map functions
    //{{AFX_MSG(CPropertyPageWIFI)
    // NOTE: the ClassWizard will add member functions here
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPERTYPAGEWIFI_H__FA47C6C3_C4F0_49AB_A63A_9C9699EA4A7B__INCLUDED_)
