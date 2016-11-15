
//#include <shellapi.h> //ShellExecute
#include "stdafx.h"
#include "Resource.h"
#include "TestObject.h"
#include "TestItemConfig.h"
#include "..\ATA_DLL\ATA_DLL.h"
#include <math.h>

TCHAR *	CtrlListHeaders[] = { _T("TestItem"),
_T("Value"),
_T("R"),
NULL
};
int	CtrlListColWidth[] = { 18, 39, 7 };
TCHAR * CtrlListTestITem[] =
{
    _T("LCM"),
		
		_T("Receiver"),
		_T("Speaker"),
		_T("HeadSet"),
		_T("Micbias"),
		_T("RF Signal"),
		
		_T("MainCamera"),
		_T("SubCamera"),
		_T("TouchPanel"),
		_T("Vibrator"),
		_T("Keypad"),
		_T("CMMB"),
		_T("RTC"),
		_T("ADC"),
		_T("LED"),
		
		_T("Ring"),
		_T("Earphone state"),
		
		_T("WIFI"),
		_T("GPS"),
		_T("BT"),
		_T("mATV"),
		
		_T("TCard"),
		_T("EMMC"),
		_T("Flash"),
		_T("SIM Card"),
		_T("Barcode"),
		_T("SW Version"),
		_T("HW Version"),
		
		_T("FM"),
		_T("Charger"),
		
		_T("GSENSOR"),
		_T("MSENSOR"),
		_T("ALSPS"),
		_T("GYROSCOPE"),
		
		_T("OTG"),
		_T("HDMI"),
		_T("MHL"),
		
		_T("Idle Current"),
		_T("Off Current"),
		NULL
};

static int glbTestObjIndex = 0;
static CTestObject *glbTestObj[4];
static int m_iUpdateBarcodeUi = 1;

CTestObject::CTestObject(CXListCtrl *xListCtrl,
                         S_ATADLL_TEST_ITEM_T	*test_items,
                         S_ATADLL_COMMON_CFG_T	*comm_cfg)
{
    this->pXListCtrl	=	xListCtrl;
    this->p_sTestItem   =   test_items;
    this->p_sCommonCFG  =   comm_cfg;
    this->buttonID		=	0;
    this->isRunning		=	false;
    isStepDebugRunning	=	false;
    m_iStepDebugRow		=	0;
	finalPassed         = true; 
	stress_test=0;
	test_times=0;
	max_testTimes=0;
	m_iUpdateBarcodeUi =	1;
    glbTestObj[glbTestObjIndex++]	=	this;
	
    hTestEvent          =   CreateEvent(NULL, FALSE, FALSE, NULL);
    m_hThread           = NULL;
    cbTestFinishFunc    = NULL;
    memset (&testCNF, 0x0, sizeof(S_ATADLL_TEST_CNF));
    memset (testcases_bg, 0x0, sizeof(S_ATADLL_TESTCASE_T)*MAX_TEST_CASE_NUM);
    memset (testcases_fg, 0x0, sizeof(S_ATADLL_TESTCASE_T)*MAX_TEST_CASE_NUM);
	
    ATADLL_GetAvailbleHandle (&ata_handle);
    UpdateTestFlag();
}

CTestObject::~CTestObject()
{
    if (hTestEvent != NULL)
    {
        CloseHandle (hTestEvent);
        hTestEvent = NULL;
		
        ATADLL_ReleaseHandle (ata_handle);
    }
}

void CTestObject::InitializeListCtrl ()
{
    if (pXListCtrl == NULL)
        return;
	
    pXListCtrl->SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
    pXListCtrl->EnableToolTips(TRUE);
	
    // set column width according to window rect
    CRect rect;
    pXListCtrl->GetWindowRect(&rect);
    int w = rect.Width() - 2;
	
    int i;
    int total_cx = 0;
    LV_COLUMN lvcolumn;
    memset(&lvcolumn, 0, sizeof(lvcolumn));
	
    //w = w - ::GetSystemMetrics(SM_CXVSCROLL);	// width of vertical scroll bar
	
    // add columns
    for (i = 0; ; i++)
    {
        if (CtrlListHeaders[i] == NULL)
            break;
		
        lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
        lvcolumn.fmt = LVCFMT_CENTER;
        lvcolumn.pszText = CtrlListHeaders[i];
        lvcolumn.iSubItem = i;
        lvcolumn.cx = (CtrlListHeaders[i+1] == NULL) ? w - total_cx - 2 : (w * CtrlListColWidth[i]) / 64;
        total_cx += lvcolumn.cx;
        pXListCtrl->InsertColumn(i, &lvcolumn);
    }
	
    // Grid Line Display
    DWORD dwExStyle = pXListCtrl->GetExtendedStyle();
    bool m_bGridlines = true;
    if (m_bGridlines)
        dwExStyle |= LVS_EX_GRIDLINES;
    else
        dwExStyle &= ~LVS_EX_GRIDLINES;
    pXListCtrl->SetExtendedStyle(dwExStyle);
	
	
    CImageList imgList;
    imgList.Create(1, 19, TRUE|ILC_COLOR32, 1,0);
    pXListCtrl->SetImageList(&imgList, LVSIL_SMALL);
}

void CTestObject::FillListCtrl ()
{
	MTRACE(g_hEBOOT_DEBUG,"[Thread%d] Entry FillListCtrl", ata_handle+1);
    if (pXListCtrl == NULL)
        return;
	
    pXListCtrl->LockWindowUpdate();	// ***** lock window updates while filling list *****
    pXListCtrl->DeleteAllItems();
	
    CString str = _T("");
    int row_index = 0;
	
#ifdef __ATA_CONCURRENT_FLOW__
	
    ATADLL_GetTestCase_BG (ata_handle, p_sTestItem, testcases_bg, &testcases_number_bg);
    ATADLL_GetTestCase_FG (ata_handle, p_sTestItem, testcases_fg, &testcases_number_fg);
	
    for (row_index = 0; row_index < testcases_number_bg + testcases_number_fg; row_index ++)
    {
        if (row_index < testcases_number_bg)
        {
            str = testcases_bg[row_index].item_name;
        }
        else
        {
            str = testcases_fg[row_index-testcases_number_bg].item_name;
        }
		
        pXListCtrl->InsertItem (row_index, str);
        pXListCtrl->SetItemText(row_index, 0, str, RGB(0,0,0), RGB(194,209,166));
    }
#else
    for (E_ATDLL_TEST_ITEM_COUNT row = E_LCD; row < E_TEST_ITEM_COUNT; row = E_ATDLL_TEST_ITEM_COUNT((int)row + 1))
    {
        if (testFlag[row])
        {
            str = CtrlListTestITem[row];
            pXListCtrl->InsertItem (row_index, str);
            pXListCtrl->SetItemText(row_index, 0, str, RGB(0,0,0), RGB(194,209,166));
            row_index ++;
        }
    }
#endif
	
    pXListCtrl->UnlockWindowUpdate();
	MTRACE(g_hEBOOT_DEBUG,"[Thread%d] Exit FillListCtrl()", ata_handle+1);
	strcpy(p_sCommonCFG->tool_version,version);//add for display tool version in ata logs
	
}


void CTestObject::FillListCtrl_StepDebug()
{
    if (pXListCtrl == NULL)
        return;
	
    pXListCtrl->LockWindowUpdate();	// ***** lock window updates while filling list *****
    pXListCtrl->DeleteAllItems();
	
    CString str = _T("");
    int row_index = 0;
	
#ifdef __ATA_CONCURRENT_FLOW__
	
    ATADLL_GetTestCase_BG (ata_handle, p_sTestItem, testcases_bg, &testcases_number_bg);
    ATADLL_GetTestCase_FG (ata_handle, p_sTestItem, testcases_fg, &testcases_number_fg);
	
    for (row_index = 0; row_index < testcases_number_bg + testcases_number_fg; row_index ++)
    {
        if (row_index < testcases_number_bg)
        {
            str = testcases_bg[row_index].item_name;
        }
        else
        {
            str = testcases_fg[row_index-testcases_number_bg].item_name;
        }
		
        pXListCtrl->InsertItem (row_index, str);
        pXListCtrl->SetItemText(row_index, 0, str, RGB(0,0,0), RGB(194,209,166));
        pXListCtrl->SetItemText(row_index, 2, "Ë«»÷²âÊÔ", RGB(0,0,0), RGB(194,209,166));
    }
#else
    for (E_ATDLL_TEST_ITEM_COUNT row = E_LCD; row < E_TEST_ITEM_COUNT; row = E_ATDLL_TEST_ITEM_COUNT((int)row + 1))
    {
        if (testFlag[row])
        {
            str = CtrlListTestITem[row];
            pXListCtrl->InsertItem (row_index, str);
            pXListCtrl->SetItemText(row_index, 0, str, RGB(0,0,0), RGB(194,209,166));
            row_index ++;
        }
    }
#endif
	
    pXListCtrl->UnlockWindowUpdate();
	
}


void CTestObject::UpdateListCtrlWidth ()
{
    if (pXListCtrl == NULL)
        return;
	
    pXListCtrl->LockWindowUpdate();	// ***** lock window updates while filling list *****
	
    // set column width according to window rect
    CRect rect;
    pXListCtrl->GetWindowRect(&rect);
    int w = rect.Width() - 2 - 36;
	
    //w = w - ::GetSystemMetrics(SM_CXVSCROLL);	// width of vertical scroll bar
	
    int total_cx = 0;
    for (int col = 0; ; col ++)
    {
        if (CtrlListHeaders[col] == NULL)
            break;
		
        int cx = (CtrlListHeaders[col+1] == NULL) ? w - total_cx - 2 : (w * CtrlListColWidth[col]) / 64;
        total_cx += cx;
        pXListCtrl->SetColumnWidth (col, cx);
    }
	
    pXListCtrl->UnlockWindowUpdate();
}

void CTestObject::SetStepDebugRow(int row)
{
    this->m_iStepDebugRow	= row;
}

void CTestObject::SetXListCtrl(CXListCtrl *xListCtrl)
{
    this->pXListCtrl    =   xListCtrl;
}

void CTestObject::SetButtonID (int ID)
{
    this->buttonID		=	ID;
}

void CTestObject::SetTestItem(S_ATADLL_TEST_ITEM_T *test_items)
{
    this->p_sTestItem   =   test_items;
    UpdateTestFlag ();
}

void CTestObject::SetCommCFG(S_ATADLL_COMMON_CFG_T *comm_cfg)
{
    this->p_sCommonCFG  =   comm_cfg;
}

void CTestObject::SetTestSpec (S_ATADLL_TESTITEM_SPEC *test_spec)
{
    this->p_sTestSpec   =   test_spec;
}

void CTestObject::SetTestFinishCallBack (CallbackTestFinished cb)
{
    this->cbTestFinishFunc  = cb;
}

void CTestObject::UpdateTestFlag ()
{
	MTRACE(g_hEBOOT_DEBUG, "[Thread%d] Entry UpdateTestFlag()",ata_handle+1);
    if (p_sTestItem != NULL)
    {
        testFlag[E_LCD]         =   p_sTestItem->b_LCD;
        testFlag[E_CAMERA]      =   p_sTestItem->b_Camera;
        testFlag[E_CAMERA_SUB]	=	p_sTestItem->b_Camera_Sub;
        testFlag[E_VIBRATOR]    =   p_sTestItem->b_Vibrator;
        testFlag[E_CHARGING]    =   p_sTestItem->b_Charging;
        testFlag[E_KEYPAD]      =   p_sTestItem->b_Keypad;
        testFlag[E_TOUCHSCR]    =   p_sTestItem->b_TouchScr;
        testFlag[E_CMMB]    	=   p_sTestItem->b_CMMB;
        testFlag[E_RTC]         =   p_sTestItem->b_RTC;
        testFlag[E_ADC]			=	p_sTestItem->b_ADC;
        testFlag[E_RSSI]		=	p_sTestItem->b_RSSI;
        testFlag[E_LED]			=	p_sTestItem->b_LED;
		
        testFlag[E_SPEAKER]     =   p_sTestItem->b_Speaker;
        testFlag[E_RECEIVER]    =   p_sTestItem->b_Receiver;
        testFlag[E_EARTHPHONE]  =   p_sTestItem->b_EarchPhone;
        testFlag[E_RING]        =   p_sTestItem->b_Ring;
		
        testFlag[E_EARTHPHONE_STATE] = p_sTestItem->b_er_state;
		
        testFlag[E_WIFI]        =   p_sTestItem->b_WIFI;
        testFlag[E_GPS]         =   p_sTestItem->b_GPS;
        testFlag[E_BT]          =   p_sTestItem->b_BT;
        testFlag[E_TV]          =   p_sTestItem->b_TV;
        testFlag[E_FM]          =   p_sTestItem->b_FM;
		
        testFlag[E_TCARD]       =   p_sTestItem->b_TCard;
        testFlag[E_EMMC]        =   p_sTestItem->b_EMMC;
        testFlag[E_FLASH]       =   p_sTestItem->b_Flash;
        testFlag[E_SIM]         =   p_sTestItem->b_SIM;
        testFlag[E_BARCODE]     =   p_sTestItem->b_Barcode;
        testFlag[E_SWVERSION]   =   p_sTestItem->b_SWVersion;
        testFlag[E_HWVERSIOM]   =   p_sTestItem->b_HWVersion;
		
        testFlag[E_GSENSOR]		=	p_sTestItem->b_gsensor;
        testFlag[E_MSENSOR]		=	p_sTestItem->b_msensor;
        testFlag[E_ALSPS]		=	p_sTestItem->b_alsps;
        testFlag[E_GYROSCOPE]	=	p_sTestItem->b_gyroscope;
		
        testFlag[E_MICBIAS]		=	p_sTestItem->b_micbias;
		testFlag[E_OTG]			=	p_sTestItem->b_OTG;
		testFlag[E_HDMI]		=	p_sTestItem->b_HDMI;
		testFlag[E_MHL]			=	p_sTestItem->b_MHL;
		testFlag[E_SLPMODE]		=	p_sTestItem->b_slpmode;
        testFlag[E_OFFMODE]		=	p_sTestItem->b_offMode;
		testFlag[E_GPS_CALI]    =   p_sTestItem->b_GPSCali;  //add by songjian 2016-3-1
		
    }
    else
    {
        for (E_ATDLL_TEST_ITEM_COUNT item = E_LCD; item < E_TEST_ITEM_COUNT; item = E_ATDLL_TEST_ITEM_COUNT((int)item + 1))
        {
            testFlag[item] = false;
        }
    }
	MTRACE(g_hEBOOT_DEBUG, "[Thread%d] Exit UpdateTestFlag()",ata_handle+1);
}

