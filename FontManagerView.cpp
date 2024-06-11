// FontManagerView.cpp : implementation of the CFontManagerView class
//

#include "stdafx.h"
#include "FontManager.h"

#include "FontManagerDoc.h"
#include "FontManagerView.h"
#include "CharEditDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#define NUM_FONTS 12
/////////////////////////////////////////////////////////////////////////////
// CFontManagerView

IMPLEMENT_DYNCREATE(CFontManagerView, CScrollView)

BEGIN_MESSAGE_MAP(CFontManagerView, CScrollView)
	//{{AFX_MSG_MAP(CFontManagerView)
	ON_WM_LBUTTONUP()
	ON_COMMAND(IDM_XLATED_VIEW, OnXlatedView)
	ON_UPDATE_COMMAND_UI(IDM_XLATED_VIEW, OnUpdateXlatedView)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_COMMAND(ID_FILE_SAVE, OnFileSave)
	ON_WM_RBUTTONUP()
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateEditCopy)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, OnUpdateEditPaste)
	ON_COMMAND(IDM_ASSIGN, OnAssign)
	ON_UPDATE_COMMAND_UI(IDM_ASSIGN, OnUpdateAssign)
	ON_COMMAND(IDM_INDEX, OnIndex)
	ON_UPDATE_COMMAND_UI(IDM_INDEX, OnUpdateIndex)
	ON_COMMAND(IDM_CLEAR, OnClear)
	ON_UPDATE_COMMAND_UI(IDM_CLEAR, OnUpdateClear)
	ON_COMMAND(IDM_WHO_USES, OnWhoUses)
	ON_UPDATE_COMMAND_UI(IDM_WHO_USES, OnUpdateWhoUses)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CScrollView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFontManagerView construction/destruction

CFontManagerView::CFontManagerView()
{
	cDisplayMode = FD_NORMAL;
	cCurSel = 0;
	cCurIndex = 0;
	cCurRow = 0;
	cCurCol = 0;
	cCopyRow = 0;
}

CFontManagerView::~CFontManagerView()
{
}

BOOL CFontManagerView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CScrollView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CFontManagerView drawing

void CFontManagerView::OnDraw(CDC* pDC)
{
	CFontManagerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if(!(pDoc->cFontUtil)) return;

	SMCharacter::CHARVERTEX_TYPE result;
	int itter;
	int charidx;
	long x,y;
	long dx,dy;
	SMCharacter * character;
	CharVertex vertex;
	int i,j,k,l,m;
	CRect textrect;
	char charid[64];

	switch(cDisplayMode)
	{
	
	case FD_NORMAL:

		if(pDC->IsPrinting())
		{
			CSize viewport = pDC->GetViewportExt();
			dx = 250;
			dy = 200;
			j=0;
			k=0;
			l=((cCurrentPage-1) * 110);
		
			while(k<11)
			{
				for(i=l; i<min(l+110,pDoc->cFontUtil->GetMaxChar()+1); i++)
				{
					character = pDoc->cFontUtil->GetCharacter(i);
					result = character->GetFirstVertex(vertex,itter);
					while(result != SMCharacter::TERMINATE)
					{
						x =(long)(vertex.X) * 10; 
						y =(long)(vertex.Y) * 10; 
						if(result == SMCharacter::MOVE_TO)
						{
							pDC->MoveTo(dx+x,dy+y);
						}
						else
						{
							pDC->LineTo(dx+x,dy+y);
						}		
						result = character->GetNextVertex(vertex,itter);
					}
					itoa(i,charid,10);
					pDC->TextOut(dx-150,dy+250,charid);
					j++;
					dx+=400;

					if(j>=10)
					{
						dx = 250;
						dy += 570;
						j=0;
						k++;
					}
				}
			}
		}
		else
		{
			dx = 25;
			dy = 20;
			pDC->SetBkMode(TRANSPARENT);
			for(i=0; i<=pDoc->cFontUtil->GetMaxChar(); i++)
			{
				textrect.left = dx - 17;
				textrect.right = dx + 20;
				textrect.top = dy - 20;
				textrect.bottom = dy + 40;
				if(pDC->RectVisible(textrect))
				{
					if(cCurSel == i)
					{
						pDC->FillSolidRect(textrect,RGB(195,195,10));
					}
					character = pDoc->cFontUtil->GetCharacter(i);
					result = character->GetFirstVertex(vertex,itter);
					while(result != SMCharacter::TERMINATE)
					{
						x =(long)(vertex.X); 
						y =(long)(vertex.Y); 
						if(result == SMCharacter::MOVE_TO)
						{
							pDC->MoveTo(dx+x,dy+y);
						}
						else
						{
							pDC->LineTo(dx+x,dy+y);
						}		
						result = character->GetNextVertex(vertex,itter);
					}
					itoa(i,charid,10);
					pDC->TextOut(dx-15,dy+25,charid);
				}
				dx+=40;
				if(dx>600)
				{
					dx = 25;
					dy += 57;
				}
			}
		}
		break;

	case FD_TABLE:
		dy = 20;
		pDC->SetBkMode(TRANSPARENT);
		for(i=0; i<256; i++)
		{
			if(cCurRow == i)
			{
				CRect client;
				GetClientRect(client);
				client.top = dy - 20;
				client.bottom = dy + 40;
				pDC->FillSolidRect(client,RGB(195,195,10));
			}
			dx = 125;
			for(m = 0; m < NUM_FONTS; m++)
			{
				textrect.left = dx - 17;
				textrect.right = dx + 20;
				textrect.top = dy - 20;
				textrect.bottom = dy + 40;
				if((cCurRow == i) && (cCurCol == m))
				{
					pDC->FillSolidRect(textrect,RGB(50,195,195));
				}
				if(pDC->RectVisible(textrect))
				{
					if(m == 0)
					{
						CString chr = char(i);
						pDC->TextOut(10,dy-7,chr);
						chr.Format("%d",i);
						pDC->TextOut(10,dy+6,chr);
					}
					charidx = pDoc->cFontUtil->GetCharID((unsigned char)i, (FONT_TYPE)m);
					if(charidx >= 0)
					{
						character = pDoc->cFontUtil->GetCharacter(charidx);
						result = character->GetFirstVertex(vertex,itter);
						while(result != SMCharacter::TERMINATE)
						{
							x =(long)(vertex.X); 
							y =(long)(vertex.Y); 
							if(result == SMCharacter::MOVE_TO)
							{
								pDC->MoveTo(dx+x,dy+y);
							}
							else
							{
								pDC->LineTo(dx+x,dy+y);
							}		
							result = character->GetNextVertex(vertex,itter);
						}
						itoa(charidx,charid,10);
						pDC->TextOut(dx-15,dy+25,charid);
					}
					else
					{
						textrect.DeflateRect(10,10);
						pDC->FillSolidRect(textrect,RGB(205,205,205));
					}
				}
				dx+=40;
			}
			dy += 57;
		}
		break;

	default:
		break;
	}
}

