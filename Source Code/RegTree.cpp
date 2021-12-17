// RegTree.cpp : implementation file
//

#include "stdafx.h"
#include "RegKit.h"
#include "RegTree.h"
#include "RegList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define ILI_CLOSED_FOLDER 0
#define ILI_OPEN_FOLDER 1

extern CSplitterWnd *g_wndSplitterY;

/////////////////////////////////////////////////////////////////////////////
// CRegTree

IMPLEMENT_DYNCREATE(CRegTree, CTreeView)

CRegTree::CRegTree()
{
}

CRegTree::~CRegTree()
{
}


BEGIN_MESSAGE_MAP(CRegTree, CTreeView)
	//{{AFX_MSG_MAP(CRegTree)
	ON_NOTIFY_REFLECT(TVN_ITEMEXPANDING, OnItemexpanding)
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, OnSelchanged)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRegTree drawing

void CRegTree::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CRegTree diagnostics

#ifdef _DEBUG
void CRegTree::AssertValid() const
{
	CTreeView::AssertValid();
}

void CRegTree::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CRegTree message handlers

void CRegTree::OnInitialUpdate() 
{
	CTreeView::OnInitialUpdate();

	m_ilRegImages.Create(IDB_REGTREE, 16, 1, RGB (255, 0, 255));
	GetTreeCtrl ().SetImageList (&m_ilRegImages, TVSIL_NORMAL);
	
	// Root items first, with automatic sorting.
	HTREEITEM hKLM = GetTreeCtrl ().InsertItem (_T ("HKEY_LOCAL_MACHINE"),
    ILI_CLOSED_FOLDER, ILI_OPEN_FOLDER, TVI_ROOT, TVI_SORT);
	AddButton(hKLM, HKEY_LOCAL_MACHINE);

	HTREEITEM hKCU = GetTreeCtrl ().InsertItem (_T ("HKEY_CURRENT_USER"),
    ILI_CLOSED_FOLDER, ILI_OPEN_FOLDER, TVI_ROOT, TVI_SORT);
	AddButton(hKCU, HKEY_CURRENT_USER);

	HTREEITEM hKCR = GetTreeCtrl ().InsertItem (_T ("HKEY_CLASSES_ROOT"),
    ILI_CLOSED_FOLDER, ILI_OPEN_FOLDER, TVI_ROOT, TVI_SORT);
	AddButton(hKCR, HKEY_CLASSES_ROOT);

	HTREEITEM hKU = GetTreeCtrl ().InsertItem (_T ("HKEY_USERS"),
    ILI_CLOSED_FOLDER, ILI_OPEN_FOLDER, TVI_ROOT, TVI_SORT);
	AddButton(hKU, HKEY_USERS);

	HTREEITEM hKDD = GetTreeCtrl ().InsertItem (_T ("HKEY_DYN_DATA"),
    ILI_CLOSED_FOLDER, ILI_OPEN_FOLDER, TVI_ROOT, TVI_SORT);
	AddButton(hKDD, HKEY_DYN_DATA);

	HTREEITEM hKCC = GetTreeCtrl ().InsertItem (_T ("HKEY_CURRENT_CONFIG"),
    ILI_CLOSED_FOLDER, ILI_OPEN_FOLDER, TVI_ROOT, TVI_SORT);
	AddButton(hKCC, HKEY_CURRENT_CONFIG);
}

BOOL CRegTree::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	if (!CTreeView::PreCreateWindow(cs))
		return FALSE;

	cs.style &=~FWS_ADDTOTITLE;
	cs.style |= TVS_HASLINES | TVS_LINESATROOT | TVS_SHOWSELALWAYS | TVS_HASBUTTONS;
	return TRUE;
}

void CRegTree::AddButton(HTREEITEM hItem, HKEY hKey)
{
	DWORD dwIndex =0;
	BOOL bSubkey = FALSE;

	while (TRUE) {
		
		LONG lResult;
		TCHAR szSubkey [320];
		DWORD dwSubkeySize = 320;
				
		lResult = RegEnumKeyEx(hKey, dwIndex++, szSubkey, &dwSubkeySize,
			0,0,0,0);

		if (lResult == ERROR_NO_MORE_ITEMS)
			break;

		if (lResult != 0)
			continue;
		
		bSubkey = TRUE;
	}
	if (bSubkey)
		GetTreeCtrl().InsertItem(_T (""), ILI_CLOSED_FOLDER, ILI_CLOSED_FOLDER, hItem);
	return;
}

