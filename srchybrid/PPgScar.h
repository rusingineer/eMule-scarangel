#pragma once
#include "preferences.h"
#include "TreeOptionsCtrlEx.h"
// CPPgScar dialog

class CPPgScar : public CPropertyPage
{
	DECLARE_DYNAMIC(CPPgScar)

public:
	CPPgScar();
	virtual ~CPPgScar();

// Dialog Data
	enum { IDD = IDD_PPG_SCAR };
protected:
	// ==> push small files [sivka] - Stulle
	bool m_bEnablePushSmallFile;
	int  m_iPushSmallFileBoost;
	// <== push small files [sivka] - Stulle
	bool m_bEnablePushRareFile; // push rare file - Stulle

	// ==> FunnyNick [SiRoB/Stulle] - Stulle
	bool m_bFnActive;
	int m_iFnTag;
	CString   m_sFnCustomTag;
	bool m_bFnTagAtEnd;
	// <== FunnyNick [SiRoB/Stulle] - Stulle

	// ==> Quick start [TPT] - Max
	bool m_bQuickStart;
	int m_iQuickStartMaxTime;
	int m_iQuickStartMaxConnPerFive;
	int m_iQuickStartMaxConn;
	int m_iQuickStartMaxConnPerFiveBack;
	int m_iQuickStartMaxConnBack;
	bool m_bQuickStartAfterIPChange;
	// <== Quick start [TPT] - Max
//	int m_iReAskFileSrc; // Timer for ReAsk File Sources - Stulle
	bool m_bACC; // ACC [Max/WiZaRd] - Max
/*
	// ==> Anti Uploader Ban - Stulle
	int m_iAntiUploaderBanLimit;
	int m_iAntiUploaderBanCase;
	// <== Anti Uploader Ban - Stulle
*/

	// ==> CreditSystems [EastShare/ MorphXT] - Stulle
	int m_iCreditSystem;
	// <== CreditSystems [EastShare/ MorphXT] - Stulle

	// ==> CPU/MEM usage [$ick$/Stulle] - Max
	bool m_bSysInfo;
	bool m_bSysInfoGlobal;
	// <== CPU/MEM usage [$ick$/Stulle] - Max
	bool showSrcInTitle; // Show sources on title - Stulle
//	bool m_bPsFilesRed; // draw PS files red - Stulle
	bool m_bFriendsBlue; // draw friends blue - Stulle
	bool m_bShowGlobalHL; // show global HL - Stulle
	bool m_bShowFileHLconst; // show HL per file constantaniously - Stulle
	bool m_bShowInMSN7; // Show in MSN7 [TPT] - Stulle
	bool m_bQueueProgressBar; // Client queue progress bar [Commander] - Stulle
//	bool m_bTrayComplete; // Completed in Tray - Stulle
	bool m_bShowClientPercentage; // Show Client Percentage [Commander/MorphXT] - Mondgott

	// ==> file settings - Stulle
	bool m_bEnableAutoDropNNSDefault;
	int m_iAutoNNS_TimerDefault;
	int m_iMaxRemoveNNSLimitDefault;
	bool m_bEnableAutoDropFQSDefault;
	int m_iAutoFQS_TimerDefault;
	int m_iMaxRemoveFQSLimitDefault;
	bool m_bEnableAutoDropQRSDefault;
	int m_iAutoHQRS_TimerDefault;
	int m_iMaxRemoveQRSDefault;
	int m_iMaxRemoveQRSLimitDefault;
	int m_iHQRXmanDefault;
	bool m_bGlobalHlDefault;
	// <== file settings - Stulle

	// ==> TBH: minimule - Max
	bool m_bShowMM;
	bool m_bMMLives;
	int m_iMMUpdateTime;
	int m_iMMTrans;
	bool m_bMMBanner;
	bool m_bMMCompl;
	bool m_bMMOpen;
	// <== TBH: minimule - Max

	bool m_bSUQWT; // SUQWT [Moonlight/EastShare/ MorphXT] - Stulle
/*	// ==> Spread Credits Slot - Stulle
	bool m_bSpreadCreditsSlot;
	int m_iSpreadCreditsSlotCounter;
	// <== Spread Credits Slot - Stulle
*/	// ==> Global Source Limit [Max/Stulle] - Stulle
	bool m_bGlobalHL;
	int m_iGlobalHL;
	bool m_bGlobalHlAll;
	// <== Global Source Limit [Max/Stulle] - Stulle

	CTreeOptionsCtrlEx m_ctrlTreeOptions;
	bool m_bInitializedTreeOpts;

	HTREEITEM m_htiPush; // push files - Stulle
	// ==> push small files [sivka] - Stulle
	HTREEITEM m_htiEnablePushSmallFile;
	HTREEITEM m_htiPushSmallFileBoost;
	// <== push small files [sivka] - Stulle
	HTREEITEM m_htiEnablePushRareFile; // push rare file - Stulle

