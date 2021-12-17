#if !defined(AFX_LISTPANE_H__ABC1BB0C_50D6_11D3_8DF8_92BA665F8068__INCLUDED_)
#define AFX_LISTPANE_H__ABC1BB0C_50D6_11D3_8DF8_92BA665F8068__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ListPane.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CListPane view

class CListPane : public CListView
{
protected:
	CListPane();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CListPane)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CListPane)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CListPane();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	void RunMiniApp(int nIndex);
	CImageList m_ilLarge;
	CImageList m_ilSmall;
	//{{AFX_MSG(CListPane)
	afx_msg void OnDblclk(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LISTPANE_H__ABC1BB0C_50D6_11D3_8DF8_92BA665F8068__INCLUDED_)
