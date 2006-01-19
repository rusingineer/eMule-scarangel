// PpgStulle.cpp : implementation file
//

#include "stdafx.h"
#include "emule.h"
#include "PPgScar.h"
#include "emuledlg.h"
#include "UserMsgs.h"
#include "opcodes.h"
#include "otherfunctions.h"
//#include "MuleToolbarCtrl.h" // TBH: minimule - Stulle
#include "ClientCredits.h" // CreditSystems [EastShare/ MorphXT] - Stulle
#include "log.h"

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
END_MESSAGE_MAP()

CPPgScar::CPPgScar()
	: CPropertyPage(CPPgScar::IDD)
	
	, m_ctrlTreeOptions(theApp.m_iDfltImageListColorFlags)
{
	m_bInitializedTreeOpts = false;
/*
	m_htiSecu = NULL;
	// ==> Sivka-Ban - Stulle
	m_htiSivkaBanGroup = NULL;
	m_htiEnableSivkaBan = NULL;
	m_htiSivkaAskTime = NULL;
	m_htiSivkaAskCounter = NULL;
	m_htiSivkaAskLog = NULL;
	// <== Sivka-Ban - Stulle
	// ==> ban systems optional - Stulle
	m_htiAntiLeecherGroup = NULL;
	m_htiEnableAntiLeecher = NULL; //MORPH - Added by IceCream, activate Anti-leecher
	m_htiBadModString = NULL;
	m_htiBadNickBan = NULL;
	m_htiGhostMod = NULL;
	m_htiAntiModIdFaker = NULL;
	m_htiAntiNickThief = NULL; // AntiNickThief - Stulle
	m_htiEmptyNick = NULL;
	m_htiFakeEmule = NULL;
	m_htiHiddenStr = NULL;
	// <== ban systems optional - Stulle
	m_htiEnableAntiCreditHack = NULL; //MORPH - Added by IceCream, activate Anti-CreditHack
	m_htiClientBanTime = NULL; // adjust ClientBanTime - Stulle
*/
	m_htiPush = NULL; // push files - Stulle
	// ==> push small files [sivka] - Stulle
	m_htiEnablePushSmallFile = NULL;
	m_iPushSmallFiles = 0;
	m_htiPushSmallFileBoost = NULL;
	// <== push small files [sivka] - Stulle
	m_htiEnablePushRareFile = NULL;  // push rare file - Stulle

/*	// ==> FunnyNick Tag - Stulle
	m_htiFnTag = NULL;
	m_htiNoTag = NULL;
	m_htiShortTag = NULL;
	m_htiFullTag = NULL;
	m_htiCustomTag = NULL;
	m_htiFnCustomTag = NULL;
	m_htiFnTagAtEnd = NULL;
	// <== FunnyNick Tag - Stulle
*/
	m_htiConTweaks = NULL;
/*	// ==> Quick start [TPT] - Stulle
	m_htiQuickStartGroup = NULL;
	m_htiQuickStart = NULL;
	m_htiQuickStartMaxTime = NULL;
	m_htiQuickStartMaxConnPerFive = NULL;
	m_htiQuickStartMaxConn = NULL;
	m_htiQuickStartMaxConnPerFiveBack = NULL;
	m_htiQuickStartMaxConnBack = NULL;
	m_htiQuickStartAfterIPChange = NULL;
	// <== Quick start [TPT] - Stulle
	m_htiReAskFileSrc = NULL; // Timer for ReAsk File Sources - Stulle
	m_htiIsreaskSourceAfterIPChange = NULL; // Xman -Reask sources after IP change- v2 - Stulle

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
	m_htiShowSrcOnTitle = NULL; // Show sources on title - Stulle
//	m_htiPsFilesRed = NULL; // draw PS files red - Stulle
	m_htiFriendsBlue = NULL; // draw friends blue - Stulle
	m_htiShowGlobalHL = NULL; // show global HL - Stulle
	m_htiShowFileHLconst = NULL; // show HL per file constantaniously - Stulle
	m_htiShowInMSN7 = NULL; // Show in MSN7 [TPT] - Stulle
//	m_htiTrayComplete = NULL; // Completed in Tray - Stulle
	m_htiCountWCSessionStats = NULL; // Show WC session stats [MorphXT] - Stulle

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
/*
	// ==> TBH: minimule - Stulle
	m_htiMMGroup = NULL;
	m_htiShowMM = NULL;
	m_htiMMLives = NULL;
	m_htiMMUpdateTime = NULL;
	m_htiMMTrans = NULL;
	m_htiMMCompl = NULL;
	m_htiMMOpen = NULL;
	// <== TBH: minimule - Stulle
*/
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
	m_htiGlobalHlAggro = NULL;
	m_htiGlobalHlDefault = NULL;
	// <== Global Source Limit [Max/Stulle] - Stulle
}

CPPgScar::~CPPgScar()
{
}

