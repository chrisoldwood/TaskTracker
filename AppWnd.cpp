/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		APPWND.CPP
** COMPONENT:	The Application.
** DESCRIPTION:	CAppWnd class definition.
**
*******************************************************************************
*/

#include "AppHeaders.hpp"

/******************************************************************************
** Method:		Default constructor.
**
** Description:	.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

CAppWnd::CAppWnd()
	: CDlgFrame(IDR_APPICON, m_AppDlg, true)
	, m_StatusBar(false)
{
}

/******************************************************************************
** Method:		Destructor.
**
** Description:	.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

CAppWnd::~CAppWnd()
{
}

/******************************************************************************
** Method:		OnCreate()
**
** Description:	Creates the toolbar and status bars.
**
** Parameters:	rcClient	The client rectangle.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppWnd::OnCreate(const CRect& rcClient)
{
	//
	// Create and attach the components.
	//
	m_Accel.LoadRsc(IDR_APPACCEL);
	Accel(&m_Accel);

	m_Menu.LoadRsc(IDR_APPMENU);
	Menu(&m_Menu);

	m_ToolBar.Create(*this, IDC_TOOL_BAR, rcClient);
	ToolBar(&m_ToolBar);

	m_StatusBar.Create(*this, IDC_STATUS_BAR, rcClient);
	StatusBar(&m_StatusBar);

	m_AppDlg.RunModeless(*this);

	// Call base class.
	CDlgFrame::OnCreate(rcClient);
}

/******************************************************************************
** Method:		OnResize()
**
** Description:	Window has been resized.
**
** Parameters:	iFlag		The resize method.
**				rNewSize	The new window dimensions.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppWnd::OnResize(int iFlag, const CSize& rNewSize)
{
	// Window minimised AND "Minimise To Tray" set?
	if ( (iFlag == SIZE_MINIMIZED) && (App.MinimiseToTray()) )
	{
		ShowTrayIcon(true);
		Show(SW_HIDE);
	}

	CDlgFrame::OnResize(iFlag, rNewSize);
}

/******************************************************************************
** Method:		OnUserMsg()
**
** Description:	Callback message from the system tray.
**				
** Parameters:	As WindowProc.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppWnd::OnUserMsg(uint /*nMsg*/, WPARAM /*wParam*/, LPARAM lParam)
{
//	ASSERT(nMsg   == WM_USER_TRAY_NOTIFY);
//	ASSERT(wParam == TRAY_ICON_ID);

	// Icon double-clicked with left button?
	if (lParam == WM_LBUTTONDBLCLK)
	{
		Restore();
	}
	// Icon clicked with right button?
	else if ( (lParam == WM_RBUTTONUP) || (lParam == WM_CONTEXTMENU) )
	{
		// Get current status.
		bool bClockedIn = App.ClockedIn();

		// Required to allow menu to be dismissed.
		::SetForegroundWindow(m_hWnd);

		CPopupMenu oMenu(IDR_TRAYMENU);

		// Disable relevant commands.
		oMenu.EnableCmd(ID_SESSION_CLOCK_IN,     !bClockedIn);
		oMenu.EnableCmd(ID_SESSION_SWITCH_TASKS, bClockedIn);
		oMenu.EnableCmd(ID_SESSION_CLOCK_OUT,    bClockedIn);

		// Show context menu.
		uint nCmdID = oMenu.TrackMenu(*this, CCursor::CurrentPos());

		if (nCmdID != 0)
		{
			// Handle command.
			switch (nCmdID)
			{
				case ID_TRAY_RESTORE:			Restore();								break;
				case ID_SESSION_CLOCK_IN:		App.m_AppCmds.OnSessionClockIn();		break;
				case ID_SESSION_SWITCH_TASKS:	App.m_AppCmds.OnSessionSwitchTasks();	break;
				case ID_SESSION_CLOCK_OUT:		App.m_AppCmds.OnSessionClockOut();		break;
				case ID_TRAY_EXIT:				Close();								break;
				default:						ASSERT_FALSE();							break;
			}
		}

		// Required to allow menu to be dismissed.
		PostMessage(WM_NULL);
	}
}

/******************************************************************************
** Method:		OnQueryClose()
**
** Description:	Check if the app can close.
**
** Parameters:	None.
**
** Returns:		true or false.
**
*******************************************************************************
*/

bool CAppWnd::OnQueryClose()
{
	// Data changed?
	if (!App.IsModified())
		return true;

	CBusyCursor BusyCursor;

	// Save changes.
	return App.SaveData();
}

/******************************************************************************
** Method:		OnClose()
**
** Description:	The main window is closing.
**				NB: Called from WM_CLOSE or WM_ENDSESSION.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppWnd::OnClose()
{
	// Remove the tray icon.
	if (m_oTrayIcon.IsVisible())
		ShowTrayIcon(false);
}

/******************************************************************************
** Method:		Restore()
**
** Description:	Restore the window from the system tray or taskbar.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppWnd::Restore()
{
	// Not already restored?
	if (!IsVisible() || IsMinimised())
	{
		Show(SW_SHOWNORMAL);
		::SetForegroundWindow(m_hWnd);

		ShowTrayIcon(false);
	}
}