void CFontManagerView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();
	CSize sizeTotal;
	sizeTotal.cx = 600;
	sizeTotal.cy = 14600;
	SetScrollSizes(MM_TEXT, sizeTotal);
	ShowScrollBar( SB_BOTH, TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// CFontManagerView printing

BOOL CFontManagerView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CFontManagerView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CFontManagerView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CFontManagerView diagnostics

#ifdef _DEBUG
void CFontManagerView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CFontManagerView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CFontManagerDoc* CFontManagerView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CFontManagerDoc)));
	return (CFontManagerDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CFontManagerView message handlers

void CFontManagerView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	CFontManagerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	long dx,dy;
	CRect textrect;
	int i;

	CPoint scroll = GetScrollPosition();
	point += scroll;

	switch(cDisplayMode)
	{
	
	case FD_NORMAL:

		dx = 25;
		dy = 20;
		for(i=0; i<=pDoc->cFontUtil->GetMaxChar(); i++)
		{
			textrect.left = dx - 20;
			textrect.right = dx + 20;
			textrect.top = dy - 20;
			textrect.bottom = dy + 40;
			
			if(textrect.PtInRect(point))
			{
				cCurSel = i;
				Invalidate();
				break;
			}

			dx+=40;
			if(dx>600)
			{
				dx = 25;
				dy += 57;
			}
		}
		break;

	case FD_TABLE:
		{
			cCurRow = point.y / 57;
			int font = (point.x-110) / 40;
			if(font > 11) font = 11;
			if(font < 0) font = 0;
			cCurCol = font;
			Invalidate();
		}
		break;

	default:
		break;
	}
}

void CFontManagerView::OnXlatedView() 
{
	static int scroll_1=0;
	static int scroll_2=0;
	//
	if(cDisplayMode == FD_NORMAL)
	{
		scroll_1 = GetScrollPos(SB_VERT);
		SetScrollPos(SB_VERT,scroll_2);
		cDisplayMode = FD_TABLE;
	}
	else
	{
		scroll_2 = GetScrollPos(SB_VERT);
		SetScrollPos(SB_VERT,scroll_1);
		cDisplayMode = FD_NORMAL;
	}
	Invalidate();
}

void CFontManagerView::OnUpdateXlatedView(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(!(cDisplayMode == FD_NORMAL));
}

void CFontManagerView::OnEditCopy() 
{
	if( cDisplayMode == FD_TABLE )
		cCopyRow = cCurRow;
	else
		cCurIndex = cCurSel;
}

