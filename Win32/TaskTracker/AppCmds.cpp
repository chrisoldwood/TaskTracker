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
#include "OptionsDlg.hpp"
#include "ExportDlg.hpp"
#include "ImportDlg.hpp"

#ifdef _DEBUG
// For memory leak detection.
#define new DBGCRT_NEW
#endif

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
		CMD_ENTRY(ID_FILE_EXPORT,			OnFileExport,			nullptr,				0)
		CMD_ENTRY(ID_FILE_IMPORT,			OnFileImport,			nullptr,				1)
		CMD_ENTRY(ID_FILE_EXIT,				OnFileExit,				nullptr,				-1)
		CMD_ENTRY(ID_SESSION_CLOCK_IN,		OnSessionClockIn,		OnUISessionClockIn,		2)
		CMD_ENTRY(ID_SESSION_SWITCH_TASKS,	OnSessionSwitchTasks,	OnUISessionSwitchTasks,	3)
		CMD_ENTRY(ID_SESSION_CLOCK_OUT,		OnSessionClockOut,		OnUISessionClockOut,	4)
		CMD_ENTRY(ID_SESSION_EDIT,			OnSessionEdit,			nullptr,				5)
		CMD_ENTRY(ID_REPORT_WINDOW,			OnReportWindow,			nullptr,				6)
		CMD_ENTRY(ID_REPORT_CLIPBOARD,		OnReportClipboard,		nullptr,				7)
		CMD_ENTRY(ID_REPORT_PRINT,			OnReportPrint,			nullptr,				8)
		CMD_ENTRY(ID_REPORT_FILE,			OnReportFile,			nullptr,				9)
		CMD_ENTRY(ID_PRUNE_SESSIONS,		OnPruneSessions,		OnUIPruneSessions,		-1)
		CMD_ENTRY(ID_PRUNE_TASKS,			OnPruneTasks,			OnUIPruneTasks,			-1)
		CMD_ENTRY(ID_PRUNE_LOCNS,			OnPruneLocations,		OnUIPruneLocations,		-1)
		CMD_ENTRY(ID_TOOLS_OPTIONS,			OnToolsOptions,			nullptr,				-1)
		CMD_ENTRY(ID_HELP_ABOUT,			OnHelpAbout,			nullptr,				10)
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
** Description:	Dump the database to a CSV format text file.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppCmds::OnFileExport()
{
	CExportDlg Dlg;

	// Get export settings.
	if (Dlg.RunModal(App.m_AppWnd) != IDOK)
		return;

	// Warn user if file exists.
	if ( (Dlg.m_strFileName.Exists())
	  && (App.QueryMsg("The file already exists:\n\n%s\n\nDo you want to overwrite it?", Dlg.m_strFileName) != IDYES) )
		return;

	try
	{
		// Template shorthands.
		typedef CSessionList::const_iterator CIter;

		CFile File;

		// Open the file.
		File.Create(Dlg.m_strFileName);

		// For all sessions.
		for(CIter oIter = App.m_oSessionList.begin(); oIter != App.m_oSessionList.end(); ++oIter)
		{
			CString     strLine;
			CSessionPtr pSession = *oIter;
			CString     strTask  = pSession->Task();
			CString     strLocn  = pSession->Location();
			int         nPos;

			// Strip special CSV characters from task.
			while ((nPos = strTask.Find('\"')) != -1)
				strTask.Delete(nPos);

			while ((nPos = strTask.Find(',')) != -1)
				strTask.Delete(nPos);

			// Strip special CSV characters from location.
			while ((nPos = strLocn.Find('\"')) != -1)
				strLocn.Delete(nPos);

			while ((nPos = strLocn.Find(',')) != -1)
				strLocn.Delete(nPos);

			// Format as Start,End,Task,Location
			strLine.Format("\"%s\",\"%s\",\"%s\",\"%s\"", 
							pSession->Start().ToString(CDate::FMT_ISO, CDate::FMT_ISO),
							pSession->Finish().ToString(CDate::FMT_ISO, CDate::FMT_ISO),
							strTask, strLocn);

			File.WriteLine(strLine);
		}

		// Done.
		File.Close();

		// Remember settings.
		App.m_strExportFile = Dlg.m_strFileName;
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
** Description:	Import the database from a CSV format text file.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppCmds::OnFileImport()
{
	CImportDlg Dlg;

	// Get export settings.
	if (Dlg.RunModal(App.m_AppWnd) != IDOK)
		return;

	try
	{
		CFile        oFile;
		CSessionList lstSessions;
		CTaskList    lstTasks;
		CLocnList    lstLocns;

		// Open the file.
		oFile.Open(Dlg.m_strFileName, GENERIC_READ);

		int nLine = 1;

		// For all lines
		while (!oFile.IsEOF())
		{
			// Read a line.
			CString strLine = oFile.ReadLine();

			// Count the number of fields.
			int nFields = strLine.Count(',') + 1;

			// Expecting "Start,End,Task,Location".
			if (nFields != 4)
			{
				// Notify user.
				App.m_AppWnd.AlertMsg("Invalid number of fields in line: %d\n\n"
									  "Found %d fields. Expected 4 [Start, End, Task, Location]\n\n"
									  "%s",
									  nLine, nFields, strLine);
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
			CSessionPtr pSession = new CSession;

			pSession->Start (dtStart, strTask, strLocation);
			pSession->Finish(dtEnd,   strTask, strLocation);
			
			// Add to the temporary collections.
			lstSessions.Add(pSession);

			if (strTask != "")
				lstTasks.Add(strTask);

			if (strLocation != "")
				lstLocns.Add(strLocation);

			nLine++;
		}

		// Cleanup.
		oFile.Close();

		// Replace existing data?
		if (Dlg.m_eAction == CImportDlg::REPLACE)
		{
			// Replace by swapping the imported and current data lists.
			App.m_oSessionList.swap(lstSessions);
			App.m_oTaskList.swap(lstTasks);
			App.m_oLocnList.swap(lstLocns);
		}
		// Merge into existing data?
		else if (Dlg.m_eAction == CImportDlg::MERGE)
		{
			// Merge unique sessions only?
			if (Dlg.m_bNoDuplicates)
			{
				for (CSessionList::iterator itSession = lstSessions.begin(); itSession != lstSessions.end(); ++itSession)
				{
					if (std::find(App.m_oSessionList.begin(), App.m_oSessionList.end(), *itSession) == App.m_oSessionList.end())
						App.m_oSessionList.Add(*itSession);
				}
			}
			// Merge all sessions.
			else
			{
				for (CSessionList::iterator itSession = lstSessions.begin(); itSession != lstSessions.end(); ++itSession)
					App.m_oSessionList.Add(*itSession);
			}

			// Merge Task list.
			for (CTaskList::const_iterator itTask = lstTasks.begin(); itTask != lstTasks.end(); ++itTask)
				App.m_oTaskList.Add(*itTask);

			// Merge Location list.
			for (CLocnList::const_iterator itLocn = lstLocns.begin(); itLocn != lstLocns.end(); ++itLocn)
				App.m_oLocnList.Add(*itLocn);
		}
		// Shouldn't happen!
		else
		{
			ASSERT_FALSE();
		}

		// Remember settings.
		App.m_strImportFile = Dlg.m_strFileName;
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
	// Use desktop as dialog parent when in system tray.
	CWnd* pParent = (App.m_rMainWnd.IsVisible()) ? &App.m_rMainWnd : &CWnd::g_oDesktop;

	CClockInDlg	Dlg;

	if (Dlg.RunModal(*pParent) == IDOK)
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
	CString   strLocn   = App.m_pCurrSession->Location();

	// Check we haven't clocked in later than now.
	if (dtCurrent < App.m_pCurrSession->Start())
	{
		const char* pszMsg = "You cannot switch tasks because the time\n"
							 "you clocked in is later than the time now.";

		App.AlertMsg(pszMsg);
		return;
	}

	// Use desktop as dialog parent when in system tray.
	CWnd* pParent = (App.m_rMainWnd.IsVisible()) ? &App.m_rMainWnd : &CWnd::g_oDesktop;

	CSwitchTasksDlg	Dlg;
	
	if (Dlg.RunModal(*pParent) == IDOK)
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
	// Use desktop as dialog parent when in system tray.
	CWnd* pParent = (App.m_rMainWnd.IsVisible()) ? &App.m_rMainWnd : &CWnd::g_oDesktop;

	CClockOutDlg Dlg;
	
	if (Dlg.RunModal(*pParent) == IDOK)
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

		// Update status.
		App.m_AppWnd.m_StatusBar.Hint("Generating report...");

		// Setup the memory stream to report to.
		CBuffer		oBuffer;
		CMemStream	TxtStream(oBuffer);
		CTextReport Device(TxtStream);

		TxtStream.Create();

		// Do the report.
		App.ReportData(Device, Dlg.m_eGrouping, Dlg.m_FromDate, Dlg.m_ToDate);

		TxtStream.Close();

		// Update status.
		App.m_AppWnd.m_StatusBar.Hint("Report generated");

		// View report, if not empty.
		// NB: Empty report is just an EOS.
		if (TxtStream.Size() > 1)
		{
			CViewReportDlg Dlg(TxtStream);
			Dlg.RunModal(App.m_rMainWnd);
		}
		else
		{
			App.NotifyMsg("The report contained no entries.");
		}
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

		// Update status.
		App.m_AppWnd.m_StatusBar.Hint("Generating report...");

		// Setup the clipbaord stream.
		CClipboard Clipboard;
		CTextReport Device(Clipboard);

		Clipboard.Open(GENERIC_WRITE, CF_TEXT);

		// Do the report.
		App.ReportData(Device, Dlg.m_eGrouping, Dlg.m_FromDate, Dlg.m_ToDate);

		// NB: Empty report is just an EOS.
		if (Clipboard.Size() <= 1)
			App.NotifyMsg("The report contained no entries.");

		Clipboard.Close();

		// Update status.
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
		CPrinterDC	DC(App.m_oPrinter);

		// Update status.
		App.m_AppWnd.m_StatusBar.Hint("Generating report...");

		// Setup the printer to print to.
		CPrintReport Device(DC);
		
		// Do the report.
		App.ReportData(Device, Dlg.m_eGrouping, Dlg.m_FromDate, Dlg.m_ToDate);

		// Update status.
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

		// Update status.
		App.m_AppWnd.m_StatusBar.Hint("Generating report...");

		// Setup the file to report to.
		CFileReport Device(Dlg.m_strFileName);
		
		// Do the report.
		App.ReportData(Device, Dlg.m_eGrouping, Dlg.m_FromDate, Dlg.m_ToDate);

		// Update status.
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

		CIsSessionInRange oRange(CDateTime::Min(), CDateTime(Dlg.m_Date, CTime::Min()));
		CIter             oIter(App.m_oSessionList.begin());

		// Delete all sessions within the period.
		while ((oIter = std::find_if(oIter, App.m_oSessionList.end(), oRange)) != App.m_oSessionList.end())
			App.m_oSessionList.erase(oIter++);

		// Update dirty flag.
		App.m_bModified = true;
	
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
			App.m_oTaskList.Remove(*oIter);
		
		// Update dirty flag.
		App.m_bModified = true;

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
		// Remove all selected locations.
		for(CIter oIter = Dlg.m_LocnList.begin(); oIter != Dlg.m_LocnList.end(); ++oIter)
			App.m_oLocnList.Remove(*oIter);
		
		// Update dirty flag.
		App.m_bModified = true;

		// Update UI.
		UpdateUI();
	}
}

/******************************************************************************
** Method:		OnToolsOptions()
**
** Description:	Show the options dialog.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppCmds::OnToolsOptions()
{
	COptionsDlg Dlg;

	Dlg.RunModal(App.m_rMainWnd);
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
	bool bClockedIn = App.m_bClockedIn;

	App.m_AppWnd.m_Menu.EnableCmd(ID_SESSION_CLOCK_IN, !bClockedIn);
	App.m_AppWnd.m_ToolBar.m_ClockInBtn.Enable(!bClockedIn);
}

void CAppCmds::OnUISessionSwitchTasks()
{
	// Get current status.
	bool bClockedIn = App.m_bClockedIn;

	App.m_AppWnd.m_Menu.EnableCmd(ID_SESSION_SWITCH_TASKS, bClockedIn);
	App.m_AppWnd.m_ToolBar.m_SwitchTasksBtn.Enable(bClockedIn);
}

void CAppCmds::OnUISessionClockOut()
{
	// Get current status.
	bool bClockedIn = App.m_bClockedIn;

	App.m_AppWnd.m_Menu.EnableCmd(ID_SESSION_CLOCK_OUT, bClockedIn);
	App.m_AppWnd.m_ToolBar.m_ClockOutBtn.Enable(bClockedIn);
}

void CAppCmds::OnUIPruneSessions()
{
	bool bItems = (App.m_oSessionList.size() > 0);

	App.m_AppWnd.m_Menu.EnableCmd(ID_PRUNE_SESSIONS, bItems);
}

void CAppCmds::OnUIPruneTasks()
{
	bool bItems = (App.m_oTaskList.size() > 0);

	App.m_AppWnd.m_Menu.EnableCmd(ID_PRUNE_TASKS, bItems);
}

void CAppCmds::OnUIPruneLocations()
{
	bool bItems = (App.m_oLocnList.size() > 0);

	App.m_AppWnd.m_Menu.EnableCmd(ID_PRUNE_LOCNS, bItems);
}
