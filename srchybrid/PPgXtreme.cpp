// PPgXtreme.cpp : implementation file
//

#include "stdafx.h"
#include "emule.h"
#include "PPgXtreme.h"
#include "emuleDlg.h"
//#include "PreferencesDlg.h"
#include "Preferences.h"
#include "OtherFunctions.h"
//#include ".\ppgxtreme.h"
#include "opcodes.h"
#include "UploadQueue.h"
#include "BandWidthControl.h"

#ifdef PRINT_STATISTIC
#include "UploadBandwidthThrottler.h"
#include "ClientCredits.h"
#include "ClientList.h"
#include "TransferWnd.h"
#include "DownloadQueue.h"
#endif

// CPPgXtreme dialog
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CPPgXtreme, CPropertyPage)
CPPgXtreme::CPPgXtreme()
	: CPropertyPage(CPPgXtreme::IDD)
{
}

CPPgXtreme::~CPPgXtreme()
{
}

void CPPgXtreme::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CPPgXtreme, CPropertyPage)
	ON_BN_CLICKED(IDC_13RATIO, OnSettingsChange)	//Xman 1:3 Ratio
	ON_BN_CLICKED(IDC_NAFCFULLCONTROL, OnBnClickedNafcfullcontrol)
	ON_BN_CLICKED(IDC_RETRYCONNECTIONATTEMPTS, OnSettingsChange)	//Xman 
	ON_EN_CHANGE(IDC_MTU_EDIT, OnSettingsChange)
	ON_BN_CLICKED(IDC_USEDOUBLESENDSIZE, OnSettingsChange)
	ON_BN_CLICKED(IDC_MULTIQUEUE, OnSettingsChange) // Maella -One-queue-per-file- (idea bloodymad)
	ON_BN_CLICKED(IDC_OPENMORESLOTS, OnSettingsChange)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_HIGHPRIO_RADIO, OnBnClickedrioRadio)
	ON_BN_CLICKED(IDC_ABOVENORMALPRIO_RADIO, OnBnClickedrioRadio)
	ON_BN_CLICKED(IDC_NORMALPRIO_RADIO, OnBnClickedrioRadio)
	//ON_BN_CLICKED(IDC_HPLINK, OnBnClickedHplink)
	//ON_BN_CLICKED(IDC_FORUMLINK, OnBnClickedForumlink)
	//ON_BN_CLICKED(IDC_VOTELINK, OnBnClickedVotelink)
	ON_BN_CLICKED(IDC_SENDBUFFER1, OnSettingsChange)
	ON_BN_CLICKED(IDC_SENDBUFFER2, OnSettingsChange)
	ON_BN_CLICKED(IDC_SENDBUFFER3, OnSettingsChange)
END_MESSAGE_MAP()

// CPPgXtreme message handlers

