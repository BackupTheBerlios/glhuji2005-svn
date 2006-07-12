// Ex2MFCDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Ex2MFC.h"
#include "Ex2MFCDlg.h"
#include ".\ex2mfcdlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "OpenGLWin.h"
extern COpenGLWin g_OpenGLWin;

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CEx2MFCDlg dialog



CEx2MFCDlg::CEx2MFCDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEx2MFCDlg::IDD, pParent)
{
	m_bCreated = false;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CEx2MFCDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PLAY, m_PlayBtn);
	DDX_Control(pDX, IDC_FRAME_NO, m_CurFrame);
	DDX_Control(pDX, IDC_FRAME_SLIDER, m_Slider);
	DDX_Control(pDX, IDC_ROTATION_CONV_SIZE, m_RotConvSize);
	DDX_Control(pDX, IDC_STATIC_FRM, m_StaticFrame);
}

BEGIN_MESSAGE_MAP(CEx2MFCDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_BN_CLICKED(IDC_NEXT, OnBnClickedNext)
	ON_BN_CLICKED(IDC_PREV, OnBnClickedPrev)
	ON_BN_CLICKED(IDC_PLAY, OnBnClickedPlay)
	ON_BN_CLICKED(IDC_FWD, OnBnClickedFwd)
	ON_BN_CLICKED(IDC_REW, OnBnClickedRew)
	ON_BN_CLICKED(IDWRITE, OnBnClickedWrite)
	ON_BN_CLICKED(IDCLOSEFILE, OnBnClickedClosefile)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_FRAME_SLIDER, OnNMReleasedcaptureFrameSlider)
	ON_EN_CHANGE(IDC_ROTATION_CONV_SIZE, OnEnChangeRotationConvSize)
	ON_EN_CHANGE(IDC_POS_CONV_SIZE, OnEnChangePosConvSize)
	ON_BN_CLICKED(IDC_MASK_CHECK, OnBnClickedMaskCheck)
	ON_EN_CHANGE(IDC_MASK_FACTOR, OnEnChangeMaskFactor)
	ON_CBN_SELCHANGE(IDC_PRESETS, OnCbnSelchangePresets)
	ON_EN_KILLFOCUS(IDC_FRAME_NO, OnEnKillfocusFrameNo)
END_MESSAGE_MAP()


// CEx2MFCDlg message handlers

