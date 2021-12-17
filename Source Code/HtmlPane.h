#if !defined(AFX_HTMLPANE_H__ABC1BB0D_50D6_11D3_8DF8_92BA665F8068__INCLUDED_)
#define AFX_HTMLPANE_H__ABC1BB0D_50D6_11D3_8DF8_92BA665F8068__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HtmlPane.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CHtmlPane html view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif
#include <afxhtml.h>

class CHtmlPane : public CHtmlView
{
protected:
	CHtmlPane();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CHtmlPane)

// html Data
public:
	//{{AFX_DATA(CHtmlPane)
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHtmlPane)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CHtmlPane();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CHtmlPane)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HTMLPANE_H__ABC1BB0D_50D6_11D3_8DF8_92BA665F8068__INCLUDED_)
