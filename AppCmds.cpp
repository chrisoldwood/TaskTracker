/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		APPCMDS.CPP
** COMPONENT:	The Application.
** DESCRIPTION:	CAppCmds class definition.
**
*******************************************************************************
*/

#include "Common.hpp"
#include "AppCmds.hpp"
#include "TaskTracker.hpp"
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
#include <algorithm>
#include <WCL/BusyCursor.hpp>
#include <WCL/File.hpp>
#include <WCL/FileException.hpp>
#include <WCL/Buffer.hpp>
#include <WCL/MemStream.hpp>
#include <WCL/Clipboard.hpp>
#include <WCL/PrinterDC.hpp>
#include "ReportOptions.hpp"
#include "TextReport.hpp"
#include "PrintReport.hpp"
#include "FileReport.hpp"

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

CAppCmds::CAppCmds(CAppWnd& appWnd)
	: CCmdControl(appWnd, IDR_APPTOOLBAR)
{
	// Define the command table.
	DEFINE_CMD_TABLE
		CMD_ENTRY(ID_FILE_EXPORT,			&CAppCmds::OnFileExport,			nullptr,							0)
		CMD_ENTRY(ID_FILE_IMPORT,			&CAppCmds::OnFileImport,			nullptr,							1)
		CMD_ENTRY(ID_FILE_EXIT,				&CAppCmds::OnFileExit,				nullptr,							-1)
		CMD_ENTRY(ID_SESSION_CLOCK_IN,		&CAppCmds::OnSessionClockIn,		&CAppCmds::OnUISessionClockIn,		2)
		CMD_ENTRY(ID_SESSION_SWITCH_TASKS,	&CAppCmds::OnSessionSwitchTasks,	&CAppCmds::OnUISessionSwitchTasks,	3)
		CMD_ENTRY(ID_SESSION_CLOCK_OUT,		&CAppCmds::OnSessionClockOut,		&CAppCmds::OnUISessionClockOut,		4)
		CMD_ENTRY(ID_SESSION_EDIT,			&CAppCmds::OnSessionEdit,			nullptr,							5)
		CMD_ENTRY(ID_REPORT_WINDOW,			&CAppCmds::OnReportWindow,			nullptr,							6)
		CMD_ENTRY(ID_REPORT_CLIPBOARD,		&CAppCmds::OnReportClipboard,		nullptr,							7)
		CMD_ENTRY(ID_REPORT_PRINT,			&CAppCmds::OnReportPrint,			nullptr,							8)
		CMD_ENTRY(ID_REPORT_FILE,			&CAppCmds::OnReportFile,			nullptr,							9)
		CMD_ENTRY(ID_PRUNE_SESSIONS,		&CAppCmds::OnPruneSessions,			&CAppCmds::OnUIPruneSessions,		-1)
		CMD_ENTRY(ID_PRUNE_TASKS,			&CAppCmds::OnPruneTasks,			&CAppCmds::OnUIPruneTasks,			-1)
		CMD_ENTRY(ID_PRUNE_LOCNS,			&CAppCmds::OnPruneLocations,		&CAppCmds::OnUIPruneLocations,		-1)
		CMD_ENTRY(ID_TOOLS_OPTIONS,			&CAppCmds::OnToolsOptions,			nullptr,							-1)
		CMD_ENTRY(ID_HELP_CONTENTS,			&CAppCmds::OnHelpContents,			nullptr,							10)
		CMD_ENTRY(ID_HELP_ABOUT,			&CAppCmds::OnHelpAbout,				nullptr,							-1)
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
	// Nothing to export?
	if (App.m_oSessionList.empty())
	{
		App.AlertMsg(TXT("There are no sessions to export."));
		return;
	}

	CExportDlg Dlg;

	// Get export settings.
	if (Dlg.RunModal(App.m_AppWnd) != IDOK)
		return;

	// Warn user if file exists.
	if ( (Dlg.m_strFileName.Exists())
	  && (App.QueryMsg(TXT("The file already exists:\n\n%s\n\nDo you want to overwrite it?"), Dlg.m_strFileName.c_str()) != IDYES) )
		return;

	CBusyCursor oBusyCursor;

	try
	{
		// Template shorthands.
		typedef CSessionList::iterator CIter;

		CFile             oFile;
		CDateTime         dtBegin(Dlg.m_dFromDate, CTime::Min());
		CDateTime         dtEnd(Dlg.m_dToDate, CTime::Max());
		CIsSessionInRange oRange(dtBegin, dtEnd);
		CIter             oIter(App.m_oSessionList.begin());

		// Open the file.
		oFile.Create(Dlg.m_strFileName);

		// For all sessions.
		while ((oIter = std::find_if(oIter, App.m_oSessionList.end(), oRange)) != App.m_oSessionList.end())
		{
			CString     strLine;
			CSessionPtr pSession = *oIter;
			CString     strTask  = pSession->Task();
			CString     strLocn  = pSession->Location();
			size_t      nPos;

			// Strip special CSV characters from task.
			while ((nPos = strTask.Find('\"')) != Core::npos)
				strTask.Delete(nPos);

			while ((nPos = strTask.Find(',')) != Core::npos)
				strTask.Delete(nPos);

			// Strip special CSV characters from location.
			while ((nPos = strLocn.Find('\"')) != Core::npos)
				strLocn.Delete(nPos);

			while ((nPos = strLocn.Find(',')) != Core::npos)
				strLocn.Delete(nPos);

			// Format as Start,End,Task,Location
			strLine.Format(TXT("\"%s\",\"%s\",\"%s\",\"%s\""), 
							pSession->Start().ToString(CDate::FMT_ISO, CDate::FMT_ISO).c_str(),
							pSession->Finish().ToString(CDate::FMT_ISO, CDate::FMT_ISO).c_str(),
							strTask.c_str(), strLocn.c_str());

			oFile.WriteLine(strLine, ANSI_TEXT);

			++oIter;
		}

		// Done.
		oFile.Close();

		// Remember settings.
		App.m_strExportFile = Dlg.m_strFileName;
	}
	catch(const CFileException& rException)
	{
		// Notify user.
		App.m_AppWnd.AlertMsg(TXT("%s"), rException.twhat());
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

	CBusyCursor oBusyCursor;

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
			CString strLine = oFile.ReadLine(ANSI_TEXT);

			// Count the number of fields.
			int nFields = static_cast<int>(strLine.Count(',')) + 1;

			// Expecting "Start,End,Task,Location".
			if (nFields != 4)
			{
				// Notify user.
				App.m_AppWnd.AlertMsg(TXT("Invalid number of fields in line: %d\n\n")
									  TXT("Found %d fields. Expected 4 [Start, End, Task, Location]\n\n")
									  TXT("%s"),
									  nLine, nFields, strLine.c_str());
				break;
			}

			// Find field separators.
			size_t nStartDateTime = Core::npos;
			size_t nEndDateTime   = strLine.Find(',', nStartDateTime + 1);
			size_t nTask          = strLine.Find(',', nEndDateTime   + 1);
			size_t nLocation      = strLine.Find(',', nTask          + 1);
			size_t nEOS           = strLine.Length();

			// Extract basic fields.
			CString strStartDateTime = strLine.Mid(nStartDateTime + 1, nEndDateTime - nStartDateTime - 1);
			CString strEndDateTime   = strLine.Mid(nEndDateTime   + 1, nTask        - nEndDateTime   - 1);
			CString strTask          = strLine.Mid(nTask          + 1, nLocation    - nTask          - 1);
			CString strLocation      = strLine.Mid(nLocation      + 1, nEOS         - nLocation      - 1);

			size_t nPos = Core::npos;

			// Trim quotes, if present.
			while ((nPos = strStartDateTime.Find('\"')) != Core::npos)
				strStartDateTime.Delete(nPos);

			while ((nPos = strEndDateTime.Find('\"')) != Core::npos)
				strEndDateTime.Delete(nPos);

			while ((nPos = strTask.Find('\"')) != Core::npos)
				strTask.Delete(nPos);

			while ((nPos = strLocation.Find('\"')) != Core::npos)
				strLocation.Delete(nPos);

			CDateTime dtStart, dtEnd;

			// Convert start date/time.
			if (!dtStart.FromString(strStartDateTime))
			{
				// Notify user.
				App.m_AppWnd.AlertMsg(TXT("Invalid 'Start' date/time format on line: %d\n\n%s"), nLine, strLine.c_str());
				break;
			}

			// Convert end date/time.
			if (!dtEnd.FromString(strEndDateTime))
			{
				// Notify user.
				App.m_AppWnd.AlertMsg(TXT("Invalid 'End' date/time format on line: %d\n\n%s"), nLine, strLine.c_str());
				break;
			}

			// Invalid dates?
			if (dtEnd < dtStart)
			{
				// Notify user.
				App.m_AppWnd.AlertMsg(TXT("'End' date/time precedes 'Start' date/time on line: %d\n\n%s"), nLine, strLine.c_str());
				break;
			}

			// Allocate a new session and initialise.
			CSessionPtr pSession(new CSession);

			pSession->Start (dtStart, strTask, strLocation);
			pSession->Finish(dtEnd,   strTask, strLocation);
			
			// Add to the temporary collections.
			lstSessions.Add(pSession);

			if (strTask != TXT(""))
				lstTasks.Add(strTask);

			if (strLocation != TXT(""))
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

		// Update state.
		App.m_bModified = true;
	}
	catch(const CFileException& rException)
	{
		// Notify user.
		App.m_AppWnd.AlertMsg(TXT("%s"), rException.twhat());
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
		const tchar* pszMsg = TXT("You cannot switch tasks because the time\n")
							  TXT("you clocked in is later than the time now.");

		App.AlertMsg(pszMsg);
		return;
	}

	// Use desktop as dialog parent when in system tray.
	CWnd* pParent = (App.m_rMainWnd.IsVisible()) ? &App.m_rMainWnd : &CWnd::g_oDesktop;

	CSwitchTasksDlg	Dlg;
	
	if (Dlg.RunModal(*pParent) == IDOK)
	{
		dtCurrent.Set();

		// Strip seconds as we only store HH:MM:00
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
**				NB: We only update the modified flag once here to try and avoid
**				the backup code firing whilst in the middle of edits.
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

	// Update application state.
	if (Dlg.m_bModified)
		App.m_bModified = Dlg.m_bModified;

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

	Dlg.m_strTitle = TXT("Report In Window");

	if (Dlg.RunModal(App.m_rMainWnd) == IDOK)
	{
		CBusyCursor BusyCursor;

		// Update status.
		App.m_AppWnd.m_StatusBar.Hint(TXT("Generating report..."));

		// Setup the memory stream to report to.
		CBuffer		oBuffer;
		CMemStream	TxtStream(oBuffer);

		// Setup the report options.
		CReportOptions oOptions;

		oOptions.m_pDevice       = CReportPtr(new CTextReport(TxtStream));	
		oOptions.m_eGrouping     = Dlg.m_eGrouping;
		oOptions.m_bShowSessions = Dlg.m_bShowSessions;
		oOptions.m_bShowIfEmpty  = Dlg.m_bShowIfEmpty;
		oOptions.m_bShowTotal    = Dlg.m_bShowTotal;

		TxtStream.Create();

		// Do the report.
		App.ReportData(oOptions, Dlg.m_FromDate, Dlg.m_ToDate);

		TxtStream.Close();

		// Update status.
		App.m_AppWnd.m_StatusBar.Hint(TXT("Report generated"));

		// View report, if not empty.
		// NB: Empty report is just an EOS.
		if (TxtStream.Size() > 1)
		{
			CViewReportDlg viewReportDlg(TxtStream);
			viewReportDlg.RunModal(App.m_rMainWnd);
		}
		else
		{
			App.NotifyMsg(TXT("The report contained no entries."));
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

	Dlg.m_strTitle = TXT("Report To Clipboard");

	if (Dlg.RunModal(App.m_rMainWnd) == IDOK)
	{
		CBusyCursor BusyCursor;

		// Update status.
		App.m_AppWnd.m_StatusBar.Hint(TXT("Generating report..."));

		// Setup the clipbaord stream.
		CClipboard Clipboard;

		// Setup the report options.
		CReportOptions oOptions;

		oOptions.m_pDevice       = CReportPtr(new CTextReport(Clipboard));	
		oOptions.m_eGrouping     = Dlg.m_eGrouping;
		oOptions.m_bShowSessions = Dlg.m_bShowSessions;
		oOptions.m_bShowIfEmpty  = Dlg.m_bShowIfEmpty;
		oOptions.m_bShowTotal    = Dlg.m_bShowTotal;

		Clipboard.Open(GENERIC_WRITE, CF_TEXT);

		// Do the report.
		App.ReportData(oOptions, Dlg.m_FromDate, Dlg.m_ToDate);

		// NB: Empty report is just an EOS.
		if (Clipboard.Size() <= 1)
			App.NotifyMsg(TXT("The report contained no entries."));

		Clipboard.Close();

		// Update status.
		App.m_AppWnd.m_StatusBar.Hint(TXT("Report generated"));
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
		App.m_AppWnd.m_StatusBar.Hint(TXT("Generating report..."));

		// Setup the report options.
		CReportOptions oOptions;

		oOptions.m_pDevice       = CReportPtr(new CPrintReport(DC));	
		oOptions.m_eGrouping     = Dlg.m_eGrouping;
		oOptions.m_bShowSessions = Dlg.m_bShowSessions;
		oOptions.m_bShowIfEmpty  = Dlg.m_bShowIfEmpty;
		oOptions.m_bShowTotal    = Dlg.m_bShowTotal;

		// Do the report.
		App.ReportData(oOptions, Dlg.m_FromDate, Dlg.m_ToDate);

		// Update status.
		App.m_AppWnd.m_StatusBar.Hint(TXT("Report generated"));
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
		App.m_AppWnd.m_StatusBar.Hint(TXT("Generating report..."));

		// Setup the report options.
		CReportOptions oOptions;

		oOptions.m_pDevice       = CReportPtr(new CFileReport(Dlg.m_strFileName));	
		oOptions.m_eGrouping     = Dlg.m_eGrouping;
		oOptions.m_bShowSessions = Dlg.m_bShowSessions;
		oOptions.m_bShowIfEmpty  = Dlg.m_bShowIfEmpty;
		oOptions.m_bShowTotal    = Dlg.m_bShowTotal;

		// Do the report.
		App.ReportData(oOptions, Dlg.m_FromDate, Dlg.m_ToDate);

		// Update status.
		App.m_AppWnd.m_StatusBar.Hint(TXT("Report generated"));
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
	ASSERT(!App.m_oSessionList.empty());

	CPruneSessionsDlg Dlg;
	
	if (Dlg.RunModal(App.m_rMainWnd) == IDOK)
	{
		// Template shorthands.
		typedef CSessionList::iterator CIter;

		CDateTime         dtBegin(Dlg.m_dFromDate, CTime::Min());
		CDateTime         dtEnd(Dlg.m_dToDate, CTime::Max());
		CIsSessionInRange oRange(dtBegin, dtEnd);
		CIter             oIter(App.m_oSessionList.begin());

		// Delete all sessions within the period.
		while ((oIter = std::find_if(oIter, App.m_oSessionList.end(), oRange)) != App.m_oSessionList.end())
			oIter = App.m_oSessionList.erase(oIter);

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
** Method:		OnHelpContents()
**
** Description:	Show the help file.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppCmds::OnHelpContents()
{
	CBusyCursor oBusyCursor;

	CPath strHelpFile(CPath::ApplicationDir(), CTaskTracker::HELPFILE);

	::ShellExecute(nullptr, nullptr, strHelpFile, nullptr, nullptr, SW_SHOW);
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
