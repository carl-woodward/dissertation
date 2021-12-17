// Summary.cpp : implementation file
//
#include "stdafx.h"
#include "regkit.h"
#include "Summary.h"

#include "HtmlPane.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define REGSNAP 0
#define FILESNAP 1
#define DLLDOC 2
#define REGDOC 3

extern CSplitterWnd *g_wndSplitterY;
extern TCHAR g_szHomeDir [ MAX_PATH ];

/////////////////////////////////////////////////////////////////////////////
// CSummary

IMPLEMENT_DYNCREATE(CSummary, CHtmlView)

CSummary::CSummary()
{
	//{{AFX_DATA_INIT(CSummary)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pDlg = new CSummaryDlg;
	m_pDlg->Create(IDD_SUMMARY);
	m_pDlg->ShowWindow(SW_SHOW);

	m_nProgress =1;
	m_pDlg->SetTotalProg(m_nProgress);
	m_pDlg->SetStaticTotal("Total Progress...");

	m_nRegPos =0;
	m_nRegCoefficient = (float) 100/70000;
	m_nBytesRead =0;
	m_nFreeBytes =0;
	m_nTotalBytes =0;
	m_nBytesCoefficient =0;

	m_nUnregDll =0;
	m_nRegDll =0;
	m_nDll =0;

	m_nRegCount=0;
	m_nInvXt =0;
	m_nInvClsid =0;
	m_nInvPaths =0;

	m_nFilesAdded =0;
	m_nFilesDeleted =0;
	m_nFiles =0;

	m_nRegAdded =0;
	m_nRegDeleted =0;
	m_nReg =0;

}

CSummary::~CSummary()
{
}

void CSummary::DoDataExchange(CDataExchange* pDX)
{
	CHtmlView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSummary)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSummary, CHtmlView)
	//{{AFX_MSG_MAP(CSummary)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSummary diagnostics

#ifdef _DEBUG
void CSummary::AssertValid() const
{
	CHtmlView::AssertValid();
}

void CSummary::Dump(CDumpContext& dc) const
{
	CHtmlView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSummary message handlers

void CSummary::EnumDrives(UINT unOp)
{
	int nPos = 0;
	int nDrivesAdded = 0;
	CString driveName = ("?:\\");

	DWORD dwDriveList = ::GetLogicalDrives();

	while (dwDriveList) {
		if (dwDriveList & 1) {
			
			m_nBytesRead =0;
			m_nFreeBytes =0;
			m_nTotalBytes =0;
			m_nBytesCoefficient =0;

			driveName.SetAt(0, ('A') + nPos);
			try {
				::SetCurrentDirectory((LPCTSTR)driveName);
				UINT nType = ::GetDriveType((LPCTSTR) driveName);
				switch (nType) {
					case DRIVE_REMOTE:
					case DRIVE_FIXED:
					case DRIVE_RAMDISK:
						if (CheckRM((LPCTSTR)driveName)) {
							GetDiskInfo((LPCTSTR)driveName);
							m_pDlg->SetProgress(0);
							Enumerate(unOp);
						}
						else
							m_pDlg->SetProgress(0);
						break;
					default:
						break;
				}
			}
			catch (CException * e) {
				e->ReportError();
				e->Delete();				
			}
		}
		dwDriveList >>= 1;
		nPos++;
	}	
}

bool CSummary::CheckRM(LPCTSTR lpszDriveRoot)
{
	TCHAR    szFileName[256];
    DWORD    dwHold;
	
	SetErrorMode( SEM_FAILCRITICALERRORS ); 
 
	lstrcpy( szFileName, lpszDriveRoot ); 
    lstrcat( szFileName, TEXT(".") ); 
	dwHold = GetFileAttributes( szFileName ); 
	SetErrorMode( 0 ); 
    
	if( dwHold != 0xFFFFFFFF ){
		return(TRUE); 
    } 
 	else{ 
		return(FALSE); 
    }  
}

void CSummary::Enumerate(UINT unOp)
{

	WIN32_FIND_DATA w32fd;
	char szBuffer[256];
	
	HANDLE hFind = ::FindFirstFile(("*.*"), &w32fd);

	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			::GetCurrentDirectory(256, szBuffer);
			if (strlen (szBuffer) > 3) {
				strcat (szBuffer, "\\");
			}
									
			if (w32fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
				
				strcat (szBuffer, w32fd.cFileName);
				CString fileName = w32fd.cFileName;

				if (fileName !=  (".") && fileName !=  ("..")) {
					::SetCurrentDirectory(w32fd.cFileName);
					strcat (szBuffer, "\n");
					fileName = szBuffer;
					m_pDlg->SetStatic(fileName);
					//if (unOp == FILESNAP)
					m_vNewFile.push_back(fileName);
					Enumerate(unOp);
					m_nBytesRead = m_nBytesRead + w32fd.nFileSizeLow;
				::SetCurrentDirectory((".."));
				}
			}
			else {
				strcat (szBuffer, w32fd.cFileName);
				CString fileName = szBuffer;
				fileName.MakeUpper();

				if (unOp == DLLDOC) {
					if (fileName.Find(".DLL") != -1)
						m_vFileDll.push_back(fileName);
				}
				//if (unOp == FILESNAP) {
				strcat(szBuffer, "\n");
				fileName = szBuffer;
				m_vNewFile.push_back(fileName);
				//}
				m_nBytesRead = m_nBytesRead + w32fd.nFileSizeLow;
				m_pDlg->SetProgress((int) (m_nBytesCoefficient * m_nBytesRead));
				
			}
		} while (::FindNextFile(hFind, &w32fd));
		::FindClose(hFind);
	}
}

