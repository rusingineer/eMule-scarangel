// PpgStulle.cpp : implementation file
//

#include "stdafx.h"
#include "emule.h"
#include "PPgScar.h"
#include "emuledlg.h"
#include "UserMsgs.h"
#include "opcodes.h"
#include "otherfunctions.h"
#include "MuleToolbarCtrl.h" // TBH: minimule - Max
#include "ClientCredits.h" // CreditSystems [EastShare/ MorphXT] - Stulle
#include "log.h"
#include "DownloadQueue.h" // Global Source Limit [Max/Stulle] - Stulle
#include "TransferWnd.h" // CPU/MEM usage [$ick$/Stulle] - Max
#include "WebCache/webcache.h" // WebCache [WC team/MorphXT] - Stulle/Max
#include "XMessageBox.h" // TBH: Backup [TBH/EastShare/MorphXT] - Stulle

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#endif


///////////////////////////////////////////////////////////////////////////////
// CPPgScar dialog

IMPLEMENT_DYNAMIC(CPPgScar, CPropertyPage)

BEGIN_MESSAGE_MAP(CPPgScar, CPropertyPage)
	ON_WM_HSCROLL()
	ON_WM_DESTROY()
	ON_MESSAGE(UM_TREEOPTSCTRL_NOTIFY, OnTreeOptsCtrlNotify)
	ON_NOTIFY(TCN_SELCHANGE, IDC_PPG_SCAR_TAB, OnTabSelectionChange) // Tabbed Preferences [TPT] - Stulle
	// ==> WebCache [WC team/MorphXT] - Stulle/Max
	ON_EN_CHANGE(IDC_webcacheName, OnSettingsChange) 
	ON_EN_CHANGE(IDC_webcachePort, OnSettingsChange)
	ON_EN_CHANGE(IDC_BLOCKS, OnSettingsChange)
	ON_BN_CLICKED(IDC_Activatewebcachedownloads, OnEnChangeActivatewebcachedownloads)
	ON_BN_CLICKED(IDC_DETECTWEBCACHE, OnBnClickedDetectWebCache)
	ON_BN_CLICKED(IDC_EXTRATIMEOUT, OnSettingsChange)
	ON_BN_CLICKED(IDC_LOCALTRAFFIC, OnSettingsChange)
	ON_BN_CLICKED(IDC_PERSISTENT_PROXY_CONNS, OnSettingsChange)
	ON_BN_CLICKED(IDC_UPDATE_WCSETTINGS, OnSettingsChange)
	ON_BN_CLICKED(IDC_ADVANCEDCONTROLS, OnBnClickedAdvancedcontrols)
	ON_BN_CLICKED(IDC_TestProxy, OnBnClickedTestProxy)//JP TMP
	// <== WebCache [WC team/MorphXT] - Stulle/Max
	// ==> TBH: Backup [TBH/EastShare/MorphXT] - Stulle
	ON_BN_CLICKED(IDC_BACKUPNOW, OnBnClickedBackupnow)
	ON_BN_CLICKED(IDC_DAT, OnBnClickedDat)
	ON_BN_CLICKED(IDC_MET, OnBnClickedMet)
	ON_BN_CLICKED(IDC_INI, OnBnClickedIni)
	ON_BN_CLICKED(IDC_PART, OnBnClickedPart)
	ON_BN_CLICKED(IDC_PARTMET, OnBnClickedPartMet)
	ON_BN_CLICKED(IDC_SELECTALL, OnBnClickedSelectall)
	ON_BN_CLICKED(IDC_AUTOBACKUP, OnBnClickedAutobackup)
	ON_BN_CLICKED(IDC_AUTOBACKUP2, OnBnClickedAutobackup2)
	// <== TBH: Backup [TBH/EastShare/MorphXT] - Stulle
	ON_WM_HELPINFO()
END_MESSAGE_MAP()

CPPgScar::CPPgScar()
	: CPropertyPage(CPPgScar::IDD)
	
	, m_ctrlTreeOptions(theApp.m_iDfltImageListColorFlags)
{
	// ==> Tabbed Preferences [TPT] - Stulle
	// Create an icon list for the tab control
	m_imageList.DeleteImageList();
	m_imageList.Create(16,16,theApp.m_iDfltImageListColorFlags|ILC_MASK,0,1);
	m_imageList.SetBkColor(CLR_NONE);
	m_imageList.Add(CTempIconLoader(_T("AAAEMULEAPP")));
	m_imageList.Add(CTempIconLoader(_T("PREF_WEBCACHE")));
	m_imageList.Add(CTempIconLoader(_T("BACKUP")));
//	m_imageList.Add(CTempIconLoader(_T("UPDATE")));
//	m_imageList.Add(CTempIconLoader(_T("SEARCHFILETYPE_PICTURE")));
	// <== Tabbed Preferences [TPT] - Stulle

	// ==> WebCache [WC team/MorphXT] - Stulle/Max
	guardian=false;
	bCreated = false;
	bCreated2 = false;
	showadvanced = false;
	// <== WebCache [WC team/MorphXT] - Stulle/Max

	m_bInitializedTreeOpts = false;
	m_htiPush = NULL; // push files - Stulle
	// ==> push small files [sivka] - Stulle
	m_htiEnablePushSmallFile = NULL;
	m_iPushSmallFiles = 0;
	m_htiPushSmallFileBoost = NULL;
	// <== push small files [sivka] - Stulle
	m_htiEnablePushRareFile = NULL;  // push rare file - Stulle

	// ==> FunnyNick [SiRoB/Stulle] - Stulle
	m_htiFnTag = NULL;
	m_htiFnActive = NULL;
	m_htiFnTagMode = NULL;
	m_htiNoTag = NULL;
	m_htiShortTag = NULL;
	m_htiFullTag = NULL;
	m_htiCustomTag = NULL;
	m_htiFnCustomTag = NULL;
	m_htiFnTagAtEnd = NULL;
	// <== FunnyNick [SiRoB/Stulle] - Stulle

	m_htiConTweaks = NULL;
	// ==> Quick start [TPT] - Max
	m_htiQuickStartGroup = NULL;
	m_htiQuickStart = NULL;
	m_htiQuickStartMaxTime = NULL;
	m_htiQuickStartMaxConnPerFive = NULL;
	m_htiQuickStartMaxConn = NULL;
	m_htiQuickStartMaxConnPerFiveBack = NULL;
	m_htiQuickStartMaxConnBack = NULL;
	m_htiQuickStartAfterIPChange = NULL;
	// <== Quick start [TPT] - Max
//	m_htiReAskFileSrc = NULL; // Timer for ReAsk File Sources - Stulle
	m_htiACC = NULL; // ACC [Max/WiZaRd] - Max
/*
	// ==> Anti Uploader Ban - Stulle
	m_htiAntiUploaderBanLimit = NULL;
	m_htiAntiCase1 = NULL;
	m_htiAntiCase2 = NULL;
	m_htiAntiCase3 = NULL;
	// <== Anti Uploader Ban - Stulle
*/
	// ==> CreditSystems [EastShare/ MorphXT] - Stulle
	m_iCreditSystem = thePrefs.GetCreditSystem();
	m_htiCreditSystem = NULL;
	m_htiOfficialCredit = NULL;
	m_htiLovelaceCredit = NULL;
	m_htiRatioCredit = NULL;
	m_htiPawcioCredit = NULL;
	m_htiESCredit = NULL;
	m_htiSivkaCredit = NULL;
	m_htiSwatCredit = NULL;
	m_htiXmanCredit = NULL;
	// <== CreditSystems [EastShare/ MorphXT] - Stulle

	m_htiDisplay = NULL;
	// ==> CPU/MEM usage [$ick$/Stulle] - Max
	m_htiSysInfoGroup = NULL;
	m_htiSysInfo = NULL;
	m_htiSysInfoGlobal = NULL;
	// <== CPU/MEM usage [$ick$/Stulle] - Max
	m_htiShowSrcOnTitle = NULL; // Show sources on title - Stulle
//	m_htiPsFilesRed = NULL; // draw PS files red - Stulle
	m_htiFriendsBlue = NULL; // draw friends blue - Stulle
	m_htiShowGlobalHL = NULL; // show global HL - Stulle
	m_htiShowFileHLconst = NULL; // show HL per file constantaniously - Stulle
	m_htiShowInMSN7 = NULL; // Show in MSN7 [TPT] - Stulle
	m_htiQueueProgressBar = NULL; // Client queue progress bar [Commander] - Stulle
//	m_htiTrayComplete = NULL; // Completed in Tray - Stulle
	m_htiShowClientPercentage = NULL; // Show Client Percentage [Commander/MorphXT] - Mondgott

	// ==> file settings - Stulle
	m_htiFileDefaults = NULL;
	m_htiAutoNNS = NULL;
	m_htiAutoNNSTimer = NULL;
	m_htiAutoNNSLimit = NULL;
	m_htiAutoFQS = NULL;
	m_htiAutoFQSTimer = NULL;
	m_htiAutoFQSLimit = NULL;
	m_htiAutoQRS = NULL;
	m_htiAutoQRSTimer = NULL;
	m_htiAutoQRSMax = NULL;
	m_htiAutoQRSLimit = NULL;
	m_htiAutoQRSWay = NULL;
	m_htiHQRXman = NULL;
	m_htiHQRSivka = NULL;
	m_iHQRXmanDefault = (thePrefs.GetHQRXmanDefault()) ? 1 : 0;
	// <== file settings - Stulle

	// ==> TBH: minimule - Max
	m_htiMMGroup = NULL;
	m_htiShowMM = NULL;
	m_htiMMLives = NULL;
	m_htiMMUpdateTime = NULL;
	m_htiMMTrans = NULL;
	m_htiMMCompl = NULL;
	m_htiMMOpen = NULL;
	// <== TBH: minimule - Max

	m_htiMisc = NULL;
	m_htiSUQWT = NULL; // SUQWT [Moonlight/EastShare/ MorphXT] - Stulle
/*	// ==> Spread Credits Slot - Stulle
	m_htiSpreadCreditsSlot = NULL;
	m_htiSpreadCreditsSlotCounter = NULL;
	// <== Spread Credits Slot - Stulle
*/	// ==> Global Source Limit [Max/Stulle] - Stulle
	m_htiGlobalHlGroup = NULL;
	m_htiGlobalHL = NULL;
	m_htiGlobalHlLimit = NULL;
	m_htiGlobalHlAll = NULL;
	m_htiGlobalHlDefault = NULL;
	// <== Global Source Limit [Max/Stulle] - Stulle
	m_htiStartupSound = NULL; // Startupsound [Commander] - mav744
}

CPPgScar::~CPPgScar()
{
}

