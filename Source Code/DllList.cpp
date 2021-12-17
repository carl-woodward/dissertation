// DllList.cpp : implementation file
//

#include "stdafx.h"
#include "regkit.h"
#include "DllList.h"

#include <algorithm>
#include <functional>
#include "HtmlPane.h"

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CSplitterWnd *g_wndSplitterY;
extern TCHAR g_szHomeDir[MAX_PATH];

/////////////////////////////////////////////////////////////////////////////
// CDllList

IMPLEMENT_DYNCREATE(CDllList, CListView)

CDllList::CDllList()
{
	// Create a new progress dialog
	m_pDlg = new CProgDlg;
	m_pDlg->Create(IDD_PROG_DLG);
	m_pDlg->ShowWindow(SW_SHOW);

	// Initialise all my variables...
	m_nRegPos =0;
	m_nRegCoefficient = (float) 100/60000;
	m_nBytesRead =0;
	m_nFreeBytes =0;
	m_nTotalBytes =0;
	m_nBytesCoefficient =0;
}

CDllList::~CDllList()
{
	// CDllList enumerates through all the directories and drives...
	// Ensure we are in the home dir and not the last drive vistited
	::SetCurrentDirectory(g_szHomeDir);
}


BEGIN_MESSAGE_MAP(CDllList, CListView)
	//{{AFX_MSG_MAP(CDllList)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDllList drawing

