// ProgDlg.cpp : implementation file
//

#include "stdafx.h"
#include "regkit.h"
#include "ProgDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CProgDlg dialog


CProgDlg::CProgDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CProgDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CProgDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CProgDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProgDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CProgDlg, CDialog)
	//{{AFX_MSG_MAP(CProgDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProgDlg message handlers

BOOL CProgDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
		//  Set the posistion of the progress bar
	RECT rect;
	CFont font;

	rect.left = 27; rect.top = 36;
	rect.right = 545; rect.bottom = 54;
	
	// Initialise the common controls (progress bar thus far)
	INITCOMMONCONTROLSEX icex;
	icex.dwSize = sizeof(icex);
	icex.dwICC = ICC_PROGRESS_CLASS;
	InitCommonControlsEx(&icex);

	// Create the static control...
	if (!m_wndStatic.Create( "Initialising...",WS_CHILD | WS_VISIBLE | SS_CENTERIMAGE,
		rect, this, IDC_STATIC_INIT)) {
		MessageBox("Could not create the static control...");
	}

	// Reset the rect structure for the position of the static text...
	rect.top = 10;
	rect.bottom = 28;

	// Create the static control...
	if (!m_wndStaticTask.Create( "Task Name...",WS_CHILD | WS_VISIBLE | SS_CENTERIMAGE,
		rect, this, IDC_STATIC_TASK)) {
		MessageBox("Could not create the static control...");
	}

	rect.top = 70;
	rect.bottom = 92; 

	// Create the progress bar and set its properties...
	if (!m_wndProg.Create(WS_VISIBLE | WS_CHILD | WS_BORDER | PBS_SMOOTH,
		rect, this, IDC_PROG_INIT))
		MessageBox("Could not create contol...");

	m_wndProg.SetRange(0,100);
	m_wndProg.SetStep(1);
	SetProgress(0);

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/////////////////////////////////////////////////////////////////////////////
// These functions recieve data concerning the state of the GUI controls
// Whatever they revieve will be translated into the dialog actions

void CProgDlg::SetProgress(int nProg)
{
	m_wndProg.SetPos(nProg);
}

void CProgDlg::SetStatic(LPCTSTR pszText)
{
	m_wndStatic.SetWindowText(pszText);
}

void CProgDlg::SetStaticTask(LPCTSTR pszTask)
{
	m_wndStaticTask.SetWindowText(pszTask);
}
