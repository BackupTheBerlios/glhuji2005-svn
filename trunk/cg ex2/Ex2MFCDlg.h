// Ex2MFCDlg.h : header file
//

#pragma once
#include "afxwin.h"


// CEx2MFCDlg dialog
class CEx2MFCDlg : public CDialog
{
// Construction
public:
	CEx2MFCDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_EX2MFC_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedNext();
	afx_msg void OnBnClickedPrev();
	afx_msg void OnBnClickedPlay();
	afx_msg void OnBnClickedFwd();
	afx_msg void OnBnClickedRew();
	CButton m_PlayBtn;
	afx_msg void OnBnClickedWrite();
	afx_msg void OnBnClickedClosefile();
};
