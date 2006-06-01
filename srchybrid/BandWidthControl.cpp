//this file is part of eMule
//Copyright (C)2002 Merkur ( merkur-@users.sourceforge.net / http://www.emule-project.net )
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

#include "StdAfx.h"
#include "BandWidthControl.h"
#include "Emule.h"
#include "Log.h"
#include "Preferences.h"
#include "opcodes.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CBandWidthControl::CBandWidthControl()
:   m_statisticHistory(1024) // size  ~= 1024*(4*8+3*4) = 1024*44 bytes = 44 KBytes
{
   m_statistic.eMuleOutOctets = 0;
   m_statistic.eMuleInOctets = 0;
   m_statistic.eMuleOutOverallOctets = 0;
   m_statistic.eMuleInOverallOctets = 0;
   m_statistic.networkOutOctets = 0;
   m_statistic.networkInOctets = 0;
   m_statistic.timeStamp = ::GetTickCount();
	m_maxDownloadLimit = 0.0f;
	m_maxUploadLimit = 0.0f;
	m_errorTraced = false;

	//Xman GlobalMaxHarlimit for fairness
	m_maxforcedDownloadlimit=0;

	// Keep last result to detect an overflow
	m_networkOutOctets = 0;
	m_networkInOctets = 0;

	//Xman show complete internettraffic
	initialnetworkOutOctets=0;
	initialnetworkInOctets=0;
	//Xman end

   // Cache index value
   m_currentAdapterIndex = 0;

   // Dynamic load library iphlpapi.dll => user of win95
   m_hIphlpapi = ::LoadLibrary(_T("iphlpapi.dll"));
   if(m_hIphlpapi != NULL){
      m_fGetIfTable = (GETIFTABLE)GetProcAddress(m_hIphlpapi, "GetIfTable");
		m_fGetIpAddrTable = (GETIPADDRTABLE)GetProcAddress(m_hIphlpapi, "GetIpAddrTable");
      m_fGetIfEntry = (GETIFENTRY)GetProcAddress(m_hIphlpapi, "GetIfEntry");
      m_fGetNumberOfInterfaces = (GETNUMBEROFINTERFACES)GetProcAddress(m_hIphlpapi, "GetNumberOfInterfaces");

		theApp.QueueDebugLogLine(false, _T("NAFC: Succeed to load library iphlpapi.dll"));
   }
   else {
      m_fGetIfTable = NULL;
      m_fGetIfEntry = NULL;
      m_fGetNumberOfInterfaces = NULL;

		theApp.QueueDebugLogLine(false, _T("NAFC: Fail to load library iphlpapi.dll"));
   }
   //Xman new adapter selection
   wasNAFCLastActive=thePrefs.GetNAFCFullControl();
}