void CPPgScar::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SCAR_OPTS, m_ctrlTreeOptions);
	if (!m_bInitializedTreeOpts)
	{
//		int iImgSecu = 8;
		int iImgPush = 8;
//		int iImgFunnyNick = 8;
		int iImgConTweaks = 8;
//		int iImgQuickstart = 8;
		int iImgCS = 8; // CreditSystems [EastShare/ MorphXT] - Stulle
		int iImgDisplay = 8;
		int iImgDropDefaults = 8;
//		int iImgMinimule = 8;
		int iImgMisc = 8;
		int iImgGlobal = 8;

		CImageList* piml = m_ctrlTreeOptions.GetImageList(TVSIL_NORMAL);
		if (piml){
//			iImgSecu = piml->Add(CTempIconLoader(_T("SECURITY")));
			iImgPush = piml->Add(CTempIconLoader(_T("SPEED")));
//			iImgFunnyNick = piml->Add(CTempIconLoader(_T("FUNNYNICK")));
			iImgConTweaks =  piml->Add(CTempIconLoader(_T("CONNECTION")));
//			iImgQuickstart = piml->Add(CTempIconLoader(_T("QUICKSTART"))); // Thx to the eF-Mod team for the icon
			iImgCS = piml->Add(CTempIconLoader(_T("STATSCLIENTS"))); // CreditSystems [EastShare/ MorphXT] - Stulle
			iImgDisplay = piml->Add(CTempIconLoader(_T("DISPLAY")));
			iImgDropDefaults = piml->Add(CTempIconLoader(_T("DROPDEFAULTS")));
//			iImgMinimule = piml->Add(CTempIconLoader(_T("MINIMULE")));
			iImgMisc = piml->Add(CTempIconLoader(_T("SRCUNKNOWN")));
			iImgGlobal = piml->Add(CTempIconLoader(_T("SEARCHMETHOD_GLOBAL")));
		}
		
		CString Buffer;
/*
		m_htiSecu = m_ctrlTreeOptions.InsertGroup(GetResString(IDS_SECURITY), iImgSecu, TVI_ROOT);
		// ==> Sivka-Ban - Stulle
		m_htiSivkaBanGroup = m_ctrlTreeOptions.InsertGroup(GetResString(IDS_SIVKA_BAN_CONTROL), iImgSecu, m_htiSecu);
		m_htiEnableSivkaBan = m_ctrlTreeOptions.InsertCheckBox(GetResString(IDS_SIVKA_BAN),m_htiSivkaBanGroup, m_bEnableSivkaBan);
		m_htiSivkaAskTime = m_ctrlTreeOptions.InsertItem(GetResString(IDS_SIVKA_ASK_TIME), TREEOPTSCTRLIMG_EDIT, TREEOPTSCTRLIMG_EDIT, m_htiSivkaBanGroup);
		m_ctrlTreeOptions.AddEditBox(m_htiSivkaAskTime, RUNTIME_CLASS(CNumTreeOptionsEdit));
		m_htiSivkaAskCounter = m_ctrlTreeOptions.InsertItem(GetResString(IDS_SIVKA_ASK_COUNTER), TREEOPTSCTRLIMG_EDIT, TREEOPTSCTRLIMG_EDIT, m_htiSivkaBanGroup);
		m_ctrlTreeOptions.AddEditBox(m_htiSivkaAskCounter, RUNTIME_CLASS(CNumTreeOptionsEdit));
		m_htiSivkaAskLog = m_ctrlTreeOptions.InsertCheckBox(GetResString(IDS_SIVKA_ASK_LOG), m_htiSivkaBanGroup, m_bSivkaAskLog);
		// <== Sivka-Ban - Stulle
		// ==> ban systems optional - Stulle
		m_htiAntiLeecherGroup = m_ctrlTreeOptions.InsertGroup(GetResString(IDS_ANTI_LEECHER_GROUP), iImgSecu, m_htiSecu);
		m_htiEnableAntiLeecher = m_ctrlTreeOptions.InsertCheckBox(GetResString(IDS_SUC_ENABLED), m_htiAntiLeecherGroup, m_bEnableAntiLeecher); //MORPH - Added by IceCream, Enable Anti-leecher
		m_htiBadModString = m_ctrlTreeOptions.InsertCheckBox(GetResString(IDS_BAD_MOD_STRING), m_htiAntiLeecherGroup, m_bBadModString);
		m_htiBadNickBan = m_ctrlTreeOptions.InsertCheckBox(GetResString(IDS_BAD_NICK_BAN), m_htiAntiLeecherGroup, m_bBadNickBan);
		m_htiGhostMod = m_ctrlTreeOptions.InsertCheckBox(GetResString(IDS_GHOST_MOD), m_htiAntiLeecherGroup, m_bGhostMod);
		m_htiAntiModIdFaker = m_ctrlTreeOptions.InsertCheckBox(GetResString(IDS_ANTI_MOD_FAKER), m_htiAntiLeecherGroup, m_bAntiModIdFaker);
		m_htiAntiNickThief = m_ctrlTreeOptions.InsertCheckBox(GetResString(IDS_ANTI_NICK_THIEF), m_htiAntiLeecherGroup, m_bAntiNickThief); // AntiNickThief - Stulle
		m_htiEmptyNick = m_ctrlTreeOptions.InsertCheckBox(GetResString(IDS_EMPTY_NICK), m_htiAntiLeecherGroup, m_bEmptyNick);
		m_htiFakeEmule = m_ctrlTreeOptions.InsertCheckBox(GetResString(IDS_FAKE_EMULE), m_htiAntiLeecherGroup, m_bFakeEmule);
		m_htiHiddenStr = m_ctrlTreeOptions.InsertCheckBox(GetResString(IDS_HIDDEN_STR), m_htiAntiLeecherGroup, m_bHiddenStr);
		m_ctrlTreeOptions.Expand(m_htiEnableAntiLeecher, TVE_EXPAND);
		// <== ban systems optional - Stulle
		m_htiEnableAntiCreditHack = m_ctrlTreeOptions.InsertCheckBox(GetResString(IDS_ANTI_CREDITHACK), m_htiSecu, m_bEnableAntiCreditHack); //MORPH - Added by IceCream, Enable Anti-CreditHack
		// ==> adjust ClientBanTime - Stulle
		m_htiClientBanTime = m_ctrlTreeOptions.InsertItem(GetResString(IDS_CLIENT_BAN_TIME), TREEOPTSCTRLIMG_EDIT, TREEOPTSCTRLIMG_EDIT, m_htiSecu);
		m_ctrlTreeOptions.AddEditBox(m_htiClientBanTime, RUNTIME_CLASS(CNumTreeOptionsEdit));
		// <== adjust ClientBanTime - Stulle
		m_ctrlTreeOptions.Expand(m_htiSecu, TVE_EXPAND);
*/		
		m_htiPush = m_ctrlTreeOptions.InsertGroup(GetResString(IDS_PUSH), iImgPush, TVI_ROOT); // push files - Stulle
		// ==> push small files [sivka]  - Stulle
		m_htiEnablePushSmallFile = m_ctrlTreeOptions.InsertCheckBox(GetResString(IDS_PUSH_SMALL), m_htiPush, m_bEnablePushSmallFile);
		m_htiPushSmallFileBoost = m_ctrlTreeOptions.InsertItem(GetResString(IDS_PUSH_SMALL_BOOST), TREEOPTSCTRLIMG_EDIT, TREEOPTSCTRLIMG_EDIT, m_htiPush);
		m_ctrlTreeOptions.AddEditBox(m_htiPushSmallFileBoost, RUNTIME_CLASS(CNumTreeOptionsEdit));
		// <== push small files [sivka]  - Stulle
		m_htiEnablePushRareFile = m_ctrlTreeOptions.InsertCheckBox(GetResString(IDS_PUSH_RARE), m_htiPush, m_bEnablePushRareFile); // push rare file - Stulle

/*		// ==> FunnyNick Tag - Stulle
		m_htiFnTag = m_ctrlTreeOptions.InsertGroup(GetResString(IDS_FN_TAG), iImgFunnyNick, TVI_ROOT);
		m_htiNoTag = m_ctrlTreeOptions.InsertRadioButton(GetResString(IDS_NO_TAG), m_htiFnTag, m_iFnTag == 0);
		m_htiShortTag = m_ctrlTreeOptions.InsertRadioButton(GetResString(IDS_SHORT_TAG), m_htiFnTag, m_iFnTag == 1);
		m_htiFullTag = m_ctrlTreeOptions.InsertRadioButton(GetResString(IDS_FULL_TAG), m_htiFnTag, m_iFnTag == 2);
		m_htiCustomTag = m_ctrlTreeOptions.InsertRadioButton(GetResString(IDS_CUSTOM_TAG),m_htiFnTag,m_iFnTag == 3);
		m_htiFnCustomTag = m_ctrlTreeOptions.InsertItem(GetResString(IDS_SET_CUSTOM_TAG), TREEOPTSCTRLIMG_EDIT, TREEOPTSCTRLIMG_EDIT, m_htiCustomTag);
		m_ctrlTreeOptions.AddEditBox(m_htiFnCustomTag, RUNTIME_CLASS(CTreeOptionsEdit));
		m_htiFnTagAtEnd = m_ctrlTreeOptions.InsertCheckBox(GetResString(IDS_FN_TAG_AT_END), m_htiFnTag, m_bFnTagAtEnd);
		m_ctrlTreeOptions.Expand(m_htiCustomTag, TVE_EXPAND);
		// <== FunnyNick Tag - Stulle
*/
//		m_htiConTweaks = m_ctrlTreeOptions.InsertGroup(GetResString(IDS_CON_TWEAKS), iImgConTweaks, TVI_ROOT);
/*		m_htiQuickStartGroup = m_ctrlTreeOptions.InsertGroup(GetResString(IDS_QUICK_START_GROUP), iImgQuickstart, m_htiConTweaks);
		// ==> Quick start [TPT] - Stulle
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
		// <== Quick start [TPT] - Stulle
		m_htiIsreaskSourceAfterIPChange = m_ctrlTreeOptions.InsertCheckBox(GetResString(IDS_RSAIC), m_htiConTweaks, m_bIsreaskSourceAfterIPChange); // Xman -Reask sources after IP change- v2 - Stulle
		// ==> Timer for ReAsk File Sources - Stulle
		m_htiReAskFileSrc = m_ctrlTreeOptions.InsertItem(GetResString(IDS_REASK_FILE_SRC), TREEOPTSCTRLIMG_EDIT, TREEOPTSCTRLIMG_EDIT, m_htiConTweaks);
		m_ctrlTreeOptions.AddEditBox(m_htiReAskFileSrc, RUNTIME_CLASS(CNumTreeOptionsEdit));
		// <== Timer for ReAsk File Sources - Stulle

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
		m_htiShowSrcOnTitle = m_ctrlTreeOptions.InsertCheckBox(GetResString(IDS_SHOWSRCONTITLE), m_htiDisplay, showSrcInTitle); // Show sources on title - Stulle
//		m_htiPsFilesRed = m_ctrlTreeOptions.InsertCheckBox(GetResString(IDS_PS_FILES_RED), m_htiDisplay, m_bPsFilesRed); // draw PS files red - Stulle
		m_htiFriendsBlue = m_ctrlTreeOptions.InsertCheckBox(GetResString(IDS_FRIENDS_BLUE), m_htiDisplay, m_bFriendsBlue); // draw friends blue - Stulle
		m_htiShowGlobalHL = m_ctrlTreeOptions.InsertCheckBox(GetResString(IDS_SHOW_GLOBAL_HL), m_htiDisplay, m_bShowGlobalHL); // show global HL - Stulle
		m_htiShowFileHLconst = m_ctrlTreeOptions.InsertCheckBox(GetResString(IDS_SHOW_FILE_HL_CONST), m_htiDisplay, m_bShowFileHLconst); // show HL per file constantaniously - Stulle
		m_htiShowInMSN7 = m_ctrlTreeOptions.InsertCheckBox(GetResString(IDS_SHOWINMSN7), m_htiDisplay, m_bShowInMSN7); // Show in MSN7 [TPT] - Stulle
//		m_htiTrayComplete = m_ctrlTreeOptions.InsertCheckBox(GetResString(IDS_TRAY_COMPLETE), m_htiDisplay, m_bTrayComplete); // Completed in Tray - Stulle
		m_htiCountWCSessionStats = m_ctrlTreeOptions.InsertCheckBox(GetResString(IDS_COUNT_WC_STATS),m_htiDisplay,m_bCountWCSessionStats); // Show WC session stats [MorphXT] - Stulle

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
/*
		// ==> TBH: minimule - Stulle
		m_htiMMGroup = m_ctrlTreeOptions.InsertGroup(_T("TBH Mini-Mule"), iImgMinimule, TVI_ROOT);
		m_htiShowMM = m_ctrlTreeOptions.InsertCheckBox(GetResString(IDS_MM_SHOW), m_htiMMGroup, m_bShowMM);
		m_htiMMLives = m_ctrlTreeOptions.InsertCheckBox(GetResString(IDS_MM_LIVES), m_htiMMGroup, m_bMMLives);
		m_htiMMUpdateTime = m_ctrlTreeOptions.InsertItem(GetResString(IDS_MM_UPDATE_TIME), TREEOPTSCTRLIMG_EDIT, TREEOPTSCTRLIMG_EDIT, m_htiMMGroup);
		m_ctrlTreeOptions.AddEditBox(m_htiMMUpdateTime, RUNTIME_CLASS(CNumTreeOptionsEdit));
		m_htiMMTrans = m_ctrlTreeOptions.InsertItem(GetResString(IDS_MM_TRANS), TREEOPTSCTRLIMG_EDIT, TREEOPTSCTRLIMG_EDIT, m_htiMMGroup);
		m_ctrlTreeOptions.AddEditBox(m_htiMMTrans, RUNTIME_CLASS(CNumTreeOptionsEdit));
		m_htiMMCompl = m_ctrlTreeOptions.InsertCheckBox(GetResString(IDS_MM_COMPL), m_htiMMGroup, m_bMMCompl);
		m_htiMMOpen = m_ctrlTreeOptions.InsertCheckBox(GetResString(IDS_MM_OPEN), m_htiMMGroup, m_bMMOpen);
		// <== TBH: minimule - Stulle
*/
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
		m_htiGlobalHlAggro = m_ctrlTreeOptions.InsertCheckBox(GetResString(IDS_GLOBAL_HL_AGGRO),m_htiGlobalHlGroup,m_bGlobalHlAggro);
		m_htiGlobalHlDefault = m_ctrlTreeOptions.InsertCheckBox(GetResString(IDS_GLOBAL_HL_DEFAULT), m_htiGlobalHlGroup, m_bGlobalHlDefault);
		m_ctrlTreeOptions.Expand(m_htiGlobalHlGroup, TVE_EXPAND);
		// <== Global Source Limit [Max/Stulle] - Stulle

		m_ctrlTreeOptions.SendMessage(WM_VSCROLL, SB_TOP);
		m_bInitializedTreeOpts = true;
	}
