// PropertyPagePMCal.cpp : implementation file
//

#include "stdafx.h"
#include "ATATool.h"
#include "PropertyPagePMCal.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropertyPagePMCal property page

IMPLEMENT_DYNCREATE(CPropertyPagePMCal, CPropertyPage)

CPropertyPagePMCal::CPropertyPagePMCal() : CPropertyPage(CPropertyPagePMCal::IDD)
{
    //{{AFX_DATA_INIT(CPropertyPagePMCal)
    m_dADC11 = 0.0;
    m_dADC12 = 0.0;
    m_dADC21 = 0.0;
    m_dADC22 = 0.0;
    m_dADC31 = 0.0;
    m_dADC32 = 0.0;
    m_dADC42 = 0.0;
    m_dADC41 = 0.0;
    m_dOffset1 = 0.0;
    m_dOffset2 = 0.0;
    m_dOffset3 = 0.0;
    m_dOffset4 = 0.0;
    m_dSlope1 = 0.0;
    m_dSlope2 = 0.0;
    m_dSlope3 = 0.0;
    m_dSlope4 = 0.0;
    m_dVol11 = 0.0;
    m_dVol12 = 0.0;
    m_dVol21 = 0.0;
    m_dVol22 = 0.0;
    m_dVol31 = 0.0;
    m_dVol32 = 0.0;
    m_dVol41 = 0.0;
    m_dVol42 = 0.0;
    //}}AFX_DATA_INIT
}

CPropertyPagePMCal::~CPropertyPagePMCal()
{
}

void CPropertyPagePMCal::DoDataExchange(CDataExchange* pDX)
{
    CPropertyPage::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CPropertyPagePMCal)
    DDX_Text(pDX, IDC_EDIT_ADC_11, m_dADC11);
    DDX_Text(pDX, IDC_EDIT_ADC_12, m_dADC12);
    DDX_Text(pDX, IDC_EDIT_ADC_21, m_dADC21);
    DDX_Text(pDX, IDC_EDIT_ADC_22, m_dADC22);
    DDX_Text(pDX, IDC_EDIT_ADC_31, m_dADC31);
    DDX_Text(pDX, IDC_EDIT_ADC_32, m_dADC32);
    DDX_Text(pDX, IDC_EDIT_ADC_42, m_dADC42);
    DDX_Text(pDX, IDC_EDIT_ADC_41, m_dADC41);
    DDX_Text(pDX, IDC_EDIT_OFFSET_1, m_dOffset1);
    DDX_Text(pDX, IDC_EDIT_OFFSET_2, m_dOffset2);
    DDX_Text(pDX, IDC_EDIT_OFFSET_3, m_dOffset3);
    DDX_Text(pDX, IDC_EDIT_OFFSET_4, m_dOffset4);
    DDX_Text(pDX, IDC_EDIT_SLOPE_1, m_dSlope1);
    DDX_Text(pDX, IDC_EDIT_SLOPE_2, m_dSlope2);
    DDX_Text(pDX, IDC_EDIT_SLOPE_3, m_dSlope3);
    DDX_Text(pDX, IDC_EDIT_SLOPE_4, m_dSlope4);
    DDX_Text(pDX, IDC_EDIT_VOL_11, m_dVol11);
    DDX_Text(pDX, IDC_EDIT_VOL_12, m_dVol12);
    DDX_Text(pDX, IDC_EDIT_VOL_21, m_dVol21);
    DDX_Text(pDX, IDC_EDIT_VOL_22, m_dVol22);
    DDX_Text(pDX, IDC_EDIT_VOL_31, m_dVol31);
    DDX_Text(pDX, IDC_EDIT_VOL_32, m_dVol32);
    DDX_Text(pDX, IDC_EDIT_VOL_41, m_dVol41);
    DDX_Text(pDX, IDC_EDIT_VOL_42, m_dVol42);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropertyPagePMCal, CPropertyPage)
    //{{AFX_MSG_MAP(CPropertyPagePMCal)
    ON_BN_CLICKED(IDC_BUTTON_CONVERT_1, OnButtonConvert1)
    ON_BN_CLICKED(IDC_BUTTON_CONVERT_2, OnButtonConvert2)
    ON_BN_CLICKED(IDC_BUTTON_CONVERT_3, OnButtonConvert3)
    ON_BN_CLICKED(IDC_BUTTON_CONVERT_4, OnButtonConvert4)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropertyPagePMCal message handlers

void CPropertyPagePMCal::SetComportSetting (S_COMPORT_ACTIVE_INFO *comport_setting, int length)
{
    this->p_sComportSetting		= comport_setting;
    this->comport_setting_len	= length;
}

