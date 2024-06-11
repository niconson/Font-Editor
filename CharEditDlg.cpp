// CharEditDlg.cpp : implementation file
//

#include "stdafx.h"
#include "FontManager.h"
#include "CharEditDlg.h"
#include <math.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CCharEditDlg dialog


CCharEditDlg::CCharEditDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCharEditDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCharEditDlg)
	//}}AFX_DATA_INIT
	pChar = NULL;
	pRevert = NULL;
	cLeftDown = FALSE;
	cRightDown = FALSE;
	cDrawing = FALSE;
	cDragging = -1;
}


void CCharEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCharEditDlg)
	DDX_Control(pDX, IDC_KILL_LINE, m_KillLine);
	DDX_Control(pDX, IDC_DRAG_LINE, m_DragLine);
	DDX_Control(pDX, IDC_KILL, m_Kill);
	DDX_Control(pDX, IDC_DRAW, m_Draw);
	DDX_Control(pDX, IDC_DRAG, m_Drag);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCharEditDlg, CDialog)
	//{{AFX_MSG_MAP(CCharEditDlg)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_DRAW, OnDraw)
	ON_BN_CLICKED(IDC_DRAG, OnDrag)
	ON_BN_CLICKED(IDC_KILL, OnKill)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_BN_CLICKED(IDC_DRAG_LINE, OnDragLine)
	ON_BN_CLICKED(IDC_KILL_LINE, OnKillLine)
	ON_BN_CLICKED(IDC_REVERT, OnRevert)
	ON_BN_CLICKED(IDC_UNDO, OnUndo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCharEditDlg message handlers

void CCharEditDlg::Stroke(void) 
{
	CClientDC dc(this);
	CPen * oldpen;
	CPen pen;
	pen.CreatePen(PS_SOLID,1,RGB(0,0,0));
	oldpen = dc.SelectObject(&pen);
	dc.SetROP2(R2_NOT);
	switch(cMode)
	{
	case DRAW:
		{
			dc.MoveTo(cPointData[0]);
			int loop;
			for(loop=1; loop < cPoints; loop++)
			{
				dc.LineTo(cPointData[loop]);
			}
		}
		break;
	case DRAG:
	case DRAG_LINE:
		{
			SMCharacter::CHARVERTEX_TYPE result;
			CharVertex vertex;
			long x,y;
			long dx,dy;
			int itter;
			int mult;
			CRect client;
			GetClientRect(client);
			dx = client.Width()/2;
			dy = client.Height()/2;
			mult = 8;
			result = pChar->GetFirstVertex(vertex,itter);
			while(result != SMCharacter::TERMINATE)
			{
				x =(long)(vertex.X) * mult; 
				y =(long)(vertex.Y) * mult; 
				if(result == SMCharacter::MOVE_TO)
				{
					dc.MoveTo(dx+x,dy+y);
				}
				else
				{
					dc.LineTo(dx+x,dy+y);
				}
				result = pChar->GetNextVertex(vertex,itter);
			}
		}
		break;
	default:
		ASSERT(FALSE);
		break;
	}
	dc.SelectObject(oldpen);
	pen.DeleteObject();

}


CPoint CCharEditDlg::Snap(CPoint pPoint, BOOL pHersheyUnits) 
{
	long dx,dy;
	double mult;
	CRect client;
	GetClientRect(client);
	dx = client.Width()/2;
	dy = client.Height()/2;
	mult = 8.0;

	pPoint.x -= dx;
	pPoint.y -= dy;

	double fx = ((double)pPoint.x) / mult;
	double ceilx = ceil(fx);
	double floorx = floor(fx);
	if(fabs(fx - ceilx) < fabs(fx - floorx))
		fx = ceilx;
	else
		fx = floorx;

	double fy = ((double)pPoint.y) / mult;
	double ceily = ceil(fy);
	double floory = floor(fy);
	if(fabs(fy - ceily) < fabs(fy - floory))
		fy = ceily;
	else
		fy = floory;
	
	CPoint result;

	if(pHersheyUnits)
	{
		result.x = (long)fx;
		result.y = (long)fy;
	}
	else
	{
		result.x = ((long)(fx * mult)) + dx;
		result.y = ((long)(fy * mult)) + dy;
	}
	return result;
}

void CCharEditDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	static int pDx=0;
	static int pDy=0;
	SMCharacter::CHARVERTEX_TYPE result;
	CharVertex vertex;
	long x,y;
	long dx,dy;
	int itter;
	int mult;

	CRect client;
	GetClientRect(client);
	dx = client.Width()/2;
	dy = client.Height()/2;

	mult = 8;

	for(x = -63; x < 64; x++)
	{
		for(y = -63; y < 64; y++)
		{
			dc.SetPixel(dx + x * mult, dy + y * mult, RGB(0,0,255)); 
			if( x%5 == 0 && y%5 == 0 )
			{
				//dc.MoveTo(dx + (x*mult) - 2, dy + (y*mult));
				//dc.LineTo(dx + (x*mult) + 2, dy + (y*mult));
				//dc.MoveTo(dx + (x*mult), dy + (y*mult) - 2);
				//dc.LineTo(dx + (x*mult), dy + (y*mult) + 2);
				dc.Rectangle( dx + (x*mult) - 1, dy + (y*mult) - 1, dx + (x*mult) + 1, dy + (y*mult) + 1 );
			}
		}
	}

	CPen * oldpen;
	CPen pen;
	pen.CreatePen(PS_SOLID,1,RGB(200,100,0));
	oldpen = dc.SelectObject(&pen);
	dc.MoveTo(0,dy);
	dc.LineTo(client.right,dy);
	dc.MoveTo(dx,0);
	dc.LineTo(dx,client.bottom);
	dc.SelectObject(oldpen);
	pen.DeleteObject();

	result = pChar->GetFirstVertex(vertex,itter);
	while(result != SMCharacter::TERMINATE)
	{
		x =(long)(vertex.X) * mult; 
		y =(long)(vertex.Y) * mult; 
		if(result == SMCharacter::MOVE_TO)
		{
			dc.MoveTo(dx+x,dy+y);
		}
		else
		{
			dc.LineTo(dx+x,dy+y);
		}
		CRect pnt(dx+x-2,dy+y-2,dx+x+2,dy+y+2);
		dc.FillSolidRect(pnt,RGB(0,100,0));
		result = pChar->GetNextVertex(vertex,itter);
	}
	if(pDx != dx || pDy != dy)
		Invalidate();
	pDx = dx;
	pDy = dy;
}