/*
	// ==> Sivka-Ban - Stulle
	DDX_TreeCheck(pDX, IDC_SCAR_OPTS, m_htiEnableSivkaBan, m_bEnableSivkaBan);
	DDX_TreeEdit(pDX, IDC_SCAR_OPTS, m_htiSivkaAskTime, m_iSivkaAskTime);
	DDV_MinMaxInt(pDX, m_iSivkaAskTime, 5, 10);
	DDX_TreeEdit(pDX, IDC_SCAR_OPTS, m_htiSivkaAskCounter, m_iSivkaAskCounter);
	DDV_MinMaxInt(pDX, m_iSivkaAskCounter, 5, 15);
	DDX_TreeCheck(pDX, IDC_SCAR_OPTS, m_htiSivkaAskLog, m_bSivkaAskLog);
	// <== Sivka-Ban - Stulle
	// ==> ban systems optional - Stulle
	DDX_TreeCheck(pDX, IDC_SCAR_OPTS, m_htiEnableAntiLeecher, m_bEnableAntiLeecher); //MORPH - Added by IceCream, enable Anti-leecher
	if(m_htiBadModString)	DDX_TreeCheck(pDX, IDC_SCAR_OPTS, m_htiBadModString, m_bBadModString);
	if(m_htiBadNickBan)		DDX_TreeCheck(pDX, IDC_SCAR_OPTS, m_htiBadNickBan, m_bBadNickBan);
	if(m_htiGhostMod)		DDX_TreeCheck(pDX, IDC_SCAR_OPTS, m_htiGhostMod, m_bGhostMod);
	if(m_htiAntiModIdFaker)	DDX_TreeCheck(pDX, IDC_SCAR_OPTS, m_htiAntiModIdFaker, m_bAntiModIdFaker);
	if(m_htiAntiNickThief)	DDX_TreeCheck(pDX, IDC_SCAR_OPTS, m_htiAntiNickThief, m_bAntiNickThief); // AntiNickThief - Stulle
	if(m_htiEmptyNick)		DDX_TreeCheck(pDX, IDC_SCAR_OPTS, m_htiEmptyNick, m_bEmptyNick);
	if(m_htiFakeEmule)		DDX_TreeCheck(pDX, IDC_SCAR_OPTS, m_htiFakeEmule, m_bFakeEmule);
	if(m_htiHiddenStr)		DDX_TreeCheck(pDX, IDC_SCAR_OPTS, m_htiHiddenStr, m_bHiddenStr);
	// <== ban systems optional - Stulle
	DDX_TreeCheck(pDX, IDC_SCAR_OPTS, m_htiEnableAntiCreditHack, m_bEnableAntiCreditHack); //MORPH - Added by IceCream, enable Anti-CreditHack
	// ==> adjust ClientBanTime - Stulle
	DDX_TreeEdit(pDX, IDC_SCAR_OPTS, m_htiClientBanTime, m_iClientBanTime);
	DDV_MinMaxInt(pDX, m_iClientBanTime, 1, 10);
	// <== adjust ClientBanTime - Stulle
*/
	// ==> push small files [sivka] - Stulle
    DDX_TreeCheck(pDX, IDC_SCAR_OPTS, m_htiEnablePushSmallFile, m_bEnablePushSmallFile);
	DDX_TreeEdit(pDX, IDC_SCAR_OPTS, m_htiPushSmallFileBoost, m_iPushSmallFileBoost);
	DDV_MinMaxInt(pDX, m_iPushSmallFileBoost, 1, 65536);
	// <== push small files [sivka] - Stulle
	DDX_TreeCheck(pDX, IDC_SCAR_OPTS, m_htiEnablePushRareFile, m_bEnablePushRareFile); // push rare file - Stulle