void CPropertyPagePMCal::UpdateParameters ()
{
    p_sComportSetting[0].com_port_info.pm_info.vol1	= m_dVol11;
    p_sComportSetting[0].com_port_info.pm_info.vol2	= m_dVol12;
    p_sComportSetting[0].com_port_info.pm_info.adc1	= m_dADC11;
    p_sComportSetting[0].com_port_info.pm_info.adc2	= m_dADC12;
    p_sComportSetting[0].com_port_info.pm_info.slope	= m_dSlope1;
    p_sComportSetting[0].com_port_info.pm_info.offset	= m_dOffset1;

    p_sComportSetting[1].com_port_info.pm_info.vol1	= m_dVol21;
    p_sComportSetting[1].com_port_info.pm_info.vol2	= m_dVol22;
    p_sComportSetting[1].com_port_info.pm_info.adc1	= m_dADC21;
    p_sComportSetting[1].com_port_info.pm_info.adc2	= m_dADC22;
    p_sComportSetting[1].com_port_info.pm_info.slope	= m_dSlope2;
    p_sComportSetting[1].com_port_info.pm_info.offset	= m_dOffset2;

    p_sComportSetting[2].com_port_info.pm_info.vol1	= m_dVol31;
    p_sComportSetting[2].com_port_info.pm_info.vol2	= m_dVol32;
    p_sComportSetting[2].com_port_info.pm_info.adc1	= m_dADC31;
    p_sComportSetting[2].com_port_info.pm_info.adc2	= m_dADC32;
    p_sComportSetting[2].com_port_info.pm_info.slope	= m_dSlope3;
    p_sComportSetting[2].com_port_info.pm_info.offset	= m_dOffset3;

    p_sComportSetting[3].com_port_info.pm_info.vol1	= m_dVol41;
    p_sComportSetting[3].com_port_info.pm_info.vol2	= m_dVol42;
    p_sComportSetting[3].com_port_info.pm_info.adc1	= m_dADC41;
    p_sComportSetting[3].com_port_info.pm_info.adc2	= m_dADC42;
    p_sComportSetting[3].com_port_info.pm_info.slope	= m_dSlope4;
    p_sComportSetting[3].com_port_info.pm_info.offset	= m_dOffset4;
}

void CPropertyPagePMCal::InitParameters ()
{
    m_dVol11	= p_sComportSetting[0].com_port_info.pm_info.vol1;
    m_dVol12	= p_sComportSetting[0].com_port_info.pm_info.vol2;
    m_dADC11	= p_sComportSetting[0].com_port_info.pm_info.adc1;
    m_dADC12	= p_sComportSetting[0].com_port_info.pm_info.adc2;
    m_dSlope1	= p_sComportSetting[0].com_port_info.pm_info.slope;
    m_dOffset1	= p_sComportSetting[0].com_port_info.pm_info.offset;

    m_dVol21	= p_sComportSetting[1].com_port_info.pm_info.vol1;
    m_dVol22	= p_sComportSetting[1].com_port_info.pm_info.vol2;
    m_dADC21	= p_sComportSetting[1].com_port_info.pm_info.adc1;
    m_dADC22	= p_sComportSetting[1].com_port_info.pm_info.adc2;
    m_dSlope2	= p_sComportSetting[1].com_port_info.pm_info.slope;
    m_dOffset2	= p_sComportSetting[1].com_port_info.pm_info.offset;

    m_dVol31	= p_sComportSetting[2].com_port_info.pm_info.vol1;
    m_dVol32	= p_sComportSetting[2].com_port_info.pm_info.vol2;
    m_dADC31	= p_sComportSetting[2].com_port_info.pm_info.adc1;
    m_dADC32	= p_sComportSetting[2].com_port_info.pm_info.adc2;
    m_dSlope3	= p_sComportSetting[2].com_port_info.pm_info.slope;
    m_dOffset3	= p_sComportSetting[2].com_port_info.pm_info.offset;

    m_dVol41	= p_sComportSetting[3].com_port_info.pm_info.vol1;
    m_dVol42	= p_sComportSetting[3].com_port_info.pm_info.vol2;
    m_dADC41	= p_sComportSetting[3].com_port_info.pm_info.adc1;
    m_dADC42	= p_sComportSetting[3].com_port_info.pm_info.adc2;
    m_dSlope4	= p_sComportSetting[3].com_port_info.pm_info.slope;
    m_dOffset4	= p_sComportSetting[3].com_port_info.pm_info.offset;
}
void CPropertyPagePMCal::OnButtonConvert1()
{
    // TODO: Add your control notification handler code here

    UpdateData (true);

    double slope = 0.0;
    if (m_dADC11 - m_dADC12 != 0)
    {
        slope = (m_dVol11 - m_dVol12) / (m_dADC11 - m_dADC12);
        slope = floor(slope * 1000000 + 0.5) /1000000;
    }
    else
    {
        MessageBox ("ADC value invalid!", "Error");
        return;
    }
    double offset = m_dVol11 - slope * m_dADC11;
    offset = floor(offset * 1000000 + 0.5) / 1000000;

    p_sComportSetting[0].com_port_info.pm_info.slope	= slope;
    p_sComportSetting[0].com_port_info.pm_info.offset	= offset;
    p_sComportSetting[0].com_port_info.pm_info.adc1		= m_dADC11;
    p_sComportSetting[0].com_port_info.pm_info.adc2		= m_dADC12;
    p_sComportSetting[0].com_port_info.pm_info.vol1		= m_dVol11;
    p_sComportSetting[0].com_port_info.pm_info.vol2		= m_dVol12;

    m_dSlope1	= slope;
    m_dOffset1	= offset;

    UpdateData (false);
}

