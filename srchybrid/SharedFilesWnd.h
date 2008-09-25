//this file is part of eMule
//Copyright (C)2002-2005 Merkur ( devs@emule-project.net / http://www.emule-project.net )
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
#include "ResizableLib\ResizableDialog.h"
#include "SharedFilesCtrl.h"
#include "ProgressCtrlX.h"
#include "IconStatic.h"
#include "SharedDirsTreeCtrl.h"
#include "SplitterControl.h"
#include "EditDelayed.h"
#include "HistoryListCtrl.h" //Xman [MoNKi: -Downloaded History-]

class CSharedFilesWnd : public CResizableDialog
{
	DECLARE_DYNAMIC(CSharedFilesWnd)

public:
	CSharedFilesWnd(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSharedFilesWnd();

	void Localize();
	//Xman [MoNKi: -Downloaded History-]
	/*
	void ShowSelectedFilesSummary();
	*/
	void ShowSelectedFilesSummary(bool bHistory =false);
	//Xman end
	void Reload();
	uint32	GetFilterColumn() const				{ return m_nFilterColumn; }

// Dialog Data
	enum { IDD = IDD_FILES };

	CSharedFilesCtrl sharedfilesctrl;
	CStringArray m_astrFilter;

private:
	CProgressCtrlX pop_bar;
	CProgressCtrlX pop_baraccept;
	CProgressCtrlX pop_bartrans;
	CFont bold;
	CIconStatic m_ctrlStatisticsFrm;
	CSharedDirsTreeCtrl m_ctlSharedDirTree;
	HICON icon_files;
	CSplitterControl m_wndSplitter;
	CEditDelayed	m_ctlFilter;
	CHeaderCtrl		m_ctlSharedListHeader;
	uint32			m_nFilterColumn;

protected:
	void SetAllIcons();
	void DoResize(int delta);

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedReloadSharedFiles();
	afx_msg void OnLvnItemActivateSharedFiles(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickSharedFiles(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSysColorChange();
	afx_msg void OnStnDblClickFilesIco();
	afx_msg void OnTvnSelChangedSharedDirsTree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg LRESULT OnChangeFilter(WPARAM wParam, LPARAM lParam);
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnShowWindow( BOOL bShow,UINT nStatus  ); //Xman [MoNKi: -Downloaded History-]
	//Xman [MoNKi: -Downloaded History-]
public:
	CHistoryListCtrl historylistctrl;
protected:
	afx_msg void OnLvnItemActivateHistorylist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickHistorylist(NMHDR *pNMHDR, LRESULT *pResult);
	//Xman end
	// ==> Design Settings [eWombat/Stulle] - Max
	CBrush m_brMyBrush;
	HBRUSH hbr;	
	COLORREF crSharedColor;
public:
	void OnBackcolor(); 
	// <== Design Settings [eWombat/Stulle] - Max
};
