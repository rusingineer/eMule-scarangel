//this file is part of eMule
//Copyright (C)2002 Merkur ( devs@emule-project.net / http://www.emule-project.net )
//
//This program is free software; you can redistribute it and/or
//modify it under the terms of the GNU General Public License
//as published by the Free Software Foundation; either
//version 2 of the License, or (at your option) any later version.
//
//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with this program; if not, write to the Free Software
//Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
#pragma once
//#include "BarShader.h"
//Xman
#include "otherfunctions.h"
#include <map>
#include "IP2Country.h" //EastShare - added by AndCycle, IP to Country


// ==> {Webcache} [Max] 
#include "Preferences.h"
#include "WebCache/WebCache.h"
#include "WebCache/WebCacheCryptography.h"
#include "WebCache/WebCacheMFRList.h"

// USING_NAMESPACE(CryptoPP)

enum EWebCacheDownState{
	WCDS_NONE = 0,
	WCDS_WAIT_CLIENT_REPLY,
	WCDS_WAIT_CACHE_REPLY,
	WCDS_DOWNLOADINGVIA,
	WCDS_DOWNLOADINGFROM
};

enum EWebCacheUpState{
	WCUS_NONE = 0,
	WCUS_UPLOADING
};


class CWebCacheDownSocket;
class CWebCacheUpSocket;
// <== {Webcache} [Max] 

class CTag; //Xman Anti-Leecher
class CClientReqSocket;
class CPeerCacheDownSocket;
class CPeerCacheUpSocket;
class CFriend;
class CPartFile;
class CClientCredits;
class CAbstractFile;
class CKnownFile;
class Packet;
class CxImage;
struct Requested_Block_Struct;
class CSafeMemFile;
class CEMSocket;
class CAICHHash;
enum EUtf8Str;

struct Pending_Block_Struct{
	Pending_Block_Struct()
	{
		block = NULL;
		zStream = NULL;
		totalUnzipped = 0;
		fZStreamError = 0;
		fRecovered = 0;
		fQueued = 0;
	}
	Requested_Block_Struct*	block;
	struct z_stream_s*      zStream;       // Barry - Used to unzip packets
	uint32                  totalUnzipped; // Barry - This holds the total unzipped bytes for all packets so far
	UINT					fZStreamError : 1,
							fRecovered    : 1,
							fQueued		  : 3;
};

//Xman
// Maella -Accurate measure of bandwidth: eDonkey data + control, network adapter-
#pragma pack(4 /*Bytes*/)
struct TransferredData {
	uint32	dataLength;
	DWORD	timeStamp; // => based on enkeyDEV(Ottavio84)
};
//Xman end


#pragma pack(1)
struct Requested_File_Struct{
	uchar	  fileid[16];
	uint32	  lastasked;
	uint8	  badrequests;
};
#pragma pack()

enum EUploadState{
	US_UPLOADING,
	US_ONUPLOADQUEUE,
	US_WAITCALLBACK,
	US_CONNECTING,
	US_PENDING,
	US_LOWTOLOWIP,
	US_BANNED,
	US_ERROR,
	US_NONE
};

enum EDownloadState{
	DS_DOWNLOADING,
	DS_ONQUEUE,
	DS_CONNECTED,
	DS_CONNECTING,
	DS_WAITCALLBACK,
	DS_WAITCALLBACKKAD,
	DS_REQHASHSET,
	DS_NONEEDEDPARTS,
	DS_TOOMANYCONNS,
	DS_TOOMANYCONNSKAD,
	DS_LOWTOLOWIP,
	DS_BANNED,
	DS_ERROR,
	DS_NONE,
	DS_REMOTEQUEUEFULL  // not used yet, except in statistics
};

enum EPeerCacheDownState{
	PCDS_NONE = 0,
	PCDS_WAIT_CLIENT_REPLY,
	PCDS_WAIT_CACHE_REPLY,
	PCDS_DOWNLOADING
};

enum EPeerCacheUpState{
	PCUS_NONE = 0,
	PCUS_WAIT_CACHE_REPLY,
	PCUS_UPLOADING
};

enum EChatState{
	MS_NONE,
	MS_CHATTING,
	MS_CONNECTING,
	MS_UNABLETOCONNECT
};

enum EKadState{
	KS_NONE,
	KS_QUEUED_FWCHECK,
	KS_CONNECTING_FWCHECK,
	KS_CONNECTED_FWCHECK,
	KS_QUEUED_BUDDY,
	KS_INCOMING_BUDDY,
	KS_CONNECTING_BUDDY,
	KS_CONNECTED_BUDDY
};

enum EClientSoftware{
	SO_EMULE			= 0,	// default
	SO_CDONKEY			= 1,	// ET_COMPATIBLECLIENT
	SO_XMULE			= 2,	// ET_COMPATIBLECLIENT
	SO_AMULE			= 3,	// ET_COMPATIBLECLIENT
	SO_SHAREAZA			= 4,	// ET_COMPATIBLECLIENT
	SO_MLDONKEY			= 10,	// ET_COMPATIBLECLIENT
	SO_LPHANT			= 20,	// ET_COMPATIBLECLIENT
	// other client types which are not identified with ET_COMPATIBLECLIENT
	SO_EDONKEYHYBRID	= 50,
	SO_EDONKEY,
	SO_OLDEMULE,
	SO_URL,

	SO_WEBCACHE, // {Webcache} [Max] 

	SO_UNKNOWN
};

enum ESecureIdentState{
	IS_UNAVAILABLE		= 0,
	IS_ALLREQUESTSSEND  = 0,
	IS_SIGNATURENEEDED	= 1,
	IS_KEYANDSIGNEEDED	= 2,
};

enum EInfoPacketState{
	IP_NONE				= 0,
	IP_EDONKEYPROTPACK  = 1,
	IP_EMULEPROTPACK	= 2,
	IP_BOTH				= 3,
};

enum ESourceFrom{
	SF_SERVER			= 0,
	SF_KADEMLIA			= 1,
	SF_SOURCE_EXCHANGE	= 2,
	SF_PASSIVE			= 3,
	SF_LINK				= 4,
	SF_SLS				= 5			//Xman SLS
};

// ==> Mod Icons - Stulle
enum EModClient{
	MOD_NONE			= 0,
	MOD_SCAR			= 1,
	MOD_STULLE			= 2,
	MOD_MAXMOD			= 3,
	MOD_XTREME			= 4,
	MOD_MORPH			= 5,
	MOD_EASTSHARE			= 6
};
// <== Mod Icons - Stulle

struct PartFileStamp{
	CPartFile*	file;
	DWORD		timestamp;
};

#define	MAKE_CLIENT_VERSION(mjr, min, upd) \
	((UINT)(mjr)*100U*10U*100U + (UINT)(min)*100U*10U + (UINT)(upd)*100U)