CBandWidthControl::~CBandWidthControl(){
   // Unload library
   if(m_hIphlpapi != NULL){
      ::FreeLibrary(m_hIphlpapi);
   }
}
//Xman new adapter selection
void CBandWidthControl::checkAdapterIndex(uint32 highid)
{
	// Check if the library was successfully loaded
	if(m_fGetNumberOfInterfaces != NULL && m_fGetIfTable != NULL && m_fGetIpAddrTable != NULL){
		DWORD dwNumIf = 0;
		if(m_fGetNumberOfInterfaces(&dwNumIf) == NO_ERROR && dwNumIf > 0 ){
			BYTE buffer[10*sizeof(MIB_IFROW)];
			ULONG size = sizeof(buffer);
			MIB_IFTABLE& mibIfTable = reinterpret_cast<MIB_IFTABLE&>(buffer[0]);
			if(m_fGetIfTable(&mibIfTable, &size, true) == NO_ERROR){
				// Trace list of Adapters
				if(m_errorTraced == false){
					for(DWORD dwNumEntries = 0; dwNumEntries < mibIfTable.dwNumEntries; dwNumEntries++){
						const MIB_IFROW& mibIfRow = mibIfTable.table[dwNumEntries];
						theApp.QueueDebugLogLine(false, _T("NAFC: Adapter %u is '%s'"), mibIfRow.dwIndex, (CString)mibIfRow.bDescr);
					}
				}

				//Xman forceNAFCadapter-option
				if(thePrefs.GetForcedNAFCAdapter()!=0)
				{
					theApp.QueueDebugLogLine(false, _T("NAFC: you forced to use NAFC-Adapter with index: %u"), thePrefs.GetForcedNAFCAdapter());
					return;
				}
				//Xman end

				// Retrieve the default used IP (=> in case of multiple adapters)
				char hostName[256];
				if(gethostname(hostName, sizeof(hostName)) == 0){
					hostent* lphost = gethostbyname(hostName);
					if(lphost != NULL){
						DWORD dwAddr = ((LPIN_ADDR)lphost->h_addr)->s_addr;
						// Pick the interface matching the IP
						BYTE buffer[10*sizeof(MIB_IPADDRROW)];
						ULONG size = sizeof(buffer);
						MIB_IPADDRTABLE& mibIPAddrTable = reinterpret_cast<MIB_IPADDRTABLE&>(buffer[0]);
						if(m_fGetIpAddrTable(&mibIPAddrTable, &size, FALSE) == 0){
							//Xman: first we seek the highid from the server
							for(DWORD i = 0; i < mibIPAddrTable.dwNumEntries; i++){
								if(mibIPAddrTable.table[i].dwAddr == highid){
									//const MIB_IPADDRROW& row = mibIPAddrTable.table[i];
									m_errorTraced = false;
									theApp.QueueDebugLogLine(false, _T("NAFC found by IP: Select adapter with index %u"), mibIPAddrTable.table[i].dwIndex);
									m_currentAdapterIndex= mibIPAddrTable.table[i].dwIndex;
									//reactivate NAFC	
									if(wasNAFCLastActive)
										thePrefs.SetNAFCFullControl(true);
									return;
								}
							}
							//Xman: if highid not found, search the hostip
							for(DWORD i = 0; i < mibIPAddrTable.dwNumEntries; i++){
								if(mibIPAddrTable.table[i].dwAddr == dwAddr){
									//const MIB_IPADDRROW& row = mibIPAddrTable.table[i];
									m_errorTraced = false;
									theApp.QueueDebugLogLine(false, _T("NAFC: Select adapter with index %u"), mibIPAddrTable.table[i].dwIndex);
									m_currentAdapterIndex= mibIPAddrTable.table[i].dwIndex;
									//reactivate NAFC	
									if(wasNAFCLastActive)
										thePrefs.SetNAFCFullControl(true);
									return;
								}
							}
						}
						else {
							if(m_errorTraced == false){
								m_errorTraced = true;
								theApp.QueueDebugLogLine(false, _T("NAFC: Failed to get IP tables error=0x%x"), ::GetLastError());
								return ;
							}
						}
					}
				}
			}
			if(m_errorTraced == false){
				m_errorTraced = true;
				theApp.QueueDebugLogLine(false, _T("NAFC: Failed to get tables of interface error=0x%x"), ::GetLastError());
				return ;
			}
		}
		if(m_errorTraced == false){
			m_errorTraced = true;
			theApp.QueueDebugLogLine(false, _T("NAFC: Failed to get the number of interface error=0x%x"), ::GetLastError());
			return ;
		}
	}
	return ;
}
//Xman new adapter selection end

