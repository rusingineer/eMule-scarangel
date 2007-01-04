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
#include "sharedfilelist.h" // PowerShare [ZZ/MorphXT] - Stulle
// ==> Design Settings [eWombat/Stulle] - Stulle
#include "SharedFilesWnd.h"
#include "SharedFilesCtrl.h"
// <== Design Settings [eWombat/Stulle] - Stulle

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
	// ==> Design Settings [eWombat/Stulle] - Stulle
	ON_BN_CLICKED(IDC_COLOR_ON_OFF, OnBnClickedOnOff)
	ON_BN_CLICKED(IDC_COLOR_BOLD, OnBnClickedBold)
	ON_BN_CLICKED(IDC_COLOR_UNDERLINED, OnBnClickedUnderlined)
	ON_BN_CLICKED(IDC_COLOR_ITALIC, OnBnClickedItalic)
    ON_MESSAGE(UM_CPN_SELCHANGE, OnColorPopupSelChange)

	ON_CBN_SELCHANGE(IDC_COLOR_MASTER_COMBO, OnCbnSelchangeStyleselMaster)
	ON_CBN_SELCHANGE(IDC_COLOR_SUB_COMBO, OnCbnSelchangeStyleselSub)
	// <== Design Settings [eWombat/Stulle] - Stulle
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
	m_imageList.Add(CTempIconLoader(_T("SEARCHFILETYPE_PICTURE")));
//	m_imageList.Add(CTempIconLoader(_T("UPDATE")));
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
	// ==> Enforce Ratio [Stulle] - Stulle
	m_htiRatioGroup = NULL;
	m_htiEnforceRatio = NULL;
	m_htiRatioValue = NULL;
	// <== Enforce Ratio [Stulle] - Stulle
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
	m_htiShowGlobalHL = NULL; // show global HL - Stulle
	m_htiShowFileHLconst = NULL; // show HL per file constantaniously - Stulle
	m_htiShowInMSN7 = NULL; // Show in MSN7 [TPT] - Stulle
	m_htiQueueProgressBar = NULL; // Client queue progress bar [Commander] - Stulle
