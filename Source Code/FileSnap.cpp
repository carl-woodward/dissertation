// FileSnap.cpp : implementation file
//

#include "stdafx.h"
#include "regkit.h"
#include "FileSnap.h"

#include <algorithm>
#include <functional>
#include "HtmlPane.h"

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define ILI_ADDED 0
#define ILI_DELETED 1

extern TCHAR g_szHomeDir[256];
extern CSplitterWnd *g_wndSplitterY;

/////////////////////////////////////////////////////////////////////////////
// CFileSnap

IMPLEMENT_DYNCREATE(CFileSnap, CListView)

CFileSnap::CFileSnap()
{
	m_pDlg = new CProgDlg;
	m_pDlg->Create(IDD_PROG_DLG);
	m_pDlg->ShowWindow(SW_SHOW);
	m_nBytesRead =0;
	m_nFreeBytes =0;
	m_nTotalBytes =0;
	m_nBytesCoefficient =0;
}

CFileSnap::~CFileSnap()
{
}


BEGIN_MESSAGE_MAP(CFileSnap, CListView)
	//{{AFX_MSG_MAP(CFileSnap)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFileSnap drawing

void CFileSnap::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CFileSnap diagnostics

#ifdef _DEBUG
void CFileSnap::AssertValid() const
{
	CListView::AssertValid();
}

void CFileSnap::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CFileSnap message handlers

void CFileSnap::OnInitialUpdate() 
{
	// Call base class constructor
	CListView::OnInitialUpdate();
	int nWidth =0;
	RECT rect;
	CBitmap bit;
	
	// Create a bitmap
	bit.LoadBitmap(IDB_DELETED);

	// Calculate size of first column
	GetClientRect(&rect);
	nWidth = ((rect.right/4)*3);
	
	// Create image lists and first column
	m_ilSmall.Create(IDB_ADDED, 16, 1, RGB(255,0,255));
	m_ilSmall.Add(&bit, RGB (255,0,255));
	GetListCtrl().SetImageList(&m_ilSmall, LVSIL_SMALL);
	GetListCtrl().InsertColumn(0, "FileName", LVCFMT_LEFT, nWidth);

	// Calculate size of and create the second column
	nWidth = (rect.right/4);
	GetListCtrl().InsertColumn(1, "State", LVCFMT_LEFT, nWidth);	
}

BOOL CFileSnap::PreCreateWindow(CREATESTRUCT& cs) 
{
	// Call base class constructor, fail if the viewis not initialised
	if (!CListView::PreCreateWindow(cs))
		return FALSE;
	
	// Create a report view
	cs.style &=~LVS_TYPEMASK;
	cs.style |= LVS_REPORT;
	return TRUE;
}

void CFileSnap::EnumDrives()
{
	int nPos = 0;
	int nDrivesAdded = 0;
	CString driveName = ("?:\\");

	// Get a bitmask containing available drives...
	DWORD dwDriveList = ::GetLogicalDrives();
	
	// While we have not reached the end of the bitmask
	while (dwDriveList) {
		
		// If there is a drive
		if (dwDriveList & 1) {
			
			// Initialise drive space variables and set drive root path
			m_nBytesRead =0;
			m_nFreeBytes =0;
			m_nTotalBytes =0;
			m_nBytesCoefficient =0;
			driveName.SetAt(0, ('A') + nPos);
						
			// Possible file I/O exceptions here
			try {
				// Change dir
				::SetCurrentDirectory((LPCTSTR)driveName);
				
				// Only check local fixed and remote drives...
				UINT nType = ::GetDriveType((LPCTSTR) driveName);
				switch (nType) {
					case DRIVE_REMOTE:
					case DRIVE_FIXED:
					case DRIVE_RAMDISK:
						
						// Check there is media in the drive
						if (CheckRM((LPCTSTR)driveName)) {
							GetDiskInfo((LPCTSTR)driveName);
							m_pDlg->SetProgress(0);
							
							// Enumerate all files and directories on the current
							// drive
							Enumerate();
						}
						
						// No media, reset the GUI
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
		// View next bit
		dwDriveList >>= 1;
		nPos++;
	}	
}

bool CFileSnap::CheckRM(LPCTSTR lpszDriveRoot)
{
	TCHAR    szFileName[256];
    DWORD    dwHold;
	
	// Create filepath "?:\." which is a default dir (Must exist)
	SetErrorMode( SEM_FAILCRITICALERRORS ); 
 
	lstrcpy( szFileName, lpszDriveRoot ); 
    lstrcat( szFileName, TEXT(".") ); 
	
	// Check to see if default dir exists
	dwHold = GetFileAttributes( szFileName ); 
	SetErrorMode( 0 ); 
    
	// There is media in the drive
	if( dwHold != 0xFFFFFFFF ){
		return(TRUE); 
    } 
 	// No media in the drive
	else{ 
		return(FALSE); 
    }  
}

void CFileSnap::Enumerate()
{
	// This code is repeated all over the place, I'll only comment important bits
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
					
					// Add the file path to the vector
					m_fileVector.push_back(fileName);
					Enumerate();
					m_nBytesRead = m_nBytesRead + w32fd.nFileSizeLow;
				::SetCurrentDirectory((".."));
				}
			}
			else {
				strcat (szBuffer, w32fd.cFileName);
				strcat (szBuffer, "\n");
				CString fileName = szBuffer;
				
				// Add the file path to the vector
				m_fileVector.push_back(fileName);
				m_nBytesRead = m_nBytesRead + w32fd.nFileSizeLow;
				m_pDlg->SetProgress((int) (m_nBytesCoefficient * m_nBytesRead));
				
			}
		} while (::FindNextFile(hFind, &w32fd));
		::FindClose(hFind);
	}
}

