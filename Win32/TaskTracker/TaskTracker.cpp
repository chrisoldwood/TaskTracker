/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		TASKTRACKER.CPP
** COMPONENT:	The Application.
** DESCRIPTION:	The CTaskTracker class definition.
**
*******************************************************************************
*/

#include "AppHeaders.hpp"
#include <stdio.h>

#ifdef _DEBUG
// For memory leak detection.
#define new DBGCRT_NEW
#endif

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
** Class constants.
**
*******************************************************************************
*/

#ifdef _DEBUG
const char* CTaskTracker::VERSION      = "v3.7 [Debug Alpha]";
#else
const char* CTaskTracker::VERSION      = "v3.7 [Alpha]";
#endif

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
	, m_bClockedIn(false)
	, m_strLastTask("")
	, m_strLastLocn("")
	, m_pCurrSession(nullptr)
	, m_bModified(false)
	, m_eMinsFormat(HoursMins)
	, m_bMinToTray(false)
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
	// Set the app title.
	m_strTitle = "Task Tracker";

	// Create .ini file path.
	m_IniFile.m_strPath  = CPath::ApplicationDir();
	m_IniFile.m_strPath += INI_FILE_NAME;

	// Load settings.
	LoadDefaults();
	
	// Load the toolbar bitmap.
	m_rCmdControl.CmdBitmap().LoadRsc(IDR_APPTOOLBAR);

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
	m_AppCmds.UpdateUI();
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
	m_strLastTask = "";
	m_strLastLocn = "";

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
	m_pCurrSession = new CSession;

	// Initialise.
	m_pCurrSession->Start(dtIn, strTask, strLocn);

	// Add task to list if set.
	if (strTask != "")
		m_TaskList.Add(strTask);

	// Add location to list if set.
	if (strLocn != "")
		m_LocnList.Add(strLocn);

	// Update state.
	m_strLastTask = strTask;
	m_strLastLocn = strLocn;
	m_bClockedIn  = true;
	m_bModified   = true;

	m_AppCmds.UpdateUI();
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
	m_SessionList.Add(m_pCurrSession);
	
	// Reset current session.
	m_pCurrSession.Reset();

	// Add task to list if set.
	if (strTask != "")
		m_TaskList.Add(strTask);
	
	// Add location to list if set.
	if (strLocn != "")
		m_LocnList.Add(strLocn);

	// Update state.
	m_strLastTask = strTask;
	m_strLastLocn = strLocn;
	m_bClockedIn  = false;
	m_bModified   = true;

	m_AppCmds.UpdateUI();
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
	CIter             oIter(m_SessionList.begin());
	ulong			  lTotal = 0;

	// For all sessions within the period.
	while ((oIter = std::find_if(oIter, m_SessionList.end(), oRange)) != m_SessionList.end())
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
		CurrSession.Finish(dtCurrent, "", "");
		
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
	int			iDay = rDate.DayOfWeek();

	// Create week limits.
	Date -= iDay;
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
	AppFilePath  = CPath::ApplicationDir();
	AppFilePath += DAT_FILE_NAME;

	// Data file exists yet?
	if (!AppFilePath.Exists())
		return true;

	try
	{
		// Open file.
		AppFile.Open(AppFilePath, GENERIC_READ);

		char	szMagic[2];
		uint16	iVersion;

		// Read magic and version number.
		AppFile.Read(szMagic, sizeof(szMagic));
		AppFile.Read(&iVersion, sizeof(iVersion));
		
		// Check header.
		if ( (szMagic[0] != 'T') || (szMagic[1] != 'T') )
			throw CFileException(CFileException::E_FORMAT_INVALID, AppFilePath);
		
		// Check version number.
		if (iVersion != DAT_FILE_VERSION)
			throw CFileException(CFileException::E_VERSION_INVALID, AppFilePath);
		
		// Read the actual data.
		ReadData(AppFile);
			
		// Done.
		AppFile.Close();
	}
	catch(CFileException& rException)
	{
		// Notify user.
		m_AppWnd.AlertMsg(rException.ErrorText());
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
		m_pCurrSession = new CSession;

		// Read current session.
		rFile >> *m_pCurrSession;
	}

	// Read last used items.
	rFile >> m_strLastTask;
	rFile >> m_strLastLocn;
	
	// Read collections.
	rFile >> m_SessionList;
	rFile >> m_TaskList;
	rFile >> m_LocnList;
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
	AppFilePath  = CPath::ApplicationDir();
	AppFilePath += DAT_FILE_NAME;

	try
	{
		// Create file.
		AppFile.Create(AppFilePath);

		char	szMagic[2] = { 'T', 'T' };
		uint16	iVersion   = DAT_FILE_VERSION;

		// Write magic and version number.
		AppFile.Write(szMagic, sizeof(szMagic));
		AppFile.Write(&iVersion, sizeof(iVersion));

		// Write the actual data.
		WriteData(AppFile);

		// Done.
		AppFile.Close();
	}
	catch(CFileException& rException)
	{
		// Notify user.
		m_AppWnd.AlertMsg(rException.ErrorText());
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
	rFile << m_SessionList;
	rFile << m_TaskList;
	rFile << m_LocnList;
}