//	m_htiTrayComplete = NULL; // Completed in Tray - Stulle
	m_htiShowClientPercentage = NULL; // Show Client Percentage optional [Stulle] - Stulle

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

	// ==> Smart Category Control (SCC) [khaos/SiRoB/Stulle] - Stulle
	m_htiSCC = NULL;
	m_htiDlMode = NULL;
	m_htiDlNone = NULL;
	m_htiDlAlph = NULL;
	m_htiDlLP = NULL;
	m_htiShowCatNames = NULL;
	m_htiSelectCat = NULL;
	m_htiUseActiveCat = NULL;
	m_htiAutoSetResOrder = NULL;
	m_htiSmallFileDLPush = NULL;
	m_htiResumeFileInNewCat = NULL;
	m_htiUseAutoCat = NULL;
	// <== Smart Category Control (SCC) [khaos/SiRoB/Stulle] - Stulle

	m_htiSharedPrefs = NULL; // Shared Files Management [Stulle] - Stulle
	// ==> PowerShare [ZZ/MorphXT] - Stulle
	m_htiPowershareMode = NULL;
	m_htiPowershareDisabled = NULL;
	m_htiPowershareActivated = NULL;
	m_htiPowershareAuto = NULL;
	m_htiPowershareLimited = NULL;
	m_htiPowerShareLimit = NULL;
	// <== PowerShare [ZZ/MorphXT] - Stulle
	m_htiPsAmountLimit = NULL; // Limit PS by amount of data uploaded [Stulle] - Stulle
	// ==> Release Bonus [sivka] - Stulle
	m_htiReleaseBonusGroup = NULL;
	m_htiReleaseBonus0 = NULL;
	m_htiReleaseBonus1 = NULL;
	m_htiReleaseBonusDays = NULL;
	m_htiReleaseBonusDaysEdit = NULL;
	// <== Release Bonus [sivka] - Stulle
	m_htiReleaseScoreAssurance = NULL; // Release Score Assurance [Stulle] - Stulle
	m_htiSpreadBars = NULL; // Spread bars [Slugfiller/MorphXT] - Stulle
	// ==> HideOS & SOTN [Slugfiller/ MorphXT] - Stulle
	m_htiHideOS = NULL;
	m_htiSelectiveShare = NULL;
	m_htiShareOnlyTheNeed = NULL;
	// <== HideOS & SOTN [Slugfiller/ MorphXT] - Stulle

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
	m_htiCompressLevel = NULL; // Adjust Compress Level [Stulle] - Stulle
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
		int iImgRatio = 8;
		int iImgCS = 8;
		int iImgDisplay = 8;
		int iImgSysInfo = 8;
		int iImgDropDefaults = 8;
		int iImgMinimule = 8;
		int iImgSCC = 8;
		int iImgDlMode = 8;
		int iImgSharedPrefs = 8;
		int iImgPS = 8;
		int iImgReleaseBonus = 8;
		int iImgMisc = 8;
		int iImgGlobal = 8;

		CImageList* piml = m_ctrlTreeOptions.GetImageList(TVSIL_NORMAL);
		if (piml){
			iImgPush = piml->Add(CTempIconLoader(_T("SPEED")));
			iImgFunnyNick = piml->Add(CTempIconLoader(_T("FUNNYNICK")));
			iImgConTweaks =  piml->Add(CTempIconLoader(_T("CONNECTION")));
			iImgQuickstart = piml->Add(CTempIconLoader(_T("QUICKSTART"))); // Thx to the eF-Mod team for the icon
			iImgRatio = piml->Add(CTempIconLoader(_T("TRANSFERUPDOWN")));
			iImgCS = piml->Add(CTempIconLoader(_T("STATSCLIENTS")));
			iImgDisplay = piml->Add(CTempIconLoader(_T("DISPLAY")));
			iImgSysInfo = piml->Add(CTempIconLoader(_T("SYSINFO")));
			iImgDropDefaults = piml->Add(CTempIconLoader(_T("DROPDEFAULTS")));
			iImgMinimule = piml->Add(CTempIconLoader(_T("MINIMULE")));
			iImgSCC = piml->Add(CTempIconLoader(_T("CATEGORY")));
			iImgDlMode = piml->Add(CTempIconLoader(_T("DLMODE")));
			iImgSharedPrefs = piml->Add(CTempIconLoader(_T("SHAREDFILESMANAGEMENT")));
			iImgPS = piml->Add(CTempIconLoader(_T("FILEPOWERSHARE")));
			iImgReleaseBonus = piml->Add(CTempIconLoader(_T("RELEASEBONUS")));
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
		// ==> Enforce Ratio [Stulle] - Stulle
		m_htiRatioGroup = m_ctrlTreeOptions.InsertGroup(GetResString(IDS_RATIO_GROUP), iImgRatio, m_htiConTweaks);
		m_htiEnforceRatio = m_ctrlTreeOptions.InsertCheckBox(GetResString(IDS_ENFORCE_RATIO), m_htiRatioGroup, m_bEnforceRatio);
		m_htiRatioValue = m_ctrlTreeOptions.InsertItem(GetResString(IDS_RATIO_VALUE), TREEOPTSCTRLIMG_EDIT, TREEOPTSCTRLIMG_EDIT, m_htiRatioGroup);
		m_ctrlTreeOptions.AddEditBox(m_htiRatioValue, RUNTIME_CLASS(CNumTreeOptionsEdit));
		// <== Enforce Ratio [Stulle] - Stulle
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
		m_htiShowGlobalHL = m_ctrlTreeOptions.InsertCheckBox(GetResString(IDS_SHOW_GLOBAL_HL), m_htiDisplay, m_bShowGlobalHL); // show global HL - Stulle
		m_htiShowFileHLconst = m_ctrlTreeOptions.InsertCheckBox(GetResString(IDS_SHOW_FILE_HL_CONST), m_htiDisplay, m_bShowFileHLconst); // show HL per file constantaniously - Stulle
		m_htiShowInMSN7 = m_ctrlTreeOptions.InsertCheckBox(GetResString(IDS_SHOWINMSN7), m_htiDisplay, m_bShowInMSN7); // Show in MSN7 [TPT] - Stulle
		m_htiQueueProgressBar = m_ctrlTreeOptions.InsertCheckBox(GetResString(IDS_CLIENTQUEUEPROGRESSBAR), m_htiDisplay, m_bQueueProgressBar); // Client queue progress bar [Commander] - Stulle
//		m_htiTrayComplete = m_ctrlTreeOptions.InsertCheckBox(GetResString(IDS_TRAY_COMPLETE), m_htiDisplay, m_bTrayComplete); // Completed in Tray - Stulle
		m_htiShowClientPercentage = m_ctrlTreeOptions.InsertCheckBox(GetResString(IDS_CLIENTPERCENTAGE), m_htiDisplay, m_bShowClientPercentage); // Show Client Percentage optional [Stulle] - Stulle

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

		// ==> Smart Category Control (SCC) [khaos/SiRoB/Stulle] - Stulle
		m_htiSCC = m_ctrlTreeOptions.InsertGroup(GetResString(IDS_SCC), iImgSCC, TVI_ROOT);
		m_htiDlMode = m_ctrlTreeOptions.InsertGroup(GetResString(IDS_DL_MODE_DEFAULT), iImgDlMode, m_htiSCC);
		m_htiDlNone = m_ctrlTreeOptions.InsertRadioButton(GetResString(IDS_DL_NONE), m_htiDlMode, m_iDlMode == 0);
		m_htiDlAlph = m_ctrlTreeOptions.InsertRadioButton(GetResString(IDS_DOWNLOAD_ALPHABETICAL), m_htiDlMode, m_iDlMode == 1);
		m_htiDlLP = m_ctrlTreeOptions.InsertRadioButton(GetResString(IDS_LP), m_htiDlMode, m_iDlMode == 2);
		m_htiShowCatNames = m_ctrlTreeOptions.InsertCheckBox(GetResString(IDS_CAT_SHOWCATNAME), m_htiSCC, m_bShowCatNames);
		m_htiSelectCat = m_ctrlTreeOptions.InsertCheckBox(GetResString(IDS_CAT_SHOWSELCATDLG), m_htiSCC, m_bSelectCat);
		m_htiUseAutoCat = m_ctrlTreeOptions.InsertCheckBox(GetResString(IDS_CAT_USEAUTOCAT), m_htiSCC, m_bUseAutoCat);
		m_htiUseActiveCat = m_ctrlTreeOptions.InsertCheckBox(GetResString(IDS_CAT_USEACTIVE), m_htiSCC, m_bUseActiveCat);
		m_htiAutoSetResOrder = m_ctrlTreeOptions.InsertCheckBox(GetResString(IDS_CAT_AUTORESUMEORD), m_htiSCC, m_bAutoSetResOrder);
		m_htiSmallFileDLPush = m_ctrlTreeOptions.InsertCheckBox(GetResString(IDS_CAT_SMALLFILEDLPUSH), m_htiSCC, m_bSmallFileDLPush);
		m_htiResumeFileInNewCat = m_ctrlTreeOptions.InsertItem(GetResString(IDS_CAT_STARTFILESONADD), TREEOPTSCTRLIMG_EDIT, TREEOPTSCTRLIMG_EDIT, m_htiSCC);
		m_ctrlTreeOptions.AddEditBox(m_htiResumeFileInNewCat, RUNTIME_CLASS(CNumTreeOptionsEdit));
		// <== Smart Category Control (SCC) [khaos/SiRoB/Stulle] - Stulle

		m_htiSharedPrefs = m_ctrlTreeOptions.InsertGroup(GetResString(IDS_SHARED_PREFS), iImgSharedPrefs, TVI_ROOT); // Shared Files Management [Stulle] - Stulle
		// ==> PowerShare [ZZ/MorphXT] - Stulle
		m_htiPowershareMode = m_ctrlTreeOptions.InsertGroup(GetResString(IDS_POWERSHARE), iImgPS, m_htiSharedPrefs);
		m_htiPowershareDisabled = m_ctrlTreeOptions.InsertRadioButton(GetResString(IDS_POWERSHARE_DISABLED), m_htiPowershareMode, m_iPowershareMode == 0);
		m_htiPowershareActivated =  m_ctrlTreeOptions.InsertRadioButton(GetResString(IDS_POWERSHARE_ACTIVATED), m_htiPowershareMode, m_iPowershareMode == 1);
		m_htiPowershareAuto =  m_ctrlTreeOptions.InsertRadioButton(GetResString(IDS_POWERSHARE_AUTO), m_htiPowershareMode, m_iPowershareMode == 2);
		m_htiPowershareLimited =  m_ctrlTreeOptions.InsertRadioButton(GetResString(IDS_POWERSHARE_LIMITED), m_htiPowershareMode, m_iPowershareMode == 3);
		m_htiPowerShareLimit = m_ctrlTreeOptions.InsertItem(GetResString(IDS_POWERSHARE_LIMIT), TREEOPTSCTRLIMG_EDIT, TREEOPTSCTRLIMG_EDIT, m_htiPowershareLimited );
		m_ctrlTreeOptions.AddEditBox(m_htiPowerShareLimit, RUNTIME_CLASS(CNumTreeOptionsEdit));
		// <== PowerShare [ZZ/MorphXT] - Stulle
		// ==> Limit PS by amount of data uploaded [Stulle] - Stulle
		m_htiPsAmountLimit = m_ctrlTreeOptions.InsertItem(GetResString(IDS_PS_AMOUNT_LIMIT), TREEOPTSCTRLIMG_EDIT, TREEOPTSCTRLIMG_EDIT, m_htiPowershareLimited );
		m_ctrlTreeOptions.AddEditBox(m_htiPsAmountLimit, RUNTIME_CLASS(CNumTreeOptionsEdit));
		// <== Limit PS by amount of data uploaded [Stulle] - Stulle
		// ==> Release Bonus [sivka] - Stulle
		m_htiReleaseBonusGroup = m_ctrlTreeOptions.InsertGroup(GetResString(IDS_RELEASE_BONUS_GROUP), iImgReleaseBonus, m_htiMisc);
		m_htiReleaseBonus0 = m_ctrlTreeOptions.InsertRadioButton(GetResString(IDS_POWERSHARE_DISABLED), m_htiReleaseBonusGroup, m_iReleaseBonus == 0);
		m_htiReleaseBonus1 = m_ctrlTreeOptions.InsertRadioButton(_T("12h"), m_htiReleaseBonusGroup, m_iReleaseBonus == 1);
		m_htiReleaseBonusDays = m_ctrlTreeOptions.InsertRadioButton(GetResString(IDS_DAYS2), m_htiReleaseBonusGroup, m_iReleaseBonus == 2);
		m_htiReleaseBonusDaysEdit = m_ctrlTreeOptions.InsertItem(GetResString(IDS_RELEASE_BONUS_EDIT), TREEOPTSCTRLIMG_EDIT, TREEOPTSCTRLIMG_EDIT, m_htiReleaseBonusDays);
		m_ctrlTreeOptions.AddEditBox(m_htiReleaseBonusDaysEdit, RUNTIME_CLASS(CNumTreeOptionsEdit));
		m_ctrlTreeOptions.Expand(m_htiReleaseBonusDays, TVE_EXPAND);
		// <== Release Bonus [sivka] - Stulle
		m_htiSpreadBars = m_ctrlTreeOptions.InsertCheckBox(GetResString(IDS_SPREAD_BARS), m_htiSharedPrefs, m_bSpreadBars); // Spread bars [Slugfiller/MorphXT] - Stulle
		// ==> HideOS & SOTN [Slugfiller/ MorphXT] - Stulle
		m_htiHideOS = m_ctrlTreeOptions.InsertItem(GetResString(IDS_HIDEOVERSHARES), TREEOPTSCTRLIMG_EDIT, TREEOPTSCTRLIMG_EDIT, m_htiSpreadBars);
		m_ctrlTreeOptions.AddEditBox(m_htiHideOS, RUNTIME_CLASS(CNumTreeOptionsEdit));
		m_htiSelectiveShare = m_ctrlTreeOptions.InsertCheckBox(GetResString(IDS_SELECTIVESHARE), m_htiHideOS, m_bSelectiveShare);
		m_ctrlTreeOptions.Expand(m_htiSpreadBars, TVE_EXPAND);
		m_ctrlTreeOptions.Expand(m_htiHideOS, TVE_EXPAND);
		m_htiShareOnlyTheNeed = m_ctrlTreeOptions.InsertCheckBox(GetResString(IDS_SHAREONLYTHENEED), m_htiSharedPrefs, m_iShareOnlyTheNeed);
		// <== HideOS & SOTN [Slugfiller/ MorphXT] - Stulle
		m_htiReleaseScoreAssurance = m_ctrlTreeOptions.InsertCheckBox(GetResString(IDS_RELEASE_SCORE_ASSURANCE), m_htiReleaseBonusGroup, m_bReleaseScoreAssurance); // Release Score Assurance [Stulle] - Stulle

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
		// ==> Adjust Compress Level [Stulle] - Stulle
		m_htiCompressLevel = m_ctrlTreeOptions.InsertItem(GetResString(IDS_COMPRESS_LVL), TREEOPTSCTRLIMG_EDIT, TREEOPTSCTRLIMG_EDIT, m_htiMisc);
		m_ctrlTreeOptions.AddEditBox(m_htiCompressLevel, RUNTIME_CLASS(CNumTreeOptionsEdit));
		// <== Adjust Compress Level [Stulle] - Stulle

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
	// ==> Enforce Ratio [Stulle] - Stulle
	DDX_TreeCheck(pDX, IDC_SCAR_OPTS, m_htiEnforceRatio, m_bEnforceRatio);
	DDX_TreeEdit(pDX, IDC_SCAR_OPTS, m_htiRatioValue, m_iRatioValue);
	DDV_MinMaxInt(pDX, m_iRatioValue, 1, 4);
	// <== Enforce Ratio [Stulle] - Stulle
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
	DDX_TreeCheck(pDX, IDC_SCAR_OPTS, m_htiShowGlobalHL, m_bShowGlobalHL); // show global HL - Stulle
	DDX_TreeCheck(pDX, IDC_SCAR_OPTS, m_htiShowFileHLconst, m_bShowFileHLconst); // show HL per file constantaniously - Stulle
	DDX_TreeCheck(pDX, IDC_SCAR_OPTS, m_htiShowInMSN7, m_bShowInMSN7); // Show in MSN7 [TPT] - Stulle
	DDX_TreeCheck(pDX, IDC_SCAR_OPTS, m_htiQueueProgressBar, m_bQueueProgressBar); // Client queue progress bar [Commander] - Stulle
//	DDX_TreeCheck(pDX, IDC_SCAR_OPTS, m_htiTrayComplete, m_bTrayComplete); // Completed in Tray - Stulle
	DDX_TreeCheck(pDX, IDC_SCAR_OPTS, m_htiShowClientPercentage, m_bShowClientPercentage); // Show Client Percentage optional [Stulle] - Stulle

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

	// ==> Smart Category Control (SCC) [khaos/SiRoB/Stulle] - Stulle
	DDX_TreeRadio(pDX, IDC_SCAR_OPTS, m_htiDlMode, (int &)m_iDlMode);
	DDX_TreeEdit(pDX, IDC_SCAR_OPTS, m_htiResumeFileInNewCat, m_iResumeFileInNewCat);
	DDX_TreeCheck(pDX, IDC_SCAR_OPTS, m_htiShowCatNames, m_bShowCatNames);
	DDX_TreeCheck(pDX, IDC_SCAR_OPTS, m_htiSelectCat, m_bSelectCat);
	DDX_TreeCheck(pDX, IDC_SCAR_OPTS, m_htiUseActiveCat, m_bUseActiveCat);
	DDX_TreeCheck(pDX, IDC_SCAR_OPTS, m_htiAutoSetResOrder, m_bAutoSetResOrder);
	DDX_TreeCheck(pDX, IDC_SCAR_OPTS, m_htiSmallFileDLPush, m_bSmallFileDLPush);
	DDX_TreeCheck(pDX, IDC_SCAR_OPTS, m_htiUseAutoCat, m_bUseAutoCat);
	// <== Smart Category Control (SCC) [khaos/SiRoB/Stulle] - Stulle

	// ==> PowerShare [ZZ/MorphXT] - Stulle
	DDX_TreeEdit(pDX, IDC_SCAR_OPTS, m_htiPowerShareLimit, m_iPowerShareLimit);
	DDV_MinMaxInt(pDX, m_iShareOnlyTheNeed, 0, INT_MAX);
	DDX_TreeRadio(pDX, IDC_SCAR_OPTS, m_htiPowershareMode, m_iPowershareMode);
	// <== PowerShare [ZZ/MorphXT] - Stulle
	// ==> Limit PS by amount of data uploaded [Stulle] - Stulle
	DDX_TreeEdit(pDX, IDC_SCAR_OPTS, m_htiPsAmountLimit, m_iPsAmountLimit);
	DDV_MinMaxInt(pDX, m_iPsAmountLimit, 0, MAX_PS_AMOUNT_LIMIT);
	// <== Limit PS by amount of data uploaded [Stulle] - Stulle
	// ==> Release Bonus [sivka] - Stulle
	DDX_TreeRadio(pDX, IDC_SCAR_OPTS, m_htiReleaseBonusGroup, m_iReleaseBonus);
	DDX_TreeEdit(pDX, IDC_SCAR_OPTS, m_htiReleaseBonusDaysEdit, m_iReleaseBonusDays);
	DDV_MinMaxInt(pDX, m_iReleaseBonusDays, 1, 16);
	// <== Release Bonus [sivka] - Stulle
	DDX_TreeCheck(pDX, IDC_SCAR_OPTS, m_htiSpreadBars, m_bSpreadBars); // Spread bars [Slugfiller/MorphXT] - Stulle
	// ==> HideOS & SOTN [Slugfiller/ MorphXT] - Stulle
	DDX_TreeEdit(pDX, IDC_SCAR_OPTS, m_htiHideOS, m_iHideOS);
	DDV_MinMaxInt(pDX, m_iHideOS, 0, INT_MAX);
	DDX_TreeCheck(pDX, IDC_SCAR_OPTS, m_htiSelectiveShare, m_bSelectiveShare);
	DDX_TreeCheck(pDX, IDC_SCAR_OPTS, m_htiShareOnlyTheNeed, m_iShareOnlyTheNeed);
	// <== HideOS & SOTN [Slugfiller/ MorphXT] - Stulle
	DDX_TreeCheck(pDX, IDC_SCAR_OPTS, m_htiReleaseScoreAssurance, m_bReleaseScoreAssurance); // Release Score Assurance [Stulle] - Stulle

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
	// ==> Adjust Compress Level [Stulle] - Stulle
	DDX_TreeEdit(pDX, IDC_SCAR_OPTS, m_htiCompressLevel, m_iCompressLevel);
	DDV_MinMaxInt(pDX, m_iCompressLevel, 0, 9);
	// <== Adjust Compress Level [Stulle] - Stulle

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
	// ==> Enforce Ratio [Stulle] - Stulle
	m_bEnforceRatio = thePrefs.GetEnforceRatio();
	m_iRatioValue = (int)thePrefs.GetRatioValue();
	// <== Enforce Ratio [Stulle] - Stulle
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
	m_bShowGlobalHL = thePrefs.GetShowGlobalHL(); // show global HL - Stulle
	m_bShowFileHLconst = thePrefs.GetShowFileHLconst(); // show HL per file constantaniously - Stulle
	m_bShowInMSN7 = thePrefs.GetShowMSN7(); // Show in MSN7 [TPT] - Stulle
	m_bQueueProgressBar = thePrefs.ShowClientQueueProgressBar(); // Client queue progress bar [Commander] - Stulle
//	m_bTrayComplete = thePrefs.GetTrayComplete(); // Completed in Tray - Stulle
	m_bShowClientPercentage = thePrefs.GetShowClientPercentage(); // Show Client Percentage optional [Stulle] - Stulle

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

	// ==> Smart Category Control (SCC) [khaos/SiRoB/Stulle] - Stulle
	m_iDlMode = thePrefs.GetDlMode();
	m_bShowCatNames = thePrefs.ShowCatNameInDownList();
	m_bSelectCat = thePrefs.SelectCatForNewDL();
	m_bUseActiveCat = thePrefs.UseActiveCatForLinks();
	m_bAutoSetResOrder = thePrefs.AutoSetResumeOrder();
	m_bSmallFileDLPush = thePrefs.SmallFileDLPush();
	m_iResumeFileInNewCat = thePrefs.StartDLInEmptyCats();
	m_bUseAutoCat = thePrefs.UseAutoCat();
	// <== Smart Category Control (SCC) [khaos/SiRoB/Stulle] - Stulle

	// ==> PowerShare [ZZ/MorphXT] - Stulle
	m_iPowershareMode = thePrefs.m_iPowershareMode;
	m_iPowerShareLimit = thePrefs.PowerShareLimit;
	// <== PowerShare [ZZ/MorphXT] - Stulle
	m_iPsAmountLimit = thePrefs.GetPsAmountLimit(); // Limit PS by amount of data uploaded [Stulle] - Stulle
	// ==> Release Bonus [sivka] - Stulle
	if (thePrefs.GetReleaseBonus() <= 1)
	{
		m_iReleaseBonus = thePrefs.GetReleaseBonus();
		m_iReleaseBonusDays = 1;
	}
	else
	{
		m_iReleaseBonus = 2;
		m_iReleaseBonusDays = (int)(thePrefs.GetReleaseBonus()/2);
	}
	// <== Release Bonus [sivka] - Stulle
	m_bSpreadBars = thePrefs.GetSpreadbarSetStatus(); // Spread bars [Slugfiller/MorphXT] - Stulle
	// ==> HideOS & SOTN [Slugfiller/ MorphXT] - Stulle
	m_iHideOS = thePrefs.hideOS;
	m_bSelectiveShare = thePrefs.selectiveShare;
	m_iShareOnlyTheNeed = thePrefs.ShareOnlyTheNeed;
	// <== HideOS & SOTN [Slugfiller/ MorphXT] - Stulle
	m_bReleaseScoreAssurance = thePrefs.GetReleaseScoreAssurance(); // Release Score Assurance [Stulle] - Stulle

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
	m_iCompressLevel = thePrefs.GetCompressLevel(); // Adjust Compress Level [Stulle] - Stulle

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

	// ==> Design Settings [eWombat/Stulle] - Stulle
	m_bDesignChanged = false;
	m_bold.SetWindowText(_T(""));
	m_bold.SetIcon((HICON)::LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE((int)IDI_FONTB), IMAGE_ICON, 16, 16, 0));
	m_underlined.SetWindowText(_T(""));
	m_underlined.SetIcon((HICON)::LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE((int)IDI_FONTUL), IMAGE_ICON, 16, 16, 0));
	m_italic.SetWindowText(_T(""));
	m_italic.SetIcon((HICON)::LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE((int)IDI_FONTI), IMAGE_ICON, 16, 16, 0));
	m_bold.SetFont(theApp.GetBoldFont());
	m_underlined.SetFont(theApp.GetULFont());
	m_italic.SetFont(theApp.GetItalicFont());

	m_FontColor.SetColor(GetSysColor(COLOR_WINDOWTEXT));
	m_FontColor.SetDefaultColor(GetSysColor(COLOR_WINDOWTEXT));
	m_BackColor.SetColor(COLORREF(RGB(255,255,255)));
	m_BackColor.SetDefaultColor(COLORREF(RGB(255,255,255)));

