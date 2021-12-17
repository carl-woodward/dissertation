// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "RegKit.h"

#include "MainFrm.h"
#include "ListPane.h"
#include "HtmlPane.h"
#include "HtmlLogo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CSplitterWnd *g_wndSplitterY;
CFrameWnd *g_wndFrame;
TCHAR g_szHomeDir[MAX_PATH];

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	::GetCurrentDirectory((DWORD) (sizeof(TCHAR) * MAX_PATH), g_szHomeDir);
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	/*if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}*/

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
	/*m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);*/

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;

	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	
	cs.style &=~FWS_ADDTOTITLE;
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// When CMainFrame is instantiated the client area is prepared and created
// before beig created. Setup two splitters so I have the view structure that
// I need...


BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	// Set the panes up to my intial default...Make sure it does not break...
	// If by any chance it does, close down the app by returning false...
	if (!m_wndSplitterX.CreateStatic(this, 1, 2) ||
		!m_wndSplitterX.CreateView(0,0, RUNTIME_CLASS(CListPane), CSize (128,0), pContext) ||
		!m_wndSplitterY.CreateStatic( &m_wndSplitterX, 2, 1, WS_CHILD | WS_VISIBLE,
			m_wndSplitterX.IdFromRowCol(0,1)) ||
		!m_wndSplitterY.CreateView(0,0, RUNTIME_CLASS(CHtmlLogo), CSize (0, 220), pContext) ||
		!m_wndSplitterY.CreateView(1,0, RUNTIME_CLASS(CHtmlPane), CSize (0, 0), pContext))
		return FALSE;
	
	// Save this as a global variable so I can split panes to my delight later on!
	g_wndSplitterY = &m_wndSplitterY;

	return TRUE;
}