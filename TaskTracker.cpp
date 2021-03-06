/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		TASKTRACKER.CPP
** COMPONENT:	The Application.
** DESCRIPTION:	The CTaskTracker class definition.
**
*******************************************************************************
*/

#include "Common.hpp"
#include "Tasktracker.hpp"
#include <stdio.h>
#include <algorithm>
#include <WCL/BusyCursor.hpp>
#include <WCL/File.hpp>
#include <WCL/FileException.hpp>
#include "ReportOptions.hpp"

/******************************************************************************
**
** Global variables.
**
*******************************************************************************
*/

// "The" application object.
CTaskTracker App;

/******************************************************************************
**
** Local constants.
**
*******************************************************************************
*/

// The current data file version.
static const uint DAT_FILE_VERSION = 35;

// The data filename.
static const tchar* DAT_FILE_NAME = TXT("TaskTrak.dat");

// The currrent cfg file version.
static const tchar* INI_FILE_VER_40 = TXT("4.0");

// The previous cfg file versions (none).
static const tchar* INI_FILE_VER_35 = TXT("");

// The cfg filename.
static const tchar* INI_FILE_NAME = TXT("TaskTrak.ini");

/******************************************************************************
**
** Class constants.
**
*******************************************************************************
*/

#ifdef _DEBUG
const tchar* CTaskTracker::VERSION = TXT("v4.0 [Debug]");
#else
const tchar* CTaskTracker::VERSION = TXT("v4.0");
#endif

const tchar* CTaskTracker::HELPFILE = TXT("TaskTrak.mht");

/******************************************************************************
** Method:		Constructor
**
** Description:	Default constructor.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

CTaskTracker::CTaskTracker()
	: CApp(m_AppWnd, m_AppCmds)
	, m_AppWnd(m_MainThread, m_AppCmds)
	, m_AppCmds(m_AppWnd)
	, m_bClockedIn(false)
	, m_pCurrSession(nullptr)
	, m_strLastTask(TXT(""))
	, m_strLastLocn(TXT(""))
	, m_bModified(false)
	, m_eDefGrouping(Ungrouped)
	, m_eDefPeriod(All)
	, m_bDefShowSessions(true)
	, m_bDefShowIfEmpty(true)
	, m_bDefShowTotal(true)
	, m_eLenFormat(HoursMins)
	, m_eWeekOrder(MonToSun)
	, m_bMinToTray(false)
	, m_strRptDlgFont(TXT("ANSI_FIXED_FONT"))
	, m_bCheckOverlap(true)
{
}

/******************************************************************************
** Method:		Destructor
**
** Description:	Cleanup.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

CTaskTracker::~CTaskTracker()
{
}

/******************************************************************************
** Method:		OnOpen()
**
** Description:	Initialises the application.
**
** Parameters:	None.
**
** Returns:		true or false.
**
*******************************************************************************
*/

bool CTaskTracker::OnOpen()
{
	// Only allow a single instance to run.
	if (CAppWnd::RestorePrevInstance())
		return false;

	// Set the app title.
	m_strTitle = TXT("Task Tracker");

	// Create .ini file path.
	m_IniFile.m_strPath  = CPath::ApplicationDir() / INI_FILE_NAME;

	// Load settings.
	LoadDefaults();
	
	// Create the main window.
	if (!m_AppWnd.Create())
		return false;

	// Show it.
	m_AppWnd.Centre();
	m_AppWnd.Show(m_iCmdShow);

	CBusyCursor BusyCursor;

	// Load the data.
	LoadData();

	// Update UI.
	m_AppCmds.InitialiseUI();
	m_AppWnd.m_AppDlg.Update();

	return true;
}

/******************************************************************************
** Method:		OnClose()
**
** Description:	Cleans up the application resources.
**
** Parameters:	None.
**
** Returns:		true or false.
**
*******************************************************************************
*/

bool CTaskTracker::OnClose()
{
	// Save settings.
	SaveDefaults();

	// Free up strings.
	m_strLastTask = TXT("");
	m_strLastLocn = TXT("");

	return true;
}

/******************************************************************************
** Method:		ClockIn()
**
** Description:	Add a new session to the session list and update the last task
**				member.
**
** Parameters:	dtIn		The date and time of clocking in.
**				strTask		The task about to be done.
**				strLocn		The location of the task.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CTaskTracker::ClockIn(const CDateTime& dtIn, const CString& strTask, const CString& strLocn)
{
	// Create a new session.
	m_pCurrSession = CSessionPtr(new CSession);

	// Initialise.
	m_pCurrSession->Start(dtIn, strTask, strLocn);

	// Add task to list if set.
	if (strTask != TXT(""))
		m_oTaskList.Add(strTask);

	// Add location to list if set.
	if (strLocn != TXT(""))
		m_oLocnList.Add(strLocn);

	// Update state.
	m_strLastTask = strTask;
	m_strLastLocn = strLocn;
	m_bClockedIn  = true;
	m_bModified   = true;

	m_AppCmds.UpdateUI();
	m_AppWnd.UpdateTrayIconTip();
}

/******************************************************************************
** Method:		ClockOut()
**
** Description:	Fill in the current session and update the last task member.
**
** Parameters:	dtOut		The date and time of clocking out.
**				strTask		The task just done.
**				strLocn		The location of the task.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CTaskTracker::ClockOut(const CDateTime& dtOut, const CString& strTask, const CString& strLocn)
{
	// Complete session.
	m_pCurrSession->Finish(dtOut, strTask, strLocn);

	// Add to list.
	m_oSessionList.Add(m_pCurrSession);
	
	// Reset current session.
	m_pCurrSession.reset();

	// Add task to list if set.
	if (strTask != TXT(""))
		m_oTaskList.Add(strTask);
	
	// Add location to list if set.
	if (strLocn != TXT(""))
		m_oLocnList.Add(strLocn);

	// Update state.
	m_strLastTask = strTask;
	m_strLastLocn = strLocn;
	m_bClockedIn  = false;
	m_bModified   = true;

	m_AppCmds.UpdateUI();
	m_AppWnd.UpdateTrayIconTip();
}

/******************************************************************************
** Method:		TotalForPeriod()
**
** Description:	Calculate the total time for those sessions within the
**				period specified.
**
** Parameters:	dtStart		The period start datetime.
**				dtEnd		The period end datetime.
**
** Returns:		The time in minutes.
**
*******************************************************************************
*/