//	m_ColorPreview.SetBackgroundColor(GetSysColor(COLOR_WINDOW));
//	m_ColorPreview.SetTextColor(GetSysColor(COLOR_WINDOWTEXT));
	// <== Design Settings [eWombat/Stulle] - Stulle

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

		// ==> Design Settings [eWombat/Stulle] - Stulle
		for (int i=0;i<style_counts;i++)
			thePrefs.GetStyle(i,&styles[i]);
		// <== Design Settings [eWombat/Stulle] - Stulle
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
	// ==> Enforce Ratio [Stulle] - Stulle
	thePrefs.m_bEnforceRatio = m_bEnforceRatio;
	thePrefs.m_uRatioValue = (uint8) m_iRatioValue;
	// <== Enforce Ratio [Stulle] - Stulle
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
	thePrefs.ShowGlobalHL = m_bShowGlobalHL; // show global HL - Stulle
	thePrefs.ShowFileHLconst = m_bShowFileHLconst; // show HL per file constantaniously - Stulle
	thePrefs.m_bShowInMSN7 = m_bShowInMSN7; // Show in MSN7 [TPT] - Stulle
	thePrefs.m_bClientQueueProgressBar = m_bQueueProgressBar; // Client queue progress bar [Commander] - Stulle
//	thePrefs.m_bTrayComplete = m_bTrayComplete; // Completed in Tray - Stulle
	thePrefs.m_bShowClientPercentage = m_bShowClientPercentage; // Show Client Percentage optional [Stulle] - Stulle

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

	// ==> Smart Category Control (SCC) [khaos/SiRoB/Stulle] - Stulle
	thePrefs.dlMode = (uint8)m_iDlMode;
	thePrefs.m_bShowCatNames = m_bShowCatNames;
	thePrefs.m_bSelCatOnAdd = m_bSelectCat;
	thePrefs.m_bActiveCatDefault = m_bUseActiveCat;
	thePrefs.m_bAutoSetResumeOrder = m_bAutoSetResOrder;
	thePrefs.m_bSmallFileDLPush = m_bSmallFileDLPush;
	thePrefs.m_iStartDLInEmptyCats = (uint8)m_iResumeFileInNewCat;
	thePrefs.m_bUseAutoCat = m_bUseAutoCat;
	// <== Smart Category Control (SCC) [khaos/SiRoB/Stulle] - Stulle

	// ==> PowerShare [ZZ/MorphXT] - Stulle
	thePrefs.m_iPowershareMode = m_iPowershareMode;
	thePrefs.PowerShareLimit = m_iPowerShareLimit;
	theApp.sharedfiles->UpdatePartsInfo();
	// <== PowerShare [ZZ/MorphXT] - Stulle
	// ==> Limit PS by amount of data uploaded [Stulle] - Stulle
	thePrefs.PsAmountLimit = m_iPsAmountLimit;
	// <== Limit PS by amount of data uploaded [Stulle] - Stulle
	// ==> Release Bonus [sivka] - Stulle
	if (m_iReleaseBonus <= 1)
        thePrefs.m_uReleaseBonus = (uint8)m_iReleaseBonus;
	else
		thePrefs.m_uReleaseBonus = (uint8)(m_iReleaseBonusDays*2);
	// <== Release Bonus [sivka] - Stulle
	thePrefs.m_bSpreadbarSetStatus = m_bSpreadBars; // Spread bars [Slugfiller/MorphXT] - Stulle
	// ==> HideOS & SOTN [Slugfiller/ MorphXT] - Stulle
	thePrefs.hideOS = m_iHideOS;
	thePrefs.selectiveShare = m_bSelectiveShare;
	thePrefs.ShareOnlyTheNeed = m_iShareOnlyTheNeed!=0;
	// <== HideOS & SOTN [Slugfiller/ MorphXT] - Stulle
	thePrefs.m_bReleaseScoreAssurance = m_bReleaseScoreAssurance; // Release Score Assurance [Stulle] - Stulle

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
	// ==> Adjust Compress Level [Stulle] - Stulle
	thePrefs.m_uCompressLevel = (uint8)m_iCompressLevel;
	if(m_iCompressLevel == 0)
		thePrefs.m_bUseCompression = false;
	// <=== Adjust Compress Level [Stulle] - Stulle

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

	// ==> Design Settings [eWombat/Stulle] - Stulle
	for (int i=0;i<style_counts;i++)
	{
		thePrefs.SetStyle(i,&styles[i]);
	}
	if(m_bDesignChanged)
	{
		m_bDesignChanged = false;
		theApp.emuledlg->transferwnd->Localize();
		theApp.emuledlg->sharedfileswnd->sharedfilesctrl.Localize();
		AddLogLine(false,_T("Design Settings changed"));
	}
	// <== Design Settings [eWombat/Stulle] - Stulle

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
		// ==> Enforce Ratio [Stulle] - Stulle
		if (m_htiEnforceRatio) m_ctrlTreeOptions.SetItemText(m_htiEnforceRatio, GetResString(IDS_ENFORCE_RATIO));
		if (m_htiRatioValue) m_ctrlTreeOptions.SetEditLabel(m_htiRatioValue, GetResString(IDS_RATIO_VALUE));
		// <== Enforce Ratio [Stulle] - Stulle
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
		if (m_htiShowGlobalHL) m_ctrlTreeOptions.SetItemText(m_htiShowGlobalHL, GetResString(IDS_SHOW_GLOBAL_HL)); // show global HL - Stulle
		if (m_htiShowFileHLconst) m_ctrlTreeOptions.SetItemText(m_htiShowFileHLconst, GetResString(IDS_SHOW_FILE_HL_CONST)); // show HL per file constantaniously - Stulle
		if (m_htiShowInMSN7) m_ctrlTreeOptions.SetItemText(m_htiShowInMSN7, GetResString(IDS_SHOWINMSN7)); // Show in MSN7 [TPT] - Stulle
		if (m_htiQueueProgressBar) m_ctrlTreeOptions.SetItemText(m_htiQueueProgressBar, GetResString(IDS_CLIENTQUEUEPROGRESSBAR)); // Client queue progress bar [Commander] - Stulle
