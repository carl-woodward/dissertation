// RegDocList.cpp : implementation file
//

#include "stdafx.h"
#include "regkit.h"
#include "RegDocList.h"

#include <algorithm>
#include "HtmlPane.h"

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CSplitterWnd *g_wndSplitterY;
extern TCHAR g_szHomeDir [ MAX_PATH ];

/////////////////////////////////////////////////////////////////////////////
// CRegDocList

IMPLEMENT_DYNCREATE(CRegDocList, CListView)

CRegDocList::CRegDocList()
{
	m_nRegCoefficient = (float) 100/60000;
	m_nRegPos =0;
	m_nInvPaths =0;
	m_nInvXt =0;
	m_nInvClsid =0;
	m_nCount =0;
	m_pDlg = new CProgDlg;
	m_pDlg->Create(IDD_PROG_DLG);
	m_pDlg->ShowWindow(SW_SHOW);
}

CRegDocList::~CRegDocList()
{
}


BEGIN_MESSAGE_MAP(CRegDocList, CListView)
	//{{AFX_MSG_MAP(CRegDocList)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRegDocList drawing

void CRegDocList::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CRegDocList diagnostics

#ifdef _DEBUG
void CRegDocList::AssertValid() const
{
	CListView::AssertValid();
}

void CRegDocList::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CRegDocList message handlers

void CRegDocList::OnInitialUpdate() 
{
	CListView::OnInitialUpdate();
	
	// Create the image list and columns in the list view
	m_ilSmall.Create(IDB_REGERROR, 16,1,RGB(255,0,255));
	GetListCtrl().SetImageList(&m_ilSmall, LVSIL_SMALL);

	GetListCtrl().InsertColumn(0, "Registry Key", LVCFMT_LEFT, 500);
	GetListCtrl().InsertColumn(1, "Problem", LVCFMT_LEFT, 200);	
}

BOOL CRegDocList::PreCreateWindow(CREATESTRUCT& cs) 
{
	// Call base class constructor and initialise the style of the view
	if (!CListView::PreCreateWindow(cs))
		return FALSE;
	cs.style &= ~LVS_TYPEMASK;
	cs.style |= LVS_REPORT;
	return TRUE;
}

void CRegDocList::ScanRegNode(HKEY hKey, LPTSTR pszRegPath)
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
		TCHAR szFileName [MAX_PATH];
		
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

		m_nRegPos++;
		m_pDlg->SetProgress((int) m_nRegPos * m_nRegCoefficient);
		
		// Check for invalid filenames...
		if(CheckForFileName(szValue, MAX_PATH, szFileName)) {
			CheckFileExists(szFileName);
		}
		if(CheckForFileName(szValueName, MAX_PATH, szFileName)) {
			CheckFileExists(szFileName);
		}
		
		// Check for invalid CLSIDs...
		if (NULL != strstr(szValueName, "{"))
			CheckClsid(szValueName);
		if (NULL != strstr(szValue, "{"))
			CheckClsid(szValue);
		
		//Count how many values scanned
		m_nCount++;
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
		
		szRegPathSubkey[(lstrlen(szRegPathSubkey)-1)] = '\0';

		m_nRegPos++;
		m_pDlg->SetProgress((int) m_nRegPos * m_nRegCoefficient);

		//Count how many subkeys have been scanned...
		m_nCount++;

		// Use recursion to enumerate all the values of the subkey
		//
		ScanRegNode (hSubkey, szRegPathSubkey);

		// Close and stop scanning the node
		//
		RegCloseKey(hSubkey);
	}
}


