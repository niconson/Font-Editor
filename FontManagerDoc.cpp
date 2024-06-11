// FontManagerDoc.cpp : implementation of the CFontManagerDoc class
//

#include "stdafx.h"
#include "FontManager.h"

#include "FontManagerDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFontManagerDoc

IMPLEMENT_DYNCREATE(CFontManagerDoc, CDocument)

BEGIN_MESSAGE_MAP(CFontManagerDoc, CDocument)
	//{{AFX_MSG_MAP(CFontManagerDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFontManagerDoc construction/destruction

CFontManagerDoc::CFontManagerDoc()
{
	cFontUtil = NULL;
}

CFontManagerDoc::~CFontManagerDoc()
{
	if(cFontUtil) delete cFontUtil;
}

BOOL CFontManagerDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	cFontUtil = new SMFontUtil(FALSE);

	SetTitle("Hershey Font Mgr");

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CFontManagerDoc serialization

void CFontManagerDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CFontManagerDoc diagnostics

#ifdef _DEBUG
void CFontManagerDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CFontManagerDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CFontManagerDoc commands