void CPPgScar::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PPG_SCAR_TAB, m_tabCtr); // Tabbed Preferences [TPT] - Stulle
	DDX_Control(pDX, IDC_SCAR_OPTS, m_ctrlTreeOptions);
	if (!m_bInitializedTreeOpts)
	{
		int iImgPush = 8;
		int iImgFunnyNick = 8;
		int iImgConTweaks = 8;
		int iImgQuickstart = 8;
		int iImgCS = 8; // CreditSystems [EastShare/ MorphXT] - Stulle
		int iImgDisplay = 8;
		int iImgSysInfo = 8;
		int iImgDropDefaults = 8;
		int iImgMinimule = 8;
		int iImgMisc = 8;
		int iImgGlobal = 8;

		CImageList* piml = m_ctrlTreeOptions.GetImageList(TVSIL_NORMAL);
		if (piml){
			iImgPush = piml->Add(CTempIconLoader(_T("SPEED")));
			iImgFunnyNick = piml->Add(CTempIconLoader(_T("FUNNYNICK")));
			iImgConTweaks =  piml->Add(CTempIconLoader(_T("CONNECTION")));
			iImgQuickstart = piml->Add(CTempIconLoader(_T("QUICKSTART"))); // Thx to the eF-Mod team for the icon
			iImgCS = piml->Add(CTempIconLoader(_T("STATSCLIENTS"))); // CreditSystems [EastShare/ MorphXT] - Stulle
			iImgDisplay = piml->Add(CTempIconLoader(_T("DISPLAY")));
			iImgSysInfo = piml->Add(CTempIconLoader(_T("SYSINFO")));
			iImgDropDefaults = piml->Add(CTempIconLoader(_T("DROPDEFAULTS")));
			iImgMinimule = piml->Add(CTempIconLoader(_T("MINIMULE")));
			iImgMisc = piml->Add(CTempIconLoader(_T("SRCUNKNOWN")));
			iImgGlobal = piml->Add(CTempIconLoader(_T("SEARCHMETHOD_GLOBAL")));
		}
		
		CString Buffer;
		m_htiPush = m_ctrlTreeOptions.InsertGroup(GetResString(IDS_PUSH), iImgPush, TVI_ROOT); // push files - Stulle
		// ==> push small files [sivka]  - Stulle
		m_htiEnablePushSmallFile = m_ctrlTreeOptions.InsertCheckBox(GetResString(IDS_PUSH_SMALL), m_htiPush, m_bEnablePushSmallFile);
		m_htiPushSmallFileBoost = m_ctrlTreeOptions.InsertItem(GetResString(IDS_PUSH_SMALL_BOOST), TREEOPTSCTRLIMG_EDIT, TREEOPTSCTRLIMG_EDIT, m_htiPush);
		m_ctrlTreeOptions.AddEditBox(m_htiPushSmallFileBoost, RUNTIME_CLASS(CNumTreeOptionsEdit));
		// <== push small files [sivka]  - Stulle
		m_htiEnablePushRareFile = m_ctrlTreeOptions.InsertCheckBox(GetResString(IDS_PUSH_RARE), m_htiPush, m_bEnablePushRareFile); // push rare file - Stulle

		// ==> FunnyNick [SiRoB/Stulle] - Stulle
		m_htiFnTag = m_ctrlTreeOptions.InsertGroup(GetResString(IDS_FUNNYNICK), iImgFunnyNick, TVI_ROOT);
		m_htiFnActive = m_ctrlTreeOptions.InsertCheckBox(GetResString(IDS_ENABLE), m_htiFnTag, m_bFnActive);
		m_htiFnTagMode = m_ctrlTreeOptions.InsertGroup(GetResString(IDS_FN_TAG), iImgFunnyNick, m_htiFnTag);
		m_htiNoTag = m_ctrlTreeOptions.InsertRadioButton(GetResString(IDS_NO_TAG), m_htiFnTagMode, m_iFnTag == 0);
		m_htiShortTag = m_ctrlTreeOptions.InsertRadioButton(GetResString(IDS_SHORT_TAG), m_htiFnTagMode, m_iFnTag == 1);
		m_htiFullTag = m_ctrlTreeOptions.InsertRadioButton(GetResString(IDS_FULL_TAG), m_htiFnTagMode, m_iFnTag == 2);
		m_htiCustomTag = m_ctrlTreeOptions.InsertRadioButton(GetResString(IDS_CUSTOM_TAG),m_htiFnTagMode,m_iFnTag == 3);
		m_htiFnCustomTag = m_ctrlTreeOptions.InsertItem(GetResString(IDS_SET_CUSTOM_TAG), TREEOPTSCTRLIMG_EDIT, TREEOPTSCTRLIMG_EDIT, m_htiCustomTag);
		m_ctrlTreeOptions.AddEditBox(m_htiFnCustomTag, RUNTIME_CLASS(CTreeOptionsEdit));
		if(m_bFnActive) m_ctrlTreeOptions.Expand(m_htiCustomTag, TVE_EXPAND);
		m_ctrlTreeOptions.Expand(m_htiFnTagMode, TVE_EXPAND);
		m_htiFnTagAtEnd = m_ctrlTreeOptions.InsertCheckBox(GetResString(IDS_FN_TAG_AT_END), m_htiFnTag, m_bFnTagAtEnd);
		// <== FunnyNick [SiRoB/Stulle] - Stulle

		m_htiConTweaks = m_ctrlTreeOptions.InsertGroup(GetResString(IDS_CON_TWEAKS), iImgConTweaks, TVI_ROOT);
		// ==> Quick start [TPT] - Max
		m_htiQuickStartGroup = m_ctrlTreeOptions.InsertGroup(GetResString(IDS_QUICK_START_GROUP), iImgQuickstart, m_htiConTweaks);
		m_htiQuickStart = m_ctrlTreeOptions.InsertCheckBox(GetResString(IDS_QUICK_START), m_htiQuickStartGroup, m_bQuickStart);
		m_htiQuickStartMaxTime = m_ctrlTreeOptions.InsertItem(GetResString(IDS_QUICK_START_MAX_TIME), TREEOPTSCTRLIMG_EDIT, TREEOPTSCTRLIMG_EDIT, m_htiQuickStart);
		m_ctrlTreeOptions.AddEditBox(m_htiQuickStartMaxTime, RUNTIME_CLASS(CNumTreeOptionsEdit));
		m_htiQuickStartMaxConnPerFive = m_ctrlTreeOptions.InsertItem(GetResString(IDS_QUICK_START_MAX_CONN_PER_FIVE), TREEOPTSCTRLIMG_EDIT, TREEOPTSCTRLIMG_EDIT, m_htiQuickStart);
		m_ctrlTreeOptions.AddEditBox(m_htiQuickStartMaxConnPerFive, RUNTIME_CLASS(CNumTreeOptionsEdit));
		m_htiQuickStartMaxConn = m_ctrlTreeOptions.InsertItem(GetResString(IDS_QUICK_START_MAX_CONN), TREEOPTSCTRLIMG_EDIT, TREEOPTSCTRLIMG_EDIT, m_htiQuickStart);
		m_ctrlTreeOptions.AddEditBox(m_htiQuickStartMaxConn, RUNTIME_CLASS(CNumTreeOptionsEdit));
		m_htiQuickStartMaxConnPerFiveBack = m_ctrlTreeOptions.InsertItem(GetResString(IDS_QUICK_START_MAX_CONN_PER_FIVE_BACK), TREEOPTSCTRLIMG_EDIT, TREEOPTSCTRLIMG_EDIT, m_htiQuickStart);
		m_ctrlTreeOptions.AddEditBox(m_htiQuickStartMaxConnPerFiveBack, RUNTIME_CLASS(CNumTreeOptionsEdit));
		m_htiQuickStartMaxConnBack = m_ctrlTreeOptions.InsertItem(GetResString(IDS_QUICK_START_MAX_CONN_BACK), TREEOPTSCTRLIMG_EDIT, TREEOPTSCTRLIMG_EDIT, m_htiQuickStart);
		m_ctrlTreeOptions.AddEditBox(m_htiQuickStartMaxConnBack, RUNTIME_CLASS(CNumTreeOptionsEdit));
		m_ctrlTreeOptions.Expand(m_htiQuickStart, TVE_EXPAND);
		m_htiQuickStartAfterIPChange = m_ctrlTreeOptions.InsertCheckBox(GetResString(IDS_QUICK_START_AFTER_IP_CHANGE), m_htiQuickStartGroup, m_bQuickStartAfterIPChange);
		// <== Quick start [TPT] - Max
/*		// ==> Timer for ReAsk File Sources - Stulle
		m_htiReAskFileSrc = m_ctrlTreeOptions.InsertItem(GetResString(IDS_REASK_FILE_SRC), TREEOPTSCTRLIMG_EDIT, TREEOPTSCTRLIMG_EDIT, m_htiConTweaks);
		m_ctrlTreeOptions.AddEditBox(m_htiReAskFileSrc, RUNTIME_CLASS(CNumTreeOptionsEdit));
*/		// <== Timer for ReAsk File Sources - Stulle
		m_htiACC = m_ctrlTreeOptions.InsertCheckBox(GetResString(IDS_ACC), m_htiConTweaks, m_bACC); // ACC [Max/WiZaRd] - Max
/*
		// ==> Anti Uploader Ban - Stulle
		m_htiAntiUploaderBanLimit = m_ctrlTreeOptions.InsertItem(GetResString(IDS_UNBAN_UPLOADER), TREEOPTSCTRLIMG_EDIT, TREEOPTSCTRLIMG_EDIT, TVI_ROOT);
		m_ctrlTreeOptions.AddEditBox(m_htiAntiUploaderBanLimit, RUNTIME_CLASS(CNumTreeOptionsEdit));
		m_htiAntiCase1 = m_ctrlTreeOptions.InsertRadioButton(GetResString(IDS_ANTI_CASE_1), m_htiAntiUploaderBanLimit, m_iAntiUploaderBanCase == 0);
		m_htiAntiCase2 = m_ctrlTreeOptions.InsertRadioButton(GetResString(IDS_ANTI_CASE_2), m_htiAntiUploaderBanLimit, m_iAntiUploaderBanCase == 1);
		m_htiAntiCase3 = m_ctrlTreeOptions.InsertRadioButton(GetResString(IDS_ANTI_CASE_3), m_htiAntiUploaderBanLimit, m_iAntiUploaderBanCase == 2);
		// <== Anti Uploader Ban - Stulle
*/
		// ==> CreditSystems [EastShare/ MorphXT] - Stulle
		m_htiCreditSystem = m_ctrlTreeOptions.InsertGroup(GetResString(IDS_CREDIT_SYSTEM), iImgCS, TVI_ROOT);
		m_htiOfficialCredit = m_ctrlTreeOptions.InsertRadioButton(GetResString(IDS_OFFICIAL_CREDIT), m_htiCreditSystem, m_iCreditSystem == 0);
		m_htiLovelaceCredit = m_ctrlTreeOptions.InsertRadioButton(GetResString(IDS_LOVELACE_CREDIT), m_htiCreditSystem, m_iCreditSystem == 1);
		m_htiRatioCredit = m_ctrlTreeOptions.InsertRadioButton(GetResString(IDS_RATIO_CREDIT), m_htiCreditSystem, m_iCreditSystem == 2);
		m_htiPawcioCredit = m_ctrlTreeOptions.InsertRadioButton(GetResString(IDS_PAWCIO_CREDIT), m_htiCreditSystem, m_iCreditSystem == 3);
		m_htiESCredit = m_ctrlTreeOptions.InsertRadioButton(GetResString(IDS_EASTSHARE_CREDIT), m_htiCreditSystem, m_iCreditSystem == 4);
		m_htiSivkaCredit = m_ctrlTreeOptions.InsertRadioButton(GetResString(IDS_SIVKA_CREDIT), m_htiCreditSystem, m_iCreditSystem == 5);
		m_htiSwatCredit = m_ctrlTreeOptions.InsertRadioButton(GetResString(IDS_SWAT_CREDIT), m_htiCreditSystem, m_iCreditSystem == 6);
		m_htiXmanCredit = m_ctrlTreeOptions.InsertRadioButton(GetResString(IDS_XMAN_CREDIT), m_htiCreditSystem, m_iCreditSystem == 7);
		// <== CreditSystems [EastShare/ MorphXT] - Stulle

		m_htiDisplay = m_ctrlTreeOptions.InsertGroup(GetResString(IDS_PW_DISPLAY), iImgDisplay, TVI_ROOT);
		// ==> CPU/MEM usage [$ick$/Stulle] - Max
		m_htiSysInfoGroup = m_ctrlTreeOptions.InsertGroup(GetResString(IDS_SYS_INFO_GROUP), iImgSysInfo, m_htiDisplay);
		m_htiSysInfo = m_ctrlTreeOptions.InsertCheckBox(GetResString(IDS_ENABLE), m_htiSysInfoGroup, m_bSysInfo);
		m_htiSysInfoGlobal = m_ctrlTreeOptions.InsertCheckBox(GetResString(IDS_SYS_INFO_GLOBAL), m_htiSysInfoGroup, m_bSysInfoGlobal);
		// <== CPU/MEM usage [$ick$/Stulle] - Max
		m_htiShowSrcOnTitle = m_ctrlTreeOptions.InsertCheckBox(GetResString(IDS_SHOWSRCONTITLE), m_htiDisplay, showSrcInTitle); // Show sources on title - Stulle
//		m_htiPsFilesRed = m_ctrlTreeOptions.InsertCheckBox(GetResString(IDS_PS_FILES_RED), m_htiDisplay, m_bPsFilesRed); // draw PS files red - Stulle
		m_htiFriendsBlue = m_ctrlTreeOptions.InsertCheckBox(GetResString(IDS_FRIENDS_BLUE), m_htiDisplay, m_bFriendsBlue); // draw friends blue - Stulle
		m_htiShowGlobalHL = m_ctrlTreeOptions.InsertCheckBox(GetResString(IDS_SHOW_GLOBAL_HL), m_htiDisplay, m_bShowGlobalHL); // show global HL - Stulle
		m_htiShowFileHLconst = m_ctrlTreeOptions.InsertCheckBox(GetResString(IDS_SHOW_FILE_HL_CONST), m_htiDisplay, m_bShowFileHLconst); // show HL per file constantaniously - Stulle
		m_htiShowInMSN7 = m_ctrlTreeOptions.InsertCheckBox(GetResString(IDS_SHOWINMSN7), m_htiDisplay, m_bShowInMSN7); // Show in MSN7 [TPT] - Stulle
		m_htiQueueProgressBar = m_ctrlTreeOptions.InsertCheckBox(GetResString(IDS_CLIENTQUEUEPROGRESSBAR), m_htiDisplay, m_bQueueProgressBar); // Client queue progress bar [Commander] - Stulle
//		m_htiTrayComplete = m_ctrlTreeOptions.InsertCheckBox(GetResString(IDS_TRAY_COMPLETE), m_htiDisplay, m_bTrayComplete); // Completed in Tray - Stulle
		m_htiShowClientPercentage = m_ctrlTreeOptions.InsertCheckBox(GetResString(IDS_CLIENTPERCENTAGE), m_htiDisplay, m_bShowClientPercentage); // Show Client Percentage [Commander/MorphXT] - Mondgott

		// ==> file settings - Stulle
		m_htiFileDefaults = m_ctrlTreeOptions.InsertGroup(GetResString(IDS_FILE_DEFAULTS), iImgDropDefaults, TVI_ROOT);
		m_htiAutoNNS = m_ctrlTreeOptions.InsertCheckBox(GetResString(IDS_AUTO_NNS), m_htiFileDefaults, m_bEnableAutoDropNNSDefault);
		m_htiAutoNNSTimer = m_ctrlTreeOptions.InsertItem(GetResString(IDS_NNS_TIMERLABEL), TREEOPTSCTRLIMG_EDIT, TREEOPTSCTRLIMG_EDIT, m_htiAutoNNS);
		m_ctrlTreeOptions.AddEditBox(m_htiAutoNNSTimer, RUNTIME_CLASS(CNumTreeOptionsEdit));
		m_htiAutoNNSLimit = m_ctrlTreeOptions.InsertItem(GetResString(IDS_REMOVENNSLIMITLABEL), TREEOPTSCTRLIMG_EDIT, TREEOPTSCTRLIMG_EDIT, m_htiAutoNNS);
		m_ctrlTreeOptions.AddEditBox(m_htiAutoNNSLimit, RUNTIME_CLASS(CNumTreeOptionsEdit));
		m_ctrlTreeOptions.Expand(m_htiAutoNNS, TVE_EXPAND);
		m_htiAutoFQS = m_ctrlTreeOptions.InsertCheckBox(GetResString(IDS_AUTO_FQS), m_htiFileDefaults, m_bEnableAutoDropFQSDefault);
		m_htiAutoFQSTimer = m_ctrlTreeOptions.InsertItem(GetResString(IDS_FQS_TIMERLABEL), TREEOPTSCTRLIMG_EDIT, TREEOPTSCTRLIMG_EDIT, m_htiAutoFQS);
		m_ctrlTreeOptions.AddEditBox(m_htiAutoFQSTimer, RUNTIME_CLASS(CNumTreeOptionsEdit));
		m_htiAutoFQSLimit = m_ctrlTreeOptions.InsertItem(GetResString(IDS_REMOVEFQSLIMITLABEL), TREEOPTSCTRLIMG_EDIT, TREEOPTSCTRLIMG_EDIT, m_htiAutoFQS);
		m_ctrlTreeOptions.AddEditBox(m_htiAutoFQSLimit, RUNTIME_CLASS(CNumTreeOptionsEdit));
		m_ctrlTreeOptions.Expand(m_htiAutoFQS, TVE_EXPAND);
		m_htiAutoQRS = m_ctrlTreeOptions.InsertCheckBox(GetResString(IDS_AUTO_QRS), m_htiFileDefaults, m_bEnableAutoDropQRSDefault);
		m_htiAutoQRSTimer = m_ctrlTreeOptions.InsertItem(GetResString(IDS_HQRS_TIMERLABEL), TREEOPTSCTRLIMG_EDIT, TREEOPTSCTRLIMG_EDIT, m_htiAutoQRS);
		m_ctrlTreeOptions.AddEditBox(m_htiAutoQRSTimer, RUNTIME_CLASS(CNumTreeOptionsEdit));
		m_htiAutoQRSLimit = m_ctrlTreeOptions.InsertItem(GetResString(IDS_REMOVEQRSLIMITLABEL), TREEOPTSCTRLIMG_EDIT, TREEOPTSCTRLIMG_EDIT, m_htiAutoQRS);
		m_ctrlTreeOptions.AddEditBox(m_htiAutoQRSLimit, RUNTIME_CLASS(CNumTreeOptionsEdit));
		m_htiAutoQRSWay = m_ctrlTreeOptions.InsertGroup(GetResString(IDS_HQR_WAY), iImgDropDefaults, m_htiAutoQRS);
		m_htiHQRSivka = m_ctrlTreeOptions.InsertRadioButton(GetResString(IDS_SIVKA_DROPPING), m_htiAutoQRSWay, m_iHQRXmanDefault == 0);
		m_htiAutoQRSMax = m_ctrlTreeOptions.InsertItem(GetResString(IDS_REMOVEQRSLABEL), TREEOPTSCTRLIMG_EDIT, TREEOPTSCTRLIMG_EDIT, m_htiHQRSivka);
		m_ctrlTreeOptions.AddEditBox(m_htiAutoQRSMax, RUNTIME_CLASS(CNumTreeOptionsEdit));
		m_ctrlTreeOptions.Expand(m_htiHQRSivka, TVE_EXPAND);
		m_htiHQRXman = m_ctrlTreeOptions.InsertRadioButton(GetResString(IDS_XMAN_DROPPING), m_htiAutoQRSWay, m_iHQRXmanDefault == 1);
		m_ctrlTreeOptions.Expand(m_htiAutoQRSWay, TVE_EXPAND);
		m_ctrlTreeOptions.Expand(m_htiAutoQRS, TVE_EXPAND);
		// <== file settings - Stulle

		// ==> TBH: minimule - Max
		m_htiMMGroup = m_ctrlTreeOptions.InsertGroup(_T("TBH Mini-Mule"), iImgMinimule, TVI_ROOT);
		m_htiShowMM = m_ctrlTreeOptions.InsertCheckBox(GetResString(IDS_MM_SHOW), m_htiMMGroup, m_bShowMM);
		m_htiMMLives = m_ctrlTreeOptions.InsertCheckBox(GetResString(IDS_MM_LIVES), m_htiMMGroup, m_bMMLives);
		m_htiMMUpdateTime = m_ctrlTreeOptions.InsertItem(GetResString(IDS_MM_UPDATE_TIME), TREEOPTSCTRLIMG_EDIT, TREEOPTSCTRLIMG_EDIT, m_htiMMGroup);
		m_ctrlTreeOptions.AddEditBox(m_htiMMUpdateTime, RUNTIME_CLASS(CNumTreeOptionsEdit));
		m_htiMMTrans = m_ctrlTreeOptions.InsertItem(GetResString(IDS_MM_TRANS), TREEOPTSCTRLIMG_EDIT, TREEOPTSCTRLIMG_EDIT, m_htiMMGroup);
		m_ctrlTreeOptions.AddEditBox(m_htiMMTrans, RUNTIME_CLASS(CNumTreeOptionsEdit));
		m_htiMMCompl = m_ctrlTreeOptions.InsertCheckBox(GetResString(IDS_MM_COMPL), m_htiMMGroup, m_bMMCompl);
		m_htiMMOpen = m_ctrlTreeOptions.InsertCheckBox(GetResString(IDS_MM_OPEN), m_htiMMGroup, m_bMMOpen);
		// <== TBH: minimule - Max

		m_htiMisc = m_ctrlTreeOptions.InsertGroup(GetResString(IDS_MISC), iImgMisc, TVI_ROOT);
		m_htiSUQWT = m_ctrlTreeOptions.InsertCheckBox(GetResString(IDS_SUQWT), m_htiMisc, m_bSUQWT); // SUQWT [Moonlight/EastShare/ MorphXT] - Stulle
/*		// ==> Spread Credits Slot - Stulle
		m_htiSpreadCreditsSlot = m_ctrlTreeOptions.InsertCheckBox(GetResString(IDS_SPREAD_CREDITS_SLOT), m_htiMisc, m_bSpreadCreditsSlot);
		m_htiSpreadCreditsSlotCounter = m_ctrlTreeOptions.InsertItem(GetResString(IDS_SPREAD_CREDITS_SLOT_COUNTER), TREEOPTSCTRLIMG_EDIT, TREEOPTSCTRLIMG_EDIT, m_htiMisc);
		m_ctrlTreeOptions.AddEditBox(m_htiSpreadCreditsSlotCounter, RUNTIME_CLASS(CNumTreeOptionsEdit));
		// <== Spread Credits Slot - Stulle
*/		// ==> Global Source Limit [Max/Stulle] - Stulle
		m_htiGlobalHlGroup = m_ctrlTreeOptions.InsertGroup(GetResString(IDS_GLOBAL_HL), iImgGlobal, m_htiMisc);
		m_htiGlobalHL = m_ctrlTreeOptions.InsertCheckBox(GetResString(IDS_ENABLE), m_htiGlobalHlGroup, m_bGlobalHL);
		m_htiGlobalHlLimit = m_ctrlTreeOptions.InsertItem(GetResString(IDS_GLOBAL_HL_LIMIT), TREEOPTSCTRLIMG_EDIT, TREEOPTSCTRLIMG_EDIT, m_htiGlobalHlGroup);
		m_ctrlTreeOptions.AddEditBox(m_htiGlobalHlLimit, RUNTIME_CLASS(CNumTreeOptionsEdit));
		m_htiGlobalHlAll = m_ctrlTreeOptions.InsertCheckBox(GetResString(IDS_GLOBAL_HL_ALL), m_htiGlobalHlGroup, m_bGlobalHlAll);
		m_htiGlobalHlDefault = m_ctrlTreeOptions.InsertCheckBox(GetResString(IDS_GLOBAL_HL_DEFAULT), m_htiGlobalHlGroup, m_bGlobalHlDefault);
		m_ctrlTreeOptions.Expand(m_htiGlobalHlGroup, TVE_EXPAND);
		// <== Global Source Limit [Max/Stulle] - Stulle
		m_htiStartupSound = m_ctrlTreeOptions.InsertCheckBox(GetResString(IDS_STARTUPSOUND), m_htiMisc, m_bStartupSound); // Startupsound [Commander] - mav744

		m_ctrlTreeOptions.SendMessage(WM_VSCROLL, SB_TOP);
		m_bInitializedTreeOpts = true;
	}
	// ==> push small files [sivka] - Stulle
    DDX_TreeCheck(pDX, IDC_SCAR_OPTS, m_htiEnablePushSmallFile, m_bEnablePushSmallFile);
	DDX_TreeEdit(pDX, IDC_SCAR_OPTS, m_htiPushSmallFileBoost, m_iPushSmallFileBoost);
	DDV_MinMaxInt(pDX, m_iPushSmallFileBoost, 1, 65536);
	// <== push small files [sivka] - Stulle
	DDX_TreeCheck(pDX, IDC_SCAR_OPTS, m_htiEnablePushRareFile, m_bEnablePushRareFile); // push rare file - Stulle

	// ==> FunnyNick [SiRoB/Stulle] - Stulle
	if(m_htiFnActive)	DDX_TreeCheck(pDX, IDC_SCAR_OPTS, m_htiFnActive, m_bFnActive);
	if(m_htiFnTagMode)	DDX_TreeRadio(pDX, IDC_SCAR_OPTS, m_htiFnTagMode, (int &)m_iFnTag);
	if(m_htiFnCustomTag)DDX_TreeEdit(pDX, IDC_SCAR_OPTS, m_htiFnCustomTag, m_sFnCustomTag);
	if(m_htiFnTagAtEnd)	DDX_TreeCheck(pDX, IDC_SCAR_OPTS, m_htiFnTagAtEnd, m_bFnTagAtEnd);
	// <== FunnyNick [SiRoB/Stulle] - Stulle
	
	// ==> Quick start [TPT] - Max
	DDX_TreeCheck(pDX, IDC_SCAR_OPTS, m_htiQuickStart, m_bQuickStart);
	DDX_TreeEdit(pDX, IDC_SCAR_OPTS, m_htiQuickStartMaxTime, m_iQuickStartMaxTime);
	DDV_MinMaxInt(pDX, m_iQuickStartMaxTime, 8, 18);
	DDX_TreeEdit(pDX, IDC_SCAR_OPTS, m_htiQuickStartMaxConnPerFive, m_iQuickStartMaxConnPerFive);
	DDV_MinMaxInt(pDX, m_iQuickStartMaxConnPerFive, 5, 200);
	DDX_TreeEdit(pDX, IDC_SCAR_OPTS, m_htiQuickStartMaxConn, m_iQuickStartMaxConn);
	DDV_MinMaxInt(pDX, m_iQuickStartMaxConn, 200, 2000);
	DDX_TreeEdit(pDX, IDC_SCAR_OPTS, m_htiQuickStartMaxConnPerFiveBack, m_iQuickStartMaxConnPerFiveBack);
	DDV_MinMaxInt(pDX, m_iQuickStartMaxConnPerFiveBack, 1, INT_MAX);
	DDX_TreeEdit(pDX, IDC_SCAR_OPTS, m_htiQuickStartMaxConnBack, m_iQuickStartMaxConnBack);
	DDV_MinMaxInt(pDX, m_iQuickStartMaxConnBack, 1, INT_MAX);
	if(m_htiQuickStartAfterIPChange) DDX_TreeCheck(pDX, IDC_SCAR_OPTS, m_htiQuickStartAfterIPChange, m_bQuickStartAfterIPChange);
	// <== Quick start [TPT] - Max