ulong CTaskTracker::TotalForPeriod(const CDateTime& dtStart, const CDateTime& dtEnd) const
{
	// Template shorthands.
	typedef CSessionList::const_iterator CIter;

	CIsSessionInRange oRange(dtStart, dtEnd);
	CIter             oIter(m_oSessionList.begin());
	ulong			  lTotal = 0;

	// For all sessions within the period.
	while ((oIter = std::find_if(oIter, m_oSessionList.end(), oRange)) != m_oSessionList.end())
	{
		lTotal += (*oIter)->Length();
		++oIter;
	}
	
	// Current session in period?
	if ( (m_bClockedIn)
	  && (m_pCurrSession->Start() >= dtStart)
	  && (m_pCurrSession->Start() <  dtEnd) )
	{
		CSession 	CurrSession = *m_pCurrSession;
		CDateTime	dtCurrent;
		
		// Create session based on current time.
		dtCurrent.Set();
		CurrSession.Finish(dtCurrent, TXT(""), TXT(""));
		
		lTotal += CurrSession.Length();
	}
	
	return lTotal;
}

/******************************************************************************
** Method:		TotalForDay()
**
** Description:	Calculate the total time for those sessions within the
**				day specified.
**
** Parameters:	rDate	The day required.
**
** Returns:		The time in minutes.
**
*******************************************************************************
*/

ulong CTaskTracker::TotalForDay(const CDate& rDate) const
{
	CDateTime	dtStart;
	CDateTime	dtEnd;
	CDate		Date = rDate;

	// Create day limits.
	dtStart.Date(Date);
	Date += 1;
	dtEnd.Date(Date);

	return TotalForPeriod(dtStart, dtEnd);
}

/******************************************************************************
** Method:		TotalForWeek()
**
** Description:	Calculate the total time for those sessions within the
**				week specified.
**
** Parameters:	rDate	A date within the week required.
**
** Returns:		The time in minutes.
**
*******************************************************************************
*/

ulong CTaskTracker::TotalForWeek(const CDate& rDate) const
{
	CDateTime	dtStart;
	CDateTime	dtEnd;
	CDate		Date = rDate;

	// Create week limits.
	Date  = GetWeekStart(Date);
	dtStart.Date(Date);
	Date += 7;
	dtEnd.Date(Date);

	return TotalForPeriod(dtStart, dtEnd);
}

/******************************************************************************
** Method:		TotalForMonth()
**
** Description:	Calculate the total time for those sessions within the
**				month specified.
**
** Parameters:	rDate	A date within the month required.
**
** Returns:		The time in minutes.
**
*******************************************************************************
*/

ulong CTaskTracker::TotalForMonth(const CDate& rDate) const
{
	CDateTime	dtStart;
	CDateTime	dtEnd;
	int			iDate, iMonth, iYear;
	int			iNumDays;
	CDate		Date = rDate;
	
	// Get limits.
	rDate.Get(iDate, iMonth, iYear);
	iNumDays = rDate.DaysInMonth();

	// Create month limits.
	Date -= iDate - 1;
	dtStart.Date(Date);
	Date += iNumDays;
	dtEnd.Date(Date);

	return TotalForPeriod(dtStart, dtEnd);
}

/******************************************************************************
** Method:		TotalOverall()
**
** Description:	Calculates the total for all sessions.
**
** Parameters:	None.
**
** Returns:		The time in minutes.
**
*******************************************************************************
*/

ulong CTaskTracker::TotalOverall() const
{
	return TotalForPeriod(CDateTime::Min(), CDateTime::Max());
}

/******************************************************************************
** Method:		LoadData()
**
** Description:	Load the applications' data.
**
** Parameters:	None.
**
** Returns:		true or false.
**
*******************************************************************************
*/

bool CTaskTracker::LoadData()
{
	CFile AppFile;
	CPath AppFilePath;

	// Setup up path.
	AppFilePath = CPath::ApplicationDir() / DAT_FILE_NAME;

	// Data file exists yet?
	if (!AppFilePath.Exists())
		return true;

	try
	{
		// Open file.
		AppFile.Open(AppFilePath, GENERIC_READ);

		byte	szMagic[2];
		uint16	iVersion;

		// Read magic and version number.
		AppFile.Read(szMagic, sizeof(szMagic));
		AppFile.Read(&iVersion, sizeof(iVersion));
		
		// Check header.
		if ( (szMagic[0] != 'T') || (szMagic[1] != 'T') )
			throw CFileException(CFileException::E_FORMAT_INVALID, AppFilePath, ERROR_FILE_CORRUPT);
		
		// Check version number.
		if (iVersion != DAT_FILE_VERSION)
			throw CFileException(CFileException::E_VERSION_INVALID, AppFilePath, NO_ERROR);
		
		// Read the actual data.
		ReadData(AppFile);
			
		// Done.
		AppFile.Close();
	}
	catch(const CFileException& rException)
	{
		// Notify user.
		m_AppWnd.AlertMsg(TXT("%s"), rException.twhat());
		return false;
	}

	return true;
}

