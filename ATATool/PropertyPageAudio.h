#if !defined(AFX_PROPERTYPAGEAUDIO_H__4897532A_B5C0_4AC9_AD4D_0CCF414817E9__INCLUDED_)
#define AFX_PROPERTYPAGEAUDIO_H__4897532A_B5C0_4AC9_AD4D_0CCF414817E9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropertyPageAudio.h : header file
//

#include "PropertyPageSuperClass.h"

/////////////////////////////////////////////////////////////////////////////
// CPropertyPageAudio dialog

class CPropertyPageAudio : public CPropertyPage, public CProperyPageSuperClass
{
    DECLARE_DYNCREATE(CPropertyPageAudio)

// Construction
public:
    CPropertyPageAudio();
    ~CPropertyPageAudio();

    virtual void UpdateParameters ();
    virtual void InitParameters ();

// Dialog Data
    //{{AFX_DATA(CPropertyPageAudio)
    enum { IDD = IDD_PROPPAGE_MEDIUM_AUDIO };
    int		m_iHeadsetMaxAmpL;
    int		m_iHeadsetMaxAmpR;
    int		m_iHeadsetMaxFreqL;
    int		m_iHeadsetMaxFreqR;
    int		m_iHeadsetMinAmpL;
    int		m_iHeadsetMinAmpR;
    int		m_iHeadsetMinFreqL;
    int		m_iHeadsetMinFreqR;
    int		m_iReceiverMaxAmpL;
    int		m_iReceiverMaxAmpR;
    int		m_iReceiverMaxFreqL;
    int		m_iReceiverMaxFreqR;
    int		m_iReceiverMinAmpL;
    int		m_iReceiverMinAmpR;
    int		m_iReceiverMinFreqL;
    int		m_iReceiverMinFreqR;
    int		m_iSpeakerMaxAmpL;
    int		m_iSpeakerMaxAmpR;
    int		m_iSpeakerMaxFreqL;
    int		m_iSpeakerMaxFreqR;
    int		m_iSpeakerMinAmpL;
    int		m_iSpeakerMinAmpR;
    int		m_iSpeakerMinFreqL;
    int		m_iSpeakerMinFreqR;
    //}}AFX_DATA


// Overrides
    // ClassWizard generate virtual function overrides
    //{{AFX_VIRTUAL(CPropertyPageAudio)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:
    // Generated message map functions
    //{{AFX_MSG(CPropertyPageAudio)
    // NOTE: the ClassWizard will add member functions here
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPERTYPAGEAUDIO_H__4897532A_B5C0_4AC9_AD4D_0CCF414817E9__INCLUDED_)
