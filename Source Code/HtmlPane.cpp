// HtmlPane.cpp : implementation file
//

#include "stdafx.h"
#include "RegKit.h"
#include "HtmlPane.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern TCHAR g_szHomeDir [MAX_PATH];

/////////////////////////////////////////////////////////////////////////////
// CHtmlPane

IMPLEMENT_DYNCREATE(CHtmlPane, CHtmlView)

CHtmlPane::CHtmlPane()
{
	//{{AFX_DATA_INIT(CHtmlPane)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CHtmlPane::~CHtmlPane()
{
}

void CHtmlPane::DoDataExchange(CDataExchange* pDX)
{
	CHtmlView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHtmlPane)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CHtmlPane, CHtmlView)
	//{{AFX_MSG_MAP(CHtmlPane)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHtmlPane diagnostics

#ifdef _DEBUG
void CHtmlPane::AssertValid() const
{
	CHtmlView::AssertValid();
}

void CHtmlPane::Dump(CDumpContext& dc) const
{
	CHtmlView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CHtmlPane message handlers

void CHtmlPane::OnInitialUpdate() 
{
	//TODO: This code navigates to a popular spot on the web.
	//Change the code to go where you'd like.
	CString strURL;
	strURL = "file://";
	strURL += g_szHomeDir;
	strURL += "\\";
	strURL += "UsingRegKit.htm";
	
	Navigate2(strURL,NULL,NULL);
			
}
