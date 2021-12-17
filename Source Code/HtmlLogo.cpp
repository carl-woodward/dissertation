// HtmlLogo.cpp : implementation file
//

#include "stdafx.h"
#include "regkit.h"
#include "HtmlLogo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern TCHAR g_szHomeDir[MAX_PATH];

/////////////////////////////////////////////////////////////////////////////
// CHtmlLogo

IMPLEMENT_DYNCREATE(CHtmlLogo, CHtmlView)

CHtmlLogo::CHtmlLogo()
{
	//{{AFX_DATA_INIT(CHtmlLogo)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CHtmlLogo::~CHtmlLogo()
{
}

void CHtmlLogo::DoDataExchange(CDataExchange* pDX)
{
	CHtmlView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHtmlLogo)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CHtmlLogo, CHtmlView)
	//{{AFX_MSG_MAP(CHtmlLogo)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHtmlLogo diagnostics

#ifdef _DEBUG
void CHtmlLogo::AssertValid() const
{
	CHtmlView::AssertValid();
}

void CHtmlLogo::Dump(CDumpContext& dc) const
{
	CHtmlView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CHtmlLogo message handlers

void CHtmlLogo::OnInitialUpdate() 
{
	//TODO: This code navigates to a popular spot on the web.
	//Change the code to go where you'd like.
	char szLogo[256];
	::SetCurrentDirectory(g_szHomeDir);
	strcpy (szLogo, "file://");
	strcat (szLogo, g_szHomeDir);
	strcat (szLogo, "/Logo.htm");
	Navigate2(_T(szLogo),NULL,NULL);
}
