#if !defined(AFX_PROPERTYPAGESENSOR_H__0004D40E_BE00_4F64_8FB3_9C754055B799__INCLUDED_)
#define AFX_PROPERTYPAGESENSOR_H__0004D40E_BE00_4F64_8FB3_9C754055B799__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropertyPageSensor.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPropertyPageSensor dialog

#include "PropertyPageSuperClass.h"


class CPropertyPageSensor : public CPropertyPage, public CProperyPageSuperClass
{
    DECLARE_DYNCREATE(CPropertyPageSensor)

// Construction
public:
    CPropertyPageSensor();
    ~CPropertyPageSensor();

    virtual void UpdateParameters ();
    virtual void InitParameters ();

// Dialog Data
    //{{AFX_DATA(CPropertyPageSensor)
	enum { IDD = IDD_PROPPAGE_MEDIUM_SENSOR };
    float	m_fGsensorMaxX;
    float	m_fGsensorMinX;
    float	m_fGsensorMaxY;
    float	m_fGsensorMinY;
    float	m_fGsensorMaxZ;
    float	m_fGsensorMinZ;
    float	m_fGyroMaxX;
    float	m_fGyroMinX;
    float	m_fGyroMaxY;
    float	m_fGyroMinY;
    float	m_fGyroMaxZ;
    float	m_fGyroMinZ;
    int		m_iMSensorMaxX;
    int		m_iMSensorMinX;
    int		m_iMSensorMaxY;
    int		m_iMSensorMinY;
    int		m_iMSensorMaxZ;
    int		m_iMSensorMinZ;
    int		m_iPSMax;
    int		m_iPSMin;
    int		m_iALSMax;
    int		m_iALSMin;
    BOOL	m_bGSX;
    BOOL	m_bGSY;
    BOOL	m_bGSZ;
    BOOL	m_bGSensorValueDiff;
	BOOL	m_bAls;
	BOOL	m_bPS;
	//}}AFX_DATA


// Overrides
    // ClassWizard generate virtual function overrides
    //{{AFX_VIRTUAL(CPropertyPageSensor)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:
    // Generated message map functions
    //{{AFX_MSG(CPropertyPageSensor)
    // NOTE: the ClassWizard will add member functions here
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPERTYPAGESENSOR_H__0004D40E_BE00_4F64_8FB3_9C754055B799__INCLUDED_)