/******************************************************************************
** Method:		ReadData()
**
** Description:	Read the actual data from the file.
**
** Parameters:	rFile	The file to read from.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CTaskTracker::ReadData(CFile& rFile)
{
	// Read current status.
	rFile >> m_bClockedIn;

	if (m_bClockedIn)
	{
		m_pCurrSession = CSessionPtr(new CSession);

		// Read current session.
		rFile >> *m_pCurrSession;
	}

	// Read last used items.
	rFile >> m_strLastTask;
	rFile >> m_strLastLocn;
	
	// Read collections.
	rFile >> m_oSessionList;
	rFile >> m_oTaskList;
	rFile >> m_oLocnList;
}

/******************************************************************************
** Method:		SaveData()
**
** Description:	Save the applications' data.
**
** Parameters:	None.
**
** Returns:		true or false.
**
*******************************************************************************
*/

bool CTaskTracker::SaveData()
{
	ASSERT(m_bModified);

	CFile AppFile;
	CPath AppFilePath;

	// Setup up path.
	AppFilePath = CPath::ApplicationDir() / DAT_FILE_NAME;

	try
	{
		// Create file.
		AppFile.Create(AppFilePath);

		byte	szMagic[2] = { 'T', 'T' };
		uint16	iVersion   = DAT_FILE_VERSION;

		// Write magic and version number.
		AppFile.Write(szMagic, sizeof(szMagic));
		AppFile.Write(&iVersion, sizeof(iVersion));

		// Write the actual data.
		WriteData(AppFile);

		// Done.
		AppFile.Close();

		// Reset state.
		m_bModified = false;
	}
	catch(const CFileException& rException)
	{
		// Notify user.
		m_AppWnd.AlertMsg(TXT("%s"), rException.twhat());
		return false;
	}

	return true;
}

/******************************************************************************
** Method:		WriteData()
**
** Description:	Write the actual data to the file.
**
** Parameters:	rFile	The file to write to.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CTaskTracker::WriteData(CFile& rFile)
{
	// Write current status.
	rFile << m_bClockedIn;

	// Write current session.
	if (m_bClockedIn)
		rFile << *m_pCurrSession;

	// Write last used items.
	rFile << m_strLastTask;
	rFile << m_strLastLocn;
	
	// Write collections.
	rFile << m_oSessionList;
	rFile << m_oTaskList;
	rFile << m_oLocnList;
}

/******************************************************************************
** Method:		ReportData()
**
** Description:	Generate a report and output to a device.
**
** Parameters:	oOptions	The report options.
**				oFromDate	The report start date.
**				oToDate		The report end date.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CTaskTracker::ReportData(CReportOptions& oOptions, const CDate& oFromDate,
								const CDate& oToDate) const
{
	ASSERT(oOptions.m_pDevice.get() != nullptr);
	ASSERT(oFromDate <= oToDate);

	ulong lSessions  = 0;
	ulong lTotalMins = 0;
	bool  bOkay      = false;

	// Initialise the device.
	if (!oOptions.m_pDevice->Init())
		return;

	// Set up datetime limits.
	CDateTime dtFrom(oFromDate, CTime(0, 0, 0   ));
	CDateTime dtTo  (oToDate,   CTime(23, 59, 59));

	// Check grouping.
	switch(oOptions.m_eGrouping)
	{
		case Ungrouped:
			bOkay = ReportUngrouped(oOptions, dtFrom, dtTo, lSessions, lTotalMins);
			break;
			
		case ByWeek:
			bOkay = ReportByWeek(oOptions, dtFrom, dtTo, lSessions, lTotalMins);
			break;
			
		case ByMonth:
			bOkay = ReportByMonth(oOptions, dtFrom, dtTo, lSessions, lTotalMins);
			break;
			
		case ByTask:
			bOkay = ReportByTask(oOptions, dtFrom, dtTo, lSessions, lTotalMins);
			break;
			
		default:
			ASSERT_FALSE();
			break;
	}

	// Display overall total?
	if (oOptions.m_bShowTotal && bOkay)
	{
		// Output total.
		CString strTotal = CString::Fmt(TXT("Total Hours: %s"), App.MinsToStr(lTotalMins).c_str());

		oOptions.m_pDevice->SendText(strTotal);
	}

	// Cleanup the device.
	oOptions.m_pDevice->Term();
}

/******************************************************************************
** Method:		ReportUngrouped()
**
** Description:	Report all data ungrouped.
**
** Parameters:	oOptions		The report options.
**				dtFrom			The starting date.
**				dtTo			The end date.
**				rlSessions		The number of sessions in the group..
**				rlTotalMins		The total time in minutes.
**
** Returns:		true or false.
**
*******************************************************************************
*/

