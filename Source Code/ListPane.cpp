// ListPane.cpp : implementation file
//

#include "stdafx.h"
#include "RegKit.h"
#include "ListPane.h"

#include "HtmlPane.h"
#include "HtmlLogo.h"
#include "RegTree.h"
#include "RegList.h"
#include "DriveTree.h"
#include "DriveList.h"
#include "DllList.h"
#include "FileSnap.h"
#include "RegSnap.h"
#include "RegDocList.h"
#include "Summary.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CSplitterWnd * g_wndSplitterY;
extern CFrameWnd *g_wndFrame;
extern TCHAR g_szHomeDir[MAX_PATH];

#define ILI_HOME 0
#define ILI_REGVIEW 1
#define ILI_FILEVIEW 2
#define ILI_REGSNAP 3
#define ILI_FILESNAP 4
#define ILI_REGDOC 5
#define ILI_DLLDOC 6
#define ILI_SUMMARY 7

/////////////////////////////////////////////////////////////////////////////
// CListPane

IMPLEMENT_DYNCREATE(CListPane, CListView)

CListPane::CListPane()
{
}

CListPane::~CListPane()
{
}


BEGIN_MESSAGE_MAP(CListPane, CListView)
	//{{AFX_MSG_MAP(CListPane)
	ON_NOTIFY_REFLECT(NM_DBLCLK, OnDblclk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CListPane drawing

void CListPane::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CListPane diagnostics

#ifdef _DEBUG
void CListPane::AssertValid() const
{
	CListView::AssertValid();
}

void CListPane::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CListPane message handlers

BOOL CListPane::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	if (!CListView::PreCreateWindow(cs))
		return FALSE;
	
	cs.style &=~LVS_TYPEMASK;
	cs.style |= LVS_REPORT;
	return TRUE;
}

void CListPane::OnInitialUpdate() 
{
	CListView::OnInitialUpdate();
	CBitmap regView;
	CBitmap fileView;
	CBitmap regSnap;
	CBitmap fileSnap;
	CBitmap regDoc;
	CBitmap dllDoc;
	CBitmap summary;
	
	// Create all the bitmap objects...
	regView.LoadBitmap(IDB_REGVIEW);
	fileView.LoadBitmap(IDB_FILEVIEW);
	regSnap.LoadBitmap(IDB_REGSNAP);
	fileSnap.LoadBitmap(IDB_FILESNAP);
	regDoc.LoadBitmap(IDB_REGDOC);
	dllDoc.LoadBitmap(IDB_DLL_DOC);
	summary.LoadBitmap(IDB_SUMMARY);

	// Create the image lists that we are going to use...
	//m_ilLarge.Create(IDB_HOME, 32, 1, RGB (255, 0, 255));
	m_ilSmall.Create(IDB_HOME, 16, 1, RGB (255, 0, 255));
	m_ilSmall.Add(&regView, RGB (255,0,255));
	m_ilSmall.Add(&fileView, RGB (255,0,255));
	m_ilSmall.Add(&regSnap, RGB (255,0,255));
	m_ilSmall.Add(&fileSnap, RGB (255,0,255));
	m_ilSmall.Add(&regDoc, RGB(255,0,255));
	m_ilSmall.Add(&dllDoc, RGB (255,0,255));
	m_ilSmall.Add(&summary, RGB (255,0,255));
		
	// Set the image list...
	GetListCtrl().SetImageList(&m_ilSmall, LVSIL_SMALL);

	// Insert one column...I have to because I am using LVS_REPORT
	GetListCtrl().InsertColumn( 0, _T ("Application"), LVCFMT_LEFT, 128);
		
	// Add some items to my report...
	GetListCtrl().InsertItem(0, "RegKit Home", ILI_HOME);
	GetListCtrl().InsertItem(1, "Registry Viewer", ILI_REGVIEW);
	GetListCtrl().InsertItem(2, "File Viewer", ILI_FILEVIEW);
	GetListCtrl().InsertItem(3, "Registry Snapshot", ILI_REGSNAP);
	GetListCtrl().InsertItem(4, "File Snapshot", ILI_FILESNAP);
	GetListCtrl().InsertItem(5, "Registry Doctor", ILI_REGDOC);
	GetListCtrl().InsertItem(6, "DLL Doctor", ILI_DLLDOC);
	GetListCtrl().InsertItem(7, "System Summary", ILI_SUMMARY);	
}

