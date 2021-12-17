// DriveTree.cpp : implementation file
//

#include "stdafx.h"
#include "RegKit.h"
#include "DriveTree.h"
#include "DriveList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// Image indexes
#define ILI_HARD_DISK 3
#define ILI_FLOPPY 1
#define ILI_CD_ROM 0
#define ILI_NET_DRIVE 2
#define ILI_CLOSED_FOLDER 4
#define ILI_OPEN_FOLDER 5

extern CSplitterWnd *g_wndSplitterY;

/////////////////////////////////////////////////////////////////////////////
// CDriveTree

IMPLEMENT_DYNCREATE(CDriveTree, CTreeView)

CDriveTree::CDriveTree()
{
}

CDriveTree::~CDriveTree()
{
}


BEGIN_MESSAGE_MAP(CDriveTree, CTreeView)
	//{{AFX_MSG_MAP(CDriveTree)
	ON_NOTIFY_REFLECT(TVN_ITEMEXPANDING, OnItemexpanding)
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, OnSelchanged)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDriveTree drawing

BOOL CDriveTree::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	if(!CTreeView::PreCreateWindow(cs))
		return FALSE;
	cs.style |= TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS;
		return TRUE;
}

void CDriveTree::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CDriveTree diagnostics

#ifdef _DEBUG
void CDriveTree::AssertValid() const
{
	CTreeView::AssertValid();
}

void CDriveTree::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDriveTree message handlers

void CDriveTree::OnInitialUpdate() 
{
	CTreeView::OnInitialUpdate();
	
	// Initialise the image list...
	m_ilDrives.Create(IDB_DRIVETREE, 16, 1, RGB (255, 0, 255));
	GetTreeCtrl().SetImageList(&m_ilDrives, TVSIL_NORMAL);
	
	AddDrives();

    // Show the folders on the current drive.
    //
    TCHAR szPath[MAX_PATH];
    ::GetCurrentDirectory (sizeof (szPath) / sizeof (TCHAR), szPath);
    CString strPath = szPath;
    strPath = strPath.Left (3);
	::SetCurrentDirectory(strPath);
    
	HTREEITEM hItem = GetTreeCtrl ().GetNextItem (NULL, TVGN_ROOT);
    while (hItem != NULL) {
        if (GetTreeCtrl ().GetItemText (hItem) == strPath)
            break;
        hItem = GetTreeCtrl ().GetNextSiblingItem (hItem);
    }
    if (hItem != NULL) {
        GetTreeCtrl ().Expand (hItem, TVE_EXPAND);
        GetTreeCtrl ().Select (hItem, TVGN_CARET);
	}	
}

void CDriveTree::AddDrives()
{
	int nPos =0;
	CString string (_T ("?:\\"));

	// Get a bitmask containing all the drives...
	DWORD dwDriveMask = GetLogicalDrives();

	// If a drive is found add
	while (dwDriveMask) {
		if (dwDriveMask & 1) {
			string.SetAt(0, 'A'+ nPos);
			AddDriveItem(string);
		}
		dwDriveMask >>= 1;
		nPos++;
	}
	return;
}


void CDriveTree::AddDriveItem(LPCTSTR pszDrive)
{
	CString string;
	HTREEITEM hItem;

	// Check the drive type, add the item with the appropriate icon and check for
	// child folders...
	UINT nType = ::GetDriveType(pszDrive);

	switch (nType) {
		case DRIVE_REMOVABLE:
			hItem = GetTreeCtrl().InsertItem(pszDrive, ILI_FLOPPY, ILI_FLOPPY);
			SetButtonState (hItem, pszDrive);
			break;
		case DRIVE_FIXED:
		case DRIVE_RAMDISK:
			hItem = GetTreeCtrl ().InsertItem (pszDrive, ILI_HARD_DISK, ILI_HARD_DISK);
			SetButtonState (hItem, pszDrive);
			break;
		case DRIVE_REMOTE:
			hItem = GetTreeCtrl ().InsertItem (pszDrive, ILI_NET_DRIVE,ILI_NET_DRIVE);
			SetButtonState (hItem, pszDrive);
			break;
		case DRIVE_CDROM:
			hItem = GetTreeCtrl ().InsertItem (pszDrive, ILI_CD_ROM, ILI_CD_ROM);
			SetButtonState (hItem, pszDrive);
		break;
		default:
			return;
	}
	return;
}

BOOL CDriveTree::SetButtonState(HTREEITEM hItem, LPCTSTR pszPath)
{
	HANDLE hFind;
    WIN32_FIND_DATA fd;
    BOOL bResult = FALSE;
    CString strPath = pszPath;
    
	// Build the file path, concatenate "\*.*" if necessary and check if there are
	// child folders
	if (strPath.Right (1) != _T ("\\"))
        strPath += _T ("\\");    strPath += _T ("*.*");
    if ((hFind = ::FindFirstFile (strPath, &fd)) == INVALID_HANDLE_VALUE)
        return bResult;
	do {
        if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            CString strComp = (LPCTSTR) &fd.cFileName;
            if ((strComp != _T (".")) && (strComp != _T (".."))) {
                GetTreeCtrl ().InsertItem (_T (""), ILI_CLOSED_FOLDER,
                    ILI_CLOSED_FOLDER, hItem);
                bResult = TRUE;
                break;
		}
        }
    } while (::FindNextFile (hFind, &fd));    ::FindClose (hFind);
    return bResult;
}