BOOL CCharEditDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	OnDraw();
	pUndo = new SMCharacter;
	pRevert = new SMCharacter;
	if( pChar->cVertexCount == 0 )
	{
		pChar->cVertex = new CharVertex[1];
		pChar->cVertex[0].X = -64;
		pChar->cVertex[0].Y = -64;
		pChar->cVertexCount = 1;
	}
	pRevert->cVertexCount = pChar->cVertexCount;
	if( pRevert->cVertexCount )
		pRevert->cVertex = new CharVertex[pRevert->cVertexCount];
	int loop;
	for(loop = 0; loop < pRevert->cVertexCount; loop++)
		pRevert->cVertex[loop] = pChar->cVertex[loop];
	SnapForUndo();
	return TRUE;
}

void CCharEditDlg::SnapForUndo() 
{
	if( pUndo->cVertex )
		delete pUndo->cVertex;
	pUndo->cVertexCount = pChar->cVertexCount;
	if( pUndo->cVertexCount )
		pUndo->cVertex = new CharVertex[pUndo->cVertexCount];
	int loop;
	for(loop = 0; loop < pUndo->cVertexCount; loop++)
		pUndo->cVertex[loop] = pChar->cVertex[loop];
}

void CCharEditDlg::OnDraw() 
{
	m_Draw.SetCheck(1);
	m_Drag.SetCheck(0);
	m_Kill.SetCheck(0);
	m_DragLine.SetCheck(0);
	m_KillLine.SetCheck(0);
	cMode = DRAW;
	cLeftDown = FALSE;
	cRightDown = FALSE;
	cDrawing = FALSE;
	cDragging = -1;
	Invalidate(FALSE);
}

void CCharEditDlg::OnDrag() 
{
	m_Draw.SetCheck(0);
	m_Drag.SetCheck(1);
	m_Kill.SetCheck(0);
	m_DragLine.SetCheck(0);
	m_KillLine.SetCheck(0);
	cMode = DRAG;
	cLeftDown = FALSE;
	cRightDown = FALSE;
	cDrawing = FALSE;
	cDragging = -1;
	Invalidate(FALSE);
}

void CCharEditDlg::OnKill() 
{
	m_Draw.SetCheck(0);
	m_Drag.SetCheck(0);
	m_Kill.SetCheck(1);
	m_DragLine.SetCheck(0);
	m_KillLine.SetCheck(0);
	cMode = KILL;
	cLeftDown = FALSE;
	cRightDown = FALSE;
	cDrawing = FALSE;
	cDragging = -1;
	Invalidate(FALSE);
}

