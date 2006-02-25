/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		PRINTREPORTDLG.CPP
** COMPONENT:	The Application.
** DESCRIPTION:	CPrintReportDlg class definition.
**
*******************************************************************************
*/

#include "AppHeaders.hpp"
#include "PrintReportDlg.hpp"

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

CPrintReportDlg::CPrintReportDlg()
	: CDialog(IDD_PRINT_REPORT)
{
	DEFINE_CTRL_TABLE
		CTRL(IDC_PRINTER,		&m_txtPrinter)
		CTRL(IDC_PORT,			&m_txtPort)
		CTRL(IDC_GROUP_NONE,	&m_rbGroupByNone)
		CTRL(IDC_GROUP_WEEK,	&m_rbGroupByWeek)
		CTRL(IDC_GROUP_MONTH,	&m_rbGroupByMonth)
		CTRL(IDC_GROUP_TASK,	&m_rbGroupByTask)
		CTRL(IDC_PERIOD,		&m_cbPeriod)
		CTRL(IDC_FROM_DATE,		&m_dtpFromDate)
		CTRL(IDC_TO_DATE, 		&m_dtpToDate)
		CTRL(IDC_REMEMBER, 		&m_ckRemember)
	END_CTRL_TABLE

	DEFINE_CTRLMSG_TABLE
		CMD_CTRLMSG(IDC_SELECT,	BN_CLICKED,		OnSelect)
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

void CPrintReportDlg::OnInitDialog()
{
	// Show printer details.
	m_txtPrinter.Text(App.m_oPrinter.m_strName);
	m_txtPort.Text(App.m_oPrinter.m_strPort);

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
	m_cbPeriod.Add("All");
	m_cbPeriod.Add("This Week");
	m_cbPeriod.Add("This Month");
	m_cbPeriod.Add("Last Week");
	m_cbPeriod.Add("Last Month");
	m_cbPeriod.Add("Custom...");
	
	// Select default.
	m_cbPeriod.CurSel(App.m_eDefPeriod);

	// Initialise date fields.
	OnPeriodChange();

	m_dtpFromDate.Format(App.DatePickerFormat());
	m_dtpToDate.Format(App.DatePickerFormat());

	// Initialise other fields.
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

bool CPrintReportDlg::OnOk()
{
	// Check printer selection.
	if ( (App.m_oPrinter.m_strName   == "")
	  || (App.m_oPrinter.m_strDriver == "") 
	  || (App.m_oPrinter.m_strPort   == "") )
	{
		AlertMsg("You must select a printer.");
		return false;
	}

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

	// Save prefs, if required.
	if (m_ckRemember.IsChecked())
	{
		App.m_eDefGrouping = m_eGrouping;
		App.m_eDefPeriod   = ePeriod;
	}
	
	return true;
}

/******************************************************************************
** Method:		OnSelect()
**
** Description:	Show the print dialog to select a printer.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CPrintReportDlg::OnSelect()
{
	CBusyCursor BusyCursor;

	// Select a new printer.
	if (App.m_oPrinter.Select(*this))
	{
		// Update printer details.
		m_txtPrinter.Text(App.m_oPrinter.m_strName);
		m_txtPort.Text(App.m_oPrinter.m_strPort);
	}
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

void CPrintReportDlg::OnPeriodChange()
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