/*	// ==> Timer for ReAsk File Sources - Stulle
	DDX_TreeEdit(pDX, IDC_SCAR_OPTS, m_htiReAskFileSrc, m_iReAskFileSrc);
	DDV_MinMaxInt(pDX, m_iReAskFileSrc, 29, 55);
*/	// <== Timer for ReAsk File Sources - Stulle
	DDX_TreeCheck(pDX, IDC_SCAR_OPTS, m_htiACC, m_bACC); // ACC [Max/WiZaRd] - Max
/*
	// ==> Anti Uploader Ban - Stulle
	DDX_TreeEdit(pDX, IDC_SCAR_OPTS, m_htiAntiUploaderBanLimit, m_iAntiUploaderBanLimit);
	DDV_MinMaxInt(pDX, m_iAntiUploaderBanLimit, 0, 20);
	DDX_TreeRadio(pDX, IDC_SCAR_OPTS, m_htiAntiUploaderBanLimit, (int &)m_iAntiUploaderBanCase);
	// <== Anti Uploader Ban - Stulle
*/
	DDX_TreeRadio(pDX, IDC_SCAR_OPTS, m_htiCreditSystem, (int &)m_iCreditSystem); // CreditSystems [EastShare/ MorphXT] - Stulle

	// ==> CPU/MEM usage [$ick$/Stulle] - Max
	DDX_TreeCheck(pDX, IDC_SCAR_OPTS, m_htiSysInfo, m_bSysInfo);
	if(m_htiSysInfoGlobal) DDX_TreeCheck(pDX, IDC_SCAR_OPTS, m_htiSysInfoGlobal, m_bSysInfoGlobal);
	// <== CPU/MEM usage [$ick$/Stulle] - Max
	DDX_TreeCheck(pDX, IDC_SCAR_OPTS, m_htiShowSrcOnTitle, showSrcInTitle); // Show sources on title - Stulle
//	DDX_TreeCheck(pDX, IDC_SCAR_OPTS, m_htiPsFilesRed, m_bPsFilesRed); // draw PS files red - Stulle
	DDX_TreeCheck(pDX, IDC_SCAR_OPTS, m_htiFriendsBlue, m_bFriendsBlue); // draw friends blue - Stulle
	DDX_TreeCheck(pDX, IDC_SCAR_OPTS, m_htiShowGlobalHL, m_bShowGlobalHL); // show global HL - Stulle
	DDX_TreeCheck(pDX, IDC_SCAR_OPTS, m_htiShowFileHLconst, m_bShowFileHLconst); // show HL per file constantaniously - Stulle
	DDX_TreeCheck(pDX, IDC_SCAR_OPTS, m_htiShowInMSN7, m_bShowInMSN7); // Show in MSN7 [TPT] - Stulle
	DDX_TreeCheck(pDX, IDC_SCAR_OPTS, m_htiQueueProgressBar, m_bQueueProgressBar); // Client queue progress bar [Commander] - Stulle
//	DDX_TreeCheck(pDX, IDC_SCAR_OPTS, m_htiTrayComplete, m_bTrayComplete); // Completed in Tray - Stulle
	DDX_TreeCheck(pDX, IDC_SCAR_OPTS, m_htiShowClientPercentage, m_bShowClientPercentage); // Show Client Percentage [Commander/MorphXT] - Mondgott

	// ==> file settings - Stulle
	DDX_TreeCheck(pDX, IDC_SCAR_OPTS, m_htiAutoNNS, m_bEnableAutoDropNNSDefault);
	DDX_TreeEdit(pDX, IDC_SCAR_OPTS, m_htiAutoNNSTimer, m_iAutoNNS_TimerDefault);
	DDV_MinMaxInt(pDX, m_iAutoNNS_TimerDefault, 0, 60);
	DDX_TreeEdit(pDX, IDC_SCAR_OPTS, m_htiAutoNNSLimit, m_iMaxRemoveNNSLimitDefault);
	DDV_MinMaxInt(pDX, m_iMaxRemoveNNSLimitDefault, 50, 100);
	DDX_TreeCheck(pDX, IDC_SCAR_OPTS, m_htiAutoFQS, m_bEnableAutoDropFQSDefault);
	DDX_TreeEdit(pDX, IDC_SCAR_OPTS, m_htiAutoFQSTimer, m_iAutoFQS_TimerDefault);
	DDV_MinMaxInt(pDX, m_iAutoFQS_TimerDefault, 0, 60);
	DDX_TreeEdit(pDX, IDC_SCAR_OPTS, m_htiAutoFQSLimit, m_iMaxRemoveFQSLimitDefault);
	DDV_MinMaxInt(pDX, m_iMaxRemoveFQSLimitDefault, 50, 100);
	DDX_TreeCheck(pDX, IDC_SCAR_OPTS, m_htiAutoQRS, m_bEnableAutoDropQRSDefault);
	DDX_TreeEdit(pDX, IDC_SCAR_OPTS, m_htiAutoQRSTimer, m_iAutoHQRS_TimerDefault);
	DDV_MinMaxInt(pDX, m_iAutoHQRS_TimerDefault, 0, 120);
	DDX_TreeEdit(pDX, IDC_SCAR_OPTS, m_htiAutoQRSMax, m_iMaxRemoveQRSDefault);
	DDV_MinMaxInt(pDX, m_iMaxRemoveQRSDefault, 2500, 10000);
	DDX_TreeEdit(pDX, IDC_SCAR_OPTS, m_htiAutoQRSLimit, m_iMaxRemoveQRSLimitDefault);
	DDV_MinMaxInt(pDX, m_iMaxRemoveQRSLimitDefault, 50, 100);
	DDX_TreeRadio(pDX, IDC_SCAR_OPTS, m_htiAutoQRSWay, (int &)m_iHQRXmanDefault);
	// <== file settings - Stulle

	// ==> TBH: minimule - Max
	DDX_TreeCheck(pDX, IDC_SCAR_OPTS, m_htiShowMM, m_bShowMM);
	DDX_TreeCheck(pDX, IDC_SCAR_OPTS, m_htiMMLives, m_bMMLives);
	DDX_TreeEdit(pDX, IDC_SCAR_OPTS, m_htiMMUpdateTime, m_iMMUpdateTime);
	DDV_MinMaxInt(pDX, m_iMMUpdateTime, 0, MIN2S(60));
	DDX_TreeEdit(pDX, IDC_SCAR_OPTS, m_htiMMTrans, m_iMMTrans);
	DDV_MinMaxInt(pDX, m_iMMTrans, 1, 255);
	DDX_TreeCheck(pDX, IDC_SCAR_OPTS, m_htiMMCompl, m_bMMCompl);
	DDX_TreeCheck(pDX, IDC_SCAR_OPTS, m_htiMMOpen, m_bMMOpen);
	// <== TBH: minimule - Max

	DDX_TreeCheck(pDX, IDC_SCAR_OPTS, m_htiSUQWT, m_bSUQWT); // SUQWT [Moonlight/EastShare/ MorphXT] - Stulle
/*	// ==> Spread Credits Slot - Stulle
	DDX_TreeCheck(pDX, IDC_SCAR_OPTS, m_htiSpreadCreditsSlot, m_bSpreadCreditsSlot);
	DDX_TreeEdit(pDX, IDC_SCAR_OPTS, m_htiSpreadCreditsSlotCounter, m_iSpreadCreditsSlotCounter);
	DDV_MinMaxInt(pDX, m_iSpreadCreditsSlotCounter, 3, 20);
	// <== Spread Credits Slot - Stulle
*/	// ==> Global Source Limit [Max/Stulle] - Stulle
	DDX_TreeCheck(pDX, IDC_SCAR_OPTS, m_htiGlobalHL, m_bGlobalHL);
	DDX_TreeEdit(pDX, IDC_SCAR_OPTS, m_htiGlobalHlLimit, m_iGlobalHL);
	DDV_MinMaxInt(pDX, m_iGlobalHL, 1000, MAX_GSL);
	DDX_TreeCheck(pDX, IDC_SCAR_OPTS, m_htiGlobalHlAll, m_bGlobalHlAll);
	DDX_TreeCheck(pDX, IDC_SCAR_OPTS, m_htiGlobalHlDefault, m_bGlobalHlDefault);
	// <== Global Source Limit [Max/Stulle] - Stulle
	DDX_TreeCheck(pDX, IDC_SCAR_OPTS, m_htiStartupSound, m_bStartupSound); // Startupsound [Commander] - mav744

	// ==> FunnyNick [SiRoB/Stulle] - Stulle
	if(m_htiFnTagMode)	m_ctrlTreeOptions.SetGroupEnable(m_htiFnTagMode, m_bFnActive);
	if(m_htiFnTagAtEnd)	m_ctrlTreeOptions.SetCheckBoxEnable(m_htiFnTagAtEnd, m_bFnActive);
	if(m_bFnActive) m_ctrlTreeOptions.Expand(m_htiCustomTag, TVE_EXPAND);
	else m_ctrlTreeOptions.Expand(m_htiCustomTag, TVE_COLLAPSE);
	// <== FunnyNick [SiRoB/Stulle] - Stulle
	// ==> Quick start [TPT] - Max
	if (m_htiQuickStartAfterIPChange)	m_ctrlTreeOptions.SetCheckBoxEnable(m_htiQuickStartAfterIPChange, m_bQuickStart);
	// <== Quick start [TPT] - Max
	// ==> CPU/MEM usage [$ick$/Stulle] - Max
	if (m_htiSysInfoGlobal)	m_ctrlTreeOptions.SetCheckBoxEnable(m_htiSysInfoGlobal, m_bSysInfo);
	// <== CPU/MEM usage [$ick$/Stulle] - Max
}