/*	// ==> FunnyNick Tag - Stulle
	DDX_TreeRadio(pDX, IDC_SCAR_OPTS, m_htiFnTag, (int &)m_iFnTag);
	DDX_TreeEdit(pDX, IDC_SCAR_OPTS, m_htiFnCustomTag, m_sFnCustomTag);
	DDX_TreeCheck(pDX, IDC_SCAR_OPTS, m_htiFnTagAtEnd, m_bFnTagAtEnd);
	// <== FunnyNick Tag - Stulle
	
	// ==> Quick start [TPT] - Stulle
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
	// <== Quick start [TPT] - Stulle
	DDX_TreeCheck(pDX, IDC_SCAR_OPTS, m_htiIsreaskSourceAfterIPChange, m_bIsreaskSourceAfterIPChange); // Xman -Reask sources after IP change- v2 - Stulle
	// ==> Timer for ReAsk File Sources - Stulle
	DDX_TreeEdit(pDX, IDC_SCAR_OPTS, m_htiReAskFileSrc, m_iReAskFileSrc);
	DDV_MinMaxInt(pDX, m_iReAskFileSrc, 29, 55);
	// <== Timer for ReAsk File Sources - Stulle

	// ==> Anti Uploader Ban - Stulle
	DDX_TreeEdit(pDX, IDC_SCAR_OPTS, m_htiAntiUploaderBanLimit, m_iAntiUploaderBanLimit);
	DDV_MinMaxInt(pDX, m_iAntiUploaderBanLimit, 0, 20);
	DDX_TreeRadio(pDX, IDC_SCAR_OPTS, m_htiAntiUploaderBanLimit, (int &)m_iAntiUploaderBanCase);
	// <== Anti Uploader Ban - Stulle
*/

	DDX_TreeRadio(pDX, IDC_SCAR_OPTS, m_htiCreditSystem, (int &)m_iCreditSystem); // CreditSystems [EastShare/ MorphXT] - Stulle

	DDX_TreeCheck(pDX, IDC_SCAR_OPTS, m_htiShowSrcOnTitle, showSrcInTitle); // Show sources on title - Stulle
//	DDX_TreeCheck(pDX, IDC_SCAR_OPTS, m_htiPsFilesRed, m_bPsFilesRed); // draw PS files red - Stulle
	DDX_TreeCheck(pDX, IDC_SCAR_OPTS, m_htiFriendsBlue, m_bFriendsBlue); // draw friends blue - Stulle
	DDX_TreeCheck(pDX, IDC_SCAR_OPTS, m_htiShowGlobalHL, m_bShowGlobalHL); // show global HL - Stulle
	DDX_TreeCheck(pDX, IDC_SCAR_OPTS, m_htiShowFileHLconst, m_bShowFileHLconst); // show HL per file constantaniously - Stulle
	DDX_TreeCheck(pDX, IDC_SCAR_OPTS, m_htiShowInMSN7, m_bShowInMSN7); // Show in MSN7 [TPT] - Stulle
//	DDX_TreeCheck(pDX, IDC_SCAR_OPTS, m_htiTrayComplete, m_bTrayComplete); // Completed in Tray - Stulle
	DDX_TreeCheck(pDX, IDC_SCAR_OPTS, m_htiCountWCSessionStats, m_bCountWCSessionStats); // Show WC session stats [MorphXT] - Stulle

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
/*
	// ==> TBH: minimule - Stulle
	DDX_TreeCheck(pDX, IDC_SCAR_OPTS, m_htiShowMM, m_bShowMM);
	DDX_TreeCheck(pDX, IDC_SCAR_OPTS, m_htiMMLives, m_bMMLives);
	DDX_TreeEdit(pDX, IDC_SCAR_OPTS, m_htiMMUpdateTime, m_iMMUpdateTime);
	DDV_MinMaxInt(pDX, m_iMMUpdateTime, 0, MIN2S(60));
	DDX_TreeEdit(pDX, IDC_SCAR_OPTS, m_htiMMTrans, m_iMMTrans);
	DDV_MinMaxInt(pDX, m_iMMTrans, 1, 255);
	DDX_TreeCheck(pDX, IDC_SCAR_OPTS, m_htiMMCompl, m_bMMCompl);
	DDX_TreeCheck(pDX, IDC_SCAR_OPTS, m_htiMMOpen, m_bMMOpen);
	// <== TBH: minimule - Stulle
*/
	DDX_TreeCheck(pDX, IDC_SCAR_OPTS, m_htiSUQWT, m_bSUQWT); // SUQWT [Moonlight/EastShare/ MorphXT] - Stulle
/*	// ==> Spread Credits Slot - Stulle
	DDX_TreeCheck(pDX, IDC_SCAR_OPTS, m_htiSpreadCreditsSlot, m_bSpreadCreditsSlot);
	DDX_TreeEdit(pDX, IDC_SCAR_OPTS, m_htiSpreadCreditsSlotCounter, m_iSpreadCreditsSlotCounter);
	DDV_MinMaxInt(pDX, m_iSpreadCreditsSlotCounter, 3, 20);
	// <== Spread Credits Slot - Stulle
*/	// ==> Global Source Limit [Max/Stulle] - Stulle
	DDX_TreeCheck(pDX, IDC_SCAR_OPTS, m_htiGlobalHL, m_bGlobalHL);
	DDX_TreeEdit(pDX, IDC_SCAR_OPTS, m_htiGlobalHlLimit, m_iGlobalHL);
	uint16 temp = (!thePrefs.m_bAcceptsourcelimit || thePrefs.m_uMaxGlobalSources > MAX_GSL) ? MAX_GSL : thePrefs.m_uMaxGlobalSources;
	DDV_MinMaxInt(pDX, m_iGlobalHL, 0, temp);
	DDX_TreeCheck(pDX, IDC_SCAR_OPTS, m_htiGlobalHlAggro, m_bGlobalHlAggro);
	DDX_TreeCheck(pDX, IDC_SCAR_OPTS, m_htiGlobalHlDefault, m_bGlobalHlDefault);
	// <== Global Source Limit [Max/Stulle] - Stulle
/*
	// ==> ban systems optional - Stulle
	if (m_htiBadModString)		m_ctrlTreeOptions.SetCheckBoxEnable(m_htiBadModString, m_bEnableAntiLeecher);
	if (m_htiBadNickBan)		m_ctrlTreeOptions.SetCheckBoxEnable(m_htiBadNickBan, m_bEnableAntiLeecher);
	if (m_htiGhostMod)			m_ctrlTreeOptions.SetCheckBoxEnable(m_htiGhostMod, m_bEnableAntiLeecher);
	if (m_htiAntiModIdFaker)	m_ctrlTreeOptions.SetCheckBoxEnable(m_htiAntiModIdFaker, m_bEnableAntiLeecher);
	if (m_htiAntiNickThief)		m_ctrlTreeOptions.SetCheckBoxEnable(m_htiAntiNickThief, m_bEnableAntiLeecher);
	if (m_htiEmptyNick)			m_ctrlTreeOptions.SetCheckBoxEnable(m_htiEmptyNick, m_bEnableAntiLeecher);
	if (m_htiFakeEmule)			m_ctrlTreeOptions.SetCheckBoxEnable(m_htiFakeEmule, m_bEnableAntiLeecher);
	if (m_htiHiddenStr)			m_ctrlTreeOptions.SetCheckBoxEnable(m_htiHiddenStr, m_bEnableAntiLeecher);
	// <== ban systems optional - Stulle

	// ==> Quick start [TPT] - Stulle
	if (m_htiQuickStartAfterIPChange)	m_ctrlTreeOptions.SetCheckBoxEnable(m_htiQuickStartAfterIPChange, m_bQuickStartAfterIPChange);
	// <== Quick start [TPT] - Stulle
*/
}


