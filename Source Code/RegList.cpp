// RegList.cpp : implementation file
//

#include "stdafx.h"
#include "regkit.h"
#include "RegList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRegList

IMPLEMENT_DYNCREATE(CRegList, CListView)

CRegList::CRegList()
{
}

CRegList::~CRegList()
{
}


BEGIN_MESSAGE_MAP(CRegList, CListView)
	//{{AFX_MSG_MAP(CRegList)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRegList drawing

void CRegList::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CRegList diagnostics

#ifdef _DEBUG
void CRegList::AssertValid() const
{
	CListView::AssertValid();
}

void CRegList::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CRegList message handlers

void CRegList::OnInitialUpdate() 
{
	RECT rect;
	int nWidth;
	CListView::OnInitialUpdate();

	CBitmap bit;
	bit.LoadBitmap(IDB_REGDWORD);
		
	GetClientRect(&rect);
	nWidth = rect.right/2;
	
	GetListCtrl().SetColumnWidth(0,nWidth);
	GetListCtrl().SetColumnWidth(1,nWidth);

	m_ilSmall.Create(IDB_REGLIST, 16, 1, RGB (255,0,255));
	m_ilSmall.Add(&bit, RGB (255,0,255));
	GetListCtrl().SetImageList(&m_ilSmall, LVSIL_SMALL);

	GetListCtrl().InsertColumn(0, _T ("Name"), LVCFMT_LEFT, nWidth);
	GetListCtrl().InsertColumn(1, _T ("Data"), LVCFMT_LEFT, nWidth);

	GetListCtrl().InsertItem(0, "(Default Value)", 0);
	GetListCtrl().SetItemText(0,1,"( value not set. )");
	
}

BOOL CRegList::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	if (!CListView::PreCreateWindow(cs))
		return FALSE;

	cs.style &= ~LVS_TYPEMASK;
	cs.style |= LVS_REPORT;
	return TRUE;
}

void CRegList::Refresh(LPCTSTR pszPath)
{
	int nIndex = 0;
	HKEY hKey = NULL;
	HKEY hSub = NULL;
	DWORD dwIndex = 0;
	
	// Kill all items in the list control
	GetListCtrl().DeleteAllItems();

	// Find out which root branch we are in and set the appropriate registry
	// key value
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
	else if (strstr (pszPath, "HKEY_CLASSES_ROOT") != NULL)
		hKey = HKEY_CLASSES_ROOT;
	
	// If we are in a subdirectory get to the end of the path so we can open the
	// correct reg branch
	if (strstr(pszPath, "\\") != NULL) {
		while(*pszPath != '\\')
			pszPath++;
		pszPath++;

		// Open the current branch and get the registry key for it
		if (RegOpenKeyEx(hKey, pszPath,0, KEY_READ, &hSub) != ERROR_SUCCESS) {
			return;
		}
	}
	
	// If we are starting from scratch use the appropriate root key
	else {
		hSub = hKey;
	}
	
	// We should always have default values, redo the value when necessary
	GetListCtrl().InsertItem(nIndex, "(Default Value)", 0);
	GetListCtrl().SetItemText(nIndex, 1, "( value not set. )");
	nIndex++;

	// Enumerate all of the subkey values
	while (TRUE) {
		TCHAR szValueName [320];
		DWORD dwValueNameSize = 320;
		TCHAR szValue [320];
		DWORD dwValueSize = 320;
		TCHAR szValDisp[320];
		DWORD dwType;
		LONG lResult;

		lResult = RegEnumValue(hSub, dwIndex++, szValueName, &dwValueNameSize,
			0, &dwType, (LPBYTE) szValue, &dwValueSize);
		
		strcat(szValueName, "\"");
		strcat(szValue, "\"");

		// There are no other items in the list, quit and return control back to
		// the GUI
		if (lResult == ERROR_NO_MORE_ITEMS) {
			return;
		}
		else {
			// Set a value for default value if we need to..
			if (strcmp(szValueName,"\"") == 0) {
				GetListCtrl().DeleteItem(0);
				GetListCtrl().InsertItem(0, "(Default Value)", 0);		
				strcpy(szValDisp, "\"");
				strcat(szValDisp, szValue);
				GetListCtrl().SetItemText(0, 1, szValDisp);
			}			
			// If this is not a string value, apologise to the user that I cant
			// be bothered extracting the values and display a polite excuse
			else {
				if (dwType == REG_SZ) {
					strcpy(szValDisp, "\"");
					strcat(szValDisp, szValue);
					szValueName[strlen(szValueName)-1] = '\0';
					GetListCtrl().InsertItem(nIndex, szValueName, 0);
				}
				else if (dwType == REG_BINARY) {
					szValueName[strlen(szValueName)-1] = '\0';
					strcpy(szValDisp,"Binary values not supported...");
					GetListCtrl().InsertItem(nIndex, szValueName, 1);
				}
				else if (dwType == REG_DWORD) {
					szValueName[strlen(szValueName)-1] = '\0';
					strcpy(szValDisp,"DWORD values not supported...");
					GetListCtrl().InsertItem(nIndex, szValueName, 1);
				}
				else if (dwType == REG_DWORD_BIG_ENDIAN) {
					szValueName[strlen(szValueName)-1] = '\0';
					strcpy(szValDisp,"Big Endian DWORD values not supported...");
					GetListCtrl().InsertItem(nIndex, szValueName, 1);
				}
				else if (dwType == REG_EXPAND_SZ) {
					strcpy(szValDisp, "\"");
					strcat(szValDisp, szValue);
					szValueName[strlen(szValueName)-1] = '\0';
					GetListCtrl().InsertItem(nIndex, szValueName, 0);
				}
				else if (dwType == REG_DWORD_BIG_ENDIAN) {
					szValueName[strlen(szValueName)-1] = '\0';
					strcpy(szValDisp,"UNICODE values not supported...");
					GetListCtrl().InsertItem(nIndex, szValueName, 1);
				}
				else if (dwType == REG_MULTI_SZ) {
					szValueName[strlen(szValueName)-1] = '\0';
					strcpy(szValDisp,"Multi-terminated values not supported...");
					GetListCtrl().InsertItem(nIndex, szValueName, 0);
				}
				else if (dwType == REG_RESOURCE_LIST) {
					szValueName[strlen(szValueName)-1] = '\0';
					strcpy(szValDisp,"Multi-terminated values not supported...");
					GetListCtrl().InsertItem(nIndex, szValueName, 1);
				}
				
				else {
					szValueName[strlen(szValueName)-1] = '\0';
					strcpy(szValDisp,"Unknown values not supported...");
					GetListCtrl().InsertItem(nIndex, szValueName, 1);
				}
				
				// Add to the list
				GetListCtrl().SetItemText(nIndex, 1, szValDisp);
				nIndex++;
			}
		}				
	}
	return;   
}
