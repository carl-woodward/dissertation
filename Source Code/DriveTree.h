#if !defined(AFX_DRIVETREE_H__93C69E81_5174_11D3_8DF8_B58DA6EE3D24__INCLUDED_)
#define AFX_DRIVETREE_H__93C69E81_5174_11D3_8DF8_B58DA6EE3D24__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DriveTree.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDriveTree view

class CDriveTree : public CTreeView
{
protected:
	CDriveTree();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CDriveTree)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDriveTree)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CDriveTree();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	CImageList m_ilDrives;

	void AddDriveItem (LPCTSTR pszDrive);
    int AddDirectories (HTREEITEM hItem, LPCTSTR pszPath);
    void DeleteAllChildren (HTREEITEM hItem);
    void DeleteFirstChild (HTREEITEM hItem);
    CString GetPathFromItem (HTREEITEM hItem);
    BOOL SetButtonState (HTREEITEM hItem, LPCTSTR pszPath);
	void AddDrives ();
 	
	//{{AFX_MSG(CDriveTree)
	afx_msg void OnItemexpanding(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DRIVETREE_H__93C69E81_5174_11D3_8DF8_B58DA6EE3D24__INCLUDED_)
