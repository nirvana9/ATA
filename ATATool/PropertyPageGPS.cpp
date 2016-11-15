// PropertyPageGPS.cpp : implementation file
//

#include "stdafx.h"
#include "ATATool.h"
#include "PropertyPageGPS.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropertyPageGPS property page

IMPLEMENT_DYNCREATE(CPropertyPageGPS, CPropertyPage)

CPropertyPageGPS::CPropertyPageGPS() : CPropertyPage(CPropertyPageGPS::IDD)
{
    //{{AFX_DATA_INIT(CPropertyPageGPS)
    m_GPSAcquision = 0;
    m_GPSBitsync = 0;
    m_GPSCNRMean = 0.0f;
    m_GPSCNRSigma = 0.0f;
    m_GPSPhaseRatio = 0.0f;
    m_GPSSvid = 0;
    m_GPSTXCODrift = 0.0f;
    m_GPSTXCOOffset = 0.0f;
    m_GPSUpdateHZ = 0.0f;
    //}}AFX_DATA_INIT
}

CPropertyPageGPS::~CPropertyPageGPS()
{
}

void CPropertyPageGPS::DoDataExchange(CDataExchange* pDX)
{
    CPropertyPage::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CPropertyPageGPS)
    DDX_Text(pDX, IDC_EDIT_GPS_ACQUISION, m_GPSAcquision);
    DDX_Text(pDX, IDC_EDIT_GPS_BIT_SYNC, m_GPSBitsync);
    DDX_Text(pDX, IDC_EDIT_GPS_CNR_MEAN, m_GPSCNRMean);
    DDX_Text(pDX, IDC_EDIT_GPS_CNR_SIGMA, m_GPSCNRSigma);
    DDX_Text(pDX, IDC_EDIT_GPS_PHASE_RATIO, m_GPSPhaseRatio);
    DDX_Text(pDX, IDC_EDIT_GPS_SVID, m_GPSSvid);
    DDX_Text(pDX, IDC_EDIT_GPS_TXCO_DRIFT, m_GPSTXCODrift);
    DDX_Text(pDX, IDC_EDIT_GPS_TXCO_OFFSET, m_GPSTXCOOffset);
    DDX_Text(pDX, IDC_EDIT_GPS_UPDATE_HZ, m_GPSUpdateHZ);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropertyPageGPS, CPropertyPage)
    //{{AFX_MSG_MAP(CPropertyPageGPS)
    // NOTE: the ClassWizard will add message map macros here
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropertyPageGPS message handlers
void CPropertyPageGPS::UpdateParameters()
{
    UpdateData (true);

    // common cfg
    p_sCommonCFG->gpsSVID   = m_GPSSvid;

    // spec
    p_sTestSpec->gps_spec.acquision     = m_GPSAcquision;
    p_sTestSpec->gps_spec.bitsync       = m_GPSBitsync;
    p_sTestSpec->gps_spec.cnr_mean      = m_GPSCNRMean;
    p_sTestSpec->gps_spec.cnr_sigma     = m_GPSCNRSigma;
    p_sTestSpec->gps_spec.phase_ratio   = m_GPSPhaseRatio;
    p_sTestSpec->gps_spec.tcxo_drift    = m_GPSTXCODrift;
    p_sTestSpec->gps_spec.tcxo_offset   = m_GPSTXCOOffset;
    p_sTestSpec->gps_spec.update_HZ     = m_GPSUpdateHZ;
}

void CPropertyPageGPS::InitParameters ()
{
    m_GPSSvid       = p_sCommonCFG->gpsSVID;

    // spec
    m_GPSAcquision  = p_sTestSpec->gps_spec.acquision;
    m_GPSBitsync    = p_sTestSpec->gps_spec.bitsync;
    m_GPSCNRMean    = p_sTestSpec->gps_spec.cnr_mean;
    m_GPSCNRSigma   = p_sTestSpec->gps_spec.cnr_sigma;
    m_GPSPhaseRatio = p_sTestSpec->gps_spec.phase_ratio;
    m_GPSTXCODrift  = p_sTestSpec->gps_spec.tcxo_drift;
    m_GPSTXCOOffset = p_sTestSpec->gps_spec.tcxo_offset;
    m_GPSUpdateHZ   = p_sTestSpec->gps_spec.update_HZ;

    UpdateData (false);
}