//#pragma pack(2)
class CUpDownClient : public CObject
{
	DECLARE_DYNAMIC(CUpDownClient)

	friend class CUploadQueue;
public:
	void PrintUploadStatus();

	//base
	CUpDownClient(CClientReqSocket* sender = 0);
	CUpDownClient(CPartFile* in_reqfile, uint16 in_port, uint32 in_userid, uint32 in_serverup, uint16 in_serverport, bool ed2kID = false);
	virtual ~CUpDownClient();


	// Maella -Upload Stop Reason-
	enum UpStopReason {USR_NONE, USR_SOCKET, USR_COMPLETEDRANSFER, USR_CANCELLED, USR_DIFFERENT_FILE, USR_EXCEPTION};
	// Maella -Download Stop Reason-
	enum DownStopReason {DSR_NONE, DSR_PAUSED, DSR_NONEEDEDPARTS, DSR_CORRUPTEDBLOCK, DSR_TIMEOUT, DSR_SOCKET, DSR_OUTOFPART, DSR_EXCEPTION};


	void			StartDownload();
	virtual void	CheckDownloadTimeout();
	virtual void	SendCancelTransfer(Packet* packet = NULL);
	virtual bool	IsEd2kClient() const							{ return true; }
	virtual bool	Disconnected(LPCTSTR pszReason, bool bFromSocket = false, UpStopReason reason = USR_NONE); // Maella -Upload Stop Reason-
	virtual bool	TryToConnect(bool bIgnoreMaxCon = false, CRuntimeClass* pClassSocket = NULL);
	virtual bool	Connect();
	virtual void	ConnectionEstablished();
	virtual void	OnSocketConnected(int nErrorCode);
	bool			CheckHandshakeFinished(UINT protocol, UINT opcode) const;
	void			CheckFailedFileIdReqs(const uchar* aucFileHash);
	uint32			GetUserIDHybrid() const							{ return m_nUserIDHybrid; }
	void			SetUserIDHybrid(uint32 val)						{ m_nUserIDHybrid = val; }
	LPCTSTR			GetUserName() const								{ return m_pszUsername; }
	void			SetUserName(LPCTSTR pszNewName);
	uint32			GetIP() const									{ return m_dwUserIP; }
	void			SetIP( uint32 val ) //Only use this when you know the real IP or when your clearing it.
						{
							m_dwUserIP = val;
							m_nConnectIP = val;
							m_szFullUserIP= ipstr(m_nConnectIP); //Xman
						}
	inline bool	HasLowID() const								{ return (m_nUserIDHybrid < 16777216); }
	uint32			GetConnectIP() const							{ return m_nConnectIP; }
	uint16			GetUserPort() const								{ return m_nUserPort; }
	void			SetUserPort(uint16 val)							{ m_nUserPort = val; }
	uint32			GetTransferredUp() const							{ return m_nTransferredUp; }
	uint32			GetTransferredDown() const						{ return m_nTransferredDown; }
	uint32			GetServerIP() const								{ return m_dwServerIP; }
	void			SetServerIP(uint32 nIP)							{ m_dwServerIP = nIP; }
	uint16			GetServerPort() const							{ return m_nServerPort; }
	void			SetServerPort(uint16 nPort)						{ m_nServerPort = nPort; }
	const uchar*	GetUserHash() const								{ return (uchar*)m_achUserHash; }
	void			SetUserHash(const uchar* pUserHash);
	bool			HasValidHash() const
						{
							return ((const int*)m_achUserHash[0]) != 0 || ((const int*)m_achUserHash[1]) != 0 || ((const int*)m_achUserHash[2]) != 0 || ((const int*)m_achUserHash[3]) != 0;
						}
	int				GetHashType() const;
	const uchar*	GetBuddyID() const								{ return (uchar*)m_achBuddyID; }
	void			SetBuddyID(const uchar* m_achTempBuddyID);
	bool			HasValidBuddyID() const							{ return m_bBuddyIDValid; }
	void			SetBuddyIP( uint32 val )						{ m_nBuddyIP = val; }
	uint32			GetBuddyIP() const								{ return m_nBuddyIP; }
	void			SetBuddyPort( uint16 val )						{ m_nBuddyPort = val; }
	uint16			GetBuddyPort() const							{ return m_nBuddyPort; }
	EClientSoftware	GetClientSoft() const							{ return (EClientSoftware)m_clientSoft; }
	const CString&	GetClientSoftVer() const						{ return m_strClientSoftware; }
	const CString&	GetClientModVer() const							{ return m_strModVersion; }
	void			InitClientSoftwareVersion();
	uint32			GetVersion() const								{ return m_nClientVersion; }
	uint8			GetMuleVersion() const							{ return m_byEmuleVersion; }
	bool			ExtProtocolAvailable() const					{ return m_bEmuleProtocol; }
	bool			SupportMultiPacket() const						{ return m_bMultiPacket; }
	bool			SupportPeerCache() const						{ return m_fPeerCache; }
	bool			IsEmuleClient() const							{ return m_byEmuleVersion!=0; }
	uint8			GetSourceExchangeVersion() const				{ return m_bySourceExchangeVer; }
	CClientCredits* Credits() const									{ return credits; }
	bool			IsBanned() const;
	const CString&	GetClientFilename() const						{ return m_strClientFilename; }
	void			SetClientFilename(const CString& fileName)		{ m_strClientFilename = fileName; }
	uint16			GetUDPPort() const								{ return m_nUDPPort; }
	void			SetUDPPort(uint16 nPort)						{ m_nUDPPort = nPort; }
	uint8			GetUDPVersion() const							{ return m_byUDPVer; }
	bool			SupportsUDP() const								{ return GetUDPVersion() != 0 && m_nUDPPort != 0; }
	uint16			GetKadPort() const								{ return m_nKadPort; }
	void			SetKadPort(uint16 nPort)						{ m_nKadPort = nPort; }
	uint8			GetExtendedRequestsVersion() const				{ return m_byExtendedRequestsVer; }
	void			RequestSharedFileList();
	void			ProcessSharedFileList(const uchar* pachPacket, uint32 nSize, LPCTSTR pszDirectory = NULL);

