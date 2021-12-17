// RegSnap.cpp : implementation file
//

#include "stdafx.h"
#include "regkit.h"
#include "RegSnap.h"

#include <algorithm>
#include "HtmlPane.h"

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern TCHAR g_szHomeDir[MAX_PATH];
extern CSplitterWnd *g_wndSplitterY;

#define ILI_ADDED 0
#define ILI_DELETED 1

/////////////////////////////////////////////////////////////////////////////
// CRegSnap

IMPLEMENT_DYNCREATE(CRegSnap, CListView)

CRegSnap::CRegSnap()
{
	m_pDlg = new CProgDlg;
	m_pDlg->Create(IDD_PROG_DLG);
	m_pDlg->ShowWindow(SW_SHOW);
	m_nRegPos = 0;
	m_nRegCoefficient = (float) 100/60000;
}

CRegSnap::~CRegSnap()
{
}


BEGIN_MESSAGE_MAP(CRegSnap, CListView)
	//{{AFX_MSG_MAP(CRegSnap)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRegSnap drawing

void CRegSnap::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CRegSnap diagnostics

#ifdef _DEBUG
void CRegSnap::AssertValid() const
{
	CListView::AssertValid();
}

void CRegSnap::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CRegSnap message handlers

BOOL CRegSnap::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	if (!CListView::PreCreateWindow(cs))
		return FALSE;
	cs.style &=~LVS_TYPEMASK;
	cs.style |= LVS_REPORT;
	return TRUE;
}

void CRegSnap::OnInitialUpdate() 
{
	// Call the base class constructor
	CListView::OnInitialUpdate();

	int nWidth =0;
	RECT rect;
	CBitmap bit;
	bit.LoadBitmap(IDB_DELETED);

	// Calculate the size of the forst column
	GetClientRect(&rect);
	nWidth = ((rect.right/4)*3);
	
	// Create and set the image list
	m_ilSmall.Create(IDB_ADDED, 16, 1, RGB(255,0,255));
	m_ilSmall.Add(&bit, RGB (255,0,255));
	GetListCtrl().SetImageList(&m_ilSmall, LVSIL_SMALL);
	GetListCtrl().InsertColumn(0, "Registry Entry", LVCFMT_LEFT, nWidth);
	
	// Create the second column
	nWidth = (rect.right/4);
	GetListCtrl().InsertColumn(1, "State", LVCFMT_LEFT, nWidth);
	
}


void CRegSnap::ScanRegNode(HKEY hKey, LPTSTR pszRegPath)
{
	DWORD regEnumIndex = 0;

	// Enumerate all registry values in a given key...
	//
	while (TRUE) {

		// Hold and check values from RegEnumValues
		//
		TCHAR szValueName[ 320 ];
		DWORD cbValueNameSize = 320;
		TCHAR szValue [ 320 ];
		DWORD cbValueSize = 320;
		DWORD type;
		LONG result;
		
		// Hold file paths
		//
		TCHAR szFullPath [512];
		
		// Get the values in the current key
		//
		result = RegEnumValue (hKey, regEnumIndex++,
			szValueName, &cbValueNameSize,
			0, &type, (LPBYTE) szValue, &cbValueSize);

		// Bail out if there is a problem or we have finished...
		//
		if (result == ERROR_NO_MORE_ITEMS) {
			break;
		}

		// Build the registry path responsible for our values...
		//
		wsprintf (szFullPath, TEXT ("%s¬%s¬%s\n\0"), pszRegPath,szValueName, szValue);
		
		// Advance the progress bar...
		m_nRegPos++;
		m_pDlg->SetProgress((int) m_nRegPos * m_nRegCoefficient);
	}

	regEnumIndex = 0;

	// Find child keys (subkeys) and use recursion to enumerate all the
	// values until there are no more subkeys left
	//
	while (TRUE) {

		// Generate subkey with data returned from RegOpenValueEx
		//
		TCHAR szSubkey [ 320 ];
		DWORD cbSubkeySize = 320;
		HKEY hSubkey;
		LONG result;
		TCHAR szRegPathSubkey [ 320 ];

		// Enumerate the subkeys
		//
		result = RegEnumKeyEx (hKey, regEnumIndex++, szSubkey, &cbSubkeySize,
			0,0,0,0);

		// Bail out if there are no more subkeys
		//
		if (result == ERROR_NO_MORE_ITEMS)
			break;

		// Ignore trivial errors
		//
		if (result != 0)
			continue;

		// Open the child key, bail out if it cant be opened
		//
		result = RegOpenKeyEx (hKey, szSubkey, 0, KEY_READ, &hSubkey);

		if (result != ERROR_SUCCESS)
			continue;

		// Build the subkey path
		//
		strcpy (szRegPathSubkey, pszRegPath);
		strcat (szRegPathSubkey, "\\");
		strcat (szRegPathSubkey, szSubkey);
		strcat (szRegPathSubkey, "\n");
		
		CString str = szRegPathSubkey;
		m_vReg.push_back(str);

		szRegPathSubkey[(lstrlen(szRegPathSubkey)-1)] = '\0';
				
		m_nRegPos++;
		m_pDlg->SetProgress((int) m_nRegPos * m_nRegCoefficient);

		// Use recursion to enumerate all the values of the subkey
		//
		ScanRegNode (hSubkey, szRegPathSubkey);

		// Close and stop scanning the node
		//
		RegCloseKey(hSubkey);
	}
}

