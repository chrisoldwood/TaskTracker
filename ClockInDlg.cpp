/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		CLKINDLG.CPP
** COMPONENT:	The Application.
** DESCRIPTION:	CClockInDlg class definition.
**
*******************************************************************************
*/

#include "AppHeaders.hpp"
#include "ClockInDlg.hpp"

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

CClockInDlg::CClockInDlg() : CDialog(IDD_CLOCK_IN)
{
	DEFINE_CTRL_TABLE
		CTRL(IDC_NOW, 		&m_rbNow)
		CTRL(IDC_ON_AT,		&m_rbOnAt)
		CTRL(IDC_DATETIME, 	&m_dtpDateTime)
		CTRL(IDC_TASK,		&m_cbTask)
	END_CTRL_TABLE

	DEFINE_CTRLMSG_TABLE
		CMD_CTRLMSG(IDC_NOW, 	BN_CLICKED,	OnNow)
		CMD_CTRLMSG(IDC_ON_AT,	BN_CLICKED,	OnOnAt)
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

void CClockInDlg::OnInitDialog()
{
	// Initialise date and time fields.
	m_dtDateTime.Set();
	m_dtpDateTime.SetDateTime(m_dtDateTime);
	m_dtpDateTime.Format(DTP_DATETIME_FORMAT);

	// Initalise clock in selection.
	m_rbNow.Check(true);
	m_rbOnAt.Check(false);
	OnNow();
	
	// Initialise task.
	m_strTask = "";
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

bool CClockInDlg::OnOk()
{
	// Clock in now?
	if (m_rbNow.IsChecked())
		m_dtDateTime.Set();
	else
		m_dtDateTime = m_dtpDateTime.GetDateTime();

	// Get task.
	m_strTask = m_cbTask.Text();

	// Strip seconds.
	m_dtDateTime -= CDateTimeSpan(m_dtDateTime.Time().Secs());

	return true;
}

/******************************************************************************
** Method:		OnNow()
**
** Description:	Disable the date and time fields.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CClockInDlg::OnNow()
{
	m_dtpDateTime.Enable(false);
}

/******************************************************************************
** Method:		OnAt()
**
** Description:	Enable the date and time fields.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CClockInDlg::OnOnAt()
{
	m_dtpDateTime.Enable(true);
}
