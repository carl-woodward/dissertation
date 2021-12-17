// RegKitView.h : interface of the CRegKitView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_REGKITVIEW_H__ABC1BB03_50D6_11D3_8DF8_92BA665F8068__INCLUDED_)
#define AFX_REGKITVIEW_H__ABC1BB03_50D6_11D3_8DF8_92BA665F8068__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RegKitDoc.h"

class CRegKitView : public CListView
{
protected: // create from serialization only
	CRegKitView();
	DECLARE_DYNCREATE(CRegKitView)

// Attributes
public:
	CRegKitDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRegKitView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CRegKitView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CRegKitView)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in RegKitView.cpp
inline CRegKitDoc* CRegKitView::GetDocument()
   { return (CRegKitDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REGKITVIEW_H__ABC1BB03_50D6_11D3_8DF8_92BA665F8068__INCLUDED_)