BOOL CRegSnap::WriteList()
{
	LPTSTR pszFile;
	::SetCurrentDirectory(g_szHomeDir);
	vector<CString>::iterator p;
	CTime t = CTime::GetCurrentTime();
	CString time = t.Format("%Y_%m_%d_%H_%M_");
	pszFile = time.GetBuffer(MAX_PATH);

	strcat (pszFile, "reg.djc");

	// Create a new file for writing and dump linkedList strings to it
	// This function must have CFileException error handling
	//
	try {
		CStdioFile file (_T(pszFile), CFile::modeWrite | CFile::modeCreate);
		p = m_vReg.begin();
		while (p != m_vReg.end()) {
			file.WriteString(*p);
			p++;
		}
		TRACE ("Files filename is %s\n", pszFile);
	}
	catch (CFileException* e) {
		e->ReportError();
		e->Delete();
		return FALSE;
	}
	return TRUE;
}

BOOL CRegSnap::AddItems()
{
	// Scan HKEY_LOCAL_USERS AND HKEY_CURRENT_USERS	
	m_pDlg->SetStaticTask("Taking snapshot of current registry...");
	m_pDlg->SetProgress(0);
	m_pDlg->SetStatic("Scanning HKEY_LOCAL_MACHINE...");
	ScanRegNode(HKEY_LOCAL_MACHINE, "HKEY_LOCAL_MACHINE");
	m_pDlg->SetStatic("Scanning HKEY_CURRENT_USER...");
	ScanRegNode(HKEY_CURRENT_USER, "HKEY_CURRENT_USER");
	m_pDlg->SetStaticTask("Preparing file snapshot...");
	m_pDlg->SetStatic("Sorting registry vector...");
	
	// SORT IT! (Binary searches...!)
	sort(m_vReg.begin(), m_vReg.end());
	m_pDlg->SetStatic("Writing registry vector...");
	
	// Write out the snapshot...
	if (!WriteList())
		return FALSE;
	m_pDlg->SetStaticTask("Comparing with previous registry snapshot...");
	m_pDlg->SetStatic("Requesting file for comparison...");
	
	// Load a new snapshot and compare them
	if (!OpenFile())
		return FALSE;
	if (!Compare())
		return FALSE;	
	return TRUE;
}

BOOL CRegSnap::OpenFile()
{
	BOOL bOKFileName = FALSE;
	
	// Open the common dialog	
	TCHAR szFilters[] =
		(_T("DjcSoft Regkit files (*.djc)|*.djc|All files(*.*)|*.*|"));
	CFileDialog dlg (TRUE, _T("djc"),_T ("*.djc"),
		OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, szFilters);
	
	// Get a new file and load it into the vector if it is a registry snapshot
	// Keep getting files until the correct type is asked for
	//
	// If the user presses cancel, return to regkit home
	//
	while (!bOKFileName) {
		if (dlg.DoModal() == IDOK) {
			CString strRead;
			m_strFileName = dlg.GetFileName();
			if (NULL != strstr(m_strFileName, "reg")) {
				bOKFileName = TRUE;
				m_pDlg->SetStatic("Loading file...");
				try {
					CStdioFile file (m_strFileName, CFile::modeRead);
					while(file.ReadString(strRead)) {
						m_vPrev.push_back(strRead);
						TRACE("%s\n", strRead);
					}
				}
				catch (CFileException *e) {
					e->ReportError();
					e->Delete();
					return FALSE;
				}				
			}
			else {
				CString strMessage;
				strMessage.Format
					("Error opening %s\n\nPlease select a file with \"reg\" in its name\n",
					m_strFileName);
				MessageBox(strMessage, "Error", MB_ICONERROR | MB_OK);
			}		
		}
		else {
			KillDlg();
			return FALSE;
		}
	}
	return TRUE;
}


