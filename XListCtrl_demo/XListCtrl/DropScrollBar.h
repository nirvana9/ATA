/////////////////////////////////////////////////////////////////////////////
// DropScrollBar.h : header file
//
// CAdvComboBox Control
// Version: 2.1
// Date: September 2002
// Author: Mathias Tunared
// Email: Mathias@inorbit.com
// Copyright (c) 2002. All Rights Reserved.
//
// This code, in compiled form or as source code, may be redistributed
// unmodified PROVIDING it is not sold for profit without the authors
// written consent, and providing that this notice and the authors name
// and all copyright notices remains intact.
//
// This file is provided "as is" with no expressed or implied warranty.
// The author accepts no liability for any damage/loss of business that
// this product may cause.
//
/////////////////////////////////////////////////////////////////////////////

#ifndef DROPSCROLLBAR_H
#define DROPSCROLLBAR_H

#include "DropWnd.h"

class CDropListBox;

/////////////////////////////////////////////////////////////////////////////
// CDropScrollBar window

class CDropScrollBar : public CScrollBar
{
// Construction
public:
    CDropScrollBar();

// Attributes
public:

// Operations
public:
    void SetListBox( CDropListBox* pListBox );

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDropScrollBar)
    //}}AFX_VIRTUAL

// Implementation
public:
    virtual ~CDropScrollBar();

    // Generated message map functions
protected:
    //{{AFX_MSG(CDropScrollBar)
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void VScroll(UINT nSBCode, UINT nPos);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    //}}AFX_MSG
    afx_msg LONG OnSetCapture( WPARAM wParam, LPARAM lParam );
    afx_msg LONG OnReleaseCapture( WPARAM wParam, LPARAM lParam );

    DECLARE_MESSAGE_MAP()

private:
    CDropListBox* m_pListBox;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif //DROPSCROLLBAR_H
