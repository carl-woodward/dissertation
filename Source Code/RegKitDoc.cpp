// RegKitDoc.cpp : implementation of the CRegKitDoc class
//

#include "stdafx.h"
#include "RegKit.h"

#include "RegKitDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRegKitDoc

IMPLEMENT_DYNCREATE(CRegKitDoc, CDocument)

BEGIN_MESSAGE_MAP(CRegKitDoc, CDocument)
	//{{AFX_MSG_MAP(CRegKitDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRegKitDoc construction/destruction

CRegKitDoc::CRegKitDoc()
{
	// TODO: add one-time construction code here

}

CRegKitDoc::~CRegKitDoc()
{
}

BOOL CRegKitDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CRegKitDoc serialization

void CRegKitDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CRegKitDoc diagnostics

#ifdef _DEBUG
void CRegKitDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CRegKitDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CRegKitDoc commands
