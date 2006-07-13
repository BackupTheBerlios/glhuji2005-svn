#pragma once


// CConvUnit dialog

class CConvUnit : public CDialog
{
	DECLARE_DYNAMIC(CConvUnit)

public:
	CConvUnit(CWnd* pParent = NULL);   // standard constructor
	virtual ~CConvUnit();

// Dialog Data
	enum { IDD = IDD_CONV };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChangeP1();
	afx_msg void OnEnChangeP0();
	afx_msg void OnEnChangeP2();
};
