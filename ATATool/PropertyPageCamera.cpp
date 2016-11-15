// PropertyPageCamera.cpp : implementation file
//

#include "stdafx.h"
#include "ATATool.h"
#include "PropertyPageCamera.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropertyPageCamera property page

IMPLEMENT_DYNCREATE(CPropertyPageCamera, CPropertyPage)

CPropertyPageCamera::CPropertyPageCamera() : CPropertyPage(CPropertyPageCamera::IDD)
{
    //{{AFX_DATA_INIT(CPropertyPageCamera)
	m_bCheckSum = FALSE;
	m_sCheckSumMainCamera = _T("");
	m_sCheckSumSubCamera = _T("");
	m_sCheckSumMain2Camera = _T("");
	//}}AFX_DATA_INIT
}

CPropertyPageCamera::~CPropertyPageCamera()
{
}

void CPropertyPageCamera::DoDataExchange(CDataExchange* pDX)
{
    CPropertyPage::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CPropertyPageCamera)
	DDX_Check(pDX, IDC_CHECK_CHECK_SUM, m_bCheckSum);
	DDX_Text(pDX, IDC_EDIT_CHECKSUM_MAIN_CAMERA, m_sCheckSumMainCamera);
	DDX_Text(pDX, IDC_EDIT_CHECKSUM_SUBCAMERA, m_sCheckSumSubCamera);
	DDX_Text(pDX, IDC_EDIT_CHECKSUM_MAIN_CAMERA2, m_sCheckSumMain2Camera);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropertyPageCamera, CPropertyPage)
    //{{AFX_MSG_MAP(CPropertyPageCamera)
    // NOTE: the ClassWizard will add message map macros here
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropertyPageCamera message handlers

void CPropertyPageCamera::UpdateParameters ()
{
    UpdateData (true);

	p_sCommonCFG->camera_cfg.bCheckSum	= m_bCheckSum;
	
	memcpy (p_sTestSpec->camera_spec.mainCameraCheckSum, m_sCheckSumMainCamera.GetBuffer(0), m_sCheckSumMainCamera.GetLength());
	p_sTestSpec->camera_spec.mainCameraCheckSum[m_sCheckSumMainCamera.GetLength()] = 0;

	memcpy (p_sTestSpec->camera_spec.subCameraCheckSum, m_sCheckSumSubCamera.GetBuffer(0), m_sCheckSumSubCamera.GetLength());
	p_sTestSpec->camera_spec.subCameraCheckSum[m_sCheckSumSubCamera.GetLength()] = 0;

	memcpy (p_sTestSpec->camera_spec.main2CameraCheckSum, m_sCheckSumMain2Camera.GetBuffer(0), m_sCheckSumMain2Camera.GetLength());
	p_sTestSpec->camera_spec.main2CameraCheckSum[m_sCheckSumMain2Camera.GetLength()] = 0;
}
void CPropertyPageCamera::InitParameters ()
{
	m_bCheckSum	= p_sCommonCFG->camera_cfg.bCheckSum;
	m_sCheckSumMainCamera	= p_sTestSpec->camera_spec.mainCameraCheckSum;
	m_sCheckSumSubCamera	= p_sTestSpec->camera_spec.subCameraCheckSum;
	m_sCheckSumMain2Camera	= p_sTestSpec->camera_spec.main2CameraCheckSum;
    UpdateData (false);
}


