#if !defined(AFX_SUMMARYDLG_H__F30A8DE0_5AD7_11D3_8DF8_A66F03C06704__INCLUDED_)
#define AFX_SUMMARYDLG_H__F30A8DE0_5AD7_11D3_8DF8_A66F03C06704__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SummaryDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSummaryDlg dialog

class CSummaryDlg : public CDialog
{
// Construction
public:
	CSummaryDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSummaryDlg)
	enum { IDD = IDD_SUMMARY };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSummaryDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetProgress(int nProg);
	void SetStatic(LPCTSTR pszText);
	void SetStaticTask(LPCTSTR pszTask);
	void SetTotalProg(int nProg);
	void SetStaticTotal(LPCTSTR pszTotal);
protected:
	
	CStatic m_wndStatic;
	CStatic m_wndStaticTotal;
	CStatic m_wndStaticTask;
	CProgressCtrl m_wndTotalProg;
	CProgressCtrl m_wndProg;

	// Generated message map functions
	//{{AFX_MSG(CSummaryDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SUMMARYDLG_H__F30A8DE0_5AD7_11D3_8DF8_A66F03C06704__INCLUDED_)
