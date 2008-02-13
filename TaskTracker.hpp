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

#if _MSC_VER > 1000
#pragma once
#endif

#include <WCL/App.hpp>
#include "AppWnd.hpp"
#include "AppCmds.hpp"
#include "AppTypes.hpp"
#include <WCL/Printer.hpp>
#include <WCL/IniFile.hpp>
#include "SessionList.hpp"
#include "TaskList.hpp"
#include "LocnList.hpp"

// Forward declarations.
class CReportOptions;
class CFile;

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

	void ReportData(CReportOptions& oOptions, const CDate& oFromDate, const CDate& oToDate) const;

	//
	// Serialisation.
	//
	bool LoadData();
	bool SaveData();

	//
	// Misc methods.
	//
	void  PeriodToDates(Period ePeriod, CDate& rFromDate, CDate& rToDate) const;
	CDate GetWeekStart(const CDate& oDate) const;
	
	CString MinsToStr(ulong lMins);

	static const tchar* DatePickerFormat();
	static const tchar* TimePickerFormat();
	static const tchar* DateTimePickerFormat();

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
	bool			m_bDefShowSessions;		//!< Report each session?
	bool			m_bDefShowIfEmpty;		//!< Report aggregations that are empty?
	bool			m_bDefShowTotal;		//!< Report the overall total?
	LenFormat		m_eLenFormat;
	WeekOrder		m_eWeekOrder;
	bool			m_bMinToTray;
	CString			m_strReportFile;
	CString			m_strExportFile;
	CString			m_strImportFile;
	CRect			m_rcReportDlg;
	CString			m_strRptDlgFont;
	CRect			m_rcEditDlg;
	bool			m_bCheckOverlap;

	//
	// Constants.
	//
	static const tchar* VERSION;
	static const tchar* HELPFILE;

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

	bool ReportUngrouped(CReportOptions& oOptions, const CDateTime& dtFrom, const CDateTime& dtTo, ulong& rlSessions, ulong& rlTotalMins) const;
	bool ReportByWeek(CReportOptions& oOptions, const CDateTime& dtFrom, const CDateTime& dtTo, ulong& rlSessions, ulong& rlTotalMins) const;
	bool ReportByMonth(CReportOptions& oOptions, const CDateTime& dtFrom, const CDateTime& dtTo, ulong& rlSessions, ulong& rlTotalMins) const;
	bool ReportByTask(CReportOptions& oOptions, const CDateTime& dtFrom, const CDateTime& dtTo, ulong& rlSessions, ulong& rlTotalMins) const;
	bool ReportDay(CReportOptions& oOptions, const CDate& rDate, ulong& rlSessions, ulong& rlTotalMins) const;
	bool ReportSession(CReportOptions& oOptions, const CSessionPtr& pSession) const;

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
