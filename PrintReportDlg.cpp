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

CPrintReportDlg::CPrintReportDlg() : CDialog(IDD_PRINT_REPORT)
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
	m_txtPrinter.Text(App.Printer().m_strName);
	m_txtPort.Text(App.Printer().m_strPort);

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

bool CPrintReportDlg::OnOk()
{
	CPrinter& rPrinter = App.Printer();

	// Check printer selection.
	if ( (rPrinter.m_strName   == "")
	  || (rPrinter.m_strDriver == "") 
	  || (rPrinter.m_strPort   == "") )
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

	// Save prefs.
	App.DefaultGrouping(m_eGrouping);
	App.DefaultPeriod(ePeriod);
	
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
	if (App.Printer().Select(*this))
	{
		// Update printer details.
		m_txtPrinter.Text(App.Printer().m_strName);
		m_txtPort.Text(App.Printer().m_strPort);
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
