#if !defined(AFX_CHAREDITDLG_H__B11C53C2_11A7_11D2_BA96_006008C9F6BB__INCLUDED_)
#define AFX_CHAREDITDLG_H__B11C53C2_11A7_11D2_BA96_006008C9F6BB__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// CharEditDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCharEditDlg dialog

class CCharEditDlg : public CDialog
{
// Construction
public:
	CCharEditDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCharEditDlg)
	enum { IDD = IDD_EDIT_DIALOG };
	CButton	m_KillLine;
	CButton	m_DragLine;
	CButton	m_Kill;
	CButton	m_Draw;
	CButton	m_Drag;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCharEditDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
	SMCharacter * pChar;
	SMCharacter * pRevert;
	SMCharacter * pUndo;

	enum MODE
	{
		DRAW,
		DRAG,
		KILL,
		DRAG_LINE,
		KILL_LINE
	};

	MODE cMode;
	BOOL cLeftDown;
	BOOL cRightDown;
	BOOL cDrawing;
	int cDragging;

	CPoint cPointData[128];
	int cPoints;
	
// Implementation

private:
	void Stroke(void);
	CPoint Snap(CPoint pPoint, BOOL pHersheyUnits = FALSE);
	void SnapForUndo();

protected:

	// Generated message map functions
	//{{AFX_MSG(CCharEditDlg)
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	afx_msg void OnDraw();
	afx_msg void OnDrag();
	afx_msg void OnKill();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnDragLine();
	afx_msg void OnKillLine();
	virtual void OnOK();
	afx_msg void OnRevert();
	virtual void OnCancel();
	afx_msg void OnUndo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHAREDITDLG_H__B11C53C2_11A7_11D2_BA96_006008C9F6BB__INCLUDED_)