void CSummary::ScanRegNode(HKEY hKey, LPTSTR pszRegPath, UINT unOp)
{
	DWORD regEnumIndex = 0;

	// Enumerate all registry values in a given key...
	//
	while (TRUE) {

		// Hold and check values from RegEnumValues
		TCHAR szValueName[ 320 ];
		DWORD cbValueNameSize = 320;
		TCHAR szValue [ 320 ];
		DWORD cbValueSize = 320;
		DWORD type;
		LONG result;
		
		// Hold file paths
		TCHAR szFullPath [512];
		TCHAR szFileName [MAX_PATH];
		
		// Get the values in the current key
		result = RegEnumValue (hKey, regEnumIndex++,
			szValueName, &cbValueNameSize,
			0, &type, (LPBYTE) szValue, &cbValueSize);

		// Bail out if there is a problem or we have finished...
		if (result == ERROR_NO_MORE_ITEMS) {
			if (unOp == DLLDOC) {
				CString str = pszRegPath;
				str.MakeUpper();
				if (str.Find("DLL") != -1)
					m_vRegDll.push_back(str);
			}
			break;
		}

		// Build the registry path responsible for our values...
		wsprintf (szFullPath, TEXT ("%s¬%s¬%s\n\0"), pszRegPath,szValueName, szValue);
		
		if (unOp == DLLDOC) {
			// Copy the path into a CString object. Convert to uppercase (as will everything
			// to make my life a bit easier) and check for a ".DLL" substring. Add to the
			// vector if we have one...
			CString str = szFullPath;
			str.MakeUpper();
			if (str.Find("DLL") != -1)
				m_vRegDll.push_back(str);
		}
		m_nRegPos++;
		m_pDlg->SetProgress((int) m_nRegPos * m_nRegCoefficient);

		if (unOp == REGDOC) {
			
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
			
			// Increment the counters...
			m_nRegCount++;
		}
	}

	regEnumIndex = 0;

	// Find child keys (subkeys) and use recursion to enumerate all the
	// values until there are no more subkeys left
	while (TRUE) {

		// Generate subkey with data returned from RegOpenValueEx
		TCHAR szSubkey [ 320 ];
		DWORD cbSubkeySize = 320;
		HKEY hSubkey;
		LONG result;
		TCHAR szRegPathSubkey [ 320 ];

		// Enumerate the subkeys
		result = RegEnumKeyEx (hKey, regEnumIndex++, szSubkey, &cbSubkeySize,
			0,0,0,0);

		// Bail out if there are no more subkeys
		if (result == ERROR_NO_MORE_ITEMS)
			break;

		// Ignore trivial errors
		if (result != 0)
			continue;

		// Open the child key, bail out if it cant be opened
		result = RegOpenKeyEx (hKey, szSubkey, 0, KEY_READ, &hSubkey);

		if (result != ERROR_SUCCESS)
			continue;

		// Build the subkey path
		strcpy (szRegPathSubkey, pszRegPath);
		strcat (szRegPathSubkey, "\\");
		strcat (szRegPathSubkey, szSubkey);
		strcat (szRegPathSubkey, "\n");
		
		// For the registry snapshot...
		CString str = szRegPathSubkey;
		m_vNewReg.push_back(str);

		szRegPathSubkey[(lstrlen(szRegPathSubkey)-1)] = '\0';

		m_nRegPos++;
		m_pDlg->SetProgress((int) m_nRegPos * m_nRegCoefficient);

		if (unOp == REGDOC)
			m_nRegCount++;
			
		// Use recursion to enumerate all the values of the subkey
		ScanRegNode (hSubkey, szRegPathSubkey, unOp);

		// Close and stop scanning the node
		RegCloseKey(hSubkey);
	}
}