BOOL CPPgScar::OnInitDialog()
{
/*
	// ==> Sivka-Ban - Stulle
	m_bEnableSivkaBan = thePrefs.enableSivkaBan;
	m_iSivkaAskTime = (int)(thePrefs.SivkaAskTime);
	m_iSivkaAskCounter = (int)(thePrefs.SivkaAskCounter);
	m_bSivkaAskLog = thePrefs.SivkaAskLog;
	// <== Sivka-Ban - Stulle
	// ==> ban systems optional - Stulle
	m_bEnableAntiLeecher = thePrefs.enableAntiLeecher; //MORPH - Added by IceCream, enabnle Anti-leecher
	m_bBadModString = thePrefs.IsBadModString();
	m_bBadNickBan = thePrefs.IsBadNickBan();
	m_bGhostMod = thePrefs.IsGhostMod();
	m_bAntiModIdFaker = thePrefs.IsAntiModIdFaker();
	m_bAntiNickThief = thePrefs.IsAntiNickThief(); // AntiNickThief - Stulle
	m_bEmptyNick = thePrefs.IsEmptyNick();
	m_bFakeEmule = thePrefs.IsFakeEmule();
	m_bHiddenStr = thePrefs.IsHiddenStr();
	// <== ban systems optional - Stulle
	m_bEnableAntiCreditHack = thePrefs.enableAntiCreditHack; //MORPH - Added by IceCream, enabnle Anti-CreditHack
	m_iClientBanTime = thePrefs.GetClientBanTime()/3600000; // adjust ClientBanTime - Stulle
*/
	// ==> push small files [sivka] - Stulle
	m_bEnablePushSmallFile = thePrefs.GetEnablePushSmallFile();
	m_iPushSmallFileBoost = thePrefs.GetPushSmallFileBoost();
	// <== push small files [sivka] - Stulle
	m_bEnablePushRareFile = thePrefs.enablePushRareFile; // push rare file - Stulle

/*	// ==> FunnyNick Tag - Stulle
	m_iFnTag = thePrefs.GetFnTag();
	m_sFnCustomTag = thePrefs.m_sFnCustomTag;
	m_bFnTagAtEnd = thePrefs.GetFnTagAtEnd();
	// <== FunnyNick Tag - Stulle

	// ==> Quick start [TPT] - Stulle
	m_bQuickStart = thePrefs.GetQuickStart();
	m_iQuickStartMaxTime = (int)(thePrefs.GetQuickStartMaxTime());
	m_iQuickStartMaxConnPerFive = (int)(thePrefs.GetQuickStartMaxConnPerFive());
	m_iQuickStartMaxConn = (int)(thePrefs.GetQuickStartMaxConn());
	m_iQuickStartMaxConnPerFiveBack = (int)(thePrefs.GetQuickStartMaxConnPerFiveBack());
	m_iQuickStartMaxConnBack = (int)(thePrefs.GetQuickStartMaxConnBack());
	m_bQuickStartAfterIPChange = thePrefs.GetQuickStartAfterIPChange();
	// <== Quick start [TPT] - Stulle
	m_bIsreaskSourceAfterIPChange = thePrefs.IsRASAIC(); // Xman -Reask sources after IP change- v2 - Stulle
	m_iReAskFileSrc = (thePrefs.GetReAskTimeDif() + FILEREASKTIME)/60000; // Timer for ReAsk File Sources - Stulle

	// ==> Anti Uploader Ban - Stulle
	m_iAntiUploaderBanLimit = thePrefs.GetAntiUploaderBanLimit();
	m_iAntiUploaderBanCase = thePrefs.GetAntiUploaderBanCase();
	// <== Anti Uploader Ban - Stulle
*/
	showSrcInTitle = thePrefs.ShowSrcOnTitle(); // Show sources on title - Stulle
//	m_bPsFilesRed = thePrefs.GetPsFilesRed(); // draw PS files red - Stulle
	m_bFriendsBlue = thePrefs.GetFriendsBlue(); // draw friends blue - Stulle
	m_bShowGlobalHL = thePrefs.GetShowGlobalHL(); // show global HL - Stulle
	m_bShowFileHLconst = thePrefs.GetShowFileHLconst(); // show HL per file constantaniously - Stulle
	m_bShowInMSN7 = thePrefs.GetShowMSN7(); // Show in MSN7 [TPT] - Stulle
//	m_bTrayComplete = thePrefs.GetTrayComplete(); // Completed in Tray - Stulle
	m_bCountWCSessionStats = thePrefs.GetCountWCSessionStats(); // Show WC session stats [MorphXT] - Stulle

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
/*
	// ==> TBH: minimule - Stulle
	m_bShowMM = thePrefs.IsMiniMuleEnabled();
	m_bMMLives = thePrefs.GetMiniMuleLives();
	m_iMMUpdateTime = thePrefs.GetMiniMuleUpdate();
	m_iMMTrans = thePrefs.GetMiniMuleTransparency();
	m_bMMCompl = thePrefs.m_bMMCompl;
	m_bMMOpen = thePrefs.m_bMMOpen;
	// <== TBH: minimule - Stulle
*/
	m_bSUQWT = thePrefs.SaveUploadQueueWaitTime(); // SUQWT [Moonlight/EastShare/ MorphXT] - Stulle
/*	// ==> Spread Credits Slot - Stulle
	m_bSpreadCreditsSlot = thePrefs.SpreadCreditsSlot;
	m_iSpreadCreditsSlotCounter = (int)(thePrefs.SpreadCreditsSlotCounter);
	// <== Spread Credits Slot - Stulle
*/	// ==> Global Source Limit [Max/Stulle] - Stulle
	m_bGlobalHL = thePrefs.IsUseGlobalHL();
	m_iGlobalHL = thePrefs.GetGlobalHL();
	m_bGlobalHlAggro = thePrefs.IsUseAgressiveMode();
	m_bGlobalHlDefault = thePrefs.GetGlobalHlDefault();
	// <== Global Source Limit [Max/Stulle] - Stulle

	CPropertyPage::OnInitDialog();

	// ==> push small files [sivka] - Stulle
	InitWindowStyles(this);
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
		((CSliderCtrl*)GetDlgItem(IDC_PUSHSMALL_SLIDER))->SetRange(1, PARTSIZE, TRUE);
		((CSliderCtrl*)GetDlgItem(IDC_PUSHSMALL_SLIDER))->SetPos(thePrefs.GetPushSmallFileSize());
		ShowPushSmallFileValues();
		// <== push small files [sivka] - Stulle
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
/*
	// ==> Sivka-Ban - Stulle
	thePrefs.enableSivkaBan = m_bEnableSivkaBan;
	thePrefs.SivkaAskTime = m_iSivkaAskTime;
	thePrefs.SivkaAskCounter = m_iSivkaAskCounter;
	thePrefs.SivkaAskLog = m_bSivkaAskLog;
	// <== Sivka-Ban - Stulle
	// ==> ban systems optional - Stulle
	thePrefs.enableAntiLeecher = m_bEnableAntiLeecher; //MORPH - Added by IceCream, enable Anti-leecher
	thePrefs.m_bBadModString = m_bBadModString;
	thePrefs.m_bBadNickBan = m_bBadNickBan;
	thePrefs.m_bGhostMod = m_bGhostMod;
	thePrefs.m_bAntiModIdFaker = m_bAntiModIdFaker;
	thePrefs.m_bAntiNickThief = m_bAntiNickThief; // AntiNickThief - Stulle
	thePrefs.m_bEmptyNick = m_bEmptyNick;
	thePrefs.m_bFakeEmule = m_bFakeEmule;
	thePrefs.m_bHiddenStr = m_bHiddenStr;
	// <== ban systems optional - Stulle
	thePrefs.enableAntiCreditHack = m_bEnableAntiCreditHack; //MORPH - Added by IceCream, enable Anti-CreditHack
	thePrefs.m_dwClientBanTime = m_iClientBanTime*3600000; // adjust ClientBanTime - Stulle
*/
	// ==> push small files [sivka] - Stulle
	thePrefs.enablePushSmallFile = m_bEnablePushSmallFile;
	thePrefs.m_iPushSmallBoost = m_iPushSmallFileBoost;
	CString strBuffer;
	((CSliderCtrl*)GetDlgItem(IDC_PUSHSMALL_SLIDER))->SetRange(1, PARTSIZE, TRUE);
	thePrefs.m_iPushSmallFiles = ((CSliderCtrl*)GetDlgItem(IDC_PUSHSMALL_SLIDER))->GetPos();
	// <== push small files [sivka] - Stulle
	thePrefs.enablePushRareFile = m_bEnablePushRareFile; // push rare file - Stulle
/*
	// ==> FunnyNick Tag - Stulle
	if(thePrefs.FnTagMode != m_iFnTag){
		thePrefs.FnTagMode = m_iFnTag;
	}
	_stprintf (thePrefs.m_sFnCustomTag,_T("%s"), m_sFnCustomTag);
	thePrefs.m_bFnTagAtEnd = m_bFnTagAtEnd;
	// <== FunnyNick Tag - Stulle

	// ==> Quick start [TPT] - Stulle
	thePrefs.m_bQuickStart = m_bQuickStart;
	thePrefs.m_iQuickStartMaxTime = m_iQuickStartMaxTime;
	thePrefs.m_iQuickStartMaxConnPerFive = m_iQuickStartMaxConnPerFive;
	thePrefs.m_iQuickStartMaxConn = m_iQuickStartMaxConn;
	thePrefs.m_iQuickStartMaxConnPerFiveBack = m_iQuickStartMaxConnPerFiveBack;
	thePrefs.m_iQuickStartMaxConnBack = m_iQuickStartMaxConnBack;
	thePrefs.m_bQuickStartAfterIPChange = m_bQuickStartAfterIPChange;
	// <== Quick start [TPT] - Stulle
	thePrefs.m_breaskSourceAfterIPChange = m_bIsreaskSourceAfterIPChange; // Xman -Reask sources after IP change- v2 - Stulle
	thePrefs.m_uReAskTimeDif = (m_iReAskFileSrc-29)*60000; // Timer for ReAsk File Sources - Stulle

	// ==> Anti Uploader Ban - Stulle
	thePrefs.m_iAntiUploaderBanLimit = m_iAntiUploaderBanLimit;
	thePrefs.AntiUploaderBanCaseMode = m_iAntiUploaderBanCase;
	// <== Anti Uploader Ban - Stulle
*/
	// ==> CreditSystems [EastShare/ MorphXT] - Stulle
	if(thePrefs.creditSystemMode != m_iCreditSystem){
		thePrefs.creditSystemMode = m_iCreditSystem;
		theApp.clientcredits->ResetCheckScoreRatio();
	}
	// <== CreditSystems [EastShare/ MorphXT] - Stulle

	thePrefs.showSrcInTitle = showSrcInTitle; // Show sources on title - Stulle
//	thePrefs.PsFilesRed = m_bPsFilesRed; // draw PS files red - Stulle
	thePrefs.FriendsBlue = m_bFriendsBlue; // draw friends blue - Stulle
	thePrefs.ShowGlobalHL = m_bShowGlobalHL; // show global HL - Stulle
	thePrefs.ShowFileHLconst = m_bShowFileHLconst; // show HL per file constantaniously - Stulle
	thePrefs.m_bShowInMSN7 = m_bShowInMSN7; // Show in MSN7 [TPT] - Stulle
//	thePrefs.m_bTrayComplete = m_bTrayComplete; // Completed in Tray - Stulle
	thePrefs.m_bCountWCSessionStats = m_bCountWCSessionStats; // Show WC session stats [MorphXT] - Stulle

	// ==> file settings - Stulle
	thePrefs.m_EnableAutoDropNNSDefault = m_bEnableAutoDropNNSDefault;
	thePrefs.m_AutoNNS_TimerDefault = (m_iAutoNNS_TimerDefault*1000);
	thePrefs.m_MaxRemoveNNSLimitDefault = m_iMaxRemoveNNSLimitDefault;
	thePrefs.m_EnableAutoDropFQSDefault = m_bEnableAutoDropFQSDefault;
	thePrefs.m_AutoFQS_TimerDefault = (m_iAutoFQS_TimerDefault*1000);
	thePrefs.m_MaxRemoveFQSLimitDefault = m_iMaxRemoveFQSLimitDefault;
	thePrefs.m_EnableAutoDropQRSDefault = m_bEnableAutoDropQRSDefault;
	thePrefs.m_AutoHQRS_TimerDefault = (m_iAutoHQRS_TimerDefault*1000);
	thePrefs.m_MaxRemoveQRSDefault = m_iMaxRemoveQRSDefault;
	thePrefs.m_MaxRemoveQRSLimitDefault = m_iMaxRemoveQRSLimitDefault;
	thePrefs.m_bHQRXmanDefault = m_iHQRXmanDefault == 1;
	// <== file settings - Stulle
/*
	// ==> TBH: minimule - Stulle
	thePrefs.m_bMiniMule = m_bShowMM;
	thePrefs.SetMiniMuleLives(m_bMMLives);
	thePrefs.m_iMiniMuleUpdate = m_iMMUpdateTime;
	thePrefs.SetMiniMuleTransparency(m_iMMTrans);
	thePrefs.m_bMMCompl = m_bMMCompl;
	thePrefs.m_bMMOpen = m_bMMOpen;
	// <== TBH: minimule - Stulle
*/
	thePrefs.m_bSaveUploadQueueWaitTime = m_bSUQWT; // SUQWT [Moonlight/EastShare/ MorphXT] - Stulle
/*	// ==> Spread Credits Slot - Stulle
	thePrefs.SpreadCreditsSlot = m_bSpreadCreditsSlot;
	thePrefs.SpreadCreditsSlotCounter = m_iSpreadCreditsSlotCounter;
	// <== Spread Credits Slot - Stulle
*/	// ==> Global Source Limit [Max/Stulle] - Stulle
	if (thePrefs.GetGlobalHL() != m_iGlobalHL ||
		thePrefs.IsUseGlobalHL() != m_bGlobalHL)
	{
		thePrefs.m_bGlobalHL = m_bGlobalHL;
		thePrefs.m_iGlobalHL = m_iGlobalHL;
		if(thePrefs.GetPassiveMode())
		{
			thePrefs.SetPassiveMode(false);
			thePrefs.SetGlobalHLUpdateTimer(50);
//			AddDebugLogLine(true,_T("{GSL} Global Source Limit settings have changed! Disabled PassiveMode!"));
			AddLogLine(true,_T("{GSL} Global Source Limit settings have changed! Disabled PassiveMode!"));
		}
	}
	thePrefs.m_bUseAgressiveMode = m_bGlobalHlAggro;
	thePrefs.m_bGlobalHlDefault = m_bGlobalHlDefault;
	// <== Global Source Limit [Max/Stulle] - Stulle

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
		GetDlgItem(IDC_WARNING)->SetWindowText(GetResString(IDS_TWEAKS_WARNING));
/*
		// ==> Sivka-Ban - Stulle
		if (m_htiEnableSivkaBan) m_ctrlTreeOptions.SetItemText(m_htiEnableSivkaBan, GetResString(IDS_SIVKA_BAN));
		if (m_htiSivkaAskTime) m_ctrlTreeOptions.SetEditLabel(m_htiSivkaAskTime, GetResString(IDS_SIVKA_ASK_TIME));
		if (m_htiSivkaAskCounter) m_ctrlTreeOptions.SetEditLabel(m_htiSivkaAskCounter, GetResString(IDS_SIVKA_ASK_COUNTER));
		if (m_htiSivkaAskLog) m_ctrlTreeOptions.SetItemText(m_htiSivkaAskLog, GetResString(IDS_SIVKA_ASK_LOG));
		// <== Sivka-Ban - Stulle
		// ==> ban systems optional - Stulle
		if (m_htiEnableAntiLeecher) m_ctrlTreeOptions.SetItemText(m_htiEnableAntiLeecher, GetResString(IDS_SUC_ENABLED)); //MORPH - Added by IceCream, enable Anti-leecher
		if (m_htiBadModString) m_ctrlTreeOptions.SetItemText(m_htiBadModString, GetResString(IDS_BAD_MOD_STRING));
		if (m_htiBadNickBan) m_ctrlTreeOptions.SetItemText(m_htiBadNickBan, GetResString(IDS_BAD_NICK_BAN));
		if (m_htiGhostMod) m_ctrlTreeOptions.SetItemText(m_htiGhostMod, GetResString(IDS_GHOST_MOD));
		if (m_htiAntiModIdFaker) m_ctrlTreeOptions.SetItemText(m_htiAntiModIdFaker, GetResString(IDS_ANTI_MOD_FAKER));
		if (m_htiAntiNickThief) m_ctrlTreeOptions.SetItemText(m_htiAntiNickThief, GetResString(IDS_ANTI_NICK_THIEF)); // AntiNickThief - Stulle
		if (m_htiEmptyNick) m_ctrlTreeOptions.SetItemText(m_htiEmptyNick, GetResString(IDS_EMPTY_NICK));
		if (m_htiFakeEmule) m_ctrlTreeOptions.SetItemText(m_htiFakeEmule, GetResString(IDS_FAKE_EMULE));
		if (m_htiHiddenStr) m_ctrlTreeOptions.SetItemText(m_htiHiddenStr, GetResString(IDS_HIDDEN_STR));
		// <== ban systems optional - Stulle
		if (m_htiEnableAntiCreditHack) m_ctrlTreeOptions.SetItemText(m_htiEnableAntiCreditHack, GetResString(IDS_ANTI_CREDITHACK)); //MORPH - Added by IceCream, enable Anti-CreditHack
		if (m_htiClientBanTime) m_ctrlTreeOptions.SetEditLabel(m_htiClientBanTime, GetResString(IDS_CLIENT_BAN_TIME)); // adjust ClientBanTime - Stulle
*/
		// ==> push small files [sivka] - Stulle
		if (m_htiEnablePushSmallFile) m_ctrlTreeOptions.SetItemText(m_htiEnablePushSmallFile, GetResString(IDS_PUSH_SMALL));
		if (m_htiPushSmallFileBoost) m_ctrlTreeOptions.SetEditLabel(m_htiPushSmallFileBoost, GetResString(IDS_PUSH_SMALL_BOOST));
		// <== push small files [sivka] - Stulle
		if (m_htiEnablePushRareFile) m_ctrlTreeOptions.SetItemText(m_htiEnablePushRareFile, GetResString(IDS_PUSH_RARE)); // push rare file - Stulle
/*
		// ==> FunnyNick Tag - Stulle
		if (m_htiFnCustomTag) m_ctrlTreeOptions.SetEditLabel(m_htiFnCustomTag, GetResString(IDS_SET_CUSTOM_TAG));
		if (m_htiFnTagAtEnd) m_ctrlTreeOptions.SetItemText(m_htiFnTagAtEnd, GetResString(IDS_FN_TAG_AT_END));
		// <== FunnyNick Tag - Stulle

		// ==> Quick start [TPT] - Stulle
		if (m_htiQuickStart) m_ctrlTreeOptions.SetItemText(m_htiQuickStart, GetResString(IDS_QUICK_START));
		if (m_htiQuickStartMaxTime) m_ctrlTreeOptions.SetEditLabel(m_htiQuickStartMaxTime, GetResString(IDS_QUICK_START_MAX_TIME));
		if (m_htiQuickStartMaxConnPerFive) m_ctrlTreeOptions.SetEditLabel(m_htiQuickStartMaxConnPerFive, GetResString(IDS_QUICK_START_MAX_CONN_PER_FIVE));
		if (m_htiQuickStartMaxConn) m_ctrlTreeOptions.SetEditLabel(m_htiQuickStartMaxConn, GetResString(IDS_QUICK_START_MAX_CONN));
		if (m_htiQuickStartMaxConnPerFiveBack) m_ctrlTreeOptions.SetEditLabel(m_htiQuickStartMaxConnPerFiveBack, GetResString(IDS_QUICK_START_MAX_CONN_PER_FIVE_BACK));
		if (m_htiQuickStartMaxConnBack) m_ctrlTreeOptions.SetEditLabel(m_htiQuickStartMaxConnBack, GetResString(IDS_QUICK_START_MAX_CONN_BACK));
		if (m_htiQuickStartAfterIPChange) m_ctrlTreeOptions.SetItemText(m_htiQuickStartAfterIPChange, GetResString(IDS_QUICK_START_AFTER_IP_CHANGE));
		// <== Quick start [TPT] - Stulle
		if (m_htiIsreaskSourceAfterIPChange) m_ctrlTreeOptions.SetItemText(m_htiIsreaskSourceAfterIPChange, GetResString(IDS_RSAIC)); // Xman -Reask sources after IP change- v2 - Stulle
		if (m_htiReAskFileSrc) m_ctrlTreeOptions.SetEditLabel(m_htiReAskFileSrc, GetResString(IDS_REASK_FILE_SRC)); // Timer for ReAsk File Sources - Stulle

		// ==> Anti Uploader Ban - Stulle
		if (m_htiAntiUploaderBanLimit) m_ctrlTreeOptions.SetEditLabel(m_htiAntiUploaderBanLimit, GetResString(IDS_UNBAN_UPLOADER));
		// <== Anti Uploader Ban - Stulle
*/
		if (m_htiShowSrcOnTitle) m_ctrlTreeOptions.SetItemText(m_htiShowSrcOnTitle, GetResString(IDS_SHOWSRCONTITLE)); // Show sources on title - Stulle
//		if (m_htiPsFilesRed) m_ctrlTreeOptions.SetItemText(m_htiPsFilesRed, GetResString(IDS_PS_FILES_RED)); // draw PS files red - Stulle
		if (m_htiFriendsBlue) m_ctrlTreeOptions.SetItemText(m_htiFriendsBlue, GetResString(IDS_FRIENDS_BLUE)); // draw friends blue - Stulle
		if (m_htiShowGlobalHL) m_ctrlTreeOptions.SetItemText(m_htiShowGlobalHL, GetResString(IDS_SHOW_GLOBAL_HL)); // show global HL - Stulle
		if (m_htiShowFileHLconst) m_ctrlTreeOptions.SetItemText(m_htiShowFileHLconst, GetResString(IDS_SHOW_FILE_HL_CONST)); // show HL per file constantaniously - Stulle
		if (m_htiShowInMSN7) m_ctrlTreeOptions.SetItemText(m_htiShowInMSN7, GetResString(IDS_SHOWINMSN7)); // Show in MSN7 [TPT] - Stulle
//		if (m_htiTrayComplete) m_ctrlTreeOptions.SetItemText(m_htiTrayComplete, GetResString(IDS_TRAY_COMPLETE)); // Completed in Tray - Stulle
		if (m_htiCountWCSessionStats) m_ctrlTreeOptions.SetItemText(m_htiCountWCSessionStats, GetResString(IDS_COUNT_WC_STATS)); // Show WC session stats [MorphXT] - Stulle

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
/*
		// ==> TBH: minimule - Stulle
		if (m_htiMMGroup) m_ctrlTreeOptions.SetItemText(m_htiMMGroup, _T("TBH Mini-Mule"));
		if (m_htiShowMM) m_ctrlTreeOptions.SetItemText(m_htiShowMM, GetResString(IDS_MM_SHOW));
		if (m_htiMMLives) m_ctrlTreeOptions.SetItemText(m_htiMMLives, GetResString(IDS_MM_LIVES));
		if (m_htiMMUpdateTime) m_ctrlTreeOptions.SetEditLabel(m_htiMMUpdateTime, GetResString(IDS_MM_UPDATE_TIME));
		if (m_htiMMTrans) m_ctrlTreeOptions.SetEditLabel(m_htiMMTrans, GetResString(IDS_MM_TRANS));
		if (m_htiMMCompl) m_ctrlTreeOptions.SetItemText(m_htiMMCompl, GetResString(IDS_MM_COMPL));
		if (m_htiMMOpen) m_ctrlTreeOptions.SetItemText(m_htiMMOpen, GetResString(IDS_MM_OPEN));
		// <== TBH: minimule - Stulle
*/
		if (m_htiSUQWT) m_ctrlTreeOptions.SetItemText(m_htiSUQWT, GetResString(IDS_SUQWT)); // SUQWT [Moonlight/EastShare/ MorphXT] - Stulle
/*		// ==> Spread Credits Slot - Stulle
		if (m_htiSpreadCreditsSlot) m_ctrlTreeOptions.SetItemText(m_htiSpreadCreditsSlot, GetResString(IDS_SPREAD_CREDITS_SLOT));
		if (m_htiSpreadCreditsSlotCounter) m_ctrlTreeOptions.SetEditLabel(m_htiSpreadCreditsSlotCounter, GetResString(IDS_SPREAD_CREDITS_SLOT_COUNTER));
		// <== Spread Credits Slot - Stulle
*/		// ==> Global Source Limit [Max/Stulle] - Stulle
		if (m_htiGlobalHL) m_ctrlTreeOptions.SetItemText(m_htiGlobalHL, GetResString(IDS_ENABLE));
		if (m_htiGlobalHlLimit) m_ctrlTreeOptions.SetEditLabel(m_htiGlobalHlLimit, GetResString(IDS_GLOBAL_HL_LIMIT));
		if (m_htiGlobalHlAggro) m_ctrlTreeOptions.SetItemText(m_htiGlobalHlAggro, GetResString(IDS_GLOBAL_HL_AGGRO));
		if (m_htiGlobalHlDefault) m_ctrlTreeOptions.SetItemText(m_htiGlobalHlDefault, GetResString(IDS_GLOBAL_HL_DEFAULT));
		// <== Global Source Limit [Max/Stulle] - Stulle
	}

}