//		if (m_htiTrayComplete) m_ctrlTreeOptions.SetItemText(m_htiTrayComplete, GetResString(IDS_TRAY_COMPLETE)); // Completed in Tray - Stulle
		if (m_htiShowClientPercentage) m_ctrlTreeOptions.SetItemText(m_htiShowClientPercentage, GetResString(IDS_CLIENTPERCENTAGE)); // Show Client Percentage optional [Stulle] - Stulle

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

		// ==> Smart Category Control (SCC) [khaos/SiRoB/Stulle] - Stulle
		if (m_htiShowCatNames) m_ctrlTreeOptions.SetItemText(m_htiShowCatNames, GetResString(IDS_CAT_SHOWCATNAME));
		if (m_htiSelectCat) m_ctrlTreeOptions.SetItemText(m_htiSelectCat, GetResString(IDS_CAT_SHOWSELCATDLG));
		if (m_htiUseAutoCat) m_ctrlTreeOptions.SetItemText(m_htiUseAutoCat, GetResString(IDS_CAT_USEAUTOCAT));
        if (m_htiUseActiveCat) m_ctrlTreeOptions.SetItemText(m_htiUseActiveCat, GetResString(IDS_CAT_USEACTIVE));
		if (m_htiAutoSetResOrder) m_ctrlTreeOptions.SetItemText(m_htiAutoSetResOrder, GetResString(IDS_CAT_AUTORESUMEORD));
		if (m_htiSmallFileDLPush) m_ctrlTreeOptions.SetItemText(m_htiSmallFileDLPush, GetResString(IDS_CAT_SMALLFILEDLPUSH));
		if (m_htiResumeFileInNewCat) m_ctrlTreeOptions.SetEditLabel(m_htiResumeFileInNewCat, GetResString(IDS_CAT_STARTFILESONADD));
		// <== Smart Category Control (SCC) [khaos/SiRoB/Stulle] - Stulle

		// ==> PowerShare [ZZ/MorphXT] - Stulle
		if (m_htiPowershareMode) m_ctrlTreeOptions.SetItemText(m_htiPowershareMode, GetResString(IDS_POWERSHARE));
		if (m_htiPowershareDisabled) m_ctrlTreeOptions.SetItemText(m_htiPowershareDisabled, GetResString(IDS_POWERSHARE_DISABLED));
		if (m_htiPowershareActivated) m_ctrlTreeOptions.SetItemText(m_htiPowershareActivated, GetResString(IDS_POWERSHARE_ACTIVATED));
		if (m_htiPowershareAuto) m_ctrlTreeOptions.SetItemText(m_htiPowershareAuto, GetResString(IDS_POWERSHARE_AUTO));
		if (m_htiPowershareLimited) m_ctrlTreeOptions.SetItemText(m_htiPowershareLimited, GetResString(IDS_POWERSHARE_LIMITED));
		if (m_htiPowerShareLimit) m_ctrlTreeOptions.SetEditLabel(m_htiPowerShareLimit, GetResString(IDS_POWERSHARE_LIMIT));
		// <== PowerShare [ZZ/MorphXT] - Stulle
		// ==> Limit PS by amount of data uploaded [Stulle] - Stulle
		if (m_htiPsAmountLimit) m_ctrlTreeOptions.SetEditLabel(m_htiPsAmountLimit, GetResString(IDS_PS_AMOUNT_LIMIT));
		// <== Limit PS by amount of data uploaded [Stulle] - Stulle
		if (m_htiSpreadBars) m_ctrlTreeOptions.SetItemText(m_htiSpreadBars, GetResString(IDS_SPREAD_BARS)); // Spread bars [Slugfiller/MorphXT] - Stulle
		// ==> HideOS & SOTN [Slugfiller/ MorphXT] - Stulle
		if (m_htiHideOS) m_ctrlTreeOptions.SetEditLabel(m_htiHideOS, GetResString(IDS_HIDEOVERSHARES));
		if (m_htiSelectiveShare) m_ctrlTreeOptions.SetItemText(m_htiSelectiveShare, GetResString(IDS_SELECTIVESHARE));
		if (m_htiShareOnlyTheNeed) m_ctrlTreeOptions.SetItemText(m_htiShareOnlyTheNeed, GetResString(IDS_SHAREONLYTHENEED));
		// <== HideOS & SOTN [Slugfiller/ MorphXT] - Stulle
		if (m_htiReleaseScoreAssurance) m_ctrlTreeOptions.SetItemText(m_htiReleaseScoreAssurance, GetResString(IDS_RELEASE_SCORE_ASSURANCE)); // Release Score Assurance [Stulle] - Stulle

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
		if (m_htiCompressLevel) m_ctrlTreeOptions.SetEditLabel(m_htiCompressLevel, GetResString(IDS_COMPRESS_LVL)); // Adjust Compress Level [Stulle] - Stulle

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

		// ==> Design Settings [eWombat/Stulle] - Stulle
		m_ColorBox.SetWindowText( GetResString(IDS_COLOR_BOX) );
		m_FontColorLabel.SetWindowText( GetResString(IDS_COLOR_FONT_LABEL) );
		m_BackColorLabel.SetWindowText( GetResString(IDS_COLOR_BACK_LABEL) );