void CSummary::GetDllInfo()
{
	int nLen =0;
	char szNo[20];
	CString strDll;
	CString strDllName;
	vector<CString>::iterator dll;
	vector<CString>::iterator reg;
	double nCount = m_vFileDll.size();
	double nCoeff = (float) ((double)100/(double)nCount);
	
	nCount =0;

	// Sort the vectors and tell the user that we are doing so... 
	m_pDlg->SetStatic("Sorting vectors...");
	sort(m_vRegDll.begin(), m_vRegDll.end());
	m_pDlg->SetTotalProg(m_nProgress++);
	sort(m_vFileDll.begin(), m_vFileDll.end());
	m_pDlg->SetTotalProg(m_nProgress++);
	
	// Set the dlg controls
	m_pDlg->SetProgress(0);
	m_pDlg->SetStatic("Comparing vectors...");

	// Iterate through each entry in the fileVector
	dll = m_vFileDll.begin();
	while (dll != m_vFileDll.end()) {
		
		// Extract the file name from the path
		strDll = *dll;
		strDll.ReverseFind('\\');
		nLen = (strDll.GetLength() - strDll.ReverseFind('\\'));
		strDllName = strDll.Right(--nLen);
		
		// Use an STL pointer function, uses strstr to find if our DllName
		// is in the registry or not...
		reg = find_if(m_vRegDll.begin(), m_vRegDll.end(),
			bind2nd(ptr_fun(strstr), strDllName));

		// If the reg iterator has reached the end of the vector it did not
		// find anything. This dll is unregistered.
		if (reg == m_vRegDll.end())
			m_nUnregDll++;
		else
			m_nRegDll++;
		dll++;
		nCount++;
		
		// Update the progress bar...
		m_pDlg->SetProgress(nCount * nCoeff);
	}
	
	m_pDlg->SetTotalProg(m_nProgress++);

	// Set the DLL counters
	m_nDll = (int) nCount;	
}

void CSummary::GetDiskInfo(LPCTSTR lpszDriveRoot)
{
	ULARGE_INTEGER lpFreeBytesAvailable;
	ULARGE_INTEGER lpTotalBytes;
	ULARGE_INTEGER lpFreeBytes;

	if(!::GetDiskFreeSpaceEx(lpszDriveRoot, &lpFreeBytesAvailable, &lpTotalBytes, &lpFreeBytes)) {
		MessageBox("Unable to initialise drives", "Error", MB_OK | MB_ICONERROR);
	}
	else {
		m_nTotalBytes = (unsigned long) lpTotalBytes.QuadPart;
		m_nFreeBytes = (unsigned long) lpFreeBytes.QuadPart;
		m_nBytesCoefficient = (float) 100/
			(m_nTotalBytes - m_nFreeBytes);
		TRACE ("DISK COEFF IS %e\n", m_nBytesCoefficient);
	}
}

