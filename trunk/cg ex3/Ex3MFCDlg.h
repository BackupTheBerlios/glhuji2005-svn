// Ex3MFCDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include "resource.h"
#include "afxcmn.h"

class CGrid;
class CQuickEdit : public CEdit
{
	CGrid* m_pParent;
	bool m_bClosing;
	void StopEdit(bool cancel);
public:
	CPoint m_Pt;
	CFont m_Font;
	CQuickEdit(CGrid* pParent, const CPoint pt) : CEdit()
	{
		m_bClosing = false;
		m_pParent = pParent;
		m_Pt = pt;
		m_Font.CreateFont(-11,0,0,1,FW_NORMAL,0,0,0,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,PROOF_QUALITY,DEFAULT_PITCH,"Arial");
	}
	~CQuickEdit()
	{}
	void Track(RECT rec);
	DECLARE_MESSAGE_MAP()
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
};

class CGrid : public CListCtrl
{
	CString m_sTitle;
public:
	CQuickEdit* m_pQuickEdit;
	CGrid() : CListCtrl()
	{
		m_pQuickEdit = NULL;
		m_nBlockChanges = 1;
	}
	~CGrid()
	{}
	int m_nBlockChanges;	//blocks header from being changed
	bool AddCol(Point3d* pPoint = NULL);
	bool DelCol();
	bool RearrangeCols();
	bool ClearCols();
	vector<Point3d> m_Values;
	DECLARE_MESSAGE_MAP()
	afx_msg void OnHdnItemdblclick(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnHdnItemchanging(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnHdnBegindrag(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnHdnBegintrack(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnOdfinditem(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnOdcachehint(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnGetdispinfo(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClick(NMHDR *pNMHDR, LRESULT *pResult);
public:
	void SetTitle(const CString sTitle);
	virtual BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);
};

// CEx3MFCDlg dialog
class CEx3MFCDlg : public CDialog
{
// Construction
public:
	CEx3MFCDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_EX3MFC_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	bool m_bCreated;
public:
	void updateFarme();

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
	afx_msg void OnBnClickedPlay();
	CButton m_PlayBtn;
	afx_msg void OnBnClickedClosefile();
protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
public:
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnBnClickedOpenfile();
	afx_msg void OnBnClickedHelp();
};
