#if !defined(AFX_PROPERTYPAGEBT_H__12CDEFF0_9614_4CE4_8419_1F47A7C52B48__INCLUDED_)
#define AFX_PROPERTYPAGEBT_H__12CDEFF0_9614_4CE4_8419_1F47A7C52B48__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropertyPageBT.h : header file
//

#include "PropertyPageSuperClass.h"

/////////////////////////////////////////////////////////////////////////////
// CPropertyPageBT dialog

class CPropertyPageBT : public CPropertyPage, public CProperyPageSuperClass
{
    DECLARE_DYNCREATE(CPropertyPageBT)

// Construction
public:
    CPropertyPageBT();
    ~CPropertyPageBT();

    virtual void UpdateParameters ();
    virtual void InitParameters ();

// Dialog Data
    //{{AFX_DATA(CPropertyPageBT)
    enum { IDD = IDD_PROPPAGE_MEDIUM_BT };
    CComboBox	m_comboSpecType;
    CString	m_BTAddr;
	
	/*zishuo20150727*/
	BOOL m_bCheckBTAddr;   
	CString	m_BTHostAddr;
	
	/*end*/
    //}}AFX_DATA


// Overrides
    // ClassWizard generate virtual function overrides
    //{{AFX_VIRTUAL(CPropertyPageBT)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:
    // Generated message map functions
    //{{AFX_MSG(CPropertyPageBT)
    // NOTE: the ClassWizard will add member functions here
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPERTYPAGEBT_H__12CDEFF0_9614_4CE4_8419_1F47A7C52B48__INCLUDED_)