BOOL CPPgScar::OnInitDialog()
{
	// ==> push small files [sivka] - Stulle
	m_bEnablePushSmallFile = thePrefs.GetEnablePushSmallFile();
	m_iPushSmallFileBoost = thePrefs.GetPushSmallFileBoost();
	// <== push small files [sivka] - Stulle
	m_bEnablePushRareFile = thePrefs.enablePushRareFile; // push rare file - Stulle

	// ==> FunnyNick [SiRoB/Stulle] - Stulle
	m_bFnActive = thePrefs.DisplayFunnyNick();
	m_iFnTag = thePrefs.GetFnTag();
	m_sFnCustomTag = thePrefs.m_sFnCustomTag;
	m_bFnTagAtEnd = thePrefs.GetFnTagAtEnd();
	// <== FunnyNick [SiRoB/Stulle] - Stulle

	// ==> Quick start [TPT] - Max
	m_bQuickStart = thePrefs.GetQuickStart();
	m_iQuickStartMaxTime = (int)(thePrefs.GetQuickStartMaxTime());
	m_iQuickStartMaxConnPerFive = (int)(thePrefs.GetQuickStartMaxConnPerFive());
	m_iQuickStartMaxConn = (int)(thePrefs.GetQuickStartMaxConn());
	m_iQuickStartMaxConnPerFiveBack = (int)(thePrefs.GetQuickStartMaxConnPerFiveBack());
	m_iQuickStartMaxConnBack = (int)(thePrefs.GetQuickStartMaxConnBack());
	m_bQuickStartAfterIPChange = thePrefs.GetQuickStartAfterIPChange();
	// <== Quick start [TPT] - Max
//	m_iReAskFileSrc = (thePrefs.GetReAskTimeDif() + FILEREASKTIME)/60000; // Timer for ReAsk File Sources - Stulle
	m_bACC = thePrefs.GetACC(); // ACC [Max/WiZaRd] - Max
/*
	// ==> Anti Uploader Ban - Stulle
	m_iAntiUploaderBanLimit = thePrefs.GetAntiUploaderBanLimit();
	m_iAntiUploaderBanCase = thePrefs.GetAntiUploaderBanCase();
	// <== Anti Uploader Ban - Stulle
*/
	// ==> CPU/MEM usage [$ick$/Stulle] - Max
	m_bSysInfo = thePrefs.GetSysInfo();
	m_bSysInfoGlobal = thePrefs.GetSysInfoGlobal();
	// <== CPU/MEM usage [$ick$/Stulle] - Max
	showSrcInTitle = thePrefs.ShowSrcOnTitle(); // Show sources on title - Stulle
//	m_bPsFilesRed = thePrefs.GetPsFilesRed(); // draw PS files red - Stulle
	m_bFriendsBlue = thePrefs.GetFriendsBlue(); // draw friends blue - Stulle
	m_bShowGlobalHL = thePrefs.GetShowGlobalHL(); // show global HL - Stulle
	m_bShowFileHLconst = thePrefs.GetShowFileHLconst(); // show HL per file constantaniously - Stulle
	m_bShowInMSN7 = thePrefs.GetShowMSN7(); // Show in MSN7 [TPT] - Stulle
	m_bQueueProgressBar = thePrefs.ShowClientQueueProgressBar(); // Client queue progress bar [Commander] - Stulle
//	m_bTrayComplete = thePrefs.GetTrayComplete(); // Completed in Tray - Stulle
	m_bShowClientPercentage = thePrefs.GetShowClientPercentage(); // Show Client Percentage [Commander/MorphXT] - Mondgott

	// ==> file settings - Stulle
	m_bEnableAutoDropNNSDefault = thePrefs.m_EnableAutoDropNNSDefault;
	m_iAutoNNS_TimerDefault = (thePrefs.m_AutoNNS_TimerDefault/1000);
	m_iMaxRemoveNNSLimitDefault = thePrefs.m_MaxRemoveNNSLimitDefault;
	m_bEnableAutoDropFQSDefault = thePrefs.m_EnableAutoDropFQSDefault;
	m_iAutoFQS_TimerDefault = (thePrefs.m_AutoFQS_TimerDefault/1000);
	m_iMaxRemoveFQSLimitDefault = thePrefs.m_MaxRemoveFQSLimitDefault;
	m_bEnableAutoDropQRSDefault = thePrefs.m_EnableAutoDropQRSDefault;
	m_iAutoHQRS_TimerDefault = (thePrefs.m_AutoHQRS_TimerDefault/1000);
	m_iMaxRemoveQRSDefault = thePrefs.m_MaxRemoveQRSDefault;
	m_iMaxRemoveQRSLimitDefault = thePrefs.m_MaxRemoveQRSLimitDefault;
	// <== file settings - Stulle

	// ==> TBH: minimule - Max
	m_bShowMM = thePrefs.IsMiniMuleEnabled();
	m_bMMLives = thePrefs.GetMiniMuleLives();
	m_iMMUpdateTime = thePrefs.GetMiniMuleUpdate();
	m_iMMTrans = thePrefs.GetMiniMuleTransparency();
	m_bMMCompl = thePrefs.m_bMMCompl;
	m_bMMOpen = thePrefs.m_bMMOpen;
	// <== TBH: minimule - Max

	m_bSUQWT = thePrefs.SaveUploadQueueWaitTime(); // SUQWT [Moonlight/EastShare/ MorphXT] - Stulle
/*	// ==> Spread Credits Slot - Stulle
	m_bSpreadCreditsSlot = thePrefs.SpreadCreditsSlot;
	m_iSpreadCreditsSlotCounter = (int)(thePrefs.SpreadCreditsSlotCounter);
	// <== Spread Credits Slot - Stulle
*/	// ==> Global Source Limit [Max/Stulle] - Stulle
	m_bGlobalHL = thePrefs.IsUseGlobalHL();
	m_iGlobalHL = thePrefs.GetGlobalHL();
	m_bGlobalHlAll = thePrefs.GetGlobalHlAll();
	m_bGlobalHlDefault = thePrefs.GetGlobalHlDefault();
	// <== Global Source Limit [Max/Stulle] - Stulle
	m_bStartupSound = thePrefs.UseStartupSound(); // Startupsound [Commander] - mav744

	CPropertyPage::OnInitDialog();

	// ==> Tabbed Preferences [TPT] - Stulle
	// Init the Tab control
	InitTab();

	// Create and Init all controls
	InitControl();

	// Set default tab
	m_currentTab = NONE;
	m_tabCtr.SetCurSel(0);
	SetTab(SCAR);//Tab x defecto Conexion
	// <== Tabbed Preferences [TPT] - Stulle

	// ==> push small files [sivka] - Stulle
//	InitWindowStyles(this);
	LoadSettings();
	// <== push small files [sivka] - Stulle

	Localize();
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CPPgScar::LoadSettings(void)
{
	if(m_hWnd)
	{
		CString strBuffer;
	
		// ==> push small files [sivka] - Stulle
		// ==> Tabbed Preferences [TPT] - Stulle
		/*
		((CSliderCtrl*)GetDlgItem(IDC_PUSHSMALL_SLIDER))->SetRange(1, PARTSIZE, TRUE);
		((CSliderCtrl*)GetDlgItem(IDC_PUSHSMALL_SLIDER))->SetPos(thePrefs.GetPushSmallFileSize());
		ShowPushSmallFileValues();
		*/
		m_iPushSmall.SetPos(thePrefs.GetPushSmallFileSize());
		ShowPushSmallFileValues();
		// <== Tabbed Preferences [TPT] - Stulle
		// <== push small files [sivka] - Stulle

		// ==> WebCache [WC team/MorphXT] - Stulle/Max
		bool bTemp = thePrefs.webcacheEnabled;
		// check/uncheck webcache
		m_bWcDl.SetCheck(bTemp);
		// enter name 
		m_webcacheAddressEdit.EnableWindow(bTemp);
		strBuffer.Format(_T("%s"), thePrefs.webcacheName);
		m_webcacheAddressEdit.SetWindowText(strBuffer);
		// enter Port
		m_webcachePortEdit.EnableWindow(bTemp);
		strBuffer.Format(_T("%d"), thePrefs.webcachePort);
		m_webcachePortEdit.SetWindowText(strBuffer);
		// load parts to download before reconnect
		m_blockEdit.EnableWindow(bTemp);
		strBuffer.Format(_T("%d"), thePrefs.GetWebCacheBlockLimit());
		m_blockEdit.SetWindowText(strBuffer);
		// load extratimeoutsetting
		m_TimeOut.EnableWindow(bTemp);
		m_TimeOut.SetCheck(thePrefs.GetWebCacheExtraTimeout());
		// load localtrafficsettings
		m_CacheISP.EnableWindow(bTemp);
		m_CacheISP.SetCheck(thePrefs.GetWebCacheCachesLocalTraffic()==false);
		// load persistent proxy conns
		m_WcDlPersistent.EnableWindow(bTemp);
		m_WcDlPersistent.SetCheck(thePrefs.PersistentConnectionsForProxyDownloads);
		// load autoupdate
		m_Update.EnableWindow(bTemp);
		m_Update.SetCheck(thePrefs.WCAutoupdate);
		// <== WebCache [WC team/MorphXT] - Stulle/Max

		// ==> TBH: Backup [TBH/EastShare/MorphXT] - Stulle
		m_AutoBackup.SetCheck(thePrefs.GetAutoBackup());
		m_AutoBackup2.EnableWindow(thePrefs.GetAutoBackup());
		m_AutoBackup2.SetCheck(thePrefs.GetAutoBackup2());
		// <== TBH: Backup [TBH/EastShare/MorphXT] - Stulle
	}
}

BOOL CPPgScar::OnKillActive()
{
	// if prop page is closed by pressing VK_ENTER we have to explicitly commit any possibly pending
	// data from an open edit control
	m_ctrlTreeOptions.HandleChildControlLosingFocus();
	return CPropertyPage::OnKillActive();
}

BOOL CPPgScar::OnApply()
{
	// if prop page is closed by pressing VK_ENTER we have to explicitly commit any possibly pending
	// data from an open edit control
	m_ctrlTreeOptions.HandleChildControlLosingFocus();

	if (!UpdateData())
		return FALSE;
	// ==> push small files [sivka] - Stulle
	thePrefs.enablePushSmallFile = m_bEnablePushSmallFile;
	thePrefs.m_iPushSmallBoost = (uint16)m_iPushSmallFileBoost;
	CString strBuffer;
	// ==> Tabbed Preferences [TPT] - Stulle
	/*
	((CSliderCtrl*)GetDlgItem(IDC_PUSHSMALL_SLIDER))->SetRange(1, PARTSIZE, TRUE);
	thePrefs.m_iPushSmallFiles = ((CSliderCtrl*)GetDlgItem(IDC_PUSHSMALL_SLIDER))->GetPos();
	*/
	thePrefs.m_iPushSmallFiles = m_iPushSmall.GetPos();
	// <== Tabbed Preferences [TPT] - Stulle
	// <== push small files [sivka] - Stulle
	thePrefs.enablePushRareFile = m_bEnablePushRareFile; // push rare file - Stulle

	// ==> FunnyNick [SiRoB/Stulle] - Stulle
	thePrefs.m_bFunnyNick = m_bFnActive;
	thePrefs.FnTagMode = (uint8)m_iFnTag;
	_stprintf (thePrefs.m_sFnCustomTag,_T("%s"), m_sFnCustomTag);
	thePrefs.m_bFnTagAtEnd = m_bFnTagAtEnd;
	// <== FunnyNick [SiRoB/Stulle] - Stulle

	// ==> Quick start [TPT] - Max
	thePrefs.m_bQuickStart = m_bQuickStart;
	thePrefs.m_iQuickStartMaxTime = (uint16)m_iQuickStartMaxTime;
	thePrefs.m_iQuickStartMaxConnPerFive = (uint16)m_iQuickStartMaxConnPerFive;
	thePrefs.m_iQuickStartMaxConn = m_iQuickStartMaxConn;
	thePrefs.m_iQuickStartMaxConnPerFiveBack = (uint16)m_iQuickStartMaxConnPerFiveBack;
	thePrefs.m_iQuickStartMaxConnBack = m_iQuickStartMaxConnBack;
	thePrefs.m_bQuickStartAfterIPChange = m_bQuickStartAfterIPChange;
	// <== Quick start [TPT] - Max
//	thePrefs.m_uReAskTimeDif = (m_iReAskFileSrc-29)*60000; // Timer for ReAsk File Sources - Stulle
	thePrefs.m_bACC = m_bACC; // ACC [Max/WiZaRd] - Max
/*
	// ==> Anti Uploader Ban - Stulle
	thePrefs.m_iAntiUploaderBanLimit = m_iAntiUploaderBanLimit;
	thePrefs.AntiUploaderBanCaseMode = m_iAntiUploaderBanCase;
	// <== Anti Uploader Ban - Stulle
*/
	// ==> CreditSystems [EastShare/ MorphXT] - Stulle
	if(thePrefs.creditSystemMode != m_iCreditSystem){
		thePrefs.creditSystemMode = (uint8)m_iCreditSystem;
		theApp.clientcredits->ResetCheckScoreRatio();
	}
	// <== CreditSystems [EastShare/ MorphXT] - Stulle

	// ==> CPU/MEM usage [$ick$/Stulle] - Max
	if(thePrefs.m_bSysInfo != m_bSysInfo)
	{
		thePrefs.m_bSysInfo = m_bSysInfo;
		theApp.emuledlg->transferwnd->EnableSysInfo(m_bSysInfo);
	}
	thePrefs.m_bSysInfoGlobal = m_bSysInfoGlobal;
	// <== CPU/MEM usage [$ick$/Stulle] - Max
	thePrefs.showSrcInTitle = showSrcInTitle; // Show sources on title - Stulle
//	thePrefs.PsFilesRed = m_bPsFilesRed; // draw PS files red - Stulle
	thePrefs.FriendsBlue = m_bFriendsBlue; // draw friends blue - Stulle
	thePrefs.ShowGlobalHL = m_bShowGlobalHL; // show global HL - Stulle
	thePrefs.ShowFileHLconst = m_bShowFileHLconst; // show HL per file constantaniously - Stulle
	thePrefs.m_bShowInMSN7 = m_bShowInMSN7; // Show in MSN7 [TPT] - Stulle
	thePrefs.m_bClientQueueProgressBar = m_bQueueProgressBar; // Client queue progress bar [Commander] - Stulle
//	thePrefs.m_bTrayComplete = m_bTrayComplete; // Completed in Tray - Stulle
	thePrefs.m_bShowClientPercentage = m_bShowClientPercentage; // Show Client Percentage [Commander/MorphXT] - Mondgott

	// ==> file settings - Stulle
	thePrefs.m_EnableAutoDropNNSDefault = m_bEnableAutoDropNNSDefault;
	thePrefs.m_AutoNNS_TimerDefault = (m_iAutoNNS_TimerDefault*1000);
	thePrefs.m_MaxRemoveNNSLimitDefault = (uint16)m_iMaxRemoveNNSLimitDefault;
	thePrefs.m_EnableAutoDropFQSDefault = m_bEnableAutoDropFQSDefault;
	thePrefs.m_AutoFQS_TimerDefault = (m_iAutoFQS_TimerDefault*1000);
	thePrefs.m_MaxRemoveFQSLimitDefault = (uint16)m_iMaxRemoveFQSLimitDefault;
	thePrefs.m_EnableAutoDropQRSDefault = m_bEnableAutoDropQRSDefault;
	thePrefs.m_AutoHQRS_TimerDefault = (m_iAutoHQRS_TimerDefault*1000);
	thePrefs.m_MaxRemoveQRSDefault = (uint16)m_iMaxRemoveQRSDefault;
	thePrefs.m_MaxRemoveQRSLimitDefault = (uint16)m_iMaxRemoveQRSLimitDefault;
	thePrefs.m_bHQRXmanDefault = m_iHQRXmanDefault == 1;
	// <== file settings - Stulle

	// ==> TBH: minimule - Max
	thePrefs.m_bMiniMule = m_bShowMM;
	thePrefs.SetMiniMuleLives(m_bMMLives);
	thePrefs.m_iMiniMuleUpdate = m_iMMUpdateTime;
	thePrefs.SetMiniMuleTransparency((uint8)m_iMMTrans);
	thePrefs.m_bMMCompl = m_bMMCompl;
	thePrefs.m_bMMOpen = m_bMMOpen;
	// <== TBH: minimule - Max

	thePrefs.m_bSaveUploadQueueWaitTime = m_bSUQWT; // SUQWT [Moonlight/EastShare/ MorphXT] - Stulle
/*	// ==> Spread Credits Slot - Stulle
	thePrefs.SpreadCreditsSlot = m_bSpreadCreditsSlot;
	thePrefs.SpreadCreditsSlotCounter = m_iSpreadCreditsSlotCounter;
	// <== Spread Credits Slot - Stulle
*/	// ==> Global Source Limit [Max/Stulle] - Stulle
	if (thePrefs.GetGlobalHL() != (UINT)m_iGlobalHL ||
		thePrefs.IsUseGlobalHL() != m_bGlobalHL ||
		thePrefs.m_bGlobalHlAll != m_bGlobalHlAll)
	{
		thePrefs.m_bGlobalHL = m_bGlobalHL;
		thePrefs.m_uGlobalHL = m_iGlobalHL;
		thePrefs.m_bGlobalHlAll = m_bGlobalHlAll;
		if(m_bGlobalHL && theApp.downloadqueue->GetPassiveMode())
		{
			theApp.downloadqueue->SetPassiveMode(false);
			theApp.downloadqueue->SetUpdateHlTime(50000); // 50 sec
			AddDebugLogLine(true,_T("{GSL} Global Source Limit settings have changed! Disabled PassiveMode!"));
		}
	}
	thePrefs.m_bGlobalHlDefault = m_bGlobalHlDefault;
	// <== Global Source Limit [Max/Stulle] - Stulle
	thePrefs.startupsound = m_bStartupSound; // Startupsound [Commander] - mav744

	// ==> WebCache [WC team/MorphXT] - Stulle/Max
	bool bRestartApp = false;

	// set thePrefs.webcacheName
	if(m_webcacheAddressEdit.GetWindowTextLength())
	{
		CString nNewwebcache;
		m_webcacheAddressEdit.GetWindowText(nNewwebcache);
		if (thePrefs.webcacheName != nNewwebcache){
			thePrefs.webcacheName = nNewwebcache;
			//MORPH - Removed by SiRoB, New ResolveWebCachename
			//bRestartApp = true;
		}
	}

	// set thePrefs.webcachePort
	if(m_webcachePortEdit.GetWindowTextLength())
	{
		m_webcachePortEdit.GetWindowText(strBuffer);
		uint16 nNewPort = (uint16)_tstol(strBuffer);
		if (!nNewPort) nNewPort=0;
		if (nNewPort != thePrefs.webcachePort){
			thePrefs.webcachePort = nNewPort;
		}
	}
	
	// set thePrefs.webcacheEnabled
	thePrefs.webcacheEnabled = m_bWcDl.GetCheck() == BST_CHECKED;
	
	
	// set thePrefs.webcacheBlockLimit
	if(m_blockEdit.GetWindowTextLength())
	{
		m_blockEdit.GetWindowText(strBuffer);
		uint16 nNewBlocks = (uint16)_tstol(strBuffer);
		if ((!nNewBlocks) || (nNewBlocks > 50000) || (nNewBlocks < 0)) nNewBlocks=0;
		if (nNewBlocks != thePrefs.GetWebCacheBlockLimit()){
			thePrefs.SetWebCacheBlockLimit(nNewBlocks);
		}
	}
	
	// set thePrefs.WebCacheExtraTimeout
	thePrefs.SetWebCacheExtraTimeout(m_TimeOut.GetCheck() == BST_CHECKED);

	// set thePrefs.WebCacheCachesLocalTraffic
	bool cachestraffic;
	cachestraffic = m_CacheISP.GetCheck() == BST_CHECKED;
	if (cachestraffic == true) thePrefs.SetWebCacheCachesLocalTraffic(0);
	else thePrefs.SetWebCacheCachesLocalTraffic(1);

	// set thePrefs.PersistentConnectionsForProxyDownloads
	thePrefs.PersistentConnectionsForProxyDownloads = m_WcDlPersistent.GetCheck() == BST_CHECKED;

	// set thePrefs.WCAutoupdate
	thePrefs.WCAutoupdate = m_Update.GetCheck() == BST_CHECKED;

	if (bRestartApp)
	{
		AfxMessageBox(GetResString(IDS_SETTINGCHANGED_RESTART));
		thePrefs.WebCacheDisabledThisSession = true;
	}
	// <== WebCache [WC team/MorphXT] - Stulle/Max

	// ==> TBH: Backup [TBH/EastShare/MorphXT] - Stulle
	thePrefs.m_bAutoBackup = m_AutoBackup.GetCheck() == BST_CHECKED;
	thePrefs.m_bAutoBackup2 = m_AutoBackup2.GetCheck() == BST_CHECKED;
	// <== TBH: Backup [TBH/EastShare/MorphXT] - Stulle

	LoadSettings();

	// ==> Show sources on title - Stulle
	TCHAR buffer[510];

	if (!thePrefs.ShowRatesOnTitle() && !thePrefs.ShowSrcOnTitle()) {
		// ==> ModID [itsonlyme/SiRoB] - Stulle
		/*
		_stprintf(buffer,_T("eMule v%s"),theApp.m_strCurVersionLong + _T(" ") + MOD_VERSION); // Xman // Maella -Support for tag ET_MOD_VERSION 0x55
		*/
		// <== ModID [itsonlyme/SiRoB] - Stulle
		_stprintf(buffer,_T("eMule v%s [%s]"),theApp.m_strCurVersionLong,theApp.m_strModLongVersion);
		theApp.emuledlg->SetWindowText(buffer);
	}
	// <== Show sources on title - Stulle

	SetModified(FALSE);

	return CPropertyPage::OnApply();
}

void CPPgScar::Localize(void)
{	
	if(m_hWnd)
	{
		// ==> Tabbed Preferences [TPT] - Stulle
		/*
		GetDlgItem(IDC_WARNING)->SetWindowText(GetResString(IDS_TWEAKS_WARNING));
		*/
		int row = m_tabCtr.GetRowCount();
		InitTab(); // To update string, could be improved
		if(row != 0 && row != m_tabCtr.GetRowCount()){
			// Shift all windows object
			// .. to do
		}

		m_strWarning.SetWindowText(GetResString(IDS_TWEAKS_WARNING));
		// <== Tabbed Preferences [TPT] - Stulle

		// ==> push small files [sivka] - Stulle
		if (m_htiEnablePushSmallFile) m_ctrlTreeOptions.SetItemText(m_htiEnablePushSmallFile, GetResString(IDS_PUSH_SMALL));
		if (m_htiPushSmallFileBoost) m_ctrlTreeOptions.SetEditLabel(m_htiPushSmallFileBoost, GetResString(IDS_PUSH_SMALL_BOOST));
		// <== push small files [sivka] - Stulle
		if (m_htiEnablePushRareFile) m_ctrlTreeOptions.SetItemText(m_htiEnablePushRareFile, GetResString(IDS_PUSH_RARE)); // push rare file - Stulle

		// ==> FunnyNick [SiRoB/Stulle] - Stulle
		if (m_htiFnActive) m_ctrlTreeOptions.SetItemText(m_htiFnActive,GetResString(IDS_ENABLE));
		if (m_htiFnCustomTag) m_ctrlTreeOptions.SetEditLabel(m_htiFnCustomTag, GetResString(IDS_SET_CUSTOM_TAG));
		if (m_htiFnTagAtEnd) m_ctrlTreeOptions.SetItemText(m_htiFnTagAtEnd, GetResString(IDS_FN_TAG_AT_END));
		// <== FunnyNick [SiRoB/Stulle] - Stulle

		// ==> Quick start [TPT] - Max
		if (m_htiQuickStart) m_ctrlTreeOptions.SetItemText(m_htiQuickStart, GetResString(IDS_QUICK_START));
		if (m_htiQuickStartMaxTime) m_ctrlTreeOptions.SetEditLabel(m_htiQuickStartMaxTime, GetResString(IDS_QUICK_START_MAX_TIME));
		if (m_htiQuickStartMaxConnPerFive) m_ctrlTreeOptions.SetEditLabel(m_htiQuickStartMaxConnPerFive, GetResString(IDS_QUICK_START_MAX_CONN_PER_FIVE));
		if (m_htiQuickStartMaxConn) m_ctrlTreeOptions.SetEditLabel(m_htiQuickStartMaxConn, GetResString(IDS_QUICK_START_MAX_CONN));
		if (m_htiQuickStartMaxConnPerFiveBack) m_ctrlTreeOptions.SetEditLabel(m_htiQuickStartMaxConnPerFiveBack, GetResString(IDS_QUICK_START_MAX_CONN_PER_FIVE_BACK));
		if (m_htiQuickStartMaxConnBack) m_ctrlTreeOptions.SetEditLabel(m_htiQuickStartMaxConnBack, GetResString(IDS_QUICK_START_MAX_CONN_BACK));
		if (m_htiQuickStartAfterIPChange) m_ctrlTreeOptions.SetItemText(m_htiQuickStartAfterIPChange, GetResString(IDS_QUICK_START_AFTER_IP_CHANGE));
		// <== Quick start [TPT] - Max
//		if (m_htiReAskFileSrc) m_ctrlTreeOptions.SetEditLabel(m_htiReAskFileSrc, GetResString(IDS_REASK_FILE_SRC)); // Timer for ReAsk File Sources - Stulle
		if (m_htiACC) m_ctrlTreeOptions.SetItemText(m_htiACC, GetResString(IDS_ACC)); // ACC [Max/WiZaRd] - Max
/*
		// ==> Anti Uploader Ban - Stulle
		if (m_htiAntiUploaderBanLimit) m_ctrlTreeOptions.SetEditLabel(m_htiAntiUploaderBanLimit, GetResString(IDS_UNBAN_UPLOADER));
		// <== Anti Uploader Ban - Stulle
*/
		// ==> CPU/MEM usage [$ick$/Stulle] - Max
		if (m_htiSysInfo) m_ctrlTreeOptions.SetItemText(m_htiSysInfo, GetResString(IDS_ENABLED));
		if (m_htiSysInfoGlobal) m_ctrlTreeOptions.SetItemText(m_htiSysInfoGlobal, GetResString(IDS_SYS_INFO_GLOBAL));
		// <== CPU/MEM usage [$ick$/Stulle] - Max
		if (m_htiShowSrcOnTitle) m_ctrlTreeOptions.SetItemText(m_htiShowSrcOnTitle, GetResString(IDS_SHOWSRCONTITLE)); // Show sources on title - Stulle
//		if (m_htiPsFilesRed) m_ctrlTreeOptions.SetItemText(m_htiPsFilesRed, GetResString(IDS_PS_FILES_RED)); // draw PS files red - Stulle
		if (m_htiFriendsBlue) m_ctrlTreeOptions.SetItemText(m_htiFriendsBlue, GetResString(IDS_FRIENDS_BLUE)); // draw friends blue - Stulle
		if (m_htiShowGlobalHL) m_ctrlTreeOptions.SetItemText(m_htiShowGlobalHL, GetResString(IDS_SHOW_GLOBAL_HL)); // show global HL - Stulle
		if (m_htiShowFileHLconst) m_ctrlTreeOptions.SetItemText(m_htiShowFileHLconst, GetResString(IDS_SHOW_FILE_HL_CONST)); // show HL per file constantaniously - Stulle
		if (m_htiShowInMSN7) m_ctrlTreeOptions.SetItemText(m_htiShowInMSN7, GetResString(IDS_SHOWINMSN7)); // Show in MSN7 [TPT] - Stulle
		if (m_htiQueueProgressBar) m_ctrlTreeOptions.SetItemText(m_htiQueueProgressBar, GetResString(IDS_CLIENTQUEUEPROGRESSBAR)); // Client queue progress bar [Commander] - Stulle
//		if (m_htiTrayComplete) m_ctrlTreeOptions.SetItemText(m_htiTrayComplete, GetResString(IDS_TRAY_COMPLETE)); // Completed in Tray - Stulle
		if (m_htiShowClientPercentage) m_ctrlTreeOptions.SetItemText(m_htiShowClientPercentage, GetResString(IDS_CLIENTPERCENTAGE)); // Show Client Percentage [Commander/MorphXT] - Mondgott

		// ==> file settings - Stulle
		if (m_htiAutoNNS) m_ctrlTreeOptions.SetItemText(m_htiAutoNNS, GetResString(IDS_AUTO_NNS));
		if (m_htiAutoNNSTimer) m_ctrlTreeOptions.SetEditLabel(m_htiAutoNNSTimer, GetResString(IDS_NNS_TIMERLABEL));
		if (m_htiAutoNNSLimit) m_ctrlTreeOptions.SetEditLabel(m_htiAutoNNSLimit, GetResString(IDS_REMOVENNSLIMITLABEL));
		if (m_htiAutoFQS) m_ctrlTreeOptions.SetItemText(m_htiAutoFQS, GetResString(IDS_AUTO_FQS));
		if (m_htiAutoFQSTimer) m_ctrlTreeOptions.SetEditLabel(m_htiAutoFQSTimer, GetResString(IDS_FQS_TIMERLABEL));
		if (m_htiAutoFQSLimit) m_ctrlTreeOptions.SetEditLabel(m_htiAutoFQSLimit, GetResString(IDS_REMOVEFQSLIMITLABEL));
		if (m_htiAutoQRS) m_ctrlTreeOptions.SetItemText(m_htiAutoQRS, GetResString(IDS_AUTO_QRS));
		if (m_htiAutoQRSTimer) m_ctrlTreeOptions.SetEditLabel(m_htiAutoQRSTimer, GetResString(IDS_HQRS_TIMERLABEL));
		if (m_htiAutoQRSMax) m_ctrlTreeOptions.SetEditLabel(m_htiAutoQRSMax, GetResString(IDS_REMOVEQRSLABEL));
		if (m_htiHQRSivka) m_ctrlTreeOptions.SetItemText(m_htiHQRSivka, GetResString(IDS_SIVKA_DROPPING));
		if (m_htiAutoQRSLimit) m_ctrlTreeOptions.SetEditLabel(m_htiAutoQRSLimit, GetResString(IDS_REMOVEQRSLIMITLABEL));
		// <== file settings - Stulle

		// ==> TBH: minimule - Max
		if (m_htiMMGroup) m_ctrlTreeOptions.SetItemText(m_htiMMGroup, _T("TBH Mini-Mule"));
		if (m_htiShowMM) m_ctrlTreeOptions.SetItemText(m_htiShowMM, GetResString(IDS_MM_SHOW));
		if (m_htiMMLives) m_ctrlTreeOptions.SetItemText(m_htiMMLives, GetResString(IDS_MM_LIVES));
		if (m_htiMMUpdateTime) m_ctrlTreeOptions.SetEditLabel(m_htiMMUpdateTime, GetResString(IDS_MM_UPDATE_TIME));
		if (m_htiMMTrans) m_ctrlTreeOptions.SetEditLabel(m_htiMMTrans, GetResString(IDS_MM_TRANS));
		if (m_htiMMCompl) m_ctrlTreeOptions.SetItemText(m_htiMMCompl, GetResString(IDS_MM_COMPL));
		if (m_htiMMOpen) m_ctrlTreeOptions.SetItemText(m_htiMMOpen, GetResString(IDS_MM_OPEN));
		// <== TBH: minimule - Max

		if (m_htiSUQWT) m_ctrlTreeOptions.SetItemText(m_htiSUQWT, GetResString(IDS_SUQWT)); // SUQWT [Moonlight/EastShare/ MorphXT] - Stulle
/*		// ==> Spread Credits Slot - Stulle
		if (m_htiSpreadCreditsSlot) m_ctrlTreeOptions.SetItemText(m_htiSpreadCreditsSlot, GetResString(IDS_SPREAD_CREDITS_SLOT));
		if (m_htiSpreadCreditsSlotCounter) m_ctrlTreeOptions.SetEditLabel(m_htiSpreadCreditsSlotCounter, GetResString(IDS_SPREAD_CREDITS_SLOT_COUNTER));
		// <== Spread Credits Slot - Stulle
*/		// ==> Global Source Limit [Max/Stulle] - Stulle
		if (m_htiGlobalHL) m_ctrlTreeOptions.SetItemText(m_htiGlobalHL, GetResString(IDS_ENABLE));
		if (m_htiGlobalHlLimit) m_ctrlTreeOptions.SetEditLabel(m_htiGlobalHlLimit, GetResString(IDS_GLOBAL_HL_LIMIT));
		if (m_htiGlobalHlAll) m_ctrlTreeOptions.SetItemText(m_htiGlobalHlAll, GetResString(IDS_GLOBAL_HL_ALL));
		if (m_htiGlobalHlDefault) m_ctrlTreeOptions.SetItemText(m_htiGlobalHlDefault, GetResString(IDS_GLOBAL_HL_DEFAULT));
		// <== Global Source Limit [Max/Stulle] - Stulle
		if (m_htiStartupSound) m_ctrlTreeOptions.SetItemText(m_htiStartupSound, GetResString(IDS_STARTUPSOUND)); // Startupsound [Commander] - mav744

		// ==> WebCache [WC team/MorphXT] - Stulle/Max
		m_WcProxyBox.SetWindowText( GetResString(IDS_WEBCACHE_ISP) );
		m_webcachePortStatic.SetWindowText( GetResString(IDS_WC_PORT) );
		m_webcacheAddressStatic.SetWindowText( GetResString(IDS_WC_ADDRESS) );
		m_WcCtrlBox.SetWindowText( GetResString(IDS_WC_CONTROLS) );
		m_bWcDl.SetWindowText( GetResString(IDS_WC_ENABLE) );
		m_autoDetect.SetWindowText( GetResString(IDS_WC_AUTO) );
		m_blockText.SetWindowText( GetResString(IDS_WC_NRBLOCKS) );
		m_blockLabel.SetWindowText( GetResString(IDS_WC_BLOCK) );
		m_TimeOut.SetWindowText( GetResString(IDS_WC_TIMEOUT) );
		m_CacheISP.SetWindowText( GetResString(IDS_WC_LOCAL) );
		m_WcDlPersistent.SetWindowText( GetResString(IDS_WC_PERSISTENT) );
		m_hideControls.SetWindowText( GetResString(IDS_WC_ADVANCED) );
		m_webcacheTest.SetWindowText( GetResString(IDS_WC_TEST) );
		m_Update.SetWindowText( GetResString(IDS_WC_UPDATESETTING) );
		m_WrongPort.SetWindowText( GetResString(IDS_WC_WRONGPORT) );
		// <== WebCache [WC team/MorphXT] - Stulle/Max

		// ==> TBH: Backup [TBH/EastShare/MorphXT] - Stulle
		m_BackupBox.SetWindowText( GetResString(IDS_BACKUP_FILEFRAME) );
		m_SelectAll.SetWindowText( GetResString(IDS_BACKUP_SELECTALL) );
		m_BackupNow.SetWindowText( GetResString(IDS_BACKUP_BACKUPNOW) );
		m_AutoBackupBox.SetWindowText( GetResString(IDS_BACKUP_AUTOFRAME) );
		m_AutoBackup.SetWindowText( GetResString(IDS_BACKUP_AUTOBACKUP));
		m_AutoBackup2.SetWindowText( GetResString(IDS_BACKUP_AUTOBACKUP2));
		m_Note.SetWindowText( GetResString(IDS_BACKUP_NOTE));
		m_NoteText.SetWindowText( GetResString(IDS_BACKUP_MESSAGE) );
		// <== TBH: Backup [TBH/EastShare/MorphXT] - Stulle
	}

}

void CPPgScar::OnDestroy()
{
	m_ctrlTreeOptions.DeleteAllItems();
	m_ctrlTreeOptions.DestroyWindow();
	m_bInitializedTreeOpts = false;
	m_htiPush = NULL; // push files - Stulle
	// ==> push small files [sivka] - Stulle
	m_htiEnablePushSmallFile = NULL;
	m_htiPushSmallFileBoost = NULL;
	// <== push small files [sivka] - Stulle
	m_htiEnablePushRareFile = NULL; // push rare file - Stulle

	// ==> FunnyNick [SiRoB/Stulle] - Stulle
	m_htiFnActive = NULL;
	m_htiFnTag = NULL;
	m_htiNoTag = NULL;
	m_htiShortTag = NULL;
	m_htiFullTag = NULL;
	m_htiCustomTag = NULL;
	m_htiFnCustomTag = NULL;
	m_htiFnTagAtEnd = NULL;
	// <== FunnyNick [SiRoB/Stulle] - Stulle

	m_htiConTweaks = NULL;
	// ==> Quick start [TPT] - Max
	m_htiQuickStartGroup = NULL;
	m_htiQuickStart = NULL;
	m_htiQuickStartMaxTime = NULL;
	m_htiQuickStartMaxConnPerFive = NULL;
	m_htiQuickStartMaxConn = NULL;
	m_htiQuickStartMaxConnPerFiveBack = NULL;
	m_htiQuickStartMaxConnBack = NULL;
	m_htiQuickStartAfterIPChange = NULL;
	// <== Quick start [TPT] - Max
//	m_htiReAskFileSrc = NULL; // Timer for ReAsk File Sources - Stulle
	m_htiACC = NULL; // ACC [Max/WiZaRd] - Max
/*
	// ==> Anti Uploader Ban - Stulle
	m_htiAntiUploaderBanLimit = NULL;
	m_htiAntiCase1 = NULL;
	m_htiAntiCase2 = NULL;
	m_htiAntiCase3 = NULL;
	// <== Anti Uploader Ban - Stulle
*/
	// ==> CreditSystems [EastShare/ MorphXT] - Stulle
	m_htiCreditSystem = NULL;
	m_htiOfficialCredit = NULL;
	m_htiLovelaceCredit = NULL;
	m_htiRatioCredit = NULL;
	m_htiPawcioCredit = NULL;
	m_htiESCredit = NULL;
	m_htiSivkaCredit = NULL;
	m_htiSwatCredit = NULL;
	m_htiXmanCredit = NULL;
	// <== CreditSystems [EastShare/ MorphXT] - Stulle

	m_htiDisplay = NULL;
	// ==> CPU/MEM usage [$ick$/Stulle] - Max
	m_htiSysInfoGroup = NULL;
	m_htiSysInfo = NULL;
	m_htiSysInfoGlobal = NULL;
	// <== CPU/MEM usage [$ick$/Stulle] - Max
	m_htiShowSrcOnTitle = NULL; // Show sources on title - Stulle
//	m_htiPsFilesRed = NULL; // draw PS files red - Stulle
	m_htiFriendsBlue = NULL; // draw friends blue - Stulle
	m_htiShowGlobalHL = NULL; // show global HL - Stulle
	m_htiShowFileHLconst = NULL; // show HL per file constantaniously - Stulle
	m_htiShowInMSN7 = NULL; // Show in MSN7 [TPT] - Stulle
	m_htiQueueProgressBar = NULL;
//	m_htiTrayComplete = NULL; // Completed in Tray - Stulle
	m_htiShowClientPercentage = NULL; // Show Client Percentage [Commander/MorphXT] - Mondgott

	// ==> file settings - Stulle
	m_htiFileDefaults = NULL;
	m_htiAutoNNS = NULL;
	m_htiAutoNNSTimer = NULL;
	m_htiAutoNNSLimit = NULL;
	m_htiAutoFQS = NULL;
	m_htiAutoFQSTimer = NULL;
	m_htiAutoFQSLimit = NULL;
	m_htiAutoQRS = NULL;
	m_htiAutoQRSTimer = NULL;
	m_htiAutoQRSMax = NULL;
	m_htiAutoQRSLimit = NULL;
	m_htiAutoQRSWay = NULL;
	m_htiHQRXman = NULL;
	m_htiHQRSivka = NULL;
	// <== file settings - Stulle

	// ==> TBH: minimule - Max
	m_htiMMGroup = NULL;
	m_htiShowMM = NULL;
	m_htiMMLives = NULL;
	m_htiMMUpdateTime = NULL;
	m_htiMMTrans = NULL;
	m_htiMMCompl = NULL;
	m_htiMMOpen = NULL;
	// <== TBH: minimule - Max

	m_htiMisc = NULL;
	m_htiSUQWT = NULL; // SUQWT [Moonlight/EastShare/ MorphXT] - Stulle
/*	// ==> Spread Credits Slot - Stulle
	m_htiSpreadCreditsSlot = NULL;
	m_htiSpreadCreditsSlotCounter = NULL;
	// <== Spread Credits Slot - Stulle
*/	// ==> Global Source Limit [Max/Stulle] - Stulle
	m_htiGlobalHlGroup = NULL;
	m_htiGlobalHL = NULL;
	m_htiGlobalHlLimit = NULL;
	m_htiGlobalHlAll = NULL;
	m_htiGlobalHlDefault = NULL;
	// <== Global Source Limit [Max/Stulle] - Stulle
	m_htiStartupSound = NULL; // Startupsound [Commander] - mav744

	CPropertyPage::OnDestroy();
}
LRESULT CPPgScar::OnTreeOptsCtrlNotify(WPARAM wParam, LPARAM lParam)
{
	if (wParam == IDC_SCAR_OPTS){
		TREEOPTSCTRLNOTIFY* pton = (TREEOPTSCTRLNOTIFY*)lParam;

	BOOL bCheck;

		// ==> FunnyNick [SiRoB/Stulle] - Stulle
		if (m_htiFnActive && pton->hItem == m_htiFnActive)
		{
			if (m_ctrlTreeOptions.GetCheckBox(m_htiFnActive, bCheck))
			{
				if(m_htiFnTagMode)	m_ctrlTreeOptions.SetGroupEnable(m_htiFnTagMode, bCheck);
				if(m_htiFnTagAtEnd)	m_ctrlTreeOptions.SetCheckBoxEnable(m_htiFnTagAtEnd, bCheck);
				if(bCheck) m_ctrlTreeOptions.Expand(m_htiCustomTag, TVE_EXPAND);
				else m_ctrlTreeOptions.Expand(m_htiCustomTag, TVE_COLLAPSE);
			}
		}
		// <== FunnyNick [SiRoB/Stulle] - Stulle
		// ==> Quick start [TPT] - Max
		if (m_htiQuickStart && pton->hItem == m_htiQuickStart)
		{
			if (m_ctrlTreeOptions.GetCheckBox(m_htiQuickStart, bCheck))
			{
				if (m_htiQuickStartAfterIPChange)	m_ctrlTreeOptions.SetCheckBoxEnable(m_htiQuickStartAfterIPChange, bCheck);
			}
		}
		// <== Quick start [TPT] - Max

		// ==> CPU/MEM usage [$ick$/Stulle] - Max
		if (m_htiSysInfo && pton->hItem == m_htiSysInfo)
		{
			if (m_ctrlTreeOptions.GetCheckBox(m_htiSysInfo, bCheck))
			{
				if (m_htiSysInfoGlobal)	m_ctrlTreeOptions.SetCheckBoxEnable(m_htiSysInfoGlobal, bCheck);
			}
		}
		// <== CPU/MEM usage [$ick$/Stulle] - Max

		SetModified();
	}
	return 0;
}

void CPPgScar::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// ==> push small files [sivka] - Stulle
	if( pScrollBar == (CScrollBar*)GetDlgItem(IDC_PUSHSMALL_SLIDER) )
		ShowPushSmallFileValues();
	// <== push small files [sivka] - Stulle

	SetModified(TRUE);
	UpdateData(false); 
	CPropertyPage::OnHScroll(nSBCode, nPos, pScrollBar);
}

