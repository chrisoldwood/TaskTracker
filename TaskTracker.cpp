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
const char* CTaskTracker::VERSION      = "v3.6 [Debug]";
#else
const char* CTaskTracker::VERSION      = "v3.6";
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
	, m_pCurrSession(NULL)
	, m_bModified(false)
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
	m_strTitle = "TaskTracker";

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

	// Free up current session.
	if (m_pCurrSession)
		delete m_pCurrSession;
	
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
	ASSERT(m_pCurrSession);

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
	m_pCurrSession = NULL;

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
	CSessionEnum	Enum(m_SessionList, dtStart, dtEnd);
	CSession*		pSession;
	ulong			lTotal = 0;
	
	// For all sessions within the period.
	while((pSession = Enum.Next()) != NULL)
		lTotal += pSession->Length();
	
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
			throw CFileException(CFileException::E_FORMAT_INVALID, AppFile);
		
		// Check version number.
		if (iVersion != DAT_FILE_VERSION)
			throw CFileException(CFileException::E_VERSION_INVALID, AppFile);
		
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
		ASSERT(m_pCurrSession);

		// Read current session.
		*m_pCurrSession << rFile;
	}

	// Read last used items.
	rFile >> m_strLastTask;
	rFile >> m_strLastLocn;
	
	// Read collections.
	m_SessionList << rFile;
	m_TaskList    << rFile;
	m_LocnList    << rFile;
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
		*m_pCurrSession >> rFile;

	// Write last used items.
	rFile << m_strLastTask;
	rFile << m_strLastLocn;
	
	// Write collections.
	m_SessionList >> rFile;
	m_TaskList    >> rFile;
	m_LocnList    >> rFile;
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
    char 		szTotal[100];
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
		sprintf(szTotal, "Total For All Sessions: %s", App.MinsToStr(lTotal));
		rDevice.SendText(szTotal);
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
    CDate			DateDone;
	CSession*		pSession = NULL;
	CSessionEnum	Enum(App.SessionList(), dtFrom, dtTo);
	ulong			lDayTotal = 0;
	
	// Init total.
	rlTotal = 0;

	// For all sessions.
	while((pSession = Enum.Next()) != NULL)
	{
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
	CDateTime		dtStart;
	CDateTime		dtEnd;
	CSession*		pSession = NULL;
	CSessionEnum	Enum(App.SessionList(), dtFrom, dtTo);

	// Init total.
	rlTotal = 0;

	// For all sessions.
	while((pSession = Enum.Next()) != NULL)
	{
		// Skip sessions still in current week.
		if ( (pSession->Start() >= dtStart) && (pSession->Start() < dtEnd) )
			continue;

		CDate	Date = pSession->Start().Date();
		int		iDay = Date.DayOfWeek();
		
		// Create week limits.
		Date -= iDay;
		dtStart.Date(Date);
		Date += 6;
		dtEnd.Date(Date);
		dtEnd.Time(CTime(23, 59, 59));

		// Create enumerator for the week.
		CSessionEnum	LenEnum(App.SessionList(), dtStart, dtEnd);
		CSession*		pLenSession = NULL;
		ulong			lWeekTotal = 0;
		
		// Get total for the week.
		while((pLenSession = LenEnum.Next()) != NULL)
			lWeekTotal += pLenSession->Length();

	    char szHeading[100];
		CString	strStartDate = dtStart.Date().ToString();
		CString strEndDate   = dtEnd.Date().ToString();
		CString strLen       = App.MinsToStr(lWeekTotal);

		// Output week heading.
		sprintf(szHeading, "Week: %s to %s (Total: %s)", strStartDate, strEndDate, strLen);
		if (!rDevice.SendHeading(szHeading))
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
	CDateTime		dtStart;
	CDateTime		dtEnd;
	CSession*		pSession = NULL;
	CSessionEnum	Enum(App.SessionList(), dtFrom, dtTo);

	// Init total.
	rlTotal = 0;

	// For all sessions.
	while((pSession = Enum.Next()) != NULL)
	{
		// Skip sessions still in current month.
		if ( (pSession->Start() >= dtStart) && (pSession->Start() < dtEnd) )
			continue;

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
		CSessionEnum	LenEnum(App.SessionList(), dtStart, dtEnd);
		CSession*		pLenSession = NULL;
		ulong			lMonthTotal = 0;
		
		// Get total for the month.
		while((pLenSession = LenEnum.Next()) != NULL)
			lMonthTotal += pLenSession->Length();

	    char szHeading[100];
		CString	strStartDate = dtStart.Date().ToString();
		CString strEndDate   = dtEnd.Date().ToString();
		CString strLen       = App.MinsToStr(lMonthTotal);

		// Output month heading.
		sprintf(szHeading, "Month: %s to %s (Total: %s)", strStartDate, strEndDate, strLen);
		if (!rDevice.SendHeading(szHeading))
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
	CTaskListEnum	TaskEnum(App.TaskList());
	CString*		pString;

	// Initialise total.
	rlTotal = 0;
	
	// For all tasks.
	while((pString = TaskEnum.Next()) != NULL)
	{
		CSession*		pSession = NULL;
		CSessionEnum	LenEnum(App.SessionList(), dtFrom, dtTo);
		ulong			lTotal = 0;
	
		// Get length of all sessions for task.
		while((pSession = LenEnum.Next()) != NULL)
		{
			if (pSession->Task() == *pString)
				lTotal += pSession->Length();
		}
		
		// Update grand total.
		rlTotal +=lTotal;

		CSessionEnum	SessEnum(App.SessionList(), dtFrom, dtTo);
		char			szHeading[256];
		const char*		pszTask = *pString;
		const char*		pszLen  = App.MinsToStr(lTotal);
		
		// Output month heading.
		sprintf(szHeading, "%s (Total: %s)", pszTask, pszLen);
		if (!rDevice.SendHeading(szHeading))
			return false;
		
		// Output all sessions for task.
		while((pSession = SessEnum.Next()) != NULL)
		{
			if (pSession->Task() == *pString)
			{
				char 		szText[100];
				CString strDate  = pSession->Start().Date().ToString();
				CString	strStart = pSession->Start().Time().ToString();
				CString	strEnd   = pSession->Finish().Time().ToString();
				CString strLen   = App.MinsToStr(pSession->Length());

				sprintf(szText,"%s from %s to %s for %s", strDate,
								strStart, strEnd, strLen);
			
				if (!rDevice.SendText(szText))
					return false;
			}
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
	CDateTime	dtStart;
	CDateTime	dtEnd;
	CSession*	pSession;

	// Create limits of day.
	dtStart.Date(rDate);
	dtEnd.Date(rDate);
	dtEnd.Time(CTime(23, 59, 59));

	// Create enumerator for the day.
	CSessionEnum	LenEnum(App.SessionList(), dtStart, dtEnd);
	rlTotal = 0;

	// Get length of all sessions on the day.
	while((pSession = LenEnum.Next()) != NULL)
		rlTotal += pSession->Length();
	
	// Create enumerator for the day.
	CSessionEnum	Enum(App.SessionList(), dtStart, dtEnd);
	
	char szText[100];
	CString	strDate = rDate.ToString(CDate::SD_DD_MM_YY);
	CString strLen  = App.MinsToStr(rlTotal);
	
	sprintf(szText, "%s (Total: %s)", strDate, strLen);
	if (!rDevice.SendText(szText))
		return false;

	// Report all sessions on the day.
	while((pSession = Enum.Next()) != NULL)
	{
		if (!ReportSession(rDevice, pSession))
			return false;
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

bool CTaskTracker::ReportSession(CReport& rDevice, CSession* pSession) const
{
	CString	strText;
	CString	strStart = pSession->Start().Time().ToString(CTime::HH_MM);
	CString	strEnd   = pSession->Finish().Time().ToString(CTime::HH_MM);
	CString strTask  = pSession->Task();
	CString strLocn  = pSession->Location();
	ulong	lLen     = pSession->Length();

	// Format core data.
	strText.Format("%s to %s for %s", strStart, strEnd,	App.MinsToStr(lLen));

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
	m_eDefGrouping = (Grouping)m_IniFile.ReadInt("Prefs", "Grouping", Ungrouped);
	m_eDefPeriod   = (Period)m_IniFile.ReadInt("Prefs", "Period", All);
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
				// Any sessions?
				if (m_SessionList.Length())
				{
					CSessionEnum	Enum(m_SessionList);
					CSession*		pSession = Enum.Next();
				
					// First session is earliest.
					rFromDate = pSession->Start().Date();
				
					// Find last session.
					while(pSession)
					{
						// Last session is latest date.
						rToDate  = pSession->Start().Date();
						pSession = Enum.Next();
					}
				}
				else
				{
					// Use todays date.
					rFromDate = Today;
					rToDate   = Today;
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