void CFontManagerView::OnEditPaste() 
{
	CFontManagerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if(!(pDoc->cFontUtil)) return;

	SMCharacter * character;
	SMCharacter * newcharacter;

	if(cDisplayMode == FD_TABLE)
	{
		int i;
		int charidx;
		int newidx;
		for(i = 0; i < NUM_FONTS; i++)
		{
			charidx = pDoc->cFontUtil->GetCharID((unsigned char)cCopyRow, (FONT_TYPE)i);
			if(charidx >= 0)
			{
				character = pDoc->cFontUtil->GetCharacter(charidx);
				newcharacter = new SMCharacter(character);
				newidx = pDoc->cFontUtil->AddCharacter(newcharacter);
				pDoc->cFontUtil->SetCharID((unsigned char)cCurRow, (FONT_TYPE)i, newidx);
			}
		}
		Invalidate();
	}
	else
	{
		delete pDoc->cFontUtil->SMCharList[cCurSel]->cVertex;
		pDoc->cFontUtil->SMCharList[cCurSel]->cVertex = new CharVertex[ pDoc->cFontUtil->SMCharList[cCurIndex]->cVertexCount ];
		pDoc->cFontUtil->SMCharList[cCurSel]->cVertexCount = pDoc->cFontUtil->SMCharList[cCurIndex]->cVertexCount;
		for( int i=0; i<pDoc->cFontUtil->SMCharList[cCurSel]->cVertexCount; i++ )
		{
			pDoc->cFontUtil->SMCharList[cCurSel]->cVertex[i] = pDoc->cFontUtil->SMCharList[cCurIndex]->cVertex[i];
		}
		Invalidate();
	}
}

void CFontManagerView::OnFileSave() 
{
	CWaitCursor wait;
	CFontManagerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if(!(pDoc->cFontUtil)) 
		return;
	pDoc->cFontUtil->SaveFontData();
	pDoc->cFontUtil->SaveXlationData();
//	MessageBox("Save complete","DONE",MB_OK);
}

void CFontManagerView::OnRButtonUp(UINT nFlags, CPoint point) 
{
	CFontManagerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	long dx,dy;
	CRect textrect;
	int i;

	CPoint scroll = GetScrollPosition();
	point += scroll;

	switch(cDisplayMode)
	{
	
	case FD_NORMAL:

		dx = 25;
		dy = 20;
		for(i=0; i<=pDoc->cFontUtil->GetMaxChar(); i++)
		{
			textrect.left = dx - 20;
			textrect.right = dx + 20;
			textrect.top = dy - 20;
			textrect.bottom = dy + 40;
			
			if(textrect.PtInRect(point))
			{
				cCurSel = i;
				CCharEditDlg dlg;
				dlg.pChar = pDoc->cFontUtil->GetCharacter(cCurSel);
				dlg.DoModal();
				Invalidate();
				break;
			}

			dx+=40;
			if(dx>600)
			{
				dx = 25;
				dy += 57;
			}
		}
		break;

	case FD_TABLE:
		{
			cCurRow = point.y / 57;
			int font = (point.x-110) / 40;
			if(font > 11) font = 11;
			if(font < 0) font = 0;
			cCurCol = font;
			int charidx;
			charidx = pDoc->cFontUtil->GetCharID((unsigned char)cCurRow, (FONT_TYPE)font);
			if( charidx >= 0 )
			{
				CCharEditDlg dlg;
				dlg.pChar = pDoc->cFontUtil->GetCharacter(charidx);
				dlg.DoModal();
			}
			Invalidate();
		}
		break;

	default:
		break;
	}
}

void CFontManagerView::OnUpdateEditCopy(CCmdUI* pCmdUI) 
{
	//pCmdUI->Enable(cDisplayMode == FD_TABLE);
}

void CFontManagerView::OnUpdateEditPaste(CCmdUI* pCmdUI) 
{
	//OnUpdateEditCopy(pCmdUI); 
}

void CFontManagerView::OnAssign() 
{
	CFontManagerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if(!(pDoc->cFontUtil)) return;
	pDoc->cFontUtil->SetCharID((unsigned char)cCurRow, 
		(FONT_TYPE)cCurCol, cCurIndex);
	Invalidate();
}

void CFontManagerView::OnUpdateAssign(CCmdUI* pCmdUI) 
{
	OnUpdateEditCopy(pCmdUI); 
}

void CFontManagerView::OnIndex() 
{
	cCurIndex = cCurSel;
}

void CFontManagerView::OnUpdateIndex(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(cDisplayMode == FD_NORMAL);
}

void CFontManagerView::OnClear() 
{
	CFontManagerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if(!(pDoc->cFontUtil)) return;
	pDoc->cFontUtil->SetCharID((unsigned char)cCurRow, 
		(FONT_TYPE)cCurCol, -1);
	Invalidate();
}

void CFontManagerView::OnUpdateClear(CCmdUI* pCmdUI) 
{
	OnUpdateAssign(pCmdUI); 
}

void CFontManagerView::OnWhoUses() 
{
	CFontManagerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if(!(pDoc->cFontUtil)) return;
	CString temp;
	CString users = "Used by:";
	BOOL none = TRUE;
	int font;
	int chr;
	for(font = 0; font < NUM_FONTS; font++)
	{
		for(chr = 0; chr < 256; chr++)
		{
			if(cCurSel == pDoc->cFontUtil->
				GetCharID((unsigned char)chr,(FONT_TYPE)font))
			{
				if(!none) users += "  &  ";
				temp.Format(" F:%d-C:%d",font,chr);
				users += temp;
				none = FALSE;
			}
		}
	}
	if(none) users += " NONE";
	MessageBox(users,"USERS",MB_OK | MB_ICONINFORMATION);
	
}

void CFontManagerView::OnUpdateWhoUses(CCmdUI* pCmdUI) 
{
	OnUpdateIndex(pCmdUI); 
}