/******************************************************************************
** Method:		ReportData()
**
** Description:	Report data to a device.
**
** Parameters:	rDevice		The device to report to.
**				eGrouping	The grouping of the data.
**				rFromDate	The starting date.
**				rToDate		The end date.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CTaskTracker::ReportData(CReport& rDevice, Grouping eGrouping, const CDate& rFromDate,
						const CDate& rToDate) const
{
	ulong		lTotal = 0;
	bool		bOkay  = false;
	CDateTime	dtFrom, dtTo;

	// Initialise the device.
	if (!rDevice.Init())
		return;

	// Set up limits.
	dtFrom.Date(rFromDate);
	dtTo.Date(rToDate);
	dtTo.Time(CTime(23, 59, 59));

	// Check grouping.
	switch(eGrouping)
	{
		case Ungrouped:
			bOkay = ReportUngrouped(rDevice, lTotal, dtFrom, dtTo);
			break;
			
		case ByWeek:
			bOkay = ReportByWeek(rDevice, lTotal, dtFrom, dtTo);
			break;
			
		case ByMonth:
			bOkay = ReportByMonth(rDevice, lTotal, dtFrom, dtTo);
			break;
			
		case ByTask:
			bOkay = ReportByTask(rDevice, lTotal, dtFrom, dtTo);
			break;
			
		default:
			ASSERT_FALSE();
			break;
	}

	// Reported okay?
	if (bOkay)
	{
		// Output total.
		CString strTotal = CString::Fmt("Total For All Sessions: %s", App.MinsToStr(lTotal));

		rDevice.SendText(strTotal);
	}

	// Cleanup the device.
	rDevice.Term();
}

/******************************************************************************
** Method:		ReportUngrouped()
**
** Description:	Report all data ungrouped.
**
** Parameters:	rDevice		The device to report to.
**				rlTotal		The total time.
**				dtFrom		The starting date.
**				dtTo		The end date.
**
** Returns:		true or false.
**
*******************************************************************************
*/

