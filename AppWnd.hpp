/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		APPWND.HPP
** COMPONENT:	The Application.
** DESCRIPTION:	The CAppWnd class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef APPWND_HPP
#define APPWND_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include <WCL/DlgFrame.hpp>
#include "AppDlg.hpp"
#include <WCL/FrameMenu.hpp>
#include "AppToolbar.hpp"
#include <WCL/StatusBar.hpp>
#include <WCL/TrayIcon.hpp>
#include <WCL/Accel.hpp>

/******************************************************************************
**
** This is the the applications main window.
**
*******************************************************************************
*/

class CAppWnd : public CDlgFrame
{
public:
	//
	// Constructors/Destructor.
	//
	CAppWnd();
	~CAppWnd();

	//
	// Methods.
	//
	void ShowTrayIcon(bool bShow);
	void UpdateTrayIconTip();

	//
	// Class methods.
	//
	static bool RestorePrevInstance();

	//
	// Members.
	//
	CAppDlg		m_AppDlg;
	CFrameMenu	m_Menu;
	CAppToolBar	m_ToolBar;
	CStatusBar	m_StatusBar;
	CTrayIcon	m_oTrayIcon;

protected:
	//
	// Internal members.
	//
	CAccel		m_Accel;

	//
	// Constants.
	//

	// Child window IDs.
	static const uint IDC_TOOL_BAR   = 100;
	static const uint IDC_STATUS_BAR = 101;

	// Tray icon IDs.
	static const uint TRAY_ICON_ID = 100;

	// Tray icon message ID.
	static const uint WM_USER_TRAY_NOTIFY = WM_USER;

	// Window class name.
	static const tchar* CLASS_NAME;

	//
	// Window creation template methods.
	//
	virtual void GetClassParams(WNDCLASS& rParams);
	virtual void GetCreateParams(WNDCREATE& rParams);

	//
	// Message processors.
	//
	virtual void OnCreate(const CRect& rcClient);
	virtual void OnUserMsg(uint nMsg, WPARAM wParam, LPARAM lParam);
	virtual void OnResize(int iFlag, const CSize& rNewSize);
	virtual bool OnQueryClose();
	virtual void OnClose();
	virtual bool ProcessMsg(MSG& rMsg);

	//
	// Internal methods.
	//
	void Restore();
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

#endif //APPWND_HPP