DWORD CBandWidthControl::getAdapterIndex(){
   // Check if the library was successfully loaded
   if(m_fGetNumberOfInterfaces != NULL && m_fGetIfTable != NULL && m_fGetIpAddrTable != NULL){
      DWORD dwNumIf = 0;
      if(m_fGetNumberOfInterfaces(&dwNumIf) == NO_ERROR && dwNumIf > 0 ){
         BYTE buffer[10*sizeof(MIB_IFROW)];
         ULONG size = sizeof(buffer);
         MIB_IFTABLE& mibIfTable = reinterpret_cast<MIB_IFTABLE&>(buffer[0]);
         if(m_fGetIfTable(&mibIfTable, &size, true) == NO_ERROR){
                // Trace list of Adapters
                if(m_errorTraced == false){
                for(DWORD dwNumEntries = 0; dwNumEntries < mibIfTable.dwNumEntries; dwNumEntries++){
                        const MIB_IFROW& mibIfRow = mibIfTable.table[dwNumEntries];
                        theApp.QueueDebugLogLine(false, _T("NAFC: Adapter %u is '%s'"), mibIfRow.dwIndex, (CString)mibIfRow.bDescr);
                }
                }

				//Xman forceNAFCadapter-option
				if(thePrefs.GetForcedNAFCAdapter()!=0)
				{
					theApp.QueueDebugLogLine(false, _T("NAFC: you forced to use NAFC-Adapter with index: %u"), thePrefs.GetForcedNAFCAdapter());
					return thePrefs.GetForcedNAFCAdapter();
				}
				//Xman end
                // Retrieve the default used IP (=> in case of multiple adapters)
                char hostName[256];
                if(gethostname(hostName, sizeof(hostName)) == 0){
                    hostent* lphost = gethostbyname(hostName);
                    if(lphost != NULL){
                        DWORD dwAddr = ((LPIN_ADDR)lphost->h_addr)->s_addr;
                        // Pick the interface matching the IP
                      BYTE buffer[10*sizeof(MIB_IPADDRROW)];
                      ULONG size = sizeof(buffer);
                      MIB_IPADDRTABLE& mibIPAddrTable = reinterpret_cast<MIB_IPADDRTABLE&>(buffer[0]);
                        if(m_fGetIpAddrTable(&mibIPAddrTable, &size, FALSE) == 0){
                            for(DWORD i = 0; i < mibIPAddrTable.dwNumEntries; i++){
                                if(mibIPAddrTable.table[i].dwAddr == dwAddr){
                                    //const MIB_IPADDRROW& row = mibIPAddrTable.table[i];

                                    m_errorTraced = false;
                                    theApp.QueueDebugLogLine(false, _T("NAFC: Select adapter with index %u"), mibIPAddrTable.table[i].dwIndex);
                                    return mibIPAddrTable.table[i].dwIndex;
                                }
                            }
                        }
                        else {
                            if(m_errorTraced == false){
                                m_errorTraced = true;
                                theApp.QueueDebugLogLine(false, _T("NAFC: Failed to get IP tables error=0x%x"), ::GetLastError());
                                return 0;
                            }
                        }
                    }
                }
         }
            if(m_errorTraced == false){
                m_errorTraced = true;
                theApp.QueueDebugLogLine(false, _T("NAFC: Failed to get tables of interface error=0x%x"), ::GetLastError());
                return 0;
            }
      }
        if(m_errorTraced == false){
            m_errorTraced = true;
            theApp.QueueDebugLogLine(false, _T("NAFC: Failed to get the number of interface error=0x%x"), ::GetLastError());
            return 0;
        }
   }
   return 0;
}