// ==> push small files [sivka] - Stulle
void CPPgScar::ShowPushSmallFileValues()
{
	// ==> Tabbed Preferences [TPT] - Stulle
	/*
	GetDlgItem(IDC_PUSHSMALL)->SetWindowText(CastItoXBytes(float(((CSliderCtrl*)GetDlgItem(IDC_PUSHSMALL_SLIDER))->GetPos())));
	*/
	m_iPushSmallLabel.SetWindowText(CastItoXBytes(float(m_iPushSmall.GetPos())));
	// <== Tabbed Preferences [TPT] - Stulle
}
// <== push small files [sivka] - Stulle

void CPPgScar::OnHelp()
{
	//theApp.ShowHelp(0);
}

BOOL CPPgScar::OnCommand(WPARAM wParam, LPARAM lParam)
{
	if (wParam == ID_HELP)
	{
		OnHelp();
		return TRUE;
	}
	return __super::OnCommand(wParam, lParam);
}

BOOL CPPgScar::OnHelpInfo(HELPINFO* /*pHelpInfo*/)
{
	OnHelp();
	return TRUE;
}

// ==> Tabbed Preferences [TPT] - Stulle
void CPPgScar::InitTab()
{
	// Clear all to be sure
	m_tabCtr.DeleteAllItems();
	
	// Change style
	// Remark: It seems that the multi-row can not be activated with the properties
	m_tabCtr.ModifyStyle(0, TCS_MULTILINE);

	// Add all items with icon
	m_tabCtr.SetImageList(&m_imageList);
	m_tabCtr.InsertItem(TCIF_TEXT | TCIF_IMAGE | TCIF_PARAM, SCAR, _T("ScarAngel"), 0, (LPARAM)SCAR);
	m_tabCtr.InsertItem(TCIF_TEXT | TCIF_IMAGE | TCIF_PARAM, WEBCACHE, _T("WebCache"), 1, (LPARAM)WEBCACHE);
	m_tabCtr.InsertItem(TCIF_TEXT | TCIF_IMAGE | TCIF_PARAM, BACKUP, _T("Backup"), 2, (LPARAM)BACKUP);
//	m_tabCtr.InsertItem(TCIF_TEXT | TCIF_IMAGE | TCIF_PARAM, UPDATE, _T("Update"), 3, (LPARAM)UPDATE);
//	m_tabCtr.InsertItem(TCIF_TEXT | TCIF_IMAGE | TCIF_PARAM, COLOR, _T("Color Settings"), 4, (LPARAM)COLOR);
}


