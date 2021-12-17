#if !defined(AFX_FILESNAP_H__C1C14D40_5489_11D3_8DF8_A088A5E44725__INCLUDED_)
#define AFX_FILESNAP_H__C1C14D40_5489_11D3_8DF8_A088A5E44725__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FileSnap.h : header file
//

#include "ProgDlg.h"
#include <vector>

using namespace std;

/////////////////////////////////////////////////////////////////////////////
// CFileSnap view

class CFileSnap : public CListView
{
protected:
	CFileSnap();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CFileSnap)

// Attributes
public:

// Operations
public:
	void KillDlg();
	BOOL AddItems();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFileSnap)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CFileSnap();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	CString m_strFileName;
	bool Compare();
	BOOL OpenFile();
	char m_szPath[MAX_PATH];
	CProgDlg *m_pDlg;
	CImageList m_ilSmall;
	vector <CString> m_fileVector;
	vector <CString> m_prevVector;

	double m_nBytesRead;
	double m_nFreeBytes;
	double m_nTotalBytes;
	double m_nBytesCoefficient;

	bool CheckRM (LPCTSTR lpszDriveRoot);
	void EnumDrives();
	void Enumerate(void);
	void GetDiskInfo(LPCTSTR lpszDriveRoot);
	bool WriteList();

	//{{AFX_MSG(CFileSnap)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FILESNAP_H__C1C14D40_5489_11D3_8DF8_A088A5E44725__INCLUDED_)