//		m_ColorPreviewBox.SetWindowText( GetResString(IDS_COLOR_PREVIEW) );

		m_FontColor.CustomText = GetResString(IDS_COL_MORECOLORS);
		m_FontColor.DefaultText = GetResString(IDS_DEFAULT);
		m_BackColor.CustomText = GetResString(IDS_COL_MORECOLORS);
		m_BackColor.DefaultText = GetResString(IDS_DEFAULT);

		InitMasterStyleCombo();
		// <== Design Settings [eWombat/Stulle] - Stulle
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
	// ==> Enforce Ratio [Stulle] - Stulle
	m_htiRatioGroup = NULL;
	m_htiEnforceRatio = NULL;
	m_htiRatioValue = NULL;
	// <== Enforce Ratio [Stulle] - Stulle
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
	m_htiShowGlobalHL = NULL; // show global HL - Stulle
	m_htiShowFileHLconst = NULL; // show HL per file constantaniously - Stulle
	m_htiShowInMSN7 = NULL; // Show in MSN7 [TPT] - Stulle
	m_htiQueueProgressBar = NULL;
//	m_htiTrayComplete = NULL; // Completed in Tray - Stulle
	m_htiShowClientPercentage = NULL; // Show Client Percentage optional [Stulle] - Stulle

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

	// ==> Smart Category Control (SCC) [khaos/SiRoB/Stulle] - Stulle
	m_htiSCC = NULL;
	m_htiDlMode = NULL;
	m_htiDlNone = NULL;
	m_htiDlAlph = NULL;
	m_htiDlLP = NULL;
	m_htiShowCatNames = NULL;
	m_htiSelectCat = NULL;
	m_htiUseActiveCat = NULL;
	m_htiAutoSetResOrder = NULL;
	m_htiSmallFileDLPush = NULL;
	m_htiResumeFileInNewCat = NULL;
	m_htiUseAutoCat = NULL;
	// <== Smart Category Control (SCC) [khaos/SiRoB/Stulle] - Stulle

	m_htiSharedPrefs = NULL; // Shared Files Management [Stulle] - Stulle
	// ==> PowerShare [ZZ/MorphXT] - Stulle
	m_htiPowershareMode = NULL;
	m_htiPowershareDisabled = NULL;
	m_htiPowershareActivated = NULL;
	m_htiPowershareAuto = NULL;
	m_htiPowershareLimited = NULL;
	m_htiPowerShareLimit = NULL;
	// <== PowerShare [ZZ/MorphXT] - Stulle
	m_htiPsAmountLimit = NULL; // Limit PS by amount of data uploaded [Stulle] - Stulle
	// ==> Release Bonus [sivka] - Stulle
	m_htiReleaseBonusGroup = NULL;
	m_htiReleaseBonus0 = NULL;
	m_htiReleaseBonus1 = NULL;
	m_htiReleaseBonusDays = NULL;
	m_htiReleaseBonusDaysEdit = NULL;
	// <== Release Bonus [sivka] - Stulle
	m_htiSpreadBars = NULL; // Spread bars [Slugfiller/MorphXT] - Stulle
	// ==> HideOS & SOTN [Slugfiller/ MorphXT] - Stulle
	m_htiHideOS = NULL;
	m_htiSelectiveShare = NULL;
	m_htiShareOnlyTheNeed = NULL;
	// <== HideOS & SOTN [Slugfiller/ MorphXT] - Stulle
	m_htiReleaseScoreAssurance = NULL; // Release Score Assurance [Stulle] - Stulle

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
	m_htiCompressLevel = NULL; // Adjust Compress Level [Stulle] - Stulle

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
	m_tabCtr.InsertItem(TCIF_TEXT | TCIF_IMAGE | TCIF_PARAM, SCAR, GetResString(IDS_SCARANGEL), 0, (LPARAM)SCAR);
	m_tabCtr.InsertItem(TCIF_TEXT | TCIF_IMAGE | TCIF_PARAM, WEBCACHE, GetResString(IDS_WEBCACHE), 1, (LPARAM)WEBCACHE);
	m_tabCtr.InsertItem(TCIF_TEXT | TCIF_IMAGE | TCIF_PARAM, BACKUP, GetResString(IDS_BACKUP), 2, (LPARAM)BACKUP);
	m_tabCtr.InsertItem(TCIF_TEXT | TCIF_IMAGE | TCIF_PARAM, COLOR, GetResString(IDS_COLOR_BOX), 3, (LPARAM)COLOR);
