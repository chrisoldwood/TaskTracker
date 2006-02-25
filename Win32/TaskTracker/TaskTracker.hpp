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

typedef enum tagLenFormat
{
	HoursMins,
	HoursOnly,
} LenFormat;

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
	// Session state.
	//
	void ClockIn (const CDateTime& dtIn,  const CString& strTask, const CString& strLocn);
	void ClockOut(const CDateTime& dtOut, const CString& strTask, const CString& strLocn);

	void DeleteAllData();

	//
	// Session reporting.
	//
	ulong TotalForPeriod(const CDateTime& dtStart, const CDateTime& dtEnd) const;
	ulong TotalForDay(const CDate& rDate) const;
	ulong TotalForWeek(const CDate& rDate) const;
	ulong TotalForMonth(const CDate& rDate) const;
	ulong TotalOverall() const;

	void ReportData(CReport& rDevice, Grouping eGrouping, const CDate& rFromDate, const CDate& rToDate) const;

	//
	// Serialisation.
	//
	bool LoadData();
	bool SaveData();

	//
	// Misc methods.
	//
	void PeriodToDates(Period ePeriod, CDate& rFromDate, CDate& rToDate) const;
	
	CString MinsToStr(ulong lMins);

	static const char* DatePickerFormat();
	static const char* TimePickerFormat();
	static const char* DateTimePickerFormat();

	//
	// UI members
	//
	CAppWnd			m_AppWnd;
	CAppCmds		m_AppCmds;
	CPrinter		m_oPrinter;

	//
	// Data members
	//
	CSessionList	m_oSessionList;
	CTaskList		m_oTaskList;
	CLocnList		m_oLocnList;
	int16			m_bClockedIn;
	CSessionPtr		m_pCurrSession;
	CString			m_strLastTask;
	CString			m_strLastLocn;
	bool			m_bModified;

	//
	// Preferences members.
	//
	CIniFile		m_IniFile;
	Grouping		m_eDefGrouping;
	Period			m_eDefPeriod;
	LenFormat		m_eLenFormat;
	bool			m_bMinToTray;
	CString			m_strReportFile;
	CString			m_strExportFile;
	CString			m_strImportFile;
	CRect			m_rcReportDlg;
	CRect			m_rcEditDlg;
	bool			m_bCheckOverlap;

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

/******************************************************************************
**
** Global variables.
**
*******************************************************************************
*/

//The application object.
extern CTaskTracker App;

#endif //TASKTRACKER_HPP
