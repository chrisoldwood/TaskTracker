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
	if (!Path.Select(App.m_AppWnd, CPath::SaveFile, szExts, "CSV"))
		return;

	// Warn user if file exists.
	if ( (Path.Exists())
	  && (App.QueryMsg("The file already exists:\n\n%s\n\nOverwrite?", Path) != IDYES) )
		return;

	try
	{
		CFile File;

		// Open the file.
		File.Create(Path);

		CSessionEnum	Enum(App.SessionList());
		CSession*		pSession;
		
		// For all sessions.
		while((pSession = Enum.Next()) != NULL)
		{
			CString strLine;

			// Create a comma separated text line
			// for the session.
			strLine.Format("%s,%s,%s,%s,%s,%s", 
							pSession->Start().Date().ToString(CDate::DD_MM_YYYY),
							pSession->Start().Time().ToString(CTime::HH_MM),
							pSession->Finish().Date().ToString(CDate::DD_MM_YYYY),
							pSession->Finish().Time().ToString(CTime::HH_MM),
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
	if (!Path.Select(App.m_AppWnd, CPath::OpenFile, szExts, "CSV"))
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

			// Count the number of felds.
			int nFields = strLine.Count(',') + 1;

			// Expecting 5 or 6 fields.
			if ( (nFields < 5) || (nFields > 6) )
			{
				// Notify user.
				App.m_AppWnd.AlertMsg("Invalid number of fields in line: %d\n"
										"(Found: %d Expected: 5 or 6).",
										nLine, nFields);
				break;
			}

			// Find field separators.
			int nStartDate = -1;
			int nStartTime = strLine.Find(',', nStartDate + 1);
			int	nEndDate   = strLine.Find(',', nStartTime + 1);
			int nEndTime   = strLine.Find(',', nEndDate   + 1);
			int nTask      = strLine.Find(',', nEndTime   + 1);
			int nLocn      = strLine.Find(',', nTask      + 1);
			int nEOS       = strLine.Length();

			// Extract basic fields.
			CString strStartDate = strLine.Mid(nStartDate + 1, nStartTime - nStartDate - 1);
			CString strStartTime = strLine.Mid(nStartTime + 1, nEndDate   - nStartTime - 1);
			CString strEndDate   = strLine.Mid(nEndDate   + 1, nEndTime   - nEndDate   - 1);
			CString strEndTime   = strLine.Mid(nEndTime   + 1, nTask      - nEndTime   - 1);
			CString strTask;
			CString strLocn;

			// Old format?
			if (nFields == 5)
			{
				// Final field is the task.
				strTask = strLine.Mid(nTask + 1, nEOS - nTask - 1);
				strLocn = "";
			}
			// New format?
			if (nFields == 6)
			{
				// Final field is the location.
				strTask = strLine.Mid(nTask + 1, nLocn - nTask - 1);
				strLocn = strLine.Mid(nLocn + 1, nEOS  - nLocn - 1);
			}

			CDate dStartDate, dEndDate;
			CTime tStartTime, tEndTime;

			// Convert start date.
			if (!dStartDate.FromString(strStartDate))
			{
				// Notify user.
				App.m_AppWnd.AlertMsg("Invalid start date for line: %d\n", nLine);
				break;
			}

			// Convert start time.
			if (!tStartTime.FromString(strStartTime))
			{
				// Notify user.
				App.m_AppWnd.AlertMsg("Invalid start time for line: %d\n", nLine);
				break;
			}

			// Convert end date.
			if (!dEndDate.FromString(strEndDate))
			{
				// Notify user.
				App.m_AppWnd.AlertMsg("Invalid end date for line: %d\n", nLine);
				break;
			}

			// Convert end time.
			if (!tEndTime.FromString(strEndTime))
			{
				// Notify user.
				App.m_AppWnd.AlertMsg("Invalid end time for line: %d\n", nLine);
				break;
			}

			// Allocate a new session and initialise.
			CSession* pSession = new CSession;

			pSession->Start (CDateTime(dStartDate, tStartTime), strTask, strLocn);
			pSession->Finish(CDateTime(dEndDate,   tEndTime),   strTask, strLocn);
			
			// Add to the collections.
			rSessions.Add(pSession);

			if (strTask != "")
				rTasks.Add(strTask);

			if (strLocn != "")
				rLocns.Add(strLocn);

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
		CDateTime	dtStart;
		CDateTime	dtEnd;

		// Setup enum limits.
		dtEnd.Date(Dlg.m_Date);
		
		CSessionList&	SessionList = App.SessionList();
		CSessionEnum	Enum(SessionList, dtStart, dtEnd);
		CSession*		pCurrSession = Enum.Next();
		CSession*		pNextSession = NULL;
		
		// For all sessions within the period.
		while(pCurrSession)
		{
			// Get next session first.
			pNextSession = Enum.Next();
			
			// Delete it.
			SessionList.Remove(pCurrSession);
			delete pCurrSession;
			
			// Move on.
			pCurrSession = pNextSession;
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
	CPruneTasksDlg Dlg;
	
	if (Dlg.RunModal(App.m_rMainWnd) == IDOK)
	{
		CTaskList&		rAppTaskList = App.TaskList();
		CTaskListEnum	Enum(Dlg.m_TaskList);
		CString*		pString;
		
		// For all tasks.
		while((pString = Enum.Next()) != NULL)
			rAppTaskList.Remove(*pString);
		
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
	CPruneLocnsDlg Dlg;
	
	if (Dlg.RunModal(App.m_rMainWnd) == IDOK)
	{
		CLocnList&		rAppLocnList = App.LocnList();
		CLocnListEnum	Enum(Dlg.m_LocnList);
		CString*		pString;
		
		// For all locations.
		while((pString = Enum.Next()) != NULL)
			rAppLocnList.Remove(*pString);
		
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
	bool bItems = (App.SessionList().Length() > 0);

	App.m_AppWnd.m_Menu.EnableCmd(ID_PRUNE_SESSIONS, bItems);
}

void CAppCmds::OnUIPruneTasks()
{
	bool bItems = (App.TaskList().Length() > 0);

	App.m_AppWnd.m_Menu.EnableCmd(ID_PRUNE_TASKS, bItems);
}

void CAppCmds::OnUIPruneLocations()
{
	bool bItems = (App.LocnList().Length() > 0);

	App.m_AppWnd.m_Menu.EnableCmd(ID_PRUNE_LOCNS, bItems);
}
