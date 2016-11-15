#if !defined(AFX_PROPERTYPAGECAMERA_H__D77CB408_0070_41C4_B858_5C069BDA3C78__INCLUDED_)
#define AFX_PROPERTYPAGECAMERA_H__D77CB408_0070_41C4_B858_5C069BDA3C78__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropertyPageCamera.h : header file
//
#include "PropertyPageSuperClass.h"

/////////////////////////////////////////////////////////////////////////////
// CPropertyPageCamera dialog

class CPropertyPageCamera : public CPropertyPage, public CProperyPageSuperClass
{
    DECLARE_DYNCREATE(CPropertyPageCamera)

// Construction
public:
    CPropertyPageCamera();
    ~CPropertyPageCamera();

    virtual void UpdateParameters ();
    virtual void InitParameters ();

// Dialog Data
    //{{AFX_DATA(CPropertyPageCamera)
	enum { IDD = IDD_PROPPAGE_MEDIUM_CAMERA };
	BOOL	m_bCheckSum;
	CString	m_sCheckSumMainCamera;
	CString	m_sCheckSumSubCamera;
	CString	m_sCheckSumMain2Camera;
	//}}AFX_DATA


// Overrides
    // ClassWizard generate virtual function overrides
    //{{AFX_VIRTUAL(CPropertyPageCamera)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:
    // Generated message map functions
    //{{AFX_MSG(CPropertyPageCamera)
    // NOTE: the ClassWizard will add member functions here
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPERTYPAGECAMERA_H__D77CB408_0070_41C4_B858_5C069BDA3C78__INCLUDED_)