//                 Only the code related to the instance bandwidth is 
//                 multithread-safe (see tag /**/)
//
void CBandWidthControl::Process()
{
	static DWORD processtime;
	if(::GetTickCount()-processtime >= 1000){
		processtime = ::GetTickCount();

		// Try to get the Adapter Index to access to the right interface
		if(m_currentAdapterIndex == 0){
			m_currentAdapterIndex = getAdapterIndex();

			// Disable NAFC
			if(m_currentAdapterIndex == 0){
				thePrefs.SetNAFCFullControl(false);
			}
		}

		/*->*/m_statisticLocker.Lock();
		/**/ // Update the datarate directly from the network Adapter
		/**/ if(m_currentAdapterIndex != 0 && m_fGetIfEntry != NULL){
		/**/ 	static int s_Log; // Static initiate with zero
		/**/ 	MIB_IFROW ifRow;       
		/**/ 	ifRow.dwIndex = m_currentAdapterIndex;
		/**/ 	if(m_fGetIfEntry(&ifRow) == NO_ERROR){
		/**/ 		s_Log = 0;
		/**/ 
		/**/		//Xman prevent overflow on adapterchange:
		/**/		if(ifRow.dwOutOctets >= m_networkOutOctets && ifRow.dwInOctets >= m_networkInOctets)
		/**/		{
		/**/			// Add the delta, since the last measure (convert 32 to 64 bits)
		/**/			m_statistic.networkInOctets += (DWORD)(ifRow.dwInOctets - m_networkInOctets);
		/**/			m_statistic.networkOutOctets += (DWORD)(ifRow.dwOutOctets - m_networkOutOctets);
		/**/		}
		/**/		//Xman show complete internettraffic
		/**/		if(initialnetworkOutOctets==0)
		/**/		{
		/**/			initialnetworkOutOctets=m_statistic.networkOutOctets;
		/**/			initialnetworkInOctets=m_statistic.networkInOctets;
		/**/		}
		/**/		//Xman end
		/**/
		/**/		// Keep last measure
		/**/		m_networkOutOctets = ifRow.dwOutOctets; 
		/**/		m_networkInOctets = ifRow.dwInOctets;
		/**/ 	}
		/**/ 	else {
		/**/ 		if(s_Log == 0){
		/**/ 			s_Log = 1;
		/**/ 			theApp.QueueDebugLogLine(false, _T("NAFC: Failed to retrieve adapter traffic, error=0x%x"), ::GetLastError());
		/**/			wasNAFCLastActive=thePrefs.GetNAFCFullControl(); //Xman  
		/**/ 			// Disable NAFC
		/**/ 			thePrefs.SetNAFCFullControl(false);
		/**/ 		}
		/**/ 
		/**/ 	}
		/**/ }
		/**/ 
		/**/ // Update large history list for the history graph
		/**/ m_statisticHistory.AddHead(Statistic(m_statistic, ::GetTickCount()));
		/**/ 
		/**/
		/**/
		/**/// Trunk size of the list (The timestamp is more accurate than the period of Process())
		/**/const uint32 averageMinTime = (uint32)(60000 * thePrefs.GetStatsAverageMinutes());
		#pragma warning(disable:4127)
		/**/while(true){
		/**/	const uint32 deltaTime = m_statisticHistory.GetHead().timeStamp - m_statisticHistory.GetTail().timeStamp;
		/**/	if(deltaTime <= averageMinTime){
		/**/		break; // exit loop
		/**/	}
		/**/	m_statisticHistory.RemoveTail(); // Trunk size
		/**/}
		#pragma warning(default:4127)
		/*->*/ m_statisticLocker.Unlock();
		

		// Calculate the dynamic download limit
		m_maxDownloadLimit = thePrefs.GetMaxDownload();
		m_maxUploadLimit = thePrefs.GetMaxUpload();
		
		//Xman GlobalMaxHarlimit for fairness
		//remark: we need a new downloadlimit, which is calculated by the real emule-upload
		//we have to calculate it on each loop.
		//also we need the additionally limit for NAFC, if the forced downloadlimit isn't be used
		m_maxforcedDownloadlimit=m_maxDownloadLimit; //initialize with the standard
		if(thePrefs.m_bAcceptsourcelimit==false) //only if user doesn't accept it
		{
			if(m_statisticHistory.GetSize() > 10){
				const Statistic& newestSample = m_statisticHistory.GetHead();
				const Statistic& oldestSample = m_statisticHistory.GetAt(m_statisticHistory.FindIndex(10)); //avg over 10 seconds, should give a smoother downloadgraph
				const uint32 deltaTime = (newestSample.timeStamp - oldestSample.timeStamp); // in [ms]
				if (deltaTime == 0) 
				{
					//Xman to be sure
					return;
				}
				const uint32 eMuleOut = (1000 * (uint32)(newestSample.eMuleOutOctets - oldestSample.eMuleOutOctets) / deltaTime); // in [Bytes/s]
				//remark: this is always a 1:4 Ratio-Limit
				float maxDownloadLimit = (float)(4*eMuleOut) / 1024.0f; // [KB/s]
				if(maxDownloadLimit < m_maxforcedDownloadlimit){
					m_maxforcedDownloadlimit = maxDownloadLimit;
				}
				if(m_maxforcedDownloadlimit<1.0f)
					m_maxforcedDownloadlimit=1.0f;
			}
		}
		//Xman end GlobalMaxHarlimit for fairness

		if(thePrefs.GetNAFCFullControl() == true ){    
			// Remark: Because there is only ONE writer thread to m_statisticHistory, there is no need to 
			//         protect its access inside this method. 
			//         => m_statisticHistory is only modified inside this method
			//         => Its access in GetDatarates() + GetFullHistoryDatarates() still must be protected

			if(m_statisticHistory.GetSize() > 10){
				const Statistic& newestSample = m_statisticHistory.GetHead();
				const Statistic& oldestSample = m_statisticHistory.GetAt(m_statisticHistory.FindIndex(10)); //avg over 10 seconds, should give a smoother downloadgraph
				const uint32 deltaTime = (newestSample.timeStamp - oldestSample.timeStamp); // in [ms]
				if (deltaTime == 0) 
				{
					//Xman shouldn't happen, but happend
					return;
				}
				const uint32 eMuleOutOverall = (1000 * (uint32)(newestSample.eMuleOutOverallOctets - oldestSample.eMuleOutOverallOctets) / deltaTime); // in [Bytes/s]
				//const uint32 networkOut = (1000 * (uint32)(newestSample.networkOutOctets - oldestSample.networkOutOctets) / deltaTime); // in [Bytes/s]
				// Dynamic limit with ratio
				if(eMuleOutOverall < 4*1024){
					// Ratio 3x
					float maxDownloadLimit = (float)(3*eMuleOutOverall) / 1024.0f; // [KB/s]
					if(maxDownloadLimit < m_maxDownloadLimit){
						m_maxDownloadLimit = maxDownloadLimit;
					}
				}
				else if(eMuleOutOverall < 11*1024){ //Xman changed to 11
					// Ratio 4x
					float maxDownloadLimit = (float)(4*eMuleOutOverall) / 1024.0f; // [KB/s]
					if(maxDownloadLimit < m_maxDownloadLimit){
						m_maxDownloadLimit = maxDownloadLimit;
					}
				}

				if(m_maxDownloadLimit < 1.0f){
					m_maxDownloadLimit = 1.0f;
				}
			}
		}
	}
	else if(thePrefs.GetNAFCFullControl() == true ){
		// Retrieve the network flow => necessary for the NAFC with 'Auto U/D limit'
		if(m_currentAdapterIndex != 0 && m_fGetIfEntry != NULL){
			MIB_IFROW ifRow;       
			ifRow.dwIndex = m_currentAdapterIndex;
			if(m_fGetIfEntry(&ifRow) == NO_ERROR){
				
				// Add the delta, since the last measure (convert 32 to 64 bits)
				m_statisticLocker.Lock();  
				/**/	//Xman prevent overflow on adapterchange:
				/**/	if(ifRow.dwOutOctets >= m_networkOutOctets && ifRow.dwInOctets >= m_networkInOctets)
				/**/	{
				/**/		m_statistic.networkInOctets += (DWORD)(ifRow.dwInOctets - m_networkInOctets);
				/**/		m_statistic.networkOutOctets += (DWORD)(ifRow.dwOutOctets - m_networkOutOctets);
				/**/	}
				m_statisticLocker.Unlock();

				// Keep last measure
				m_networkOutOctets = ifRow.dwOutOctets;
				m_networkInOctets = ifRow.dwInOctets;
			}
		}
	}
}