	void			ClearHelloProperties();
	bool			ProcessHelloAnswer(const uchar* pachPacket, uint32 nSize);
	bool			ProcessHelloPacket(const uchar* pachPacket, uint32 nSize);
	void			SendHelloAnswer();
	virtual bool	SendHelloPacket();
	void			SendMuleInfoPacket(bool bAnswer);
	void			ProcessMuleInfoPacket(const uchar* pachPacket, uint32 nSize);
	void			ProcessMuleCommentPacket(const uchar* pachPacket, uint32 nSize);
	void			ProcessEmuleQueueRank(const uchar* packet, UINT size);
	void			ProcessEdonkeyQueueRank(const uchar* packet, UINT size);
	void			CheckQueueRankFlood();
	bool			Compare(const CUpDownClient* tocomp, bool bIgnoreUserhash = false) const;
	void			ResetFileStatusInfo();
	uint32			GetLastSrcReqTime() const						{ return m_dwLastSourceRequest; }
	void			SetLastSrcReqTime()								{ m_dwLastSourceRequest = ::GetTickCount(); }
	uint32			GetLastSrcAnswerTime() const					{ return m_dwLastSourceAnswer; }
	void			SetLastSrcAnswerTime()							{ m_dwLastSourceAnswer = ::GetTickCount(); }
	uint32			GetLastAskedForSources() const					{ return m_dwLastAskedForSources; }
	void			SetLastAskedForSources()						{ m_dwLastAskedForSources = ::GetTickCount(); }
	bool			GetFriendSlot() const;
	void			SetFriendSlot(bool bNV)							{ m_bFriendSlot = bNV; }
	bool			IsFriend() const								{ return m_Friend != NULL; }
	void			SetCommentDirty(bool bDirty = true)				{ m_bCommentDirty = bDirty; }
	bool			GetSentCancelTransfer() const					{ return m_fSentCancelTransfer; }
	void			SetSentCancelTransfer(bool bVal)				{ m_fSentCancelTransfer = bVal; }
	void			ProcessPublicIPAnswer(const BYTE* pbyData, UINT uSize);
	void			SendPublicIPRequest();
	uint8			GetKadVersion()									{ return m_byKadVersion; }
	bool			SendBuddyPingPong()								{ return m_dwLastBuddyPingPongTime < ::GetTickCount(); }
	bool			AllowIncomeingBuddyPingPong()					{ return m_dwLastBuddyPingPongTime < (::GetTickCount()-(3*60*1000)); }
	void			SetLastBuddyPingPongTime()						{ m_dwLastBuddyPingPongTime = (::GetTickCount()+(10*60*1000)); }
	// secure ident
	void			SendPublicKeyPacket();
	void			SendSignaturePacket();
	void			ProcessPublicKeyPacket(const uchar* pachPacket, uint32 nSize);
	void			ProcessSignaturePacket(const uchar* pachPacket, uint32 nSize);
	uint8			GetSecureIdentState() const						{ return m_SecureIdentState; }
	void			SendSecIdentStatePacket();
	void			ProcessSecIdentStatePacket(const uchar* pachPacket, uint32 nSize);
	uint8			GetInfoPacketsReceived() const					{ return m_byInfopacketsReceived; }
	void			InfoPacketsReceived();
	// preview
	void			SendPreviewRequest(const CAbstractFile* pForFile);
	void			SendPreviewAnswer(const CKnownFile* pForFile, CxImage** imgFrames, uint8 nCount);
	void			ProcessPreviewReq(const uchar* pachPacket, uint32 nSize);
	void			ProcessPreviewAnswer(const uchar* pachPacket, uint32 nSize);
	bool			GetPreviewSupport() const						{ return m_fSupportsPreview && GetViewSharedFilesSupport(); }
	bool			GetViewSharedFilesSupport() const				{ return m_fNoViewSharedFiles==0; }
	bool			SafeSendPacket(Packet* packet);
	void			CheckForGPLEvilDoer();
	//upload
	EUploadState	GetUploadState() const							{ return (EUploadState)m_nUploadState; }
	void			SetUploadState(EUploadState news);
	// ==> SUQWT [Moonlight/EastShare/ MorphXT] - Stulle
	/*
	uint32			GetWaitStartTime() const;
	*/
	sint64			GetWaitStartTime() const;
	// <== SUQWT [Moonlight/EastShare/ MorphXT] - Stulle
	void 			SetWaitStartTime();
	void 			ClearWaitStartTime();
	uint32			GetWaitTime() const								{ return m_dwUploadTime - GetWaitStartTime(); }
	bool			IsDownloading() const							{ return (m_nUploadState == US_UPLOADING); }
	bool			HasBlocks() const								{ return !m_BlockRequests_queue.IsEmpty(); }
	uint32			GetScore(bool sysvalue, bool isdownloading = false, bool onlybasevalue = false) const;
	void			AddReqBlock(Requested_Block_Struct* reqblock);
	void			CreateNextBlockPackage();
	uint32			GetUpStartTimeDelay() const						{ return ::GetTickCount() - m_dwUploadTime; }
	void 			SetUpStartTime()								{ m_dwUploadTime = ::GetTickCount(); }
	void			SendHashsetPacket(const uchar* fileid);
	const uchar*	GetUploadFileID() const							{ return requpfileid; }
	void			SetUploadFileID(CKnownFile* newreqfile);
	uint32			SendBlockData();
	void			ClearUploadBlockRequests();
	void			SendRankingInfo();
	void			SendCommentInfo(/*const*/ CKnownFile *file);
	void			AddRequestCount(const uchar* fileid);
	void			UnBan();
	void			Ban(LPCTSTR pszReason = NULL);
	uint32			GetAskedCount() const							{ return m_cAsked; }
	void			AddAskedCount()									{ m_cAsked++; }
	void			SetAskedCount(uint32 m_cInAsked)				{ m_cAsked = m_cInAsked; }
	void			FlushSendBlocks(); // call this when you stop upload, or the socket might be not able to send
	uint32			GetLastUpRequest() const						{ return m_dwLastUpRequest; }
	void			SetLastUpRequest()								{ m_dwLastUpRequest = ::GetTickCount(); }
	void			SetCollectionUploadSlot(bool bValue);
	bool			HasCollectionUploadSlot() const					{ return m_bCollectionUploadSlot; }

	uint32			GetSessionUp() const							{ return m_nTransferredUp - m_nCurSessionUp; }
	void			ResetSessionUp() {
						m_nCurSessionUp = m_nTransferredUp;
						m_addedPayloadQueueSession = 0;
						m_nCurQueueSessionPayloadUp = 0;
					}

	uint32			GetSessionDown() const							{ return m_nTransferredDown - m_nCurSessionDown; }
	void			ResetSessionDown() {
						m_nCurSessionDown = m_nTransferredDown;
					}
	uint32			GetQueueSessionPayloadUp() const				{ return m_nCurQueueSessionPayloadUp; }
    uint32          GetPayloadInBuffer() const						{ return m_addedPayloadQueueSession - GetQueueSessionPayloadUp(); }

	void			ProcessExtendedInfo(CSafeMemFile* packet, CKnownFile* tempreqfile, bool isUDP=false); //Xman better passive source finding
	uint16			GetUpPartCount() const							{ return m_nUpPartCount; }
	void			DrawUpStatusBar(CDC* dc, RECT* rect, bool onlygreyrect, bool  bFlat) const;
	bool			IsUpPartAvailable(uint16 iPart) const {
						return (iPart>=m_nUpPartCount || !m_abyUpPartStatus) ? false : m_abyUpPartStatus[iPart]!=0;
					}
	uint8*			GetUpPartStatus() const							{ return m_abyUpPartStatus; }
    float           GetCombinedFilePrioAndCredit();