void CTestObject::StartTestThread()
{
    TRACE ("%d: StartTestThread\r\n", ata_handle);
	MTRACE(g_hEBOOT_DEBUG,"[Thread%d] StartTestThread", ata_handle+1);
	//finalPassed         = true;//init finalpassed flag
    m_hThread = (HANDLE)_beginthreadex(
		NULL,
		0,
		(TESTOBJTHREAD_THREADFUNC)CTestObject::HelpThreadFunc,
		(LPVOID)this,
		0,
		(unsigned *) &m_ThreadId
		);
	
    if(m_hThread)
        CloseHandle (m_hThread);
}

void CTestObject::StopTestThread()
{
	TRACE ("%d: StopTestThread\r\n", ata_handle);
    m_hThread = (HANDLE)_beginthreadex(
		NULL,
		0,
		(TESTOBJTHREAD_THREADFUNC)CTestObject::HelpThreadFuncStop,
		(LPVOID)this,
		0,
		(unsigned *) &m_ThreadId
		);
	
    if(m_hThread)
        CloseHandle (m_hThread);
}

void CTestObject::StartTestThread_StepDebug()
{
    m_hThread = (HANDLE)_beginthreadex(
		NULL,
		0,
		(TESTOBJTHREAD_THREADFUNC)CTestObject::HelpThreadFunc_StepDebug,
		(LPVOID)this,
		0,
		(unsigned *) &m_ThreadId
		);
	
    if(m_hThread)
        CloseHandle (m_hThread);
}


DWORD CTestObject::HelpThreadFunc(LPVOID param)
{
    CTestObject *tic = (CTestObject *)param;
    return tic->ThreadFunc();
}

DWORD CTestObject::HelpThreadFuncStop(LPVOID param)
{
    CTestObject *tic = (CTestObject *)param;
    return tic->ThreadFuncStop();
}

DWORD CTestObject::HelpThreadFunc_StepDebug(LPVOID param)
{
    CTestObject *tic = (CTestObject *)param;
    return tic->ThreadFunc_StepDebug();
}

DWORD CTestObject::ThreadFunc()
{
    isRunning	= true;
	/*int stress_test=0;
	int test_times=0,max_testTimes=0;*/
	
    p_sCommonCFG->cbTestProgess = TestProgressCallback;
    p_sCommonCFG->cbTestPreProcess = TestPreprocessCallback;
    p_sCommonCFG->cbQueryTestItemResult	= QueryTestResultCallback;
	p_sCommonCFG->cbUpdateResult		= UpdateTestResultCallback;
    p_sCommonCFG->com_port_info.usb_enable = true;
	p_sCommonCFG->isStartSend=false;
	
	char StressFilePath[512];
    GetCurrentDirectory (512, StressFilePath);
    sprintf (StressFilePath, "%s\\StressTest.ini", StressFilePath);
	FILE *fp=NULL;
	fp=fopen(StressFilePath,"r");
	if(fp!=NULL)
	{
		if (strlen(StressFilePath) == 0)
		{
			stress_test=0;
		}
		else
		{
			stress_test = GetPrivateProfileInt("StressSetting","StressTest",0,StressFilePath);
			max_testTimes = GetPrivateProfileInt("StressSetting","TestTimes",0,StressFilePath);
			stressfailStop =  GetPrivateProfileInt("StressSetting","FailStop",1,StressFilePath);
		}
		fclose(fp);
		MTRACE(g_hEBOOT_DEBUG,"[Thread%d]StressTest=%d,MaxTestTimes=%d,FailStop=%d",ata_handle+1,stress_test,max_testTimes,stressfailStop);
	}
	else
	{
		stress_test=0;
	}
    do
    {
    	finalPassed         = true;//init finalpassed flag
        // Reset status
        memset (&testCNF, 0x0, sizeof(S_ATADLL_TEST_CNF));
        for (E_ATDLL_TEST_ITEM_COUNT index = E_LCD; index < E_TEST_ITEM_COUNT; index = (E_ATDLL_TEST_ITEM_COUNT)((int)index + 1))
        {
            testCNF.test_result[index] = E_ATADLL_RESULT_NULL;
        }
		
        // If stopped by user
        if (*(p_sCommonCFG->stop_flag) == ATA_DLL_STOP_FLAG)
        {
            break;
        }
		//mingxue20150721
		if(p_sCommonCFG->barcodeFlag_cfg.bSetFlag)
		{
			m_iUpdateBarcodeUi=0;
		}
		else
		{
			m_iUpdateBarcodeUi=1;
		}
		
        // Start Test
        int ret = ATADLL_StartTest (ata_handle, p_sTestItem, p_sCommonCFG, &testCNF);
		if(ret==E_ATADLL_CONNECT_PM_FAIL)//if connect to testbox fail
			{
			    *(p_sCommonCFG->stop_flag) = ATA_DLL_STOP_FLAG;  //set stop flag to update ui
			}
		
        // If step debugging, not stop test.
        if (p_sCommonCFG->b_step_debug)
        {
            FillListCtrl_StepDebug ();
            return 0;
        }
		
        // Upate test result onto UI
        UpdateTestResultToCtrlList ();
		
        // Stop Test
        if (!p_sTestItem->b_offMode &&!stress_test)
        {
            ATADLL_StopTest (ata_handle);
        }
		if(stress_test)
		{
			p_sCommonCFG->isStartSend=true;
			if(stressfailStop==1)
			{
				p_sCommonCFG->stop_if_failed=true;
			}
			if(test_times<max_testTimes-1)
			{
				test_times++;
				if((stressfailStop==1)&& (!finalPassed))
				{
					break;  
				}
				Sleep(1000);
				FillListCtrl();
				continue;
			}
			else
				break;
		}
		
        // If no need auto polling, break
        if (!p_sCommonCFG->auto_polling)
        {
            break;
        }
    }
    while (1);
	if(stress_test && (test_times ==max_testTimes-1 || !finalPassed))
	{
		
		ATADLL_StopTest (ata_handle);
		//power off target to save logs
		//HINSTANCE adbresult = ShellExecute(0,"open","adb.exe","adb shell reboot -p","",SW_RESTORE);//fail
		//system("adb shell < tmp.bat");//OK
		//system("adb shell reboot -p");//OK
		system("adb\\adb.exe shell reboot -p");
	}
	
	// Update running status
    isRunning	= false;
	
    // Notify main UI
    if (cbTestFinishFunc != NULL)
    {
        cbTestFinishFunc(buttonID);
    }
	
    return 0;
}

DWORD CTestObject::ThreadFunc_StepDebug()
{
    isStepDebugRunning = true;
	
    p_sCommonCFG->cbTestProgess = TestProgressCallback;
    p_sCommonCFG->cbTestPreProcess = NULL;;
    p_sCommonCFG->cbQueryTestItemResult	= QueryTestResultCallback;
    p_sCommonCFG->com_port_info.usb_enable = true;
	
    E_ATDLL_TEST_ITEM_COUNT item = E_LCD;
    if (m_iStepDebugRow < testcases_number_bg)
    {
        item = testcases_bg[m_iStepDebugRow].item;
    }
    else if (m_iStepDebugRow < testcases_number_bg + testcases_number_fg)
    {
        item = testcases_fg[m_iStepDebugRow-testcases_number_bg].item;
    }
    else
    {
        isStepDebugRunning = false;
        return 0;
    }
	
    //Reset status
    testCNF.test_result[item] = E_ATADLL_RESULT_NULL;
    pXListCtrl->SetItemText(m_iStepDebugRow, 0, NULL, RGB(0,0,0), RGB(194,209,166));
    pXListCtrl->SetItemText(m_iStepDebugRow, 2, "", RGB(0,0,0), RGB(194,209,166));
	
    // Start Test
    ATADLL_StartTestStep (ata_handle, item, p_sTestItem, p_sCommonCFG, &testCNF);
    UpdateTestResultToCtrlList_Sub(item);
	
    isStepDebugRunning = false;
    return 0;
}

DWORD CTestObject::ThreadFuncStop()
{
	ATADLL_UpdateStopFlag (ata_handle);
	TRACE ("111\r\n");
	
    if (p_sCommonCFG->b_step_debug)
    {
		
        // Stop test
        ATADLL_StopTest (ata_handle);
		
		// Update running status
        isRunning	= false;
		
        // Notify main UI
        if (cbTestFinishFunc != NULL)
        {
            cbTestFinishFunc(buttonID);
        }
    }
	return 0;
}


#define ITEM_COLUMN_INDEX       0
#define DATA_COLUMN_INDEX       1
#define RESULT_COLUMN_INDEX     2

void __stdcall CTestObject::TestProgressCallback(int handle_index, E_ATDLL_TEST_ITEM_COUNT test_item,int percent)
{

    glbTestObj[handle_index]->pXListCtrl->LockWindowUpdate();

    int row_index = 0;
	
#ifdef __ATA_CONCURRENT_FLOW__
    int testcase_index = 0;
    for (testcase_index = 0; testcase_index < glbTestObj[handle_index]->testcases_number_bg; testcase_index ++)
    {
        if (glbTestObj[handle_index]->testcases_bg[testcase_index].item == test_item)
        {
            glbTestObj[handle_index]->pXListCtrl->SetProgress (row_index, DATA_COLUMN_INDEX, TRUE);
            glbTestObj[handle_index]->pXListCtrl->UpdateProgress (row_index, DATA_COLUMN_INDEX, percent);
			
            if (0 == percent)
            {
                glbTestObj[handle_index]->pXListCtrl->SetItemText (row_index, RESULT_COLUMN_INDEX, "Retry");
                glbTestObj[handle_index]->pXListCtrl->SetBold (row_index, RESULT_COLUMN_INDEX, TRUE);
            }
            break;
        }
        row_index ++;
    }
    for (testcase_index = 0; testcase_index < glbTestObj[handle_index]->testcases_number_fg; testcase_index ++)
    {
        if (glbTestObj[handle_index]->testcases_fg[testcase_index].item == test_item)
        {
            glbTestObj[handle_index]->pXListCtrl->SetProgress (row_index, DATA_COLUMN_INDEX, TRUE);
            glbTestObj[handle_index]->pXListCtrl->UpdateProgress (row_index, DATA_COLUMN_INDEX, percent);
			
            if (0 == percent)
            {
                glbTestObj[handle_index]->pXListCtrl->SetItemText (row_index, RESULT_COLUMN_INDEX, "Retry");
                glbTestObj[handle_index]->pXListCtrl->SetBold (row_index, RESULT_COLUMN_INDEX, TRUE);
            }
            break;
        }
        row_index ++;
    }
#else
    for (E_ATDLL_TEST_ITEM_COUNT row = E_LCD; row < E_TEST_ITEM_COUNT; row = E_ATDLL_TEST_ITEM_COUNT((int)row + 1))
    {
        if (test_item == row)
        {
            glbTestObj[handle_index]->pXListCtrl->SetProgress (row_index, DATA_COLUMN_INDEX, TRUE);
            glbTestObj[handle_index]->pXListCtrl->UpdateProgress (row_index, DATA_COLUMN_INDEX, percent);
			
            if (0 == percent)
            {
                glbTestObj[handle_index]->pXListCtrl->SetItemText (row_index, RESULT_COLUMN_INDEX, "Retry");
                glbTestObj[handle_index]->pXListCtrl->SetBold (row_index, RESULT_COLUMN_INDEX, TRUE);
            }
            break;
        }
		
        if (glbTestObj[handle_index]->testFlag[row])
        {
            row_index ++;
        }
    }
	
#endif

    glbTestObj[handle_index]->pXListCtrl->UnlockWindowUpdate();

}

void __stdcall CTestObject::TestPreprocessCallback (int handle_index)
{
    glbTestObj[handle_index]->FillListCtrl();
}

void __stdcall CTestObject::QueryTestResultCallback (int handle_index, E_ATDLL_TEST_ITEM_COUNT item, bool *result)
{
    *result = glbTestObj[handle_index]->QueryTestItemResult(item);
}

