/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		CLKINDLG.CPP
** COMPONENT:	The Application.
** DESCRIPTION:	CClockInDlg class definition.
**
*******************************************************************************
*/

#include "Common.hpp"
#include "ClockInDlg.hpp"
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

CClockInDlg::CClockInDlg() : CDialog(IDD_CLOCK_IN)
{
	DEFINE_CTRL_TABLE
		CTRL(IDC_NOW, 		&m_rbNow)
		CTRL(IDC_ON_AT,		&m_rbOnAt)
		CTRL(IDC_DATETIME, 	&m_dtpDateTime)
		CTRL(IDC_TASK,		&m_cbTask)
		CTRL(IDC_LOCN,		&m_cbLocn)
	END_CTRL_TABLE

	DEFINE_CTRLMSG_TABLE
		CMD_CTRLMSG(IDC_NOW, 	BN_CLICKED,	&CClockInDlg::OnNow)
		CMD_CTRLMSG(IDC_ON_AT,	BN_CLICKED,	&CClockInDlg::OnOnAt)
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
	m_dtpDateTime.Format(App.DateTimePickerFormat());

	// Initalise clock in selection.
	m_rbNow.Check(true);
	m_rbOnAt.Check(false);
	OnNow();
	
	// Initialise task and location.
	m_strTask = TXT("");
	m_strLocn = TXT("");
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

	// Check for session overlap?
	if (App.m_bCheckOverlap)
	{
		// Template shorthands.
		typedef CSessionList::const_iterator CIter;

		// Check all sessions...
		for(CIter oIter = App.m_oSessionList.begin(); oIter != App.m_oSessionList.end(); ++oIter)
		{
			CSessionPtr pSession = *oIter;

			// Overlaps another?
			if ( (m_dtDateTime >= pSession->Start()) && (m_dtDateTime < pSession->Finish()) )
			{
				CString strDate    = pSession->Start().Date().ToString();
				CString strStart   = pSession->Start().Time().ToString(CTime::FMT_WIN_SHORT);
				CString strFinish  = pSession->Finish().Time().ToString(CTime::FMT_WIN_SHORT);
				CString strTask    = pSession->Task();
				CString strSession = CString::Fmt(TXT("%s  %s - %s  %s"), strDate.c_str(), strStart.c_str(), strFinish.c_str(), strTask.c_str());

				// Query user for action.
				if (QueryMsg(TXT("This session will overlap another:-\n\n%s\n\nDo you want to continue?"), strSession.c_str()) != IDYES)
					return false;
				else
					break;
			}
		}

	}

	// Get task and location.
	m_strTask = m_cbTask.Text();
	m_strLocn = m_cbLocn.Text();

	// Strip seconds as we only store HH:MM:00
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
