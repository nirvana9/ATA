// PropertyPageAudio.cpp : implementation file
//

#include "stdafx.h"
#include "ATATool.h"
#include "PropertyPageAudio.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropertyPageAudio property page

IMPLEMENT_DYNCREATE(CPropertyPageAudio, CPropertyPage)

CPropertyPageAudio::CPropertyPageAudio() : CPropertyPage(CPropertyPageAudio::IDD)
{
    //{{AFX_DATA_INIT(CPropertyPageAudio)
    m_iHeadsetMaxAmpL = 0;
    m_iHeadsetMaxAmpR = 0;
    m_iHeadsetMaxFreqL = 0;
    m_iHeadsetMaxFreqR = 0;
    m_iHeadsetMinAmpL = 0;
    m_iHeadsetMinAmpR = 0;
    m_iHeadsetMinFreqL = 0;
    m_iHeadsetMinFreqR = 0;
    m_iReceiverMaxAmpL = 0;
    m_iReceiverMaxAmpR = 0;
    m_iReceiverMaxFreqL = 0;
    m_iReceiverMaxFreqR = 0;
    m_iReceiverMinAmpL = 0;
    m_iReceiverMinAmpR = 0;
    m_iReceiverMinFreqL = 0;
    m_iReceiverMinFreqR = 0;
    m_iSpeakerMaxAmpL = 0;
    m_iSpeakerMaxAmpR = 0;
    m_iSpeakerMaxFreqL = 0;
    m_iSpeakerMaxFreqR = 0;
    m_iSpeakerMinAmpL = 0;
    m_iSpeakerMinAmpR = 0;
    m_iSpeakerMinFreqL = 0;
    m_iSpeakerMinFreqR = 0;
    //}}AFX_DATA_INIT
}

CPropertyPageAudio::~CPropertyPageAudio()
{
}

void CPropertyPageAudio::DoDataExchange(CDataExchange* pDX)
{
    CPropertyPage::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CPropertyPageAudio)
    DDX_Text(pDX, IDC_EDIT_HEADSET_MAX_AMP_L, m_iHeadsetMaxAmpL);
    DDX_Text(pDX, IDC_EDIT_HEADSET_MAX_AMP_R, m_iHeadsetMaxAmpR);
    DDX_Text(pDX, IDC_EDIT_HEADSET_MAX_FREQ_L, m_iHeadsetMaxFreqL);
    DDX_Text(pDX, IDC_EDIT_HEADSET_MAX_FREQ_R, m_iHeadsetMaxFreqR);
    DDX_Text(pDX, IDC_EDIT_HEADSET_MIN_AMP_L, m_iHeadsetMinAmpL);
    DDX_Text(pDX, IDC_EDIT_HEADSET_MIN_AMP_R, m_iHeadsetMinAmpR);
    DDX_Text(pDX, IDC_EDIT_HEADSET_MIN_FREQ_L, m_iHeadsetMinFreqL);
    DDX_Text(pDX, IDC_EDIT_HEADSET_MIN_FREQ_R, m_iHeadsetMinFreqR);
    DDX_Text(pDX, IDC_EDIT_RECEIVER_MAX_AMP_L, m_iReceiverMaxAmpL);
    DDX_Text(pDX, IDC_EDIT_RECEIVER_MAX_AMP_R, m_iReceiverMaxAmpR);
    DDX_Text(pDX, IDC_EDIT_RECEIVER_MAX_FREQ_L, m_iReceiverMaxFreqL);
    DDX_Text(pDX, IDC_EDIT_RECEIVER_MAX_FREQ_R, m_iReceiverMaxFreqR);
    DDX_Text(pDX, IDC_EDIT_RECEIVER_MIN_AMP_L, m_iReceiverMinAmpL);
    DDX_Text(pDX, IDC_EDIT_RECEIVER_MIN_AMP_R, m_iReceiverMinAmpR);
    DDX_Text(pDX, IDC_EDIT_RECEIVER_MIN_FREQ_L, m_iReceiverMinFreqL);
    DDX_Text(pDX, IDC_EDIT_RECEIVER_MIN_FREQ_R, m_iReceiverMinFreqR);
    DDX_Text(pDX, IDC_EDIT_SPEAKER_MAX_AMP_L, m_iSpeakerMaxAmpL);
    DDX_Text(pDX, IDC_EDIT_SPEAKER_MAX_AMP_R, m_iSpeakerMaxAmpR);
    DDX_Text(pDX, IDC_EDIT_SPEAKER_MAX_FREQ_L, m_iSpeakerMaxFreqL);
    DDX_Text(pDX, IDC_EDIT_SPEAKER_MAX_FREQ_R, m_iSpeakerMaxFreqR);
    DDX_Text(pDX, IDC_EDIT_SPEAKER_MIN_AMP_L, m_iSpeakerMinAmpL);
    DDX_Text(pDX, IDC_EDIT_SPEAKER_MIN_AMP_R, m_iSpeakerMinAmpR);
    DDX_Text(pDX, IDC_EDIT_SPEAKER_MIN_FREQ_L, m_iSpeakerMinFreqL);
    DDX_Text(pDX, IDC_EDIT_SPEAKER_MIN_FREQ_R, m_iSpeakerMinFreqR);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropertyPageAudio, CPropertyPage)
    //{{AFX_MSG_MAP(CPropertyPageAudio)
    // NOTE: the ClassWizard will add message map macros here
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropertyPageAudio message handlers
void CPropertyPageAudio::UpdateParameters()
{
    UpdateData (true);

    // headset
    p_sTestSpec->audio_spec.max_headset_data.amplL	= m_iHeadsetMaxAmpL;
    p_sTestSpec->audio_spec.max_headset_data.amplR	= m_iHeadsetMaxAmpR;
    p_sTestSpec->audio_spec.max_headset_data.freqL	= m_iHeadsetMaxFreqL;
    p_sTestSpec->audio_spec.max_headset_data.freqR	= m_iHeadsetMaxFreqR;

    p_sTestSpec->audio_spec.min_headset_data.amplL	= m_iHeadsetMinAmpL;
    p_sTestSpec->audio_spec.min_headset_data.amplR	= m_iHeadsetMinAmpR;
    p_sTestSpec->audio_spec.min_headset_data.freqL	= m_iHeadsetMinFreqL;
    p_sTestSpec->audio_spec.min_headset_data.freqR	= m_iHeadsetMinFreqR;

    // speaker
    p_sTestSpec->audio_spec.max_speaker_data.amplL	= m_iSpeakerMaxAmpL;
    p_sTestSpec->audio_spec.max_speaker_data.amplR	= m_iSpeakerMaxAmpR;
    p_sTestSpec->audio_spec.max_speaker_data.freqL	= m_iSpeakerMaxFreqL;
    p_sTestSpec->audio_spec.max_speaker_data.freqR	= m_iSpeakerMaxFreqR;

    p_sTestSpec->audio_spec.min_speaker_data.amplL	= m_iSpeakerMinAmpL;
    p_sTestSpec->audio_spec.min_speaker_data.amplR	= m_iSpeakerMinAmpR;
    p_sTestSpec->audio_spec.min_speaker_data.freqL	= m_iSpeakerMinFreqL;
    p_sTestSpec->audio_spec.min_speaker_data.freqR	= m_iSpeakerMinFreqR;

    // Receiver
    p_sTestSpec->audio_spec.max_receiver_data.amplL	= m_iReceiverMaxAmpL;
    p_sTestSpec->audio_spec.max_receiver_data.amplR	= m_iReceiverMaxAmpR;
    p_sTestSpec->audio_spec.max_receiver_data.freqL	= m_iReceiverMaxFreqL;
    p_sTestSpec->audio_spec.max_receiver_data.freqR	= m_iReceiverMaxFreqR;

    p_sTestSpec->audio_spec.min_receiver_data.amplL	= m_iReceiverMinAmpL;
    p_sTestSpec->audio_spec.min_receiver_data.amplR	= m_iReceiverMinAmpR;
    p_sTestSpec->audio_spec.min_receiver_data.freqL	= m_iReceiverMinFreqL;
    p_sTestSpec->audio_spec.min_receiver_data.freqR	= m_iReceiverMinFreqR;

}

