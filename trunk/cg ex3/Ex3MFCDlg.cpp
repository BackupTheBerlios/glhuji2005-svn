// Ex3MFCDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Ex3MFC.h"
#include "Ex3MFCDlg.h"
#include ".\Ex3mfcdlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "OpenGLWin.h"
extern COpenGLWin g_OpenGLWin;
void usage();

// CAboutDlg dialog used for App About

#define FIX_FLOAT_TEXT(text)\
		while (text[text.GetLength()-1] == '0')\
		{\
			text = text.Left(text.GetLength()-1);\
			if (text[text.GetLength()-1] == '.')\
			{\
				text = text.Left(text.GetLength()-1);\
				break;\
			}\
		}

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


// CEx3MFCDlg dialog



CEx3MFCDlg::CEx3MFCDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEx3MFCDlg::IDD, pParent)
{
	m_bCreated = false;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CEx3MFCDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PLAY, m_PlayBtn);
}

BEGIN_MESSAGE_MAP(CEx3MFCDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_BN_CLICKED(IDC_NEXT, OnBnClickedNext)
	ON_BN_CLICKED(IDC_PLAY, OnBnClickedPlay)
	ON_BN_CLICKED(IDCLOSEFILE, OnBnClickedClosefile)
	ON_WM_CHAR()
	ON_BN_CLICKED(IDB_OPENFILE, OnBnClickedOpenfile)
	ON_BN_CLICKED(ID_HELP, OnBnClickedHelp)
END_MESSAGE_MAP()


// CEx3MFCDlg message handlers

BOOL CEx3MFCDlg::OnInitDialog()
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

	m_bCreated = true;
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CEx3MFCDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CEx3MFCDlg::OnPaint()
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
HCURSOR CEx3MFCDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
void CEx3MFCDlg::OnBnClickedOk()
{
}

void CEx3MFCDlg::OnBnClickedCancel()
{
//	OnCancel();
	exit(0);
}

void CEx3MFCDlg::OnBnClickedNext()
{
	g_OpenGLWin.gotoNextFrame();
}

void CEx3MFCDlg::OnBnClickedPlay()
{

	UpdateData(TRUE);
	g_OpenGLWin.playPause();
	UpdateData(FALSE);
}

void CEx3MFCDlg::OnBnClickedClosefile()
{
	g_OpenGLWin.Close();
}

void CEx3MFCDlg::updateFarme()
{
	int nFrames = g_OpenGLWin.getFrameCount();
	int nFrame = g_OpenGLWin.getCurFrame();
	/*UpdateData(TRUE);
	CString sFrame;
	sFrame.Format("%d / %d", nFrame, nFrames==0?nFrames:nFrames-1);
	m_CurFrame.SetWindowText(sFrame);
	UpdateData(FALSE);*/
}

BOOL CEx3MFCDlg::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
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
	ON_NOTIFY_REFLECT(LVN_ODFINDITEM, OnLvnOdfinditem)
	ON_NOTIFY_REFLECT(LVN_ODCACHEHINT, OnLvnOdcachehint)
	ON_NOTIFY_REFLECT(LVN_GETDISPINFO, OnLvnGetdispinfo)
	ON_NOTIFY_REFLECT(NM_CLICK, OnNMClick)
END_MESSAGE_MAP()

void CGrid::OnHdnItemdblclick(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
	*pResult = m_nBlockChanges;
}

void CGrid::OnHdnItemchanging(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = m_nBlockChanges;
}

void CGrid::OnHdnBegindrag(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = m_nBlockChanges;
}

void CGrid::OnHdnBegintrack(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = m_nBlockChanges;
}

void CGrid::OnLvnOdfinditem(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLVFINDITEM pFindInfo = reinterpret_cast<LPNMLVFINDITEM>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}

void CGrid::OnLvnOdcachehint(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLVCACHEHINT pCacheHint = reinterpret_cast<LPNMLVCACHEHINT>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}
#define RAND_DBL (((double)rand())/((double)RAND_MAX))

bool CGrid::AddCol(Point3d* pPoint)
{
	if (pPoint == NULL)
		m_Values.push_back (Point3d(1,1,1));
	else
		m_Values.push_back (*pPoint);
	return RearrangeCols();
}
bool CGrid::DelCol()
{
	if (m_Values.size() > 0)
	{
		m_Values.erase(m_Values.end()-1);
	}
	return RearrangeCols();
}
bool CGrid::ClearCols()
{
	m_Values.clear();
	return RearrangeCols();
}
bool CGrid::RearrangeCols()
{
	char buff [500];
	if (GetSafeHwnd() == NULL)
		return false;
	SetRedraw(FALSE);
	m_nBlockChanges = 0;
	CHeaderCtrl* pHeader = GetHeaderCtrl();
	LVCOLUMN item;
	item.mask = LVCF_TEXT | LVCF_WIDTH;
	item.pszText = buff;
	item.cchTextMax = 499;
	int nCols = pHeader->GetItemCount()-1;	//Number of columns - excluding the oadding column
	CRect rec;
	pHeader->GetWindowRect(&rec);
	int w = rec.Width() + 2;
	while (nCols < (int)m_Values.size()+1)
	{
		InsertColumn(0," ", LVCFMT_CENTER, 55);
		nCols++;
	}
	while (nCols > (int)m_Values.size()+1)
	{
		DeleteColumn(0);
		nCols--;
	}
	int r = -((nCols-1)/2);
	CString sText;
	for (int i=0; i<=nCols; i++)
	{
		GetColumn(i, &item);
		if (i == 0)
		{
			item.cx = 40;
			w -= 40;
			sText = m_sTitle;
		}
		else if (i == nCols)
		{
//			SetColumnWidth(i,max(w,0));
			item.cx = max(w,0);
			sText.Empty();
		}
		else
		{
//			SetColumnWidth(i,55);
			item.cx = 55;
			w -= 55;
			sText.Format("%d", r++);
		}
		lstrcpyn(item.pszText, sText, item.cchTextMax);
		SetColumn(i, &item);
	}
	m_nBlockChanges = 1;
	SetRedraw(TRUE);
	return true;
}

void CGrid::OnLvnGetdispinfo(NMHDR *pNMHDR, LRESULT *pResult)
{

	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);

    int nRow = pDispInfo->item.iItem;
    int nCol = pDispInfo->item.iSubItem;

	//Do the list need text information?
    if (pDispInfo->item.mask & LVIF_TEXT)
    {
        CString text;
		if (nCol > 0 && nCol < (int)(m_Values.size()+1))
		{
			text.Format("%.3f", m_Values[nCol-1][nRow]);
			FIX_FLOAT_TEXT(text);
		}
		else
		{
			if (nCol == 0)
			{
				switch (nRow)
				{
				case 0:
					text = "x";
					break;
				case 1:
					text = "y";
					break;
				case 2:
					text = "z";
					break;
				default:
					text = "";
					break;
				}
			}
			else
				text = "";
		}
		lstrcpyn(pDispInfo->item.pszText, text, pDispInfo->item.cchTextMax);
	}
	*pResult = 0;
}

