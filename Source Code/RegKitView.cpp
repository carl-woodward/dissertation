// RegKitView.cpp : implementation of the CRegKitView class
//

#include "stdafx.h"
#include "RegKit.h"

#include "RegKitDoc.h"
#include "RegKitView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRegKitView

IMPLEMENT_DYNCREATE(CRegKitView, CListView)

BEGIN_MESSAGE_MAP(CRegKitView, CListView)
	//{{AFX_MSG_MAP(CRegKitView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CListView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CListView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CListView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRegKitView construction/destruction

CRegKitView::CRegKitView()
{
	// TODO: add construction code here

}

CRegKitView::~CRegKitView()
{
}

BOOL CRegKitView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CListView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CRegKitView drawing

void CRegKitView::OnDraw(CDC* pDC)
{
	CRegKitDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

void CRegKitView::OnInitialUpdate()
{
	CListView::OnInitialUpdate();

	// TODO: add code to initialise my list view...
}

/////////////////////////////////////////////////////////////////////////////
// CRegKitView printing

BOOL CRegKitView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CRegKitView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CRegKitView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CRegKitView diagnostics

#ifdef _DEBUG
void CRegKitView::AssertValid() const
{
	CListView::AssertValid();
}

void CRegKitView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}

CRegKitDoc* CRegKitView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CRegKitDoc)));
	return (CRegKitDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CRegKitView message handlers