BOOL CRegDocList::AddItems()
{
	m_pDlg->SetStaticTask
		("Scanning registry for errors...Make yourself a cuppa...");
	m_pDlg->SetProgress(0);
	m_pDlg->SetStatic("Checking for invalid COM/ActiveX entries...");
	GetClsid();
	m_pDlg->SetStatic("Scanning HKEY_LOCAL_MACHINE...");
	ScanRegNode(HKEY_LOCAL_MACHINE, "HKEY_LOCAL_MACHINE");
	m_pDlg->SetStatic("Scanning HKEY_CURRENT_USER...");
	ScanRegNode(HKEY_CURRENT_USER, "HKEY_CURRENT_USER");
	m_pDlg->SetProgress(0);
	CheckFileExt();

	m_pDlg->DestroyWindow();
	delete m_pDlg;

		// Get a pointer to the HtmlView and reset the current directory (drive
	// enumeration changes the current dir)
	CHtmlPane *pView = (CHtmlPane*) g_wndSplitterY->GetPane(1,0);
	::SetCurrentDirectory(g_szHomeDir);
	
	CString strHtml;
	CString strURL;
	TCHAR szNo [ MAX_PATH ];

	// Build a very small HTML file to show our results...
	strHtml = "<HTML><HEAD>";
	strHtml += "<META HTTP-EQUIV=\"Content-Type\" CONTENT=\"text/html; charset=windows-1252\">";
	strHtml += "<META NAME=\"Generator\" CONTENT=\"Microsoft Word 97\">";
	strHtml += "<TITLE>Summary</TITLE></HEAD>";
	strHtml += "<BODY LINK=\"#0000ff\" VLINK=\"#800080\" BACKGROUND=\"DLL_Summary.jpg\">";
	strHtml += "<U><FONT FACE=\"Arial\" SIZE=6><P>Registry Doctor Summary</P>";
	strHtml += "</U></FONT><FONT FACE=\"Arial\" SIZE=4>";
	strHtml += "<P>";
	wsprintf(szNo, " %d errors found in %d registry entries",
		(int) GetListCtrl().GetItemCount(), (int) m_nCount);
	strHtml += szNo;
	strHtml += "</P><P>Number of invalid file paths = ";
	wsprintf (szNo, " %d ", (int) (m_nInvPaths));
	strHtml += szNo;
	strHtml += "</P><P> Number of invalid CLSID entries = ";
	wsprintf (szNo, " %d ", (int) m_nInvClsid);
	strHtml += szNo;
	strHtml += "</P><P> Number of invalid file extensions = ";
	wsprintf (szNo, " %d ", (int) m_nInvXt);
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
	
	return TRUE;
}

BOOL CRegDocList::CheckForFileName (LPTSTR pszValue, UINT cbOut, LPTSTR szOut) {

	LPTSTR pszStartFilename = pszValue;
	LPTSTR pszEndFilename;
	BOOL fBeginQuote = FALSE;
	
	// Eliminate strings that are too short
	//
	if (lstrlen (pszValue)<=2)
		return FALSE;

	// If the string does not contain ":\\" ignore it
	//
	if (NULL == strstr (pszStartFilename, ":\\")) {
		return FALSE;		
	}

	// Check to see if the string contains rundll or rundll32. If so then the dll path
	// is a second string in the path seperated by a space...
	//
	if (NULL != strstr(pszStartFilename, "rundll32.exe") ||
		(NULL != strstr(pszStartFilename, "RUNDLL32.EXE")) || 
		(NULL != strstr(pszStartFilename, "rundll.exe"))||
		(NULL != strstr(pszStartFilename, "RUNDLL.EXE"))) {
		while (*pszStartFilename != ' ')
			pszStartFilename++;

		pszStartFilename++;
	}
	
	// Check to see if the value begins with a quote, if so increment it past...
	//
	if (*pszStartFilename == '"') {
		pszStartFilename++;
	}

	// Remove any illegal or unlikely characters
	//
	pszEndFilename = strpbrk (pszStartFilename, "<>{|-/^%,;\"");
	
	// Handle any filepaths with no quotes and all legal characters
	//
	if (!pszEndFilename)
		pszEndFilename = pszValue + lstrlen(pszValue);
	
	// Chuck out those that are too small
	//
	if ((pszEndFilename - pszStartFilename) < 2) {
		return FALSE;
	}
	
	pszEndFilename--;
	
	// If there is whitespace at the end of the string, remove it
	//
	if (*pszEndFilename == ' ') {
		while ((pszEndFilename > pszStartFilename) && (*pszEndFilename ==' '))
			pszEndFilename--;
	}
	
	pszEndFilename++;

	// Check that the buffer to recieve the filepath will not overflow
	//
	unsigned nameLen = pszEndFilename - pszStartFilename;
	if (nameLen >= cbOut) {
		return FALSE;
	}

	// Copy the filename string to the output buffer and null terminate it!
	//
	memcpy (szOut, pszStartFilename, (nameLen * sizeof(TCHAR)));
	szOut[nameLen] = 0;

	return TRUE;
}

void CRegDocList::CheckFileExists (LPTSTR pszFilePath) {

	UINT cDriveType;
	TCHAR szDriveRoot[4];

	// Fail if the file is not on a fixed or network drive
	//
	strncpy (szDriveRoot, pszFilePath, 3);
	szDriveRoot[3] = '\0';

	cDriveType = GetDriveType(szDriveRoot);
	
	if ((cDriveType != DRIVE_FIXED) && (cDriveType != DRIVE_REMOTE)) {
		return;
	}
	
	// Really quick way to check that a file exists or not...
	//
	if (GetFileAttributes(pszFilePath) != 0xFFFFFFFF) {
		return;
	}
	
	GetListCtrl().InsertItem(m_nInvPaths, pszFilePath, 0);
	GetListCtrl().SetItemText(m_nInvPaths, 1, "Path does not exist");
	m_nInvPaths++;
}