bool CTaskTracker::ReportUngrouped(CReportOptions& oOptions, const CDateTime& dtFrom,
							const CDateTime& dtTo, ulong& rlSessions, ulong& rlTotalMins) const
{
	// Template shorthands.
	typedef CSessionList::const_iterator CIter;

	// Initialise totals.
	rlSessions  = 0;
	rlTotalMins = 0;

	CIsSessionInRange oRange(dtFrom, dtTo);
	CIter             oIter(m_oSessionList.begin());
    CDate             DateDone;

	// For all sessions in range...
	while ((oIter = std::find_if(oIter, m_oSessionList.end(), oRange)) != m_oSessionList.end())
	{
		CSessionPtr pSession = *oIter;

		// New day?
		if (pSession->Start().Date() > DateDone)
		{
			ulong lDaySessions ;
			ulong lDayTotal;
	
			// Report days' data.
			if (!ReportDay(oOptions, pSession->Start().Date(), lDaySessions, lDayTotal))
				return false;
				
			DateDone = pSession->Start().Date();

			// Update totals.
			rlSessions  += lDaySessions;
			rlTotalMins += lDayTotal;
		}

		++oIter;
	}

	return true;
}

/******************************************************************************
** Method:		ReportByWeek()
**
** Description:	Report all data ordered by weeks.
**
** Parameters:	oOptions		The report options.
**				dtFrom			The starting date.
**				dtTo			The end date.
**				rlSessions		The number of sessions in the group..
**				rlTotalMins		The total time in minutes.
**
** Returns:		true or false.
**
*******************************************************************************
*/

bool CTaskTracker::ReportByWeek(CReportOptions& oOptions, const CDateTime& dtFrom,
						const CDateTime& dtTo, ulong& rlSessions, ulong& rlTotalMins) const
{
	// Template shorthands.
	typedef CSessionList::const_iterator CIter;

	CDateTime dtStart;
	CDateTime dtEnd;

	// Initialise totals.
	rlSessions  = 0;
	rlTotalMins = 0;

	CIsSessionInRange oRange(dtFrom, dtTo);
	CIter             oIter(m_oSessionList.begin());

	// For all sessions in range...
	while ((oIter = std::find_if(oIter, m_oSessionList.end(), oRange)) != m_oSessionList.end())
	{
		CSessionPtr pSession = *oIter;

		// Skip sessions still in current week.
		if ( (pSession->Start() >= dtStart) && (pSession->Start() < dtEnd) )
		{
			++oIter;
			continue;
		}

		CDate	Date = pSession->Start().Date();
		int		iDay = Date.DayOfWeek();
		
		// Create week limits.
		Date  = GetWeekStart(Date);
		dtStart.Date(Date);
		Date += 6;
		dtEnd.Date(Date);
		dtEnd.Time(CTime(23, 59, 59));

		// Create enumerator for the week.
		CIsSessionInRange oWeekRange(dtStart, dtEnd);
		CIter             oWeekIter(m_oSessionList.begin());
		ulong             lWeekSessions = 0;
		ulong			  lWeekTotal    = 0;

		// Get total for the week.
		while ((oWeekIter = std::find_if(oWeekIter, m_oSessionList.end(), oWeekRange)) != m_oSessionList.end())
		{
			lWeekSessions++;
			lWeekTotal += (*oWeekIter)->Length();
			++oWeekIter;
		}

		// Output week heading.
		CString	strStartDate = dtStart.Date().ToString(CDate::FMT_WIN_SHORT);
		CString strEndDate   = dtEnd.Date().ToString(CDate::FMT_WIN_SHORT);
		CString strLen       = App.MinsToStr(lWeekTotal);
		CString strHeading   = CString::Fmt(TXT("Week: %s to %s (Total: %s)"), strStartDate.c_str(), strEndDate.c_str(), strLen.c_str());

		if (!oOptions.m_pDevice->SendHeading(strHeading))
			return false;

		if (!oOptions.m_pDevice->SendLineBreak())
			return false;

		// Get date for week start.
		Date = dtStart.Date();
		
		// For all days in the week.
		for(iDay = 0; iDay < 7; iDay++, Date += 1)
		{
			ulong lDaySessions;
			ulong lDayTotal;

			// Report data for the day..
			if (!ReportDay(oOptions, Date, lDaySessions, lDayTotal))
				return false;
		}
		
		// End of week.
		if (!oOptions.m_pDevice->SendLineBreak())
			return false;
		
		// Update totals.
		rlSessions  += lWeekSessions;
		rlTotalMins += lWeekTotal;

		++oIter;
	}

	return true;
}

/******************************************************************************
** Method:		ReportByMonth()
**
** Description:	Report all data ordered by months.
**
** Parameters:	oOptions		The report options.
**				dtFrom			The starting date.
**				dtTo			The end date.
**				rlSessions		The number of sessions in the group..
**				rlTotalMins		The total time in minutes.
**
** Returns:		true or false.
**
*******************************************************************************
*/

