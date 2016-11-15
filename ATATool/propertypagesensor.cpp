// PropertyPageSensor.cpp : implementation file
//

#include "stdafx.h"
#include "ATATool.h"
#include "PropertyPageSensor.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropertyPageSensor property page

IMPLEMENT_DYNCREATE(CPropertyPageSensor, CPropertyPage)

CPropertyPageSensor::CPropertyPageSensor() : CPropertyPage(CPropertyPageSensor::IDD)
{
    //{{AFX_DATA_INIT(CPropertyPageSensor)
    m_fGsensorMaxX = 0.0f;
    m_fGsensorMinX = 0.0f;
    m_fGsensorMaxY = 0.0f;
    m_fGsensorMinY = 0.0f;
    m_fGsensorMaxZ = 0.0f;
    m_fGsensorMinZ = 0.0f;
    m_fGyroMaxX = 0.0f;
    m_fGyroMinX = 0.0f;
    m_fGyroMaxY = 0.0f;
    m_fGyroMinY = 0.0f;
    m_fGyroMaxZ = 0.0f;
    m_fGyroMinZ = 0.0f;
    m_iMSensorMaxX = 0;
    m_iMSensorMinX = 0;
    m_iMSensorMaxY = 0;
    m_iMSensorMinY = 0;
    m_iMSensorMaxZ = 0;
    m_iMSensorMinZ = 0;
    m_iPSMax = 0;
    m_iPSMin = 0;
    m_iALSMax = 0;
    m_iALSMin = 0;
    m_bGSX = FALSE;
    m_bGSY = FALSE;
    m_bGSZ = FALSE;
    m_bGSensorValueDiff = FALSE;
	m_bAls = FALSE;
	m_bPS = FALSE;
	//}}AFX_DATA_INIT
}

CPropertyPageSensor::~CPropertyPageSensor()
{
}

void CPropertyPageSensor::DoDataExchange(CDataExchange* pDX)
{
    CPropertyPage::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CPropertyPageSensor)
    DDX_Text(pDX, IDC_EDIT_GSENSOR_X_MAX, m_fGsensorMaxX);
    DDX_Text(pDX, IDC_EDIT_GSENSOR_X_MIN, m_fGsensorMinX);
    DDX_Text(pDX, IDC_EDIT_GSENSOR_Y_MAX, m_fGsensorMaxY);
    DDX_Text(pDX, IDC_EDIT_GSENSOR_Y_MIN, m_fGsensorMinY);
    DDX_Text(pDX, IDC_EDIT_GSENSOR_Z_MAX, m_fGsensorMaxZ);
    DDX_Text(pDX, IDC_EDIT_GSENSOR_Z_MIN, m_fGsensorMinZ);
    DDX_Text(pDX, IDC_EDIT_GYROSCOPE_X_MAX, m_fGyroMaxX);
    DDX_Text(pDX, IDC_EDIT_GYROSCOPE_X_MIN, m_fGyroMinX);
    DDX_Text(pDX, IDC_EDIT_GYROSCOPE_Y_MAX, m_fGyroMaxY);
    DDX_Text(pDX, IDC_EDIT_GYROSCOPE_Y_MIN, m_fGyroMinY);
    DDX_Text(pDX, IDC_EDIT_GYROSCOPE_Z_MAX, m_fGyroMaxZ);
    DDX_Text(pDX, IDC_EDIT_GYROSCOPE_Z_MIN, m_fGyroMinZ);
    DDX_Text(pDX, IDC_EDIT_MSENSOR_X_MAX, m_iMSensorMaxX);
    DDX_Text(pDX, IDC_EDIT_MSENSOR_X_MIN, m_iMSensorMinX);
    DDX_Text(pDX, IDC_EDIT_MSENSOR_Y_MAX, m_iMSensorMaxY);
    DDX_Text(pDX, IDC_EDIT_MSENSOR_Y_MIN, m_iMSensorMinY);
    DDX_Text(pDX, IDC_EDIT_MSENSOR_Z_MAX, m_iMSensorMaxZ);
    DDX_Text(pDX, IDC_EDIT_MSENSOR_Z_MIN, m_iMSensorMinZ);
    DDX_Text(pDX, IDC_EDIT_PS_MAX, m_iPSMax);
    DDX_Text(pDX, IDC_EDIT_PS_MIN, m_iPSMin);
    DDX_Text(pDX, IDC_EDIT_ALS_MAX, m_iALSMax);
    DDX_Text(pDX, IDC_EDIT_ALS_MIN, m_iALSMin);
    DDX_Check(pDX, IDC_CHECK_GS_X, m_bGSX);
    DDX_Check(pDX, IDC_CHECK_GS_Y, m_bGSY);
    DDX_Check(pDX, IDC_CHECK_GS_Z, m_bGSZ);
    DDX_Check(pDX, IDC_CHECK_GSENSOR_DIFF, m_bGSensorValueDiff);
	DDX_Check(pDX, IDC_CHECK_ALS, m_bAls);
	DDX_Check(pDX, IDC_CHECK_PS, m_bPS);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropertyPageSensor, CPropertyPage)
    //{{AFX_MSG_MAP(CPropertyPageSensor)
    // NOTE: the ClassWizard will add message map macros here
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropertyPageSensor message handlers