void CRegDocList::GetClsid()
{
	HKEY hKey;
	DWORD dwIndex =0;
	
	// Open HKEY_CLASSES_ROOT\CLSID
	if (RegOpenKey (HKEY_CLASSES_ROOT, "CLSID", &hKey) != ERROR_SUCCESS)
		return;
	
	// Enumerate all subkeys in the open registry path, add each 
	// one to the vector
	while (TRUE) {

		LONG lResult;
		TCHAR szKey [MAX_PATH];
		DWORD dwKey = sizeof (TCHAR) * MAX_PATH;
		
		lResult = RegEnumKeyEx(hKey, dwIndex++, szKey, &dwKey, 0,0,0,0);

		if (lResult == ERROR_NO_MORE_ITEMS)
			break;

		if (lResult != ERROR_SUCCESS)
			continue;
	
		// Do not add the subkey CLSID, it is the only subkey we 
		// not want...
		if (strcmp(szKey, "CLSID"))
			m_vClsid.push_back(szKey);		
	}
	
	// Sort the vector and close the handle to the open key.
	sort (m_vClsid.begin(), m_vClsid.end());
	
	RegCloseKey(hKey);
	return;
}

void CRegDocList::CheckClsid(LPCTSTR pszClsid)
{
	int nIndex;
	LPTSTR pszStart;
	LPTSTR pszEnd;
	
	// Some CLSID value are substrings. Find '{'...
	pszStart = strstr(pszClsid, "{");
	
	// Toss out strings that are too short
	if (strlen(pszStart)<8)
		return;
	
	// Toss out strings when first value is not alphanumeric
	pszStart++;
	if (!isalnum(*pszStart))
		return;
	pszStart--;
	
	// Toss out strings that do not have '}'
	pszEnd = strstr(pszStart, "}");
	
	// Null terminate the string (get rid of rubbish on the end
	if (pszEnd == NULL)
		return;
	pszEnd++;
	*pszEnd = '\0';

	// If the CLSID is not in the vector, the CLSID is improperly registered,
	// stick it in the List Box Control...
	nIndex = GetListCtrl().GetItemCount();		
	if (!binary_search(m_vClsid.begin(), m_vClsid.end(), pszStart)) {
		GetListCtrl().InsertItem(nIndex, pszStart, 0);
		GetListCtrl().SetItemText(nIndex, 1, "Invalid CLSID descriptor...");
		m_nInvClsid++;
	}
}

void CRegDocList::CheckFileExt()
{
	DWORD dwIndex = 0;
	int nIndex =0;

	while (TRUE) {
		
		LONG lResult;
		TCHAR szKey [ MAX_PATH ];
		DWORD dwKey = sizeof (TCHAR) * MAX_PATH;
	
		lResult = RegEnumKeyEx(HKEY_CLASSES_ROOT, dwIndex++, szKey, &dwKey, 0,0,0,0);

		if (lResult == ERROR_NO_MORE_ITEMS)
			break;
		if (lResult != 0)
			continue;
		
		// If the subkey begins with '.' It is a valid filename
		// extension. Continue checking...
		if (szKey[0] == '.') {
			HKEY hKey;
			TCHAR szValue [ MAX_PATH ];
			DWORD dwValue = sizeof (TCHAR) * MAX_PATH;

			// Open the subkey and query the default value for
			// the associated program identifier
			if (RegOpenKeyEx(HKEY_CLASSES_ROOT, szKey, 0, KEY_QUERY_VALUE, &hKey)
				== ERROR_SUCCESS) {
				if (RegQueryValueEx(hKey, NULL ,0, NULL, (LPBYTE) szValue,&dwValue)
					== ERROR_SUCCESS) {					
					if (lstrlen(szValue) != 0) {
						HKEY hTest = NULL;
						
						// Attempt to open the program identifier
						// subkey. If it does not exist (we can't open
						// it, the filename extension is invalid
						if (RegOpenKeyEx(HKEY_CLASSES_ROOT, szValue, 0, 0, &hTest)
							!= ERROR_SUCCESS) {
							nIndex = GetListCtrl().GetItemCount();
							GetListCtrl().InsertItem(nIndex++, szKey, 0);
							TCHAR szColumn [ MAX_PATH ] =
								"Invalid filename extension subkey ";
							strcat (szColumn, szKey);
							GetListCtrl().SetItemText(--nIndex, 1, szColumn);
							m_nInvXt++;
						}
					}
				}
			}
		}
	}
}