// FontManager.h : main header file for the FONTMANAGER application
//

#if !defined(AFX_FONTMANAGER_H__15A533DA_1180_11D2_BA96_006008C9F6BB__INCLUDED_)
#define AFX_FONTMANAGER_H__15A533DA_1180_11D2_BA96_006008C9F6BB__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CFontManagerApp:
// See FontManager.cpp for the implementation of this class
//

class CFontManagerApp : public CWinApp
{
public:
	CFontManagerApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFontManagerApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CFontManagerApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FONTMANAGER_H__15A533DA_1180_11D2_BA96_006008C9F6BB__INCLUDED_)