	//download
	uint32			GetAskedCountDown() const						{ return m_cDownAsked; }
	void			AddAskedCountDown()								{ m_cDownAsked++; }
	void			SetAskedCountDown(uint32 m_cInDownAsked)		{ m_cDownAsked = m_cInDownAsked; }
	EDownloadState	GetDownloadState() const						{ return (EDownloadState)m_nDownloadState; }
	void			SetDownloadState(EDownloadState nNewState, LPCTSTR pszReason = _T("Unspecified"),DownStopReason reason = DSR_NONE); //Xman // Maella -Download Stop Reason-
	bool			IsPartAvailable(uint16 iPart) const {
						return (iPart>=m_nPartCount || !m_abyPartStatus) ? false : m_abyPartStatus[iPart]!=0;
					}
	uint8*			GetPartStatus() const							{ return m_abyPartStatus; }
	uint16			GetPartCount() const							{ return m_nPartCount; }
	uint16			GetRemoteQueueRank() const						{ return m_nRemoteQueueRank; }
	bool			IsRemoteQueueFull() const						{ return m_bRemoteQueueFull; }
	void			SetRemoteQueueFull(bool flag)				        	{ m_bRemoteQueueFull = flag; }
	void			DrawStatusBar(CDC* dc, LPCRECT rect, bool onlygreyrect, bool  bFlat) const;
	bool			AskForDownload();
	virtual void	SendFileRequest();
	void			SendStartupLoadReq();
	void			ProcessFileInfo(CSafeMemFile* data, CPartFile* file);
	void			ProcessFileStatus(bool bUdpPacket, CSafeMemFile* data, CPartFile* file);
	void			ProcessHashSet(const uchar* data, uint32 size);
	void			ProcessAcceptUpload();
	bool			AddRequestForAnotherFile(CPartFile* file);
	void			CreateBlockRequests(int iMaxBlocks);
	virtual void	SendBlockRequests();
	virtual bool	SendHttpBlockRequests();
	virtual void	ProcessBlockPacket(const uchar* packet, uint32 size, bool packed = false);
	virtual void	ProcessHttpBlockPacket(const BYTE* pucData, UINT uSize);
	void			ClearDownloadBlockRequests();
	void			SendOutOfPartReqsAndAddToWaitingQueue();
	UINT			GetAvailablePartCount() const;
	void			UDPReaskACK(uint16 nNewQR);
	void			UDPReaskFNF();
	void			UDPReaskForDownload();
	bool			IsSourceRequestAllowed() const;
    bool            IsSourceRequestAllowed(CPartFile* partfile, bool sourceExchangeCheck = false) const; // ZZ:DownloadManager

	bool			IsValidSource() const;
	ESourceFrom		GetSourceFrom() const							{ return (ESourceFrom)m_nSourceFrom; }
	void			SetSourceFrom(ESourceFrom val)					{ m_nSourceFrom = val; }

	void			SetDownStartTime()								{ m_dwDownStartTime = ::GetTickCount(); }
	uint32			GetDownTimeDifference(boolean clear = true)	{
						uint32 myTime = m_dwDownStartTime;
						if(clear) m_dwDownStartTime = 0;
						return ::GetTickCount() - myTime;
					}
	bool			GetTransferredDownMini() const					{ return m_bTransferredDownMini; }
	void			SetTransferredDownMini()						{ m_bTransferredDownMini = true; }
	void			InitTransferredDownMini()						{ m_bTransferredDownMini = false; }
	uint16			GetA4AFCount() const							{ return m_OtherRequests_list.GetCount(); }

	uint16			GetUpCompleteSourcesCount() const				{ return m_nUpCompleteSourcesCount; }
	void			SetUpCompleteSourcesCount(uint16 n)				{ m_nUpCompleteSourcesCount = n; }

	//chat
	EChatState		GetChatState() const							{ return (EChatState)m_nChatstate; }
	void			SetChatState(EChatState nNewS)					{ m_nChatstate = nNewS; }

	//KadIPCheck
	EKadState		GetKadState() const								{ return (EKadState)m_nKadState; }
	void			SetKadState(EKadState nNewS)					{ m_nKadState = nNewS; }

	//File Comment
	bool			HasFileComment() const							{ return !m_strFileComment.IsEmpty(); }
    const CString&	GetFileComment() const							{ return m_strFileComment; } 
    void			SetFileComment(LPCTSTR pszComment)				{ m_strFileComment = pszComment; }

	bool			HasFileRating() const							{ return m_uFileRating > 0; }
    uint8			GetFileRating() const							{ return m_uFileRating; }
    void			SetFileRating(uint8 uRating)					{ m_uFileRating = uRating; }

	// Barry - Process zip file as it arrives, don't need to wait until end of block
	int				unzip(Pending_Block_Struct *block, const BYTE *zipped, uint32 lenZipped, BYTE **unzipped, uint32 *lenUnzipped, int iRecursion = 0);
	void			UpdateDisplayedInfo(bool force = false);
	int             GetFileListRequested() const					{ return m_iFileListRequested; }
    void            SetFileListRequested(int iFileListRequested)	{ m_iFileListRequested = iFileListRequested; }

	// message filtering
	uint8			GetMessagesReceived() const						{ return m_cMessagesReceived; }
	void			SetMessagesReceived(uint8 nCount)				{ m_cMessagesReceived = nCount; }
	void			IncMessagesReceived()							{ m_cMessagesReceived++; }
	uint8			GetMessagesSent() const							{ return m_cMessagesSent; }
	void			SetMessagesSent(uint8 nCount)					{ m_cMessagesSent = nCount; }
	void			IncMessagesSent()								{ m_cMessagesSent++; }
	bool			IsSpammer() const								{ return m_fIsSpammer; }
	void			SetSpammer(bool bVal)							{ m_fIsSpammer = bVal ? 1 : 0; }
	bool			GetMessageFiltered() const						{ return m_fMessageFiltered; }
	void			SetMessageFiltered(bool bVal)					{ m_fMessageFiltered = bVal ? 1 : 0; }

	virtual void	SetRequestFile(CPartFile* pReqFile);
	CPartFile*		GetRequestFile() const							{ return reqfile; }

	// AICH Stuff
	void			SetReqFileAICHHash(CAICHHash* val);
	CAICHHash*		GetReqFileAICHHash() const						{ return m_pReqFileAICHHash; }
	bool			IsSupportingAICH() const						{ return m_fSupportsAICH & 0x01; }
	void			SendAICHRequest(CPartFile* pForFile, uint16 nPart);
	bool			IsAICHReqPending() const						{ return m_fAICHRequested; }
	void			ProcessAICHAnswer(const uchar* packet, UINT size);
	void			ProcessAICHRequest(const uchar* packet, UINT size);
	void			ProcessAICHFileHash(CSafeMemFile* data, CPartFile* file);

