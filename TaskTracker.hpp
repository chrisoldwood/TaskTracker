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
	bool ClockedIn() const;
	CTaskList& TaskList();
	const CString& LastTask() const;
	CSessionList& SessionList();
	const CSession* CurrentSession() const;
	bool IsModified() const;
	void Modified();

	CPrinter& Printer();
	const char* DefaultFile() const;
	Grouping DefaultGrouping() const;
	Period DefaultPeriod() const;

	//
	// Member setting.
	//
	void DefaultFile(const char* pszPath);
	void DefaultGrouping(Grouping eGrouping);
	void DefaultPeriod(Period ePeriod);

	//
	// Session status.
	//
	void ClockIn(const CDateTime& dtIn, const CString& strTask);
	void ClockOut(const CDateTime& dtOut, const CString& strTask);
	ulong TotalForPeriod(const CDateTime& dtStart, const CDateTime& dtEnd) const;
	ulong TotalForDay(const CDate& rDate) const;
	ulong TotalForWeek(const CDate& rDate) const;
	ulong TotalForMonth(const CDate& rDate) const;
	ulong TotalOverall() const;

	//
	// Serialisation.
	//
	bool LoadData();
	bool SaveData();
	void ReportData(CReport& rDevice, Grouping eGrouping, const CDate& rFromDate, const CDate& rToDate) const;
	void ReportFileError(FileErr eErr, const CPath& rPath) const;

	//
	// Misc methods.
	//
	void PeriodToDates(Period ePeriod, CDate& rFromDate, CDate& rToDate) const;
	
	const char* MinsToStr(ulong lMins);

	//
	// Members
	//
	CAppWnd		m_AppWnd;
	CAppCmds	m_AppCmds;

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
	int16			m_bClockedIn;
	CSession*		m_pCurrSession;
	CString			m_strLastTask;
	char			m_szMins[20];
	bool			m_bModified;
	CPrinter		m_Printer;

	//
	// Preferences.
	//
	CIniFile		m_IniFile;
	CString			m_strDefFile;
	Grouping		m_eDefGrouping;
	Period			m_eDefPeriod;

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
	bool ReportSession(CReport& rDevice, CSession* pSession) const;

	void LoadDefaults();
	void SaveDefaults();
};

// The title for files.
#define FILE_TITLE			"TaskTrak"

// The current file version.
#define DAT_FILE_VERSION	30

// The data filename.
#define	DAT_FILE_NAME		FILE_TITLE ".dat"

// The cfg filename.
#define INI_FILE_NAME		FILE_TITLE ".ini"

// The default report filename.
#define	RPT_FILE_NAME		FILE_TITLE ".txt"

// The format used for all date DateTime pickers.
#define	DTP_DATE_FORMAT		"ddd' 'dd'/'MM'/'yy"

// The format used for all time DateTime pickers.
#define	DTP_TIME_FORMAT		"HH':'mm"

// The format used for all date & time DateTime pickers.
#define	DTP_DATETIME_FORMAT	"ddd' 'dd'/'MM'/'yy' 'HH':'mm"

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

inline CSessionList& CTaskTracker::SessionList()
{
	return m_SessionList;
}

inline const CSession* CTaskTracker::CurrentSession() const
{
	return m_pCurrSession;
}

inline const char* CTaskTracker::MinsToStr(ulong lMins)
{
	wsprintf(m_szMins, "%ld h %02ld m", (lMins / 60), (lMins % 60));
	return m_szMins;
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

#endif //TASKTRACKER_HPP
