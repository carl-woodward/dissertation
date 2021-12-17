#if !defined(AFX_REGLIST_H__9A41DC60_5346_11D3_8DF8_C149E48D8D0F__INCLUDED_)
#define AFX_REGLIST_H__9A41DC60_5346_11D3_8DF8_C149E48D8D0F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RegList.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRegList view

class CRegList : public CListView
{
protected:
	CRegList();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CRegList)

// Attributes
public:

// Operations
public:
	void Refresh (LPCTSTR pszPath);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRegList)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CRegList();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	CImageList m_ilSmall;
	//{{AFX_MSG(CRegList)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REGLIST_H__9A41DC60_5346_11D3_8DF8_C149E48D8D0F__INCLUDED_)