BOOL CPPgXtreme::OnInitDialog()
{
	CPropertyPage::OnInitDialog();
	InitWindowStyles(this);

	((CSliderCtrl*)GetDlgItem(IDC_SAMPLERATESLIDER))->SetRange(1,20,true);

	LoadSettings();
	Localize();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CPPgXtreme::LoadSettings(void)
{
		
		CString buffer;
		//Xman 1:3 Ratio
		CheckDlgButton(IDC_13RATIO, thePrefs.Is13Ratio());
		//Xman end

		CheckDlgButton(IDC_NAFCFULLCONTROL, thePrefs.GetNAFCFullControl());

		CheckDlgButton(IDC_RETRYCONNECTIONATTEMPTS, thePrefs.retryconnectionattempts); //Xman 

		switch(thePrefs.GetSendbuffersize())
		{
		case 6000:
			CheckDlgButton(IDC_SENDBUFFER1, TRUE);
			break;
		case 12000:
			CheckDlgButton(IDC_SENDBUFFER3, TRUE);
			break;
		default:
			CheckDlgButton(IDC_SENDBUFFER2, TRUE);
		}

		// Maella -One-queue-per-file- (idea bloodymad)
		CheckDlgButton(IDC_MULTIQUEUE, thePrefs.GetEnableMultiQueue());

		//Xman process prio
		processprio=thePrefs.GetMainProcessPriority();
		switch (thePrefs.GetWindowsVersion())
		{
		case _WINVER_98_:
		case _WINVER_95_:	
		case _WINVER_ME_:
			GetDlgItem(IDC_ABOVENORMALPRIO_RADIO)->EnableWindow(false);
			break;
		default: 
			GetDlgItem(IDC_ABOVENORMALPRIO_RADIO)->EnableWindow(true);
		}
		switch(processprio)
		{
		case NORMAL_PRIORITY_CLASS:
			CheckDlgButton(IDC_NORMALPRIO_RADIO,true);
			break;
		case ABOVE_NORMAL_PRIORITY_CLASS:
			CheckDlgButton(IDC_ABOVENORMALPRIO_RADIO,true);
			break;
		case HIGH_PRIORITY_CLASS:
			CheckDlgButton(IDC_HIGHPRIO_RADIO,true);
			break;
		}
		//Xman end



		CheckDlgButton(IDC_OPENMORESLOTS, thePrefs.m_openmoreslots);

		buffer.Format(_T("%u"), thePrefs.GetMTU());
		GetDlgItem(IDC_MTU_EDIT)->SetWindowText(buffer);

		CheckDlgButton(IDC_USEDOUBLESENDSIZE, thePrefs.usedoublesendsize);

		buffer.Format(GetResString(IDS_AVG_DATARATE_TIME),thePrefs.GetDatarateSamples());
		GetDlgItem(IDC_AVG_DATARATE_TIME)->SetWindowText(buffer);

		((CSliderCtrl*)GetDlgItem(IDC_SAMPLERATESLIDER))->SetPos(thePrefs.GetDatarateSamples());
}

BOOL CPPgXtreme::OnApply()
{
	
	//UpdateData(false);

	//Xman process prio
	thePrefs.SetMainProcessPriority(processprio);
	SetPriorityClass(GetCurrentProcess(), thePrefs.GetMainProcessPriority());
	//Xman end

	if(IsDlgButtonChecked(IDC_SENDBUFFER1))
		thePrefs.SetSendbuffersize(6000);
	else if(IsDlgButtonChecked(IDC_SENDBUFFER2))
		thePrefs.SetSendbuffersize(8192);
	else if(IsDlgButtonChecked(IDC_SENDBUFFER3))
		thePrefs.SetSendbuffersize(12000);
	theApp.uploadqueue->ChangeSendBufferSize(thePrefs.GetSendbuffersize());


	//Xman 1:3 Ratio
	thePrefs.Set13Ratio(IsDlgButtonChecked(IDC_13RATIO)!=0);
	//Xman end

	thePrefs.retryconnectionattempts=IsDlgButtonChecked(IDC_RETRYCONNECTIONATTEMPTS)!=0; //Xman 

	// Maella -One-queue-per-file- (idea bloodymad)
	thePrefs.SetEnableMultiQueue(IsDlgButtonChecked(IDC_MULTIQUEUE)!=0);

	thePrefs.m_openmoreslots=IsDlgButtonChecked(IDC_OPENMORESLOTS)!=0;

	thePrefs.SetNAFCFullControl(IsDlgButtonChecked(IDC_NAFCFULLCONTROL)!=0);
	theApp.pBandWidthControl->SetWasNAFCLastActive(thePrefs.GetNAFCFullControl()); //Xman x4.1

	thePrefs.SetDatarateSamples((uint8)((CSliderCtrl*)GetDlgItem(IDC_SAMPLERATESLIDER))->GetPos());

	{
		CString buffer;
		GetDlgItem(IDC_MTU_EDIT)->GetWindowText(buffer);
		uint16 MTU = (uint16)_tstoi(buffer);
		if(MTU > 1500) MTU = 1500;
		if(MTU < 500) MTU = 500;
		thePrefs.SetMTU(MTU);
	}
	
	thePrefs.usedoublesendsize=IsDlgButtonChecked(IDC_USEDOUBLESENDSIZE)!=0;

	LoadSettings();
	SetModified(FALSE);


	return CPropertyPage::OnApply();
}

void CPPgXtreme::Localize(void)
{	
	if(m_hWnd)
	{
		CString buffer;

		GetDlgItem(IDC_UPLOADMANAGEMENT)->SetWindowText(GetResString(IDS_UPLOADMANAGEMENT_FRAME));

		//Xman amount base ratio:
		GetDlgItem(IDC_13RATIO)->SetWindowText(GetResString(IDS_13RATIO));

		GetDlgItem(IDC_RETRYCONNECTIONATTEMPTS)->SetWindowText(GetResString(IDS_RETRYCONNECTIONATTEMPTS)); //Xman 

		//Xman process prio
		GetDlgItem(IDC_HIGHPRIO_RADIO)->SetWindowText(GetResString(IDS_HIGHPRIOPROCESS));
		GetDlgItem(IDC_ABOVENORMALPRIO_RADIO)->SetWindowText(GetResString(IDS_ABOVENORMALPROCESS));
		GetDlgItem(IDC_NORMALPRIO_RADIO)->SetWindowText(GetResString(IDS_NORMALPROCESS));
		GetDlgItem(IDC_PROCESSPRIO_STATIC)->SetWindowText(GetResString(IDS_PROCESSGROUP));
		//Xman end

		// Maella -One-queue-per-file- (idea bloodymad)
		GetDlgItem(IDC_MULTIQUEUE)->SetWindowText(GetResString(IDS_MULTIQUEUE));

		GetDlgItem(IDC_USEDOUBLESENDSIZE)->SetWindowText(GetResString(IDS_USEDOUBLESENDSIZE));
		GetDlgItem(IDC_STATIC_MTU)->SetWindowText(GetResString(IDS_PPG_MAELLA_MTU_STATIC));
		GetDlgItem(IDC_OPENMORESLOTS)->SetWindowText(GetResString(IDS_OPENMORESLOTS));
		GetDlgItem(IDC_NAFCFULLCONTROL)->SetWindowText(GetResString(IDS_PPG_MAELLA_NAFC_CHECK02));
		buffer.Format(GetResString(IDS_AVG_DATARATE_TIME),thePrefs.GetDatarateSamples());
		GetDlgItem(IDC_AVG_DATARATE_TIME)->SetWindowText(buffer);

		GetDlgItem(IDC_SENDBUFFER_STATIC)->SetWindowText(GetResString(IDS_SENDBUFFER));
	}
}


void CPPgXtreme::OnBnClickedNafcfullcontrol()
{
	OnSettingsChange();
}

void CPPgXtreme::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	SetModified(TRUE);

	int pos=((CSliderCtrl*)GetDlgItem(IDC_SAMPLERATESLIDER))->GetPos();
	CString buffer;
	buffer.Format(GetResString(IDS_AVG_DATARATE_TIME),pos);
	GetDlgItem(IDC_AVG_DATARATE_TIME)->SetWindowText(buffer);

	CPropertyPage::OnHScroll(nSBCode, nPos, pScrollBar);
}