bool CTaskTracker::ReportByMonth(CReportOptions& oOptions, const CDateTime& dtFrom,
							const CDateTime& dtTo, ulong& rlSessions, ulong& rlTotalMins) const
{
	// Template shorthands.
	typedef CSessionList::const_iterator CIter;

	CDateTime dtStart;
	CDateTime dtEnd;

	// Initialise totals.
	rlSessions  = 0;
	rlTotalMins = 0;

	CIsSessionInRange oRange(dtFrom, dtTo);
	CIter             oIter(m_oSessionList.begin());

	// For all sessions in range...
	while ((oIter = std::find_if(oIter, m_oSessionList.end(), oRange)) != m_oSessionList.end())
	{
		CSessionPtr pSession = *oIter;

		// Skip sessions still in current month.
		if ( (pSession->Start() >= dtStart) && (pSession->Start() < dtEnd) )
		{
			++oIter;
			continue;
		}

		CDate	Date = pSession->Start().Date();
		int		iDate, iMonth, iYear;
		int 	iNumDays;

		// Create month limits.
		Date.Get(iDate, iMonth, iYear);
		iNumDays = Date.DaysInMonth();
		
		Date -= iDate - 1;
		dtStart.Date(Date);
		Date += iNumDays - 1;
		dtEnd.Date(Date);
		dtEnd.Time(CTime(23, 59, 59));

		// Create enumerator for the month.
		CIsSessionInRange oMonthRange(dtStart, dtEnd);
		CIter             oMonthIter(m_oSessionList.begin());
		ulong             lMonthSessions = 0;
		ulong			  lMonthTotal    = 0;
		
		// Get total for the month.
		while ((oMonthIter = std::find_if(oMonthIter, m_oSessionList.end(), oMonthRange)) != m_oSessionList.end())
		{
			lMonthTotal += (*oMonthIter)->Length();
			++oMonthIter;
		}

		// Output month heading.
		CString	strStartDate = dtStart.Date().ToString(CDate::FMT_WIN_SHORT);
		CString strEndDate   = dtEnd.Date().ToString(CDate::FMT_WIN_SHORT);
		CString strLen       = App.MinsToStr(lMonthTotal);
		CString strHeading   = CString::Fmt(TXT("Month: %s to %s (Total: %s)"), strStartDate.c_str(), strEndDate.c_str(), strLen.c_str());

		if (!oOptions.m_pDevice->SendHeading(strHeading))
			return false;

		if (!oOptions.m_pDevice->SendLineBreak())
			return false;

		// Get date for month start.
		Date = dtStart.Date();
		
		// For all days in the month.
		for(int iDay = 0; iDay < iNumDays; iDay++, Date += 1)
		{
			ulong lDaySessions;
			ulong lDayTotal;

			// Report data for the day..
			if (!ReportDay(oOptions, Date, lDaySessions, lDayTotal))
				return false;
		}
		
		// End of month.
		if (!oOptions.m_pDevice->SendLineBreak())
			return false;
		
		// Update total.
		rlSessions  += lMonthSessions;
		rlTotalMins += lMonthTotal;

		++oIter;
	}

	return true;
}

/******************************************************************************
** Method:		ReportByTask()
**
** Description:	Report all data by its task.
**
** Parameters:	oOptions		The report options.
**				dtFrom			The starting date.
**				dtTo			The end date.
**				rlSessions		The number of sessions in the group..
**				rlTotalMins		The total time in minutes.
**
** Returns:		true or false.
**
*******************************************************************************
*/

bool CTaskTracker::ReportByTask(CReportOptions& oOptions, const CDateTime& dtFrom,
						const CDateTime& dtTo, ulong& rlSessions, ulong& rlTotalMins) const
{
	// Template shorthands.
	typedef CTaskList::const_iterator CTaskIter;
	typedef CSessionList::const_iterator CSessIter;

	// Initialise totals.
	rlSessions  = 0;
	rlTotalMins = 0;
	
	// For all tasks.
	for(CTaskIter oIter = App.m_oTaskList.begin(); oIter != App.m_oTaskList.end(); ++oIter)
	{
		CSessIter         oTotalIter(m_oSessionList.begin());
		CIsSessionInRange oTotalRange(dtFrom, dtTo);
		ulong             lSessions = 0;
		ulong			  lTotal    = 0;
	
		// Get length of all sessions for task.
		while ((oTotalIter = std::find_if(oTotalIter, m_oSessionList.end(), oTotalRange)) != m_oSessionList.end())
		{
			CSessionPtr pSession = *oTotalIter;

			if (pSession->Task() == (*oIter))
			{
				lSessions++;
				lTotal += pSession->Length();
			}

			++oTotalIter;
		}
		
		// Update grand total.
		rlSessions  += lSessions;
		rlTotalMins += lTotal;

		if ( (lSessions > 0) || (oOptions.m_bShowIfEmpty) )
		{
			// Output task heading.
			CString strHeading = CString::Fmt(TXT("%s (Total: %s)"), (*oIter).c_str(), App.MinsToStr(lTotal).c_str());

			// Heading style depends on whether sessions follow.
			if (oOptions.m_bShowSessions)
			{
				if (!oOptions.m_pDevice->SendHeading(strHeading))
					return false;

				if (!oOptions.m_pDevice->SendLineBreak())
					return false;
			}
			else
			{
				if (!oOptions.m_pDevice->SendText(strHeading))
					return false;
			}

			// Write each session?
			if (oOptions.m_bShowSessions)
			{
				oTotalIter = m_oSessionList.begin();

				// Output all sessions for task.
				while ((oTotalIter = std::find_if(oTotalIter, m_oSessionList.end(), oTotalRange)) != m_oSessionList.end())
				{
					CSessionPtr pSession = *oTotalIter;

					if (pSession->Task() == (*oIter))
					{
						// Output session.
						CString strDay   = pSession->Start().Date().DayOfWeekStr(false);
						CString strDate  = pSession->Start().Date().ToString(CDate::FMT_WIN_SHORT);
						CString	strStart = pSession->Start().Time().ToString(CDate::FMT_WIN_SHORT);
						CString	strEnd   = pSession->Finish().Time().ToString(CDate::FMT_WIN_SHORT);
						CString strLen   = App.MinsToStr(pSession->Length());
						CString strText  = CString::Fmt(TXT("%s %s from %s to %s for %s"), strDay.c_str(), strDate.c_str(), strStart.c_str(), strEnd.c_str(), strLen.c_str());
					
						if (!oOptions.m_pDevice->SendText(strText))
							return false;
					}

					++oTotalIter;
				}

				// End of task.
				if ( (!oOptions.m_pDevice->SendLineBreak())
				|| (!oOptions.m_pDevice->SendLineBreak()) )
					return false;
			}
		}
	}
	
	return true;
}

