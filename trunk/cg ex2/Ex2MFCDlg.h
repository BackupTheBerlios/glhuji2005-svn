// Ex2MFCDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include "resource.h"
#include "afxcmn.h"
#include "ConvUnit.h"

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

class CConvMetadata
{
	double m_fMask;
	vector<Point3d> m_Rotations;
	vector<Point3d> m_Positions;
};

// CEx2MFCDlg dialog
class CEx2MFCDlg : public CDialog
{
// Construction
public:
	CEx2MFCDlg(CWnd* pParent = NULL);	// standard constructor

	vector<CConvMetadata> m_Convs;
	bool ApplyConv(CConvMetadata* pConv);
// Dialog Data
	enum { IDD = IDD_EX2MFC_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	bool m_bCreated;
public:
	void updateFarme();

	CGrid m_RotVec;
	CGrid m_PosVec;
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
	CEdit m_CurFrame;
	CSliderCtrl m_Slider;
	afx_msg void OnNMReleasedcaptureFrameSlider(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeRotationConvSize();
	afx_msg void OnEnChangePosConvSize();
	afx_msg void OnBnClickedMaskCheck();
	afx_msg void OnEnChangeMaskFactor();
	afx_msg void OnCbnSelchangePresets();
	afx_msg void OnEnKillfocusFrameNo();
	CEdit m_RotConvSize;
protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
public:
	CStatic m_StaticFrame;
	CEdit m_PosConvSize;
	afx_msg void OnBnClickedApplyConv();
	CEdit m_MaskFactor;
	CButton m_Mask;
	CComboBox m_Presets;
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	CButton m_ApplyConv;
	afx_msg void OnBnClickedOpenfile();
	afx_msg void OnBnClickedRemoveConv();
	CStatic m_VRemove;
	CStatic m_VApply;
};
