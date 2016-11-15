#if !defined(AFX_PROPERTYPAGEGPS_H__6B011835_5A02_4198_A0CE_4C74F66BF9D8__INCLUDED_)
#define AFX_PROPERTYPAGEGPS_H__6B011835_5A02_4198_A0CE_4C74F66BF9D8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropertyPageGPS.h : header file
//

#include "PropertyPageSuperClass.h"
/////////////////////////////////////////////////////////////////////////////
// CPropertyPageGPS dialog

class CPropertyPageGPS : public CPropertyPage, public CProperyPageSuperClass
{
    DECLARE_DYNCREATE(CPropertyPageGPS)

// Construction
public:
    CPropertyPageGPS();
    ~CPropertyPageGPS();

    virtual void UpdateParameters ();
    virtual void InitParameters ();

// Dialog Data
    //{{AFX_DATA(CPropertyPageGPS)
    enum { IDD = IDD_PROPPAGE_MEDIUM_GPS };
    int		m_GPSAcquision;
    int		m_GPSBitsync;
    float	m_GPSCNRMean;
    float	m_GPSCNRSigma;
    float	m_GPSPhaseRatio;
    int		m_GPSSvid;
    float	m_GPSTXCODrift;
    float	m_GPSTXCOOffset;
    float	m_GPSUpdateHZ;
    //}}AFX_DATA


// Overrides
    // ClassWizard generate virtual function overrides
    //{{AFX_VIRTUAL(CPropertyPageGPS)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:
    // Generated message map functions
    //{{AFX_MSG(CPropertyPageGPS)
    // NOTE: the ClassWizard will add member functions here
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPERTYPAGEGPS_H__6B011835_5A02_4198_A0CE_4C74F66BF9D8__INCLUDED_)
