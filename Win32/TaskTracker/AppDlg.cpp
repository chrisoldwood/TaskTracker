/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		APPDLG.CPP
** COMPONENT:	The Application.
** DESCRIPTION:	CAppDlg class definition.
**
*******************************************************************************
*/

#include "AppHeaders.hpp"

#define TIMER_ID	10
#define TIMEOUT 	1000

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

CAppDlg::CAppDlg() : CMainDlg(IDD_MAIN)
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

void CAppDlg::OnTimer(uint iTimerID)
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
	m_txtCurrDate.Text(m_dtCurrent.Date().ToString());
	m_txtCurrTime.Text(m_dtCurrent.Time().ToString());

	ulong lCurrLen = 0;
	
	// Update current session, if one.
	if (App.ClockedIn())
	{
		// Create a copy of the current session.
		CSession m_CurrSession(*App.CurrentSession());
		
		// Set to finish now.
		m_CurrSession.Finish(m_dtCurrent, App.CurrentSession()->Task());
		
		// Update fields.
		m_txtSessionDate.Text(m_CurrSession.Start().Date().ToString());
		m_txtSessionTime.Text(m_CurrSession.Start().Time().ToString(CTime::HH_MM));
		m_txtSessionTask.Text(m_CurrSession.Task());

		lCurrLen = m_CurrSession.Length();
		m_txtSessionLen.Text(App.MinsToStr(lCurrLen));
	}
	else
	{
		// Blank out fields.
		m_txtSessionDate.Text("");
		m_txtSessionTime.Text("");
		m_txtSessionLen.Text("");
		m_txtSessionTask.Text("");
	}

	// Update totals.
	m_txtTotalToday.Text(App.MinsToStr(App.TotalForDay(m_dtCurrent.Date())));
	m_txtTotalWeek.Text(App.MinsToStr(App.TotalForWeek(m_dtCurrent.Date())));
	m_txtTotalMonth.Text(App.MinsToStr(App.TotalForMonth(m_dtCurrent.Date())));
	m_txtTotalTotal.Text(App.MinsToStr(App.TotalOverall()));
}
