/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		TASKTRACKER.HPP
** COMPONENT:	The Application.
** DESCRIPTION:	The CTaskTracker class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef TASKTRACKER_HPP
#define TASKTRACKER_HPP

/******************************************************************************
** 
** Orderings used when reporting.
**
*******************************************************************************
*/

typedef enum tagGrouping
{
	Ungrouped,
	ByWeek,
	ByMonth,
	ByTask
} Grouping;

/******************************************************************************
** 
** Periods used when reporting.
**
*******************************************************************************
*/

typedef enum tagPeriod
{
	All,
	ThisWeek,
	ThisMonth,
	LastWeek,
	LastMonth,
	Custom
} Period;

/******************************************************************************
** 
** Formats used for converting 'n' minutes to a string.
**
*******************************************************************************
*/

typedef enum tagMinsFormat
{
	HoursMins,
	HoursOnly,
} MinsFormat;

/******************************************************************************
** 
** File errors.
**
*******************************************************************************
*/

typedef enum tagFileErr
{
	OpenErr,
	CreateErr,
	ReadOnlyErr,
	ReadErr,
	WriteErr,
	FormatErr,
	VersionErr
} FileErr;

/******************************************************************************
** 
** The application class.
**
*******************************************************************************
*/

class CTaskTracker : public CApp
{
public:
	//
	// Constructors/Destructor.
	//
	CTaskTracker();
	~CTaskTracker();

	//
	// Member access
	//
	CSessionList& SessionList();
	CTaskList& TaskList();
	CLocnList& LocnList();

	bool ClockedIn() const;

	const CSessionPtr& CurrentSession() const;

	const CString& LastTask() const;
	void SetLastTask(const CString& strTask);

	const CString& LastLocn() const;
	void SetLastLocn(const CString& strLocn);

	bool IsModified() const;
	void Modified();

	//
	// Preferences access.
	//
	CPrinter& Printer();
	const char* DefaultFile() const;
	Grouping DefaultGrouping() const;
	Period DefaultPeriod() const;
	bool MinimiseToTray() const;

	//
	// Preferences setting.
	//
	void DefaultFile(const char* pszPath);
	void DefaultGrouping(Grouping eGrouping);
	void DefaultPeriod(Period ePeriod);
	bool MinimiseToTray(bool bMinToTray);

	//
	// Session status.
	//
	void ClockIn (const CDateTime& dtIn,  const CString& strTask, const CString& strLocn);
	void ClockOut(const CDateTime& dtOut, const CString& strTask, const CString& strLocn);
	ulong TotalForPeriod(const CDateTime& dtStart, const CDateTime& dtEnd) const;
	ulong TotalForDay(const CDate& rDate) const;
	ulong TotalForWeek(const CDate& rDate) const;
	ulong TotalForMonth(const CDate& rDate) const;
	ulong TotalOverall() const;

	void DeleteAllData();

	//
	// Serialisation.
	//
	bool LoadData();
	bool SaveData();
	void ReportData(CReport& rDevice, Grouping eGrouping, const CDate& rFromDate, const CDate& rToDate) const;

	//
	// Misc methods.
	//
	void PeriodToDates(Period ePeriod, CDate& rFromDate, CDate& rToDate) const;
	
	CString MinsToStr(ulong lMins);

	static const char* DatePickerFormat();
	static const char* TimePickerFormat();
	static const char* DateTimePickerFormat();

	//
	// Members
	//
	CAppWnd		m_AppWnd;
	CAppCmds	m_AppCmds;

	//
	// Constants.
	//
	static const char* VERSION;

protected:
	//
	// Startup and Shutdown template methods.
	//
	virtual	bool OnOpen();
	virtual	bool OnClose();

	//
	// Members
	//
	CSessionList	m_SessionList;
	CTaskList		m_TaskList;
	CLocnList		m_LocnList;
	int16			m_bClockedIn;
	CSessionPtr		m_pCurrSession;
	CString			m_strLastTask;
	CString			m_strLastLocn;
	bool			m_bModified;
	CPrinter		m_Printer;