void CPPgScar::InitControl()
{
	// Remark: don't use the dialog editor => avoid to merge rc

	// Retrieve the bottom of the tab's header
	RECT rect1;
	RECT rect2;
	m_tabCtr.GetWindowRect(&rect1);
	ScreenToClient(&rect1);
	m_tabCtr.GetItemRect(m_tabCtr.GetItemCount() - 1 , &rect2);
	const int top = rect1.top + (rect2.bottom - rect2.top + 1) * m_tabCtr.GetRowCount() + 10;
	const int left = rect1.left + 6;
	const int bottom = rect1.bottom-10;
	const int right = rect1.right - 6;

	// ScarAngel
	m_strWarning.CreateEx(0, _T("STATIC"), _T(""), 
							WS_CHILD /*| WS_VISIBLE*/, 
							CRect(left, top, right, top+39), this, IDC_WARNING);
	m_strWarning.SetFont(GetFont());	

	m_ctrlTreeOptions.MoveWindow(CRect(left, top+49, right, bottom-39),TRUE);

	m_strPushSmall.CreateEx(0, _T("STATIC"), _T("Push Small Files"), 
							WS_CHILD /*| WS_VISIBLE*/, 
							CRect(left, bottom-34, left+92, bottom-16), this, IDC_PUSHSMALL_LABEL);
	m_strPushSmall.SetFont(GetFont());	

	m_iPushSmallLabel.CreateEx(0, _T("STATIC"), _T("MB"), 
							WS_CHILD /*| WS_VISIBLE*/, 
							CRect(right-50, bottom-34, right, bottom-16), this, IDC_PUSHSMALL);
	m_iPushSmallLabel.SetFont(GetFont());	

	m_iPushSmall.CreateEx(WS_EX_STATICEDGE,
							  WS_CHILD /*| WS_VISIBLE*/ | WS_TABSTOP | WS_BORDER |
							  /*TBS_TOOLTIPS | */TBS_BOTH/* | TBS_VERT*/ | TBS_NOTICKS | WS_TABSTOP,
							  CRect(left, bottom-16, right, bottom), this, IDC_PUSHSMALL_SLIDER);
	m_iPushSmall.SetFont(GetFont());
	m_iPushSmall.SetRange(1, PARTSIZE);	


	// WebCache
	m_WcCtrlBox.CreateEx(0, _T("BUTTON"), _T("Webcache Controls"), 
						   WS_CHILD /*| WS_VISIBLE*/ | WS_TABSTOP |
						   BS_GROUPBOX,
						   CRect(left, top, right, top+50), this, IDC_STATIC_CONTROLS);
	m_WcCtrlBox.SetFont(GetFont());

	m_bWcDl.CreateEx(0, _T("BUTTON"), _T("Enable Webcached Downloads"), 
									WS_CHILD /*| WS_VISIBLE*/ | WS_TABSTOP |
									BS_AUTOCHECKBOX, 
									CRect(left+5, top+25, right-137, top+40), this, IDC_Activatewebcachedownloads);
	m_bWcDl.SetFont(GetFont());

	m_hideControls.CreateEx(0, _T("BUTTON"), _T("show advanced controls"), 
									WS_CHILD /*| WS_VISIBLE*/ | WS_TABSTOP |
									BS_FLAT,
									CRect(right-135, top+15, right-8, top+40), this, IDC_ADVANCEDCONTROLS);
	m_hideControls.SetFont(GetFont());

	m_WcProxyBox.CreateEx(0, _T("BUTTON"), _T("ISP Proxyserver Settings"), 
						   WS_CHILD /*| WS_VISIBLE*/ | WS_TABSTOP |
						   BS_GROUPBOX,
						   CRect(left, top+55, right, bottom), this, IDC_STATIC_CONTROLS);
	m_WcProxyBox.SetFont(GetFont());

	m_webcacheAddressStatic.CreateEx(0, _T("STATIC"), _T("Port"), 
						WS_CHILD /*| WS_VISIBLE*/, 
						CRect(left+10, top+73, left+65, top+88), this, IDC_STATIC_PORT);
	m_webcacheAddressStatic.SetFont(GetFont());

	m_webcacheAddressEdit.CreateEx(WS_EX_CLIENTEDGE, _T("EDIT"), _T(""), 
						WS_CHILD /*| WS_VISIBLE*/ | WS_TABSTOP |
						ES_LEFT | ES_AUTOHSCROLL, 
						CRect(left+70, top+71, right-15, top+90), this, IDC_webcacheName);
	m_webcacheAddressEdit.SetFont(GetFont());

	m_webcachePortStatic.CreateEx(0, _T("STATIC"), _T("Address"), 
						WS_CHILD /*| WS_VISIBLE*/, 
						CRect(left+10, top+98, left+65, top+113), this, IDC_STATIC_ADDRESS);
	m_webcachePortStatic.SetFont(GetFont());

	m_webcachePortEdit.CreateEx(WS_EX_CLIENTEDGE, _T("EDIT"), _T(""), 
						WS_CHILD /*| WS_VISIBLE*/ | WS_TABSTOP |
						ES_LEFT | ES_AUTOHSCROLL | ES_NUMBER, 
						CRect(left+70, top+96, left+120, top+115), this, IDC_webcachePort);
	m_webcachePortEdit.SetFont(GetFont());
	m_webcachePortEdit.SetLimitText(5);

	m_webcacheTest.CreateEx(0, _T("BUTTON"), _T("Test webcache configuration. Might take up to 30 Seconds"), 
									WS_CHILD /*| WS_VISIBLE*/ | WS_TABSTOP |
									BS_FLAT,
									CRect(left+7, top+118, right-7, top+140), this, IDC_TestProxy);
	m_webcacheTest.SetFont(GetFont());

	m_blockText.CreateEx(0, _T("STATIC"), _T("Number of blocks to download before reconnecting to the proxy (may help to solve disconnects in some cases)"), 
						WS_CHILD /*| WS_VISIBLE*/, 
						CRect(left+10, top+143, right-125, top+183), this, IDC_STATIC_NRBLOCKS);
	m_blockText.SetFont(GetFont());

	m_blockEdit.CreateEx(WS_EX_CLIENTEDGE, _T("EDIT"), _T(""), 
						WS_CHILD /*| WS_VISIBLE*/ | WS_TABSTOP |
						ES_LEFT | ES_AUTOHSCROLL | ES_NUMBER, 
						CRect(right-120, top+156, right-70, top+175), this, IDC_BLOCKS);
	m_blockEdit.SetFont(GetFont());
	m_blockEdit.SetLimitText(5);

	m_blockLabel.CreateEx(0, _T("STATIC"), _T("180k-Blocks"), 
						WS_CHILD /*| WS_VISIBLE*/, 
						CRect(right-65, top+158, right-10, top+173), this, IDC_STATIC_BLOCKS);
	m_blockLabel.SetFont(GetFont());

	m_TimeOut.CreateEx(0, _T("BUTTON"), _T("Extra long timeout for webcache connections"), 
									WS_CHILD /*| WS_VISIBLE*/ | WS_TABSTOP |
									BS_AUTOCHECKBOX, 
									CRect(left+7, top+184, right-7, top+199), this, IDC_EXTRATIMEOUT);
	m_TimeOut.SetFont(GetFont());

	m_CacheISP.CreateEx(0, _T("BUTTON"), _T("This webcache does NOT cache traffic within the same ISP"), 
									WS_CHILD /*| WS_VISIBLE*/ | WS_TABSTOP |
									BS_AUTOCHECKBOX, 
									CRect(left+7, top+201, right-7, top+216), this, IDC_LOCALTRAFFIC);
	m_CacheISP.SetFont(GetFont());

	m_WcDlPersistent.CreateEx(0, _T("BUTTON"), _T("Persistent connections for proxy downloads"), 
									WS_CHILD /*| WS_VISIBLE*/ | WS_TABSTOP |
									BS_AUTOCHECKBOX, 
									CRect(left+7, top+218, right-7, top+233), this, IDC_PERSISTENT_PROXY_CONNS);
	m_WcDlPersistent.SetFont(GetFont());

	m_Update.CreateEx(0, _T("BUTTON"), _T("Update webcachesettings automatically"), 
									WS_CHILD /*| WS_VISIBLE*/ | WS_TABSTOP |
									BS_AUTOCHECKBOX, 
									CRect(left+7, top+235, right-7, top+250), this, IDC_UPDATE_WCSETTINGS);
	m_Update.SetFont(GetFont());

	m_autoDetect.CreateEx(0, _T("BUTTON"), _T("Autodetect Webcache"), 
									WS_CHILD /*| WS_VISIBLE*/ | WS_TABSTOP |
									BS_FLAT,
									CRect(left+7, top+252, right-7, top+274), this, IDC_DETECTWEBCACHE);
	m_autoDetect.SetFont(GetFont());

	m_WrongPort.CreateEx(0, _T("STATIC"), _T("You are using a TCP port that is usually not cached by ISP proxy-servers. Please change your TCP port to  21, 70, 80, 210, 443 563  or 1025-65535 to be able to activate the webcache download!"), 
						WS_CHILD /*| WS_VISIBLE*/, 
						CRect(left+7, top+277, right-7, bottom-5), this, IDC_WrongPortWarning);
	m_WrongPort.SetFont(GetFont());

	// Backup
	m_BackupBox.CreateEx(0, _T("BUTTON"), _T("Select File Types to Backup"), 
						   WS_CHILD /*| WS_VISIBLE*/ | WS_TABSTOP |
						   BS_GROUPBOX,
						   CRect(left, top, right, top+120), this, IDC_BACKUP_FILEFRAME);
	m_BackupBox.SetFont(GetFont());

	m_Dat.CreateEx(0, _T("BUTTON"), _T("*.dat"), 
									WS_CHILD /*| WS_VISIBLE*/ | WS_TABSTOP |
									BS_AUTOCHECKBOX, 
									CRect(left+12, top+15, right-90, top+30), this, IDC_DAT);
	m_Dat.SetFont(GetFont());

	m_Met.CreateEx(0, _T("BUTTON"), _T("*.met"), 
									WS_CHILD /*| WS_VISIBLE*/ | WS_TABSTOP |
									BS_AUTOCHECKBOX, 
									CRect(left+12, top+35, right-90, top+50), this, IDC_MET);
	m_Met.SetFont(GetFont());

	m_Ini.CreateEx(0, _T("BUTTON"), _T("*.ini"), 
									WS_CHILD /*| WS_VISIBLE*/ | WS_TABSTOP |
									BS_AUTOCHECKBOX, 
									CRect(left+12, top+55, right-90, top+70), this, IDC_INI);
	m_Ini.SetFont(GetFont());

	m_Part.CreateEx(0, _T("BUTTON"), _T("*.part"), 
									WS_CHILD /*| WS_VISIBLE*/ | WS_TABSTOP |
									BS_AUTOCHECKBOX, 
									CRect(left+12, top+75, right-90, top+90), this, IDC_PART);
	m_Part.SetFont(GetFont());

	m_PartMet.CreateEx(0, _T("BUTTON"), _T("*.part.met"), 
									WS_CHILD /*| WS_VISIBLE*/ | WS_TABSTOP |
									BS_AUTOCHECKBOX, 
									CRect(left+12, top+95, right-90, top+110), this, IDC_PARTMET);
	m_PartMet.SetFont(GetFont());

	m_SelectAll.CreateEx(0, _T("BUTTON"), _T("Select All"), 
									WS_CHILD /*| WS_VISIBLE*/ | WS_TABSTOP |
									BS_FLAT | BS_DEFPUSHBUTTON,
									CRect(right-87, top+55, right-12, top+80), this, IDC_SELECTALL);
	m_SelectAll.SetFont(GetFont());

	m_BackupNow.CreateEx(0, _T("BUTTON"), _T("Backup Now"), 
									WS_CHILD /*| WS_VISIBLE*/ | WS_TABSTOP |
									BS_FLAT,
									CRect(right-87, top+85, right-12, top+110), this, IDC_BACKUPNOW);
	m_BackupNow.SetFont(GetFont());

	m_AutoBackupBox.CreateEx(0, _T("BUTTON"), _T("Auto Backup"), 
						   WS_CHILD /*| WS_VISIBLE*/ | WS_TABSTOP |
						   BS_GROUPBOX,
						   CRect(left, top+130, left+170, top+190), this, IDC_BACKUP_AUTO);
	m_AutoBackupBox.SetFont(GetFont());

	m_AutoBackup.CreateEx(0, _T("BUTTON"), _T("Auto Backup on Exit"), 
									WS_CHILD /*| WS_VISIBLE*/ | WS_TABSTOP |
									BS_AUTOCHECKBOX, 
									CRect(left+12, top+147, left+158, top+162), this, IDC_AUTOBACKUP);
	m_AutoBackup.SetFont(GetFont());

	m_AutoBackup2.CreateEx(0, _T("BUTTON"), _T("Double Backup"), 
									WS_CHILD /*| WS_VISIBLE*/ | WS_TABSTOP |
									BS_AUTOCHECKBOX, 
									CRect(left+22, top+165, left+158, top+180), this, IDC_AUTOBACKUP2);
	m_AutoBackup2.SetFont(GetFont());

	m_Note.CreateEx(0, _T("STATIC"), _T("Note:"),
						WS_CHILD /*| WS_VISIBLE*/, 
						CRect(left+10, top+200, right-10, top+215), this, IDC_BACKUP_NOTE);
	m_Note.SetFont(GetFont());

	m_NoteText.CreateEx(0, _T("STATIC"), _T(""), 
						WS_CHILD /*| WS_VISIBLE*/,
						CRect(left+10, top+220, right-10, bottom), this, IDC_BACKUP_MESSAGE);
	m_NoteText.SetFont(GetFont());
}