	EUtf8Str		GetUnicodeSupport() const;

	CString			GetDownloadStateDisplayString() const;
	CString			GetUploadStateDisplayString() const;

	LPCTSTR			DbgGetDownloadState() const;
	LPCTSTR			DbgGetUploadState() const;
	LPCTSTR			DbgGetKadState() const;
	CString			DbgGetClientInfo(bool bFormatIP = false) const;
	CString			DbgGetFullClientSoftVer() const;
	const CString&	DbgGetHelloInfo() const							{ return m_strHelloInfo; }
	const CString&	DbgGetMuleInfo() const							{ return m_strMuleInfo; }

	const bool      IsInNoNeededList(const CPartFile* fileToCheck) const; //Xman used for downloadlistctrl


#ifdef _DEBUG
	// Diagnostic Support
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	CClientReqSocket* socket;
	CClientCredits*	credits;
	CFriend*		m_Friend;
	uint8*			m_abyUpPartStatus;
	CTypedPtrList<CPtrList, CPartFile*> m_OtherRequests_list;
	CTypedPtrList<CPtrList, CPartFile*> m_OtherNoNeeded_list;
	uint16			m_lastPartAsked;
	bool			m_bAddNextConnect;

    //Xman not used
	//void			SetSlotNumber(uint32 newValue)					{ m_slotNumber = newValue; }
    //uint32			GetSlotNumber() const							{ return m_slotNumber; }
    CClientReqSocket*		GetFileUploadSocket(bool log = false) const ; //Xman Xtreme Upload: Peercache-part


// ==> {Webcache} [Max] 

///////////////////////////////////////////////////////////////////////////
// WebCache client
private:
	bool m_bIsTrustedOHCBSender;
	bool m_bIsAllowedToSendOHCBs;
	uint32 m_uWebCacheFlags;
	EWebCacheDownState m_eWebCacheDownState;
	EWebCacheUpState m_eWebCacheUpState;
	bool b_webcacheInfoNeeded;
protected:
	bool m_bProxy;
public:
	bool m_bIsAcceptingOurOhcbs; // default - true, set to false on OP_DONT_SEND_OHCBS
	CWebCacheDownSocket* m_pWCDownSocket;
	CWebCacheUpSocket* m_pWCUpSocket;

	bool SupportsWebCacheUDP() const {return (m_uWebCacheFlags & WC_FLAGS_UDP) && SupportsUDP();}
	bool SupportsOhcbSuppression() const {return (m_uWebCacheFlags & WC_FLAGS_NO_OHCBS)!=0;}
	bool SupportsWebCacheProtocol() const {return SupportsOhcbSuppression();} // this is the first version that supports that
	bool IsProxy() const {return m_bProxy;}
	bool IsUploadingToWebCache() const;
	bool IsDownloadingFromWebCache() const;
	bool ProcessWebCacheDownHttpResponse(const CStringAArray& astrHeaders);
	bool ProcessWebCacheDownHttpResponseBody(const BYTE* pucData, UINT uSize);
	bool ProcessWebCacheUpHttpResponse(const CStringAArray& astrHeaders);
	UINT ProcessWebCacheUpHttpRequest(const CStringAArray& astrHeaders);
	void OnWebCacheDownSocketClosed(int nErrorCode);
	void OnWebCacheDownSocketTimeout();
	void SetWebCacheDownState(EWebCacheDownState eState);
	EWebCacheDownState CUpDownClient::GetWebCacheDownState() const {return m_eWebCacheDownState;}
	void SetWebCacheUpState(EWebCacheUpState eState);
	EWebCacheUpState CUpDownClient::GetWebCacheUpState() const {return m_eWebCacheUpState;}
	virtual bool SendWebCacheBlockRequests();
	void PublishWebCachedBlock( const Requested_Block_Struct* block );
	bool IsWebCacheUpSocketConnected() const;
	bool IsWebCacheDownSocketConnected() const;
//	uint16 blocksLoaded;	// Superlexx - block transfer limiter //JP blocks are counted in the socket code now
	uint16 GetNumberOfClientsBehindOurWebCacheAskingForSameFile();	// what a name ;)
	uint16 GetNumberOfClientsBehindOurWebCacheHavingSameFileAndNeedingThisBlock(Pending_Block_Struct* pending); // Superlexx - COtN - it's getting better all the time...
	bool WebCacheInfoNeeded() {return b_webcacheInfoNeeded;}
	void SetWebCacheInfoNeeded(bool value) {b_webcacheInfoNeeded = value;}
//JP trusted-OHCB-senders START
	uint32 WebCachedBlockRequests;
	uint32 SuccessfulWebCachedBlockDownloads;
	bool IsTrustedOHCBSender() const {return m_bIsTrustedOHCBSender;}
	void AddWebCachedBlockToStats( bool IsGood );
//JP trusted-OHCB-senders END
//JP stop sendig OHCBs START
	void SendStopOHCBSending();
	void SendResumeOHCBSendingTCP();
	void SendResumeOHCBSendingUDP();
//JP stop sendig OHCBs END
	CWebCacheCryptography Crypt; // Superlexx - encryption
	uint32 lastMultiOHCBPacketSent;
	void SendOHCBsNow();
	bool a(uint32 a) {return a <= WC_MAX_OHCBS_IN_UDP_PACKET;}
	bool b(uint32 a, uint32 b) { return (a * 100.0 / (b + 1) < 0.2)!=0; }
	bool c(uint32 a, uint32 b) { return a > 4 && (b * 100) / a < 20; }

// <== {Webcache} [Max] 

	///////////////////////////////////////////////////////////////////////////
	// PeerCache client
	//
	bool IsDownloadingFromPeerCache() const;
	bool IsUploadingToPeerCache() const;
	bool HasPeerCacheState() {return m_ePeerCacheUpState == PCUS_WAIT_CACHE_REPLY || m_ePeerCacheUpState == PCUS_UPLOADING;} //Xman Xtreme Upload: Peercache-part
	void SetPeerCacheDownState(EPeerCacheDownState eState);
	void SetPeerCacheUpState(EPeerCacheUpState eState);

	int  GetHttpSendState() const									{ return m_iHttpSendState; }
	void SetHttpSendState(int iState)								{ m_iHttpSendState = iState; }

	bool SendPeerCacheFileRequest();
	bool ProcessPeerCacheQuery(const uchar* packet, UINT size);
	bool ProcessPeerCacheAnswer(const uchar* packet, UINT size);
	bool ProcessPeerCacheAcknowledge(const uchar* packet, UINT size);
	void OnPeerCacheDownSocketClosed(int nErrorCode);
	bool OnPeerCacheDownSocketTimeout();
	
