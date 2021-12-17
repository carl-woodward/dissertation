// DriveList.cpp : implementation file
//

#include "stdafx.h"
#include "RegKit.h"
#include "DriveList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDriveList

IMPLEMENT_DYNCREATE(CDriveList, CListView)

CDriveList::CDriveList()
{
}

CDriveList::~CDriveList()
{
}


BEGIN_MESSAGE_MAP(CDriveList, CListView)
	//{{AFX_MSG_MAP(CDriveList)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDriveList drawing

void CDriveList::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CDriveList diagnostics

#ifdef _DEBUG
void CDriveList::AssertValid() const
{
	CListView::AssertValid();
}

void CDriveList::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDriveList message handlers

void CDriveList::OnInitialUpdate() 
{
	CListView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class

	// Create the image lists that we are going to use...
	m_ilLarge.Create(IDB_LARGE_FILE, 32, 1, RGB (255, 0, 255));
	m_ilSmall.Create(IDB_SMALL_FILE, 16, 1, RGB (255, 0, 255));
	
	// Set the image list...
	GetListCtrl().SetImageList(&m_ilLarge, LVSIL_NORMAL);
	GetListCtrl().SetImageList(&m_ilSmall, LVSIL_SMALL);

	//
	// Add columns to the list view.
	//
    GetListCtrl ().InsertColumn (0, _T ("File Name"), LVCFMT_LEFT, 192);
    GetListCtrl ().InsertColumn (1, _T ("Size (Bytes)"), LVCFMT_RIGHT, 96);
    GetListCtrl ().InsertColumn (2, _T ("Last Modified"), LVCFMT_RIGHT, 128);

	//
	// Populate the list view with items.
	//
	TCHAR szPath[MAX_PATH];
	::GetCurrentDirectory (sizeof (szPath) / sizeof (TCHAR), szPath);
	Refresh (szPath);

	
}

/////////////////////////////////////////////////////////////////////////////
// Initialise the view before we create it...

BOOL CDriveList::PreCreateWindow(CREATESTRUCT& cs) 
{
	// Call the base class constructor, fail if the view cannot be initialised
	if(!CListView::PreCreateWindow(cs))
		return FALSE;
	
	// List view styles
	cs.style &= ~LVS_TYPEMASK;
	cs.style |= LVS_REPORT;
	return TRUE;

}

/////////////////////////////////////////////////////////////////////////////
// CDriveList called when CDriveTree selection changes...(set public)

int CDriveList::Refresh(LPCTSTR pszPath)
{
	HANDLE hFind;
    WIN32_FIND_DATA fd;
    int nCount = 0;

	// Concatenate \ and *.* if necessary
	CString strPath = pszPath;
    if (strPath.Right (1) != _T ("\\"))
        strPath += _T ("\\");
    strPath += _T ("*.*");

    if ((hFind = ::FindFirstFile (strPath, &fd)) != INVALID_HANDLE_VALUE) {

		// Delete existing items (if any).
		//
		GetListCtrl ().DeleteAllItems ();
	
		// Add items representing files to the list view.
		//
        if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
            AddItem (nCount++, &fd);
        while (::FindNextFile (hFind, &fd)) {
            if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
                if (!AddItem (nCount++, &fd))
                    break;
        }
        // Close the open handle...
		::FindClose (hFind);
    }
    return nCount;
}

/////////////////////////////////////////////////////////////////////////////
// CDriveList adds items to the list view, called internally

BOOL CDriveList::AddItem(int nIndex, WIN32_FIND_DATA *pfd)
{
	const int ILI_FILE = 0;	
	CString strTime;
	char szSize[80];
	char szTime[80];
	
	//Get the size of the file...
	wsprintf(szSize, "%d", pfd->nFileSizeLow);
	
	// Get the time the file was modified last and build a suitable string
	// representation...
	FILETIME ft = pfd->ftLastWriteTime;
	CTime time (ft);

	strTime.Format (_T ("%d/%0.2d/%0.2d %d:%0.2d"),
		time.GetMonth (), time.GetDay (), time.GetYear () % 100,
        time.GetHour(), time.GetMinute ());
	::lstrcpy (szTime, strTime);

	// Add the items to the list view
	GetListCtrl().InsertItem(nIndex, pfd->cFileName, ILI_FILE);
	GetListCtrl().SetItemText( nIndex, 1, szSize);
	GetListCtrl().SetItemText(nIndex, 2, szTime);
	
    return TRUE;
}


void CDriveList::OnDestroy() 
{
	CListView::OnDestroy ();
}