bool CSummary::DoSummary()
{
	CString strURL;
	CString strHtml;
	TCHAR szNo [MAX_PATH];

	// Do the DLL Doctor bit...
	m_pDlg->SetStaticTask("Performing DLL Doctor 1.0...");
	m_pDlg->SetTotalProg(0);
	EnumDrives(DLLDOC);
	m_pDlg->SetTotalProg(m_nProgress++);
	m_pDlg->SetStaticTask("Scanning Registry for references to DLLs...");
	m_pDlg->SetStatic("Scanning HKEY_LOCAL_MACHINE for registered DLLs");
	ScanRegNode(HKEY_LOCAL_MACHINE, "HKEY_LOCAL_MACHINE", DLLDOC);
	m_pDlg->SetTotalProg(m_nProgress++);
	m_pDlg->SetStatic("Scanning HKEY_CURRENT_USER for registered DLLs");
	ScanRegNode(HKEY_CURRENT_USER, "HKEY_CURRENT_USER", DLLDOC);
	m_pDlg->SetTotalProg(m_nProgress++);
	m_pDlg->SetStaticTask("Building list of unregistered DLLs...");
	GetDllInfo();
	m_vRegDll.clear();
	m_vFileDll.clear();
	m_pDlg->SetTotalProg(m_nProgress++);

	//Do the REGDOC bit...
	m_nRegPos =0;
	m_pDlg->SetProgress(0);
	m_pDlg->SetStaticTask
		("Performing Registry Doctor 1.0...");
	m_pDlg->SetProgress(0);
	m_pDlg->SetStatic("Checking for invalid COM/ActiveX entries...");
	GetClsid();
	m_pDlg->SetTotalProg(m_nProgress++);
	m_pDlg->SetStatic("Scanning HKEY_LOCAL_MACHINE for errors...");
	ScanRegNode(HKEY_LOCAL_MACHINE, "HKEY_LOCAL_MACHINE", REGDOC);
	m_pDlg->SetTotalProg(m_nProgress++);
	m_pDlg->SetStatic("Scanning HKEY_CURRENT_USER for errors...");
	ScanRegNode(HKEY_CURRENT_USER, "HKEY_CURRENT_USER", REGDOC);
	m_pDlg->SetTotalProg(m_nProgress++);
	m_pDlg->SetProgress(0);
	CheckFileExt();
	m_pDlg->SetTotalProg(m_nProgress++);
	m_vClsid.clear();
	m_pDlg->SetTotalProg(m_nProgress++);
	
	// Do the filesnap bit...
	m_pDlg->SetProgress(100);
	m_pDlg->SetStaticTask("Preparing file snapshot...");
	m_pDlg->SetStatic("Sorting file vector...");
	sort(m_vNewFile.begin(), m_vNewFile.end());
	m_pDlg->SetTotalProg(m_nProgress++);
	m_pDlg->SetStatic("Writing file vector...");
	if (!WriteList(FILESNAP))
		return FALSE;
	m_pDlg->SetTotalProg(m_nProgress++);
	m_pDlg->SetStaticTask("Comparing with previous snapshot...");
	m_pDlg->SetStatic("Requesting file for comparison....");
	if (!OpenFile(FILESNAP))
		return FALSE;
	m_pDlg->SetTotalProg(m_nProgress++);
	GetFileSnapInfo();
	m_pDlg->SetTotalProg(m_nProgress++);
	m_vNewFile.clear();
	m_vOldFile.clear();
	m_pDlg->SetTotalProg(m_nProgress++);
	
	// Do the regsnap bit...
	m_pDlg->SetStaticTask("Preparing registry snapshot...");
	m_pDlg->SetStatic("Sorting registry vector...");
	sort(m_vNewReg.begin(), m_vNewReg.end());
	m_pDlg->SetTotalProg(m_nProgress++);
	m_pDlg->SetStatic("Writing registry vector...");
	if (!WriteList(REGSNAP))
		return FALSE;
	m_pDlg->SetTotalProg(m_nProgress++);
	m_pDlg->SetStaticTask("Comparing with previous registry snapshot...");
	m_pDlg->SetStatic("Requesting file for comparison...");
	if (!OpenFile(REGSNAP))
		return FALSE;
	m_pDlg->SetTotalProg(m_nProgress++);
	GetRegSnapInfo();
	m_pDlg->SetTotalProg(m_nProgress++);
	m_vNewReg.clear();
	m_vOldReg.clear();
	m_pDlg->SetTotalProg(m_nProgress++);
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
	strHtml += "<U><FONT FACE=\"Arial\" SIZE=6><P>RegKit 1.0 Summary</P>";
	strHtml += "</U></FONT><FONT FACE=\"Arial\" SIZE=4>";
	strHtml += "<P>";
	strHtml += time;
	strHtml += "</P>";
	strHtml += "<P>Number of unregistered DLL files = ";
	wsprintf(szNo, " %d ", m_nUnregDll);
	strHtml += szNo;
	strHtml += "</P><P>Number of registered DLL files = ";
	wsprintf (szNo, " %d ", m_nRegDll);
	strHtml += szNo;
	strHtml += "</P><P> Number of DLL files = ";
	wsprintf (szNo, " %d ", m_nDll);
	strHtml += szNo;
	strHtml += "<P>";
	wsprintf(szNo, " %d errors found in %d registry entries",
		(int) (m_nInvPaths+m_nInvClsid+m_nInvXt), (int) m_nRegCount);
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
	strHtml += "<P> Previous File Snapshot: ";
	strHtml += m_strFileSnapName;
	strHtml += "</P>";
	strHtml += "<P>Number of files added = ";
	wsprintf(szNo, " %d ", (int) m_nFilesAdded);
	strHtml += szNo;
	strHtml += "</P><P>Number of files deleted = ";
	wsprintf (szNo, " %d ", (int) m_nFilesDeleted);
	strHtml += szNo;
	strHtml += "</P><P> Number of files on local file store = ";
	wsprintf (szNo, " %d ", (int) m_nFiles);
	strHtml += szNo;
	strHtml += "<P> Previous Reg Snapshot: ";
	strHtml += m_strRegSnapName;
	strHtml += "</P>";
	strHtml += "<P>Number of registry keys added = ";
	wsprintf(szNo, " %d ", (int) m_nRegAdded);
	strHtml += szNo;
	strHtml += "</P><P>Number of registry keys deleted = ";
	wsprintf (szNo, " %d ", (int) m_nRegDeleted);
	strHtml += szNo;
	strHtml += "</P><P> Number of registry keys = ";
	wsprintf (szNo, " %d ", (int) m_nReg);
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

	if (m_pDlg != NULL) {
		m_pDlg->DestroyWindow();
		delete m_pDlg;
	}
}

