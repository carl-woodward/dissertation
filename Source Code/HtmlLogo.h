#if !defined(AFX_HTMLLOGO_H__4E56D8E0_5306_11D3_8DF8_C149E48D8D0F__INCLUDED_)
#define AFX_HTMLLOGO_H__4E56D8E0_5306_11D3_8DF8_C149E48D8D0F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HtmlLogo.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CHtmlLogo html view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif
#include <afxhtml.h>

class CHtmlLogo : public CHtmlView
{
protected:
	CHtmlLogo();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CHtmlLogo)

// html Data
public:
	//{{AFX_DATA(CHtmlLogo)
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHtmlLogo)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CHtmlLogo();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CHtmlLogo)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HTMLLOGO_H__4E56D8E0_5306_11D3_8DF8_C149E48D8D0F__INCLUDED_)
