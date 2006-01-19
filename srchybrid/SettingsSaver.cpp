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
#include "SettingsSaver.h"
#include "PartFile.h"
#include "emule.h"
#include "Preferences.h" // for thePrefs
#include "emuledlg.h" // for theApp.emuledlg
#include "log.h" // for log


CSettingsSaver::CSettingsSaver(void){}
CSettingsSaver::~CSettingsSaver(void){}

void CSettingsSaver::DeleteFile(CPartFile* file)
{
	CString datafilepath;
	datafilepath.Format(_T("%s\\%s\\%s.sivka"), file->GetTempPath(), _T("Extra Lists"), file->GetPartMetFileName());
	if (_tremove(datafilepath)) if (errno != ENOENT)
		AddLogLine(true, _T("Failed to delete %s, you will need to do this by hand"), datafilepath);
}

void CSettingsSaver::LoadSettings(CPartFile* file)
{
	SettingsList daten;
	CString datafilepath;
	datafilepath.Format(_T("%s\\%s\\%s.sivka"), file->GetTempPath(), _T("Extra Lists"), file->GetPartMetFileName());

	CString strLine;
	CStdioFile f;
	if (!f.Open(datafilepath, CFile::modeReadWrite | CFile::typeText))
		return;
	while(f.ReadString(strLine))
	{
		if (strLine.GetAt(0) == _T('#'))
			continue;
		int pos = strLine.Find(_T('\0'));
		if (pos == -1)
			continue;
		CString strData = strLine.Left(pos);
		CSettingsData* newdata = new CSettingsData(_tstol(strData));
		daten.AddTail(newdata);
	}
	f.Close();

	POSITION pos = daten.GetHeadPosition();
	if(!pos)
		return;

	if( ((CSettingsData*)daten.GetAt(pos))->dwData == 0 || ((CSettingsData*)daten.GetAt(pos))->dwData == 1)
	{
		file->SetEnableAutoDropNNS((((CSettingsData*)daten.GetAt(pos))->dwData)!=0);
	}
	else
		file->SetEnableAutoDropNNS(thePrefs.GetEnableAutoDropNNSDefault());

	daten.GetNext(pos);
	if( ((CSettingsData*)daten.GetAt(pos))->dwData >= 0 && ((CSettingsData*)daten.GetAt(pos))->dwData <= 60000)
		file->SetAutoNNS_Timer(((CSettingsData*)daten.GetAt(pos))->dwData);
	else
		file->SetAutoNNS_Timer(thePrefs.GetAutoNNS_TimerDefault());

	daten.GetNext(pos);
	if( ((CSettingsData*)daten.GetAt(pos))->dwData >= 50 && ((CSettingsData*)daten.GetAt(pos))->dwData <= 100)
		file->SetMaxRemoveNNSLimit(((CSettingsData*)daten.GetAt(pos))->dwData);
	else
		file->SetMaxRemoveNNSLimit(thePrefs.GetMaxRemoveNNSLimitDefault());

	daten.GetNext(pos);
	if( ((CSettingsData*)daten.GetAt(pos))->dwData == 0 || ((CSettingsData*)daten.GetAt(pos))->dwData == 1)
	{
		file->SetEnableAutoDropFQS((((CSettingsData*)daten.GetAt(pos))->dwData)!=0);
	}
	else
		file->SetEnableAutoDropFQS(thePrefs.GetEnableAutoDropFQSDefault());

	daten.GetNext(pos);
	if( ((CSettingsData*)daten.GetAt(pos))->dwData >= 0 && ((CSettingsData*)daten.GetAt(pos))->dwData <= 60000)
		file->SetAutoFQS_Timer(((CSettingsData*)daten.GetAt(pos))->dwData);
	else
		file->SetAutoFQS_Timer(thePrefs.GetAutoFQS_TimerDefault());

	daten.GetNext(pos);
	if( ((CSettingsData*)daten.GetAt(pos))->dwData >= 50 && ((CSettingsData*)daten.GetAt(pos))->dwData <= 100)
		file->SetMaxRemoveFQSLimit(((CSettingsData*)daten.GetAt(pos))->dwData);
	else
		file->SetMaxRemoveFQSLimit(thePrefs.GetMaxRemoveFQSLimitDefault());

	daten.GetNext(pos);
	if( ((CSettingsData*)daten.GetAt(pos))->dwData == 0 || ((CSettingsData*)daten.GetAt(pos))->dwData == 1)
	{
		file->SetEnableAutoDropQRS((((CSettingsData*)daten.GetAt(pos))->dwData)!=0);
	}
	else
		file->SetEnableAutoDropQRS(thePrefs.GetEnableAutoDropQRSDefault());

	daten.GetNext(pos);
	if( ((CSettingsData*)daten.GetAt(pos))->dwData >= 0 && ((CSettingsData*)daten.GetAt(pos))->dwData <= 12000)
		file->SetAutoHQRS_Timer(((CSettingsData*)daten.GetAt(pos))->dwData);
	else
		file->SetAutoHQRS_Timer(thePrefs.GetAutoHQRS_TimerDefault());

	daten.GetNext(pos);
	if( ((CSettingsData*)daten.GetAt(pos))->dwData >= 1000 && ((CSettingsData*)daten.GetAt(pos))->dwData <= 10000)
		file->SetMaxRemoveQRS(((CSettingsData*)daten.GetAt(pos))->dwData);
	else
		file->SetMaxRemoveQRS(thePrefs.GetMaxRemoveQRSDefault());

	daten.GetNext(pos);
	if( ((CSettingsData*)daten.GetAt(pos))->dwData >= 50 && ((CSettingsData*)daten.GetAt(pos))->dwData <= 100)
		file->SetMaxRemoveQRSLimit(((CSettingsData*)daten.GetAt(pos))->dwData);
	else
		file->SetMaxRemoveQRSLimit(thePrefs.GetMaxRemoveQRSLimitDefault());
	// emulate StulleMule <= v2.2 files
	if(daten.GetCount() > 10)
	{
		// ==> Global Source Limit (customize for files) - Stulle
		daten.GetNext(pos);
		if( ((CSettingsData*)daten.GetAt(pos))->dwData == 0 || ((CSettingsData*)daten.GetAt(pos))->dwData == 1)
		{
			file->SetGlobalHL((((CSettingsData*)daten.GetAt(pos))->dwData)!=0);
		}
		else
			file->SetGlobalHL(thePrefs.GetGlobalHlDefault());
		// <== Global Source Limit (customize for files) - Stulle
	}
	else
	{
			file->SetGlobalHL(thePrefs.GetGlobalHlDefault());
	}
	if(daten.GetCount() > 11)
	{
		daten.GetNext(pos);
		if( ((CSettingsData*)daten.GetAt(pos))->dwData == 0 || ((CSettingsData*)daten.GetAt(pos))->dwData == 1)
		{
			file->SetHQRXman((((CSettingsData*)daten.GetAt(pos))->dwData)!=0);
		}
		else
			file->SetHQRXman(thePrefs.GetHQRXmanDefault());
	}
	else
	{
			file->SetHQRXman(thePrefs.GetHQRXmanDefault());
	}

	while (!daten.IsEmpty()) 
		delete daten.RemoveHead();
}

