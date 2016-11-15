// ATAToolDlg.h : header file
//

#if !defined(AFX_ATATOOLDLG_H__B05C34C3_CF95_4B97_9A04_4B9AB7792DC2__INCLUDED_)
#define AFX_ATATOOLDLG_H__B05C34C3_CF95_4B97_9A04_4B9AB7792DC2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ResizableDialog.h"
#include "..\ATA_DLL\ATA_DLL.h"
#include "XListCtrl\XListCtrl.h"
#include "TestObject.h"
#include "TestItemConfig.h"

/////////////////////////////////////////////////////////////////////////////
// CATAToolDlg dialog

class CATAToolDlg : public CResizableDialog
{
// Construction
public:
    CATAToolDlg(CWnd* pParent = NULL);	// standard constructor
    ~CATAToolDlg();

// Dialog Data
    //{{AFX_DATA(CATAToolDlg)
    enum { IDD = IDD_ATATOOL_DIALOG };
    CXListCtrl	m_ListCtrlDUT4;
    CXListCtrl	m_ListCtrlDUT3;
    CXListCtrl	m_ListCtrlDUT2;
    CXListCtrl	m_ListCtrlDUT1;
    //}}AFX_DATA

    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CATAToolDlg)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:
    HICON m_hIcon;

    // Generated message map functions
    //{{AFX_MSG(CATAToolDlg)
    virtual BOOL OnInitDialog();
    virtual void SaveConfigToSetupFile();
    afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    afx_msg void OnButtonConfig();
    afx_msg void OnButtonStartAll();
    afx_msg void OnButtonStopAll();
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnButton1();
    afx_msg void OnButton2();
    afx_msg void OnButton3();
    afx_msg void OnButton4();
    afx_msg void OnDblclkListDut1(NMHDR* pNMHDR, LRESULT* pResult);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()

    virtual void UpdateTestObj ();
    static void __stdcall UpdateButtonStatus (int buttonID);
    void UpdateButtonStatus_Internal(int buttonID);

protected:
    int		stopFlag;

    S_ATADLL_TEST_ITEM_T	ata_test_items;
    S_ATADLL_COMMON_CFG_T	ata_common_cfg;
    S_ATADLL_COMMON_CFG_T	ata_common_cfg1;
    S_ATADLL_COMMON_CFG_T	ata_common_cfg2;
    S_ATADLL_COMMON_CFG_T	ata_common_cfg3;
    S_ATADLL_COMMON_CFG_T	ata_common_cfg4;
    S_ATADLL_TESTITEM_SPEC	ata_test_spec;
    S_COMPORT_ACTIVE_INFO	comport_setting[MAX_DUT_NUM];

    CTestObject		testObj1;
    CTestObject		testObj2;
    CTestObject		testObj3;
    CTestObject		testObj4;

    int		stopFlag1;
    int		stopFlag2;
    int		stopFlag3;
    int		stopFlag4;

    HANDLE	m_hButtonEvent;
    int		m_hTestDUTNum;
    int		m_hFinishedDUTNum;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ATATOOLDLG_H__B05C34C3_CF95_4B97_9A04_4B9AB7792DC2__INCLUDED_)