	// ==> FunnyNick [SiRoB/Stulle] - Stulle
	HTREEITEM m_htiFnTag;
	HTREEITEM m_htiFnActive;
	HTREEITEM m_htiFnTagMode;
	HTREEITEM m_htiNoTag;
	HTREEITEM m_htiShortTag;
	HTREEITEM m_htiFullTag;
	HTREEITEM m_htiCustomTag;
	HTREEITEM m_htiFnCustomTag;
	HTREEITEM m_htiFnTagAtEnd;
	// <== FunnyNick [SiRoB/Stulle] - Stulle

	HTREEITEM m_htiConTweaks;
	// ==> Quick start [TPT] - Max
	HTREEITEM m_htiQuickStartGroup;
	HTREEITEM m_htiQuickStart;
	HTREEITEM m_htiQuickStartMaxTime;
	HTREEITEM m_htiQuickStartMaxConnPerFive;
	HTREEITEM m_htiQuickStartMaxConn;
	HTREEITEM m_htiQuickStartMaxConnPerFiveBack;
	HTREEITEM m_htiQuickStartMaxConnBack;
	HTREEITEM m_htiQuickStartAfterIPChange;
	// <== Quick start [TPT] - Max
//	HTREEITEM m_htiReAskFileSrc; // Timer for ReAsk File Sources - Stulle
	HTREEITEM m_htiACC; // ACC [Max/WiZaRd] - Max
/*
	// ==> Anti Uploader Ban - Stulle
	HTREEITEM m_htiAntiUploaderBanLimit;
	HTREEITEM m_htiAntiCase1;
	HTREEITEM m_htiAntiCase2;
	HTREEITEM m_htiAntiCase3;
	// <== Anti Uploader Ban - Stulle
*/
	// ==> CreditSystems [EastShare/ MorphXT] - Stulle
	HTREEITEM m_htiCreditSystem;
	HTREEITEM m_htiOfficialCredit;
	HTREEITEM m_htiLovelaceCredit;
	HTREEITEM m_htiRatioCredit;
	HTREEITEM m_htiPawcioCredit;
	HTREEITEM m_htiESCredit;
	HTREEITEM m_htiSivkaCredit;
	HTREEITEM m_htiSwatCredit;
	HTREEITEM m_htiXmanCredit;
	// <== CreditSystems [EastShare/ MorphXT] - Stulle

	HTREEITEM m_htiDisplay;
	// ==> CPU/MEM usage [$ick$/Stulle] - Max
	HTREEITEM m_htiSysInfoGroup;
	HTREEITEM m_htiSysInfo;
	HTREEITEM m_htiSysInfoGlobal;
	// <== CPU/MEM usage [$ick$/Stulle] - Max
	HTREEITEM m_htiShowSrcOnTitle; // Show sources on title - Stulle
//	HTREEITEM m_htiPsFilesRed; // draw PS files red - Stulle
	HTREEITEM m_htiFriendsBlue; // draw friends blue - Stulle
	HTREEITEM m_htiShowGlobalHL; // show global HL - Stulle
	HTREEITEM m_htiShowFileHLconst; // show HL per file constantaniously - Stulle
	HTREEITEM m_htiShowInMSN7; // Show in MSN7 [TPT] - Stulle
	HTREEITEM m_htiQueueProgressBar; // Client queue progress bar [Commander] - Stulle
//	HTREEITEM m_htiTrayComplete; // Completed in Tray - Stulle
	HTREEITEM m_htiShowClientPercentage; // Show Client Percentage [Commander/MorphXT] - Mondgott

	// ==> file settings - Stulle
	HTREEITEM m_htiFileDefaults;
	HTREEITEM m_htiAutoNNS;
	HTREEITEM m_htiAutoNNSTimer;
	HTREEITEM m_htiAutoNNSLimit;
	HTREEITEM m_htiAutoFQS;
	HTREEITEM m_htiAutoFQSTimer;
	HTREEITEM m_htiAutoFQSLimit;
	HTREEITEM m_htiAutoQRS;
	HTREEITEM m_htiAutoQRSTimer;
	HTREEITEM m_htiAutoQRSMax;
	HTREEITEM m_htiAutoQRSLimit;
	HTREEITEM m_htiAutoQRSWay;
	HTREEITEM m_htiHQRXman;
	HTREEITEM m_htiHQRSivka;
	HTREEITEM m_htiGlobalHlDefault;
	// <== file settings - Stulle

	// ==> TBH: minimule - Max
	HTREEITEM m_htiMMGroup;
	HTREEITEM m_htiShowMM;
	HTREEITEM m_htiMMLives;
	HTREEITEM m_htiMMUpdateTime;
	HTREEITEM m_htiMMTrans;
	HTREEITEM m_htiMMBanner;
	HTREEITEM m_htiMMCompl;
	HTREEITEM m_htiMMOpen;
	// <== TBH: minimule - Max