void CBandWidthControl::GetDatarates(UINT samples, 
									 uint32& eMuleIn, uint32& eMuleInOverall, 
									 uint32& eMuleOut, uint32& eMuleOutOverall,
									 uint32& networkIn, uint32& networkOut) const 
{
	eMuleIn = 0;
	eMuleInOverall = 0;
	eMuleOut = 0;
	eMuleOutOverall = 0;
	networkIn = 0;
	networkOut = 0;

	// Check if the list is already long enough
	/*->*/ m_statisticLocker.Lock();
	/**/ if(m_statisticHistory.GetSize() >= 2 && samples >= 1){
	/**/ 
	/**/	// Retrieve the location of the n previous sample
	/**/	POSITION pos = m_statisticHistory.FindIndex(samples);
	/**/    if(pos == NULL){
	/**/		pos = m_statisticHistory.GetTailPosition();
	/**/    }           
	/**/
	/**/    const Statistic& newestSample = m_statisticHistory.GetHead();
	/**/    const Statistic& oldestSample = m_statisticHistory.GetAt(pos);
	/**/    const uint32 deltaTime = (newestSample.timeStamp - oldestSample.timeStamp); // in [ms]
	/**/     
	/**/    if(deltaTime > 0){
	/**/		eMuleIn = (uint32)(1000 * (newestSample.eMuleInOctets - oldestSample.eMuleInOctets) / deltaTime); // in [Bytes/s]
	/**/        eMuleInOverall = (uint32)(1000 * (newestSample.eMuleInOverallOctets - oldestSample.eMuleInOverallOctets) / deltaTime); // in [Bytes/s]
	/**/        eMuleOut = (uint32)(1000 * (newestSample.eMuleOutOctets - oldestSample.eMuleOutOctets) / deltaTime); // in [Bytes/s]
	/**/        eMuleOutOverall = (uint32)(1000 * (newestSample.eMuleOutOverallOctets - oldestSample.eMuleOutOverallOctets) / deltaTime); // in [Bytes/s]
	/**/        networkIn = (uint32)(1000 * (newestSample.networkInOctets - oldestSample.networkInOctets) / deltaTime); // in [Bytes/s]
	/**/        networkOut = (uint32)(1000 * (newestSample.networkOutOctets - oldestSample.networkOutOctets) / deltaTime); // in [Bytes/s]
	/**/    }
	/**/ }
	/*->*/ m_statisticLocker.Unlock();
}

