#if !defined(AFX_PROPERTYPAGEVIBRATOR_H__DABE99E8_E27D_4F01_9860_40884684B51B__INCLUDED_)
#define AFX_PROPERTYPAGEVIBRATOR_H__DABE99E8_E27D_4F01_9860_40884684B51B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropertyPageVibrator.h : header file
//

#include "PropertyPageSuperClass.h"
/////////////////////////////////////////////////////////////////////////////
// CPropertyPageVibrator dialog

class CPropertyPageVibrator : public CPropertyPage, public CProperyPageSuperClass
{
    DECLARE_DYNCREATE(CPropertyPageVibrator)

// Construction
public:
    CPropertyPageVibrator();
    ~CPropertyPageVibrator();

    virtual void UpdateParameters ();
    virtual void InitParameters ();

// Dialog Data
    //{{AFX_DATA(CPropertyPageVibrator)
    enum { IDD = IDD_PROPPAGE_MEDIUM_VIBRATOR };
    double	m_VibratorCurrentMax;
    double	m_VibratorCurrentMin;
    double	m_VibratorCurrentDiffMin;
    CString	m_PSUGpibAddr;
    double	m_adcMaxDiff;
    double	m_dSlpmodeMaxCurr;
    CString	m_PSUGpibAddr2;
    CString	m_PSUGpibAddr3;
    CString	m_PSUGpibAddr4;
    double	m_dChargerMin;
    double	m_dChargerMax;
    double	m_dOffModeMaxCurrent;
    BOOL	m_bVibratorOnBoard;
    //}}AFX_DATA


// Overrides
    // ClassWizard generate virtual function overrides
    //{{AFX_VIRTUAL(CPropertyPageVibrator)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:
    // Generated message map functions
    //{{AFX_MSG(CPropertyPageVibrator)
    // NOTE: the ClassWizard will add member functions here
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPERTYPAGEVIBRATOR_H__DABE99E8_E27D_4F01_9860_40884684B51B__INCLUDED_)
