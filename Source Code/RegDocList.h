#if !defined(AFX_REGDOCLIST_H__50CD6F41_5558_11D3_8DF8_D8B700C4AB22__INCLUDED_)
#define AFX_REGDOCLIST_H__50CD6F41_5558_11D3_8DF8_D8B700C4AB22__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RegDocList.h : header file
//

#include "ProgDlg.h"
#include <vector>

using namespace std;

/////////////////////////////////////////////////////////////////////////////
// CRegDocList view

class CRegDocList : public CListView
{
protected:
	CRegDocList();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CRegDocList)

// Attributes
public:

// Operations
public:
	BOOL AddItems();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRegDocList)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CRegDocList();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	int m_nInvXt;
	int m_nCount;
	int m_nInvClsid;
	int m_nInvPaths;
	void CheckFileExt();
	void CheckClsid(LPCTSTR pszClsid);
	void GetClsid();
	CImageList m_ilSmall;
	double m_nRegPos;
	CProgDlg *m_pDlg;
	double m_nRegCoefficient;
	vector <CString> m_vClsid;

	void ScanRegNode(HKEY hKey, LPTSTR pszRegPath);
	BOOL CheckForFileName (LPTSTR pszValue, UINT cbOut, LPTSTR szOut);
	void CheckFileExists (LPTSTR pszFilePath);

	//{{AFX_MSG(CRegDocList)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REGDOCLIST_H__50CD6F41_5558_11D3_8DF8_D8B700C4AB22__INCLUDED_)