CString CDriveTree::GetPathFromItem(HTREEITEM hItem){
    CString strResult = GetTreeCtrl ().GetItemText (hItem);
    HTREEITEM hParent;

    while ((hParent = GetTreeCtrl ().GetParentItem (hItem)) != NULL) {
        CString string = GetTreeCtrl ().GetItemText (hParent);
        if (string.Right (1) != _T ("\\"))
            string += _T ("\\");
        strResult = string + strResult;        
		hItem = hParent;
    }
    return strResult;
}

void CDriveTree::DeleteFirstChild(HTREEITEM hItem){
    
	HTREEITEM hChildItem;
    
	// Delete the empty folder which denotes the parent item has child
	// directories
	if ((hChildItem = GetTreeCtrl ().GetChildItem (hItem)) != NULL)
        GetTreeCtrl ().DeleteItem (hChildItem);
}

void CDriveTree::DeleteAllChildren(HTREEITEM hItem) {

    HTREEITEM hChildItem;
    
	// Delete all child items when a parent collapses
	if ((hChildItem = GetTreeCtrl ().GetChildItem (hItem)) == NULL)
        return;
    do {
        HTREEITEM hNextItem =
            GetTreeCtrl ().GetNextSiblingItem (hChildItem);
        GetTreeCtrl ().DeleteItem (hChildItem);
        hChildItem = hNextItem;
    } while (hChildItem != NULL);
}

int CDriveTree::AddDirectories(HTREEITEM hItem, LPCTSTR pszPath){
    
	HANDLE hFind;
    WIN32_FIND_DATA fd;
    HTREEITEM hNewItem;

    // Build the file path, conncatenate "\*.*" if necessary
	int nCount = 0;
    CString strPath = pszPath;
    if (strPath.Right (1) != _T ("\\"))
        strPath += _T ("\\");
    strPath += _T ("*.*");
    
	// Get handle to child folder and insert item...
	if ((hFind = ::FindFirstFile (strPath, &fd)) == INVALID_HANDLE_VALUE) {
        if (GetTreeCtrl ().GetParentItem (hItem) == NULL)
            GetTreeCtrl ().InsertItem (_T (""), ILI_CLOSED_FOLDER,
                ILI_CLOSED_FOLDER, hItem, TVI_SORT);
        return 0;
    }
	
	// Check for additional folders add to the treeview until there are no more
	// folders
	do {
        if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            
			CString strComp = (LPCTSTR) &fd.cFileName;
            if ((strComp != _T (".")) && (strComp != _T (".."))) {
                hNewItem = GetTreeCtrl ().InsertItem ((LPCTSTR) &fd.cFileName,
                    ILI_CLOSED_FOLDER, ILI_OPEN_FOLDER, hItem, TVI_SORT);
                
				CString strNewPath = pszPath;
                if (strNewPath.Right (1) != _T ("\\"))
                    strNewPath += _T ("\\");
                strNewPath += (LPCTSTR) &fd.cFileName;
                SetButtonState (hNewItem, strNewPath);
                nCount++;
            }
        }
    } while (::FindNextFile (hFind, &fd));
    ::FindClose (hFind);
    return nCount;
}

void CDriveTree::OnItemexpanding(NMHDR* pNMHDR, LRESULT* pResult) 
{
	
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
    HTREEITEM hItem = pNMTreeView->itemNew.hItem;
	
	// Get dir path of expanding or collapsing branch
	CString string = GetPathFromItem (hItem);
	
	*pResult = FALSE;
    
	// If the tree is going to expand...
	if (pNMTreeView->action == TVE_EXPAND) {
		
		// Get rid of the empty folder which denotes there are children
		DeleteFirstChild (hItem);
	
		// Add the directories for the current dir...
		if (AddDirectories (hItem, string) == 0)
			*pResult = TRUE;
    }
	else { // pNMTreeView->action == TVE_COLLAPSE
        
		// Free up memory (delete child items of collapsing folder)
		DeleteAllChildren (hItem);
        
		// Reset the icon (to close folder) and add check for children (add box)
		if (GetTreeCtrl ().GetParentItem (hItem) == NULL)
            GetTreeCtrl ().InsertItem (_T (""), ILI_CLOSED_FOLDER, ILI_CLOSED_FOLDER, hItem);
        else
            SetButtonState (hItem, string);
	}
	
	// Get a pointer to the CDriveList class so we can play with it...
	//
	CDriveList *pView = (CDriveList*) g_wndSplitterY->GetPane(1,0);

	// Load the list box with files from the set path...
	pView->Refresh(string);
	
}
void CDriveTree::OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	
	// Get the handle to the newly selected item
	HTREEITEM hItem = pNMTreeView->itemNew.hItem;
	
	// Get the path to work with
	CString string = GetPathFromItem (hItem);
		
	*pResult = 0;

	// Get a pointer to the runtime class and re-load the list view with items
	CDriveList *pView = (CDriveList*) g_wndSplitterY->GetPane(1,0);
	pView->Refresh(string);
}
