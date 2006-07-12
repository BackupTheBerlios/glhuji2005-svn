// Ex2MFCDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include "resource.h"
#include "afxcmn.h"
#include "ConvUnit.h"


class CGrid : public CListCtrl
{
public:
	CGrid() : CListCtrl()
	{
	}
	~CGrid()
	{}
	DECLARE_MESSAGE_MAP()
	afx_msg void OnHdnItemdblclick(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnHdnItemchanging(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnHdnBegindrag(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnHdnBegintrack(NMHDR *pNMHDR, LRESULT *pResult);
};
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
};
