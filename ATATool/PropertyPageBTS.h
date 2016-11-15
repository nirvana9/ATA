#if !defined(AFX_PROPERTYPAGEBTS_H__78F95877_7C22_4BB7_A80A_7115A223EBA6__INCLUDED_)
#define AFX_PROPERTYPAGEBTS_H__78F95877_7C22_4BB7_A80A_7115A223EBA6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropertyPageBTS.h : header file
//
#include "PropertyPageSuperClass.h"

/////////////////////////////////////////////////////////////////////////////
// CPropertyPageBTS dialog

class CPropertyPageBTS : public CPropertyPage, public CProperyPageSuperClass
{
	DECLARE_DYNCREATE(CPropertyPageBTS)

// Construction
public:
	CPropertyPageBTS();
	~CPropertyPageBTS();

	virtual void UpdateParameters ();
    virtual void InitParameters ();

// Dialog Data
	//{{AFX_DATA(CPropertyPageBTS)
	enum { IDD = IDD_PROPPAGE_MEDIUM_BTS };
	float	m_iMDPATempMax;
	float	m_iMDPATempMin;
	float	m_iSOCTempMax;
	float	m_iSOCTempMin;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPropertyPageBTS)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPropertyPageBTS)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPERTYPAGEBTS_H__78F95877_7C22_4BB7_A80A_7115A223EBA6__INCLUDED_)
