#if !defined(AFX_REGTREE_H__CAD86A81_515F_11D3_8DF8_B58DA6EE3D24__INCLUDED_)
#define AFX_REGTREE_H__CAD86A81_515F_11D3_8DF8_B58DA6EE3D24__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RegTree.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRegTree view

class CRegTree : public CTreeView
{
protected:
	CRegTree();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CRegTree)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRegTree)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CRegTree();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
public:
	HKEY GetHkey(LPCTSTR pszPath);
protected:
	void DeleteAllChildren(HTREEITEM hTree);
	void DeleteFirstItem(HTREEITEM hItem);
	CString GetPathFromItem (HTREEITEM hItem);
	void AddButton (HTREEITEM hItem, HKEY hKey);
	int AddSubkeys(HTREEITEM hItem, LPCTSTR pszPath);
	CImageList m_ilRegImages;
	//{{AFX_MSG(CRegTree)
	afx_msg void OnItemexpanding(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REGTREE_H__CAD86A81_515F_11D3_8DF8_B58DA6EE3D24__INCLUDED_)