void CGrid::OnNMClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	SetRedraw(FALSE);
	LVHITTESTINFO hti;
	POINT pt;
	*pResult = 0;
	if (!GetCursorPos(&pt))
		return;	//Can't hittest wthout posiiton
	ScreenToClient(&pt);
	hti.pt = pt;
	SubItemHitTest(&hti);
	switch (hti.flags)
	{
	case LVHT_ONITEMICON:
	case LVHT_ONITEMLABEL:
	case LVHT_ONITEMSTATEICON:
		break;
	default:
		return;	//Click is not on a cell
	}
	int nIndex = hti.iSubItem-1;
	if (nIndex < 0 || nIndex >= (int)m_Values.size())
		return;	//Click is not on value cell

	CRect IRec,WRec;
	GetWindowRect(WRec);
	GetSubItemRect(hti.iItem, hti.iSubItem, LVIR_BOUNDS, IRec);
	if (IRec.right >= WRec.Width())
		Scroll(CSize(IRec.right-WRec.Width()+5,0));
	if (IRec.left <= 0)
		Scroll(CSize(IRec.left-5,0));
	GetSubItemRect(hti.iItem, hti.iSubItem, LVIR_BOUNDS, IRec);

	CString text;
	text.Format("%.3f",m_Values[nIndex][hti.iItem]);
	FIX_FLOAT_TEXT(text);
	if (m_pQuickEdit == NULL)
	{
		m_pQuickEdit = new CQuickEdit(this, CPoint(nIndex, hti.iItem));
		m_pQuickEdit->Create(ES_LEFT|WS_CHILD, IRec, this, 1234);
	}
	SetRedraw(TRUE);
	m_pQuickEdit->Track(IRec);
	m_pQuickEdit->m_Pt = CPoint(nIndex, hti.iItem);
	m_pQuickEdit->SetWindowText(text);
	m_pQuickEdit->SetFocus();
}

