/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		FILEREPORTDLG.CPP
** COMPONENT:	The Application.
** DESCRIPTION:	CFileReportDlg class definition.
**
*******************************************************************************
*/

#include "AppHeaders.hpp"
#include "FileReportDlg.hpp"

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

CFileReportDlg::CFileReportDlg() : CDialog(IDD_FILE_REPORT)
{
	DEFINE_CTRL_TABLE
		CTRL(IDC_FILENAME,		&m_ebFileName)
		CTRL(IDC_GROUP_NONE,	&m_rbGroupByNone)
		CTRL(IDC_GROUP_WEEK,	&m_rbGroupByWeek)
		CTRL(IDC_GROUP_MONTH,	&m_rbGroupByMonth)
		CTRL(IDC_GROUP_TASK,	&m_rbGroupByTask)
		CTRL(IDC_PERIOD,		&m_cbPeriod)
		CTRL(IDC_FROM_DATE,		&m_dtpFromDate)
		CTRL(IDC_TO_DATE, 		&m_dtpToDate)
	END_CTRL_TABLE

	DEFINE_CTRLMSG_TABLE
		CMD_CTRLMSG(IDC_BROWSE,	BN_CLICKED,		OnBrowse)
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

void CFileReportDlg::OnInitDialog()
{
	// Initialise filename.
	m_strFileName = App.DefaultFile();
	m_ebFileName.Text(m_strFileName);

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

bool CFileReportDlg::OnOk()
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

	// Get filename from edit control.
	m_strFileName = m_ebFileName.Text();

	// Save prefs.
	App.DefaultFile(m_strFileName);
	App.DefaultGrouping(m_eGrouping);
	App.DefaultPeriod(ePeriod);
	
	return true;
}

/******************************************************************************
** Method:		OnBrowse()
**
** Description:	Show the file dialog to select a file.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CFileReportDlg::OnBrowse()
{
	// File extensions.
	static char szExts[] = {	"Text Files (*.TXT)\0*.TXT\0"
								"All Files (*.*)\0*.*\0"
								"\0\0"							};

	// Select a filename.
	if (m_strFileName.Select(*this, CPath::SaveFile, szExts, "TXT"))
	{
		// Update the dialog.
		m_ebFileName.Text(m_strFileName);
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

void CFileReportDlg::OnPeriodChange()
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