void CDllList::OnDraw(CDC* pDC)
{
	//CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CDllList diagnostics

#ifdef _DEBUG
void CDllList::AssertValid() const
{
	CListView::AssertValid();
}

void CDllList::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDllList view initialisation..

void CDllList::OnInitialUpdate() 
{
	// Call base class constructor
	CListView::OnInitialUpdate();
	RECT rect;
	
	// Create and add the 16X16 image list
	m_ilSmall.Create(IDB_DLL_FILE, 16, 1, RGB (255,0,255));
	GetListCtrl().SetImageList(&m_ilSmall, LVSIL_SMALL);
		
	// Create the one and only column in this view to be full size
	GetClientRect(&rect);
	GetListCtrl().InsertColumn(0, "Unregistered Dlls", LVCFMT_LEFT, rect.right);
}

BOOL CDllList::PreCreateWindow(CREATESTRUCT& cs) 
{
	// Call base class constructor
	if(!CListView::PreCreateWindow(cs))
		return FALSE;
	
	// Create the list view in a report style
	cs.style &= ~LVS_TYPEMASK;
	cs.style |= LVS_REPORT;
	return TRUE;
}

BOOL CDllList::AddItems()
{	
	// Enumerate the entire file store and the physical registry
	m_pDlg->SetStaticTask("Scanning file store for DLLs...");
	EnumDrives();
	m_pDlg->SetStaticTask("Scanning Registry for references to DLLs...");
	m_pDlg->SetStatic("Scanning HKEY_LOCAL_MACHINE...");
	ScanRegNode(HKEY_LOCAL_MACHINE, "HKEY_LOCAL_MACHINE");
	m_pDlg->SetStatic("Scanning HKEY_CURRENT_USER...");
	ScanRegNode(HKEY_CURRENT_USER, "HKEY_CURRENT_USER");
	m_pDlg->SetStaticTask("Building list of unregistered DLLs...");
	
	// Find unregsitered DLLs
	Compare();
	
	return TRUE;
}

void CDllList::EnumDrives()
{
	int nPos = 0;
	int nDrivesAdded = 0;
	CString driveName = ("?:\\");

	// Get a bitmask containing all the drives...
	DWORD dwDriveList = ::GetLogicalDrives();
	
	// Examine each bit in the bitmask
	while (dwDriveList) {
		
		// If a drive is found...
		if (dwDriveList & 1) {
			
			m_nBytesRead =0;
			m_nFreeBytes =0;
			m_nTotalBytes =0;
			m_nBytesCoefficient =0;

			// Build the drive root path
			driveName.SetAt(0, ('A') + nPos);
						
			// Execptions!!! CATCH 'EM!
			try {
				::SetCurrentDirectory((LPCTSTR)driveName);
							
				// If there is media in the drive, update the GIU and enumerate
				if (CheckRM((LPCTSTR)driveName)) {
					GetDiskInfo((LPCTSTR)driveName);
					m_pDlg->SetProgress(0);
					Enumerate();
				}
				// If there is no media, reset the GUI
				else
					m_pDlg->SetProgress(0);
			}
			catch (CException * e) {
				e->ReportError();
				e->Delete();				
			}
		}
		// Look at the next element in the bitmask and incrememnt the drive letter
		// counter...
		dwDriveList >>= 1;
		nPos++;
	}	
}

bool CDllList::CheckRM(LPCTSTR lpszDriveRoot)
{
	TCHAR    szFileName[256];
    DWORD    dwHold;
	
	SetErrorMode( SEM_FAILCRITICALERRORS ); 
 
	// Create "?:\." where ? is the drive letter. This represents an omnipresent file
	lstrcpy( szFileName, lpszDriveRoot ); 
    lstrcat( szFileName, TEXT(".") ); 
	
	// Check if the file is there or not (or if the drive contains media)
	dwHold = GetFileAttributes( szFileName ); 
	SetErrorMode( 0 ); 
    
	if( dwHold != 0xFFFFFFFF ){
		return(TRUE); 
    } 
 	// There is no media in the drive
	else{ 
		return(FALSE); 
    }  
}

void CDllList::Enumerate()
{
	WIN32_FIND_DATA w32fd;
	char szBuffer[256];
	
	// Get a handle to the first file in the current directory
	HANDLE hFind = ::FindFirstFile(("*.*"), &w32fd);

	// If we have a handle, we can enumerate all the files and directories...
	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			// Attain the current dir, and later concatenate file
			// and directory names
			::GetCurrentDirectory(256, szBuffer);
			if (strlen (szBuffer) > 3) {
				strcat (szBuffer, "\\");
			}
									
			// If we found a directory
			if (w32fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
				
				// Build the file path
				strcat (szBuffer, w32fd.cFileName);
				CString fileName = w32fd.cFileName;

				// If the directory is not one of the defaults...
				if (fileName !=  (".") && fileName !=  ("..")) {
					// Change dir to the new file path and enumerate again...
					::SetCurrentDirectory(w32fd.cFileName);
					strcat (szBuffer, "\n");
					fileName = szBuffer;
					m_pDlg->SetStatic(fileName);
					Enumerate();
					m_nBytesRead = m_nBytesRead + w32fd.nFileSizeLow;
					
					// Jump up one directory level
					::SetCurrentDirectory((".."));
				}
			}
			else {
				// Build the filepath
				strcat (szBuffer, w32fd.cFileName);
				CString fileName = szBuffer;
				fileName.MakeUpper();
				// Search for instances of .DLL and add posistives to a vector
				if (fileName.Find(".DLL") != -1)
					m_fileVector.push_back(fileName);
				
				m_nBytesRead = m_nBytesRead + w32fd.nFileSizeLow;
				m_pDlg->SetProgress((int) (m_nBytesCoefficient * m_nBytesRead));
				
			}
		} while (::FindNextFile(hFind, &w32fd));
		
		// Quit, we've enumerated the entire file-store...
		::FindClose(hFind);
	}
}

void CDllList::ScanRegNode(HKEY hKey, LPTSTR pszRegPath)
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
			CString str = pszRegPath;
			str.MakeUpper();
			if (str.Find("DLL") != -1)
				m_regVector.push_back(str);
			break;
		}

		// Build the registry path responsible for our values...
		//
		wsprintf (szFullPath, TEXT ("%s¬%s¬%s\n\0"), pszRegPath,szValueName, szValue);
		
		// Copy the path into a CString object. Convert to uppercase (as will everything
		// to make my life a bit easier) and check for a ".DLL" substring. Add to the
		// vector if we have one...
		CString str = szFullPath;
		str.MakeUpper();
		if (str.Find("DLL") != -1)
			m_regVector.push_back(str);
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