bool CTestObject::QueryTestItemResult (E_ATDLL_TEST_ITEM_COUNT item)
{
    int row_index = 0;
    bool specPassed	= true;
    E_ATADLL_RESULT	ata_result = E_ATADLL_RESULT_SUCCESS;
    E_ATDLL_TEST_ITEM_COUNT test_item = item;
	
	char logStr[1024];
	
#if 0
    for (row_index = 0; row_index < testcases_number_bg + testcases_number_fg; row_index ++)
    {
        if (row_index < testcases_number_bg)
        {
            test_item = testcases_bg[row_index].item;
        }
        else
        {
            test_item = testcases_fg[row_index - testcases_number_bg].item;
        }
#endif
		
        ata_result = testCNF.test_result[test_item];
        if (ata_result != E_ATADLL_RESULT_SUCCESS)
        {
            return false;
        }
		
        specPassed  = false;
        unsigned int index  = 0;
        double curr_before  = 0.0;
        double curr_after   = 0.0;
        double diff         = 0.0;
        switch (test_item)
        {
        case E_LCD:
            if (testCNF.bLCMDetected)
            {
                specPassed = true;
            }
			else
			{
				specPassed = false;
			}
            break;
		case E_USBTYPEC:
            if (testCNF.bUSBTypeCOK)
            {
                specPassed = true;
            }
			else
			{
				specPassed = false;
			}
            break;
		case E_HUMIDITY:
            if (testCNF.bHumidityOK)
            {
                specPassed = true;
            }
			else
			{
				specPassed = false;
			}
            break;
		case E_FINGERPRINT:
            if (testCNF.bFingerPrintOK)
            {
                specPassed = true;
            }
			else
			{
				specPassed = false;
			}
            break;
		
		case E_EXT_BUCK:
            if (testCNF.bExtBuckOK)
            {
                specPassed = true;
            }
			else
			{
				specPassed = false;
			}
            break;
		case E_BAROMETER:
            if (testCNF.bBarometerOK)
            {
                specPassed = true;
            }
			else
			{
				specPassed = false;
			}
            break;
        case E_SLPMODE:
            if (testCNF.slpmode_cnf.current <= p_sTestSpec->slpmode_spec.max_current)
            {
                specPassed = true;
            }
            else
            {
                specPassed = false;
            }
            break;
        case E_CAMERA:
			if (p_sCommonCFG->camera_cfg.bCheckSum)
			{
				if (strcmp(testCNF.camera_cnf.mainCameraCheckSum, p_sTestSpec->camera_spec.mainCameraCheckSum) == 0)
				{
					specPassed = true;
				}
				else
				{
					specPassed = false;
				}
			}
			else
			{
				if (testCNF.camera_cnf.b_mainCamera)
				{
					specPassed = true;
				}
				else
				{
					specPassed = false;
				}
			}
            break;
        case E_CAMERA_SUB:
			if (p_sCommonCFG->camera_cfg.bCheckSum)
			{
				if (strcmp(testCNF.camera_cnf.subCameraCheckSum, p_sTestSpec->camera_spec.subCameraCheckSum) == 0)
				{
					specPassed = true;
				}
				else
				{
					specPassed = false;
				}
			}
			else
			{
				if (testCNF.camera_cnf.b_subCamera)
				{
					specPassed = true;
				}
				else
				{
					specPassed = false;
				}
			}
            break;
		case E_CAMERA_MAIN2:
			if (p_sCommonCFG->camera_cfg.bCheckSum)
			{
				if (strcmp(testCNF.camera_cnf.main2CameraCheckSum, p_sTestSpec->camera_spec.main2CameraCheckSum) == 0)
				{
					specPassed = true;
				}
				else
				{
					specPassed = false;
				}
			}
			else
			{
				if (testCNF.camera_cnf.b_main2Camera)
				{
					specPassed = true;
				}
				else
				{
					specPassed = false;
				}
			}
            break;
			
        case E_VIBRATOR:
#ifdef INTERNEL_DEBUG_VERSION
            specPassed = true;
#else
            if (p_sCommonCFG->vibrator_cfg.bOnBoard)
            {
                if (testCNF.vibrator_cnf.bPass)
                {
                    specPassed = true;
                }
                else
                {
                    specPassed = false;
                }
            }
            else
            {
                curr_before = testCNF.vibrator_cnf.d_current_before;
                curr_after = testCNF.vibrator_cnf.d_current_after;
                diff = testCNF.vibrator_cnf.d_current_after - testCNF.vibrator_cnf.d_current_before;
                if (diff >= p_sTestSpec->vibrator_spec.min_diff &&
					curr_after >= p_sTestSpec->vibrator_spec.min_current &&
					curr_after <= p_sTestSpec->vibrator_spec.max_current)
                {
                    specPassed = true;
                }
                else
                {
                    specPassed = false;
                }
            }
#endif
            break;
			
        case E_CHARGING:
            if (testCNF.adc_cnf.meas_charger_current <= p_sTestSpec->adc_spec.charger_current_max &&
				testCNF.adc_cnf.meas_charger_current >= p_sTestSpec->adc_spec.charger_current_min )
            {
                specPassed = true;
            }
            else
            {
                specPassed = false;
            }
            break;

		case E_BTS:
			if((testCNF.bts_cnf.bts[0].bts_temperature<= p_sTestSpec->bts_spec.soc_temp_max) && (testCNF.bts_cnf.bts[0].bts_temperature>=p_sTestSpec->bts_spec.soc_temp_min)
					&& (testCNF.bts_cnf.bts[1].bts_temperature<= p_sTestSpec->bts_spec.mdpa_temp_max) && (testCNF.bts_cnf.bts[1].bts_temperature>= p_sTestSpec->bts_spec.mdpa_temp_min))

			{
				specPassed = true;
			}
			else
			{
				specPassed = false;
			}

				break;
        case E_KEYPAD:
            if (testCNF.kpad_cnf.result)
            {
                specPassed = true;
            }
            else
            {
                specPassed = false;
            }
            break;
			
        case E_TOUCHSCR:
            if (testCNF.bTPOK)
            {
                specPassed = true;
            }
            else
            {
                specPassed = false;
            }
			
            break;
			
        case E_CMMB:
            if (testCNF.cmmb_cnf.bPass)
            {
                specPassed = true;
            }
            else
            {
                specPassed = false;
            }
            break;
			
        case E_RTC:
            if (testCNF.bRTCOK)
            {
                specPassed = true;
            }
            else
            {
                specPassed = false;
            }
            break;
			
        case E_ADC:
            if (fabs(testCNF.adc_cnf.meas_v1-testCNF.adc_cnf.psu_v1) <= p_sTestSpec->adc_spec.max_vol_diff)
            {
                specPassed = true;
            }
            else
            {
                specPassed = false;
            }
            break;
			
        case E_LED:
#ifdef INTERNEL_DEBUG_VERSION
            specPassed = true;
#else
            if (strcmp(testCNF.led_cnf.state, p_sTestSpec->led_spec.state) == 0)
            {
                specPassed = true;
            }
            else
            {
                specPassed = false;
            }
			
#endif
            break;
			
        case E_SPEAKER:
            if (testCNF.audio_cnf.speaker_data.amplL <= p_sTestSpec->audio_spec.max_speaker_data.amplL &&
				testCNF.audio_cnf.speaker_data.amplL >= p_sTestSpec->audio_spec.min_speaker_data.amplL &&
				testCNF.audio_cnf.speaker_data.amplR <= p_sTestSpec->audio_spec.max_speaker_data.amplR &&
				testCNF.audio_cnf.speaker_data.amplR >= p_sTestSpec->audio_spec.min_speaker_data.amplR &&
				testCNF.audio_cnf.speaker_data.freqL <= p_sTestSpec->audio_spec.max_speaker_data.freqL &&
				testCNF.audio_cnf.speaker_data.freqL >= p_sTestSpec->audio_spec.min_speaker_data.freqL &&
				testCNF.audio_cnf.speaker_data.freqR <= p_sTestSpec->audio_spec.max_speaker_data.freqR &&
				testCNF.audio_cnf.speaker_data.freqR >= p_sTestSpec->audio_spec.min_speaker_data.freqR)
            {
                specPassed = true;
            }
            else
            {
                specPassed = false;
            }
            break;
			
        case E_RECEIVER:
            if (testCNF.audio_cnf.receiver_data.amplL <= p_sTestSpec->audio_spec.max_receiver_data.amplL &&
				testCNF.audio_cnf.receiver_data.amplL >= p_sTestSpec->audio_spec.min_receiver_data.amplL &&
				testCNF.audio_cnf.receiver_data.amplR <= p_sTestSpec->audio_spec.max_receiver_data.amplR &&
				testCNF.audio_cnf.receiver_data.amplR >= p_sTestSpec->audio_spec.min_receiver_data.amplR &&
				testCNF.audio_cnf.receiver_data.freqL <= p_sTestSpec->audio_spec.max_receiver_data.freqL &&
				testCNF.audio_cnf.receiver_data.freqL >= p_sTestSpec->audio_spec.min_receiver_data.freqL &&
				testCNF.audio_cnf.receiver_data.freqR <= p_sTestSpec->audio_spec.max_receiver_data.freqR &&
				testCNF.audio_cnf.receiver_data.freqR >= p_sTestSpec->audio_spec.min_receiver_data.freqR)
            {
                specPassed = true;
            }
            else
            {
                specPassed = false;
            }
            break;
			
        case E_EARTHPHONE:
            if (testCNF.audio_cnf.headset_data.amplL <= p_sTestSpec->audio_spec.max_headset_data.amplL &&
				testCNF.audio_cnf.headset_data.amplL >= p_sTestSpec->audio_spec.min_headset_data.amplL &&
				testCNF.audio_cnf.headset_data.amplR <= p_sTestSpec->audio_spec.max_headset_data.amplR &&
				testCNF.audio_cnf.headset_data.amplR >= p_sTestSpec->audio_spec.min_headset_data.amplR &&
				testCNF.audio_cnf.headset_data.freqL <= p_sTestSpec->audio_spec.max_headset_data.freqL &&
				testCNF.audio_cnf.headset_data.freqL >= p_sTestSpec->audio_spec.min_headset_data.freqL &&
				testCNF.audio_cnf.headset_data.freqR <= p_sTestSpec->audio_spec.max_headset_data.freqR &&
				testCNF.audio_cnf.headset_data.freqR >= p_sTestSpec->audio_spec.min_headset_data.freqR)
            {
                specPassed = true;
            }
            else
            {
                specPassed = false;
            }
            break;
			
        case E_RSSI:
            if (testCNF.rssi_cnf.bPass)
            {
                specPassed = true;
            }
            else
            {
                specPassed = false;
            }
            break;
			
        case E_RING:
            break;
			
        case E_EARTHPHONE_STATE:
            specPassed = false;
            if (testCNF.er_state == p_sTestSpec->er_state_spec.state)
                specPassed = true;
            break;
			
        case E_WIFI:
            specPassed = false;
            if (testCNF.wifi_cnf.bPass)
            {
            	specPassed = true;
				// delete 
				/*
                if ((signed)testCNF.wifi_cnf.ftm_wifi.wifi_rssi >= p_sTestSpec->wifi_spec.ap_list[0].rssi)
                {
                    
                }
				else
				{
					specPassed = false;
				}
				*/
				// delete 
            }
			
            for (index = 0; index < strlen(p_sTestSpec->wifi_spec.local_bssid); index ++)
            {
                if (p_sTestSpec->wifi_spec.local_bssid[index] == 'x' ||
					p_sTestSpec->wifi_spec.local_bssid[index] == 'X')
                {
                    continue;
                }
				
                if (index + 1 > strlen(p_sTestSpec->wifi_spec.local_bssid))
                {
                    specPassed = false;
                    break;
                }
                else
                {
                    if (testCNF.wifi_cnf.main_mac[index] != p_sTestSpec->wifi_spec.local_bssid[index])
                    {
                        specPassed = false;
                        break;
                    }
                }
            }
            break;
			
        case E_GPS:
            if (testCNF.gps_cnf.bPass)
            {
                specPassed = true;
            }
            else
            {
                specPassed = false;
            }
            break;
			
        case E_BT:
            if (testCNF.bt_cnf.bPass)
            {
                specPassed = true;
            }
            else
            {
                specPassed = false;
            }
            break;
			
        case E_TV:
            if (testCNF.matv_cnf.bPass)
            {
                specPassed = true;
            }
            else
            {
                specPassed = false;
            }
            break;
			
        case E_FM:
            specPassed = true;
            if (testCNF.fm_cnf.cqi < p_sTestSpec->fm_spec.min_cqi)
                specPassed = false;
            break;
			
        case E_TCARD:
            if (testCNF.bMSDCOK)
            {
                specPassed = true;
            }
            else
            {
                specPassed = false;
            }
            break;
			
        case E_EMMC:
            if (testCNF.emmc_cnf.result)
            {
                specPassed = true;
            }
            else
            {
                specPassed = false;
            }
            break;
			
        case E_SIM:
            if (testCNF.sim_cnf.bPass)
            {
                specPassed = true;
            }
            else
            {
                specPassed = false;
            }
            break;
			
        case E_BARCODE:
            specPassed = true;
            for (index = 0; index < strlen(p_sTestSpec->barcode_spec.barcode); index ++)
            {
                if (p_sTestSpec->barcode_spec.barcode[index] == 'x' ||
					p_sTestSpec->barcode_spec.barcode[index] == 'X')
                {
                    continue;
                }
				
                if (index + 1 > strlen(testCNF.barcode))
                {
                    specPassed = false;
                    break;
                }
                else
                {
                    if (testCNF.barcode[index] != p_sTestSpec->barcode_spec.barcode[index])
                    {
                        specPassed = false;
                        break;
                    }
                }
            }
			
			if (specPassed)
			{
				sprintf (logStr, "Barcode result pass.\r\n");
				ATADLL_AddLog (ata_handle, logStr);
			}
			else
			{
				sprintf (logStr, "Barcode result %s is out of spec %s\r\n",
					testCNF.barcode, p_sTestSpec->barcode_spec.barcode);
				
				ATADLL_AddLog (ata_handle, logStr);
			}
            break;
			
        case E_SWVERSION:
            if (strstr(testCNF.sw_ver_cnf.modem_ver, p_sTestSpec->sw_spec.modem_version) != NULL &&
				strstr(testCNF.sw_ver_cnf.ap_ver, p_sTestSpec->sw_spec.ap_version) != NULL)
            {
                specPassed = true;
            }
            else
            {
                specPassed = false;
            }
            break;
			
        case E_HWVERSIOM:
            if (testCNF.hwver_cnf.d_voltage >= p_sTestSpec->hwver_spec.d_voltage_min &&
				testCNF.hwver_cnf.d_voltage <= p_sTestSpec->hwver_spec.d_voltage_max &&
				testCNF.hwver_cnf.gpio == p_sTestSpec->hwver_spec.i_gpio)
            {
                specPassed = true;
            }
            else
            {
                specPassed = false;
            }
            break;
			
        case E_GSENSOR:
#if 0
            if (testCNF.sensor_cnf.gsensor.g_sensor_x >= p_sTestSpec->gsensor_spec.min_gsensor.g_sensor_x &&
				testCNF.sensor_cnf.gsensor.g_sensor_y >= p_sTestSpec->gsensor_spec.min_gsensor.g_sensor_y &&
				testCNF.sensor_cnf.gsensor.g_sensor_z >= p_sTestSpec->gsensor_spec.min_gsensor.g_sensor_z &&
				testCNF.sensor_cnf.gsensor.g_sensor_x <= p_sTestSpec->gsensor_spec.max_gsensor.g_sensor_x &&
				testCNF.sensor_cnf.gsensor.g_sensor_y <= p_sTestSpec->gsensor_spec.max_gsensor.g_sensor_y &&
				testCNF.sensor_cnf.gsensor.g_sensor_z <= p_sTestSpec->gsensor_spec.max_gsensor.g_sensor_z)
            {
                specPassed = true;
            }
#endif
			
            specPassed = true;
            if (p_sCommonCFG->gsensor_cfg.bX)
            {
                if (testCNF.sensor_cnf.gsensor.g_sensor_x >= p_sTestSpec->gsensor_spec.min_gsensor.g_sensor_x &&
					testCNF.sensor_cnf.gsensor.g_sensor_x <= p_sTestSpec->gsensor_spec.max_gsensor.g_sensor_x )
                {
                }
                else
                {
                    specPassed = false;
                }
            }
			
            if (p_sCommonCFG->gsensor_cfg.bY)
            {
                if (testCNF.sensor_cnf.gsensor.g_sensor_y >= p_sTestSpec->gsensor_spec.min_gsensor.g_sensor_y &&
					testCNF.sensor_cnf.gsensor.g_sensor_y <= p_sTestSpec->gsensor_spec.max_gsensor.g_sensor_y )
                {
                }
                else
                {
                    specPassed = false;
                }
            }
			
            if (p_sCommonCFG->gsensor_cfg.bZ)
            {
                if (testCNF.sensor_cnf.gsensor.g_sensor_z >= p_sTestSpec->gsensor_spec.min_gsensor.g_sensor_z &&
					testCNF.sensor_cnf.gsensor.g_sensor_z <= p_sTestSpec->gsensor_spec.max_gsensor.g_sensor_z )
                {
                }
                else
                {
                    specPassed = false;
                }
            }
			
            if (testCNF.sensor_cnf.gsensor.g_sensor_x == 0.0 &&
				testCNF.sensor_cnf.gsensor.g_sensor_y == 0.0 &&
				testCNF.sensor_cnf.gsensor.g_sensor_z == 0.0)
            {
                specPassed = false;
            }
            break;
			
        case E_MSENSOR:
            if (testCNF.sensor_cnf.msensor.m_sensor_x >= p_sTestSpec->msensor_spec.min_msensor.m_sensor_x &&
				testCNF.sensor_cnf.msensor.m_sensor_y >= p_sTestSpec->msensor_spec.min_msensor.m_sensor_y &&
				testCNF.sensor_cnf.msensor.m_sensor_z >= p_sTestSpec->msensor_spec.min_msensor.m_sensor_z &&
				testCNF.sensor_cnf.msensor.m_sensor_x <= p_sTestSpec->msensor_spec.max_msensor.m_sensor_x &&
				testCNF.sensor_cnf.msensor.m_sensor_y <= p_sTestSpec->msensor_spec.max_msensor.m_sensor_y &&
				testCNF.sensor_cnf.msensor.m_sensor_z <= p_sTestSpec->msensor_spec.max_msensor.m_sensor_z)
            {
                specPassed = true;
            }
            else
            {
                specPassed = false;
            }
			
            if (testCNF.sensor_cnf.msensor.m_sensor_x == 0 &&
				testCNF.sensor_cnf.msensor.m_sensor_y == 0 &&
				testCNF.sensor_cnf.msensor.m_sensor_z == 0)
            {
                specPassed = false;
            }
            break;
			
        case E_ALSPS:
            if (testCNF.sensor_cnf.alsps.als <= p_sTestSpec->alsps_spec.max_alsps.als &&
				testCNF.sensor_cnf.alsps.ps  <= p_sTestSpec->alsps_spec.max_alsps.ps  &&
				testCNF.sensor_cnf.alsps.als >= p_sTestSpec->alsps_spec.min_alsps.als &&
				testCNF.sensor_cnf.alsps.ps  >= p_sTestSpec->alsps_spec.min_alsps.ps)
            {
                specPassed = true;
            }
            else
            {
                specPassed = false;
            }
			
            if (testCNF.sensor_cnf.alsps.als == 0 &&
				testCNF.sensor_cnf.alsps.ps == 0)
            {
                specPassed = false;
            }
			
            break;
			
        case E_GYROSCOPE:
            if (testCNF.sensor_cnf.gyroscope.gyroscope_x <= p_sTestSpec->gyroscope_spec.max_gyroscope.gyroscope_x &&
				testCNF.sensor_cnf.gyroscope.gyroscope_y <= p_sTestSpec->gyroscope_spec.max_gyroscope.gyroscope_y &&
				testCNF.sensor_cnf.gyroscope.gyroscope_z <= p_sTestSpec->gyroscope_spec.max_gyroscope.gyroscope_z &&
				testCNF.sensor_cnf.gyroscope.gyroscope_x >= p_sTestSpec->gyroscope_spec.min_gyroscope.gyroscope_x &&
				testCNF.sensor_cnf.gyroscope.gyroscope_y >= p_sTestSpec->gyroscope_spec.min_gyroscope.gyroscope_y &&
				testCNF.sensor_cnf.gyroscope.gyroscope_z >= p_sTestSpec->gyroscope_spec.min_gyroscope.gyroscope_z)
            {
                specPassed = true;
            }
            else
            {
                specPassed = false;
            }
            break;
			
        case E_MICBIAS:
#ifdef INTERNEL_DEBUG_VERSION
            specPassed = true;
#else
            if (testCNF.micbias_cnf.d_vMicbias0 <= p_sTestSpec->micbias_spec.dBias0Max &&
				testCNF.micbias_cnf.d_vMicbias0 >= p_sTestSpec->micbias_spec.dBias0Min &&
				testCNF.micbias_cnf.d_vMicbias1 <= p_sTestSpec->micbias_spec.dBias1Max &&
				testCNF.micbias_cnf.d_vMicbias1 >= p_sTestSpec->micbias_spec.dBias1Min)
            {
                specPassed = true;
            }
            else
            {
                specPassed = false;
            }
#endif
            break;
			
		case E_OTG:
#ifdef INTERNEL_DEBUG_VERSION
			specPassed = true;
#else
			if (testCNF.otg_cnf.d_OTGVol >= p_sTestSpec->otg_spec.dMinVol &&
				testCNF.otg_cnf.d_OTGVol <= p_sTestSpec->otg_spec.dMaxVol)
			{
				specPassed = true;
			}
			else
			{
				specPassed = false;
			}
			
			if (specPassed)
			{
				sprintf (logStr, "OTG result pass.\r\n");
				ATADLL_AddLog (ata_handle, logStr);
			}
			/*else
			{
				sprintf (logStr, "OTG result %.3f is out of spec (%.3f ~ %.3f)\r\n",
					testCNF.otg_cnf.d_OTGVol,
					p_sTestSpec->otg_spec.dMinVol,
					p_sTestSpec->otg_spec.dMaxVol
					);
				
				ATADLL_AddLog (ata_handle, logStr);
			}*/
#endif
			break;
			
		case E_HDMI:
			specPassed = testCNF.hdmi_cnf.bResult;
			break;

		//add by songjian 2016-3-1 (start)	
		/*
		case E_GPS_CALI:
        if (strstr(testCNF.sw_ver_cnf.modem_ver, p_sTestSpec->sw_spec.modem_version) != NULL &&
			strstr(testCNF.sw_ver_cnf.ap_ver, p_sTestSpec->sw_spec.ap_version) != NULL)
		{
			specPassed = true;
        }
        else
        {
            specPassed = true;
        }
        break;
        */
		//add by songjian 2016-3-1 (end)	
        }
		
#if 0
        row_index ++;
        break;
    }
#endif
	
    return specPassed;
}