//	m_tabCtr.InsertItem(TCIF_TEXT | TCIF_IMAGE | TCIF_PARAM, UPDATE, _T("Update"), 4, (LPARAM)UPDATE);
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

	// Design settings
	m_ColorBox.CreateEx(0, _T("BUTTON"), _T("Design Settings"), 
						   WS_CHILD /*| WS_VISIBLE*/ | WS_TABSTOP |
						   BS_GROUPBOX,
						   CRect(left, top, right, top+175), this, IDC_COLOR_BOX);
	m_ColorBox.SetFont(GetFont());

	m_MasterCombo.Create(WS_CHILD | CBS_DROPDOWNLIST | WS_VSCROLL | WS_TABSTOP,							  
							  CRect(left+10, top+15, right-60, top+35), this, IDC_COLOR_MASTER_COMBO);
	m_MasterCombo.SetFont(GetFont());

	m_SubCombo.Create(WS_CHILD | CBS_DROPDOWNLIST | WS_VSCROLL | WS_TABSTOP,							  
							  CRect(left+10, top+45, right-60, top+65), this, IDC_COLOR_SUB_COMBO);
	m_SubCombo.SetFont(GetFont());

	m_OnOff.CreateEx(0, _T("BUTTON"), _T(""), 
									WS_CHILD /*| WS_VISIBLE*/ | WS_TABSTOP |
									/*BS_FLAT | */BS_PUSHBUTTON/* | BS_AUTOCHECKBOX*/,
									CRect(right-50, top+25, right-10, top+65), this, IDC_COLOR_ON_OFF);
	m_OnOff.SetFont(GetFont());

	m_bold.CreateEx(0, _T("BUTTON"), _T("B"), 
									WS_CHILD /*| WS_VISIBLE*/ | WS_TABSTOP | BS_PUSHBUTTON |
									BS_AUTOCHECKBOX,
									CRect(left+90, top+75, left+108, top+95), this, IDC_COLOR_BOLD);
	m_bold.SetFont(GetFont());

	m_underlined.CreateEx(0, _T("BUTTON"), _T("U"), 
									WS_CHILD /*| WS_VISIBLE*/ | WS_TABSTOP | BS_PUSHBUTTON |
									BS_AUTOCHECKBOX,
									CRect(left+115, top+75, left+133, top+95), this, IDC_COLOR_UNDERLINED);
	m_underlined.SetFont(GetFont());

	m_italic.CreateEx(0, _T("BUTTON"), _T("I"), 
									WS_CHILD /*| WS_VISIBLE*/ | WS_TABSTOP | BS_PUSHBUTTON |
									BS_AUTOCHECKBOX,
									CRect(left+140, top+75, left+158, top+95), this, IDC_COLOR_ITALIC);
	m_italic.SetFont(GetFont());

	m_FontColorLabel.CreateEx(0, _T("STATIC"), _T("Font color"),
						WS_CHILD /*| WS_VISIBLE*/, 
						CRect(left+10, top+110, right-70, top+125), this, IDC_COLOR_FONT_LABEL);
	m_FontColorLabel.SetFont(GetFont());

	m_FontColor.CreateEx(0, _T("BUTTON"), _T(""), 
									WS_CHILD | WS_TABSTOP | BS_PUSHBUTTON,
									CRect(right-60, top+105, right-10, top+130), this, IDC_COLOR_FONT);
	m_FontColor.SetFont(GetFont());

	m_BackColorLabel.CreateEx(0, _T("STATIC"), _T("Background color"),
						WS_CHILD /*| WS_VISIBLE*/, 
						CRect(left+10, top+145, right-70, top+160), this, IDC_COLOR_BACK_LABEL);
	m_BackColorLabel.SetFont(GetFont());

	m_BackColor.CreateEx(0, _T("BUTTON"), _T(""), 
									WS_CHILD | WS_TABSTOP | BS_PUSHBUTTON,
									CRect(right-60, top+140, right-10, top+165), this, IDC_COLOR_BACK);
	m_BackColor.SetFont(GetFont());

