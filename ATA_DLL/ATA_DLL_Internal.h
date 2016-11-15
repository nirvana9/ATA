// ATA_DLL.h : main header file for the ATA_DLL DLL
//

#if !defined(AFX_ATA_DLL_H__252A9E8A_0992_4622_B5AD_0BB573A5CC47__INCLUDED_)
#define AFX_ATA_DLL_H__252A9E8A_0992_4622_B5AD_0BB573A5CC47__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CATA_DLLApp
// See ATA_DLL.cpp for the implementation of this class
//

class CATA_DLLApp : public CWinApp
{
public:
    CATA_DLLApp();

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CATA_DLLApp)
    //}}AFX_VIRTUAL

    //{{AFX_MSG(CATA_DLLApp)
    // NOTE - the ClassWizard will add and remove member functions here.
    //    DO NOT EDIT what you see in these blocks of generated code !
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ATA_DLL_H__252A9E8A_0992_4622_B5AD_0BB573A5CC47__INCLUDED_)