void CPropertyPageSensor::UpdateParameters ()
{
    UpdateData (true);

    p_sCommonCFG->gsensor_cfg.bX	= m_bGSX ? true : false;
    p_sCommonCFG->gsensor_cfg.bY	= m_bGSY ? true : false;
    p_sCommonCFG->gsensor_cfg.bZ	= m_bGSZ ? true : false;
    p_sCommonCFG->gsensor_cfg.bGSensorValueDiff	= m_bGSensorValueDiff ? true : false;

	p_sCommonCFG->alsps_cfg.bAls	= m_bAls ? true : false;
	p_sCommonCFG->alsps_cfg.bPs		= m_bPS ? true : false;

    p_sTestSpec->gsensor_spec.max_gsensor.g_sensor_x	= m_fGsensorMaxX;
    p_sTestSpec->gsensor_spec.max_gsensor.g_sensor_y	= m_fGsensorMaxY;
    p_sTestSpec->gsensor_spec.max_gsensor.g_sensor_z	= m_fGsensorMaxZ;
    p_sTestSpec->gsensor_spec.min_gsensor.g_sensor_x	= m_fGsensorMinX;
    p_sTestSpec->gsensor_spec.min_gsensor.g_sensor_y	= m_fGsensorMinY;
    p_sTestSpec->gsensor_spec.min_gsensor.g_sensor_z	= m_fGsensorMinZ;

    p_sTestSpec->msensor_spec.max_msensor.m_sensor_x	= m_iMSensorMaxX;
    p_sTestSpec->msensor_spec.max_msensor.m_sensor_y	= m_iMSensorMaxY;
    p_sTestSpec->msensor_spec.max_msensor.m_sensor_z	= m_iMSensorMaxZ;
    p_sTestSpec->msensor_spec.min_msensor.m_sensor_x	= m_iMSensorMinX;
    p_sTestSpec->msensor_spec.min_msensor.m_sensor_y	= m_iMSensorMinY;
    p_sTestSpec->msensor_spec.min_msensor.m_sensor_z	= m_iMSensorMinZ;

    p_sTestSpec->alsps_spec.max_alsps.als	= m_iALSMax;
    p_sTestSpec->alsps_spec.max_alsps.ps	= m_iPSMax;
    p_sTestSpec->alsps_spec.min_alsps.als	= m_iALSMin;
    p_sTestSpec->alsps_spec.min_alsps.ps	= m_iPSMin;

    p_sTestSpec->gyroscope_spec.max_gyroscope.gyroscope_x	= m_fGyroMaxX;
    p_sTestSpec->gyroscope_spec.max_gyroscope.gyroscope_y	= m_fGyroMaxY;
    p_sTestSpec->gyroscope_spec.max_gyroscope.gyroscope_z	= m_fGyroMaxZ;
    p_sTestSpec->gyroscope_spec.min_gyroscope.gyroscope_x	= m_fGyroMinX;
    p_sTestSpec->gyroscope_spec.min_gyroscope.gyroscope_y	= m_fGyroMinY;
    p_sTestSpec->gyroscope_spec.min_gyroscope.gyroscope_z	= m_fGyroMinZ;

}