/******************************************************************************
** Method:		ReportDay()
**
** Description:	Report all data for the day to the device.
**
** Parameters:	oOptions		The report options.
**				rDate			The date of the day to report.
**				rlSessions		The number of sessions on the day.
**				rlTotalMins		The total time for all sessions on the day.
**
** Returns:		true or false.
**
*******************************************************************************
*/

bool CTaskTracker::ReportDay(CReportOptions& oOptions, const CDate& rDate,
								ulong& rlSessions, ulong& rlTotalMins) const
{
	// Template shorthands.
	typedef CSessionList::const_iterator CIter;

	CDateTime	dtStart;
	CDateTime	dtEnd;

	// Create limits of day.
	dtStart.Date(rDate);
	dtEnd.Date(rDate);
	dtEnd.Time(CTime(23, 59, 59));

	// Initialise totals.
	rlSessions  = 0;
	rlTotalMins = 0;

	CIter             oIter(m_oSessionList.begin());
	CIsSessionInRange oRange(dtStart, dtEnd);

	// Get length of all sessions on the day.
	while ((oIter = std::find_if(oIter, m_oSessionList.end(), oRange)) != m_oSessionList.end())
	{
		rlSessions++;
		rlTotalMins += (*oIter)->Length();

		++oIter;
	}

	// Write sessions details, if requested.
	if ( (rlSessions > 0) || (oOptions.m_bShowIfEmpty) )
	{
		CString strDay  = rDate.DayOfWeekStr(false);
		CString	strDate = rDate.ToString(CDate::FMT_WIN_SHORT);
		CString strLen  = App.MinsToStr(rlTotalMins);
		CString strText = CString::Fmt(TXT("%s %s (Total: %s)"), strDay.c_str(), strDate.c_str(), strLen.c_str());

		if (!oOptions.m_pDevice->SendText(strText))
			return false;

		// Write each session?
		if (oOptions.m_bShowSessions)
		{
			oIter = m_oSessionList.begin();

			// Report all sessions on the day.
			while ((oIter = std::find_if(oIter, m_oSessionList.end(), oRange)) != m_oSessionList.end())
			{
				if (!ReportSession(oOptions, *oIter))
					return false;

				++oIter;
			}

			// End of day.
			if (!oOptions.m_pDevice->SendLineBreak())
				return false;
		}
	}

	return true;
}

/******************************************************************************
** Method:		ReportSession()
**
** Description:	Report a session to the device.
**
** Parameters:	oOptions		The report options.
**				pSession		The session to report.
**
** Returns:		true or false.
**
*******************************************************************************
*/

bool CTaskTracker::ReportSession(CReportOptions& oOptions, const CSessionPtr& pSession) const
{
	CString	strStart = pSession->Start().Time().ToString(CTime::FMT_WIN_SHORT);
	CString	strEnd   = pSession->Finish().Time().ToString(CTime::FMT_WIN_SHORT);
	CString strTask  = pSession->Task();
	CString strLocn  = pSession->Location();
	ulong	lLen     = pSession->Length();

	// Format core data.
	CString strText = CString::Fmt(TXT("%s to %s for %s"), strStart.c_str(), strEnd.c_str(), App.MinsToStr(lLen).c_str());

	// Append task, if supplied.
	if (strTask != TXT(""))
		strText += TXT(" on ") + strTask;

	// Append location, if supplied.
	if (strLocn != TXT(""))
		strText += TXT(" at ") + strLocn;

	return oOptions.m_pDevice->SendText(strText);
}

