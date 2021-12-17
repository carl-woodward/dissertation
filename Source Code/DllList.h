#if !defined(AFX_DLLLIST_H__3242DF42_53AC_11D3_8DF8_B9B3C3A33A25__INCLUDED_)
#define AFX_DLLLIST_H__3242DF42_53AC_11D3_8DF8_B9B3C3A33A25__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DllList.h : header file
//
#include <vector>
#include "Data.h"
#include "ProgDlg.h"	// Added by ClassView

using namespace std;

/////////////////////////////////////////////////////////////////////////////
// CDllList view

class CDllList : public CListView
{
protected:
	CDllList();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CDllList)

// Attributes
public:

// Operations
public:
	BOOL AddItems();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDllList)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CDllList();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	CProgDlg *m_pDlg;
	CImageList m_ilSmall;
	vector<CString> m_regVector;
	vector<CString> m_fileVector;
	int m_nRegPos;
	CString m_strHtml;

	double m_nRegCoefficient;
	double m_nBytesRead;
	double m_nFreeBytes;
	double m_nTotalBytes;
	double m_nBytesCoefficient;

	bool CheckRM (LPCTSTR lpszDriveRoot);
	void EnumDrives();
	void Enumerate(void);
	void GetDiskInfo(LPCTSTR lpszDriveRoot);
	void Compare();
	void ScanRegNode (HKEY hKey, LPTSTR lpszRegPath);
	
	//{{AFX_MSG(CDllList)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLLLIST_H__3242DF42_53AC_11D3_8DF8_B9B3C3A33A25__INCLUDED_)
