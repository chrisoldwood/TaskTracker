/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		CLPWNDREPORTDLG.CPP
** COMPONENT:	The Application.
** DESCRIPTION:	CClpWndReportDlg class definition.
**
*******************************************************************************
*/

#include "AppHeaders.hpp"
#include "ClpWndReportDlg.hpp"

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

CClpWndReportDlg::CClpWndReportDlg() : CDialog(IDD_CLPWND_REPORT)
{
	DEFINE_CTRL_TABLE
		CTRL(IDC_GROUP_NONE,	&m_rbGroupByNone)
		CTRL(IDC_GROUP_WEEK,	&m_rbGroupByWeek)
		CTRL(IDC_GROUP_MONTH,	&m_rbGroupByMonth)
		CTRL(IDC_GROUP_TASK,	&m_rbGroupByTask)
		CTRL(IDC_PERIOD,		&m_cbPeriod)
		CTRL(IDC_FROM_DATE,		&m_dtpFromDate)
		CTRL(IDC_TO_DATE, 		&m_dtpToDate)
	END_CTRL_TABLE

	DEFINE_CTRLMSG_TABLE
		CMD_CTRLMSG(IDC_PERIOD,	CBN_SELCHANGE,	OnPeriodChange)
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
	switch (App.DefaultGrouping())
	{
		case Ungrouped:	m_rbGroupByNone.Check(true);	break;
		case ByWeek:	m_rbGroupByWeek.Check(true);	break;
		case ByMonth:	m_rbGroupByMonth.Check(true);	break;
		case ByTask:	m_rbGroupByTask.Check(true);	break;
		default:		ASSERT(false);					break;
	}
	
	// Fill period list.
	m_cbPeriod.Add("All");
	m_cbPeriod.Add("This Week");
	m_cbPeriod.Add("This Month");
	m_cbPeriod.Add("Last Week");
	m_cbPeriod.Add("Last Month");
	m_cbPeriod.Add("Custom...");
	
	// Select default.
	m_cbPeriod.CurSel(App.DefaultPeriod());

	// Initialise date fields.
	OnPeriodChange();

	m_dtpFromDate.Format(DTP_DATE_FORMAT);
	m_dtpToDate.Format(DTP_DATE_FORMAT);
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
			AlertMsg("'From' date must be before 'To' date.");
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

	// Save prefs.
	App.DefaultGrouping(m_eGrouping);
	App.DefaultPeriod(ePeriod);
	
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
	int i = m_cbPeriod.CurSel();
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