void CPropertyPageAudio::InitParameters ()
{
    // headset
    m_iHeadsetMaxAmpL	= p_sTestSpec->audio_spec.max_headset_data.amplL;
    m_iHeadsetMaxAmpR	= p_sTestSpec->audio_spec.max_headset_data.amplR;
    m_iHeadsetMaxFreqL	= p_sTestSpec->audio_spec.max_headset_data.freqL;
    m_iHeadsetMaxFreqR	= p_sTestSpec->audio_spec.max_headset_data.freqR;

    m_iHeadsetMinAmpL	= p_sTestSpec->audio_spec.min_headset_data.amplL;
    m_iHeadsetMinAmpR	= p_sTestSpec->audio_spec.min_headset_data.amplR;
    m_iHeadsetMinFreqL	= p_sTestSpec->audio_spec.min_headset_data.freqL;
    m_iHeadsetMinFreqR	= p_sTestSpec->audio_spec.min_headset_data.freqR;

    // Speaker
    m_iSpeakerMaxAmpL	= p_sTestSpec->audio_spec.max_speaker_data.amplL;
    m_iSpeakerMaxAmpR	= p_sTestSpec->audio_spec.max_speaker_data.amplR;
    m_iSpeakerMaxFreqL	= p_sTestSpec->audio_spec.max_speaker_data.freqL;
    m_iSpeakerMaxFreqR	= p_sTestSpec->audio_spec.max_speaker_data.freqR;

    m_iSpeakerMinAmpL	= p_sTestSpec->audio_spec.min_speaker_data.amplL;
    m_iSpeakerMinAmpR	= p_sTestSpec->audio_spec.min_speaker_data.amplR;
    m_iSpeakerMinFreqL	= p_sTestSpec->audio_spec.min_speaker_data.freqL;
    m_iSpeakerMinFreqR	= p_sTestSpec->audio_spec.min_speaker_data.freqR;

    // Receiver
    m_iReceiverMaxAmpL	= p_sTestSpec->audio_spec.max_receiver_data.amplL;
    m_iReceiverMaxAmpR	= p_sTestSpec->audio_spec.max_receiver_data.amplR;
    m_iReceiverMaxFreqL	= p_sTestSpec->audio_spec.max_receiver_data.freqL;
    m_iReceiverMaxFreqR	= p_sTestSpec->audio_spec.max_receiver_data.freqR;

    m_iReceiverMinAmpL	= p_sTestSpec->audio_spec.min_receiver_data.amplL;
    m_iReceiverMinAmpR	= p_sTestSpec->audio_spec.min_receiver_data.amplR;
    m_iReceiverMinFreqL	= p_sTestSpec->audio_spec.min_receiver_data.freqL;
    m_iReceiverMinFreqR	= p_sTestSpec->audio_spec.min_receiver_data.freqR;

    UpdateData (false);
}