/////////////////////////////////////////////////////////////////////////////
// Process double clicks in the list view pane
//

void CListPane::OnDblclk(NMHDR* pNMHDR, LRESULT* pResult) 
{
	int nIndex;	// Holds the number of the mini-app clicked
	
	// Calculate where the mouse click is 
	DWORD dwPos = ::GetMessagePos ();
    CPoint point ((int) LOWORD (dwPos), (int) HIWORD (dwPos));
    
	// Convert the screen co-ordinates into a list view index
	GetListCtrl ().ScreenToClient (&point);
	if ((nIndex = GetListCtrl ().HitTest (point)) != -1)
        
		// Change the views
		RunMiniApp(nIndex);		
	*pResult = 0;

}

/////////////////////////////////////////////////////////////////////////////
// This function is entirely responsible for managing the views that we are
// using. It is not complicated, it gets a handle to the view(s) concerned
// deletes them and then builds an appropriate new view...
//

void CListPane::RunMiniApp(int nIndex)
{
	CCreateContext context;
	CHtmlPane *pHtmlView;
	CHtmlLogo *pHtmlLogo;
	CRegTree *pRegTree;
	CRegList *pRegList;
	CDriveTree *pDTView;
	CDriveList *pDLView;
	CDllList *pDllList;
	CFileSnap *pFileSnap;
	CRegSnap *pRegSnap;
	CRegDocList *pRegDocList;
	CSummary *pSummary;
	
	::SetCurrentDirectory(g_szHomeDir);

	switch (nIndex) {
		
		// Create two html panes...one logo and one html help
		case ILI_HOME:
							
			// Fill our context structure...
			context.m_pNewViewClass = RUNTIME_CLASS(CHtmlLogo);
			context.m_pCurrentFrame = g_wndFrame;
			context.m_pCurrentDoc = NULL;
			context.m_pNewDocTemplate = NULL;
			context.m_pLastView = (CView*) g_wndSplitterY->GetPane(0,0);
			
			// Set up the new pane then create it...
			g_wndSplitterY->DeleteView(0,0);
			g_wndSplitterY->CreateView(0,0, RUNTIME_CLASS(CHtmlLogo),
				CSize (0,220), &context);
			g_wndSplitterY->RecalcLayout();
				
			// Show the new frame in all its glory...
			pHtmlLogo = (CHtmlLogo*)g_wndSplitterY->GetPane(0,0);
			pHtmlLogo->GetParentFrame()->RecalcLayout();
			pHtmlLogo->OnInitialUpdate();
			g_wndSplitterY->SetActivePane(0,0);

			// Set up the second pane and create it...
			context.m_pNewViewClass = RUNTIME_CLASS (CHtmlPane);
			context.m_pLastView = (CView*) g_wndSplitterY->GetPane(1,0);
			g_wndSplitterY->DeleteView(1,0);
			g_wndSplitterY->CreateView(1,0, RUNTIME_CLASS (CHtmlPane),
				CSize (0,0), &context);
			g_wndSplitterY->RecalcLayout();
	
			// Show the new pane in all its glory...
			pHtmlView = (CHtmlPane*)g_wndSplitterY->GetPane(1,0);
			pHtmlView->GetParentFrame()->RecalcLayout();
			pHtmlView->OnInitialUpdate();
			g_wndSplitterY->SetActivePane(1,0);
			break;
		
		// Creates a tree pane for registry structure and a second for the list
		// view which contains the registry values in the branch
		case ILI_REGVIEW:
			// Fill our context structure...
			context.m_pNewViewClass = RUNTIME_CLASS(CRegTree);
			context.m_pCurrentFrame = g_wndFrame;
			context.m_pCurrentDoc = NULL;
			context.m_pNewDocTemplate = NULL;
			context.m_pLastView = (CView*) g_wndSplitterY->GetPane(0,0);
			
			// Set up the new pane then create it...
			g_wndSplitterY->DeleteView(0,0);
			g_wndSplitterY->CreateView(0,0, RUNTIME_CLASS(CRegTree),
				CSize (0,220), &context);
			g_wndSplitterY->RecalcLayout();
				
			// Show the new frame in all its glory...
			pRegTree = (CRegTree*)g_wndSplitterY->GetPane(0,0);
			pRegTree->GetParentFrame()->RecalcLayout();
			pRegTree->OnInitialUpdate();
			g_wndSplitterY->SetActivePane(0,0);

			// Set up the second pane and create it...
			context.m_pNewViewClass = RUNTIME_CLASS (CRegList);
			context.m_pLastView = (CView*) g_wndSplitterY->GetPane(1,0);
			g_wndSplitterY->DeleteView(1,0);
			g_wndSplitterY->CreateView(1,0, RUNTIME_CLASS (CRegList),
				CSize (0,0), &context);
			g_wndSplitterY->RecalcLayout();
	
			// Show the new pane in all its glory...
			pRegList = (CRegList*)g_wndSplitterY->GetPane(1,0);
			pRegList->GetParentFrame()->RecalcLayout();
			pRegList->OnInitialUpdate();
			g_wndSplitterY->SetActivePane(1,0);			
			break;
		case ILI_FILEVIEW:
			// Fill our context structure...
			context.m_pNewViewClass = RUNTIME_CLASS(CDriveTree);
			context.m_pCurrentFrame = g_wndFrame;
			context.m_pCurrentDoc = NULL;
			context.m_pNewDocTemplate = NULL;
			context.m_pLastView = (CView*) g_wndSplitterY->GetPane(0,0);
			
			// Set up the new pane then create it...
			g_wndSplitterY->DeleteView(0,0);
			g_wndSplitterY->CreateView(0,0, RUNTIME_CLASS(CDriveTree),
				CSize (0,220), &context);
			g_wndSplitterY->RecalcLayout();
				
			// Show the new frame in all its glory...
			pDTView = (CDriveTree*)g_wndSplitterY->GetPane(0,0);
			pDTView->GetParentFrame()->RecalcLayout();
			pDTView->OnInitialUpdate();
			g_wndSplitterY->SetActivePane(0,0);

			// Set up the second pane and create it...
			context.m_pNewViewClass = RUNTIME_CLASS (CDriveList);
			context.m_pLastView = (CView*) g_wndSplitterY->GetPane(1,0);
			g_wndSplitterY->DeleteView(1,0);
			g_wndSplitterY->CreateView(1,0, RUNTIME_CLASS (CDriveList),
				CSize (0,0), &context);
			g_wndSplitterY->RecalcLayout();
	
			// Show the new pane in all its glory...
			pDLView = (CDriveList*)g_wndSplitterY->GetPane(1,0);
			pDLView->GetParentFrame()->RecalcLayout();
			pDLView->OnInitialUpdate();
			g_wndSplitterY->SetActivePane(1,0);
			break;
		case ILI_REGSNAP:
			// Fill our context structure...
			context.m_pNewViewClass = RUNTIME_CLASS(CRegSnap);
			context.m_pCurrentFrame = g_wndFrame;
			context.m_pCurrentDoc = NULL;
			context.m_pNewDocTemplate = NULL;
			context.m_pLastView = (CView*) g_wndSplitterY->GetPane(0,0);
			
			// Set up the new pane then create it...
			g_wndSplitterY->DeleteView(0,0);
			g_wndSplitterY->CreateView(0,0, RUNTIME_CLASS(CRegSnap),
				CSize (0,220), &context);
			g_wndSplitterY->RecalcLayout();
				
			// Show the new frame in all its glory...
			pRegSnap = (CRegSnap*)g_wndSplitterY->GetPane(0,0);
			pRegSnap ->GetParentFrame()->RecalcLayout();
			pRegSnap ->OnInitialUpdate();
			g_wndSplitterY->SetActivePane(0,0);

			// Set up the second pane and create it...
			context.m_pNewViewClass = RUNTIME_CLASS (CHtmlPane);
			context.m_pLastView = (CView*) g_wndSplitterY->GetPane(1,0);
			g_wndSplitterY->DeleteView(1,0);
			g_wndSplitterY->CreateView(1,0, RUNTIME_CLASS (CHtmlPane),
				CSize (0,0), &context);
			g_wndSplitterY->RecalcLayout();
	
			// Show the new pane in all its glory...
			pHtmlView = (CHtmlPane*)g_wndSplitterY->GetPane(1,0);
			pHtmlView->GetParentFrame()->RecalcLayout();
			pHtmlView->OnInitialUpdate();
			g_wndSplitterY->SetActivePane(1,0);

			if(!pRegSnap->AddItems()) {
				MessageBox(
					"Could not complete operation.\nReturning to RegKit Home.",
					"Error executing RegSnap", MB_ICONERROR | MB_OK);
				RunMiniApp(ILI_HOME);
			}
			break;

		case ILI_FILESNAP:
			// Fill our context structure...
			context.m_pNewViewClass = RUNTIME_CLASS(CFileSnap);
			context.m_pCurrentFrame = g_wndFrame;
			context.m_pCurrentDoc = NULL;
			context.m_pNewDocTemplate = NULL;
			context.m_pLastView = (CView*) g_wndSplitterY->GetPane(0,0);
			
			// Set up the new pane then create it...
			g_wndSplitterY->DeleteView(0,0);
			g_wndSplitterY->CreateView(0,0, RUNTIME_CLASS(CFileSnap),
				CSize (0,220), &context);
			g_wndSplitterY->RecalcLayout();
				
			// Show the new frame in all its glory...
			pFileSnap = (CFileSnap*)g_wndSplitterY->GetPane(0,0);
			pFileSnap ->GetParentFrame()->RecalcLayout();
			pFileSnap ->OnInitialUpdate();
			g_wndSplitterY->SetActivePane(0,0);

				// Set up the second pane and create it...
			context.m_pNewViewClass = RUNTIME_CLASS (CHtmlPane);
			context.m_pLastView = (CView*) g_wndSplitterY->GetPane(1,0);
			g_wndSplitterY->DeleteView(1,0);
			g_wndSplitterY->CreateView(1,0, RUNTIME_CLASS (CHtmlPane),
				CSize (0,0), &context);
			g_wndSplitterY->RecalcLayout();
	
			// Show the new pane in all its glory...
			pHtmlView = (CHtmlPane*)g_wndSplitterY->GetPane(1,0);
			pHtmlView->GetParentFrame()->RecalcLayout();
			pHtmlView->OnInitialUpdate();
			g_wndSplitterY->SetActivePane(1,0);

			if(!pFileSnap->AddItems()) {
				MessageBox(
					"Could not complete operation.\nReturning to RegKit Home.",
					"Error executing FileSnap", MB_ICONERROR | MB_OK);
				RunMiniApp(ILI_HOME);
			}
			break;
		case ILI_REGDOC:
			// Fill our context structure...
			context.m_pNewViewClass = RUNTIME_CLASS(CRegDocList);
			context.m_pCurrentFrame = g_wndFrame;
			context.m_pCurrentDoc = NULL;
			context.m_pNewDocTemplate = NULL;
			context.m_pLastView = (CView*) g_wndSplitterY->GetPane(0,0);
			
			// Set up the new pane then create it...
			g_wndSplitterY->DeleteView(0,0);
			g_wndSplitterY->CreateView(0,0, RUNTIME_CLASS(CRegDocList),
				CSize (0,220), &context);
			g_wndSplitterY->RecalcLayout();
				
			// Show the new frame in all its glory...
			pRegDocList = (CRegDocList*)g_wndSplitterY->GetPane(0,0);
			pRegDocList->GetParentFrame()->RecalcLayout();
			pRegDocList->OnInitialUpdate();
			g_wndSplitterY->SetActivePane(0,0);

			// Set up the second pane and create it...
			context.m_pNewViewClass = RUNTIME_CLASS (CHtmlPane);
			context.m_pLastView = (CView*) g_wndSplitterY->GetPane(1,0);
			g_wndSplitterY->DeleteView(1,0);
			g_wndSplitterY->CreateView(1,0, RUNTIME_CLASS (CHtmlPane),
				CSize (0,0), &context);
			g_wndSplitterY->RecalcLayout();
	
			// Show the new pane in all its glory...
			pHtmlView = (CHtmlPane*)g_wndSplitterY->GetPane(1,0);
			pHtmlView->GetParentFrame()->RecalcLayout();
			pHtmlView->OnInitialUpdate();
			g_wndSplitterY->SetActivePane(1,0);


			pRegDocList->AddItems();
			
			break;
		case ILI_DLLDOC:
			// Fill our context structure...
			context.m_pNewViewClass = RUNTIME_CLASS(CDllList);
			context.m_pCurrentFrame = g_wndFrame;
			context.m_pCurrentDoc = NULL;
			context.m_pNewDocTemplate = NULL;
			context.m_pLastView = (CView*) g_wndSplitterY->GetPane(0,0);
			
			// Set up the new pane then create it...
			g_wndSplitterY->DeleteView(0,0);
			g_wndSplitterY->CreateView(0,0, RUNTIME_CLASS(CDllList), CSize (0,220), &context);
			g_wndSplitterY->RecalcLayout();
				
			// Show the new frame in all its glory...
			pDllList = (CDllList*)g_wndSplitterY->GetPane(0,0);
			pDllList->GetParentFrame()->RecalcLayout();
			pDllList->OnInitialUpdate();
			g_wndSplitterY->SetActivePane(0,0);

			// Set up the second pane and create it...
			context.m_pNewViewClass = RUNTIME_CLASS (CHtmlPane);
			context.m_pLastView = (CView*) g_wndSplitterY->GetPane(1,0);
			g_wndSplitterY->DeleteView(1,0);
			g_wndSplitterY->CreateView(1,0, RUNTIME_CLASS (CHtmlPane),
				CSize (0,0), &context);
			g_wndSplitterY->RecalcLayout();
	
			// Show the new pane in all its glory...
			pHtmlView = (CHtmlPane*)g_wndSplitterY->GetPane(1,0);
			pHtmlView->GetParentFrame()->RecalcLayout();
			pHtmlView->OnInitialUpdate();
			g_wndSplitterY->SetActivePane(1,0);

			if(!pDllList->AddItems()) {
				MessageBox("Bugger");
			}

			break;
		case ILI_SUMMARY:
			// Fill our context structure...
			context.m_pNewViewClass = RUNTIME_CLASS(CHtmlPane);
			context.m_pCurrentFrame = g_wndFrame;
			context.m_pCurrentDoc = NULL;
			context.m_pNewDocTemplate = NULL;
			context.m_pLastView = (CView*) g_wndSplitterY->GetPane(0,0);
			
			// Set up the new pane then create it...
			g_wndSplitterY->DeleteView(0,0);
			g_wndSplitterY->CreateView(0,0, RUNTIME_CLASS(CHtmlLogo),
				CSize (0,220), &context);
			g_wndSplitterY->RecalcLayout();
				
			// Show the new frame in all its glory...
			pHtmlLogo = (CHtmlLogo*)g_wndSplitterY->GetPane(0,0);
			pHtmlLogo->GetParentFrame()->RecalcLayout();
			pHtmlLogo->OnInitialUpdate();
			g_wndSplitterY->SetActivePane(0,0);

			// Set up the second pane and create it...
			context.m_pNewViewClass = RUNTIME_CLASS (CSummary);
			context.m_pLastView = (CView*) g_wndSplitterY->GetPane(1,0);
			g_wndSplitterY->DeleteView(1,0);
			g_wndSplitterY->CreateView(1,0, RUNTIME_CLASS (CSummary),
				CSize (0,0), &context);
			g_wndSplitterY->RecalcLayout();
	
			// Show the new pane in all its glory...
			pSummary= (CSummary*)g_wndSplitterY->GetPane(1,0);
			pSummary->GetParentFrame()->RecalcLayout();
			pSummary->OnInitialUpdate();
			g_wndSplitterY->SetActivePane(1,0);

			pSummary->DoSummary();
			break;
	}
}


