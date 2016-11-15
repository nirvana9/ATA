
#ifndef __TESTOBJECT_H__
#define __TESTOBJECT_H__

#include <windows.h>
#include <process.h>

#include "..\ATA_DLL\ATA_DLL.h"
#include "XListCtrl.h"

typedef unsigned (WINAPI *TESTOBJTHREAD_THREADFUNC) (LPVOID LpThreadParameter);
typedef void (__stdcall *CallbackTestFinished)(int buttonID);
extern char version[64];
	
class CTestObject
{
// Construction
public:
    CTestObject(CXListCtrl *xListCtrl = NULL,
                S_ATADLL_TEST_ITEM_T	*test_items	= NULL,
                S_ATADLL_COMMON_CFG_T	*comm_cfg	= NULL);
    virtual ~CTestObject();

    void InitializeListCtrl ();
    void FillListCtrl ();
    void FillListCtrl_StepDebug();
    void UpdateListCtrlWidth ();
    void SetStepDebugRow(int row);
    void SetXListCtrl(CXListCtrl *xListCtrl);
    void SetButtonID (int ID);
    void SetTestItem(S_ATADLL_TEST_ITEM_T *test_items);
    void SetCommCFG(S_ATADLL_COMMON_CFG_T *comm_cfg);
    void SetTestSpec (S_ATADLL_TESTITEM_SPEC *test_spec);
    void SetTestFinishCallBack (CallbackTestFinished cb);

    void StartTestThread ();
    void StartTestThread_StepDebug();
    void StopTestThread();

    bool isRunning;
    bool isStepDebugRunning;

// Implementation
protected:

    void UpdateTestFlag ();
    bool QueryTestItemResult (E_ATDLL_TEST_ITEM_COUNT item);
    void UpdateTestResultToCtrlList_Sub (E_ATDLL_TEST_ITEM_COUNT item);
    void UpdateTestResultToCtrlList ();
    void UpdateTestFailTimesToCtrlList ();
    static DWORD WINAPI HelpThreadFunc(LPVOID param);
    static DWORD WINAPI HelpThreadFuncStop(LPVOID param);
    static DWORD WINAPI HelpThreadFunc_StepDebug(LPVOID param);
    virtual DWORD ThreadFunc();
    virtual DWORD ThreadFunc_StepDebug();
    virtual DWORD ThreadFuncStop();
    static void __stdcall TestPreprocessCallback (int handle_index);
    static void __stdcall TestProgressCallback(int handle_index, E_ATDLL_TEST_ITEM_COUNT test_item,int percent);
    static void __stdcall QueryTestResultCallback (int handle_index, E_ATDLL_TEST_ITEM_COUNT item, bool *result);
    static void __stdcall UpdateTestResultCallback (int handle_index, E_ATDLL_TEST_ITEM_COUNT item);
   


protected:
    S_ATADLL_TEST_ITEM_T	*p_sTestItem;
    S_ATADLL_COMMON_CFG_T	*p_sCommonCFG;
    S_ATADLL_TEST_CNF		testCNF;
    S_ATADLL_TESTITEM_SPEC	*p_sTestSpec;

    S_ATADLL_TESTCASE_T     testcases_bg[MAX_TEST_CASE_NUM];
    S_ATADLL_TESTCASE_T     testcases_fg[MAX_TEST_CASE_NUM];
    int                     testcases_number_bg;
    int                     testcases_number_fg;

    CXListCtrl		*pXListCtrl;
    int             buttonID;

    int             m_iStepDebugRow;

    HANDLE			hTestEvent;
    bool			testFlag[E_TEST_ITEM_COUNT];

    int		ata_handle;
    HANDLE	m_hThread;
    DWORD	m_ThreadId;
	bool finalPassed; 
	int stressfailStop;
	int stress_test;
	int test_times;
	int max_testTimes;

	
	
	//char					m_log_buf[MAX_LOG_BUF_SIZE];

    CallbackTestFinished cbTestFinishFunc;
};

#endif
