// ATATool.h : main header file for the ATATOOL application
//

#if !defined(AFX_ATATOOL_H__1191A8B6_0432_46DC_87BE_C6383FBB8049__INCLUDED_)
#define AFX_ATATOOL_H__1191A8B6_0432_46DC_87BE_C6383FBB8049__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CATAToolApp:
// See ATATool.cpp for the implementation of this class
//

class CATAToolApp : public CWinApp
{
public:
    CATAToolApp();

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CATAToolApp)
public:
    virtual BOOL InitInstance();
    //}}AFX_VIRTUAL

// Implementation

    //{{AFX_MSG(CATAToolApp)
    // NOTE - the ClassWizard will add and remove member functions here.
    //    DO NOT EDIT what you see in these blocks of generated code !
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ATATOOL_H__1191A8B6_0432_46DC_87BE_C6383FBB8049__INCLUDED_)