/******************************************************************************
** Method:		LoadDefaults()
**
** Description:	Load the default settings.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CTaskTracker::LoadDefaults()
{
	// Read the file version.
	CString strVer = m_IniFile.ReadString(TXT("Version"), TXT("Version"), INI_FILE_VER_35);

	// Fix-up old .ini file settings.
//	if (strVer != INI_FILE_VER_40)
//	{
//	}

	// Load UI settings.
	m_eLenFormat    = static_cast<LenFormat>(m_IniFile.ReadInt(TXT("Prefs"), TXT("LenFormat"), m_eLenFormat));
	m_eWeekOrder    = static_cast<WeekOrder>(m_IniFile.ReadInt(TXT("Prefs"), TXT("WeekOrder"), m_eWeekOrder));
	m_bMinToTray    = m_IniFile.ReadBool(TXT("Prefs"), TXT("MinToTray"), m_bMinToTray);
	m_bCheckOverlap = m_IniFile.ReadBool(TXT("Prefs"), TXT("CheckOverlap"), m_bCheckOverlap);
	m_rcReportDlg   = m_IniFile.ReadRect(TXT("Prefs"), TXT("ReportDlg"), m_rcReportDlg);
	m_strRptDlgFont = m_IniFile.ReadString(TXT("Prefs"), TXT("ReportDlgFont"), m_strRptDlgFont);
	m_rcEditDlg     = m_IniFile.ReadRect(TXT("Prefs"), TXT("EditDlg"),   m_rcEditDlg);

	// Load report settings.
	m_eDefGrouping     = static_cast<Grouping>(m_IniFile.ReadInt(TXT("Prefs"), TXT("Grouping"), m_eDefGrouping));
	m_eDefPeriod       = static_cast<Period>(m_IniFile.ReadInt(TXT("Prefs"), TXT("Period"), m_eDefPeriod));
	m_bDefShowSessions = m_IniFile.ReadBool(TXT("Prefs"), TXT("ReportSessions"), m_bDefShowSessions);
	m_bDefShowIfEmpty  = m_IniFile.ReadBool(TXT("Prefs"), TXT("ReportIfEmpty"), m_bDefShowIfEmpty);
	m_bDefShowTotal    = m_IniFile.ReadBool(TXT("Prefs"), TXT("ReportTotal"), m_bDefShowTotal);
	m_strReportFile    = m_IniFile.ReadString(TXT("Prefs"), TXT("ReportFile"), m_strReportFile);

	// Load export/import settings.
	m_strExportFile = m_IniFile.ReadString(TXT("Prefs"), TXT("ExportFile"), m_strExportFile);
	m_strImportFile = m_IniFile.ReadString(TXT("Prefs"), TXT("ImportFile"), m_strImportFile);
}

/******************************************************************************
** Method:		SaveDefaults()
**
** Description:	Save the default settings.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CTaskTracker::SaveDefaults()
{
	// Delete old settings, if old format file.
	if (m_IniFile.ReadString(TXT("Version"), TXT("Version"), INI_FILE_VER_35) == INI_FILE_VER_35)
		m_IniFile.DeleteSection(TXT("Prefs"));

	// Write the file version.
	m_IniFile.WriteString(TXT("Version"), TXT("Version"), INI_FILE_VER_40);

	// Save UI settings.
	m_IniFile.WriteInt(TXT("Prefs"), TXT("LenFormat"), m_eLenFormat);
	m_IniFile.WriteInt(TXT("Prefs"), TXT("WeekOrder"), m_eWeekOrder);
	m_IniFile.WriteBool(TXT("Prefs"), TXT("MinToTray"), m_bMinToTray);
	m_IniFile.WriteBool(TXT("Prefs"), TXT("CheckOverlap"), m_bCheckOverlap);
	m_IniFile.WriteRect(TXT("Prefs"), TXT("ReportDlg"), m_rcReportDlg);
	m_IniFile.WriteString(TXT("Prefs"), TXT("ReportDlgFont"), m_strRptDlgFont);
	m_IniFile.WriteRect(TXT("Prefs"), TXT("EditDlg"), m_rcEditDlg);

	// Save report settings.
	m_IniFile.WriteInt(TXT("Prefs"), TXT("Grouping"), m_eDefGrouping);
	m_IniFile.WriteInt(TXT("Prefs"), TXT("Period"), m_eDefPeriod);
	m_IniFile.WriteBool(TXT("Prefs"), TXT("ReportSessions"), m_bDefShowSessions);
	m_IniFile.WriteBool(TXT("Prefs"), TXT("ReportIfEmpty"), m_bDefShowIfEmpty);
	m_IniFile.WriteBool(TXT("Prefs"), TXT("ReportTotal"), m_bDefShowTotal);
	m_IniFile.WriteString(TXT("Prefs"), TXT("ReportFile"), m_strReportFile);

	// Save export/import settings.
	m_IniFile.WriteString(TXT("Prefs"), TXT("ExportFile"), m_strExportFile);
	m_IniFile.WriteString(TXT("Prefs"), TXT("ImportFile"), m_strImportFile);
}

/******************************************************************************
** Method:		PeriodToDates()
**
** Description:	Gets the dates that correspond to the given period.
**
** Parameters:	ePeriod		The period.
**				rFromDate	The returned from date.
**				rToDate		The returned to date.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CTaskTracker::PeriodToDates(Period ePeriod, CDate& rFromDate, CDate& rToDate) const
{
	CDate	Today;

	// Get todays date.
	Today.Set();

	// Decode period.
	switch(ePeriod)
	{
		case All:
			{
				// Default to todays date.
				rFromDate = Today;
				rToDate   = Today;

				// Any sessions?
				if (!m_oSessionList.empty())
				{
					// Use first and last sessions dates.
					rFromDate = m_oSessionList.front()->Start().Date();
					rToDate   = m_oSessionList.back()->Start().Date();
				}
			}
			break;
		
		case ThisWeek:
			{
				CDate	Date = Today;

				// Adjust for week.
				Date      = GetWeekStart(Date);
				rFromDate = Date;
				Date     += 6;
				rToDate   = Date;
			}
			break;
		
		case ThisMonth:
			{
				int		iDate, iMonth, iYear;
				int		iNumDays;
				CDate	Date = Today;
				
				// Get month limits.
				Date.Get(iDate, iMonth, iYear);
				iNumDays = Date.DaysInMonth();
			
				// Create month limits.
				Date     -= iDate - 1;
				rFromDate = Date;
				Date     += iNumDays - 1;
				rToDate   = Date;
			}
			break;
		
		case LastWeek:
			{
				CDate Date = Today;

				// Go back 7 days.
				Date -= 7;

				// Adjust for week.
				Date      = GetWeekStart(Date);
				rFromDate = Date;
				Date     += 6;
				rToDate   = Date;
			}
			break;
		
		case LastMonth:
			{
				int		iDate, iMonth, iYear;
				CDate	Date = Today;

				// Move back to end of previous month.
				Date.Get(iDate, iMonth, iYear);
				Date -= iDate;

				// Create month limits.
				rToDate   = Date;
				Date     -= rToDate.DaysInMonth() - 1;
				rFromDate = Date;
			}
			break;
		
		case Custom:
			{
				// Use todays date.
				rFromDate = Today;
				rToDate   = Today;
			}
			break;
		
		default:
			ASSERT_FALSE();
			break;
	}
}

/******************************************************************************
** Method:		GetWeekStart()
**
** Description:	Calculates the date of the first day of the week for the date
**				provided.
**
** Parameters:	oDate	A date within the week.
**
** Returns:		The date for the start of the week.
**
*******************************************************************************
*/

