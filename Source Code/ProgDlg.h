#if !defined(AFX_PROGDLG_H__B698A082_5402_11D3_8DF8_B8EF06693A02__INCLUDED_)
#define AFX_PROGDLG_H__B698A082_5402_11D3_8DF8_B8EF06693A02__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ProgDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CProgDlg dialog

class CProgDlg : public CDialog
{
// Construction
public:
	void SetProgress(int nProg);
	void SetStatic(LPCTSTR pszText);
		void SetStaticTask(LPCTSTR pszTask);
	CProgDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CProgDlg)
	enum { IDD = IDD_PROG_DLG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProgDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CStatic m_wndStaticTask;
	CStatic m_wndStatic;
	CProgressCtrl m_wndProg;

	// Generated message map functions
	//{{AFX_MSG(CProgDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROGDLG_H__B698A082_5402_11D3_8DF8_B8EF06693A02__INCLUDED_)