BOOL CRegSnap::Compare()
{
	int nIndex =0;
	int nAdded =0;
	int nDeleted =0;
	char szNo[20];
	CString strFile;
	CString strPrev;
	CString strURL;
	CString strHtml;
	vector<CString>::iterator reg;
	vector<CString>::iterator prev;
	double nCount = m_vReg.size();
	double nCoeff = (float) ((double)100/(double)nCount);
	
	nCount =0;
	
	// Set the dlg controls
	m_pDlg->SetProgress(0);
	m_pDlg->SetStatic("Searching for added registry keys...");

	// Iterate through each entry in the fileVector
	reg = m_vReg.begin();
	while (reg != m_vReg.end()) {
		
		strFile = *reg;
		strFile.Remove('\n');
						
		if (!binary_search(m_vPrev.begin(), m_vPrev.end(), strFile)) {
			GetListCtrl().InsertItem(nIndex++, strFile, ILI_ADDED);
			GetListCtrl().SetItemText(--nIndex, 1, "Added");
			nIndex++;
		}
		reg++;
		nCount++;
		
		// Update the progress bar...
		m_pDlg->SetProgress(nCount * nCoeff);
	}
	nAdded = nIndex;
	nCount = 0;

	// Set the dlg controls
	m_pDlg->SetProgress(0);
	m_pDlg->SetStatic("Searching for deleted regsitry keys...");

	// Iterate through each entry in the fileVector
	prev = m_vPrev.begin();
	while (prev != m_vPrev.end()) {
		
		strPrev = *prev;
		strPrev += '\n';
		// Use a binary search to see if the file has been deleted or not..
		
		if (!binary_search(m_vReg.begin(), m_vReg.end(), strPrev)) {
			GetListCtrl().InsertItem(nIndex++, *prev, ILI_DELETED);
			GetListCtrl().SetItemText(--nIndex, 1, "Deleted");
			nIndex++;
		}
		prev++;
		nCount++;
		
		// Update the progress bar...
		m_pDlg->SetProgress(nCount * nCoeff);
	}
	nDeleted = (nIndex - nAdded);
	
	// Get a pointer to the HtmlView and reset the current directory (drive
	// enumeration changes the current dir)
	CHtmlPane *pView = (CHtmlPane*) g_wndSplitterY->GetPane(1,0);
	::SetCurrentDirectory(g_szHomeDir);
	
	CTime t = CTime::GetCurrentTime();
	CString time = t.Format("Time: %H:%M %A, %B %d, %Y");

	// Build a very small HTML file to show our results...
	strHtml = "<HTML><HEAD>";
	strHtml += "<META HTTP-EQUIV=\"Content-Type\" CONTENT=\"text/html; charset=windows-1252\">";
	strHtml += "<META NAME=\"Generator\" CONTENT=\"Microsoft Word 97\">";
	strHtml += "<TITLE>Summary</TITLE></HEAD>";
	strHtml += "<BODY LINK=\"#0000ff\" VLINK=\"#800080\" BACKGROUND=\"DLL_Summary.jpg\">";
	strHtml += "<U><FONT FACE=\"Arial\" SIZE=6><P>Registry Snapshot Summary</P>";
	strHtml += "</U></FONT><FONT FACE=\"Arial\" SIZE=4>";
	strHtml += "<P>";
	strHtml += time;
	strHtml += "</P>";
	strHtml += "<P>Comparing against: ";
	strHtml += m_strFileName;
	strHtml += "</P>";
	strHtml += "<P>Number of registry entries added = ";
	wsprintf(szNo, " %d ", (int) nAdded);
	strHtml += szNo;
	strHtml += "</P><P>Number of registry entries deleted = ";
	wsprintf (szNo, " %d ", (int) nDeleted);
	strHtml += szNo;
	strHtml += "</P><P> Number of registry entries = ";
	wsprintf (szNo, " %d ", (int) m_vReg.size());
	strHtml += szNo;
	strHtml += "</P></FONT></BODY></HTML>";
	
	// Use Exception handling to write the HTML file
	try {
		CFile file (_T("DLL_Summary.htm"), CFile::modeWrite | CFile::modeCreate);
		file.Write(strHtml, strHtml.GetLength());
	}
	catch (CFileException *e) {
		e->ReportError();
		e->Delete();
	}	
	
	// Build a path to the file
	strURL = "file://";
	strURL += g_szHomeDir;
	strURL += '\\';
	strURL += "DLL_Summary.htm";
	
	// Show the file we just built in the Html View
	pView->Navigate2(strURL, NULL,NULL);
	KillDlg();
	return TRUE;

	
}

void CRegSnap::KillDlg()
{
	// Destroy the dialog and release allocated memory
	if (m_pDlg != NULL) {
		m_pDlg->DestroyWindow();
		delete m_pDlg;
	}
}
