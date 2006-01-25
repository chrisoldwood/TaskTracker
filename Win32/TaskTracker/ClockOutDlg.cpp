/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		CLKOTDLG.CPP
** COMPONENT:	The Application.
** DESCRIPTION:	CClockOutDlg class definition.
**
*******************************************************************************
*/

#include "AppHeaders.hpp"
#include "ClockOutDlg.hpp"

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

CClockOutDlg::CClockOutDlg() : CDialog(IDD_CLOCK_OUT)
{
	DEFINE_CTRL_TABLE
		CTRL(IDC_NOW, 		&m_rbNow)
		CTRL(IDC_ON_AT,		&m_rbOnAt)
		CTRL(IDC_DATETIME, 	&m_dtpDateTime)
		CTRL(IDC_TASK,		&m_cbTask)
		CTRL(IDC_LOCN,		&m_cbLocn)
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

void CClockOutDlg::OnInitDialog()
{
	ASSERT(App.ClockedIn());
	ASSERT(App.CurrentSession().Get() != nullptr);

	// Get the time now.
	CTime tNow;
	tNow.Set();

	// Initialise date and time controls.
	m_dtDateTime = App.CurrentSession()->Start();
	m_dtDateTime.Time(tNow);

	m_dtpDateTime.SetDateTime(m_dtDateTime);
	m_dtpDateTime.Format(App.DateTimePickerFormat());

	// Initalise clock out selection.
	m_rbNow.Check(true);
	m_rbOnAt.Check(false);
	OnNow();

	// Initialise task and location.
	m_strTask = "";
	m_strLocn = "";
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

bool CClockOutDlg::OnOk()
{
	// Clock out now?
	if (m_rbNow.IsChecked())
		m_dtDateTime.Set();
	else
		m_dtDateTime = m_dtpDateTime.GetDateTime();

	// Check clocking out later than clocking in.
	if (m_dtDateTime < App.CurrentSession()->Start())
	{
		AlertMsg("You cannot clock out earlier than you clocked in.");
		return false;
	}
	
	// Get task and location.
	m_strTask = m_cbTask.Text();
	m_strLocn = m_cbLocn.Text();

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

void CClockOutDlg::OnNow()
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

void CClockOutDlg::OnOnAt()
{
	m_dtpDateTime.Enable(true);
}