//	m_ColorPreviewBox.CreateEx(0, _T("BUTTON"), _T("Preview"), 
//						   WS_CHILD /*| WS_VISIBLE*/ | WS_TABSTOP |
//						   BS_GROUPBOX,
//						CRect(left+10, top+180, right-10, top+240), this, IDC_COLOR_PREVIEW_BOX);
//	m_ColorPreviewBox.SetFont(GetFont());

//	m_ColorPreview.CreateEx(0, _T("STATIC"), _T("Thus have I politicly begun my reign."),
//						WS_CHILD | ES_CENTER, 
//						CRect(left+15, top+205, right-15, top+220), this, IDC_COLOR_PREVIEW);
//	m_ColorPreview.SetFont(GetFont());
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
				m_NoteText.EnableWindow(FALSE);
				break;
			case COLOR:
				m_ColorBox.ShowWindow(SW_HIDE);
				m_ColorBox.EnableWindow(FALSE);
				m_MasterCombo.ShowWindow(SW_HIDE);
				m_MasterCombo.EnableWindow(FALSE);
				m_SubCombo.ShowWindow(SW_HIDE);
				m_SubCombo.EnableWindow(FALSE);
				m_OnOff.ShowWindow(SW_HIDE);
				m_OnOff.EnableWindow(FALSE);
				m_bold.ShowWindow(SW_HIDE);
				m_bold.EnableWindow(FALSE);
				m_underlined.ShowWindow(SW_HIDE);
				m_underlined.EnableWindow(FALSE);
				m_italic.ShowWindow(SW_HIDE);
				m_italic.EnableWindow(FALSE);
				m_FontColorLabel.ShowWindow(SW_HIDE);
				m_FontColorLabel.EnableWindow(FALSE);
				m_FontColor.ShowWindow(SW_HIDE);
				m_FontColor.EnableWindow(FALSE);
				m_BackColorLabel.ShowWindow(SW_HIDE);
				m_BackColorLabel.EnableWindow(FALSE);
				m_BackColor.ShowWindow(SW_HIDE);
				m_BackColor.EnableWindow(FALSE);
//				m_ColorPreviewBox.ShowWindow(SW_HIDE);
//				m_ColorPreviewBox.EnableWindow(FALSE);
//				m_ColorPreview.ShowWindow(SW_HIDE);
//				m_ColorPreview.EnableWindow(FALSE);
				break; 
/*			case UPDATE:
				break;*/
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
			case COLOR:
				m_ColorBox.ShowWindow(SW_SHOW);
				m_ColorBox.EnableWindow(TRUE);
				m_MasterCombo.ShowWindow(SW_SHOW);
				m_MasterCombo.EnableWindow(TRUE);
				m_SubCombo.ShowWindow(SW_SHOW);
				m_SubCombo.EnableWindow(TRUE);
				m_OnOff.ShowWindow(SW_SHOW);
				m_OnOff.EnableWindow(TRUE);
				m_bold.ShowWindow(SW_SHOW);
				m_bold.EnableWindow(TRUE);
				m_underlined.ShowWindow(SW_SHOW);
				m_underlined.EnableWindow(TRUE);
				m_italic.ShowWindow(SW_SHOW);
				m_italic.EnableWindow(TRUE);
				m_FontColorLabel.ShowWindow(SW_SHOW);
				m_FontColorLabel.EnableWindow(TRUE);
				m_FontColor.ShowWindow(SW_SHOW);
				m_FontColor.EnableWindow(TRUE);
				m_BackColorLabel.ShowWindow(SW_SHOW);
				m_BackColorLabel.EnableWindow(TRUE);
				m_BackColor.ShowWindow(SW_SHOW);
//				m_BackColor.EnableWindow(TRUE);
//				m_ColorPreviewBox.ShowWindow(SW_SHOW);
//				m_ColorPreviewBox.EnableWindow(TRUE);
//				m_ColorPreview.ShowWindow(SW_SHOW);
//				m_ColorPreview.EnableWindow(TRUE);
				UpdateStyles(true);
				break;
