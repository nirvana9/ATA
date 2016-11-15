#if !defined(AFX_PROPERTYPAGEPMCAL_H__0C266A3A_F7BB_4600_B739_6E7EF8CD8924__INCLUDED_)
#define AFX_PROPERTYPAGEPMCAL_H__0C266A3A_F7BB_4600_B739_6E7EF8CD8924__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropertyPagePMCal.h : header file
//

#include "..\ATA_DLL\ATA_DLL.h"
#include "PropertyPageSuperClass.h"
#include ".\GlobleSetting.h"
#include <math.h>

/////////////////////////////////////////////////////////////////////////////
// CPropertyPagePMCal dialog
#include "PropertyPageSuperClass.h"

class CPropertyPagePMCal : public CPropertyPage, public CProperyPageSuperClass
{
    DECLARE_DYNCREATE(CPropertyPagePMCal)

// Construction
public:
    CPropertyPagePMCal();
    ~CPropertyPagePMCal();

    virtual void UpdateParameters ();
    virtual void InitParameters ();
    void SetComportSetting (S_COMPORT_ACTIVE_INFO *comport_setting, int length);

// Dialog Data
    //{{AFX_DATA(CPropertyPagePMCal)
    enum { IDD = IDD_PROPPAGE_PM_CAL };
    double	m_dADC11;
    double	m_dADC12;
    double	m_dADC21;
    double	m_dADC22;
    double	m_dADC31;
    double	m_dADC32;
    double	m_dADC42;
    double	m_dADC41;
    double	m_dOffset1;
    double	m_dOffset2;
    double	m_dOffset3;
    double	m_dOffset4;
    double	m_dSlope1;
    double	m_dSlope2;
    double	m_dSlope3;
    double	m_dSlope4;
    double	m_dVol11;
    double	m_dVol12;
    double	m_dVol21;
    double	m_dVol22;
    double	m_dVol31;
    double	m_dVol32;
    double	m_dVol41;
    double	m_dVol42;
    // NOTE - ClassWizard will add data members here.
    //    DO NOT EDIT what you see in these blocks of generated code !
    //}}AFX_DATA


// Overrides
    // ClassWizard generate virtual function overrides
    //{{AFX_VIRTUAL(CPropertyPagePMCal)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:
    // Generated message map functions
    //{{AFX_MSG(CPropertyPagePMCal)
    afx_msg void OnButtonConvert1();
    afx_msg void OnButtonConvert2();
    afx_msg void OnButtonConvert3();
    afx_msg void OnButtonConvert4();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()

protected:
    S_COMPORT_ACTIVE_INFO   *p_sComportSetting;
    int                     comport_setting_len;


};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPERTYPAGEPMCAL_H__0C266A3A_F7BB_4600_B739_6E7EF8CD8924__INCLUDED_)