void CPPgScar::OnTabSelectionChange(NMHDR* /*pNMHDR*/, LRESULT *pResult)
{
	// Retrieve tab to display
	TCITEM tabCtrlItem; 
	tabCtrlItem.mask = TCIF_PARAM;
	if(m_tabCtr.GetItem(m_tabCtr.GetCurSel(), &tabCtrlItem) == TRUE){
		SetTab(static_cast<eTab>(tabCtrlItem.lParam));
	}

	*pResult = 0;
}

void CPPgScar::SetTab(eTab tab){
	if(m_currentTab != tab){
		// Hide all control
		switch(m_currentTab){
			case SCAR:
				m_strWarning.ShowWindow(SW_HIDE);
				m_strWarning.EnableWindow(FALSE);
				m_ctrlTreeOptions.ShowWindow(SW_HIDE);
				m_ctrlTreeOptions.EnableWindow(FALSE);
				m_strPushSmall.ShowWindow(SW_HIDE);
				m_strPushSmall.EnableWindow(FALSE);
				m_iPushSmallLabel.ShowWindow(SW_HIDE);
				m_iPushSmallLabel.EnableWindow(FALSE);
				m_iPushSmall.ShowWindow(SW_HIDE);
				m_iPushSmall.EnableWindow(FALSE);
				break;
			case WEBCACHE:
				m_WcCtrlBox.ShowWindow(SW_HIDE);
				m_WcCtrlBox.EnableWindow(FALSE);
				m_bWcDl.ShowWindow(SW_HIDE);
				m_bWcDl.EnableWindow(FALSE);
				m_hideControls.ShowWindow(SW_HIDE);
				m_hideControls.EnableWindow(FALSE);
				m_WcProxyBox.ShowWindow(SW_HIDE);
				m_WcProxyBox.EnableWindow(FALSE);
				m_webcacheAddressStatic.ShowWindow(SW_HIDE);
				m_webcacheAddressStatic.EnableWindow(FALSE);
				m_webcacheAddressEdit.ShowWindow(SW_HIDE);
				m_webcacheAddressEdit.EnableWindow(FALSE);
				m_webcachePortStatic.ShowWindow(SW_HIDE);
				m_webcachePortStatic.EnableWindow(FALSE);
				m_webcachePortEdit.ShowWindow(SW_HIDE);
				m_webcachePortEdit.EnableWindow(FALSE);
				m_webcacheTest.ShowWindow(SW_HIDE);
				m_webcacheTest.EnableWindow(FALSE);
				m_blockText.ShowWindow(SW_HIDE);
				m_blockText.EnableWindow(FALSE);
				m_blockEdit.ShowWindow(SW_HIDE);
				m_blockEdit.EnableWindow(FALSE);
				m_blockLabel.ShowWindow(SW_HIDE);
				m_blockLabel.EnableWindow(FALSE);
				m_TimeOut.ShowWindow(SW_HIDE);
				m_TimeOut.EnableWindow(FALSE);
				m_CacheISP.ShowWindow(SW_HIDE);
				m_CacheISP.EnableWindow(FALSE);
				m_WcDlPersistent.ShowWindow(SW_HIDE);
				m_WcDlPersistent.EnableWindow(FALSE);
				m_Update.ShowWindow(SW_HIDE);
				m_Update.EnableWindow(FALSE);
				m_autoDetect.ShowWindow(SW_HIDE);
				m_autoDetect.EnableWindow(FALSE);
				m_WrongPort.ShowWindow(SW_HIDE);
				m_WrongPort.EnableWindow(FALSE);
				break;
			case BACKUP:
				m_BackupBox.ShowWindow(SW_HIDE);
				m_BackupBox.EnableWindow(FALSE);
				m_Dat.ShowWindow(SW_HIDE);
				m_Dat.EnableWindow(FALSE);
				m_Met.ShowWindow(SW_HIDE);
				m_Met.EnableWindow(FALSE);
				m_Ini.ShowWindow(SW_HIDE);
				m_Ini.EnableWindow(FALSE);
				m_Part.ShowWindow(SW_HIDE);
				m_Part.EnableWindow(FALSE);
				m_PartMet.ShowWindow(SW_HIDE);
				m_PartMet.EnableWindow(FALSE);
				m_SelectAll.ShowWindow(SW_HIDE);
				m_SelectAll.EnableWindow(FALSE);
				m_BackupNow.ShowWindow(SW_HIDE);
				m_BackupNow.EnableWindow(FALSE);
				m_AutoBackupBox.ShowWindow(SW_HIDE);
				m_AutoBackupBox.EnableWindow(FALSE);
				m_AutoBackup.ShowWindow(SW_HIDE);
				m_AutoBackup.EnableWindow(FALSE);
				m_AutoBackup2.ShowWindow(SW_HIDE);
				m_AutoBackup2.EnableWindow(FALSE);
				m_Note.ShowWindow(SW_HIDE);
				m_Note.EnableWindow(FALSE);
				m_NoteText.ShowWindow(SW_HIDE);
				m_NoteText.ShowWindow(SW_HIDE);
				break;
/*			case UPDATE:
				break;
			case COLOR:
				break; */
			default:
				break;
		}

		// Show new controls
		m_currentTab = tab;
		switch(m_currentTab){
			case SCAR:
				m_strWarning.ShowWindow(SW_SHOW);
				m_strWarning.EnableWindow(TRUE);
				m_ctrlTreeOptions.ShowWindow(SW_SHOW);
				m_ctrlTreeOptions.EnableWindow(TRUE);
				m_strPushSmall.ShowWindow(SW_SHOW);
				m_strPushSmall.EnableWindow(TRUE);
				m_iPushSmallLabel.ShowWindow(SW_SHOW);
				m_iPushSmallLabel.EnableWindow(TRUE);
				m_iPushSmall.ShowWindow(SW_SHOW);
				m_iPushSmall.EnableWindow(TRUE);
				break;
			case WEBCACHE:
				m_WcCtrlBox.ShowWindow(SW_SHOW);
				m_WcCtrlBox.EnableWindow(TRUE);
				m_bWcDl.ShowWindow(SW_SHOW);
				m_bWcDl.EnableWindow(TRUE);
				m_hideControls.ShowWindow(SW_SHOW);
				m_WcProxyBox.ShowWindow(SW_SHOW);
				m_WcProxyBox.EnableWindow(TRUE);
				m_webcacheAddressStatic.ShowWindow(SW_SHOW);
				m_webcacheAddressEdit.ShowWindow(SW_SHOW);
				m_webcachePortStatic.ShowWindow(SW_SHOW);
				m_webcachePortEdit.ShowWindow(SW_SHOW);
				m_webcacheTest.ShowWindow(SW_SHOW);
				m_autoDetect.ShowWindow(SW_SHOW);
				if (showadvanced)
				{
					m_blockText.ShowWindow(SW_SHOW);
					m_blockEdit.ShowWindow(SW_SHOW);
					m_blockLabel.ShowWindow(SW_SHOW);
					m_TimeOut.ShowWindow(SW_SHOW);
					m_CacheISP.ShowWindow(SW_SHOW);
					m_WcDlPersistent.ShowWindow(SW_SHOW);
					m_Update.ShowWindow(SW_SHOW);
				}
				else
				{
					m_blockText.ShowWindow(SW_HIDE);
					m_blockEdit.ShowWindow(SW_HIDE);
					m_blockLabel.ShowWindow(SW_HIDE);
					m_TimeOut.ShowWindow(SW_HIDE);
					m_CacheISP.ShowWindow(SW_HIDE);
					m_WcDlPersistent.ShowWindow(SW_HIDE);
					m_Update.ShowWindow(SW_HIDE);
				}
				OnEnChangeActivatewebcachedownloads();
				break;
			case BACKUP:
				m_BackupBox.ShowWindow(SW_SHOW);
				m_BackupBox.EnableWindow(TRUE);
				m_Dat.ShowWindow(SW_SHOW);
				m_Dat.EnableWindow(TRUE);
				m_Met.ShowWindow(SW_SHOW);
				m_Met.EnableWindow(TRUE);
				m_Ini.ShowWindow(SW_SHOW);
				m_Ini.EnableWindow(TRUE);
				m_Part.ShowWindow(SW_SHOW);
				m_Part.EnableWindow(TRUE);
				m_PartMet.ShowWindow(SW_SHOW);
				m_PartMet.EnableWindow(TRUE);
				m_SelectAll.ShowWindow(SW_SHOW);
				m_SelectAll.EnableWindow(TRUE);
				m_BackupNow.ShowWindow(SW_SHOW);
				BackupNowEnable();
				m_AutoBackupBox.ShowWindow(SW_SHOW);
				m_AutoBackupBox.EnableWindow(TRUE);
				m_AutoBackup.ShowWindow(SW_SHOW);
				m_AutoBackup.EnableWindow(TRUE);
				m_AutoBackup2.ShowWindow(SW_SHOW);
				m_AutoBackup2.EnableWindow(m_AutoBackup.GetCheck() == BST_CHECKED);
				m_Note.ShowWindow(SW_SHOW);
				m_Note.EnableWindow(TRUE);
				m_NoteText.ShowWindow(SW_SHOW);
				m_NoteText.ShowWindow(TRUE);
				break;
/*			case UPDATE:
				break;
			case COLOR:
				break;
*/			default:
				break;
		}
	}
}
// <== Tabbed Preferences [TPT] - Stulle

// ==> WebCache [WC team/MorphXT] - Stulle/Max
void CPPgScar::OnEnChangeActivatewebcachedownloads(){
		
		if (guardian) return;

		guardian=true;

		SetModified();
		bool bTemp;

		if(thePrefs.UsesCachedTCPPort() == false)
		{
			m_bWcDl.SetCheck(false);
			thePrefs.webcacheEnabled=false;
			m_webcacheTest.EnableWindow(FALSE);
			if(m_currentTab == WEBCACHE)
			{
				m_WrongPort.ShowWindow(SW_SHOW);
				m_WrongPort.EnableWindow(TRUE);
			}
			bTemp = false;
		}
		else
		{
			m_webcacheTest.EnableWindow(true);
			bTemp = m_bWcDl.GetCheck() == BST_CHECKED;
			m_WrongPort.ShowWindow(SW_HIDE);
			m_WrongPort.EnableWindow(FALSE);
		}

		m_hideControls.EnableWindow(bTemp);
		m_webcacheAddressStatic.EnableWindow(bTemp);
		m_webcacheAddressEdit.EnableWindow(bTemp);
		m_webcachePortStatic.EnableWindow(bTemp);
		m_webcachePortEdit.EnableWindow(bTemp);
		m_autoDetect.EnableWindow(bTemp);
		m_blockText.EnableWindow(bTemp);
		m_blockEdit.EnableWindow(bTemp);
		m_blockLabel.EnableWindow(bTemp);
		m_TimeOut.EnableWindow(bTemp);
		m_CacheISP.EnableWindow(bTemp);
		m_WcDlPersistent.EnableWindow(bTemp);
		m_Update.EnableWindow(bTemp);

		guardian=false;
}