/*			case UPDATE:
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

// ==> Design Settings [eWombat/Stulle] - Stulle
void CPPgScar::InitMasterStyleCombo()
{
	int iSel = m_MasterCombo.GetCurSel();
	m_MasterCombo.ResetContent();
	m_MasterCombo.AddString(GetResString(IDS_COLOR_MASTER1));
	m_MasterCombo.AddString(GetResString(IDS_COLOR_MASTER2));
	m_MasterCombo.AddString(GetResString(IDS_COLOR_MASTER3));
	m_MasterCombo.AddString(GetResString(IDS_COLOR_MASTER4));

	m_MasterCombo.SetCurSel(iSel != CB_ERR ? iSel : 0);

	InitSubStyleCombo();

	UpdateStyles();

	return;
}

void CPPgScar::InitSubStyleCombo()
{
	int iMasterSel = m_MasterCombo.GetCurSel();
	m_SubCombo.ResetContent();

	switch(iMasterSel)
	{
		case 0: // client styles
		{
			m_SubCombo.AddString(GetResString(IDS_DEFAULT));
			m_SubCombo.AddString(GetResString(IDS_COLOR_C1));
			m_SubCombo.AddString(GetResString(IDS_COLOR_C2));
			m_SubCombo.AddString(GetResString(IDS_COLOR_C3));
			m_SubCombo.AddString(GetResString(IDS_COLOR_C4));
			m_SubCombo.AddString(GetResString(IDS_COLOR_C5));
			m_SubCombo.AddString(GetResString(IDS_COLOR_C6));
			m_SubCombo.AddString(GetResString(IDS_COLOR_C7));
			m_SubCombo.AddString(GetResString(IDS_COLOR_C8));
		}break;
		case 1: // download styles
		{
			m_SubCombo.AddString(GetResString(IDS_DEFAULT));
			m_SubCombo.AddString(GetResString(IDS_DOWNLOADING));
			m_SubCombo.AddString(GetResString(IDS_COMPLETE));
			m_SubCombo.AddString(GetResString(IDS_COMPLETING));
			m_SubCombo.AddString(GetResString(IDS_HASHING));
			m_SubCombo.AddString(GetResString(IDS_PAUSED));
			m_SubCombo.AddString(GetResString(IDS_STOPPED));
			m_SubCombo.AddString(GetResString(IDS_ERRORLIKE));
		}break;
		case 2: // share styles
		{
			m_SubCombo.AddString(GetResString(IDS_DEFAULT));
			m_SubCombo.AddString(GetResString(IDS_COLOR_S1));
			m_SubCombo.AddString(GetResString(IDS_COLOR_S2));
			CString strTemp = GetResString(IDS_PRIORITY) + _T(" (") + GetResString(IDS_PW_CON_UPLBL) + _T("): ");
			m_SubCombo.AddString(strTemp + GetResString(IDS_PRIOAUTO));
			m_SubCombo.AddString(strTemp + GetResString(IDS_PRIOVERYLOW));
			m_SubCombo.AddString(strTemp + GetResString(IDS_PRIOLOW));
			m_SubCombo.AddString(strTemp + GetResString(IDS_PRIONORMAL));
			m_SubCombo.AddString(strTemp + GetResString(IDS_PRIOHIGH));
			m_SubCombo.AddString(strTemp + GetResString(IDS_PRIORELEASE));
			m_SubCombo.AddString(strTemp + GetResString(IDS_POWERRELEASE));
		}break;
		case 3: // background styles
		{
			m_SubCombo.AddString(GetResString(IDS_COLOR_B1));
			m_SubCombo.AddString(GetResString(IDS_COLOR_B2));
			m_SubCombo.AddString(GetResString(IDS_COLOR_B3));
			m_SubCombo.AddString(GetResString(IDS_COLOR_B4));
			m_SubCombo.AddString(GetResString(IDS_COLOR_B5));
			m_SubCombo.AddString(GetResString(IDS_COLOR_B6));
		}break;
		default:
			break;
	}

	m_SubCombo.SetCurSel(0); // alway first one!
}

void CPPgScar::UpdateStyles(bool bShow)
{
	int iCurStyle = GetStyleValue();
	int iMasterValue = m_MasterCombo.GetCurSel();
	bool bEnable = false;
	bool bOnOff = (iCurStyle != style_c_default && iCurStyle != style_d_default && iCurStyle != style_s_default);

	m_BackColor.SetColor(styles[iCurStyle].nBackColor);
	if(iMasterValue < 3)
	{
		m_FontColor.SetColor(styles[iCurStyle].nFontColor);
		bEnable = true;

//		m_ColorPreview.SetTextColor(styles[iCurStyle].nFontColor);
//		m_ColorPreview.SetBackgroundColor(styles[iCurStyle].nBackColor);
	}

	if(bOnOff)
	{
		m_OnOff.EnableWindow(bEnable);

		if(styles[iCurStyle].nOnOff != 0 && bEnable)
		{
			m_BackColor.EnableWindow(true);
			m_OnOff.SetWindowText( GetResString(IDS_COLOR_OFF) );
		}
		else
		{
			m_OnOff.SetWindowText( GetResString(IDS_COLOR_ON) );
			bEnable = false;
			m_BackColor.EnableWindow(iMasterValue == 3);
		}
	}
	else
	{
		m_BackColor.EnableWindow(true);
		m_OnOff.EnableWindow(FALSE);
		m_OnOff.SetWindowText( GetResString(IDS_COLOR_ON) );
	}
	m_bold.EnableWindow(bEnable);
	m_underlined.EnableWindow(bEnable);
	m_italic.EnableWindow(bEnable);
	m_FontColor.EnableWindow(bEnable);
	if(bShow) // stupid, yet working workarround some gui glitch...
	{
		m_FontColor.ShowWindow(SW_HIDE);
		m_BackColor.ShowWindow(SW_HIDE);
		m_FontColor.ShowWindow(SW_SHOW);
		m_BackColor.ShowWindow(SW_SHOW);
	}

	int iStyle = (styles[iCurStyle].nFlags & STYLE_FONTMASK);
	m_bold.SetCheck(iStyle== STYLE_BOLD ? 1:0);
	m_underlined.SetCheck(iStyle== STYLE_UNDERLINE ? 1:0);
	m_italic.SetCheck(iStyle== STYLE_ITALIC ? 1:0);

//	switch (iStyle)
//	{
//		case STYLE_BOLD:		m_ColorPreview.SetFont(m_bold.GetFont(),TRUE);break;
//		case STYLE_UNDERLINE:	m_ColorPreview.SetFont(m_underlined.GetFont(),TRUE);break;
//		case STYLE_ITALIC:		m_ColorPreview.SetFont(m_italic.GetFont(),TRUE);break;
//		default:				m_ColorPreview.SetFont(GetFont(),TRUE);break;
//	}
}

int CPPgScar::GetStyleValue()
{
	int iMasterValue = m_MasterCombo.GetCurSel();
	int iCurStyle = 0;

	switch(iMasterValue)
	{
		case 0:
			break;
		case 1:
			iCurStyle = style_d_default;
			break;
		case 2:
			iCurStyle = style_s_default;
			break;
		case 3:
			iCurStyle = style_b_clientlist;
			break;
		default:
			break;
	}
	iCurStyle += m_SubCombo.GetCurSel();

	return iCurStyle;
}

void CPPgScar::OnFontStyle(int iStyle)
{
	m_bold.SetCheck(iStyle==1 ? 1:0);
	m_underlined.SetCheck(iStyle==2 ? 1:0);
	m_italic.SetCheck(iStyle==3 ? 1:0);
	iStyle = 0 ; //Now select font

	if (m_bold.GetCheck())
		iStyle = STYLE_BOLD;
	else if (m_underlined.GetCheck())
		iStyle = STYLE_UNDERLINE;
	else if (m_italic.GetCheck())
		iStyle = STYLE_ITALIC;

	int iCurStyle = GetStyleValue();
	DWORD flags = (styles[iCurStyle].nFlags & ~STYLE_FONTMASK) | iStyle;
	if (flags != styles[iCurStyle].nFlags)
	{
		SetModified();
		styles[iCurStyle].nFlags = flags;
		UpdateStyles();
		m_bDesignChanged = true;
	}
}

LONG CPPgScar::OnColorPopupSelChange(UINT /*lParam*/, LONG /*wParam*/)
{
	int iSel = GetStyleValue();

	// font
	if (iSel >= 0)
	{
		COLORREF crColor = m_FontColor.GetColor();
		if (crColor != styles[iSel].nFontColor)
		{
			styles[iSel].nFontColor = crColor;
			SetModified(TRUE);
			UpdateStyles();
			m_bDesignChanged = true;
		}
	}

	// background
	if (iSel >= 0)
	{
		COLORREF crColor = m_BackColor.GetColor();
		if (crColor != styles[iSel].nBackColor)
		{
			styles[iSel].nBackColor = crColor;
			SetModified(TRUE);
			UpdateStyles();
			m_bDesignChanged = true;
		}
	}

	return TRUE;
}

void CPPgScar::OnBnClickedBold()
{
	OnFontStyle(m_bold.GetCheck() ? 1:0);
}

void CPPgScar::OnBnClickedUnderlined()
{
	OnFontStyle(m_underlined.GetCheck() ? 2:0);
}

void CPPgScar::OnBnClickedItalic()
{
	OnFontStyle(m_italic.GetCheck() ? 3:0);
}

void CPPgScar::OnCbnSelchangeStyleselMaster()
{
	InitSubStyleCombo();
	UpdateStyles();
}

void CPPgScar::OnCbnSelchangeStyleselSub()
{
	UpdateStyles();
}

void CPPgScar::OnBnClickedOnOff()
{
	int iSel = GetStyleValue();
	short sOnOff = styles[iSel].nOnOff;

	if(sOnOff == 1)
		sOnOff = 0;
	else
		sOnOff = 1;
	styles[iSel].nOnOff = sOnOff;

	UpdateStyles();
	SetModified(TRUE);
	m_bDesignChanged = true;
}
// <== Design Settings [eWombat/Stulle] - Stulle