void CPropertyPageSensor::InitParameters ()
{
    m_bGSX	= p_sCommonCFG->gsensor_cfg.bX ? 1 : 0;
    m_bGSY	= p_sCommonCFG->gsensor_cfg.bY ? 1 : 0;
    m_bGSZ	= p_sCommonCFG->gsensor_cfg.bZ ? 1 : 0;
    m_bGSensorValueDiff	= p_sCommonCFG->gsensor_cfg.bGSensorValueDiff ? 1 : 0;

	m_bAls	= p_sCommonCFG->alsps_cfg.bAls ? 1 : 0;
	m_bPS	= p_sCommonCFG->alsps_cfg.bPs ? 1 : 0;

    m_fGsensorMaxX	= p_sTestSpec->gsensor_spec.max_gsensor.g_sensor_x;
    m_fGsensorMaxY	= p_sTestSpec->gsensor_spec.max_gsensor.g_sensor_y;
    m_fGsensorMaxZ	= p_sTestSpec->gsensor_spec.max_gsensor.g_sensor_z;
    m_fGsensorMinX	= p_sTestSpec->gsensor_spec.min_gsensor.g_sensor_x;
    m_fGsensorMinY	= p_sTestSpec->gsensor_spec.min_gsensor.g_sensor_y;
    m_fGsensorMinZ	= p_sTestSpec->gsensor_spec.min_gsensor.g_sensor_z;

    m_iMSensorMaxX	= p_sTestSpec->msensor_spec.max_msensor.m_sensor_x;
    m_iMSensorMaxY	= p_sTestSpec->msensor_spec.max_msensor.m_sensor_y;
    m_iMSensorMaxZ	= p_sTestSpec->msensor_spec.max_msensor.m_sensor_z;
    m_iMSensorMinX	= p_sTestSpec->msensor_spec.min_msensor.m_sensor_x;
    m_iMSensorMinY	= p_sTestSpec->msensor_spec.min_msensor.m_sensor_y;
    m_iMSensorMinZ	= p_sTestSpec->msensor_spec.min_msensor.m_sensor_z;

    m_iALSMax	= p_sTestSpec->alsps_spec.max_alsps.als;
    m_iPSMax	= p_sTestSpec->alsps_spec.max_alsps.ps;
    m_iALSMin	= p_sTestSpec->alsps_spec.min_alsps.als;
    m_iPSMin	= p_sTestSpec->alsps_spec.min_alsps.ps;

    m_fGyroMaxX		= p_sTestSpec->gyroscope_spec.max_gyroscope.gyroscope_x;
    m_fGyroMaxY		= p_sTestSpec->gyroscope_spec.max_gyroscope.gyroscope_y;
    m_fGyroMaxZ		= p_sTestSpec->gyroscope_spec.max_gyroscope.gyroscope_z;
    m_fGyroMinX		= p_sTestSpec->gyroscope_spec.min_gyroscope.gyroscope_x;
    m_fGyroMinY		= p_sTestSpec->gyroscope_spec.min_gyroscope.gyroscope_y;
    m_fGyroMinZ		= p_sTestSpec->gyroscope_spec.min_gyroscope.gyroscope_z;

    UpdateData (false);
}