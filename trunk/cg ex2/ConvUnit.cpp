// ConvUnit.cpp : implementation file
//

#include "stdafx.h"
#include "Ex2MFC.h"
#include "ConvUnit.h"
#include ".\convunit.h"


// CConvUnit dialog

IMPLEMENT_DYNAMIC(CConvUnit, CDialog)
CConvUnit::CConvUnit(CWnd* pParent /*=NULL*/)
	: CDialog(CConvUnit::IDD, pParent)
{
}

CConvUnit::~CConvUnit()
{
}

void CConvUnit::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CConvUnit, CDialog)
	ON_EN_CHANGE(IDC_P0, OnEnChangeP0)
	ON_EN_CHANGE(IDC_P1, OnEnChangeP1)
	ON_EN_CHANGE(IDC_P2, OnEnChangeP2)
END_MESSAGE_MAP()


// CConvUnit message handlers

void CConvUnit::OnEnChangeP0()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}
void CConvUnit::OnEnChangeP1()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}
void CConvUnit::OnEnChangeP2()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}
