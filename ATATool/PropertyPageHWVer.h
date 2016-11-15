#if !defined(AFX_PROPERTYPAGEHWVER_H__A6D7305E_2D5B_4424_811A_38102AA7F2E4__INCLUDED_)
#define AFX_PROPERTYPAGEHWVER_H__A6D7305E_2D5B_4424_811A_38102AA7F2E4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropertyPageHWVer.h : header file
//

#include "PropertyPageSuperClass.h"
/////////////////////////////////////////////////////////////////////////////
// CPropertyPageHWVer dialog

class CPropertyPageHWVer : public CPropertyPage, public CProperyPageSuperClass
{
    DECLARE_DYNCREATE(CPropertyPageHWVer)

// Construction
public:
    CPropertyPageHWVer();
    ~CPropertyPageHWVer();

// Dialog Data
    //{{AFX_DATA(CPropertyPageHWVer)
    enum { IDD = IDD_PROPPAGE_MEDIUM_HWVER };
    float	m_dVolMax;
    float	m_dVolMin;
    int		m_iGPIO;
    CString	m_swSubStr;
    CString	m_Barcode;
    CString	m_sAPSW;
    int		m_iBarcodeFlagIndex;
    CString	m_sPassFlag;
    CString	m_sFailFlag;
    //}}AFX_DATA

public:
    virtual void UpdateParameters ();
    virtual void InitParameters ();


// Overrides
    // ClassWizard generate virtual function overrides
    //{{AFX_VIRTUAL(CPropertyPageHWVer)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:
    // Generated message map functions
    //{{AFX_MSG(CPropertyPageHWVer)
    // NOTE: the ClassWizard will add member functions here
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPERTYPAGEHWVER_H__A6D7305E_2D5B_4424_811A_38102AA7F2E4__INCLUDED_)