void __stdcall CTestObject::UpdateTestResultCallback (int handle_index, E_ATDLL_TEST_ITEM_COUNT item)
{	
	if (item == E_BARCODE)
		{
			if(m_iUpdateBarcodeUi==0)
        		return;
		}

    glbTestObj[handle_index]->UpdateTestResultToCtrlList_Sub(item);
}

void CTestObject::UpdateTestResultToCtrlList_Sub (E_ATDLL_TEST_ITEM_COUNT item)
{
    char logStr[1024];
	
    pXListCtrl->LockWindowUpdate();	// ***** lock window updates while filling list *****
	
    int row_index = 0;
    int sim = 1;
    E_ATADLL_RESULT ata_result  = E_ATADLL_RESULT_SUCCESS;
    bool specPassed	= true;
    CString str     = _T("");
    char tmpChar[1024];
	
   // char item_name[128];
   // char item_result_str[128];
	
    bool firstTimeout = true;
    E_ATDLL_TEST_ITEM_COUNT test_item = E_LCD;
	char name_show[64];
	char str_show[6];
	bool ui_check=true;
	
#ifdef __ATA_CONCURRENT_FLOW__
    for (row_index = 0; row_index < testcases_number_bg + testcases_number_fg; row_index ++)
    {
        if (row_index < testcases_number_bg)
        {
            test_item = testcases_bg[row_index].item;
			if (item == test_item)
        	{
        		strcpy(name_show,testcases_bg[row_index].item_name);
           		 break;
       	 	}
        }
        else
        {
            test_item = testcases_fg[row_index - testcases_number_bg].item;

			if (item == test_item)
        	{
        		strcpy(name_show,testcases_fg[row_index - testcases_number_bg].item_name);
            	break;
       	 	}
        }
		
        /*if (item == test_item)
        {
            break;
        }*/
    }
#else
	for (test_item= E_LCD; test_item < E_TEST_ITEM_COUNT; test_item = (E_ATDLL_TEST_ITEM_COUNT)((int)test_item + 1))
    {
        if (!testFlag[test_item])
        {
            continue;
        }
		
		if (item == test_item)
		{
			//sprintf (item_name, "%s", CtrlListTestITem[item]);
			break;
		}
		
		row_index ++;
	}
#endif
	
	{
		pXListCtrl->DeleteProgress (row_index, DATA_COLUMN_INDEX);
		
        ata_result = testCNF.test_result[item];
		if (item == E_MHL)
		{
			ata_result = E_ATADLL_RESULT_SUCCESS;
		}
		
		
        if (ata_result == E_ATADLL_RESULT_NOT_SUPPORT)
        {
            str = "Fail";
			strcpy(str_show,"Fail");
            if (true)
            {
                pXListCtrl->SetItemText(row_index, RESULT_COLUMN_INDEX, str, RGB(255,255,255), RGB(214,247,30));
                pXListCtrl->SetItemText(row_index, DATA_COLUMN_INDEX, "Not Support", RGB(255,255,255), RGB(214,247,30));
                pXListCtrl->SetItemText(row_index, ITEM_COLUMN_INDEX, NULL, RGB(255,255,255), RGB(214,247,30));
            }
			
            pXListCtrl->SetBold (row_index, RESULT_COLUMN_INDEX, true);
            pXListCtrl->SetBold (row_index, DATA_COLUMN_INDEX, true);
            pXListCtrl->SetBold (row_index, ITEM_COLUMN_INDEX, true);
            goto End;
        }
		
        if (ata_result != E_ATADLL_RESULT_SUCCESS && ata_result !=E_ATADLL_RESULT_BARCODE_FLAG_OK)
        {
            str = "Fail";
			strcpy(str_show,"Fail");
			
#ifdef INTERNEL_DEBUG_VERSION
            if (firstTimeout && (*(p_sCommonCFG->stop_flag) != ATA_DLL_STOP_FLAG))
            {
                pXListCtrl->SetItemText(row_index, RESULT_COLUMN_INDEX, str, RGB(255,255,255), RGB(255, 0, 0));
                pXListCtrl->SetItemText(row_index, DATA_COLUMN_INDEX, NULL, RGB(255,255,255), RGB(255, 0, 0));
                pXListCtrl->SetItemText(row_index, ITEM_COLUMN_INDEX, NULL, RGB(255,255,255), RGB(255, 0, 0));
            }
            else
#endif
            {
                pXListCtrl->SetItemText(row_index, RESULT_COLUMN_INDEX, str, RGB(255,255,255), RGB(255, 0, 0));
                pXListCtrl->SetItemText(row_index, DATA_COLUMN_INDEX, NULL, RGB(255,255,255), RGB(255, 0, 0));
                pXListCtrl->SetItemText(row_index, ITEM_COLUMN_INDEX, NULL, RGB(255,255,255), RGB(255, 0, 0));
            }
			
            pXListCtrl->SetBold (row_index, RESULT_COLUMN_INDEX, true);
            pXListCtrl->SetBold (row_index, DATA_COLUMN_INDEX, true);
            pXListCtrl->SetBold (row_index, ITEM_COLUMN_INDEX, true);
			
            firstTimeout = false;
            goto End;
        }
		
        str = "";
        sprintf (tmpChar, "");
        specPassed  = false;
        unsigned int index  = 0;
        double curr_before  = 0.0;
        double curr_after   = 0.0;
        double diff         = 0.0;
        switch (item)
        {
        case E_LCD:
            if (testCNF.bLCMDetected)
            {
                specPassed = true;
				str = "Detected";
            }
			else
			{
				specPassed = false;
				str = "Undetected";
			}
            break;
		case E_USBTYPEC:
			if (testCNF.bUSBTypeCOK)
            {
                specPassed = true;
				str = "OK";
            }
			else
			{
				specPassed = false;
				str = "Fail";
			}
            break;
		case E_HUMIDITY:
            if (testCNF.bHumidityOK)
            {
                specPassed = true;
				str = "OK";
            }
			else
			{
				specPassed = false;
				str = "Fail";
			}
            break;
		case E_FINGERPRINT:
            if (testCNF.bFingerPrintOK)
            {
                specPassed = true;
				str = "OK";
            }
			else
			{
				specPassed = false;
				str = "Fail";
			}
            break;
	
		case E_EXT_BUCK:
            if (testCNF.bExtBuckOK)
            {
                specPassed = true;
				str = "OK";
            }
			else
			{
				specPassed = false;
				str = "Fail";
			}
            break;
		case E_BAROMETER:
            if (testCNF.bBarometerOK)
            {
                specPassed = true;
				str = "OK";
            }
			else
			{
				specPassed = false;
				str = "Fail";
			}
            break;
						
        case E_SLPMODE:
#ifdef INTERNEL_DEBUG_VERSION
            specPassed = true;
#else
            sprintf (tmpChar, "%.3f mA", testCNF.slpmode_cnf.current);
            str = tmpChar;
            if (testCNF.slpmode_cnf.current <= p_sTestSpec->slpmode_spec.max_current)
            {
                specPassed = true;
            }
            else
            {
            	sprintf(logStr,"[Idle Current][Spec Check fail]current is:%.3f mA  >   UI's max current (%.3f mA)   ",testCNF.slpmode_cnf.current,p_sTestSpec->slpmode_spec.max_current);
            	ATADLL_AddLog(ata_handle,logStr);
				specPassed = false;
				ui_check = false;
			}
#endif
            break;
			
		case E_OFFMODE:
#ifdef INTERNEL_DEBUG_VERSION
            specPassed = true;
#else
			sprintf (tmpChar, "%.3f uA", testCNF.offmode_cnf.current);
			str = tmpChar;
			if (testCNF.offmode_cnf.current <= p_sTestSpec->offmode_spec.d_maxCurrent)
            {
                specPassed = true;
            }
            else
            {
            	sprintf(logStr,"[Off Current][Spec Check fail]current is:%.3f mA  >   UI's max current (%.3f mA)   ",testCNF.offmode_cnf.current,p_sTestSpec->offmode_spec.d_maxCurrent);
            	ATADLL_AddLog(ata_handle,logStr);
        		specPassed = false;
				ui_check = false;
            }
#endif
			break;
			
        case E_CAMERA:
			if (p_sCommonCFG->camera_cfg.bCheckSum)
			{
				if (strcmp(testCNF.camera_cnf.mainCameraCheckSum, p_sTestSpec->camera_spec.mainCameraCheckSum) == 0)
				{
					specPassed = true;
				}
				else
				{
					sprintf(logStr,"[Main Camera][Spec Check fail]target's CRC is:%s  UI's CRC is:%s    mismatch",testCNF.camera_cnf.mainCameraCheckSum,p_sTestSpec->camera_spec.mainCameraCheckSum);
					ATADLL_AddLog(ata_handle,logStr);
					specPassed = false;
					ui_check = false;
				}
				
				sprintf (tmpChar, "%s", testCNF.camera_cnf.mainCameraCheckSum);
				str = tmpChar;
			}
			else
			{
				if (testCNF.camera_cnf.b_mainCamera)
				{
					specPassed = true;
					sprintf (tmpChar, "OK");
					str = tmpChar;
				}
				else
				{
					specPassed = false;
					sprintf (tmpChar, "Fail");
					str = tmpChar;
				}
			}
            break;
			
        case E_CAMERA_SUB:
			if (p_sCommonCFG->camera_cfg.bCheckSum)
			{
				if (strcmp(testCNF.camera_cnf.subCameraCheckSum, p_sTestSpec->camera_spec.subCameraCheckSum) == 0)
				{
					specPassed = true;
				}
				else
				{
					sprintf(logStr,"[Sub Camera][Spec Check fail] target's CRC is:%s  UI's CRC is:%s    mismatch",testCNF.camera_cnf.subCameraCheckSum,p_sTestSpec->camera_spec.subCameraCheckSum);
					ATADLL_AddLog(ata_handle,logStr);
					specPassed = false;
					ui_check = false;
				}
				
				sprintf (tmpChar, "%s", testCNF.camera_cnf.subCameraCheckSum);
				str = tmpChar;
			}
			else
			{
				if (testCNF.camera_cnf.b_subCamera)
				{
					specPassed = true;
					sprintf (tmpChar, "OK");
					str = tmpChar;
				}
				else
				{
					specPassed = false;
					sprintf (tmpChar, "Fail");
					str = tmpChar;
				}
			}
            break;
		case E_CAMERA_MAIN2:
			if (p_sCommonCFG->camera_cfg.bCheckSum)
			{
				if (strcmp(testCNF.camera_cnf.main2CameraCheckSum, p_sTestSpec->camera_spec.main2CameraCheckSum) == 0)
				{
					specPassed = true;
				}
				else
				{
					sprintf(logStr,"[Main2 Camera][Spec Check fail]target's CRC is:%s  UI's CRC is:%s    mismatch",testCNF.camera_cnf.main2CameraCheckSum,p_sTestSpec->camera_spec.main2CameraCheckSum);
					ATADLL_AddLog(ata_handle,logStr);
					specPassed = false;
					ui_check = false;
				}
				
				sprintf (tmpChar, "%s", testCNF.camera_cnf.main2CameraCheckSum);
				str = tmpChar;
			}
			else
			{
				if (testCNF.camera_cnf.b_main2Camera)
				{
					specPassed = true;
					sprintf (tmpChar, "OK");
					str = tmpChar;
				}
				else
				{
					specPassed = false;
					sprintf (tmpChar, "Fail");
					str = tmpChar;
				}
			}
            break;
			
        case E_VIBRATOR:
#ifdef INTERNEL_DEBUG_VERSION
			if(testCNF.vibrator_cnf.bPass)
			{
            	specPassed = true;
				str = "OK";
			}
			else
			{
				specPassed = false;
				str = "Fail";
			}

#else
            curr_before = testCNF.vibrator_cnf.d_current_before;
            curr_after = testCNF.vibrator_cnf.d_current_after;
            diff = testCNF.vibrator_cnf.d_current_after - testCNF.vibrator_cnf.d_current_before;
            if (diff >= p_sTestSpec->vibrator_spec.min_diff &&
				curr_after >= p_sTestSpec->vibrator_spec.min_current &&
				curr_after <= p_sTestSpec->vibrator_spec.max_current)
            {
                specPassed = true;
            }
			else
			{
				sprintf(logStr,"[Vibrator][Spec Check fail]target's current value is:%.3f   diff is:%.3f\r\nspec value range is:[%.3f,%.3f] spec min diff is:%.3f",testCNF.vibrator_cnf.d_current_after,diff,p_sTestSpec->vibrator_spec.min_current,p_sTestSpec->vibrator_spec.max_current,p_sTestSpec->vibrator_spec.min_diff);
				ATADLL_AddLog(ata_handle,logStr);
				specPassed = false;
				ui_check = false;
			}
            sprintf (tmpChar, "%.3f, %.3f", curr_before, curr_after);
            str = tmpChar;
#endif
            break;
			
        case E_CHARGING:
            if (testCNF.adc_cnf.meas_charger_current <= p_sTestSpec->adc_spec.charger_current_max &&
				testCNF.adc_cnf.meas_charger_current >= p_sTestSpec->adc_spec.charger_current_min )
            {
                specPassed = true;
            }
            else
            {
            	sprintf(logStr,"[Charger][Spec Check fail]target's Charger current is:%.3f mA  spec's current range is[%.3f,%.3f]",testCNF.adc_cnf.meas_charger_current,p_sTestSpec->adc_spec.charger_current_min,p_sTestSpec->adc_spec.charger_current_max);
				ATADLL_AddLog(ata_handle,logStr);
				specPassed = false;
				ui_check = false;
            }
			
            sprintf (tmpChar, "%.3f mA", testCNF.adc_cnf.meas_charger_current);
#ifdef __ATA30__
			sprintf (tmpChar, "%s, %d C", tmpChar, testCNF.adc_cnf.vbattemp);
#endif
            str += tmpChar;
            break;
			
        case E_KEYPAD:
            if (testCNF.kpad_cnf.result)
            {
                specPassed = true;
				str = "OK";
            }
			else
			{
				specPassed = false;
				str = "Fail";
			}
            break;
			
        case E_TOUCHSCR:
            if (testCNF.bTPOK)
            {
                specPassed = true;
				str = "OK";
            }
			else
			{
				specPassed = false;
				str = "Fail";
			}
            break;
			
        case E_CMMB:
            if (testCNF.cmmb_cnf.bPass)
            {
                specPassed = true;
				str = "OK";
            }
			else
			{
				specPassed = false;
				str = "Fail";
			}
            break;
			
        case E_RTC:
            if (testCNF.bRTCOK)
            {
                specPassed = true;
				str = "OK";
            }
			else
			{
				specPassed = false;
				str = "Fail";
			}
            break;
			
        case E_ADC:
#ifdef INTERNEL_DEBUG_VERSION
			if(testCNF.bChrgerDetected)
			{
				specPassed=true;
				str = "OK";
			}
			else
			{
				specPassed=false;
				str = "Fail";
			}
#else
            if (fabs(testCNF.adc_cnf.meas_v1-testCNF.adc_cnf.psu_v1) <= p_sTestSpec->adc_spec.max_vol_diff)
            {
                specPassed = true;
            }
			else
			{
				specPassed = false;
				sprintf(logStr,"[ADC][Spec check fail]target's battery voltage is:%.3f  testbox's measure voltage is:%.3f  ,the diff>spec max diff(%.3f)",
					testCNF.adc_cnf.meas_v1,testCNF.adc_cnf.psu_v1,p_sTestSpec->adc_spec.max_vol_diff);
				ATADLL_AddLog(ata_handle,logStr);
				ui_check = false;
			}
			
            sprintf (tmpChar, "%.3f, %.3f", testCNF.adc_cnf.psu_v1, testCNF.adc_cnf.meas_v1);
            str = tmpChar;
#endif
            break;
			
        case E_LED:
#ifdef INTERNEL_DEBUG_VERSION
			if(testCNF.bLEDDetected)
			{
            	specPassed = true;
				str = "OK";
			}
			else
			{
				specPassed = false;
				str = "Fail";
			}
#else
            if (strcmp(testCNF.led_cnf.state, p_sTestSpec->led_spec.state) == 0)
            {
                specPassed = true;     
            }
            else
            {
            	specPassed = false;
				sprintf(logStr,"[LED][Spec check fail]led state(%s) is different with spec(%s)",testCNF.led_cnf.state,p_sTestSpec->led_spec.state);
				ATADLL_AddLog(ata_handle,logStr);
				ui_check = false;
            }
			str = testCNF.led_cnf.state;
#endif
            break;
			
        case E_SPEAKER:
#ifdef __ATA30__
			if (testCNF.audio_cnf.bSpeakerPass)
#else
				if (testCNF.audio_cnf.speaker_data.amplL <= p_sTestSpec->audio_spec.max_speaker_data.amplL &&
                    testCNF.audio_cnf.speaker_data.amplL >= p_sTestSpec->audio_spec.min_speaker_data.amplL &&
                    testCNF.audio_cnf.speaker_data.amplR <= p_sTestSpec->audio_spec.max_speaker_data.amplR &&
                    testCNF.audio_cnf.speaker_data.amplR >= p_sTestSpec->audio_spec.min_speaker_data.amplR &&
                    testCNF.audio_cnf.speaker_data.freqL <= p_sTestSpec->audio_spec.max_speaker_data.freqL &&
                    testCNF.audio_cnf.speaker_data.freqL >= p_sTestSpec->audio_spec.min_speaker_data.freqL &&
                    testCNF.audio_cnf.speaker_data.freqR <= p_sTestSpec->audio_spec.max_speaker_data.freqR &&
                    testCNF.audio_cnf.speaker_data.freqR >= p_sTestSpec->audio_spec.min_speaker_data.freqR)
#endif
				{
					specPassed = true;
					str = "OK";
				}
				else
				{
					specPassed = false;
					str = "Fail";
					sprintf(logStr,"[Speaker][Speck check fail]target's amplL is:%d  amplR is:%d   freqL is:%d  freqR is:%d not match with spec amplL[%d,%d]  amplR[%d,%d] or freqL[%d,%d]  freqR[%d,%d]",
					testCNF.audio_cnf.speaker_data.amplL,testCNF.audio_cnf.speaker_data.amplR,testCNF.audio_cnf.speaker_data.freqL,testCNF.audio_cnf.speaker_data.freqR,p_sTestSpec->audio_spec.min_speaker_data.amplL,
					p_sTestSpec->audio_spec.max_speaker_data.amplL,p_sTestSpec->audio_spec.min_speaker_data.amplR,p_sTestSpec->audio_spec.max_speaker_data.amplR,
					p_sTestSpec->audio_spec.min_speaker_data.freqL,p_sTestSpec->audio_spec.max_speaker_data.freqL,p_sTestSpec->audio_spec.min_speaker_data.freqR,
					p_sTestSpec->audio_spec.max_speaker_data.freqR);
					ATADLL_AddLog(ata_handle,logStr);
					ui_check = false;
				}
				
				sprintf (tmpChar, "(%d, %d)(%d, %d)",
					testCNF.audio_cnf.speaker_data.freqL,
					testCNF.audio_cnf.speaker_data.amplL,
					testCNF.audio_cnf.speaker_data.freqR,
					testCNF.audio_cnf.speaker_data.amplR);
				
				str = tmpChar;
				break;
				
        case E_RECEIVER:
#ifdef __ATA30__
			if (testCNF.audio_cnf.bReceiverPass)
#else
				if (testCNF.audio_cnf.receiver_data.amplL <= p_sTestSpec->audio_spec.max_receiver_data.amplL &&
                    testCNF.audio_cnf.receiver_data.amplL >= p_sTestSpec->audio_spec.min_receiver_data.amplL &&
                    testCNF.audio_cnf.receiver_data.amplR <= p_sTestSpec->audio_spec.max_receiver_data.amplR &&
                    testCNF.audio_cnf.receiver_data.amplR >= p_sTestSpec->audio_spec.min_receiver_data.amplR &&
                    testCNF.audio_cnf.receiver_data.freqL <= p_sTestSpec->audio_spec.max_receiver_data.freqL &&
                    testCNF.audio_cnf.receiver_data.freqL >= p_sTestSpec->audio_spec.min_receiver_data.freqL &&
                    testCNF.audio_cnf.receiver_data.freqR <= p_sTestSpec->audio_spec.max_receiver_data.freqR &&
                    testCNF.audio_cnf.receiver_data.freqR >= p_sTestSpec->audio_spec.min_receiver_data.freqR)
#endif
				{
					specPassed = true;
					str = "OK";
				}
				else
				{
					specPassed = false;
					str = "Fail";
					sprintf(logStr,"[Receiver][Spec check fail]target's amplL is:%d  amplR is:%d   freqL is:%d  freqR is:%d not match with spec amplL[%d,%d]  amplR[%d,%d] or freqL[%d,%d]  freqR[%d,%d]",
					testCNF.audio_cnf.receiver_data.amplL,testCNF.audio_cnf.receiver_data.amplR,testCNF.audio_cnf.receiver_data.freqL,testCNF.audio_cnf.receiver_data.freqR,p_sTestSpec->audio_spec.min_receiver_data.amplL,
					p_sTestSpec->audio_spec.max_receiver_data.amplL,p_sTestSpec->audio_spec.min_receiver_data.amplR,p_sTestSpec->audio_spec.max_receiver_data.amplR,
					p_sTestSpec->audio_spec.min_receiver_data.freqL,p_sTestSpec->audio_spec.max_receiver_data.freqL,p_sTestSpec->audio_spec.min_receiver_data.freqR,
					p_sTestSpec->audio_spec.max_receiver_data.freqR);
					ATADLL_AddLog(ata_handle,logStr);
					ui_check = false;
				}
				
				sprintf (tmpChar, "(%d, %d)(%d, %d)",
					testCNF.audio_cnf.receiver_data.freqL,
					testCNF.audio_cnf.receiver_data.amplL,
					testCNF.audio_cnf.receiver_data.freqR,
					testCNF.audio_cnf.receiver_data.amplR);
				
				str = tmpChar;
				break;
				
        case E_EARTHPHONE:
#ifdef __ATA30__
			if (testCNF.audio_cnf.bHeadsetPass)
#else
				if (testCNF.audio_cnf.headset_data.amplL <= p_sTestSpec->audio_spec.max_headset_data.amplL &&
                    testCNF.audio_cnf.headset_data.amplL >= p_sTestSpec->audio_spec.min_headset_data.amplL &&
                    testCNF.audio_cnf.headset_data.amplR <= p_sTestSpec->audio_spec.max_headset_data.amplR &&
                    testCNF.audio_cnf.headset_data.amplR >= p_sTestSpec->audio_spec.min_headset_data.amplR &&
                    testCNF.audio_cnf.headset_data.freqL <= p_sTestSpec->audio_spec.max_headset_data.freqL &&
                    testCNF.audio_cnf.headset_data.freqL >= p_sTestSpec->audio_spec.min_headset_data.freqL &&
                    testCNF.audio_cnf.headset_data.freqR <= p_sTestSpec->audio_spec.max_headset_data.freqR &&
                    testCNF.audio_cnf.headset_data.freqR >= p_sTestSpec->audio_spec.min_headset_data.freqR)
#endif
				{
					specPassed = true;
					str = "OK";
				}
				else
				{
					specPassed = false;
					str = "Fail";
					sprintf(logStr,"[Earphone][Spec check fail]target's amplL is:%d  amplR is:%d   freqL is:%d  freqR is:%d not match with spec amplL[%d,%d]  amplR[%d,%d] or freqL[%d,%d]  freqR[%d,%d]",
					testCNF.audio_cnf.headset_data.amplL,testCNF.audio_cnf.headset_data.amplR,testCNF.audio_cnf.headset_data.freqL,testCNF.audio_cnf.headset_data.freqR,p_sTestSpec->audio_spec.min_headset_data.amplL,
					p_sTestSpec->audio_spec.max_headset_data.amplL,p_sTestSpec->audio_spec.min_headset_data.amplR,p_sTestSpec->audio_spec.max_headset_data.amplR,
					p_sTestSpec->audio_spec.min_headset_data.freqL,p_sTestSpec->audio_spec.max_headset_data.freqL,p_sTestSpec->audio_spec.min_headset_data.freqR,
					p_sTestSpec->audio_spec.max_headset_data.freqR);
					ATADLL_AddLog(ata_handle,logStr);
					ui_check = false;
				}
				
#ifdef __ATA30__
				sprintf (tmpChar, "(%d, %d)(%d, %d)",
					testCNF.audio_cnf.headset_data_L.freqL,
					testCNF.audio_cnf.headset_data_L.amplL,
					testCNF.audio_cnf.headset_data_R.freqR,
					testCNF.audio_cnf.headset_data_R.amplR);		
#else
				sprintf (tmpChar, "(%d, %d)(%d, %d)",
					testCNF.audio_cnf.headset_data.freqL,
					testCNF.audio_cnf.headset_data.amplL,
					testCNF.audio_cnf.headset_data.freqR,
					testCNF.audio_cnf.headset_data.amplR);
#endif
				
				str = tmpChar;
				break;
				
        case E_RSSI:
#if (defined __ATA30__) || (defined __ATA_LOAD30_TEST20__)
			testCNF.rssi_cnf.dBm = -1 * abs(testCNF.rssi_cnf.dBm);
			testCNF.rssi_cnf.dBm_lte = -1 * abs(testCNF.rssi_cnf.dBm_lte);
			if (testCNF.rssi_cnf.dBm >= p_sTestSpec->rssi_spec.min_dBm &&
				testCNF.rssi_cnf.dBm_lte >= p_sTestSpec->rssi_spec.min_dBm_lte)
			{
				specPassed = true;
			}
			else
			{
				specPassed = false;
				sprintf(logStr,"[RSSI][Spec check fail]target's 2G rssi is:%d   LTE rssi is:%d  spec's min 2G rssi level is:%d, min LTE rssi level is:%d",
					testCNF.rssi_cnf.dBm,testCNF.rssi_cnf.dBm_lte,p_sTestSpec->rssi_spec.min_dBm,p_sTestSpec->rssi_spec.min_dBm_lte);
				ATADLL_AddLog(ata_handle,logStr);
				ui_check = false;
			}
			sprintf (tmpChar, "%d, %d", testCNF.rssi_cnf.dBm, testCNF.rssi_cnf.dBm_lte);
			str = tmpChar;
#else
            if (testCNF.rssi_cnf.bPass)
            {
                specPassed = true;
                str = "OK";
            }
            else
            {
                specPassed = false;
                str = "Fail";
            }
#endif
            break;
			
        case E_RING:
            break;
			
        case E_EARTHPHONE_STATE:
            if (testCNF.er_state == p_sTestSpec->er_state_spec.state)
                specPassed = true;
			
            if (testCNF.er_state == 0)
            {
                str = "Plug out";
            }
            else if (testCNF.er_state == 1)
            {
                str = "Plug in";
            }
            else if (testCNF.er_state == 2)
            {
                str = "Mic in";
            }
            else if (testCNF.er_state == 3)
            {
                str = "Hook switch pressed";
            }
            else
            {
                str = "Unknown";
            }
            break;
			
        case E_WIFI:
            specPassed = true;
            if (testCNF.wifi_cnf.bPass)
            {
                if (/*(signed)testCNF.wifi_cnf.ftm_wifi.wifi_rssi >= p_sTestSpec->wifi_spec.ap_list[0].rssi &&*/
					strcmp(testCNF.wifi_cnf.ftm_wifi.wifi_mac, "") != 0)
                {
                    
                }
				else
				{
					specPassed = false;
					sprintf(logStr,"[WIFI][Spec check fail]target's RSSI(%d)<spec's RSSI(%d) or wifi mac %s ",(signed)testCNF.wifi_cnf.ftm_wifi.wifi_rssi,p_sTestSpec->wifi_spec.ap_list[0].rssi,testCNF.wifi_cnf.ftm_wifi.wifi_mac);
					ATADLL_AddLog(ata_handle,logStr);
					ui_check = false;
					
				}
            }
			else
			{
				specPassed = false;
			}
			
			if (p_sCommonCFG->wifi_cfg.bWIFIMacCheck) {
				for (index = 0; index < strlen(p_sTestSpec->wifi_spec.local_bssid); index ++)
				{
					if (p_sTestSpec->wifi_spec.local_bssid[index] == 'x' ||
						p_sTestSpec->wifi_spec.local_bssid[index] == 'X')
					{
						continue;
					}
					
					if (index + 1 > strlen(testCNF.wifi_cnf.main_mac))
					{
						specPassed = false;
						sprintf(logStr,"[WIFI][Spec check fail] target's mac(%s) mismatch with spec(%s)\r\n",testCNF.wifi_cnf.main_mac,p_sTestSpec->wifi_spec.local_bssid);
						ATADLL_AddLog(ata_handle,logStr);
						ui_check = false;
						break;
					}
					else
					{
						if (testCNF.wifi_cnf.main_mac[index] != p_sTestSpec->wifi_spec.local_bssid[index])
						{
							sprintf(logStr,"[WIFI][Spec check fail] target's mac(%s) mismatch with spec(%s)\r\n",testCNF.wifi_cnf.main_mac,p_sTestSpec->wifi_spec.local_bssid);
							ATADLL_AddLog(ata_handle,logStr);
							specPassed = false;
							ui_check = false;
							break;
						}
					}
				}
			}
			
			if (p_sCommonCFG->wifi_cfg.bWIFIMacCheck)
			{
				sprintf (tmpChar, "%s,%s", testCNF.wifi_cnf.main_mac, testCNF.wifi_cnf.ftm_wifi.wifi_name);
			}
			else
			{
				sprintf (tmpChar, "%s", testCNF.wifi_cnf.ftm_wifi.wifi_name);
			}
            str = tmpChar;
            break;
			
        case E_GPS:
            if (testCNF.gps_cnf.bPass)
            {
                specPassed = true;
                str = "OK";
            }
            else
            {
                specPassed = false;
                str = "Fail";
            }
            break;
			
        case E_BT:
		specPassed = true;
		 if (testCNF.bt_cnf.bPass)
	     {
	          specPassed = true;
	           str = "OK";     
	     }
	     else
	     {
	          specPassed = false;
	           str = "Fail";      
	     }
		 if(p_sCommonCFG->bt_cfg.bBtAddrCheck)
		 {
			for (index = 0; index < strlen(p_sTestSpec->bt_spec.local_bssid); index ++)
				{
					if (p_sTestSpec->bt_spec.local_bssid[index] == 'x' ||
						p_sTestSpec->bt_spec.local_bssid[index] == 'X')
					{
						continue;
					}
					
					if (index + 1 > strlen(testCNF.bt_cnf.bt_addr))
					{
						specPassed = false;
						sprintf(logStr,"[BT][Spec check fail] target's add(%s) mismatch with spec(%s)\r\n",testCNF.bt_cnf.bt_addr,p_sTestSpec->bt_spec.local_bssid);
						ATADLL_AddLog(ata_handle,logStr);
						ui_check = false;
						break;
					}
					else
					{
						if (testCNF.bt_cnf.bt_addr[index] != p_sTestSpec->bt_spec.local_bssid[index])
						{
							specPassed = false;
							sprintf(logStr,"[BT][Spec check fail] target's add(%s) mismatch with spec(%s)\r\n",testCNF.bt_cnf.bt_addr,p_sTestSpec->bt_spec.local_bssid);
							ATADLL_AddLog(ata_handle,logStr);
							ui_check = false;
							break;
						}
					}
				}
			
			}
		 if (p_sCommonCFG->bt_cfg.bBtAddrCheck)
		 {
				sprintf (tmpChar, "%s", testCNF.bt_cnf.bt_addr);
				str = tmpChar;
		 }  
            break;
			
        case E_TV:
            specPassed = true;
            if (strstr(testCNF.matv_cnf.matv_chip, p_sTestSpec->atv_chip) == NULL)
                specPassed = false;
            if (!testCNF.matv_cnf.freq_lock_status)
                specPassed = false;
            break;
			
        case E_FM:
            specPassed = true;
            if (testCNF.fm_cnf.cqi < p_sTestSpec->fm_spec.min_cqi)
            {
                specPassed = false;
				sprintf(logStr,"[FM][Spec check fail]target's rssi(%d)<spec's min rssi(%d)",testCNF.fm_cnf.cqi,p_sTestSpec->fm_spec.min_cqi);
				ATADLL_AddLog(ata_handle,logStr);
				ui_check = false;
            }
			
            sprintf (tmpChar, "Freq:%d -> ", p_sCommonCFG->fm_freq);
            sprintf (tmpChar, "%sRSSI:%d", tmpChar, testCNF.fm_cnf.cqi);
            str = tmpChar;
            break;
			
        case E_TCARD:
            if (testCNF.bMSDCOK)
            {
                specPassed = true;
#if (defined __ATA30__) || (defined __ATA_LOAD30_TEST20__)
				sprintf (tmpChar, "(%d,%d)(%d,%d)", 
					testCNF.memcard_cnf.memcard.sd1_total_size,
					testCNF.memcard_cnf.memcard.sd1_free_size,
					testCNF.memcard_cnf.memcard.sd2_total_size,
					testCNF.memcard_cnf.memcard.sd2_free_size);
				str = tmpChar;
#else
				str = "Detected";
#endif
            }
            else
            {
                str = "Undetected";
            }
            break;
			
        case E_EMMC:
            if (testCNF.emmc_cnf.result)
            {
                specPassed = true;
#if (defined __ATA30__) || (defined __ATA_LOAD30_TEST20__)
				sprintf (tmpChar, "%.3f GB", testCNF.emmc_cnf.emmc.capacity);
				str = tmpChar;
#else
				str = "Detected";
#endif
            }
            else
            {
                str = "Undetected";
            }
            break;
		case E_BTS:                         
#if (defined __ATA30__) || (defined __ATA_LOAD30_TEST20__)
				sprintf (tmpChar, "%s, %.1f¡æ; %s,%.1f¡æ", testCNF.bts_cnf.bts[0].bts_sensor_name,testCNF.bts_cnf.bts[0].bts_temperature,testCNF.bts_cnf.bts[1].bts_sensor_name,testCNF.bts_cnf.bts[1].bts_temperature);
				str = tmpChar;

				if((testCNF.bts_cnf.bts[0].bts_temperature<= p_sTestSpec->bts_spec.soc_temp_max) && (testCNF.bts_cnf.bts[0].bts_temperature>=p_sTestSpec->bts_spec.soc_temp_min)
					&& (testCNF.bts_cnf.bts[1].bts_temperature<= p_sTestSpec->bts_spec.mdpa_temp_max) && (testCNF.bts_cnf.bts[1].bts_temperature>= p_sTestSpec->bts_spec.mdpa_temp_min))
					specPassed = true;
				else
					specPassed = false;
#else
				str = "Not Support";
				specPassed = false;
#endif
           
            break;
			
        case E_SIM:
            if (testCNF.sim_cnf.bPass)
            {
                specPassed = true;
                str = "Detected";
            }
            else
            {
                specPassed = false;
                str = "Undetected";
            }
            break;
			
        case E_BARCODE:
            specPassed = true;
            for (index = 0; index < strlen(p_sTestSpec->barcode_spec.barcode); index ++)
            {
                if (p_sTestSpec->barcode_spec.barcode[index] == 'x' ||
					p_sTestSpec->barcode_spec.barcode[index] == 'X')
                {
                    continue;
                }
				
                if (index + 1 > strlen(testCNF.barcode))
                {
                    specPassed = false;
					sprintf(logStr,"[Barcode][Spec check fail]barcode(%s) mismatch with spec(%s)",testCNF.barcode,p_sTestSpec->barcode_spec.barcode);
					ATADLL_AddLog(ata_handle,logStr);
					ui_check = false;
                    break;
                }
                else
                {
                    if (testCNF.barcode[index] != p_sTestSpec->barcode_spec.barcode[index])
                    {
                        specPassed = false;
						sprintf(logStr,"[Barcode][Spec check fail]barcode(%s) mismatch with spec(%s)",testCNF.barcode,p_sTestSpec->barcode_spec.barcode);
						ATADLL_AddLog(ata_handle,logStr);
						ui_check = false;
                        break;
                    }
                }
            }
			
            str = testCNF.barcode;
            break;
			
        case E_SWVERSION:
            if (strstr(testCNF.sw_ver_cnf.modem_ver, p_sTestSpec->sw_spec.modem_version) != NULL &&
				strstr(testCNF.sw_ver_cnf.ap_ver, p_sTestSpec->sw_spec.ap_version) != NULL)
            {
                specPassed = true;
            }
			else
			{
				specPassed = false;
				sprintf(logStr,"[SW Version][Spec check fail] target's ap version(%s),modem version(%s) not mismatch with spec ap(%s),modem(%s)",testCNF.sw_ver_cnf.ap_ver,
					testCNF.sw_ver_cnf.modem_ver,p_sTestSpec->sw_spec.ap_version,p_sTestSpec->sw_spec.modem_version);
				ATADLL_AddLog(ata_handle,logStr);
				ui_check = false;
			}
			
            str = testCNF.sw_ver_cnf.modem_ver;
            str += ";";
            str += testCNF.sw_ver_cnf.ap_ver;
            break;
			
        case E_HWVERSIOM:
            specPassed = true;
            str = "OK";
            break;
			
        case E_GSENSOR:
			
			specPassed = true;
			if (p_sCommonCFG->gsensor_cfg.bX)
			{
				if (testCNF.sensor_cnf.gsensor.g_sensor_x >= p_sTestSpec->gsensor_spec.min_gsensor.g_sensor_x &&
                    testCNF.sensor_cnf.gsensor.g_sensor_x <= p_sTestSpec->gsensor_spec.max_gsensor.g_sensor_x)
				{
				}
				else
				{
					specPassed = false;
					sprintf(logStr,"[Gsensor X-cor][Spec check fail]target's X-Cor is :%.3f not in the spec range[%.3f,%.3f]",
						testCNF.sensor_cnf.gsensor.g_sensor_x,p_sTestSpec->gsensor_spec.min_gsensor.g_sensor_x,p_sTestSpec->gsensor_spec.max_gsensor.g_sensor_x);
					ATADLL_AddLog(ata_handle,logStr);
					ui_check = false;
				}
			}
			
			if (p_sCommonCFG->gsensor_cfg.bY)
			{
				if (testCNF.sensor_cnf.gsensor.g_sensor_y >= p_sTestSpec->gsensor_spec.min_gsensor.g_sensor_y &&
					testCNF.sensor_cnf.gsensor.g_sensor_y <= p_sTestSpec->gsensor_spec.max_gsensor.g_sensor_y)
				{
				}
				else
				{
					specPassed = false;
					sprintf(logStr,"[Gsensor Y-cor][Spec check fail]target's Y-Cor is :%.3f not in the spec range[%.3f,%.3f]",
						testCNF.sensor_cnf.gsensor.g_sensor_y,p_sTestSpec->gsensor_spec.min_gsensor.g_sensor_y,p_sTestSpec->gsensor_spec.max_gsensor.g_sensor_y);
					ATADLL_AddLog(ata_handle,logStr);
					ui_check = false;
				}
			}
			
			if (p_sCommonCFG->gsensor_cfg.bZ)
			{
				if (testCNF.sensor_cnf.gsensor.g_sensor_z >= p_sTestSpec->gsensor_spec.min_gsensor.g_sensor_z &&
					testCNF.sensor_cnf.gsensor.g_sensor_z <= p_sTestSpec->gsensor_spec.max_gsensor.g_sensor_z)
				{
				}
				else
				{
					specPassed = false;
					sprintf(logStr,"[Gsensor Z-cor][Spec check fail]target's Z-Cor is :%.3f not in the spec range[%.3f,%.3f]",
						testCNF.sensor_cnf.gsensor.g_sensor_z,p_sTestSpec->gsensor_spec.min_gsensor.g_sensor_z,p_sTestSpec->gsensor_spec.max_gsensor.g_sensor_z);
					ATADLL_AddLog(ata_handle,logStr);
					ui_check = false;
				}
			}
			
			
            if (testCNF.sensor_cnf.gsensor.g_sensor_x == 0.0 &&
				testCNF.sensor_cnf.gsensor.g_sensor_y == 0.0 &&
				testCNF.sensor_cnf.gsensor.g_sensor_z == 0.0)
            {
                specPassed = false;
				sprintf(logStr,"[Gsensor][Spec check fail]target's x-cor=0 ,y-cor=0,z-cor=0,do not get the correct sensor value,please check!");
				ATADLL_AddLog(ata_handle,logStr);
				ui_check = false;
            }
			
			
            sprintf (tmpChar, "(%f)-(%f)-(%f)-(%d)",
				testCNF.sensor_cnf.gsensor.g_sensor_x,
				testCNF.sensor_cnf.gsensor.g_sensor_y,
				testCNF.sensor_cnf.gsensor.g_sensor_z,
				testCNF.sensor_cnf.gsensor.accuracy);
            str = tmpChar;
            break;
			
        case E_MSENSOR:
            if (testCNF.sensor_cnf.msensor.m_sensor_x >= p_sTestSpec->msensor_spec.min_msensor.m_sensor_x &&
				testCNF.sensor_cnf.msensor.m_sensor_y >= p_sTestSpec->msensor_spec.min_msensor.m_sensor_y &&
				testCNF.sensor_cnf.msensor.m_sensor_z >= p_sTestSpec->msensor_spec.min_msensor.m_sensor_z &&
				testCNF.sensor_cnf.msensor.m_sensor_x <= p_sTestSpec->msensor_spec.max_msensor.m_sensor_x &&
				testCNF.sensor_cnf.msensor.m_sensor_y <= p_sTestSpec->msensor_spec.max_msensor.m_sensor_y &&
				testCNF.sensor_cnf.msensor.m_sensor_z <= p_sTestSpec->msensor_spec.max_msensor.m_sensor_z)
            {
                specPassed = true;
            }
			else
			{
				specPassed = false;
				sprintf(logStr,"[Msensor][Spec check fail]target's value(%d,%d,%d) not in the spec range(%d,%d,%d)~(%d,%d,%d)",
					testCNF.sensor_cnf.msensor.m_sensor_x,testCNF.sensor_cnf.msensor.m_sensor_y,testCNF.sensor_cnf.msensor.m_sensor_z,p_sTestSpec->msensor_spec.min_msensor.m_sensor_x,
					p_sTestSpec->msensor_spec.min_msensor.m_sensor_y,p_sTestSpec->msensor_spec.min_msensor.m_sensor_z,p_sTestSpec->msensor_spec.max_msensor.m_sensor_x,
					p_sTestSpec->msensor_spec.max_msensor.m_sensor_y,p_sTestSpec->msensor_spec.max_msensor.m_sensor_z);
				ATADLL_AddLog(ata_handle,logStr);
				ui_check = false;
			}
			
            if (testCNF.sensor_cnf.msensor.m_sensor_x == 0 &&
				testCNF.sensor_cnf.msensor.m_sensor_y == 0 &&
				testCNF.sensor_cnf.msensor.m_sensor_z == 0)
            {
                specPassed = false;
				sprintf(logStr,"[Msensor][Spec check fail]target's x-cor=0 ,y-cor=0,z-cor=0,do not get the correct sensor value,please check!");
				ATADLL_AddLog(ata_handle,logStr);
				ui_check = false;
            }
			
            sprintf (tmpChar, "(%d)-(%d)-(%d)-(%d)",
				testCNF.sensor_cnf.msensor.m_sensor_x,
				testCNF.sensor_cnf.msensor.m_sensor_y,
				testCNF.sensor_cnf.msensor.m_sensor_z,
				testCNF.sensor_cnf.msensor.accuracy);
            str = tmpChar;
            break;
			
        case E_ALSPS:
			
			specPassed = true;
			if (p_sCommonCFG->alsps_cfg.bAls)
			{
				if (testCNF.sensor_cnf.alsps.als <= p_sTestSpec->alsps_spec.max_alsps.als &&
					testCNF.sensor_cnf.alsps.als >= p_sTestSpec->alsps_spec.min_alsps.als)
				{
				}
				else
				{
					specPassed = false;
					sprintf(logStr,"[ALS][Spec check fail]target's ALS value(%d) not in the spec range[%d,%d]",testCNF.sensor_cnf.alsps.als,p_sTestSpec->alsps_spec.min_alsps.als,p_sTestSpec->alsps_spec.max_alsps.als);
					ATADLL_AddLog(ata_handle,logStr);
					ui_check = false;
				}
			}
			
			if (p_sCommonCFG->alsps_cfg.bPs)
			{
				if (testCNF.sensor_cnf.alsps.ps  <= p_sTestSpec->alsps_spec.max_alsps.ps &&
					testCNF.sensor_cnf.alsps.ps  >= p_sTestSpec->alsps_spec.min_alsps.ps)
				{
				}
				else
				{
					specPassed = false;
					sprintf(logStr,"[PS][Spec check fail]target's PS value(%d) not in the spec range[%d,%d]",testCNF.sensor_cnf.alsps.ps,p_sTestSpec->alsps_spec.min_alsps.ps,p_sTestSpec->alsps_spec.max_alsps.ps);
					ATADLL_AddLog(ata_handle,logStr);
					ui_check = false;
				}
			}
			
            if (testCNF.sensor_cnf.alsps.als == 0 &&
				testCNF.sensor_cnf.alsps.ps == 0)
            {
                specPassed =  false;
				sprintf(logStr,"[ALS/PS][Spec check fail]target's als=0 ,ps=0,do not get the correct sensor value,please check!");
				ATADLL_AddLog(ata_handle,logStr);
				ui_check = false;
            }
			
            sprintf (tmpChar, "(%d)-(%d)",
				testCNF.sensor_cnf.alsps.als, testCNF.sensor_cnf.alsps.ps);
            str = tmpChar;
            break;
			
        case E_GYROSCOPE:
            if (testCNF.sensor_cnf.gyroscope.gyroscope_x <= p_sTestSpec->gyroscope_spec.max_gyroscope.gyroscope_x &&
				testCNF.sensor_cnf.gyroscope.gyroscope_y <= p_sTestSpec->gyroscope_spec.max_gyroscope.gyroscope_y &&
				testCNF.sensor_cnf.gyroscope.gyroscope_z <= p_sTestSpec->gyroscope_spec.max_gyroscope.gyroscope_z &&
				testCNF.sensor_cnf.gyroscope.gyroscope_x >= p_sTestSpec->gyroscope_spec.min_gyroscope.gyroscope_x &&
				testCNF.sensor_cnf.gyroscope.gyroscope_y >= p_sTestSpec->gyroscope_spec.min_gyroscope.gyroscope_y &&
				testCNF.sensor_cnf.gyroscope.gyroscope_z >= p_sTestSpec->gyroscope_spec.min_gyroscope.gyroscope_z)
            {
                specPassed = true;
            }
			else
			{
				specPassed =false;
				sprintf(logStr,"[Gyroscope][Spec check fail]target's value(%d,%d,%d) not in the spec range(%d,%d,%d)~(%d,%d,%d)",testCNF.sensor_cnf.gyroscope.gyroscope_x,
					testCNF.sensor_cnf.gyroscope.gyroscope_y,testCNF.sensor_cnf.gyroscope.gyroscope_z,p_sTestSpec->gyroscope_spec.min_gyroscope.gyroscope_x,
					p_sTestSpec->gyroscope_spec.min_gyroscope.gyroscope_y,p_sTestSpec->gyroscope_spec.min_gyroscope.gyroscope_z,p_sTestSpec->gyroscope_spec.max_gyroscope.gyroscope_x,
					p_sTestSpec->gyroscope_spec.max_gyroscope.gyroscope_y,p_sTestSpec->gyroscope_spec.max_gyroscope.gyroscope_z);
				ATADLL_AddLog(ata_handle,logStr);
				ui_check = false;
			}
			
            if (testCNF.sensor_cnf.gyroscope.gyroscope_x == 0.0 &&
				testCNF.sensor_cnf.gyroscope.gyroscope_y == 0.0 &&
				testCNF.sensor_cnf.gyroscope.gyroscope_z == 0.0)
            {
                specPassed = false;
				sprintf(logStr,"[Gyroscope][Spec check fail]target's x-cor=0 ,y-cor=0,z-cor=0,do not get the correct sensor value,please check!");
				ATADLL_AddLog(ata_handle,logStr);
				ui_check = false;
            }
			
            sprintf (tmpChar, "(%f)-(%f)-(%f)-(%d)",
				testCNF.sensor_cnf.gyroscope.gyroscope_x,
				testCNF.sensor_cnf.gyroscope.gyroscope_y,
				testCNF.sensor_cnf.gyroscope.gyroscope_z,
				testCNF.sensor_cnf.gyroscope.accuracy);
            str = tmpChar;
            break;
			
		case E_MICBIAS:
#ifdef INTERNEL_DEBUG_VERSION
            specPassed = true;
#else
            if (testCNF.micbias_cnf.d_vMicbias0 <= p_sTestSpec->micbias_spec.dBias0Max &&
				testCNF.micbias_cnf.d_vMicbias0 >= p_sTestSpec->micbias_spec.dBias0Min &&
				testCNF.micbias_cnf.d_vMicbias1 <= p_sTestSpec->micbias_spec.dBias1Max &&
				testCNF.micbias_cnf.d_vMicbias1 >= p_sTestSpec->micbias_spec.dBias1Min)
            {
                specPassed = true;
            }
            else
            {
                specPassed = false;
				sprintf(logStr,"[Micbias][Spec check fail]target's main micbias voltage(%.3f),sub micbias voltage(%.3f) not in the spec range main[%.3f,%.3f],sub[%.3f,%.3f]",
				testCNF.micbias_cnf.d_vMicbias0,testCNF.micbias_cnf.d_vMicbias1,p_sTestSpec->micbias_spec.dBias0Min,p_sTestSpec->micbias_spec.dBias0Max,
				p_sTestSpec->micbias_spec.dBias1Min,p_sTestSpec->micbias_spec.dBias1Max);
				ATADLL_AddLog(ata_handle,logStr);
				ui_check = false;
            }
#endif
			
			sprintf (tmpChar, "%.3f, %.3f", testCNF.micbias_cnf.d_vMicbias0, testCNF.micbias_cnf.d_vMicbias1);
			str = tmpChar;
			
            break;
			
		case E_OTG:
#ifdef INTERNEL_DEBUG_VERSION
			specPassed = true;
#else
			
			if (testCNF.otg_cnf.d_OTGVol >= p_sTestSpec->otg_spec.dMinVol &&
				testCNF.otg_cnf.d_OTGVol <= p_sTestSpec->otg_spec.dMaxVol)
			{
				specPassed = true;
			}
			else
			{
				specPassed = false;
				sprintf(logStr,"[OTG][Spec check fail]target's voltage(%.3f V) not in the spec range[%.3f,%.3f]",testCNF.otg_cnf.d_OTGVol,p_sTestSpec->otg_spec.dMinVol,
					p_sTestSpec->otg_spec.dMaxVol);
				ATADLL_AddLog(ata_handle,logStr);
				ui_check = false;
			}
#endif
			
			sprintf (tmpChar, "%.3f V", testCNF.otg_cnf.d_OTGVol);
			str = tmpChar;
			break;
			
		case E_HDMI:
			specPassed = testCNF.hdmi_cnf.bResult;
			if (specPassed)
				sprintf (tmpChar, "Finished");
			else
				sprintf (tmpChar, "Failed");
			
			str = tmpChar;
			break;
			
		case E_MHL:
			specPassed = true;
			sprintf (tmpChar, "Wating Cable Plug In");
			str = tmpChar;
			break;

		//add by songjian 2016-3-1 (start)	
		case E_GPS_CALI:
            if (testCNF.bGPSCALIOK)
            {
                specPassed = true;
				str = "GPS Cali Pass";
            }
			else
			{
				specPassed = false;
				str = "GPS Cali Fail";
				ui_check = false;
			}
            break;
		//add by songjian 2016-3-1 (end)	
        }
		if(specPassed)
			strcpy(str_show,"OK");
		else
			strcpy(str_show,"Fail");
		
        if (str != "")
        {
            pXListCtrl->SetItemToolTipText (row_index, RESULT_COLUMN_INDEX, str);
            pXListCtrl->SetItemToolTipText (row_index, DATA_COLUMN_INDEX, str);
            pXListCtrl->SetItemToolTipText (row_index, ITEM_COLUMN_INDEX, str);
			
            if (specPassed)
            {
                pXListCtrl->SetItemText(row_index, DATA_COLUMN_INDEX, str, RGB(0,0,0), RGB(194,209,166));
            }
            else
            {
                pXListCtrl->SetItemText(row_index, DATA_COLUMN_INDEX, str, RGB(255,255,255), RGB(255,0,0));
            }
        }
		
        if (specPassed)
        {
			if (item != E_HDMI)
			{
				str = "P";
			}
			else
			{
				str = "P";
			}
            
            pXListCtrl->SetItemText(row_index, RESULT_COLUMN_INDEX, str, RGB(0,0,0), RGB(194,209,166));
            pXListCtrl->SetItemText(row_index, DATA_COLUMN_INDEX, NULL, RGB(0,0,0), RGB(194,209,166));
            pXListCtrl->SetItemText(row_index, ITEM_COLUMN_INDEX, NULL, RGB(0,0,0), RGB(194,209,166));
			
			if (item != E_HDMI && item != E_MHL)
			{
				pXListCtrl->SetItemText(row_index, RESULT_COLUMN_INDEX, str, RGB(0,0,0), RGB(0,255,0));
				pXListCtrl->SetItemText(row_index, DATA_COLUMN_INDEX, NULL, RGB(0,0,0), RGB(0,255,0));
				pXListCtrl->SetItemText(row_index, ITEM_COLUMN_INDEX, NULL, RGB(0,0,0), RGB(0,255,0));
			}
			else
			{
				pXListCtrl->SetItemText(row_index, RESULT_COLUMN_INDEX, str, RGB(0,0,0), RGB(255,255,0));
				pXListCtrl->SetItemText(row_index, DATA_COLUMN_INDEX, NULL, RGB(0,0,0), RGB(255,255,0));
				pXListCtrl->SetItemText(row_index, ITEM_COLUMN_INDEX, NULL, RGB(0,0,0), RGB(255,255,0));
			}
			
            pXListCtrl->SetBold (row_index, RESULT_COLUMN_INDEX, true);
            pXListCtrl->SetBold (row_index, DATA_COLUMN_INDEX, true);
            pXListCtrl->SetBold (row_index, ITEM_COLUMN_INDEX, true);
        }
        else
        {
            str = "F";
            pXListCtrl->SetItemText(row_index, RESULT_COLUMN_INDEX, str, RGB(255,255,255), RGB(255,0,0));
            pXListCtrl->SetItemText(row_index, DATA_COLUMN_INDEX, NULL, RGB(255,255,255), RGB(255,0,0));
            pXListCtrl->SetItemText(row_index, ITEM_COLUMN_INDEX, NULL, RGB(255,255,255), RGB(255,0,0));
			
            pXListCtrl->SetBold (row_index, RESULT_COLUMN_INDEX, true);
            pXListCtrl->SetBold (row_index, DATA_COLUMN_INDEX, true);
            pXListCtrl->SetBold (row_index, ITEM_COLUMN_INDEX, true);
        }
    }	
End:
//update  csv log file buffer
	char item_result[512];
	char result_status[64];
	if(ui_check==false)
		strcpy(result_status,"E_ATADLL_RESULT_SPEC_CHECK_FAIL");
	else
		sprintf(result_status,"%s",AtaStatusToString(testCNF.test_result[item]));

		sprintf(item_result,"%s,%d,%s,%s",name_show,test_times+1,str_show,result_status);
		ATADLL_AddLog(ata_handle,item_result);

		ATADLL_CSVLog(ata_handle,item_result);

		pXListCtrl->UnlockWindowUpdate();
}