void CBandWidthControl::GetFullHistoryDatarates(uint32& eMuleInHistory, uint32& eMuleOutHistory, 
												uint32& eMuleInSession, uint32& eMuleOutSession) const 
{
	eMuleInHistory = 0;
	eMuleOutHistory = 0;
	eMuleInSession = 0;
	eMuleOutSession = 0;

	// Check if the list is already long enough
	/*->*/ m_statisticLocker.Lock();
	/**/ if(m_statisticHistory.GetSize() >= 2){
	/**/
	/**/	const Statistic& newestSample = m_statisticHistory.GetHead();
	/**/    const Statistic& oldestSample = m_statisticHistory.GetTail();
	/**/
	/**/    // Average value since the last n minutes
	/**/    uint32 deltaTime = (newestSample.timeStamp - oldestSample.timeStamp); // in [ms]
	/**/    if(deltaTime > 0){
	/**/        eMuleInHistory = (uint32)(1000 * (newestSample.eMuleInOctets - oldestSample.eMuleInOctets) / deltaTime); // in [Bytes/s]
	/**/        eMuleOutHistory = (uint32)(1000 * (newestSample.eMuleOutOctets - oldestSample.eMuleOutOctets) / deltaTime); // in [Bytes/s]
	/**/    }
	/**/ 
	/**/    // Average value since the start of the client
	/**/    deltaTime = (::GetTickCount() - GetStartTick()) / 1000; // in [s]
	/**/    if(deltaTime > 0){
	/**/        eMuleInSession = (uint32)(newestSample.eMuleInOctets / deltaTime); // in [Bytes/s]
	/**/        eMuleOutSession = (uint32)(newestSample.eMuleOutOctets / deltaTime); // in [Bytes/s]
	/**/    }
	/**/ }
	/*->*/ m_statisticLocker.Unlock();
}

//Xman 1:3 Ratio
float CBandWidthControl::GetMaxDownloadEx(bool force)
{
	//Xman GlobalMaxHarlimit for fairness
	if(force && GeteMuleIn()>GeteMuleOut()*3) //session/amount based ratio
		return GetForcedDownloadlimit();
	//Xman end

   if(thePrefs.Is13Ratio() && GeteMuleIn()<=GeteMuleOut()*3) 
       return UNLIMITED;
   else
      if(thePrefs.GetNAFCFullControl()==true)
         return GetMaxDownload();
      else
         return thePrefs.GetMaxDownload();
}
//Xman end

uint64 CBandWidthControl::GeteMuleOut() const 
{
   uint64 value;
   m_statisticLocker.Lock();
   /**/ value = m_statistic.eMuleOutOctets;
   m_statisticLocker.Unlock();
   return value;
}

uint64 CBandWidthControl::GeteMuleIn() const 
{
   uint64 value;
   m_statisticLocker.Lock();
   /**/ value = m_statistic.eMuleInOctets;
   m_statisticLocker.Unlock();
   return value;
}

uint64 CBandWidthControl::GeteMuleOutOverall() const 
{
   uint64 value;
   m_statisticLocker.Lock();
   /**/ value = m_statistic.eMuleOutOverallOctets;
   m_statisticLocker.Unlock();
   return value;
}