void CSettingsSaver::SaveSettings(CPartFile* file)
{
	CString datafilepath;
	datafilepath.Format(_T("%s\\%s\\%s.sivka"), file->GetTempPath(), _T("Extra Lists"), file->GetPartMetFileName());
	
	CString strLine;
	CStdioFile f;

	if (!f.Open(datafilepath, CFile::modeCreate | CFile::modeWrite | CFile::typeText))
		return;

	f.WriteString(_T("#Sivka File Settings:\n"));
	strLine.Format(_T("%ld\n"), file->GetEnableAutoDropNNS());
	f.WriteString(strLine);
	strLine.Format(_T("%ld\n"), file->GetAutoNNS_Timer());
	f.WriteString(strLine);
	strLine.Format(_T("%ld\n"), file->GetMaxRemoveNNSLimit());
	f.WriteString(strLine);
	strLine.Format(_T("%ld\n"), file->GetEnableAutoDropFQS());
	f.WriteString(strLine);
	strLine.Format(_T("%ld\n"), file->GetAutoFQS_Timer());
	f.WriteString(strLine);
	strLine.Format(_T("%ld\n"), file->GetMaxRemoveFQSLimit());
	f.WriteString(strLine);
	strLine.Format(_T("%ld\n"), file->GetEnableAutoDropQRS());
	f.WriteString(strLine);
	strLine.Format(_T("%ld\n"), file->GetAutoHQRS_Timer());
	f.WriteString(strLine);
	strLine.Format(_T("%ld\n"), file->GetMaxRemoveQRS());
	f.WriteString(strLine);
	strLine.Format(_T("%ld\n"), file->GetMaxRemoveQRSLimit());
	f.WriteString(strLine);
	// ==> Global Source Limit (customize for files) - Stulle
	strLine.Format(_T("%ld\n"), file->GetGlobalHL());
	f.WriteString(strLine);
	// <== Global Source Limit (customize for files) - Stulle
	strLine.Format(_T("%ld\n"), file->GetHQRXman());
	f.WriteString(strLine);

	f.Close();
}