void CPPgScar::OnBnClickedDetectWebCache()
{
	WCInfo_Struct* detectedWebcache = new WCInfo_Struct();
	bool reaskedDNS;	// tells if a DNS reverse lookup has been performed during detection; unneeded since we don't show it anymore

	try
	{
		reaskedDNS=DetectWebCache(detectedWebcache);
	}
	catch(CString strError)
	{
		delete detectedWebcache;
		AfxMessageBox(strError ,MB_OK | MB_ICONINFORMATION,0);
		return;
	}
	catch (...)
	{
		delete detectedWebcache;
		AfxMessageBox(_T("Autodetection failed") ,MB_OK | MB_ICONINFORMATION,0);
		return;
	}

	CString comment = detectedWebcache->comment;
	for (int i=1; i*45 < comment.GetLength(); i++) // some quick-n-dirty beautifying  
		comment = comment.Left(i*45) + _T(" \n\t\t\t") + comment.Right(comment.GetLength() - i*45);

	CString message =	_T("Your ISP is:\t\t") + detectedWebcache->isp + _T(", ") + detectedWebcache->country + _T("\n") +
		_T("Your proxy name is:\t") + detectedWebcache->webcache + _T("\n") +
						_T("The proxy port is:\t\t") + detectedWebcache->port + _T("\n") +
						(comment != _T("") ? _T("comment: \t\t") + comment : _T(""));
	if (detectedWebcache->active == "0")
		message += _T("\n\ndue to detection results, webcache downloading has been deactivated;\nsee the comment for more details");

	if (AfxMessageBox(message, MB_OKCANCEL | MB_ICONINFORMATION,0) == IDCANCEL)
	{
		delete detectedWebcache;
		return;
	}

	m_bWcDl.SetCheck(detectedWebcache->active == "1");
	m_webcacheAddressEdit.SetWindowText(detectedWebcache->webcache);
	m_webcachePortEdit.SetWindowText(detectedWebcache->port);
	m_blockEdit.SetWindowText(detectedWebcache->blockLimit);
	m_TimeOut.SetCheck(detectedWebcache->active == "1");
	m_CacheISP.SetCheck(detectedWebcache->active == "0");
	m_WcDlPersistent.SetCheck(detectedWebcache->active == "1");

	delete detectedWebcache;
}

void CPPgScar::OnBnClickedAdvancedcontrols(){
	showadvanced = !showadvanced;
	if (showadvanced)
	{
		m_hideControls.SetWindowText(GetResString(IDS_WC_HIDE_ADV));
		m_TimeOut.ShowWindow(SW_SHOW);
		m_CacheISP.ShowWindow(SW_SHOW);
		m_blockEdit.ShowWindow(SW_SHOW);
		m_blockText.ShowWindow(SW_SHOW);
		m_blockLabel.ShowWindow(SW_SHOW);
		m_WcDlPersistent.ShowWindow(SW_SHOW);
		m_Update.ShowWindow(SW_SHOW);
	}
	else
	{
		m_hideControls.SetWindowText(GetResString(IDS_WC_ADVANCED));
		m_TimeOut.ShowWindow(SW_HIDE);
		m_CacheISP.ShowWindow(SW_HIDE);
		m_blockEdit.ShowWindow(SW_HIDE);
		m_blockText.ShowWindow(SW_HIDE);
		m_blockLabel.ShowWindow(SW_HIDE);
		m_WcDlPersistent.ShowWindow(SW_HIDE);
		m_Update.ShowWindow(SW_HIDE);
	}
}
//JP proxy configuration test
void CPPgScar::OnBnClickedTestProxy()
{
	if (thePrefs.IsWebCacheTestPossible())
	{
		if (!thePrefs.expectingWebCachePing)
		{
			//get webcache name from IDC_webcacheName
			CString cur_WebCacheName;
			m_webcacheAddressEdit.GetWindowText(cur_WebCacheName);
			if (cur_WebCacheName.GetLength() > 15 && cur_WebCacheName.Left(12) == "transparent@") //doesn't work for transparent proxies
			{
				AfxMessageBox(GetResString(IDC_MSG_WEBCACHE_TRANSPAR)); // leuk_he to rc.
				// Proxy Test can not test Transparent proxies. Test Canceled!
				return;
			}
			//get webcache port from IDC_webcachePort
			CString buffer;			
			m_webcachePortEdit.GetWindowText(buffer);
			uint16 cur_WebCachePort = (uint16)_tstol(buffer);
			if (PingviaProxy(cur_WebCacheName, cur_WebCachePort))
			{
				thePrefs.WebCachePingSendTime = ::GetTickCount();
				thePrefs.expectingWebCachePing = true;
				AfxMessageBox(GetResString(IDC_MSG_WEBCACHE_TESTRUNNING  )); // leuk_he to rc
				//Performing Proxy Test! Please check the log in the serverwindow for the results!
			}
			else
				AfxMessageBox(GetResString(IDC_MSG_WEBCACHE_TESTERR)); // leuk_he to rc
		}
		else 
			AfxMessageBox(GetResString(IDC_MSG_WEBCACHE_ALREADYRUNNING));// leuk_he to rc
	}
	else
		AfxMessageBox(GetResString(IDC_MSG_WEBCACHE_TESTREQ));// leuk_he to rc
}
// <== WebCache [WC team/MorphXT] - Stulle/Max

// ==> TBH: Backup [TBH/EastShare/MorphXT] - Stulle
void CPPgScar::BackupNowEnable()
{
	bool bTemp = (	m_Dat.GetCheck() == BST_CHECKED ||
					m_Met.GetCheck() == BST_CHECKED ||
					m_Ini.GetCheck() == BST_CHECKED ||
					m_Part.GetCheck() == BST_CHECKED ||
					m_PartMet.GetCheck() == BST_CHECKED
				 );
	m_BackupNow.EnableWindow(bTemp);
}

void CPPgScar::OnBnClickedDat()
{
	if (m_Dat.GetCheck() == BST_CHECKED) {
		m_BackupNow.EnableWindow(true);
	} else {
		if (m_Met.GetCheck() != BST_CHECKED && m_Ini.GetCheck() != BST_CHECKED && m_Part.GetCheck() != BST_CHECKED && m_PartMet.GetCheck() != BST_CHECKED)
			m_BackupNow.EnableWindow(false);
	}

}

void CPPgScar::OnBnClickedMet()
{
	if (m_Met.GetCheck() == BST_CHECKED) {
		m_BackupNow.EnableWindow(true);
	} else {
		if (m_Dat.GetCheck() != BST_CHECKED && m_Ini.GetCheck() != BST_CHECKED && m_Part.GetCheck() != BST_CHECKED && m_PartMet.GetCheck() != BST_CHECKED)
			m_BackupNow.EnableWindow(false);
	}

}

void CPPgScar::OnBnClickedIni()
{
	if (m_Ini.GetCheck() == BST_CHECKED) {
		m_BackupNow.EnableWindow(true);
	} else {
		if (m_Dat.GetCheck() != BST_CHECKED && m_Met.GetCheck() != BST_CHECKED && m_Part.GetCheck() != BST_CHECKED && m_PartMet.GetCheck() != BST_CHECKED)
			m_BackupNow.EnableWindow(false);
	}

}

void CPPgScar::OnBnClickedPart()
{
	if (m_Part.GetCheck() == BST_CHECKED) {
		m_BackupNow.EnableWindow(true);
	} else {
		if (m_Dat.GetCheck() != BST_CHECKED && m_Met.GetCheck() != BST_CHECKED && m_Ini.GetCheck() != BST_CHECKED && m_PartMet.GetCheck() != BST_CHECKED)
			m_BackupNow.EnableWindow(false);
	}

}

void CPPgScar::OnBnClickedPartMet()
{
	if (m_PartMet.GetCheck() == BST_CHECKED) {
		m_BackupNow.EnableWindow(true);
	} else {
		if (m_Dat.GetCheck() != BST_CHECKED && m_Met.GetCheck() != BST_CHECKED && m_Ini.GetCheck() != BST_CHECKED && m_Part.GetCheck() != BST_CHECKED)
			m_BackupNow.EnableWindow(false);
	}
}


void CPPgScar::OnBnClickedBackupnow()
{
	TCHAR buffer[200];
	y2All = FALSE;
	if (m_Dat.GetCheck() == BST_CHECKED)
	{
		Backup(_T("*.dat"), true);
		m_Dat.SetCheck(false);
	}

	if (m_Met.GetCheck() == BST_CHECKED)
	{
		Backup(_T("*.met"), true);
		m_Met.SetCheck(false);
	}

	if (m_Ini.GetCheck() == BST_CHECKED)
	{
		// Mighty Knife: Save current settings before backup
		theApp.emuledlg->SaveSettings (false);
		// [end] Mighty Knife
		Backup(_T("*.ini"), true);
		m_Ini.SetCheck(false);
	}

	if (m_PartMet.GetCheck() == BST_CHECKED)
	{
		Backup2(_T("*.part.met"));
		m_PartMet.SetCheck(false);
	}

	if (m_Part.GetCheck() == BST_CHECKED)
	{
		_stprintf(buffer,_T("Because of their size, backing up *.part files may take a few minutes.\nAre you sure you want to do this?"));
		if(MessageBox(buffer,_T("Are you sure?"),MB_ICONQUESTION|MB_YESNO)== IDYES)
			Backup2(_T("*.part"));
		m_Part.SetCheck(false);

	}

	BackupNowEnable();

	MessageBox(_T("File(s) Copied Successfully."), _T("BackUp complete."), MB_OK);
	y2All = FALSE;
}



void CPPgScar::Backup(LPCTSTR extensionToBack, BOOL conFirm)  
{
	WIN32_FIND_DATA FileData; 
	HANDLE hSearch; 
	TCHAR buffer[200];
	//CString szDirPath = CString(thePrefs.GetAppDir());
	CString szDirPath = CString(thePrefs.GetConfigDir());
	TCHAR szNewPath[MAX_PATH]; 

	SetCurrentDirectory(szDirPath);
	BOOL error = FALSE;
	BOOL OverWrite = TRUE;
	szDirPath +="Backup\\";

	BOOL fFinished = FALSE; 

	// Create a new directory if one does not exist
	if(!PathFileExists(szDirPath))
		CreateDirectory(szDirPath, NULL);

	// Start searching for files in the current directory. 

	hSearch = FindFirstFile(extensionToBack, &FileData); 
	if (hSearch == INVALID_HANDLE_VALUE) 
	{ 
		error = TRUE;
	} 

	// Copy each file to the new directory 
	CString str;
	while (!fFinished && !error) 
	{ 
		lstrcpy(szNewPath, szDirPath); 
		lstrcat(szNewPath, FileData.cFileName); 

		if(PathFileExists(szNewPath))
		{
			if (conFirm)
			{
				if (y2All == FALSE)
				{
					_stprintf(buffer, _T("File %s Already Exists. OverWrite It?"), FileData.cFileName);
					int rc = ::XMessageBox(m_hWnd,buffer,_T("OverWrite?"),MB_YESNO|MB_YESTOALL|MB_ICONQUESTION);
					if (rc == IDYES)
						OverWrite = TRUE;
					else if (rc == IDYESTOALL)
					{
						OverWrite = TRUE;
						y2All = TRUE;
					}
					else 
						OverWrite = FALSE;
				} else
					OverWrite = TRUE;
			} 
			else
				OverWrite = TRUE;
		}	
		if(OverWrite)
			CopyFile(FileData.cFileName, szNewPath, FALSE);

		if (!FindNextFile(hSearch, &FileData)) 
		{
			if (GetLastError() == ERROR_NO_MORE_FILES) 
			{ 
				//MessageBox("File Copied Successfully.", "BackUp complete", MB_OK); 
				fFinished = TRUE; 

			} 
			else 
			{ 
				error = TRUE;
			} 
		}

	} 


	// Close the search handle. 
	if (!FindClose(hSearch)) 
	{ 
		error = TRUE;
	} 
	if (error)
		MessageBox(_T("Error encountered during backup"),_T("Error"),MB_OK);
}


void CPPgScar::Backup2(LPCTSTR extensionToBack)  
{
	WIN32_FIND_DATA FileData;   
	HANDLE hSearch;   
	TCHAR buffer[200];  


	//CString szDirPath = CString(thePrefs.GetAppDir());  
	CString szDirPath = CString(thePrefs.GetConfigDir());
	CString szTempPath = CString(thePrefs.GetTempDir());  
	TCHAR szNewPath[MAX_PATH]; 

	BOOL fFinished = FALSE;     
	BOOL error = FALSE;  
	BOOL OverWrite = TRUE;  
	szDirPath +="Backup\\";

	if(!PathFileExists(szDirPath))  
		CreateDirectory(szDirPath, NULL);  

	szDirPath+="Temp\\";  

	if(!PathFileExists(szDirPath))  
		CreateDirectory(szDirPath, NULL);  


	// Start searching for files in the current directory.   
	SetCurrentDirectory(szTempPath);  

	hSearch = FindFirstFile(extensionToBack, &FileData);   

	if (hSearch == INVALID_HANDLE_VALUE)   
	{   
		error = TRUE;
	}   

	// Copy each file to the new directory   
	while (!fFinished && !error)   
	{   
		lstrcpy(szNewPath, szDirPath);   
		lstrcat(szNewPath, FileData.cFileName);   

		//MessageBox(szNewPath,"New Path",MB_OK);  
		if(PathFileExists(szNewPath))  
		{  
				if (y2All == FALSE)
				{
					_stprintf(buffer, _T("File %s Already Exists. OverWrite It?"), FileData.cFileName);
					int rc = ::XMessageBox(m_hWnd,buffer,_T("OverWrite?"),MB_YESNO|MB_YESTOALL|MB_ICONQUESTION);
					if (rc == IDYES)
						OverWrite = TRUE;
					else if (rc == IDYESTOALL)
					{
						OverWrite = TRUE;
						y2All = TRUE;
					}
					else 
						OverWrite = FALSE;
				} else
					OverWrite = TRUE;  
		}  

		if(OverWrite)  
			CopyFile(FileData.cFileName, szNewPath, FALSE);  

		if (!FindNextFile(hSearch, &FileData))   
		{  
			if (GetLastError() == ERROR_NO_MORE_FILES)   
			{   

				fFinished = TRUE;   
			}   
			else   
			{   
				error = TRUE;  
			}   
		}  

	}   

	// Close the search handle.   
	if (!FindClose(hSearch))   
	{   
		error = TRUE;  
	}   
	SetCurrentDirectory(CString(thePrefs.GetConfigDir()));  

	if (error)  
		MessageBox(_T("Error encountered during backup"),_T("Error"),MB_OK);  

} 

void CPPgScar::OnBnClickedSelectall()
{
	m_Dat.SetCheck(true);
	m_Met.SetCheck(true);
	m_Ini.SetCheck(true);
	m_Part.SetCheck(true);
	m_PartMet.SetCheck(true);
	m_BackupNow.EnableWindow(true);
}

void CPPgScar::OnBnClickedAutobackup()
{
	m_AutoBackup2.EnableWindow(m_AutoBackup.GetCheck() == BST_CHECKED);
	SetModified();
}

void CPPgScar::OnBnClickedAutobackup2()
{
	SetModified();
}

void CPPgScar::Backup3()
{
	WIN32_FIND_DATA FileData; 
	HANDLE hSearch; 
	CString szDirPath = CString(thePrefs.GetConfigDir())+_T("Backup\\");
	if(!PathFileExists(szDirPath)) return;
	TCHAR szNewPath[MAX_PATH]; 

	SetCurrentDirectory(szDirPath);
	BOOL error = FALSE;
	szDirPath = CString(thePrefs.GetConfigDir())+_T("Backup2\\");

	BOOL fFinished = FALSE; 

	// Create a new directory if one does not exist
	if(!PathFileExists(szDirPath))
		CreateDirectory(szDirPath, NULL);

	// Start searching for files in the current directory. 

	hSearch = FindFirstFile(_T("*.*"), &FileData); 
	if (hSearch == INVALID_HANDLE_VALUE) 
	{ 
		error = TRUE;
	} 

	// Copy each file to the new directory 
	while (!fFinished && !error) 
	{ 
		lstrcpy(szNewPath, szDirPath); 
		lstrcat(szNewPath, FileData.cFileName); 

		CopyFile(FileData.cFileName, szNewPath, FALSE);

		if (!FindNextFile(hSearch, &FileData)) 
		{
			if (GetLastError() == ERROR_NO_MORE_FILES) 
			{ 
				//MessageBox("File Copied Successfully.", "BackUp complete", MB_OK); 
				fFinished = TRUE; 

			} 
			else 
			{ 
				error = TRUE;
			} 
		}

	} 


	// Close the search handle. 
	if (!FindClose(hSearch)) 
	{ 
		error = TRUE;
	} 
	if (error)
		MessageBox(_T("Error encountered during backup"),_T("Error"),MB_OK);
}
// <== TBH: Backup [TBH/EastShare/MorphXT] - Stulle