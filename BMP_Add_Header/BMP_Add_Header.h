// BMP_Add_Header.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CBMP_Add_HeaderApp:
// See BMP_Add_Header.cpp for the implementation of this class
//

class CBMP_Add_HeaderApp : public CWinApp
{
public:
	CBMP_Add_HeaderApp();

// Overrides
	public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CBMP_Add_HeaderApp theApp;