	bool ProcessPeerCacheDownHttpResponse(const CStringAArray& astrHeaders);
	bool ProcessPeerCacheDownHttpResponseBody(const BYTE* pucData, UINT uSize);
	bool ProcessPeerCacheUpHttpResponse(const CStringAArray& astrHeaders);
	UINT ProcessPeerCacheUpHttpRequest(const CStringAArray& astrHeaders);

	virtual bool ProcessHttpDownResponse(const CStringAArray& astrHeaders);
	virtual bool ProcessHttpDownResponseBody(const BYTE* pucData, UINT uSize);

	CPeerCacheDownSocket* m_pPCDownSocket;
	CPeerCacheUpSocket* m_pPCUpSocket;


// ==> {Webcache} [Max] 

	bool HasWebCacheState() {return m_eWebCacheUpState == WCUS_UPLOADING;} 
   

	// Squid defaults..
	bool			UsesCachedTCPPort() { return ( (GetUserPort()==80)
													|| (GetUserPort()==21)
													|| (GetUserPort()==443)
													|| (GetUserPort()==563)
													|| (GetUserPort()==70)
													|| (GetUserPort()==210)
													|| ((GetUserPort()>=1025) && (GetUserPort()<=65535)));}
	bool			SupportsWebCache() const { return m_bWebCacheSupport; }
	bool			SupportsMultiOHCBs() const {return m_bWebCacheSupportsMultiOHCBs;}
	bool			IsBehindOurWebCache() const
					{
						// WC-TODO: make this more efficient
						return( thePrefs.webcacheName == GetWebCacheName() );
					}
// jp webcache
	CString			GetWebCacheName() const 
					{ 
						if (SupportsWebCache())
							return WebCacheIndex2Name(m_WA_webCacheIndex);
						else
							return _T("");
					}
	bool			m_bWebCacheSupport;
	bool            m_bWebCacheSupportsMultiOHCBs;
	int		m_WA_webCacheIndex;	// index of the webcache name
	uint16	m_WA_HTTPPort;		// remote webserver port
	uint32	m_uWebCacheDownloadId;	// we must attach this ID when sending HTTP download request to the remote client.
	uint32	m_uWebCacheUploadId;	// incoming HTTP requests are identified as WC-requests,
									// if the header contains this ID and there is a known client with same ID in downloading state.
									// used for client authorization, should be substituted by a HttpIdList? later
									// for efficiency reasons.
		// Superlexx - MFR
	CWebCacheMFRList	requestedFiles; // the files this client requested from us
	Packet*	CreateMFRPacket();		// builds a separate MFR-packet
	uint8	AttachMultiOHCBsRequest(CSafeMemFile &data); // Superlexx - attaches a multiple files request
	uint8	IsPartAvailable(uint16 iPart, const byte* fileHash) {return requestedFiles.IsPartAvailable(iPart, fileHash);}
 
// <== {Webcache} [Max] 

//-----------------------------------------------------------------
//Xman:
public:
	
	//Xman Full Chunk
	bool upendsoon;
	bool 			IsDifferentPartBlock();
	//Xman end
	
	// Maella -Accurate measure of bandwidth: eDonkey data + control, network adapter-
    uint32			GetUploadDatarate(uint32 samples) const;
    uint32			GetUploadDatarate() const {return m_nUpDatarate;}
	uint32			GetDownloadDatarate() const {return m_nDownDatarate;}

	void			AddUploadRate(const uint32 size);

	void			AddDownloadRate(const uint32 size) { m_nDownDatarateMeasure += size; } 

	void			CompUploadRate(); // Actualize datarate

	void			CompDownloadRate(); // Actualize datarate
	
	// Maella end

	const CString			GetUserIPString() const { return m_szFullUserIP; }

	//Xman filter clients with failed downloads
	uint16 m_faileddownloads;

	//Xman askfordownload priority
	sint8 m_downloadpriority;

	//Xman Xtreme Upload
	//CompUploadRate and looks if a datarate is too high
	//this forces the uploadbandwidththrottler to set a new Full Slot
	bool			CheckDatarate(uint8 dataratestocheck);

	//Xman uploading problem client
	bool isupprob;

	CString			GetFullIP()					{return m_szFullUserIP;}

	//Xman see OnUploadqueue
	void			SetOldUploadFileID();
	uchar*			GetOldUploadFileID()		{return oldfileid;} 
	//Xman fix for startupload
	void			SetLastAction(UINT action)	{lastaction=action;}
	UINT			GetLastAction()				{return lastaction;}
	//Xman end
	
	//Xman fix for startupload (downloading side)
	bool protocolstepflag1;

	//Xman Xtreme Downloadmanager
	void			TrigNextSafeAskForDownload(CPartFile* pFile); // Maella -Unnecessary Protocol Overload-
	bool			SwapToAnotherFile(bool bIgnoreNoNeeded, bool ignoreSuspensions, bool bRemoveCompletely, CPartFile* toFile = NULL, bool lookatmintime=false, bool allow_go_over_hardlimit=false);
	void			DontSwapTo(CPartFile* file);
	bool			IsSwapSuspended(CPartFile* file);
	bool			DoSwap(CPartFile* SwapTo, bool bRemoveCompletely=false);
	void			SetRemoteQueueRank(uint16 nr, bool updatedisplay=true);	//remark: updatedisplay is used on different way than official

	//Xman Anti-Leecher
	void			TestLeecher();
	void			BanLeecher(LPCTSTR pszReason , uint8 leechercategory); 
	uint8			IsLeecher()	const		{return m_bLeecher;}
	void			ProcessUnknownHelloTag(CTag *tag, CString &pszReason);
	void			ProcessUnknownInfoTag(CTag *tag, CString &pszReason);
	void			ProcessBanMessage();

	//>>> Anti-XS-Exploit (Xman)
	void IncXSAnswer()  {m_uiXSAnswer++;}
	void IncXSReqs()  {m_uiXSReqs++;}
	uint32 GetXSReqs() const { return m_uiXSReqs;}
	uint32 GetXSAnswers() const {return m_uiXSAnswer;}
	bool IsXSExploiter() const { return m_uiXSReqs>2 && ((float)(m_uiXSAnswer+1))/m_uiXSReqs<0.5f;}
	//<<< Anti-XS-Exploit

	//Xman end

	void			ShowRequestedFiles(); // - show requested files (sivka)

	// Maella -Unnecessary Protocol Overload-
	uint32 GetLastAskedTime() const {return m_dwLastAskedTime;}
	uint32 GetNextTCPAskedTime() const {return m_dwNextTCPAskedTime;}
	void   SetNextTCPAskedTime(uint32 time) {m_dwNextTCPAskedTime = time;}
	uint32 GetLastFileAskedTime(CPartFile* pFile) {return m_partStatusMap[pFile].dwStartUploadReqTime;}
	uint32 GetJitteredFileReaskTime() const {return m_jitteredFileReaskTime;} // range 25.5..29.5 min 
	//Xman own method
	bool   HasTooManyFailedUDP() const {return m_nTotalUDPPackets > 3 && ((float)(m_nFailedUDPPackets/m_nTotalUDPPackets) > .3);} 
	//Maella end