void CDllList::Compare()
{
	int nLen =0;
	int nIndex =0;
	char szNo[20];
	CString strDll;
	CString strURL;
	CString strHtml;
	CString strDllName;
	vector<CString>::iterator dll;
	vector<CString>::iterator reg;
	double nCount = m_fileVector.size();
	double nCoeff = (float) ((double)100/(double)nCount);
	
	nCount =0;

	// Sort the vectors and tell the user that we are doing so... 
	m_pDlg->SetStatic("Sorting vectors...");
	sort(m_regVector.begin(), m_regVector.end());
	sort(m_fileVector.begin(), m_fileVector.end());
	
	// Set the dlg controls
	m_pDlg->SetProgress(0);
	m_pDlg->SetStatic("Comparing vectors...");

	// Iterate through each entry in the fileVector
	dll = m_fileVector.begin();
	while (dll != m_fileVector.end()) {
		
		// Extract the file name from the path
		strDll = *dll;
		strDll.ReverseFind('\\');
		nLen = (strDll.GetLength() - strDll.ReverseFind('\\'));
		strDllName = strDll.Right(--nLen);
		
		// Use an STL pointer function, uses strstr to find if our DllName
		// is in the registry or not...
		reg = find_if(m_regVector.begin(), m_regVector.end(),
			bind2nd(ptr_fun(strstr), strDllName));

		// If the reg iterator has reached the end of the vector it did not
		// find anything. This dll is unregistered.
		if (reg == m_regVector.end())
			GetListCtrl().InsertItem(nIndex++, strDll, 0);
		dll++;
		nCount++;
		
		// Update the progress bar...
		m_pDlg->SetProgress(nCount * nCoeff);
	}

	// Get a pointer to the HtmlView and reset the current directory (drive
	// enumeration changes the current dir)
	CHtmlPane *pView = (CHtmlPane*) g_wndSplitterY->GetPane(1,0);
	::SetCurrentDirectory(g_szHomeDir);
	
	// Build a very small HTML file to show our results...
	strHtml = "<HTML><HEAD>";
	strHtml += "<META HTTP-EQUIV=\"Content-Type\" CONTENT=\"text/html; charset=windows-1252\">";
	strHtml += "<META NAME=\"Generator\" CONTENT=\"Microsoft Word 97\">";
	strHtml += "<TITLE>Summary</TITLE></HEAD>";
	strHtml += "<BODY LINK=\"#0000ff\" VLINK=\"#800080\" BACKGROUND=\"DLL_Summary.jpg\">";
	strHtml += "<U><FONT FACE=\"Arial\" SIZE=6><P>DLL Doctor Summary</P>";
	strHtml += "</U></FONT><FONT FACE=\"Arial\" SIZE=4>";
	strHtml += "<P>Number of unregistered DLL files = ";
	wsprintf(szNo, " %d ", (int) nIndex);
	strHtml += szNo;
	strHtml += "</P><P>Number of registered DLL files = ";
	wsprintf (szNo, " %d ", (int) ((int) m_fileVector.size() - (int) nIndex));
	strHtml += szNo;
	strHtml += "</P><P> Number of DLL files = ";
	wsprintf (szNo, " %d ", (int) m_fileVector.size());
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
	
	// Kill the dialog or we will have a memory leak...
	m_pDlg->DestroyWindow();
	delete m_pDlg;
}

void CDllList::GetDiskInfo(LPCTSTR lpszDriveRoot)
{
	ULARGE_INTEGER lpFreeBytesAvailable;
	ULARGE_INTEGER lpTotalBytes;
	ULARGE_INTEGER lpFreeBytes;

	// Get the free disk space...report if this fails...
	if(!::GetDiskFreeSpaceEx(lpszDriveRoot, &lpFreeBytesAvailable,
							&lpTotalBytes, &lpFreeBytes)) {
		MessageBox("Unable to initialise drives", "Error", MB_OK | MB_ICONERROR);
	}
	// Get the number of bytes available, bytes free and a co-efficient
	// representing 1% of the used drive space
	else {
		m_nTotalBytes = (unsigned long) lpTotalBytes.QuadPart;
		m_nFreeBytes = (unsigned long) lpFreeBytes.QuadPart;
		m_nBytesCoefficient = (float) 100/
			(m_nTotalBytes - m_nFreeBytes);
		TRACE ("DISK COEFF IS %e\n", m_nBytesCoefficient);
	}
}