void CCharEditDlg::OnDragLine() 
{
	m_Draw.SetCheck(0);
	m_Drag.SetCheck(0);
	m_Kill.SetCheck(0);
	m_DragLine.SetCheck(1);
	m_KillLine.SetCheck(0);
	cMode = DRAG_LINE;
	cLeftDown = FALSE;
	cRightDown = FALSE;
	cDrawing = FALSE;
	cDragging = -1;
	Invalidate(FALSE);
}

void CCharEditDlg::OnKillLine() 
{
	m_Draw.SetCheck(0);
	m_Drag.SetCheck(0);
	m_Kill.SetCheck(0);
	m_DragLine.SetCheck(0);
	m_KillLine.SetCheck(1);
	cMode = KILL_LINE;
	cLeftDown = FALSE;
	cRightDown = FALSE;
	cDrawing = FALSE;
	cDragging = -1;
	Invalidate(FALSE);
}

void CCharEditDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	switch(cMode)
	{
	case DRAW:
		if(!cDrawing)
		{
			SnapForUndo();
			cDrawing = TRUE;
			cPoints = 2;
			cPointData[0] = Snap(point);
			cPointData[1] = point;
			Stroke();
		}
		else
		{
			Stroke();
			cPointData[cPoints - 1] = Snap(point);
			cPoints++;
			cPointData[cPoints - 1] = point;
			Stroke();
		}
		break;
	case DRAG: //fall through
	case KILL: //fall through
	case KILL_LINE: //fall through
	case DRAG_LINE:
		{
			SnapForUndo();
			SMCharacter::CHARVERTEX_TYPE result;
			CharVertex vertex;
			int bestiter = -1;
			int iter;
			double dist;
			double bestdist = 99999999999.0;
			CPoint pPnt = Snap(point,TRUE);
			result = pChar->GetFirstVertex(vertex,iter);
			while(result != SMCharacter::TERMINATE)
			{
				dist = sqrt(
					pow(((double)(vertex.X-pPnt.x)),2)+ 
					pow(((double)(vertex.Y-pPnt.y)),2)); 
				if(dist < 40)
				{
					if(dist < bestdist)
					{
						bestdist = dist;
						bestiter = iter;
					}
				}
				result = pChar->GetNextVertex(vertex,iter);
			}
			cDragging = bestiter;
			if(cDragging >= 0)
			{
				if(cMode == KILL)
				{
					BOOL legal = TRUE;
					if((cDragging == 1) || (pChar->cVertex[cDragging-1].X < -63.5))
					{
						if(pChar->cVertex[cDragging+2].X < -63.5)
						{
							legal = FALSE;
						}
					}
					if(pChar->cVertex[cDragging+1].X < -63.5)
					{
						if(cDragging < 3)
						{
							legal = FALSE;
						}
						else if(pChar->cVertex[cDragging-2].X < -63.5)
						{
							legal = FALSE;
						}
					}
					if(legal)
					{
						pChar->cVertexCount--;
						int loop;
						for(loop = cDragging; loop < pChar->cVertexCount; loop++)
						{
							pChar->cVertex[loop] = pChar->cVertex[loop+1];
						}
						Invalidate();
					}
					else
					{
						MessageBox("Not a legal vertex for deletion.\nLine would have too few vertices.",
							"BAD VERT",MB_OK | MB_ICONEXCLAMATION);
					}
				}
				else if(cMode == KILL_LINE)
				{
					int index = cDragging;
					int loop;
					while((index > 0) && (pChar->cVertex[index].X > -63.5)) index--;
					pChar->cVertexCount--;
					for(loop = index; loop < pChar->cVertexCount; loop++)
					{
						pChar->cVertex[loop] = pChar->cVertex[loop+1];
					}
					while(pChar->cVertex[index].X > -63.5)
					{
						pChar->cVertexCount--;
						for(loop = index; loop < pChar->cVertexCount; loop++)
						{
							pChar->cVertex[loop] = pChar->cVertex[loop+1];
						}
					}
					Invalidate();
				}
				else
				{
					Stroke();
				}
			}
		}
		break;
	default:
		ASSERT(FALSE);
		break;
	}
	CDialog::OnLButtonDown(nFlags, point);
	cLeftDown = TRUE;
}

void CCharEditDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
	switch(cMode)
	{
	case DRAW:
		break;
	case DRAG: //fall through
	case DRAG_LINE:
		Stroke();
		cDragging = -1;
		Invalidate();
		break;
	case KILL:
		break;
	case KILL_LINE:
		break;
	default:
		ASSERT(FALSE);
		break;
	}
	CDialog::OnLButtonUp(nFlags, point);
	cLeftDown = FALSE;
}

void CCharEditDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	switch(cMode)
	{
	case DRAW:
		if(cDrawing)
		{
			Stroke();
			cPointData[cPoints-1] = point;
			Stroke();
		}
		break;
	case DRAG:
		if(cDragging >= 0)
		{
			Stroke();
			CPoint cPnt = Snap(point,TRUE);
			pChar->cVertex[cDragging].X = cPnt.x;
			pChar->cVertex[cDragging].Y = cPnt.y;
			Stroke();
		}
		break;
	case KILL:
		break;
	case DRAG_LINE:
		if(cDragging >= 0)
		{
			Stroke();
			CPoint cPnt = Snap(point,TRUE);
			long dx,dy;
			dx = ((long)(pChar->cVertex[cDragging].X)) - cPnt.x;
			dy = ((long)(pChar->cVertex[cDragging].Y)) - cPnt.y;
			int index = cDragging;
			while((index > 0) && (pChar->cVertex[index].X > -63.5)) index--;
			index ++;
			while(pChar->cVertex[index].X > -63.5)
			{
				pChar->cVertex[index].X -= dx;
				pChar->cVertex[index].Y -= dy;
				index ++;
			}
			Stroke();
		}
		break;
	case KILL_LINE:
		break;
	default:
		ASSERT(FALSE);
		break;
	}
	CDialog::OnMouseMove(nFlags, point);
}

void CCharEditDlg::OnRButtonDown(UINT nFlags, CPoint point) 
{
	switch(cMode)
	{
	case DRAW:
		if(cDrawing)
		{
			Stroke();
			cPointData[cPoints-1] = Snap(point);
			cDrawing = FALSE;

			//place these points in the character definition
			BOOL pres = !pChar->cVertexCount;
			CharVertex * newVertex = new CharVertex[pChar->cVertexCount + cPoints + pres + 1];
			int index = 0;
			int loop;
			for(loop = 0; loop < pChar->cVertexCount-1; loop++)
			{
				newVertex[index] = pChar->cVertex[loop];
				index++;
			}
			newVertex[index].X = -64;
			newVertex[index].Y = 0;
			index++;
			for(loop = 0; loop < cPoints; loop++)
			{
				CPoint pnt = Snap(cPointData[loop],TRUE);
				newVertex[index].X = pnt.x;
				newVertex[index].Y = pnt.y;
				index++;
			}
			newVertex[index].X = -64;
			newVertex[index].Y = -64;
			index++;
			if( pChar->cVertex )
				delete pChar->cVertex;
			pChar->cVertex = newVertex;
			pChar->cVertexCount = index;
			Invalidate(FALSE);
		}
		break;
	case DRAG:
		break;
	case KILL:
		break;
	case DRAG_LINE:
		break;
	case KILL_LINE:
		break;
	default:
		ASSERT(FALSE);
		break;
	}
	CDialog::OnRButtonDown(nFlags, point);
	cRightDown = TRUE;
}

void CCharEditDlg::OnRButtonUp(UINT nFlags, CPoint point) 
{
	switch(cMode)
	{
	case DRAW:
		break;
	case DRAG:
		break;
	case KILL:
		break;
	case DRAG_LINE:
		break;
	case KILL_LINE:
		break;
	default:
		ASSERT(FALSE);
		break;
	}
	CDialog::OnRButtonUp(nFlags, point);
	cRightDown = FALSE;
}


void CCharEditDlg::OnOK() 
{
	if(pRevert) 
		delete pRevert;
	if( pUndo )
		delete pUndo;
	CDialog::OnOK();
}

void CCharEditDlg::OnRevert() 
{
	if( pChar->cVertexCount != pRevert->cVertexCount )
		SnapForUndo();
	if( pChar->cVertex )
		delete pChar->cVertex;
	pChar->cVertexCount = pRevert->cVertexCount;
	if( pChar->cVertexCount )
	{
		pChar->cVertex = new CharVertex[pChar->cVertexCount];
		int loop;
		for(loop = 0; loop < pChar->cVertexCount; loop++)
			pChar->cVertex[loop] = pRevert->cVertex[loop];
	}
	Invalidate();
}

void CCharEditDlg::OnCancel() 
{
	OnRevert();
	OnOK();
}

void CCharEditDlg::OnUndo() 
{
	cDrawing = FALSE;
	cDragging = -1;
	if( pChar->cVertex )
		delete pChar->cVertex;
	pChar->cVertexCount = pUndo->cVertexCount;
	if( pChar->cVertexCount )
	{
		pChar->cVertex = new CharVertex[pChar->cVertexCount];
		int loop;
		for(loop = 0; loop < pChar->cVertexCount; loop++)
			pChar->cVertex[loop] = pUndo->cVertex[loop];
	}
	Invalidate();
}