uint64 CBandWidthControl::GeteMuleInOverall() const 
{
   uint64 value;
   m_statisticLocker.Lock();
   /**/ value = m_statistic.eMuleInOverallOctets;
   m_statisticLocker.Unlock();
   return value;
}

uint64 CBandWidthControl::GetNetworkOut() const 
{
   uint64 value;
   m_statisticLocker.Lock();
   /**/ value = m_statistic.networkOutOctets;
   m_statisticLocker.Unlock();
   return value;
}

uint64 CBandWidthControl::GetNetworkIn() const 
{
   uint64 value;
   m_statisticLocker.Lock();
   /**/ value = m_statistic.networkInOctets;
   m_statisticLocker.Unlock();
   return value;
}

//Xman show complete internettraffic
uint64 CBandWidthControl::GetSessionNetworkIn() const
{
	uint64 value;
	m_statisticLocker.Lock();
	/**/ value = m_statistic.networkInOctets;
	m_statisticLocker.Unlock();
	return value - initialnetworkInOctets;

}

uint64 CBandWidthControl::GetSessionNetworkOut() const
{
	uint64 value;
	m_statisticLocker.Lock();
	/**/ value = m_statistic.networkOutOctets;
	m_statisticLocker.Unlock();
	return value - initialnetworkOutOctets;

}
//Xman end
uint32 CBandWidthControl::GetStartTick() const 
{
   uint32 value;
   m_statisticLocker.Lock();
   /**/ value = m_statistic.timeStamp;
   m_statisticLocker.Unlock();
   return value;
}

void CBandWidthControl::AddeMuleOutUDPOverall(uint32 octets)
{
   octets += (20 /* IP */ + 8 /* UDP */);
   m_statisticLocker.Lock();
   /**/ m_statistic.eMuleOutOverallOctets += octets;
   m_statisticLocker.Unlock();
}

void CBandWidthControl::AddeMuleOutTCPOverall(uint32 octets) 
{
   octets += (20 /* IP */ + 20 /* TCP */);
   m_statisticLocker.Lock();
   /**/ m_statistic.eMuleOutOverallOctets += octets;
   m_statisticLocker.Unlock();
}

void CBandWidthControl::AddeMuleOutOverallNoHeader(uint32 octets) 
{
   m_statisticLocker.Lock();
   /**/ m_statistic.eMuleOutOverallOctets += octets;
   m_statisticLocker.Unlock();
}
void CBandWidthControl::AddeMuleInUDPOverall(uint32 octets) 
{
   octets += (20 /* IP */ + 8 /* UDP */);
   m_statisticLocker.Lock();
   /**/ m_statistic.eMuleInOverallOctets += octets;
   m_statisticLocker.Unlock();
}

void CBandWidthControl::AddeMuleInTCPOverall(uint32 octets)
{
   octets += (20 /* IP */ + 20 /* TCP */);
   m_statisticLocker.Lock();
   /**/ m_statistic.eMuleInOverallOctets += octets;
   m_statisticLocker.Unlock();
}

void CBandWidthControl::AddeMuleOut(uint32 octets)
{
   m_statisticLocker.Lock();
   /**/ m_statistic.eMuleOutOctets += octets;
   m_statisticLocker.Unlock();
}

void CBandWidthControl::AddeMuleIn(uint32 octets)
{
   m_statisticLocker.Lock();
   /**/ m_statistic.eMuleInOctets += octets;
   m_statisticLocker.Unlock();
}

void CBandWidthControl::AddeMuleSYNACK()
{
	m_statisticLocker.Lock();
	/**/ m_statistic.eMuleInOverallOctets += 40; // IP + TCP
	/**/ m_statistic.eMuleOutOverallOctets += 40; // IP + TCP
	m_statisticLocker.Unlock();
}

#ifdef PRINT_STATISTIC
void CBandWidthControl::PrintStatistic()
{
	m_statisticLocker.Lock();
	AddLogLine(false, _T("Bandwidthcontrol: number of statistic-elements: %u"), m_statisticHistory.GetSize());
	m_statisticLocker.Unlock();
}
#endif