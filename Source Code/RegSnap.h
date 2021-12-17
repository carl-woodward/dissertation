#if !defined(AFX_REGSNAP_H__C1C14D41_5489_11D3_8DF8_A088A5E44725__INCLUDED_)
#define AFX_REGSNAP_H__C1C14D41_5489_11D3_8DF8_A088A5E44725__INCLUDED_

#include "ProgDlg.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RegSnap.h : header file
//

#include <vector>

using namespace std;

/////////////////////////////////////////////////////////////////////////////
// CRegSnap view

class CRegSnap : public CListView
{
protected:
	CRegSnap();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CRegSnap)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRegSnap)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CRegSnap();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
public:
	void KillDlg();
	BOOL AddItems();
	// Generated message map functions
protected:
	CString m_strFileName;
	BOOL Compare();
	BOOL OpenFile();
	BOOL WriteList();
	int m_nRegPos;
	CProgDlg *m_pDlg;
	CImageList m_ilSmall;
	double m_nRegCoefficient;
	vector<CString> m_vReg;
	vector<CString> m_vPrev;

	void ScanRegNode(HKEY hKey, LPTSTR pszRegPath);
	//{{AFX_MSG(CRegSnap)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REGSNAP_H__C1C14D41_5489_11D3_8DF8_A088A5E44725__INCLUDED_)
