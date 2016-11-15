// PropertyPageMATV.cpp : implementation file
//

#include "stdafx.h"
#include "ATATool.h"
#include "PropertyPageMATV.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropertyPageMATV property page

IMPLEMENT_DYNCREATE(CPropertyPageMATV, CPropertyPage)

char MATVCountry[][64] =
{
    "TV_AFGHANISTAN",
    "TV_ARGENTINA",
    "TV_AUSTRALIA",
    "TV_BRAZIL",
    "TV_BURMA",
    "TV_CAMBODIA",
    "TV_CANADA",
    "TV_CHILE",
    "TV_CHINA",
    "TV_CHINA_HONGKONG",
    "TV_CHINA_SHENZHEN",
    "TV_EUROPE_EASTERN",
    "TV_EUROPE_WESTERN",
    "TV_FRANCE",
    "TV_FRENCH_COLONIE",
    "TV_INDIA",
    "TV_INDONESIA",
    "TV_IRAN",
    "TV_ITALY",
    "TV_JAPAN",
    "TV_KOREA",
    "TV_LAOS",
    "TV_MALAYSIA",
    "TV_MEXICO",
    "TV_NEWZEALAND",
    "TV_PAKISTAN",
    "TV_PARAGUAY",
    "TV_PHILIPPINES",
    "TV_PORTUGAL",
    "TV_RUSSIA",
    "TV_SINGAPORE",
    "TV_SOUTHAFRICA",
    "TV_SPAIN",
    "TV_TAIWAN",
    "TV_THAILAND",
    "TV_TURKEY",
    "TV_UNITED_ARAB_EMIRATES",
    "TV_UNITED_KINGDOM",
    "TV_USA",
    "TV_URUGUAY",
    "TV_VENEZUELA",
    "TV_VIETNAM",
    "TV_IRELAND",
    "TV_MOROCCO",
    "TV_COUNTRY_MAX"
};



CPropertyPageMATV::CPropertyPageMATV() : CPropertyPage(CPropertyPageMATV::IDD)
{
    //{{AFX_DATA_INIT(CPropertyPageMATV)
    m_MATVChip = _T("");
    m_MATVFreq = 0;
    //}}AFX_DATA_INIT
}

CPropertyPageMATV::~CPropertyPageMATV()
{
}

void CPropertyPageMATV::DoDataExchange(CDataExchange* pDX)
{
    CPropertyPage::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CPropertyPageMATV)
    DDX_Control(pDX, IDC_COMBO_MATV_COUNTRY, m_comboCountry);
    DDX_Text(pDX, IDC_EDIT_MATV_CHIP, m_MATVChip);
    DDX_Text(pDX, IDC_EDIT_MATV_FREQ, m_MATVFreq);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPropertyPageMATV, CPropertyPage)
    //{{AFX_MSG_MAP(CPropertyPageMATV)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropertyPageMATV message handlers

void CPropertyPageMATV::UpdateParameters ()
{
    UpdateData (true);

    // common cfg
    p_sCommonCFG->matv_freq     = m_MATVFreq;
    p_sCommonCFG->matv_country  = m_comboCountry.GetCurSel ();

    // spec
    memcpy (p_sTestSpec->atv_chip, m_MATVChip.GetBuffer(0), m_MATVChip.GetLength());
    p_sTestSpec->atv_chip[m_MATVChip.GetLength()] = '\0';
}

void CPropertyPageMATV::InitParameters ()
{
    // common cfg
    m_MATVFreq      = p_sCommonCFG->matv_freq;
    m_comboCountry.SetCurSel (p_sCommonCFG->matv_country);

    // spec
    m_MATVChip      = p_sTestSpec->atv_chip;

    UpdateData (false);
}


BOOL CPropertyPageMATV::OnInitDialog()
{
    CPropertyPage::OnInitDialog();

    // TODO: Add extra initialization here
    for (int index = 0; ; index ++)
    {
        if (strcmp (MATVCountry[index], "TV_COUNTRY_MAX") == 0)
        {
            break;
        }
        CString str;
        str.Format ("%s", MATVCountry[index]);
        m_comboCountry.AddString (str);
    }
    m_comboCountry.SetCurSel (10);

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}