bool CTaskTracker::ReportUngrouped(CReport& rDevice, ulong& rlTotal, const CDateTime& dtFrom,
							const CDateTime& dtTo) const
{
	// Template shorthands.
	typedef CSessionList::const_iterator CIter;

    CDate DateDone;
	ulong lDayTotal = 0;
	
	// Init total.
	rlTotal = 0;

	CIsSessionInRange oRange(dtFrom, dtTo);
	CIter             oIter(m_SessionList.begin());

	// For all sessions in range...
	while ((oIter = std::find_if(oIter, m_SessionList.end(), oRange)) != m_SessionList.end())
	{
		CSessionPtr pSession = *oIter;

		// New day?
		if (pSession->Start().Date() > DateDone)
		{
			// Report days' data.
			if (!ReportDay(rDevice, pSession->Start().Date(), lDayTotal))
				return false;
				
			if (!rDevice.SendLineBreak())
				return false;
			
			DateDone = pSession->Start().Date();
			rlTotal += lDayTotal;
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
** Parameters:	rDevice		The device to report to.
**				rlTotal		The total time.
**				dtFrom		The starting date.
**				dtTo		The end date.
**
** Returns:		true or false.
**
*******************************************************************************
*/

bool CTaskTracker::ReportByWeek(CReport& rDevice, ulong& rlTotal, const CDateTime& dtFrom,
						const CDateTime& dtTo) const
{
	// Template shorthands.
	typedef CSessionList::const_iterator CIter;

	CDateTime dtStart;
	CDateTime dtEnd;

	// Init total.
	rlTotal = 0;

	CIsSessionInRange oRange(dtFrom, dtTo);
	CIter             oIter(m_SessionList.begin());

	// For all sessions in range...
	while ((oIter = std::find_if(oIter, m_SessionList.end(), oRange)) != m_SessionList.end())
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
		Date -= iDay;
		dtStart.Date(Date);
		Date += 6;
		dtEnd.Date(Date);
		dtEnd.Time(CTime(23, 59, 59));

		// Create enumerator for the week.
		CIsSessionInRange oWeekRange(dtStart, dtEnd);
		CIter             oWeekIter(m_SessionList.begin());
		ulong			  lWeekTotal = 0;

		// Get total for the week.
		while ((oWeekIter = std::find_if(oWeekIter, m_SessionList.end(), oWeekRange)) != m_SessionList.end())
		{
			lWeekTotal += (*oWeekIter)->Length();
			++oWeekIter;
		}

		// Output week heading.
		CString	strStartDate = dtStart.Date().ToString(CDate::FMT_WIN_SHORT);
		CString strEndDate   = dtEnd.Date().ToString(CDate::FMT_WIN_SHORT);
		CString strLen       = App.MinsToStr(lWeekTotal);
		CString strHeading   = CString::Fmt("Week: %s to %s (Total: %s)", strStartDate, strEndDate, strLen);

		if (!rDevice.SendHeading(strHeading))
			return false;

		// Get date for week start.
		Date = dtStart.Date();
		
		// For all days in the week.
		for(iDay = 0; iDay < 7; iDay++, Date += 1)
		{
			ulong lDayTotal;

			// Report data for the day..
			if (!ReportDay(rDevice, Date, lDayTotal))
				return false;

			// End of day.
			if (!rDevice.SendLineBreak())
				return false;
		}
		
		// End of week.
		if (!rDevice.SendLineBreak())
			return false;
		
		// Update total.
		rlTotal += lWeekTotal;

		++oIter;
	}

	return true;
}

/******************************************************************************
** Method:		ReportByMonth()
**
** Description:	Report all data ordered by months.
**
** Parameters:	rDevice		The device to report to.
**				rlTotal		The total time.
**				dtFrom		The starting date.
**				dtTo		The end date.
**
** Returns:		true or false.
**
*******************************************************************************
*/

bool CTaskTracker::ReportByMonth(CReport& rDevice, ulong& rlTotal, const CDateTime& dtFrom,
							const CDateTime& dtTo) const
{
	// Template shorthands.
	typedef CSessionList::const_iterator CIter;

	CDateTime dtStart;
	CDateTime dtEnd;

	// Init total.
	rlTotal = 0;

	CIsSessionInRange oRange(dtFrom, dtTo);
	CIter             oIter(m_SessionList.begin());

	// For all sessions in range...
	while ((oIter = std::find_if(oIter, m_SessionList.end(), oRange)) != m_SessionList.end())
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
		CIter             oMonthIter(m_SessionList.begin());
		ulong			  lMonthTotal = 0;
		
		// Get total for the month.
		while ((oMonthIter = std::find_if(oMonthIter, m_SessionList.end(), oMonthRange)) != m_SessionList.end())
		{
			lMonthTotal += (*oMonthIter)->Length();
			++oMonthIter;
		}

		// Output month heading.
		CString	strStartDate = dtStart.Date().ToString(CDate::FMT_WIN_SHORT);
		CString strEndDate   = dtEnd.Date().ToString(CDate::FMT_WIN_SHORT);
		CString strLen       = App.MinsToStr(lMonthTotal);
		CString strHeading   = CString::Fmt("Month: %s to %s (Total: %s)", strStartDate, strEndDate, strLen);

		if (!rDevice.SendHeading(strHeading))
			return false;

		// Get date for month start.
		Date = dtStart.Date();
		
		// For all days in the month.
		for(int iDay = 0; iDay < iNumDays; iDay++, Date += 1)
		{
			ulong lDayTotal;

			// Report data for the day..
			if (!ReportDay(rDevice, Date, lDayTotal))
				return false;

			// End of day.
			if (!rDevice.SendLineBreak())
				return false;
		}
		
		// End of week.
		if (!rDevice.SendLineBreak())
			return false;
		
		// Update total.
		rlTotal += lMonthTotal;

		++oIter;
	}

	return true;
}

/******************************************************************************
** Method:		ReportByTask()
**
** Description:	Report all data by its task.
**
** Parameters:	rDevice		The device to report to.
**				rlTotal		The total time.
**				dtFrom		The starting date.
**				dtTo		The end date.
**
** Returns:		true or false.
**
*******************************************************************************
*/

bool CTaskTracker::ReportByTask(CReport& rDevice, ulong& rlTotal, const CDateTime& dtFrom,
						const CDateTime& dtTo) const
{
	// Template shorthands.
	typedef CTaskList::const_iterator CTaskIter;
	typedef CSessionList::const_iterator CSessIter;

	// Initialise total.
	rlTotal = 0;
	
	// For all tasks.
	for(CTaskIter oIter = App.TaskList().begin(); oIter != App.TaskList().end(); ++oIter)
	{
		CSessIter         oTotalIter(m_SessionList.begin());
		CIsSessionInRange oTotalRange(dtFrom, dtTo);
		ulong			  lTotal = 0;
	
		// Get length of all sessions for task.
		while ((oTotalIter = std::find_if(oTotalIter, m_SessionList.end(), oTotalRange)) != m_SessionList.end())
		{
			CSessionPtr pSession = *oTotalIter;

			if (pSession->Task() == (*oIter))
				lTotal += pSession->Length();

			++oTotalIter;
		}
		
		// Update grand total.
		rlTotal +=lTotal;

		// Output month heading.
		CString strHeading = CString::Fmt("%s (Total: %s)", *oIter, App.MinsToStr(lTotal));

		if (!rDevice.SendHeading(strHeading))
			return false;

		oTotalIter = m_SessionList.begin();

		// Output all sessions for task.
		while ((oTotalIter = std::find_if(oTotalIter, m_SessionList.end(), oTotalRange)) != m_SessionList.end())
		{
			CSessionPtr pSession = *oTotalIter;

			if (pSession->Task() == (*oIter))
			{
				// Output session.
				CString strDate  = pSession->Start().Date().ToString(CDate::FMT_WIN_SHORT);
				CString	strStart = pSession->Start().Time().ToString(CDate::FMT_WIN_SHORT);
				CString	strEnd   = pSession->Finish().Time().ToString(CDate::FMT_WIN_SHORT);
				CString strLen   = App.MinsToStr(pSession->Length());
				CString strText  = CString::Fmt("%s from %s to %s for %s", strDate, strStart, strEnd, strLen);
			
				if (!rDevice.SendText(strText))
					return false;
			}

			++oTotalIter;
		}

		// End of task.
		if (!rDevice.SendLineBreak())
			return false;
	}
	
	return true;
}

/******************************************************************************
** Method:		ReportDay()
**
** Description:	Report all data for the day to the device.
**
** Parameters:	rDevice		The device to report to.
**				rDate		The date of the day to report.
**				rlTotal		The total time for all sessions on the day.
**
** Returns:		true or false.
**
*******************************************************************************
*/

bool CTaskTracker::ReportDay(CReport& rDevice, const CDate& rDate, ulong& rlTotal) const
{
	// Template shorthands.
	typedef CSessionList::const_iterator CIter;

	CDateTime	dtStart;
	CDateTime	dtEnd;

	// Create limits of day.
	dtStart.Date(rDate);
	dtEnd.Date(rDate);
	dtEnd.Time(CTime(23, 59, 59));

	// Initialise total.
	rlTotal = 0;

	CIter             oIter(m_SessionList.begin());
	CIsSessionInRange oRange(dtStart, dtEnd);

	// Get length of all sessions on the day.
	while ((oIter = std::find_if(oIter, m_SessionList.end(), oRange)) != m_SessionList.end())
	{
		rlTotal += (*oIter)->Length();

		++oIter;
	}
	
	CString	strDate = rDate.ToString(CDate::FMT_WIN_SHORT);
	CString strLen  = App.MinsToStr(rlTotal);
	CString strText = CString::Fmt("%s (Total: %s)", strDate, strLen);

	if (!rDevice.SendText(strText))
		return false;

	oIter = m_SessionList.begin();

	// Report all sessions on the day.
	while ((oIter = std::find_if(oIter, m_SessionList.end(), oRange)) != m_SessionList.end())
	{
		if (!ReportSession(rDevice, *oIter))
			return false;

		++oIter;
	}
		
	return true;
}

/******************************************************************************
** Method:		ReportSession()
**
** Description:	Report a session to the device.
**
** Parameters:	rDevice		The device to report to.
**				pSession	The session to report.
**
** Returns:		true or false.
**
*******************************************************************************
*/

bool CTaskTracker::ReportSession(CReport& rDevice, const CSessionPtr& pSession) const
{
	CString	strStart = pSession->Start().Time().ToString(CTime::FMT_WIN_SHORT);
	CString	strEnd   = pSession->Finish().Time().ToString(CTime::FMT_WIN_SHORT);
	CString strTask  = pSession->Task();
	CString strLocn  = pSession->Location();
	ulong	lLen     = pSession->Length();

	// Format core data.
	CString strText = CString::Fmt("%s to %s for %s", strStart, strEnd, App.MinsToStr(lLen));

	// Append task, if supplied.
	if (strTask != "")
		strText += " on " + strTask;

	// Append location, if supplied.
	if (strLocn != "")
		strText += " at " + strLocn;

	return rDevice.SendText(strText);
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
	m_strDefFile   = m_IniFile.ReadString("Prefs", "ReportFile", RPT_FILE_NAME);
	m_eDefGrouping = static_cast<Grouping>(m_IniFile.ReadInt("Prefs", "Grouping", Ungrouped));
	m_eDefPeriod   = static_cast<Period>(m_IniFile.ReadInt("Prefs", "Period", All));
	m_eMinsFormat  = static_cast<MinsFormat>(m_IniFile.ReadInt("Prefs", "MinsFormat", m_eMinsFormat));
	m_bMinToTray   = m_IniFile.ReadBool("Prefs", "MinToTray", m_bMinToTray);
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
	m_IniFile.WriteString("Prefs", "ReportFile", m_strDefFile);
	m_IniFile.WriteInt("Prefs", "Grouping", m_eDefGrouping);
	m_IniFile.WriteInt("Prefs", "Period",   m_eDefPeriod);
	m_IniFile.WriteInt("Prefs", "MinsFormat", m_eMinsFormat);
	m_IniFile.WriteBool("Prefs", "MinToTray", m_bMinToTray);
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
				if (!m_SessionList.empty())
				{
					// Use first and last sessions dates.
					rFromDate = m_SessionList.front()->Start().Date();
					rToDate   = m_SessionList.back()->Start().Date();
				}
			}
			break;
		
		case ThisWeek:
			{
				CDate	Date = Today;
				int 	iDay = Today.DayOfWeek();

				// Adjust for week.
				Date     -= iDay;
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
				// Go back 7 days.
				CDate Date = Today;
				Date -= 7;
				int	iDay = Today.DayOfWeek();

				// Adjust for week.
				Date     -= iDay;
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

	switch (m_eMinsFormat)
	{
		case HoursMins:
			str.Format("%d h %02d m", (lMins / 60), (lMins % 60));
			break;

		case HoursOnly:
			str.Format("%.2f h", (double)lMins / 60.0);
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
	m_SessionList.RemoveAll();
	m_TaskList.RemoveAll();
	m_LocnList.RemoveAll();

	// Reset last used items.
	m_strLastTask = "";
	m_strLastLocn = "";

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

static const char* DayMonthYearFormat()
{
	static CString s_strFormat;

	// Format on first request.
	if (s_strFormat.Empty())
	{
		CDate::DateOrder eOrder = CDate::DateFormatOrder();
		CString          strSep = CDate::FieldSeparator();

		ASSERT((eOrder >= 0) && (eOrder <=2));

		// CDate::DateOrder formats:           MONTH_DAY_YEAR    DAY_MONTH_YEAR    YEAR_MONTH_DAY
		static const char* s_pszFormats[3] = { "MM'%s'dd'%s'yyyy", "dd'%s'MM'%s'yyyy", "yyyy'%s'MM'%s'dd" };

		s_strFormat.Format(s_pszFormats[eOrder], strSep, strSep);
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

const char* CTaskTracker::DatePickerFormat()
{
	static CString s_strFormat;

	// Format on first request.
	if (s_strFormat.Empty())
		s_strFormat.Format("ddd' '%s", DayMonthYearFormat());

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

const char* CTaskTracker::TimePickerFormat()
{
	static CString s_strFormat;

	// Format on first request.
	if (s_strFormat.Empty())
	{
		CString strSep = CTime::FieldSeparator();

		s_strFormat.Format("HH'%s'mm", strSep);
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

const char* CTaskTracker::DateTimePickerFormat()
{
	static CString s_strFormat;

	// Format on first request.
	if (s_strFormat.Empty())
		s_strFormat.Format("ddd' '%s' '%s", DayMonthYearFormat(), TimePickerFormat());

	return s_strFormat;
}