void CPropertyPagePMCal::OnButtonConvert2()
{
    // TODO: Add your control notification handler code here
    UpdateData (true);

    double slope = 0.0;
    if (m_dADC21 - m_dADC22 != 0)
    {
        slope = (m_dVol21 - m_dVol22) / (m_dADC21 - m_dADC22);
        slope = floor(slope * 1000000 + 0.5) /1000000;
    }
    else
    {
        MessageBox ("ADC value invalid!", "Error");
        return;
    }
    double offset = m_dVol21 - slope * m_dADC21;
    offset = floor(offset * 1000000 + 0.5) / 1000000;

    p_sComportSetting[1].com_port_info.pm_info.slope	= slope;
    p_sComportSetting[1].com_port_info.pm_info.offset	= offset;
    p_sComportSetting[1].com_port_info.pm_info.adc1		= m_dADC21;
    p_sComportSetting[1].com_port_info.pm_info.adc2		= m_dADC22;
    p_sComportSetting[1].com_port_info.pm_info.vol1		= m_dVol21;
    p_sComportSetting[1].com_port_info.pm_info.vol2		= m_dVol22;

    m_dSlope2	= slope;
    m_dOffset2	= offset;

    UpdateData (false);
}

void CPropertyPagePMCal::OnButtonConvert3()
{
    // TODO: Add your control notification handler code here
    UpdateData (true);

    double slope = 0.0;
    if (m_dADC31 - m_dADC32 != 0)
    {
        slope = (m_dVol31 - m_dVol32) / (m_dADC31 - m_dADC32);
        slope = floor(slope * 1000000 + 0.5) /1000000;
    }
    else
    {
        MessageBox ("ADC value invalid!", "Error");
        return;
    }
    double offset = m_dVol31 - slope * m_dADC31;
    offset = floor(offset * 1000000 + 0.5) / 1000000;

    p_sComportSetting[2].com_port_info.pm_info.slope	= slope;
    p_sComportSetting[2].com_port_info.pm_info.offset	= offset;
    p_sComportSetting[2].com_port_info.pm_info.adc1		= m_dADC31;
    p_sComportSetting[2].com_port_info.pm_info.adc2		= m_dADC32;
    p_sComportSetting[2].com_port_info.pm_info.vol1		= m_dVol31;
    p_sComportSetting[2].com_port_info.pm_info.vol2		= m_dVol32;

    m_dSlope3	= slope;
    m_dOffset3	= offset;

    UpdateData (false);
}

void CPropertyPagePMCal::OnButtonConvert4()
{
    // TODO: Add your control notification handler code here
    UpdateData (true);

    double slope = 0.0;
    if (m_dADC41 - m_dADC42 != 0)
    {
        slope = (m_dVol41 - m_dVol42) / (m_dADC41 - m_dADC42);
        slope = floor(slope * 1000000 + 0.5) /1000000;
    }
    else
    {
        MessageBox ("ADC value invalid!", "Error");
        return;
    }
    double offset = m_dVol41 - slope * m_dADC41;
    offset = floor(offset * 1000000 + 0.5) / 1000000;

    p_sComportSetting[3].com_port_info.pm_info.slope	= slope;
    p_sComportSetting[3].com_port_info.pm_info.offset	= offset;
    p_sComportSetting[3].com_port_info.pm_info.adc1		= m_dADC41;
    p_sComportSetting[3].com_port_info.pm_info.adc2		= m_dADC42;
    p_sComportSetting[3].com_port_info.pm_info.vol1		= m_dVol41;
    p_sComportSetting[3].com_port_info.pm_info.vol2		= m_dVol42;

    m_dSlope4	= slope;
    m_dOffset4	= offset;

    UpdateData (false);
}
