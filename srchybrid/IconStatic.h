#pragma once

/////////////////////////////////////////////////////////////////////////////
// CIconStatic 

class CIconStatic : public CStatic
{
	DECLARE_DYNAMIC(CIconStatic)
public:
	CIconStatic();
	virtual ~CIconStatic();

	// ==> Design Settings [eWombat/Stulle] - Max
	/*
	void SetIcon(LPCTSTR pszIconID);
	void SetWindowText(LPCTSTR lpszString);
	*/
	void SetIcon(LPCTSTR pszIconID, COLORREF clrInput = CLR_DEFAULT);
	void SetWindowText(LPCTSTR lpszString, COLORREF clrInput = CLR_DEFAULT);
	// <== Design Settings [eWombat/Stulle] - Max

protected:
	CStatic m_wndPicture;
	CString m_strIconID;
	CString m_strText;
	CBitmap m_MemBMP;

	DECLARE_MESSAGE_MAP()
	afx_msg void OnSysColorChange();
};
