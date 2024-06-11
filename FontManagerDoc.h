// FontManagerDoc.h : interface of the CFontManagerDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_FONTMANAGERDOC_H__15A533E0_1180_11D2_BA96_006008C9F6BB__INCLUDED_)
#define AFX_FONTMANAGERDOC_H__15A533E0_1180_11D2_BA96_006008C9F6BB__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


class CFontManagerDoc : public CDocument
{
protected: // create from serialization only
	CFontManagerDoc();
	DECLARE_DYNCREATE(CFontManagerDoc)

// Attributes
public:
	SMFontUtil * cFontUtil;


// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFontManagerDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CFontManagerDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CFontManagerDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FONTMANAGERDOC_H__15A533E0_1180_11D2_BA96_006008C9F6BB__INCLUDED_)