	HTREEITEM m_htiMisc;
	HTREEITEM m_htiSUQWT; // SUQWT [Moonlight/EastShare/ MorphXT] - Stulle
/*	// ==> Spread Credits Slot - Stulle
	HTREEITEM m_htiSpreadCreditsSlot;
	HTREEITEM m_htiSpreadCreditsSlotCounter;
	// <== Spread Credits Slot - Stulle,
*/	// ==> Global Source Limit [Max/Stulle] - Stulle
	HTREEITEM m_htiGlobalHlGroup;
	HTREEITEM m_htiGlobalHL;
	HTREEITEM m_htiGlobalHlLimit;
	HTREEITEM m_htiGlobalHlAll;
	HTREEITEM m_htiGlobalHlAggro;
	// <== Global Source Limit [Max/Stulle] - Stulle

	// ==> push small files [sivka] - Stulle
	uint32 m_iPushSmallFiles;
	void ShowPushSmallFileValues();
	// <== push small files [sivka] - Stulle

	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnDestroy();
	afx_msg LRESULT OnTreeOptsCtrlNotify(WPARAM wParam, LPARAM lParam);
	afx_msg void OnHelp();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	void LoadSettings(void);
	virtual BOOL OnApply();
	virtual BOOL OnInitDialog();
	virtual BOOL OnKillActive();
	afx_msg void OnSettingsChange()			{ SetModified(); }
	afx_msg void OnEnChangeModified() { SetModified();}
	afx_msg void OnTabSelectionChange(NMHDR *pNMHDR, LRESULT *pResult); // Tabbed Preferences [TPT] - Stulle
public:
	void Localize(void);	

	// ==> Tabbed Preferences [TPT] - Stulle
private:
	enum eTab{
	NONE,
	SCAR,
	WEBCACHE,
	BACKUP,
	UPDATE,
	COLOR};
	void SetTab(eTab tab);

	void InitTab();
	void InitControl();	


	// Tab	
	CTabCtrl   m_tabCtr;
	eTab       m_currentTab;
	CImageList m_imageList;

	// ScarAngel
	CStatic		m_strWarning;
	CStatic		m_strPushSmall, m_iPushSmallLabel;
	CSliderCtrl m_iPushSmall;

	// WebCache
	CButton		m_WcCtrlBox;
	CButton		m_bWcDl;
	CButton		m_hideControls;
	CButton		m_WcProxyBox;
	CStatic		m_webcacheAddressStatic;
	CEdit		m_webcacheAddressEdit;
	CStatic		m_webcachePortStatic;
	CEdit		m_webcachePortEdit;
	CButton		m_webcacheTest;
	CStatic		m_blockText;
	CEdit		m_blockEdit;
	CStatic		m_blockLabel;
	CButton		m_TimeOut;
	CButton		m_CacheISP;
	CButton		m_WcDlPersistent;
	CButton		m_Update;
	CButton		m_autoDetect;
	CStatic		m_WrongPort;

	// Backup
	CButton		m_BackupBox;
	CButton		m_Dat;
	CButton		m_Met;
	CButton		m_Ini;
	CButton		m_Part;
	CButton		m_PartMet;
	CButton		m_SelectAll;
	CButton		m_BackupNow;
	CButton		m_AutoBackupBox;
	CButton		m_AutoBackup;
	CButton		m_AutoBackup2;
	CButton		m_Note;
	CButton		m_NoteText;
	// <== Tabbed Preferences [TPT] - Stulle

	// ==> WebCache [WC team/MorphXT] - Stulle/Max
protected:
	bool guardian;
	bool showadvanced;
	void ShowLimitValues();
	bool bCreated, bCreated2;
	afx_msg void OnEnChangeActivatewebcachedownloads();
	afx_msg void OnBnClickedDetectWebCache();
	afx_msg void OnBnClickedAdvancedcontrols();
	afx_msg void OnBnClickedTestProxy(); //JP Proxy Configuration Test
public:
	virtual void UpdateEnableWC()	{ OnEnChangeActivatewebcachedownloads(); }
	virtual bool GetWcDlCheckBox()	{ return m_bWcDl.GetCheck() == BST_CHECKED; }
	// <== WebCache [WC team/MorphXT] - Stulle/Max

	// ==> TBH: Backup [TBH/EastShare/MorphXT] - Stulle
	void Backup(LPCTSTR extensionToBack, BOOL conFirm);
	void Backup3();
	afx_msg void OnBnClickedBackupnow();
	afx_msg void OnBnClickedDat();
	afx_msg void OnBnClickedMet();
	afx_msg void OnBnClickedIni();
	afx_msg void OnBnClickedPart();
	afx_msg void OnBnClickedPartMet();
	afx_msg void OnBnClickedSelectall();
	afx_msg void OnBnClickedAutobackup();
	afx_msg void OnBnClickedAutobackup2();
private:
	void Backup2(LPCTSTR extensionToBack);
	void BackupNowEnable();
	BOOL y2All;
	// <== TBH: Backup [TBH/EastShare/MorphXT] - Stulle
};