void CFileSnap::GetDiskInfo(LPCTSTR lpszDriveRoot)
{
	// Again this code is commented elsewhere...give me a break!!!	
	ULARGE_INTEGER lpFreeBytesAvailable;
	ULARGE_INTEGER lpTotalBytes;
	ULARGE_INTEGER lpFreeBytes;

	if(!::GetDiskFreeSpaceEx
		(lpszDriveRoot, &lpFreeBytesAvailable,
		&lpTotalBytes, &lpFreeBytes)) {
		MessageBox("Unable to initialise drives",
			"Error", MB_OK | MB_ICONERROR);
	}
	else {
		m_nTotalBytes = (unsigned long) lpTotalBytes.QuadPart;
		m_nFreeBytes = (unsigned long) lpFreeBytes.QuadPart;
		m_nBytesCoefficient = (float) 100/
			(m_nTotalBytes - m_nFreeBytes);
		TRACE ("DISK COEFF IS %e\n", m_nBytesCoefficient);
	}
}

bool CFileSnap::WriteList()
{
	LPTSTR pszFile;
	::SetCurrentDirectory(g_szHomeDir);
	vector<CString>::iterator p;
	
	// Get the current time and format a string (to be used in the filename...)
	CTime t = CTime::GetCurrentTime();
	CString time = t.Format("%Y_%m_%d_%H_%M_");
	pszFile = time.GetBuffer(MAX_PATH);

	// Indicate the file is a file snaphot...
	strcat (pszFile, "file.djc");

	// Create a new file for writing and dump linkedList strings to it
	// This function must have CFileException error handling
	try {
		CStdioFile file (_T(pszFile), CFile::modeWrite | CFile::modeCreate);
		p = m_fileVector.begin();
		while (p != m_fileVector.end()) {
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

BOOL CFileSnap::AddItems()
{
	// Enumerate all local and remote fixed drives files 
	m_pDlg->SetStaticTask("Taking snapshot of file-system...");
	EnumDrives();
	m_pDlg->SetProgress(100);
	m_pDlg->SetStaticTask("Doing some serious processing...");
	m_pDlg->SetStatic("Sorting file vector...");
	
	// sort the vector so we can do binary searches
	sort(m_fileVector.begin(), m_fileVector.end());
	m_pDlg->SetStatic("Writing file vector...");
	
	// Write the list
	if (!WriteList())
		return FALSE;
	m_pDlg->SetStaticTask("Comparing with previous snapshot...");
	m_pDlg->SetStatic("Requesting file for comparison....");
	
	// Open a second (older) snapshot and compare the two vectors
	if (!OpenFile())
		return FALSE;
	Compare();
	
	return TRUE;
}

void CFileSnap::KillDlg()
{
	// Destroy the progress dialog and free up allocated memory
	if (m_pDlg !=NULL) {
		m_pDlg->DestroyWindow();
		delete m_pDlg;
	}
}

BOOL CFileSnap::OpenFile()
{
	// Flag set true when an acceptable filename is chosen
	BOOL bOKFileName =FALSE;
	
	// Construct an open file common dialog
	TCHAR szFilters[] =
		(_T("DjcSoft Regkit files (*.djc)|*.djc|All files(*.*)|*.*|"));
	CFileDialog dlg (TRUE, _T("djc"),_T ("*.djc"),
		OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, szFilters);

	// Ask for a file to open until an acceptable filename is chosen
	while (!bOKFileName) {
		
		// Process the user clicking OK
		if (dlg.DoModal() == IDOK) {
			
			// Get the filename
			CString strRead;
			m_strFileName = dlg.GetFileName();
			
			// If the user has picked an acceptable filename
			if (NULL != strstr(m_strFileName, "file")) {
				m_pDlg->SetStatic("Loading file...");
				
				// Possible file I/O exceptions
				try {
					// Open the file and read the data line by line into a vector
					CStdioFile file (m_strFileName, CFile::modeRead);
					while(file.ReadString(strRead))
						m_prevVector.push_back(strRead);
				}
				catch (CFileException *e) {
					e->ReportError();
					e->Delete();
					return FALSE;
				}
				bOKFileName = TRUE;
			}
			
			// Cant use this file
			else {
				CString strMessage;
				strMessage.Format
					("Error opening %s\n\nPlease select a file with \"file\" in its name\n",
					m_strFileName);
				MessageBox(strMessage, "Error", MB_ICONERROR | MB_OK);
			}
		}
		
		// The user clicked cancel, kill the progress dialog and go home...
		else {
			KillDlg();
			return FALSE;		
		}
	}
	return TRUE;
}

bool CFileSnap::Compare()
{
	int nIndex =0;
	int nAdded =0;
	int nDeleted =0;
	char szNo[20];
	CString strFile;
	CString strPrev;
	CString strURL;
	CString strHtml;
	vector<CString>::iterator file;
	vector<CString>::iterator prev;
	double nCount = m_fileVector.size();
	double nCoeff = (float) ((double)100/(double)nCount);
	
	nCount =0;
	
	// Set the dlg controls
	m_pDlg->SetProgress(0);
	m_pDlg->SetStatic("Searching for added files...");

	// Iterate through each entry in the fileVector
	file = m_fileVector.begin();
	while (file != m_fileVector.end()) {
		
		strFile = *file;
		strFile.Remove('\n');
						
		if (!binary_search(m_prevVector.begin(), m_prevVector.end(), strFile)) {
			GetListCtrl().InsertItem(nIndex++, strFile, ILI_ADDED);
			GetListCtrl().SetItemText(--nIndex, 1, "Added");
			nIndex++;
		}
		file++;
		nCount++;
		
		// Update the progress bar...
		m_pDlg->SetProgress(nCount * nCoeff);
	}
	nAdded = nIndex;
	nCount = 0;

	// Set the dlg controls
	m_pDlg->SetProgress(0);
	m_pDlg->SetStatic("Searching for deleted files");

	// Iterate through each entry in the fileVector
	prev = m_prevVector.begin();
	while (prev != m_prevVector.end()) {
		
		strPrev = *prev;
		strPrev += '\n';
		// Use a binary search to see if the file has been deleted or not..
		
		if (!binary_search(m_fileVector.begin(), m_fileVector.end(), strPrev)) {
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
	strHtml += "<U><FONT FACE=\"Arial\" SIZE=6><P>File Snapshot Summary</P>";
	strHtml += "</U></FONT><FONT FACE=\"Arial\" SIZE=4>";
	strHtml += "<P>";
	strHtml += time;
	strHtml += "</P>";
	strHtml += "<P>Comparing against: ";
	strHtml += m_strFileName;
	strHtml += "</P>";
	strHtml += "<P>Number of files added = ";
	wsprintf(szNo, " %d ", (int) nAdded);
	strHtml += szNo;
	strHtml += "</P><P>Number of files deleted = ";
	wsprintf (szNo, " %d ", (int) nDeleted);
	strHtml += szNo;
	strHtml += "</P><P> Number of files on local file store = ";
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
	KillDlg();
	return TRUE;
}
