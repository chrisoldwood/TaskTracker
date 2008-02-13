/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		APPWND.CPP
** COMPONENT:	The Application.
** DESCRIPTION:	CAppWnd class definition.
**
*******************************************************************************
*/

#include "Common.hpp"
#include "AppWnd.hpp"
#include "TaskTracker.hpp"
#include <WCL/Cursor.hpp>
#include <WCL/BusyCursor.hpp>

/******************************************************************************
**
** Constants.
**
*******************************************************************************
*/

// Window class name.
const tchar* CAppWnd::CLASS_NAME = TXT("TaskTrackerAppWnd");

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
** Method:		GetClassParams()
**
** Description:	Template method to get the window class data.
**
** Parameters:	rParams		The class structure to fill.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppWnd::GetClassParams(WNDCLASS& rParams)
{
	// Get base class settings.
	CDlgFrame::GetClassParams(rParams);

	// Override any settings.
	rParams.lpszClassName = CLASS_NAME;
}

/******************************************************************************
** Method:		GetCreateParams()
**
** Description:	Template method to get the window creation data.
**
** Parameters:	rParams		The create structure to fill.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppWnd::GetCreateParams(WNDCREATE& rParams)
{
	// Get base class settings.
	CDlgFrame::GetCreateParams(rParams);

	// Override any settings.
	rParams.pszClassName = CLASS_NAME;
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
	if ( (iFlag == SIZE_MINIMIZED) && (App.m_bMinToTray) )
	{
		// Check state first, in case we are recursing
		// due to the ShowWindow() call that follows.
		if (!m_oTrayIcon.IsVisible())
			ShowTrayIcon(true);

		Show(SW_HIDE);
	}
	// Window restored AND Tray Icon still visible?
	else if ( (iFlag == SIZE_RESTORED) && (m_oTrayIcon.IsVisible()) )
	{
		ShowTrayIcon(false);
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
		// Required to allow menu to be dismissed.
		::SetForegroundWindow(m_hWnd);

		CPopupMenu oMenu(IDR_TRAYMENU);

		// Disable relevant commands.
		oMenu.EnableCmd(ID_SESSION_CLOCK_IN,     !App.m_bClockedIn);
		oMenu.EnableCmd(ID_SESSION_SWITCH_TASKS, App.m_bClockedIn);
		oMenu.EnableCmd(ID_SESSION_CLOCK_OUT,    App.m_bClockedIn);

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
	if (!App.m_bModified)
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
** Method:		ProcessMsg()
**
** Description:	Check for mouse movement over the main dialog area and remove
**              any outstanding tooltip.
**
** Parameters:	pMsg	The message.
**
** Returns:		true or false.
**
*******************************************************************************
*/

bool CAppWnd::ProcessMsg(MSG& rMsg)
{
	// Mouse moved over dialog OR dialog control?
	if ( (rMsg.message == WM_MOUSEMOVE)
	  && ((rMsg.hwnd == m_AppDlg.Handle() || (::GetParent(rMsg.hwnd) == m_AppDlg.Handle()))) )
	{
		m_StatusBar.Hint(TXT(""));
	}

	return CDlgFrame::ProcessMsg(rMsg);
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
	}
}

/******************************************************************************
** Method:		RestorePrevInstance()
**
** Description:	Look for a previous instance, and if found restore it so that
**				only one instance is running.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

bool CAppWnd::RestorePrevInstance()
{
	HWND hPrevWnd = ::FindWindow(CLASS_NAME, NULL);

	// First instance?
	if (hPrevWnd == NULL)
		return false;

	// Restore it.
	::ShowWindow(hPrevWnd, SW_RESTORE);
	::SetForegroundWindow(hPrevWnd);

	return true;
}

/******************************************************************************
** Method:		ShowTrayIcon()
**
** Description:	Show or hide the tray icon.
**
** Parameters:	bShow	true or false.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppWnd::ShowTrayIcon(bool bShow)
{
	if (bShow)
	{
		m_oTrayIcon.Add(*this, TRAY_ICON_ID, WM_USER_TRAY_NOTIFY, IDR_APPICON);

		UpdateTrayIconTip();
	}
	else
	{
		m_oTrayIcon.Remove();
	}
}

/******************************************************************************
** Method:		UpdateTrayIconTip()
**
** Description:	Update the tooltip shown by the tray icon, if it's visible.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppWnd::UpdateTrayIconTip()
{
	if (m_oTrayIcon.IsVisible())
	{
		// Start with app name.
		CString strTip = App.m_strTitle;

		// If clocked in, append details.
		if (App.m_bClockedIn)
		{
			ASSERT(App.m_pCurrSession.Get() != nullptr);

			// Create a session from clock-in time to now.
			CSession oSession(App.m_pCurrSession->Start(), CDateTime::Current());

			// We only have room for 64 chars total, so trim the task name to 15.
			CString strTask = App.m_pCurrSession->Task();

			if (strTask.Length() > 15)
				strTask = strTask.Left(12) + TXT("...");
			
			// Build the tooltip.
			strTip += TXT("\n");
			strTip += TXT("Time: ")  + App.m_pCurrSession->Start().Time().ToString(CDate::FMT_WIN_SHORT) + TXT("\n");
			strTip += TXT("Task: ")  + strTask + TXT("\n");
			strTip += TXT("Len:   ") + App.MinsToStr(oSession.Length());
		}

		m_oTrayIcon.ModifyToolTip(strTip);
	}
}
