/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		APPCMDS.CPP
** COMPONENT:	The Application.
** DESCRIPTION:	CAppCmds class definition.
**
*******************************************************************************
*/

#include "AppHeaders.hpp"
#include "AboutDlg.hpp"
#include "ClockInDlg.hpp"
#include "ClockOutDlg.hpp"
#include "SwitchTasksDlg.hpp"
#include "EditSessionDlg.hpp"
#include "PruneSessionsDlg.hpp"
#include "PruneTasksDlg.hpp"
#include "PruneLocnsDlg.hpp"
#include "ClpWndReportDlg.hpp"
#include "FileReportDlg.hpp"
#include "PrintReportDlg.hpp"
#include "ViewReportDlg.hpp"

/******************************************************************************
** Method:		Constructor.
**
** Description:	.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

CAppCmds::CAppCmds()
{
	// Define the command table.
	DEFINE_CMD_TABLE
		CMD_ENTRY(ID_FILE_EXPORT,			OnFileExport,			NULL,					0)
		CMD_ENTRY(ID_FILE_IMPORT,			OnFileImport,			NULL,					1)
		CMD_ENTRY(ID_FILE_EXIT,				OnFileExit,				NULL,					-1)
		CMD_ENTRY(ID_SESSION_CLOCK_IN,		OnSessionClockIn,		OnUISessionClockIn,		2)
		CMD_ENTRY(ID_SESSION_SWITCH_TASKS,	OnSessionSwitchTasks,	OnUISessionSwitchTasks,	3)
		CMD_ENTRY(ID_SESSION_CLOCK_OUT,		OnSessionClockOut,		OnUISessionClockOut,	4)
		CMD_ENTRY(ID_SESSION_EDIT,			OnSessionEdit,			NULL,					5)
		CMD_ENTRY(ID_REPORT_WINDOW,			OnReportWindow,			NULL,					6)
		CMD_ENTRY(ID_REPORT_CLIPBOARD,		OnReportClipboard,		NULL,					7)
		CMD_ENTRY(ID_REPORT_PRINT,			OnReportPrint,			NULL,					8)
		CMD_ENTRY(ID_REPORT_FILE,			OnReportFile,			NULL,					9)
		CMD_ENTRY(ID_PRUNE_SESSIONS,		OnPruneSessions,		OnUIPruneSessions,		-1)
		CMD_ENTRY(ID_PRUNE_TASKS,			OnPruneTasks,			OnUIPruneTasks,			-1)
		CMD_ENTRY(ID_PRUNE_LOCNS,			OnPruneLocations,		OnUIPruneLocations,		-1)
		CMD_ENTRY(ID_HELP_ABOUT,			OnHelpAbout,			NULL,					10)
	END_CMD_TABLE
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

CAppCmds::~CAppCmds()
{
}

/******************************************************************************
** Method:		OnFileExport()
**
** Description:	Dump the database to a Comma Separated Values text file.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppCmds::OnFileExport()
{
	CPath Path;

	// File extensions.
	static char szExts[] = {	"Data Files (*.CSV)\0*.CSV\0"
								"All Files (*.*)\0*.*\0"
								"\0\0"							};

	// Select a filename.
	if (!Path.Select(App.m_AppWnd, CPath::SaveFile, szExts, "CSV", CPath::ApplicationDir()))
		return;

	// Warn user if file exists.
	if ( (Path.Exists())
	  && (App.QueryMsg("The file already exists:\n\n%s\n\nDo you want to overwrite it?", Path) != IDYES) )
		return;

	try
	{
		// Template shorthands.
		typedef CSessionList::const_iterator CIter;

		CFile File;

		// Open the file.
		File.Create(Path);

		// For all sessions.
		for(CIter oIter = App.SessionList().begin(); oIter != App.SessionList().end(); ++oIter)
		{
			CSession* pSession = *oIter;

			CString strLine;

			// Format as Start,End,Task,Location
			strLine.Format("\"%s\",\"%s\",\"%s\",\"%s\"", 
							pSession->Start().ToString(CDate::FMT_ISO, CDate::FMT_ISO),
							pSession->Finish().ToString(CDate::FMT_ISO, CDate::FMT_ISO),
							pSession->Task(),
							pSession->Location());

			File.WriteLine(strLine);
		}

		// Done.
		File.Close();
	}
	catch(CFileException& rException)
	{
		// Notify user.
		App.m_AppWnd.AlertMsg(rException.ErrorText());
	}
}

/******************************************************************************
** Method:		OnFileImport()
**
** Description:	.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppCmds::OnFileImport()
{
	CPath Path;

	// File extensions.
	static char szExts[] = {	"Data Files (*.CSV)\0*.CSV\0"
								"All Files (*.*)\0*.*\0"
								"\0\0"							};

	// Select a filename.
	if (!Path.Select(App.m_AppWnd, CPath::OpenFile, szExts, "CSV", CPath::ApplicationDir()))
		return;

	try
	{
		CFile File;
		CSessionList&	rSessions = App.SessionList();
		CTaskList&		rTasks    = App.TaskList();
		CLocnList&		rLocns    = App.LocnList();

		// Free the existing data.
		App.DeleteAllData();

		// Open the file.
		File.Open(Path, GENERIC_READ);

		int nLine = 1;

		// For all lines
		while (!File.IsEOF())
		{
			// Read a line.
			CString strLine = File.ReadLine();

			// Count the number of fields.
			int nFields = strLine.Count(',') + 1;

			// Expecting "Start,End,Task,Location".
			if (nFields != 4)
			{
				// Notify user.
				App.m_AppWnd.AlertMsg("Invalid number of fields in line: %d\n\n"
									  "Found: %d fields. Expected 'Start,End,Task,Location'",
									  nLine, nFields);
				break;
			}

			// Find field separators.
			int nStartDateTime = -1;
			int	nEndDateTime   = strLine.Find(',', nStartDateTime + 1);
			int nTask          = strLine.Find(',', nEndDateTime   + 1);
			int nLocation      = strLine.Find(',', nTask          + 1);
			int nEOS           = strLine.Length();

			// Extract basic fields.
			CString strStartDateTime = strLine.Mid(nStartDateTime + 1, nEndDateTime - nStartDateTime - 1);
			CString strEndDateTime   = strLine.Mid(nEndDateTime   + 1, nTask        - nEndDateTime   - 1);
			CString strTask          = strLine.Mid(nTask          + 1, nLocation    - nTask          - 1);
			CString strLocation      = strLine.Mid(nLocation      + 1, nEOS         - nLocation      - 1);

			int nPos = -1;

			// Trim quotes, if present.
			while ((nPos = strStartDateTime.Find('\"')) != -1)
				strStartDateTime.Delete(nPos);

			while ((nPos = strEndDateTime.Find('\"')) != -1)
				strEndDateTime.Delete(nPos);

			while ((nPos = strTask.Find('\"')) != -1)
				strTask.Delete(nPos);

			while ((nPos = strLocation.Find('\"')) != -1)
				strLocation.Delete(nPos);

			CDateTime dtStart, dtEnd;

			// Convert start date/time.
			if (!dtStart.FromString(strStartDateTime))
			{
				// Notify user.
				App.m_AppWnd.AlertMsg("Invalid 'Start' date/time format on line: %d\n\n%s", nLine, strLine);
				break;
			}

			// Convert end date/time.
			if (!dtEnd.FromString(strEndDateTime))
			{
				// Notify user.
				App.m_AppWnd.AlertMsg("Invalid 'End' date/time format on line: %d\n\n%s", nLine, strLine);
				break;
			}

			// Invalid dates?
			if (dtEnd < dtStart)
			{
				// Notify user.
				App.m_AppWnd.AlertMsg("'End' date/time precedes 'Start' date/time on line: %d\n\n%s", nLine, strLine);
				break;
			}

			// Allocate a new session and initialise.
			CSession* pSession = new CSession;

			pSession->Start (dtStart, strTask, strLocation);
			pSession->Finish(dtEnd,   strTask, strLocation);
			
			// Add to the collections.
			rSessions.Add(pSession);

			if (strTask != "")
				rTasks.Add(strTask);

			if (strLocation != "")
				rLocns.Add(strLocation);

			nLine++;
		}

		// Done.
		File.Close();
	}
	catch(CFileException& rException)
	{
		// Notify user.
		App.m_AppWnd.AlertMsg(rException.ErrorText());
	}

	// Update UI.
	UpdateUI();
	App.m_AppWnd.m_AppDlg.Update();
}

/******************************************************************************
** Method:		OnFileExit()
**
** Description:	Terminate the app.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppCmds::OnFileExit()
{
	App.m_AppWnd.Close();
}

/******************************************************************************
** Method:		OnSessionClockIn()
**
** Description:	Show the clock in dialog and clock in if accepted.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppCmds::OnSessionClockIn()
{
	CClockInDlg	Dlg;
	
	if (Dlg.RunModal(App.m_rMainWnd) == IDOK)
	{
		App.ClockIn(Dlg.m_dtDateTime, Dlg.m_strTask, Dlg.m_strLocn);
		App.m_AppWnd.m_AppDlg.Update();
	}
}

/******************************************************************************
** Method:		OnSessionSwitchTasks()
**
** Description:	Show the switch tasks dialog and switch tasks if accepted.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppCmds::OnSessionSwitchTasks()
{
	CDateTime dtCurrent = CDateTime::Current();
	CString   strLocn   = App.CurrentSession()->Location();

	// Check we haven't clocked in later than now.
	if (dtCurrent < App.CurrentSession()->Start())
	{
		const char* pszMsg = "You cannot switch tasks because the time\n"
							 "you clocked in is later than the time now.";

		App.AlertMsg(pszMsg);
		return;
	}

	CSwitchTasksDlg	Dlg;
	
	if (Dlg.RunModal(App.m_rMainWnd) == IDOK)
	{
		dtCurrent.Set();

		// Strip seconds.
		dtCurrent -= CDateTimeSpan(dtCurrent.Time().Secs());
	
		App.ClockOut(dtCurrent, Dlg.m_strPrevTask, strLocn);
		App.ClockIn(dtCurrent, Dlg.m_strNextTask, strLocn);
		App.m_AppWnd.m_AppDlg.Update();
	}
}

/******************************************************************************
** Method:		OnSessionClockOut()
**
** Description:	Show the clock out dialog and clock out if accepted.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppCmds::OnSessionClockOut()
{
	CClockOutDlg Dlg;
	
	if (Dlg.RunModal(App.m_rMainWnd) == IDOK)
	{
		App.ClockOut(Dlg.m_dtDateTime, Dlg.m_strTask, Dlg.m_strLocn);
		App.m_AppWnd.m_AppDlg.Update();
	}
}

/******************************************************************************
** Method:		OnSessionEdit()
**
** Description:	Show the edit sessions dialog.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppCmds::OnSessionEdit()
{
	CEditSessionDlg	Dlg;
	
	Dlg.RunModal(App.m_rMainWnd);

	// Update UI.
	UpdateUI();
	App.m_AppWnd.m_AppDlg.Update();
}

/******************************************************************************
** Method:		OnReportWindow()
**
** Description:	.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppCmds::OnReportWindow()
{
	CClpWndReportDlg Dlg;

	Dlg.m_strTitle = "Report In Window";

	if (Dlg.RunModal(App.m_rMainWnd) == IDOK)
	{
		CBusyCursor BusyCursor;

		// Report status.
		App.m_AppWnd.m_StatusBar.Hint("Generating report...");

		// Setup the memory stream to report to.
		CBuffer		oBuffer;
		CMemStream	TxtStream(oBuffer);
		CTextReport Device(TxtStream);

		TxtStream.Create();

		// Do the report.
		App.ReportData(Device, Dlg.m_eGrouping, Dlg.m_FromDate, Dlg.m_ToDate);

		TxtStream.Close();

		// Report status.
		App.m_AppWnd.m_StatusBar.Hint("Report generated");

		// View it.
		CViewReportDlg Dlg(TxtStream);
		Dlg.RunModal(App.m_rMainWnd);
	}
}

/******************************************************************************
** Method:		OnReportClipboard()
**
** Description:	.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppCmds::OnReportClipboard()
{
	CClpWndReportDlg Dlg;

	Dlg.m_strTitle = "Report To Clipboard";

	if (Dlg.RunModal(App.m_rMainWnd) == IDOK)
	{
		CBusyCursor BusyCursor;

		// Report status.
		App.m_AppWnd.m_StatusBar.Hint("Generating report...");

		// Setup the clipbaord stream.
		CClipboard Clipboard;
		CTextReport Device(Clipboard);

		Clipboard.Open(GENERIC_WRITE, CF_TEXT);

		// Do the report.
		App.ReportData(Device, Dlg.m_eGrouping, Dlg.m_FromDate, Dlg.m_ToDate);

		Clipboard.Close();

		// Report status.
		App.m_AppWnd.m_StatusBar.Hint("Report generated");
	}
}

/******************************************************************************
** Method:		OnReportPrint()
**
** Description:	Show the print report dialog and generate the report.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppCmds::OnReportPrint()
{
	CPrintReportDlg Dlg;
	
	if (Dlg.RunModal(App.m_rMainWnd) == IDOK)
	{
		CBusyCursor BusyCursor;
		CPrinterDC	DC(App.Printer());

		// Report status.
		App.m_AppWnd.m_StatusBar.Hint("Generating report...");

		// Setup the printer to print to.
		CPrintReport Device(DC);
		
		// Do the report.
		App.ReportData(Device, Dlg.m_eGrouping, Dlg.m_FromDate, Dlg.m_ToDate);

		// Report status.
		App.m_AppWnd.m_StatusBar.Hint("Report generated");
	}
}

/******************************************************************************
** Method:		OnReportFile()
**
** Description:	Show the report to file dialog and generate the report.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppCmds::OnReportFile()
{
	CFileReportDlg Dlg;
	
	if (Dlg.RunModal(App.m_rMainWnd) == IDOK)
	{
		CBusyCursor BusyCursor;

		// Report status.
		App.m_AppWnd.m_StatusBar.Hint("Generating report...");

		// Setup the file to report to.
		CFileReport Device(Dlg.m_strFileName);
		
		// Do the report.
		App.ReportData(Device, Dlg.m_eGrouping, Dlg.m_FromDate, Dlg.m_ToDate);

		// Report status.
		App.m_AppWnd.m_StatusBar.Hint("Report generated");
	}
}

/******************************************************************************
** Method:		OnPruneSessions()
**
** Description:	Show the prune sessions dialog.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppCmds::OnPruneSessions()
{
	CPruneSessionsDlg Dlg;
	
	if (Dlg.RunModal(App.m_rMainWnd) == IDOK)
	{
		// Template shorthands.
		typedef CSessionList::iterator CIter;

		CIsSessionInRange oRange(CDateTime::Min(), CDateTime(Dlg.m_Date, CTime::Max()));
		CIter             oIter(App.SessionList().begin());

		// Delete all sessions within the period.
		while ((oIter = std::find_if(oIter, App.SessionList().end(), oRange)) != App.SessionList().end())
		{
			CIter oCurrIter = oIter++;

			delete *oCurrIter;

			App.SessionList().erase(oCurrIter);
		}

		// Update dirty flag.
		App.Modified();
	
		// Update UI.
		UpdateUI();
		App.m_AppWnd.m_AppDlg.Update();
	}
}

/******************************************************************************
** Method:		OnPruneTasks()
**
** Description:	Show the prune tasks dialog.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppCmds::OnPruneTasks()
{
	// Template shorthands.
	typedef CTaskList::const_iterator CIter;

	CPruneTasksDlg Dlg;
	
	if (Dlg.RunModal(App.m_rMainWnd) == IDOK)
	{
		// For all tasks.
		for(CIter oIter = Dlg.m_TaskList.begin(); oIter != Dlg.m_TaskList.end(); ++oIter)
			App.TaskList().Remove(*oIter);
		
		// Update dirty flag.
		App.Modified();

		// Update UI.
		UpdateUI();
	}
}

/******************************************************************************
** Method:		OnPruneLocations()
**
** Description:	Show the prune locations dialog.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppCmds::OnPruneLocations()
{
	// Template shorthands.
	typedef CLocnList::const_iterator CIter;

	CPruneLocnsDlg Dlg;
	
	if (Dlg.RunModal(App.m_rMainWnd) == IDOK)
	{
		// For all locations.
		for(CIter oIter = Dlg.m_LocnList.begin(); oIter != Dlg.m_LocnList.end(); ++oIter)
			App.LocnList().Remove(*oIter);
		
		// Update dirty flag.
		App.Modified();

		// Update UI.
		UpdateUI();
	}
}

/******************************************************************************
** Method:		OnHelpAbout()
**
** Description:	Show the about dialog.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppCmds::OnHelpAbout()
{
	CAboutDlg Dlg;

	Dlg.RunModal(App.m_rMainWnd);
}

/******************************************************************************
** Method:		OnUI...()
**
** Description:	UI update handlers.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppCmds::OnUISessionClockIn()
{
	// Get current status.
	bool bClockedIn = App.ClockedIn();

	App.m_AppWnd.m_Menu.EnableCmd(ID_SESSION_CLOCK_IN, !bClockedIn);
	App.m_AppWnd.m_ToolBar.m_ClockInBtn.Enable(!bClockedIn);
}

void CAppCmds::OnUISessionSwitchTasks()
{
	// Get current status.
	bool bClockedIn = App.ClockedIn();

	App.m_AppWnd.m_Menu.EnableCmd(ID_SESSION_SWITCH_TASKS, bClockedIn);
	App.m_AppWnd.m_ToolBar.m_SwitchTasksBtn.Enable(bClockedIn);
}

void CAppCmds::OnUISessionClockOut()
{
	// Get current status.
	bool bClockedIn = App.ClockedIn();

	App.m_AppWnd.m_Menu.EnableCmd(ID_SESSION_CLOCK_OUT, bClockedIn);
	App.m_AppWnd.m_ToolBar.m_ClockOutBtn.Enable(bClockedIn);
}

void CAppCmds::OnUIPruneSessions()
{
	bool bItems = (App.SessionList().size() > 0);

	App.m_AppWnd.m_Menu.EnableCmd(ID_PRUNE_SESSIONS, bItems);
}

void CAppCmds::OnUIPruneTasks()
{
	bool bItems = (App.TaskList().size() > 0);

	App.m_AppWnd.m_Menu.EnableCmd(ID_PRUNE_TASKS, bItems);
}

void CAppCmds::OnUIPruneLocations()
{
	bool bItems = (App.LocnList().size() > 0);

	App.m_AppWnd.m_Menu.EnableCmd(ID_PRUNE_LOCNS, bItems);
}