BOOL CEx2MFCDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	g_OpenGLWin.m_pParent = this;

	CRect SBRec,HRec,IRec,rec(10,10,110,110);
	m_RotVec.Create(LVS_SINGLESEL|LVS_REPORT, rec, this, 255);
	m_RotVec.SetExtendedStyle(m_RotVec.GetExtendedStyle()|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
//	m_RotVec.ShowWindow(SW_SHOW);
	m_RotVec.InsertColumn(0,"-1", LVCFMT_CENTER, 25);
	m_RotVec.InsertColumn(1,"0", LVCFMT_CENTER, 25);
	m_RotVec.InsertColumn(2,"1", LVCFMT_CENTER, 25);
	m_RotVec.InsertItem(0," ");
	m_RotVec.InsertItem(1," ");
	m_RotVec.InsertItem(2," ");
	rec.OffsetRect(0,110);
	m_PosVec.Create(LVS_SINGLESEL|LVS_REPORT, rec, this, 256);
	m_PosVec.SetExtendedStyle(m_PosVec.GetExtendedStyle()|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	m_PosVec.ShowWindow(SW_SHOW);
	m_PosVec.InsertColumn(0,"-1", LVCFMT_CENTER, 25);
	m_PosVec.InsertColumn(1,"0", LVCFMT_CENTER, 25);
	m_PosVec.InsertColumn(2,"1", LVCFMT_CENTER, 25);
	m_PosVec.InsertColumn(3,"-1", LVCFMT_CENTER, 25);
	m_PosVec.InsertColumn(4,"0", LVCFMT_CENTER, 25);
	m_PosVec.InsertColumn(5,"1", LVCFMT_CENTER, 25);
	m_PosVec.InsertColumn(6,"-1", LVCFMT_CENTER, 25);
	m_PosVec.InsertColumn(7,"0", LVCFMT_CENTER, 25);
	m_PosVec.InsertColumn(8,"1", LVCFMT_CENTER, 25);
	m_PosVec.InsertColumn(9,"-1", LVCFMT_CENTER, 2225);
	m_PosVec.InsertItem(0," ");
	m_PosVec.InsertItem(1," ");
	m_PosVec.InsertItem(2," ");
	m_PosVec.GetHeaderCtrl()->GetWindowRect(HRec);
	m_PosVec.GetItemRect(0, IRec, LVIR_BOUNDS);
	int nScrollHeigh = GetSystemMetrics(SM_CYHSCROLL);
	int nHeaderHight = HRec.Height();
	int nItemHight = IRec.Height();
	m_StaticFrame.GetWindowRect(rec);
	m_PosVec.SetWindowPos(NULL, rec.left, rec.top, rec.Width()-8, nHeaderHight+nItemHight*3+nScrollHeigh+2, SWP_NOZORDER);
	m_bCreated = true;
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CEx2MFCDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CEx2MFCDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CEx2MFCDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
void CEx2MFCDlg::OnBnClickedOk()
{
	static char BASED_CODE szFilter[] = "BVH Files (*.bvh)|*.bvh|All Files (*.*)|*.*||";
	char filename [520];
	filename[0] = '\0';	//ToDO: use current filename
	int len = GetCurrentDirectory(497, filename);
	if (len > 0 && filename[len-1] != '\\')
	{
		filename[len] = '\\';
		filename[len+1] = '\0';
		strcat(filename, "*.bvh");
	}
	CFileDialog file (true, "bvh", filename, OFN_FILEMUSTEXIST|OFN_ENABLESIZING, szFilter, this);
	if (file.DoModal() != IDOK)
		return;

	UpdateData(TRUE);
	m_PlayBtn.SetWindowText("||");
	UpdateData(FALSE);
	g_OpenGLWin.Run(file.GetFileName());
}

void CEx2MFCDlg::OnBnClickedCancel()
{
//	OnCancel();
	exit(0);
}

void CEx2MFCDlg::OnBnClickedNext()
{
	g_OpenGLWin.gotoNextFrame();
}

void CEx2MFCDlg::OnBnClickedPrev()
{
	g_OpenGLWin.gotoPrevFrame();
}

void CEx2MFCDlg::OnBnClickedPlay()
{

	UpdateData(TRUE);
	if (g_OpenGLWin.playPause())
		m_PlayBtn.SetWindowText("|>");
	else
		m_PlayBtn.SetWindowText("||");
	UpdateData(FALSE);
}

void CEx2MFCDlg::OnBnClickedFwd()
{
	g_OpenGLWin.gotoFrame(g_OpenGLWin.getFrameCount()-1);
}

void CEx2MFCDlg::OnBnClickedRew()
{
	g_OpenGLWin.gotoFrame(0);
}

void CEx2MFCDlg::OnBnClickedWrite()
{
	static char BASED_CODE szFilter[] = "BVH Files (*.bvh)|*.bvh|All Files (*.*)|*.*||";
	char filename [520];
	filename[0] = '\0';	//ToDO: use current filename
	int len = GetCurrentDirectory(497, filename);
	if (len > 0 && filename[len-1] != '\\')
	{
		filename[len] = '\\';
		filename[len+1] = '\0';
		strcat(filename, "*.bvh");
	}
	CFileDialog file (false, "bvh", g_OpenGLWin.getFileName(), OFN_ENABLESIZING, szFilter, this);
	if (file.DoModal() != IDOK)
		return;

	g_OpenGLWin.Save(file.GetFileName());
}

void CEx2MFCDlg::OnBnClickedClosefile()
{
	g_OpenGLWin.Close();
}

void CEx2MFCDlg::updateFarme()
{
	int nFrames = g_OpenGLWin.getFrameCount();
	int nFrame = g_OpenGLWin.getCurFrame();
	UpdateData(TRUE);
	m_Slider.SetRange(0,nFrames-1, TRUE);
	m_Slider.SetPos(nFrame);
	CString sFrame;
	sFrame.Format("%d / %d", nFrame, nFrames==0?nFrames:nFrames-1);
	m_CurFrame.SetWindowText(sFrame);
	UpdateData(FALSE);
}

void CEx2MFCDlg::OnNMReleasedcaptureFrameSlider(NMHDR *pNMHDR, LRESULT *pResult)
{
	UpdateData(TRUE);
	g_OpenGLWin.gotoFrame(m_Slider.GetPos());
	*pResult = 0;
}



void CEx2MFCDlg::OnEnKillfocusFrameNo()
{
	static bool bSettingFrameNo = false;
	UpdateData(TRUE);
	CString sText;
	if (g_OpenGLWin.isPaused() && !bSettingFrameNo)
	{
		bSettingFrameNo = true;
		m_CurFrame.GetWindowText(sText);
		g_OpenGLWin.gotoFrame(atoi(sText));
		bSettingFrameNo = false;		
	}
}

void CEx2MFCDlg::OnEnChangeRotationConvSize()
{
	if (!m_bCreated)
		return;
	UpdateData(TRUE);
	CString sText;
	m_RotConvSize.GetWindowText(sText);
	int nCount = atoi(sText);
}

void CEx2MFCDlg::OnEnChangePosConvSize()
{
}

void CEx2MFCDlg::OnBnClickedMaskCheck()
{
}

void CEx2MFCDlg::OnEnChangeMaskFactor()
{
}

void CEx2MFCDlg::OnCbnSelchangePresets()
{
}

BOOL CEx2MFCDlg::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	// TODO: Add your specialized code here and/or call the base class

	return CDialog::OnNotify(wParam, lParam, pResult);
}
BEGIN_MESSAGE_MAP(CGrid, CListCtrl)
	ON_NOTIFY(HDN_ITEMDBLCLICKA, 0, OnHdnItemdblclick)
	ON_NOTIFY(HDN_ITEMDBLCLICKW, 0, OnHdnItemdblclick)
	ON_NOTIFY(HDN_ITEMCHANGINGA, 0, OnHdnItemchanging)
	ON_NOTIFY(HDN_ITEMCHANGINGW, 0, OnHdnItemchanging)
	ON_NOTIFY(HDN_BEGINDRAG, 0, OnHdnBegindrag)
	ON_NOTIFY(HDN_BEGINTRACKA, 0, OnHdnBegintrack)
	ON_NOTIFY(HDN_BEGINTRACKW, 0, OnHdnBegintrack)
END_MESSAGE_MAP()

void CGrid::OnHdnItemdblclick(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}

void CGrid::OnHdnItemchanging(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 1;
}

void CGrid::OnHdnBegindrag(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 1;
}

void CGrid::OnHdnBegintrack(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 1;
}
