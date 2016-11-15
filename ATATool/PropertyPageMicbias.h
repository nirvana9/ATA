#if !defined(AFX_PROPERTYPAGEMICBIAS_H__99E917BC_7097_40C8_BE7E_57FEA40757B2__INCLUDED_)
#define AFX_PROPERTYPAGEMICBIAS_H__99E917BC_7097_40C8_BE7E_57FEA40757B2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropertyPageMicbias.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPropertyPageMicbias dialog
#include "PropertyPageSuperClass.h"

class CPropertyPageMicbias : public CPropertyPage, public CProperyPageSuperClass
{
    DECLARE_DYNCREATE(CPropertyPageMicbias)

// Construction
public:
    CPropertyPageMicbias();
    ~CPropertyPageMicbias();

// Dialog Data
    //{{AFX_DATA(CPropertyPageMicbias)
	enum { IDD = IDD_PROPPAGE_MEDIUM_MICBIAS };
    double	m_dMicbias0Max;
    double	m_dMicbias0Min;
    double	m_dMicbias1Max;
    double	m_dMicbias1Min;
	double	m_dOTGMax;
	double	m_dOTGMin;
	//}}AFX_DATA


// Overrides
    // ClassWizard generate virtual function overrides
    //{{AFX_VIRTUAL(CPropertyPageMicbias)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

public:
    virtual void UpdateParameters ();
    virtual void InitParameters ();

// Implementation
protected:
    // Generated message map functions
    //{{AFX_MSG(CPropertyPageMicbias)
    // NOTE: the ClassWizard will add member functions here
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPERTYPAGEMICBIAS_H__99E917BC_7097_40C8_BE7E_57FEA40757B2__INCLUDED_)