	//
	// Preferences.
	//
	CIniFile		m_IniFile;
	CString			m_strDefFile;
	Grouping		m_eDefGrouping;
	Period			m_eDefPeriod;
	MinsFormat		m_eMinsFormat;
	bool			m_bMinToTray;

	//
	// Internal methods.
	//
	void ReadData(CFile& rFile);
	void WriteData(CFile& rFile);

	bool ReportUngrouped(CReport& rDevice, ulong& rlTotal, const CDateTime& dtFrom, const CDateTime& dtTo) const;
	bool ReportByWeek(CReport& rDevice, ulong& rlTotal, const CDateTime& dtFrom, const CDateTime& dtTo) const;
	bool ReportByMonth(CReport& rDevice, ulong& rlTotal, const CDateTime& dtFrom, const CDateTime& dtTo) const;
	bool ReportByTask(CReport& rDevice, ulong& rlTotal, const CDateTime& dtFrom, const CDateTime& dtTo) const;
	bool ReportDay(CReport& rDevice, const CDate& rDate, ulong& rlTotal) const;
	bool ReportSession(CReport& rDevice, const CSessionPtr& pSession) const;

	void LoadDefaults();
	void SaveDefaults();
};

// The title for the .dat and .ini files.
#define FILE_TITLE			"TaskTrak"

// The current file version.
#define DAT_FILE_VERSION	35

// The data filename.
#define	DAT_FILE_NAME		FILE_TITLE ".dat"

// The cfg filename.
#define INI_FILE_NAME		FILE_TITLE ".ini"

// The default report filename.
#define	RPT_FILE_NAME		FILE_TITLE ".txt"

/******************************************************************************
**
** Global variables.
**
*******************************************************************************
*/

//The application object.
extern CTaskTracker App;

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

inline bool CTaskTracker::ClockedIn() const
{
	return m_bClockedIn;
}

inline CTaskList& CTaskTracker::TaskList()
{
	return m_TaskList;
}

inline const CString& CTaskTracker::LastTask() const
{
	return m_strLastTask;
}

inline void CTaskTracker::SetLastTask(const CString& strTask)
{
	m_strLastTask = strTask;
}

inline CSessionList& CTaskTracker::SessionList()
{
	return m_SessionList;
}

inline const CSessionPtr& CTaskTracker::CurrentSession() const
{
	return m_pCurrSession;
}

inline CLocnList& CTaskTracker::LocnList()
{
	return m_LocnList;
}

inline const CString& CTaskTracker::LastLocn() const
{
	return m_strLastLocn;
}

inline void CTaskTracker::SetLastLocn(const CString& strLocn)
{
	m_strLastLocn = strLocn;
}

inline bool CTaskTracker::IsModified() const
{
	return m_bModified;
}

inline void CTaskTracker::Modified()
{
	m_bModified = true;
}

inline CPrinter& CTaskTracker::Printer()
{
	return m_Printer;
}

inline const char* CTaskTracker::DefaultFile() const
{
	return m_strDefFile;
}

inline Grouping CTaskTracker::DefaultGrouping() const
{
	return m_eDefGrouping;
}

inline Period CTaskTracker::DefaultPeriod() const
{
	return m_eDefPeriod;
}

inline bool CTaskTracker::MinimiseToTray() const
{
	return m_bMinToTray;
}

inline void CTaskTracker::DefaultFile(const char* pszPath)
{
	m_strDefFile = pszPath;
}

inline void CTaskTracker::DefaultGrouping(Grouping eGrouping)
{
	m_eDefGrouping = eGrouping;
}

inline void CTaskTracker::DefaultPeriod(Period ePeriod)
{
	m_eDefPeriod = ePeriod;
}

inline bool CTaskTracker::MinimiseToTray(bool bMinToTray)
{
	m_bMinToTray = bMinToTray;
}

#endif //TASKTRACKER_HPP
