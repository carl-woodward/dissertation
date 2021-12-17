#if !defined(AFX_DRIVELIST_H__93C69E82_5174_11D3_8DF8_B58DA6EE3D24__INCLUDED_)
#define AFX_DRIVELIST_H__93C69E82_5174_11D3_8DF8_B58DA6EE3D24__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DriveList.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDriveList view

class CDriveList : public CListView
{
protected:
	CDriveList();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CDriveList)

// Attributes
public:

// Operations
public:
	int Refresh (LPCTSTR pszPath);
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDriveList)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CDriveList();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	CImageList m_ilLarge;
	CImageList m_ilSmall;
	
	CString m_strPath;
	BOOL AddItem (int nIndex, WIN32_FIND_DATA* pfd);
	//{{AFX_MSG(CDriveList)
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()	
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DRIVELIST_H__93C69E82_5174_11D3_8DF8_B58DA6EE3D24__INCLUDED_)
