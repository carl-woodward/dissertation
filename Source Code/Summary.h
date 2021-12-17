#if !defined(AFX_SUMMARY_H__1C2D7E00_58D8_11D3_8DF8_C2182F41DF15__INCLUDED_)
#define AFX_SUMMARY_H__1C2D7E00_58D8_11D3_8DF8_C2182F41DF15__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Summary.h : header file
//

#include <vector>
#include <algorithm>

#include "SummaryDlg.h"
#include  <functional>

using namespace std;

/////////////////////////////////////////////////////////////////////////////
// CSummary html view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif
#include <afxhtml.h>

class CSummary : public CHtmlView
{
protected:
	CSummary();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CSummary)

// html Data
public:
	//{{AFX_DATA(CSummary)
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSummary)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CSummary();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:
	bool DoSummary();

protected:
	CString m_strRegSnapName;
	CString m_strFileSnapName;
	int m_nProgress;
	int m_nInvXt;
	int m_nRegCount;
	int m_nInvClsid;
	int m_nInvPaths;
	
	CSummaryDlg *m_pDlg;
	vector<CString> m_vClsid;
	vector<CString> m_vNewReg;
	vector<CString> m_vOldReg;
	vector<CString> m_vRegDll;
	vector<CString> m_vFileDll;
	vector<CString> m_vNewFile;
	vector<CString> m_vOldFile;
		
	double m_nBytesRead;
	double m_nFreeBytes;
	double m_nTotalBytes;
	double m_nRegCoefficient;
	double m_nBytesCoefficient;

	int m_nDll;
	int m_nRegDll;
	int m_nRegPos;
	int m_nUnregDll;
	
	int m_nFilesAdded;
	int m_nFilesDeleted;
	int m_nFiles;

	int m_nRegAdded;
	int m_nRegDeleted;
	int m_nReg;

	void GetClsid();
	void GetDllInfo();
	void CheckFileExt();
	bool GetRegSnapInfo();
	bool GetFileSnapInfo();
	bool OpenFile(UINT unOp);
	bool WriteList(UINT unOp);
	void Enumerate(UINT unOp);
	void EnumDrives(UINT unOp);
	void CheckClsid(LPCTSTR pszClsid);
	bool CheckRM (LPCTSTR lpszDriveRoot);
	void GetDiskInfo(LPCTSTR lpszDriveRoot);
	void CheckFileExists (LPTSTR pszFilePath);
	void ScanRegNode (HKEY hKey, LPTSTR lpszRegPath, UINT unOp);
	bool CheckForFileName (LPTSTR pszValue, UINT cbOut, LPTSTR szOut);
	

	// Generated message map functions
	//{{AFX_MSG(CSummary)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SUMMARY_H__1C2D7E00_58D8_11D3_8DF8_C2182F41DF15__INCLUDED_)