//Xman process prio
void CPPgXtreme::OnBnClickedrioRadio()
{
	SetModified(true);
	if(IsDlgButtonChecked(IDC_NORMALPRIO_RADIO))
		processprio=NORMAL_PRIORITY_CLASS;
	else if(IsDlgButtonChecked(IDC_ABOVENORMALPRIO_RADIO))
		processprio=ABOVE_NORMAL_PRIORITY_CLASS;
	else if(IsDlgButtonChecked(IDC_HIGHPRIO_RADIO))
		processprio=HIGH_PRIORITY_CLASS;
	
}
//Xman end

//Xman Xtreme Links

/*
void CPPgXtreme::OnBnClickedHplink()
{
	ShellExecute(NULL, NULL, MOD_HPLINK, NULL, thePrefs.GetAppDir(), SW_SHOWDEFAULT);
}

void CPPgXtreme::OnBnClickedForumlink()
{
	ShellExecute(NULL, NULL, MOD_FORUMLINK, NULL, thePrefs.GetAppDir(), SW_SHOWDEFAULT);
}

void CPPgXtreme::OnBnClickedVotelink()
{
#ifdef PRINT_STATISTIC
	AddLogLine(false,_T("############################################"));
	AddLogLine(false,_T("##"));
	theApp.uploadBandwidthThrottler->PrintStatistic();
	theApp.clientcredits->PrintStatistic();
	theApp.clientlist->PrintStatistic();
	theApp.pBandWidthControl->PrintStatistic();
	theApp.emuledlg->transferwnd->downloadlistctrl.PrintStatistic();
	theApp.downloadqueue->PrintStatistic();
	AddLogLine(false,_T("############################################"));
#else
	ShellExecute(NULL, NULL, MOD_VOTELINK, NULL, thePrefs.GetAppDir(), SW_SHOWDEFAULT);
#endif
}
*/
