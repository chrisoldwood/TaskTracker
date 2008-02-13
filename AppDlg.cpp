/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		APPDLG.CPP
** COMPONENT:	The Application.
** DESCRIPTION:	CAppDlg class definition.
**
*******************************************************************************
*/

#include "Common.hpp"
#include "AppDlg.hpp"
#include "TaskTracker.hpp"
#include <WCL/AutoBool.hpp>

/******************************************************************************
**
** Constants.
**
*******************************************************************************
*/

//! The ID of the background timer.
static const uint TIMER_ID = 10;

//! The frequency of the background timer (ms).
static const uint TIMEOUT  = 1000;

//! The frequency of database backups (ms).
static const DWORD BACKUP_FREQ = 5*60*1000;

/******************************************************************************
** Method:		Default constructor.
**
** Description:	.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

CAppDlg::CAppDlg()
	: CMainDlg(IDD_MAIN)
	, m_dwLastBackup(::GetTickCount())
{
	DEFINE_CTRL_TABLE
		CTRL(IDC_CURRENT_DATE, &m_txtCurrDate)
		CTRL(IDC_CURRENT_TIME, &m_txtCurrTime)
		CTRL(IDC_SESSION_DATE, &m_txtSessionDate)
		CTRL(IDC_SESSION_TIME, &m_txtSessionTime)
		CTRL(IDC_SESSION_LEN,  &m_txtSessionLen)
		CTRL(IDC_SESSION_TASK, &m_txtSessionTask)
		CTRL(IDC_TOTAL_TODAY,  &m_txtTotalToday)
		CTRL(IDC_TOTAL_WEEK,   &m_txtTotalWeek)
		CTRL(IDC_TOTAL_MONTH,  &m_txtTotalMonth)
		CTRL(IDC_TOTAL_TOTAL,  &m_txtTotalTotal)
	END_CTRL_TABLE
}

/******************************************************************************
** Method:		OnInitDialog()
**
** Description:	Initialise the dialog.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppDlg::OnInitDialog()
{
	// Start the clock timer.
	StartTimer(TIMER_ID, TIMEOUT);

	// Get the current time.
	m_dtCurrent.Set();

	// Created underlined dialog font.
	CLogFont oLogFont(Font().Handle());

	oLogFont.lfUnderline = TRUE;

	m_oHeaderFont.Create(oLogFont);

	// Apply to section headers.
	Control(IDC_SESSION_HDR).Font(m_oHeaderFont);
	Control(IDC_TOTAL_HDR  ).Font(m_oHeaderFont);

	// Show inital figures.
	Update();
}

/******************************************************************************
** Method:		OnDestroy()
**
** Description:	Clean up any resources.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppDlg::OnDestroy()
{
	// Stop the clock timer.
	StopTimer(TIMER_ID);
}

/******************************************************************************
** Method:		OnTimer()
**
** Description:	Checks the clock and updates the display if the time has
**				changed.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppDlg::OnTimer(uint /*iTimerID*/)
{
	CDateTime	dtCurrent;
	
	// Get the current time.
	dtCurrent.Set();
	
	// Changed?
	if (m_dtCurrent != dtCurrent)
	{
		// Save current and update.
		m_dtCurrent = dtCurrent;

		Update();
		App.m_AppWnd.UpdateTrayIconTip();
	}

	DWORD dwNow = ::GetTickCount();

	// Time to save a backup?
	if (dwNow > (m_dwLastBackup + BACKUP_FREQ)) 
	{
		// Reentrancy guard, in case of an error.
		static bool g_bGuard = false;

		// Anything to save AND not re-entered?
		if ( (App.m_bModified) && (g_bGuard == false) )
		{
			CAutoBool l(&g_bGuard);

			App.SaveData();
		}

		m_dwLastBackup = dwNow;
	}
}

/******************************************************************************
** Method:		Update()
**
** Description:	Updates the info displayed on the dialog.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppDlg::Update()
{
	// Update current date and time.
	m_txtCurrDate.Text(m_dtCurrent.Date().ToString(CDate::FMT_WIN_SHORT));
	m_txtCurrTime.Text(m_dtCurrent.Time().ToString(CDate::FMT_WIN_LONG));

	// Update current session, if one.
	if (App.m_bClockedIn)
	{
		ASSERT(App.m_pCurrSession.Get() != nullptr);

		// Create a session from clock-in time to now.
		CSession oSession(App.m_pCurrSession->Start(), m_dtCurrent);
		
		// Update fields.
		m_txtSessionDate.Text(oSession.Start().Date().ToString(CDate::FMT_WIN_SHORT));
		m_txtSessionTime.Text(oSession.Start().Time().ToString(CTime::FMT_WIN_SHORT));
		m_txtSessionTask.Text(App.m_pCurrSession->Task());
		m_txtSessionLen.Text(App.MinsToStr(oSession.Length()));
	}
	else
	{
		// Blank out fields.
		m_txtSessionDate.Text(TXT(""));
		m_txtSessionTime.Text(TXT(""));
		m_txtSessionLen.Text(TXT(""));
		m_txtSessionTask.Text(TXT(""));
	}

	// Update totals.
	m_txtTotalToday.Text(App.MinsToStr(App.TotalForDay(m_dtCurrent.Date())));
	m_txtTotalWeek.Text(App.MinsToStr(App.TotalForWeek(m_dtCurrent.Date())));
	m_txtTotalMonth.Text(App.MinsToStr(App.TotalForMonth(m_dtCurrent.Date())));
	m_txtTotalTotal.Text(App.MinsToStr(App.TotalOverall()));
}
