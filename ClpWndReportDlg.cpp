/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		CLPWNDREPORTDLG.CPP
** COMPONENT:	The Application.
** DESCRIPTION:	CClpWndReportDlg class definition.
**
*******************************************************************************
*/

#include "Common.hpp"
#include "ClpWndReportDlg.hpp"
#include "TaskTracker.hpp"

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

CClpWndReportDlg::CClpWndReportDlg()
	: CDialog(IDD_CLPWND_REPORT)
{
	DEFINE_CTRL_TABLE
		CTRL(IDC_GROUP_NONE,	&m_rbGroupByNone)
		CTRL(IDC_GROUP_WEEK,	&m_rbGroupByWeek)
		CTRL(IDC_GROUP_MONTH,	&m_rbGroupByMonth)
		CTRL(IDC_GROUP_TASK,	&m_rbGroupByTask)
		CTRL(IDC_PERIOD,		&m_cbPeriod)
		CTRL(IDC_FROM_DATE,		&m_dtpFromDate)
		CTRL(IDC_TO_DATE, 		&m_dtpToDate)
		CTRL(IDC_SHOW_SESSIONS,	&m_ckShowSessions)
		CTRL(IDC_SHOW_EMPTY,	&m_ckShowIfEmpty)
		CTRL(IDC_SHOW_TOTAL,	&m_ckShowTotal)
		CTRL(IDC_REMEMBER, 		&m_ckRemember)
	END_CTRL_TABLE

	DEFINE_CTRLMSG_TABLE
		CMD_CTRLMSG(IDC_PERIOD,	CBN_SELCHANGE,	&CClpWndReportDlg::OnPeriodChange)
	END_CTRLMSG_TABLE
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

void CClpWndReportDlg::OnInitDialog()
{
	Title(m_strTitle);

	// Select default grouping.
	switch (App.m_eDefGrouping)
	{
		case Ungrouped:	m_rbGroupByNone.Check(true);	break;
		case ByWeek:	m_rbGroupByWeek.Check(true);	break;
		case ByMonth:	m_rbGroupByMonth.Check(true);	break;
		case ByTask:	m_rbGroupByTask.Check(true);	break;
		default:		ASSERT_FALSE();					break;
	}
	
	// Fill period list.
	m_cbPeriod.Add(TXT("All"));
	m_cbPeriod.Add(TXT("This Week"));
	m_cbPeriod.Add(TXT("This Month"));
	m_cbPeriod.Add(TXT("Last Week"));
	m_cbPeriod.Add(TXT("Last Month"));
	m_cbPeriod.Add(TXT("Custom..."));
	
	// Select default.
	m_cbPeriod.CurSel(App.m_eDefPeriod);

	// Initialise date fields.
	OnPeriodChange();

	m_dtpFromDate.Format(App.DatePickerFormat());
	m_dtpToDate.Format(App.DatePickerFormat());

	// Initialise other fields.
	m_ckShowSessions.Check(App.m_bDefShowSessions);
	m_ckShowIfEmpty.Check(App.m_bDefShowIfEmpty);
	m_ckShowTotal.Check(App.m_bDefShowTotal);
	m_ckRemember.Check(true);
}

/******************************************************************************
** Method:		OnOk()
**
** Description:	Check if the user can okay the dialog.
**
** Parameters:	None.
**
** Returns:		true or false.
**
*******************************************************************************
*/

bool CClpWndReportDlg::OnOk()
{
	// Get period selection.
	Period ePeriod = (Period) m_cbPeriod.CurSel();

	// Custom period?
	if (ePeriod == Custom)
	{
		// Get dates from the controls.
		m_FromDate = m_dtpFromDate.GetDate();
		m_ToDate   = m_dtpToDate.GetDate();

		// Check from date is before to date.
		if (m_FromDate > m_ToDate)
		{
			AlertMsg(TXT("'From' date must be before 'To' date."));
			return false;
		}
	}

	// Get grouping selection.
	if (m_rbGroupByNone.IsChecked())
		m_eGrouping = Ungrouped;
	else if (m_rbGroupByWeek.IsChecked())
		m_eGrouping = ByWeek;
	else if (m_rbGroupByMonth.IsChecked())
		m_eGrouping = ByMonth;
	else if (m_rbGroupByTask.IsChecked())
		m_eGrouping = ByTask;

	// Get other settings.
	m_bShowSessions = m_ckShowSessions.IsChecked();
	m_bShowIfEmpty  = m_ckShowIfEmpty.IsChecked();
	m_bShowTotal    = m_ckShowTotal.IsChecked();

	// Save prefs, if required.
	if (m_ckRemember.IsChecked())
	{
		App.m_eDefGrouping     = m_eGrouping;
		App.m_eDefPeriod       = ePeriod;
		App.m_bDefShowSessions = m_bShowSessions;
		App.m_bDefShowIfEmpty  = m_bShowIfEmpty;
		App.m_bDefShowTotal    = m_bShowTotal;
	}

	return true;
}

/******************************************************************************
** Method:		OnPeriodChange()
**
** Description:	Update the from and to date controls' states.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CClpWndReportDlg::OnPeriodChange()
{
	// Get current selection.
	size_t i = m_cbPeriod.CurSel();
	ASSERT(i != CB_ERR);

	// Convert selelction to date range.
	App.PeriodToDates((Period)i, m_FromDate, m_ToDate);
	
	// Update date fields.
	m_dtpFromDate.SetDate(m_FromDate);
	m_dtpToDate.SetDate(m_ToDate);

	// Custom period?
	if (i == Custom)
	{
		// Enable date fields.
		m_dtpFromDate.Enable(true);
		m_dtpToDate.Enable(true);
	}
	else
	{
		// Disable date fields.
		m_dtpFromDate.Enable(false);
		m_dtpToDate.Enable(false);
	}
}
