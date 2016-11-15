#if !defined(AFX_PROPERTYPAGEFM_H__886803BC_9920_4408_8FF5_3D2ABB86D215__INCLUDED_)
#define AFX_PROPERTYPAGEFM_H__886803BC_9920_4408_8FF5_3D2ABB86D215__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropertyPageFM.h : header file
//

#include "PropertyPageSuperClass.h"
/////////////////////////////////////////////////////////////////////////////
// CPropertyPageFM dialog

class CPropertyPageFM : public CPropertyPage, public CProperyPageSuperClass
{
    DECLARE_DYNCREATE(CPropertyPageFM)

// Construction
public:
    CPropertyPageFM();
    ~CPropertyPageFM();

    virtual void UpdateParameters ();
    virtual void InitParameters ();

// Dialog Data
    //{{AFX_DATA(CPropertyPageFM)
	enum { IDD = IDD_PROPPAGE_MEDIUM_FM };
    int		m_FMFreq;
    int		m_FMCQI;
    CString	m_FMChip;
    int		m_rssiDBM;
	int		m_rssiDBMLTE;
	//}}AFX_DATA


// Overrides
    // ClassWizard generate virtual function overrides
    //{{AFX_VIRTUAL(CPropertyPageFM)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:
    // Generated message map functions
    //{{AFX_MSG(CPropertyPageFM)
    // NOTE: the ClassWizard will add member functions here
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPERTYPAGEFM_H__886803BC_9920_4408_8FF5_3D2ABB86D215__INCLUDED_)