	//Xman Xtreme Downloadmanager
	uint32 droptime;
	uint32 enterqueuetime;	//indicates, when a source was first asked
	sint32 GetDiffQR() const {return oldQR==0 ? 0 : m_nRemoteQueueRank - oldQR;} //Xman diffQR


	//check completed sources which want to download their "complete" file
	bool HasFileComplete() const {return m_bCompleteSource;}

	// Maella -Extended clean-up II-
	void CleanUp(CPartFile* pDeletedFile);
	DWORD m_lastCleanUpCheck;
	// Maella end

// Maella -Unnecessary Protocol Overload-
protected:
	struct PartStatus{
		PartStatus() : dwStartUploadReqTime(0) {}
		uint32 dwStartUploadReqTime; // Used to avoid Ban()
	};
	typedef std::map<CPartFile*, PartStatus> PartStatusMap;
	PartStatusMap m_partStatusMap;



private:
	// Maella -Accurate measure of bandwidth: eDonkey data + control, network adapter-
	uint8	m_displayUpDatarateCounter; // refresh display timer
	uint8	m_displayDownDatarateCounter; // refresh display timer

	uint32	m_nUpDatarate; // current datarate (updated every seconds)
	uint32	m_nUpDatarateMeasure; // transfered byte since the last measure
	CList<TransferredData> m_upHistory_list; 

	uint32	m_nDownDatarate; // current datarate (updated every seconds)
	uint32	m_nDownDatarate10; //Xman 0.46b: hold the avg over 10 secondes, needed for the faster endgame
	uint32	m_nDownDatarateMeasure; // transfered byte since the last measure
	//uint32	m_sumDownHistory; // max 4GB
	CList<TransferredData> m_downHistory_list;
	// Maella end

	CString m_szFullUserIP; //Xman hold the IP-String

	// Maella -Unnecessary Protocol Overload-
	uint32 m_dwLastAskedTime;     // Last attempt to refresh the download session with TCP or UDP
	uint32 m_dwLastUDPReaskTime;  // Last attempt to refresh the download session with UDP
	uint32 m_dwNextTCPAskedTime;  // Time of the next refresh for the download session with TCP
	uint32 m_jitteredFileReaskTime;

	//Xman Xtreme Downloadmanager
	bool isduringswap;	//indicates, that we are during a swap operation
	uint16 oldQR; //Xman diffQR


	// Maella -Upload Stop Reason-
public:
	static void   AddUpStopCount(bool failed, UpStopReason reason) {++m_upStopReason[failed?0:1][reason];}
	static uint32 GetUpStopCount(bool failed, UpStopReason reason) {return m_upStopReason[failed?0:1][reason];}
	//Xman uploading problem client
	static void		SubUploadSocketStopCount(){--m_upStopReason[0][USR_SOCKET];}

	uint8	m_cFailed; //Xman Downloadmanager / Xtreme Mod // holds the failed connection attempts

	// Maella -Download Stop Reason-

	static uint32 GetDownStopCount(bool failed, DownStopReason reason) {return m_downStopReason[failed?0:1][reason];}

private:
	static void   AddDownStopCount(bool failed, DownStopReason reason) {++m_downStopReason[failed?0:1][reason];}

	static uint32 m_upStopReason[2][USR_EXCEPTION+1];
	static uint32 m_downStopReason[2][DSR_EXCEPTION+1];
	// Maella end
	
	//Xman Anti-Leecher
	uint8	m_bLeecher; 
	CString	old_m_strClientSoftwareFULL;
	CString	old_m_pszUsername;
	CString m_strBanMessage;

	//>>> Anti-XS-Exploit (Xman)
	uint32 m_uiXSAnswer;
	uint32 m_uiXSReqs;
	//<<< Anti-XS-Exploit

	//Xman end

	//Xman Anti-Leecher: simple Anti-Thief
	float	GetXtremeVersion(CString modversion) const;
	static const CString str_ANTAddOn;
	static const CString GetANTAddOn()
	{
		CString nick=_T("[");
		srand( (unsigned)time( NULL ) );
		for(uint8 i = 0; i < 4; ++i)
		{
				nick.AppendFormat(_T("%c"), 32+rand()%95);
		}
		nick += _T(']');

		return nick;
	}

	//Xman end

//Xman end
//--------------------------------------------------------------------------------------
protected:
	int		m_iHttpSendState;
	uint32	m_uPeerCacheDownloadPushId;
	uint32	m_uPeerCacheUploadPushId;
	uint32	m_uPeerCacheRemoteIP;
	bool	m_bPeerCacheDownHit;
	bool	m_bPeerCacheUpHit;
	EPeerCacheDownState m_ePeerCacheDownState;
	EPeerCacheUpState m_ePeerCacheUpState;

protected:
	// base
	void	Init();
	bool	ProcessHelloTypePacket(CSafeMemFile* data);
	void	SendHelloTypePacket(CSafeMemFile* data);
	void	CreateStandartPackets(byte* data,uint32 togo, Requested_Block_Struct* currentblock, bool bFromPF = true);
	void	CreatePackedPackets(byte* data,uint32 togo, Requested_Block_Struct* currentblock, bool bFromPF = true);

	uint32	m_nConnectIP;		// holds the supposed IP or (after we had a connection) the real IP
	uint32	m_dwUserIP;			// holds 0 (real IP not yet available) or the real IP (after we had a connection)
	uint32	m_dwServerIP;
	uint32	m_nUserIDHybrid;
	uint16	m_nUserPort;
	uint16	m_nServerPort;
	uint32	m_nClientVersion;
	//--group to aligned int32
	uint8	m_byEmuleVersion;
	uint8	m_byDataCompVer;
	bool	m_bEmuleProtocol;
	bool	m_bIsHybrid;
	//--group to aligned int32
	TCHAR*	m_pszUsername;
	uchar	m_achUserHash[16];
	uint16	m_nUDPPort;
	uint16	m_nKadPort;
	//--group to aligned int32
	uint8	m_byUDPVer;
	uint8	m_bySourceExchangeVer;
	uint8	m_byAcceptCommentVer;
	uint8	m_byExtendedRequestsVer;
	//--group to aligned int32
	uint8	m_byCompatibleClient;
	bool	m_bFriendSlot;
	bool	m_bCommentDirty;
	bool	m_bIsML;
	//--group to aligned int32
	bool	m_bGPLEvildoer;
	bool	m_bHelloAnswerPending;
	uint8	m_byInfopacketsReceived;	// have we received the edonkeyprot and emuleprot packet already (see InfoPacketsReceived() )
	uint8	m_bySupportSecIdent;
	//--group to aligned int32
	uint32	m_dwLastSignatureIP;
	CString m_strClientSoftware;
	CString m_strModVersion;
	uint32	m_dwLastSourceRequest;
	uint32	m_dwLastSourceAnswer;
	uint32	m_dwLastAskedForSources;
    int     m_iFileListRequested;
	CString	m_strFileComment;
	//--group to aligned int32
	uint8	m_uFileRating;
	uint8	m_cMessagesReceived;		// count of chatmessages he sent to me
	uint8	m_cMessagesSent;			// count of chatmessages I sent to him
	bool	m_bMultiPacket;
	//--group to aligned int32
	bool	m_bUnicodeSupport;
	bool	m_bBuddyIDValid;
	uint16	m_nBuddyPort;
	//--group to aligned int32
	uint32	m_nBuddyIP;
	uint32	m_dwLastBuddyPingPongTime;
	uchar	m_achBuddyID[16];
	CString m_strHelloInfo;
	CString m_strMuleInfo;
	uint8	m_byKadVersion;