bool CSummary::CheckForFileName (LPTSTR pszValue, UINT cbOut, LPTSTR szOut) {

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

void CSummary::CheckFileExists (LPTSTR pszFilePath) {

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
	
	m_nInvPaths++;
}

void CSummary::GetClsid()
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

void CSummary::CheckClsid(LPCTSTR pszClsid)
{
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
	if (!binary_search(m_vClsid.begin(), m_vClsid.end(), pszStart))
		m_nInvClsid++;	
}

void CSummary::CheckFileExt()
{
	DWORD dwIndex = 0;

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
							m_nInvXt++;
						}
					}					
				}
			}
		}
	}
}

bool CSummary::WriteList(UINT unOp)
{
	LPTSTR pszFile;
	::SetCurrentDirectory(g_szHomeDir);
	vector<CString>::iterator p;
	CTime t = CTime::GetCurrentTime();
	CString time = t.Format("%Y_%m_%d_%H_%M_");
	pszFile = time.GetBuffer(MAX_PATH);

	if (unOp == FILESNAP)
		strcat (pszFile, "file.djc");
	else
		strcat (pszFile, "reg.djc");

	// Create a new file for writing and dump linkedList strings to it
	// This function must have CFileException error handling
	//
	try {
		CStdioFile file (_T(pszFile), CFile::modeWrite | CFile::modeCreate);
		if (unOp ==FILESNAP) {
			p = m_vNewFile.begin();
			while (p != m_vNewFile.end()) {
				file.WriteString(*p);
				p++;
			}
		}
		else {
			p = m_vNewReg.begin();
			while (p != m_vNewReg.end()) {
				file.WriteString(*p);
				p++;
			}
		}		
	}
	catch (CFileException* e) {
		e->ReportError();
		e->Delete();
		return FALSE;
	}
	return TRUE;
}

