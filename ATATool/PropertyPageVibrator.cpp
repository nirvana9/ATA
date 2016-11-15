// PropertyPageVibrator.cpp : implementation file
//

#include "stdafx.h"
#include "ATATool.h"
#include "PropertyPageVibrator.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropertyPageVibrator property page

IMPLEMENT_DYNCREATE(CPropertyPageVibrator, CPropertyPage)

CPropertyPageVibrator::CPropertyPageVibrator() : CPropertyPage(CPropertyPageVibrator::IDD)
{
    //{{AFX_DATA_INIT(CPropertyPageVibrator)
    m_VibratorCurrentMax = 0.0;
    m_VibratorCurrentMin = 0.0;
    m_VibratorCurrentDiffMin = 0.0;
    m_PSUGpibAddr = _T("");
    m_adcMaxDiff = 0.0;
    m_dSlpmodeMaxCurr = 0.0;
    m_PSUGpibAddr2 = _T("");
    m_PSUGpibAddr3 = _T("");
    m_PSUGpibAddr4 = _T("");
    m_dChargerMin = 0.0;
    m_dChargerMax = 0.0;
    m_dOffModeMaxCurrent = 0.0;
    m_bVibratorOnBoard = FALSE;
    //}}AFX_DATA_INIT
}

CPropertyPageVibrator::~CPropertyPageVibrator()
{
}

void CPropertyPageVibrator::DoDataExchange(CDataExchange* pDX)
{
    CPropertyPage::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CPropertyPageVibrator)
    DDX_Text(pDX, IDC_EDIT_VIBRATOR_CURR_MAX, m_VibratorCurrentMax);
    DDX_Text(pDX, IDC_EDIT_VIBRATOR_CURR_MIN, m_VibratorCurrentMin);
    DDX_Text(pDX, IDC_EDIT_VIBRATOR_CURRENT_DIFF_MIN, m_VibratorCurrentDiffMin);
    DDX_Text(pDX, IDC_EDIT_PSU_GPIB_ADDR, m_PSUGpibAddr);
    DDX_Text(pDX, IDC_EDIT_ADC_MAX_DIFF, m_adcMaxDiff);
    DDX_Text(pDX, IDC_EDIT_SLPMOD_CURRENT, m_dSlpmodeMaxCurr);
    DDX_Text(pDX, IDC_EDIT_PSU_GPIB_ADDR2, m_PSUGpibAddr2);
    DDX_Text(pDX, IDC_EDIT_PSU_GPIB_ADDR3, m_PSUGpibAddr3);
    DDX_Text(pDX, IDC_EDIT_PSU_GPIB_ADDR4, m_PSUGpibAddr4);
    DDX_Text(pDX, IDC_EDIT_CHRGR_MIN, m_dChargerMin);
    DDX_Text(pDX, IDC_EDIT1_CHRGR_MAX, m_dChargerMax);
    DDX_Text(pDX, IDC_EDIT_OFFMOD_CURRENT, m_dOffModeMaxCurrent);
    DDX_Check(pDX, IDC_CHECK_VIBRATOR_ON_BOARD, m_bVibratorOnBoard);
    //}}AFX_DATA_MAP
}

void CPropertyPageVibrator::UpdateParameters()
{
    UpdateData (true);

    p_sCommonCFG->vibrator_cfg.bOnBoard 	= m_bVibratorOnBoard ? 1 : 0;
    p_sTestSpec->vibrator_spec.min_current  = m_VibratorCurrentMin;
    p_sTestSpec->vibrator_spec.max_current  = m_VibratorCurrentMax;
    p_sTestSpec->vibrator_spec.min_diff     = m_VibratorCurrentDiffMin;

    p_sTestSpec->adc_spec.max_vol_diff		= m_adcMaxDiff;
    //p_sCommonCFG->adc_cfg.v1				= m_adcV1;
    //p_sCommonCFG->adc_cfg.v2				= m_adcV2;
    p_sTestSpec->adc_spec.charger_current_max = m_dChargerMax;
    p_sTestSpec->adc_spec.charger_current_min = m_dChargerMin;

    p_sTestSpec->slpmode_spec.max_current	= m_dSlpmodeMaxCurr;
    p_sTestSpec->offmode_spec.d_maxCurrent	= m_dOffModeMaxCurrent;
}

void CPropertyPageVibrator::InitParameters ()
{
    m_bVibratorOnBoard		= p_sCommonCFG->vibrator_cfg.bOnBoard ? true : false;
    m_VibratorCurrentMin    = p_sTestSpec->vibrator_spec.min_current;
    m_VibratorCurrentMax    = p_sTestSpec->vibrator_spec.max_current;
    m_VibratorCurrentDiffMin    = p_sTestSpec->vibrator_spec.min_diff;

    m_adcMaxDiff			= p_sTestSpec->adc_spec.max_vol_diff;
    //m_adcV1					= p_sCommonCFG->adc_cfg.v1;
    //m_adcV2					= p_sCommonCFG->adc_cfg.v2;
    m_dChargerMax			= p_sTestSpec->adc_spec.charger_current_max;
    m_dChargerMin			= p_sTestSpec->adc_spec.charger_current_min;

    m_dSlpmodeMaxCurr		= p_sTestSpec->slpmode_spec.max_current;
    m_dOffModeMaxCurrent	= p_sTestSpec->offmode_spec.d_maxCurrent;

    UpdateData (false);
}

BEGIN_MESSAGE_MAP(CPropertyPageVibrator, CPropertyPage)
    //{{AFX_MSG_MAP(CPropertyPageVibrator)
    // NOTE: the ClassWizard will add message map macros here
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropertyPageVibrator message handlers