void CEx3MFCDlg::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default

	CDialog::OnChar(nChar, nRepCnt, nFlags);
}

BOOL CGrid::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	return CListCtrl::Create(dwStyle, rect, pParentWnd, nID);
	InsertColumn(0,"", LVCFMT_CENTER, 25);
	InsertColumn(1,"", LVCFMT_CENTER, 340);
}

void CGrid::SetTitle(const CString sTitle)
{
	m_sTitle = sTitle;
	RearrangeCols();
}

BEGIN_MESSAGE_MAP(CQuickEdit, CEdit)
ON_WM_KILLFOCUS()
ON_WM_CHAR()
END_MESSAGE_MAP()

void CQuickEdit::OnKillFocus(CWnd* pNewWnd)
{
	CEdit::OnKillFocus(pNewWnd);
	if(!m_bClosing)
		StopEdit(false);
}

void CQuickEdit::OnChar (UINT nChar, UINT nRepCnt, UINT nFlags)
{
    BOOL Shift = GetKeyState (VK_SHIFT) < 0;
    switch (nChar)
    {
		case VK_ESCAPE :
		{
			//GetParent()->SetFocus();
			StopEdit(TRUE);
			return;
		}
		case VK_RETURN :
		{
			StopEdit(FALSE);
			return;
		}
		case '-':
		case '.':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
		case '0':
		case 0x08:	//Backspace
			break;
		default:
			return;
    }
	CEdit::OnChar(nChar, nRepCnt, nFlags);
}

//Stop editing
void CQuickEdit::StopEdit(bool cancel)
{
	m_bClosing = true;
	CString text;
	GetWindowText (text);

	if(m_pParent != NULL)
	{
		m_pParent->m_pQuickEdit = NULL;
		if (!cancel)
		{
			m_pParent->m_Values[(int)m_Pt.x][(int)m_Pt.y] = atof(text);
		}
	}
	ShowWindow(SW_HIDE);
	if(m_pParent != NULL)
	{
		m_pParent->RedrawWindow();
	}
}

void CQuickEdit::Track(RECT rec)
{
	SetWindowPos(NULL, rec.left+2, rec.top, -3+rec.right-rec.left, -2+rec.bottom-rec.top, SWP_NOZORDER);
	SetFont(&m_Font, FALSE);
	m_bClosing = false;
	CString sText;
	GetWindowText(sText);
	ShowWindow(SW_SHOW);
	SetSel(0,sText.GetLength()-1);
}

void CEx3MFCDlg::OnBnClickedOpenfile()
{
	static char BASED_CODE szFilter[] = "Simulation Files (*.sim)|*.sim|All Files (*.*)|*.*||";
	char filename [520];
	filename[0] = '\0';	//ToDO: use current filename
	int len = GetCurrentDirectory(497, filename);
	if (len > 0 && filename[len-1] != '\\')
	{
		filename[len] = '\\';
		filename[len+1] = '\0';
		strcat(filename, "*.sim");
	}
	CFileDialog file (true, "sim", filename, OFN_FILEMUSTEXIST|OFN_ENABLESIZING, szFilter, this);
	if (file.DoModal() != IDOK)
		return;

	UpdateData(TRUE);
	m_PlayBtn.SetWindowText("||");
	UpdateData(FALSE);
	g_OpenGLWin.Run(file.GetFileName());
}

void CEx3MFCDlg::OnBnClickedHelp()
{

	char buff [1500];
	strcpy(buff, "Ex3_1 in Advanced topics in computer graphics\n");
	strcat(buff, "By: Jeremy Moskovich, Dagan Eshar & Itai Basel\n");
	strcat(buff, "\n");
	strcat(buff, "Open simulation file with the open button\n");
	strcat(buff, "\n");
	strcat(buff, "When OpenGL Window is focused, press 'h' to list more options\n");
	strcat(buff, "like controlling the speed, shading, and more.\n");
	strcat(buff, "\n");
	AfxMessageBox(buff, MB_OK|MB_ICONINFORMATION);
}