bool CSummary::OpenFile(UINT unOp)
{
	BOOL bOKFileName =FALSE;
	TCHAR szFilters[] =
		(_T("DjcSoft Regkit files (*.djc)|*.djc|All files(*.*)|*.*|"));
	
	CFileDialog dlg (TRUE, _T("djc"),_T ("*.djc"),
		OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, szFilters);

	while (!bOKFileName) {
		if (dlg.DoModal() == IDOK) {
			CString strRead;
			if (unOp == FILESNAP) {
				m_strFileSnapName = dlg.GetFileName();
				if (NULL != strstr(m_strFileSnapName, "file")) {
					m_pDlg->SetStatic("Loading file...");
					try {
						CStdioFile file (m_strFileSnapName, CFile::modeRead);
						while(file.ReadString(strRead))
							m_vOldFile.push_back(strRead);
					}
					catch (CFileException *e) {
						e->ReportError();
						e->Delete();
						return FALSE;
					}
					bOKFileName = TRUE;
				}
				else {
					CString strMessage;
					strMessage.Format
						("Error opening %s\n\nPlease select a file with \"file\" in its name\n",
						m_strFileSnapName);
					MessageBox(strMessage, "Error", MB_ICONERROR | MB_OK);
				}
			}
			else if (unOp == REGSNAP) {
				m_strRegSnapName = dlg.GetFileName();
				if (NULL != strstr(m_strRegSnapName, "reg")) {
					bOKFileName = TRUE;
					m_pDlg->SetStatic("Loading file...");
					try {
						CStdioFile file (m_strRegSnapName, CFile::modeRead);
						while(file.ReadString(strRead)) {
							m_vOldReg.push_back(strRead);
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
						m_strRegSnapName);
					MessageBox(strMessage, "Error", MB_ICONERROR | MB_OK);
				}
			}			
		}
		else {
			if (m_pDlg == NULL) {
				m_pDlg->DestroyWindow();
				delete m_pDlg;
			}
			return FALSE;		
		}
	}
	return TRUE;
}

bool CSummary::GetFileSnapInfo()
{
	TCHAR szNo[20];
	CString strFile;
	CString strPrev;
	vector<CString>::iterator file;
	vector<CString>::iterator prev;
	double nCount = m_vNewFile.size();
	double nCoeff = (float) ((double)100/(double)nCount);
	
	nCount =0;
	
	// Set the dlg controls
	m_pDlg->SetProgress(0);
	m_pDlg->SetStatic("Searching for added files...");

	// Iterate through each entry in the fileVector
	file = m_vNewFile.begin();
	while (file != m_vNewFile.end()) {
		
		strFile = *file;
		strFile.Remove('\n');
						
		if (!binary_search(m_vOldFile.begin(), m_vOldFile.end(), strFile))
			m_nFilesAdded++;
		file++;
		nCount++;
		
		// Update the progress bar...
		m_pDlg->SetProgress(nCount * nCoeff);
	}
	nCount = 0;

	// Set the dlg controls
	m_pDlg->SetTotalProg(m_nProgress++);
	m_pDlg->SetProgress(0);
	m_pDlg->SetStatic("Searching for deleted files");

	// Iterate through each entry in the fileVector
	prev = m_vOldFile.begin();
	while (prev != m_vOldFile.end()) {
		
		strPrev = *prev;
		strPrev += '\n';
		// Use a binary search to see if the file has been deleted or not..
		
		if (!binary_search(m_vNewFile.begin(), m_vNewFile.end(), strPrev))
			m_nFilesDeleted++;
		prev++;
		nCount++;
		
		// Update the progress bar...
		m_pDlg->SetProgress(nCount * nCoeff);
	}
	
	m_pDlg->SetTotalProg(m_nProgress++);
	m_nFiles = m_vNewFile.size();
	return TRUE;
}

bool CSummary::GetRegSnapInfo()
{
	char szNo[20];
	CString strFile;
	CString strPrev;
	vector<CString>::iterator reg;
	vector<CString>::iterator prev;
	double nCount = m_vNewReg.size();
	double nCoeff = (float) ((double)100/(double)nCount);
	
	nCount =0;
	
	// Set the dlg controls
	m_pDlg->SetProgress(0);
	m_pDlg->SetStatic("Searching for added registry keys...");

	// Iterate through each entry in the fileVector
	reg = m_vNewReg.begin();
	while (reg != m_vNewReg.end()) {
		
		strFile = *reg;
		strFile.Remove('\n');
						
		if (!binary_search(m_vOldReg.begin(), m_vOldReg.end(), strFile))
			m_nRegAdded++;
		reg++;
		nCount++;
		
		// Update the progress bar...
		m_pDlg->SetProgress(nCount * nCoeff);
	}
	nCount = 0;

	// Set the dlg controls
	m_pDlg->SetTotalProg(m_nProgress++);
	m_pDlg->SetProgress(0);
	m_pDlg->SetStatic("Searching for deleted regsitry keys...");

	// Iterate through each entry in the fileVector
	prev = m_vOldReg.begin();
	while (prev != m_vOldReg.end()) {
		
		strPrev = *prev;
		strPrev += '\n';
		// Use a binary search to see if the file has been deleted or not..
		
		if (!binary_search(m_vNewReg.begin(), m_vNewReg.end(), strPrev))
			m_nRegDeleted++;
		prev++;
		nCount++;
		
		// Update the progress bar...
		m_pDlg->SetProgress(nCount * nCoeff);
	}
	m_pDlg->SetTotalProg(m_nProgress++);
	m_nReg = m_vNewReg.size();
	return TRUE;
}