void CPPgScar::OnDestroy()
{
	m_ctrlTreeOptions.DeleteAllItems();
	m_ctrlTreeOptions.DestroyWindow();
	m_bInitializedTreeOpts = false;
/*
	m_htiSecu = NULL;
	// ==> Sivka-Ban - Stulle
	m_htiSivkaBanGroup = NULL;
	m_htiEnableSivkaBan = NULL;
	m_htiSivkaAskTime = NULL;
	m_htiSivkaAskCounter = NULL;
	m_htiSivkaAskLog = NULL;
	// <== Sivka-Ban - Stulle
	// ==> ban systems optional - Stulle
	m_htiAntiLeecherGroup = NULL;
	m_htiEnableAntiLeecher = NULL; //MORPH - Added by IceCream, activate Anti-leecher
	m_htiBadModString = NULL;
	m_htiBadNickBan = NULL;
	m_htiGhostMod = NULL;
	m_htiAntiModIdFaker = NULL;
	m_htiAntiNickThief = NULL; // AntiNickThief - Stulle
	m_htiEmptyNick = NULL;
	m_htiFakeEmule = NULL;
	m_htiHiddenStr = NULL;
	// <== ban systems optional - Stulle
	m_htiEnableAntiCreditHack = NULL; //MORPH - Added by IceCream, activate Anti-CreditHack
	m_htiClientBanTime = NULL; // adjust ClientBanTime - Stulle
*/
	m_htiPush = NULL; // push files - Stulle
	// ==> push small files [sivka] - Stulle
	m_htiEnablePushSmallFile = NULL;
	m_htiPushSmallFileBoost = NULL;
	// <== push small files [sivka] - Stulle
	m_htiEnablePushRareFile = NULL; // push rare file - Stulle
/*
	// ==> FunnyNick Tag - Stulle
	m_htiFnTag = NULL;
	m_htiNoTag = NULL;
	m_htiShortTag = NULL;
	m_htiFullTag = NULL;
	m_htiCustomTag = NULL;
	m_htiFnCustomTag = NULL;
	m_htiFnTagAtEnd = NULL;
	// <== FunnyNick Tag - Stulle

	m_htiConTweaks = NULL;
	// ==> Quick start [TPT] - Stulle
	m_htiQuickStartGroup = NULL;
	m_htiQuickStart = NULL;
	m_htiQuickStartMaxTime = NULL;
	m_htiQuickStartMaxConnPerFive = NULL;
	m_htiQuickStartMaxConn = NULL;
	m_htiQuickStartMaxConnPerFiveBack = NULL;
	m_htiQuickStartMaxConnBack = NULL;
	m_htiQuickStartAfterIPChange = NULL;
	// <== Quick start [TPT] - Stulle
	m_htiReAskFileSrc = NULL; // Timer for ReAsk File Sources - Stulle
	m_htiIsreaskSourceAfterIPChange = NULL; // Xman -Reask sources after IP change- v2 - Stulle

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
	m_htiShowSrcOnTitle = NULL; // Show sources on title - Stulle
//	m_htiPsFilesRed = NULL; // draw PS files red - Stulle
	m_htiFriendsBlue = NULL; // draw friends blue - Stulle
	m_htiShowGlobalHL = NULL; // show global HL - Stulle
	m_htiShowFileHLconst = NULL; // show HL per file constantaniously - Stulle
	m_htiShowInMSN7 = NULL; // Show in MSN7 [TPT] - Stulle
//	m_htiTrayComplete = NULL; // Completed in Tray - Stulle
	m_htiCountWCSessionStats = NULL; // Show WC session stats [MorphXT] - Stulle

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
/*
	// ==> TBH: minimule - Stulle
	m_htiMMGroup = NULL;
	m_htiShowMM = NULL;
	m_htiMMLives = NULL;
	m_htiMMUpdateTime = NULL;
	m_htiMMTrans = NULL;
	m_htiMMCompl = NULL;
	m_htiMMOpen = NULL;
	// <== TBH: minimule - Stulle
*/
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
	m_htiGlobalHlAggro = NULL;
	m_htiGlobalHlDefault = NULL;
	// <== Global Source Limit [Max/Stulle] - Stulle

	CPropertyPage::OnDestroy();
}
LRESULT CPPgScar::OnTreeOptsCtrlNotify(WPARAM wParam, LPARAM lParam)
{
	if (wParam == IDC_SCAR_OPTS){
		TREEOPTSCTRLNOTIFY* pton = (TREEOPTSCTRLNOTIFY*)lParam;

	BOOL bCheck;

		// ==> ban systems optional - Stulle
		/*
		if (m_htiEnableAntiLeecher && pton->hItem == m_htiEnableAntiLeecher)
		{
			if (m_ctrlTreeOptions.GetCheckBox(m_htiEnableAntiLeecher, bCheck))
			{
				if (m_htiBadModString)		m_ctrlTreeOptions.SetCheckBoxEnable(m_htiBadModString, bCheck);
				if (m_htiBadNickBan)		m_ctrlTreeOptions.SetCheckBoxEnable(m_htiBadNickBan, bCheck);
				if (m_htiGhostMod)			m_ctrlTreeOptions.SetCheckBoxEnable(m_htiGhostMod, bCheck);
				if (m_htiAntiModIdFaker)	m_ctrlTreeOptions.SetCheckBoxEnable(m_htiAntiModIdFaker, bCheck);
				if (m_htiAntiNickThief)		m_ctrlTreeOptions.SetCheckBoxEnable(m_htiAntiNickThief, bCheck);
				if (m_htiEmptyNick)			m_ctrlTreeOptions.SetCheckBoxEnable(m_htiEmptyNick, bCheck);
				if (m_htiFakeEmule)			m_ctrlTreeOptions.SetCheckBoxEnable(m_htiFakeEmule, bCheck);
				if (m_htiHiddenStr)			m_ctrlTreeOptions.SetCheckBoxEnable(m_htiHiddenStr, bCheck);
			}
		}
		// <== ban systems optional - Stulle

		// ==> Quick start [TPT] - Stulle
		if (m_htiQuickStart && pton->hItem == m_htiQuickStart)
		{
			if (m_ctrlTreeOptions.GetCheckBox(m_htiQuickStart, bCheck))
			{
				if (m_htiQuickStartAfterIPChange)	m_ctrlTreeOptions.SetCheckBoxEnable(m_htiQuickStartAfterIPChange, bCheck);
			}
		}
		*/
		// <== Quick start [TPT] - Stulle

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
	GetDlgItem(IDC_PUSHSMALL)->SetWindowText(CastItoXBytes(float(((CSliderCtrl*)GetDlgItem(IDC_PUSHSMALL_SLIDER))->GetPos())));
}
// <== push small files [sivka] - Stulle