	// States
#ifdef _DEBUG
	// use the 'Enums' only for debug builds, each enum costs 4 bytes (3 unused)
	EClientSoftware		m_clientSoft;
	EChatState			m_nChatstate;
	EKadState			m_nKadState;
	ESecureIdentState	m_SecureIdentState;
	EUploadState		m_nUploadState;
	EDownloadState		m_nDownloadState;
	ESourceFrom			m_nSourceFrom;
	EModClient          m_uModClient; // Mod Icons - Stulle
#else
	uint8 m_clientSoft;
	uint8 m_nChatstate;
	uint8 m_nKadState;
	uint8 m_SecureIdentState;
	uint8 m_nUploadState;
	uint8 m_nDownloadState;
	uint8 m_nSourceFrom;
	uint8 m_uModClient; // Mod Icons - Stulle
#endif

	CTypedPtrList<CPtrList, Packet*> m_WaitingPackets_list;
	CList<PartFileStamp> m_DontSwap_list;

	////////////////////////////////////////////////////////////////////////
	// Upload
	//
	int GetFilePrioAsNumber() const;

	uint32		m_nTransferredUp;
	uint32		m_dwUploadTime;
	uint32		m_cAsked;
	uint32		m_dwLastUpRequest;
	uint32		m_nCurSessionUp;
	uint32		m_nCurSessionDown;
    uint32      m_nCurQueueSessionPayloadUp;
    uint32      m_addedPayloadQueueSession;
	uint16		m_nUpPartCount;
	uint16		m_nUpCompleteSourcesCount;
	uchar		requpfileid[16];
	bool		m_bCollectionUploadSlot;
	//uint32      m_slotNumber; //Xman not used
	uchar		oldfileid[16]; //Xman see OnUploadqueue
	UINT		lastaction; //Xman fix for startupload

	CTypedPtrList<CPtrList, Requested_Block_Struct*> m_BlockRequests_queue;
	CTypedPtrList<CPtrList, Requested_Block_Struct*> m_DoneBlocks_list;
	CTypedPtrList<CPtrList, Requested_File_Struct*>	 m_RequestedFiles_list;

	//////////////////////////////////////////////////////////
	// Download
	//
public: // file settings - Stulle
	CPartFile*	reqfile;
protected: // file settings - Stulle
	CAICHHash*  m_pReqFileAICHHash; 
	uint32		m_cDownAsked;
	uint8*		m_abyPartStatus;
	CString		m_strClientFilename;
	uint32		m_nTransferredDown;
	uint32		m_dwDownStartTime;
	uint32      m_nLastBlockOffset;
	uint32		m_dwLastBlockReceived;
	uint32		m_nTotalUDPPackets;
	uint32		m_nFailedUDPPackets;
	//--group to aligned int32
	uint16		m_nRemoteQueueRank;
	//--group to aligned int32
	uint16		m_nPartCount;
	bool		m_bRemoteQueueFull;
	bool		m_bCompleteSource;
	//--group to aligned int32
	bool		m_bReaskPending;
	bool		m_bUDPPending;
	bool		m_bTransferredDownMini;

	// Download from URL
	CStringA	m_strUrlPath;
	UINT		m_uReqStart;
	UINT		m_uReqEnd;
	UINT		m_nUrlStartPos;



	//////////////////////////////////////////////////////////
	// GUI helpers
	//
	DWORD		m_lastRefreshedDLDisplay;
    DWORD		m_lastRefreshedULDisplay;
    uint32      m_random_update_wait;

	// using bitfield for less important flags, to save some bytes
	UINT m_fHashsetRequesting : 1, // we have sent a hashset request to this client in the current connection
		 m_fSharedDirectories : 1, // client supports OP_ASKSHAREDIRS opcodes
		 m_fSentCancelTransfer: 1, // we have sent an OP_CANCELTRANSFER in the current connection
		 m_fNoViewSharedFiles : 1, // client has disabled the 'View Shared Files' feature, if this flag is not set, we just know that we don't know for sure if it is enabled
		 m_fSupportsPreview   : 1,
		 m_fPreviewReqPending : 1,
		 m_fPreviewAnsPending : 1,
		 m_fIsSpammer		  : 1,
		 m_fMessageFiltered   : 1,
		 m_fPeerCache		  : 1,
		 m_fQueueRankPending  : 1,
		 m_fUnaskQueueRankRecv: 2,
		 m_fFailedFileIdReqs  : 4, // nr. of failed file-id related requests per connection
		 m_fNeedOurPublicIP	  : 1, // we requested our IP from this client
		 m_fSupportsAICH	  : 3,
		 m_fAICHRequested     : 1,
		 m_fSentOutOfPartReqs : 1;

	CTypedPtrList<CPtrList, Pending_Block_Struct*>	 m_PendingBlocks_list;
	CTypedPtrList<CPtrList, Requested_Block_Struct*> m_DownloadBlocks_list;


//EastShare Start - added by AndCycle, IP to Country
public:
	CString			GetCountryName(bool longName = true) const; //Xman changed 
	int				GetCountryFlagIndex() const;
	void			ResetIP2Country(uint32 dwIP = 0);

private:
	struct	IPRange_Struct2* m_structUserCountry; //EastShare - added by AndCycle, IP to Country
//EastShare End - added by AndCycle, IP to Country

public:
	bool	GetSmallFilePush() const; // push small files [sivka] - Stulle

	float	GetRareFilePushRatio() const ; // push rare file - Stulle

	// ==> Mod Icons - Stulle
	EModClient	GetModClient() const	{ return (EModClient)m_uModClient; }
	// <== Mod Icons - Stulle
};
//#pragma pack()