void CTestObject::UpdateTestResultToCtrlList ()
{
    int row_index = 0;
    E_ATDLL_TEST_ITEM_COUNT test_item = E_LCD;
	
    // Write test flag into barcode
    if (p_sCommonCFG->barcodeFlag_cfg.bSetFlag || stressfailStop==1)
    {
        //bool finalPassed = true;
		
#ifdef __ATA_CONCURRENT_FLOW__
		for (row_index = 0; row_index < testcases_number_bg + testcases_number_fg; row_index ++)
		{
			if (row_index < testcases_number_bg)
            {
                test_item = testcases_bg[row_index].item;
            }
            else
            {
                test_item = testcases_fg[row_index - testcases_number_bg].item;
            }
			
            if (!QueryTestItemResult (test_item))
            {
                finalPassed = false;
                break;
            }
		}
#else
		for (test_item = E_LCD; test_item < E_TEST_ITEM_COUNT; test_item = (E_ATDLL_TEST_ITEM_COUNT)((int)test_item + 1))
		{
			if (!testFlag[test_item])
			{
				continue;
			}
			
			if (!QueryTestItemResult (test_item))
            {
                finalPassed = false;
                break;
            }
		}
#endif
		
		if (p_sCommonCFG->barcodeFlag_cfg.bSetFlag)
		{
			E_ATADLL_RESULT dll_result = ATADLL_UpdateBarcodeFlag (ata_handle, finalPassed);
			if (dll_result != E_ATADLL_RESULT_SUCCESS && dll_result != E_ATADLL_RESULT_WARNING_IGNORED)
			{
				char errInfo[256];
				sprintf (errInfo, "DUT%d write barcode flag failed!!", ata_handle+1);
				::AfxMessageBox (errInfo, 0, 0);
				/*Mingxue 20150721 add*/
				testCNF.test_result[E_BARCODE]=E_ATADLL_RESULT_BARCODE_FLAG_FAIL; 				
			}
			else
			{
				testCNF.test_result[E_BARCODE]=E_ATADLL_RESULT_BARCODE_FLAG_OK;
			}		
			m_iUpdateBarcodeUi=1;
			if(*(p_sCommonCFG->stop_flag) != ATA_DLL_STOP_FLAG)
				{
					UpdateTestResultToCtrlList_Sub(E_BARCODE);
				}
			/*Mingxue 20150721 end*/
        }
    }
	
    if (p_sTestItem->b_offMode)
    {
    	if(*(p_sCommonCFG->stop_flag) == ATA_DLL_STOP_FLAG)
    		{
			ATADLL_StopTest(ata_handle);
			return;
    		}
        ATADLL_TestOffCurrent (ata_handle);
		UpdateTestResultToCtrlList_Sub(E_OFFMODE);       
		ATADLL_StopTest(ata_handle);
   }
#ifdef __ATA_CONCURRENT_FLOW__
	else
	{
		if(*(p_sCommonCFG->stop_flag) == ATA_DLL_STOP_FLAG)//if stoped by user update result to ui,else update inTestItemFinishedCallback function
			{
				for (row_index = 0; row_index < testcases_number_bg + testcases_number_fg; row_index ++)
    			{
        			if (row_index < testcases_number_bg)
        			{
           				 test_item = testcases_bg[row_index].item;
       				 }
        			else
        			{
            			test_item = testcases_fg[row_index - testcases_number_bg].item;
        			}
		
        			UpdateTestResultToCtrlList_Sub(test_item);
    			}
			}
		
	}
#else
	for (test_item = E_LCD; test_item < E_TEST_ITEM_COUNT; test_item = (E_ATDLL_TEST_ITEM_COUNT)((int)test_item + 1))
    {
        if (!testFlag[test_item])
        {
            continue;
        }
		
		UpdateTestResultToCtrlList_Sub(test_item);
    }
#endif
	

/*
#ifdef __ATA_CONCURRENT_FLOW__

    for (row_index = 0; row_index < testcases_number_bg + testcases_number_fg; row_index ++)
    {
        if (row_index < testcases_number_bg)
        {
            test_item = testcases_bg[row_index].item;
        }
        else
        {
            test_item = testcases_fg[row_index - testcases_number_bg].item;
        }
		
        UpdateTestResultToCtrlList_Sub(test_item);
    }
#else
	for (test_item = E_LCD; test_item < E_TEST_ITEM_COUNT; test_item = (E_ATDLL_TEST_ITEM_COUNT)((int)test_item + 1))
    {
        if (!testFlag[test_item])
        {
            continue;
        }
		
		UpdateTestResultToCtrlList_Sub(test_item);
    }
#endif*/
#ifndef __ATA_CONCURRENT_FLOW__   //for ATA1.0
for (test_item = E_LCD; test_item < E_TEST_ITEM_COUNT; test_item = (E_ATDLL_TEST_ITEM_COUNT)((int)test_item + 1))
    {
        if (!testFlag[test_item])
        {
            continue;
        }
		
		UpdateTestResultToCtrlList_Sub(test_item);
    }
#endif
}

void CTestObject::UpdateTestFailTimesToCtrlList ()
{
    pXListCtrl->LockWindowUpdate();	// ***** lock window updates while filling list *****
	
    int row_index = 0;
    char tmpChar[64];
	
    for (E_ATDLL_TEST_ITEM_COUNT e_row = E_LCD; e_row < E_TEST_ITEM_COUNT; e_row = (E_ATDLL_TEST_ITEM_COUNT)((int)e_row + 1))
    {
        if (!testFlag[e_row])
        {
            continue;
        }
		
        sprintf (tmpChar, "%d", loop_test_info.fail_times[e_row]);
        CString str = tmpChar;
        pXListCtrl->SetItemText (row_index, RESULT_COLUMN_INDEX, str);
		
        row_index ++;
    }
	
    pXListCtrl->UnlockWindowUpdate();
}



