// FontManagerView.h : interface of the CFontManagerView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_FONTMANAGERVIEW_H__15A533E2_1180_11D2_BA96_006008C9F6BB__INCLUDED_)
#define AFX_FONTMANAGERVIEW_H__15A533E2_1180_11D2_BA96_006008C9F6BB__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CFontManagerView : public CScrollView
{
protected: // create from serialization only
	CFontManagerView();
	DECLARE_DYNCREATE(CFontManagerView)

// Attributes
public:
	CFontManagerDoc* GetDocument();

	enum FONT_DISPLAY_MODES
	{
		FD_NORMAL,
		FD_TABLE
	};


	UINT cCurrentPage;
	int cCurSel;
	int cCurIndex;
	int cCurRow;
	int cCurCol;
	int cCopyRow;
	FONT_DISPLAY_MODES cDisplayMode;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFontManagerView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CFontManagerView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CFontManagerView)
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnXlatedView();
	afx_msg void OnUpdateXlatedView(CCmdUI* pCmdUI);
	afx_msg void OnEditCopy();
	afx_msg void OnEditPaste();
	afx_msg void OnFileSave();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnUpdateEditCopy(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditPaste(CCmdUI* pCmdUI);
	afx_msg void OnAssign();
	afx_msg void OnUpdateAssign(CCmdUI* pCmdUI);
	afx_msg void OnIndex();
	afx_msg void OnUpdateIndex(CCmdUI* pCmdUI);
	afx_msg void OnClear();
	afx_msg void OnUpdateClear(CCmdUI* pCmdUI);
	afx_msg void OnWhoUses();
	afx_msg void OnUpdateWhoUses(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in FontManagerView.cpp
inline CFontManagerDoc* CFontManagerView::GetDocument()
   { return (CFontManagerDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FONTMANAGERVIEW_H__15A533E2_1180_11D2_BA96_006008C9F6BB__INCLUDED_)