void CRegTree::OnItemexpanding(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	HTREEITEM hItem = pNMTreeView->itemNew.hItem;
	*pResult = FALSE;

	if (pNMTreeView->action == TVE_EXPAND) {
		
		DeleteFirstItem(hItem);
		CString strPath = GetPathFromItem(hItem);
		if (AddSubkeys(hItem, strPath) == 0)
			*pResult = TRUE;
		CRegList *pView = (CRegList*) g_wndSplitterY->GetPane (1,0);
		pView->Refresh(strPath);
	}
	else {
		CString strPath = GetPathFromItem(hItem);
		HKEY hKey = GetHkey(strPath);
		DeleteAllChildren(hItem);
		AddButton(hItem, hKey);		
	}

	// REFRESH THE REG LIST PANE WITH THE STRING VALUES FROM THE SUBKEY

}

void CRegTree::OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	HTREEITEM hItem = pNMTreeView->itemNew.hItem;

	*pResult = 0;

	// Get the path of the item
	CString strPath = GetPathFromItem(hItem);
	
	// Update the listpane view
	CRegList *pView = (CRegList*) g_wndSplitterY->GetPane(1,0);
	pView->Refresh(strPath);
}

int CRegTree::AddSubkeys(HTREEITEM hItem, LPCTSTR pszPath) {
	
	int nCount =0;
	HKEY hKey = NULL;
	HKEY hSub = NULL;
	DWORD dwIndex = 0;
	
	hKey = GetHkey(pszPath);

	if (strstr(pszPath, "\\") != NULL) {
		while(*pszPath != '\\')
			pszPath++;
		pszPath++;

		if (RegOpenKeyEx(hKey, pszPath,0, KEY_READ, &hSub) != ERROR_SUCCESS) {
			MessageBox("It did not work...");
			return 0;
		}
	}
	else {
		hSub = hKey;
	}
		while (TRUE) {
			TCHAR szSubkey [320];
			DWORD dwSubkeySize = 320;
			HKEY hSubkey;
			LONG lResult;

			lResult = RegEnumKeyEx(hSub, dwIndex++, szSubkey, &dwSubkeySize, 0,0,0,0);

			if (lResult == ERROR_NO_MORE_ITEMS)
				break;
			if (lResult != 0)
				continue;
			HTREEITEM hNew = GetTreeCtrl().InsertItem(szSubkey, 
				ILI_CLOSED_FOLDER,ILI_OPEN_FOLDER, hItem, TVI_SORT);
			lResult = RegOpenKeyEx(hSub, szSubkey, 0, KEY_READ, &hSubkey);
			AddButton(hNew, hSubkey);
			nCount++;
		}
	return nCount;   
}

CString CRegTree::GetPathFromItem(HTREEITEM hItem)
{
	HTREEITEM hParent;
	CString strPath = GetTreeCtrl().GetItemText(hItem);

	while (hParent = GetTreeCtrl().GetParentItem(hItem)) {

		CString strItem = GetTreeCtrl().GetItemText(hParent);
		if (strItem.Right(1) != "\\")
			strItem += _T("\\");
		strPath = strItem + strPath;
		hItem = hParent;
	}
	return strPath;


}

void CRegTree::DeleteFirstItem(HTREEITEM hItem)
{
	HTREEITEM hChildItem = GetTreeCtrl().GetChildItem(hItem);
	if(hChildItem != NULL)
		GetTreeCtrl().DeleteItem(hChildItem);
}

HKEY CRegTree::GetHkey(LPCTSTR pszPath)
{
	HKEY hKey;

	if (strstr (pszPath, "HKEY_LOCAL_MACHINE") != NULL)
		hKey = HKEY_LOCAL_MACHINE;		
	else if (strstr (pszPath, "HKEY_CURRENT_CONFIG") != NULL)
		hKey = HKEY_CURRENT_CONFIG;
	else if (strstr (pszPath, "HKEY_DYN_DATA") != NULL)
		hKey = HKEY_DYN_DATA;
	else if (strstr (pszPath, "HKEY_USERS") != NULL)
		hKey = HKEY_USERS;
	else if (strstr (pszPath, "HKEY_CURRENT_USER") != NULL)
		hKey = HKEY_CURRENT_USER;
	else // (strstr (pszPath, "HKEY_CLASSES_ROOT") != NULL)
		hKey = HKEY_CLASSES_ROOT;

	return hKey;
}

void CRegTree::DeleteAllChildren(HTREEITEM hItem)
{
	HTREEITEM hChildItem;
    
	if ((hChildItem = GetTreeCtrl ().GetChildItem (hItem)) == NULL)
        return;
    do {
        HTREEITEM hNextItem =
            GetTreeCtrl ().GetNextSiblingItem (hChildItem);
        GetTreeCtrl ().DeleteItem (hChildItem);
        hChildItem = hNextItem;
    } while (hChildItem != NULL);
}