CDate CTaskTracker::GetWeekStart(const CDate& oDate) const
{
	CDate oStart = oDate;
	int   nDay   = oStart.DayOfWeek();

	// Find first day of the week.
	switch (m_eWeekOrder)
	{
		case SatToFri:	oStart -= (nDay + 2) % 7;	break;
		case SunToSat:	oStart -= (nDay + 1) % 7;	break;
		case MonToSun:	oStart -= (nDay + 0) % 7;	break;
		default:		ASSERT_FALSE();				break;
	}

	return oStart;
}

/******************************************************************************
** Method:		MinsToStr()
**
** Description:	Format a number of minutes as a string according to the user
**				setting.
**
** Parameters:	lMins	The number of minutes.
**
** Returns:		The formatted string.
**
*******************************************************************************
*/

CString CTaskTracker::MinsToStr(ulong lMins)
{
	CString str;

	switch (m_eLenFormat)
	{
		case HoursMins:
			str.Format(TXT("%d h %02d m"), (lMins / 60), (lMins % 60));
			break;

		case HoursOnly:
			str.Format(TXT("%.2f h"), (double)lMins / 60.0);
			break;

		default:
			ASSERT_FALSE();
			break;
	}

	return str;
}

/******************************************************************************
** Method:		DeleteAllData()
**
** Description:	Deletes all sessions, tasks and locations.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CTaskTracker::DeleteAllData()
{
	// Reset collections.
	m_oSessionList.clear();
	m_oTaskList.RemoveAll();
	m_oLocnList.RemoveAll();

	// Reset last used items.
	m_strLastTask = TXT("");
	m_strLastLocn = TXT("");

	// Update state.
	m_bModified = true;
}

/******************************************************************************
** Method:		DayMonthYearFormat()
**
** Description:	Generate the basic format string for the day, month and year.
**
** Parameters:	None.
**
** Returns:		The format string.
**
*******************************************************************************
*/

static const tchar* DayMonthYearFormat()
{
	static CString s_strFormat;

	// Format on first request.
	if (s_strFormat.Empty())
	{
		CDate::DateOrder eOrder = CDate::DateFormatOrder();
		CString          strSep = CDate::FieldSeparator();

		ASSERT((eOrder >= 0) && (eOrder <=2));

		// CDate::DateOrder formats:           MONTH_DAY_YEAR    DAY_MONTH_YEAR    YEAR_MONTH_DAY
		static const tchar* s_pszFormats[3] = { TXT("MM'%s'dd'%s'yyyy"), TXT("dd'%s'MM'%s'yyyy"), TXT("yyyy'%s'MM'%s'dd") };

		s_strFormat.Format(s_pszFormats[eOrder], strSep.c_str(), strSep.c_str());
	}

	return s_strFormat;
}

/******************************************************************************
** Method:		DatePickerFormat()
**
** Description:	Gets the format to use for date only date/time picker.
**
** Parameters:	None.
**
** Returns:		The format string.
**
*******************************************************************************
*/

const tchar* CTaskTracker::DatePickerFormat()
{
	static CString s_strFormat;

	// Format on first request.
	if (s_strFormat.Empty())
		s_strFormat.Format(TXT("ddd' '%s"), DayMonthYearFormat());

	return s_strFormat;
}

/******************************************************************************
** Method:		TimePickerFormat()
**
** Description:	Gets the format to use for time only date/time picker.
**
** Parameters:	None.
**
** Returns:		The format string.
**
*******************************************************************************
*/

const tchar* CTaskTracker::TimePickerFormat()
{
	static CString s_strFormat;

	// Format on first request.
	if (s_strFormat.Empty())
	{
		CString strSep = CTime::FieldSeparator();

		s_strFormat.Format(TXT("HH'%s'mm"), strSep.c_str());
	}

	return s_strFormat;
}

/******************************************************************************
** Method:		DateTimePickerFormat()
**
** Description:	Gets the format to use for a date/time picker.
**
** Parameters:	None.
**
** Returns:		The format string.
**
*******************************************************************************
*/

const tchar* CTaskTracker::DateTimePickerFormat()
{
	static CString s_strFormat;

	// Format on first request.
	if (s_strFormat.Empty())
		s_strFormat.Format(TXT("ddd' '%s' '%s"), DayMonthYearFormat(), TimePickerFormat());

	return s_strFormat;
}
