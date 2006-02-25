/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		ADDSNDLG.CPP
** COMPONENT:	The Application.
** DESCRIPTION:	CAddSessionDlg class definition.
**
*******************************************************************************
*/

#include "AppHeaders.hpp"
#include "AddSessionDlg.hpp"

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

CAddSessionDlg::CAddSessionDlg() : CDialog(IDD_ADD_SESSION)
{
	DEFINE_CTRL_TABLE
		CTRL(IDC_IN_DATETIME, 	&m_dtpInDateTime)
		CTRL(IDC_OUT_DATETIME, 	&m_dtpOutDateTime)
		CTRL(IDC_TASK,			&m_cbTask)
		CTRL(IDC_LOCN,			&m_cbLocn)
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

void CAddSessionDlg::OnInitDialog()
{
	// Initialise date and time fields.
	m_dtInDateTime.Set();
	m_dtpInDateTime.SetDateTime(m_dtInDateTime);
	m_dtpInDateTime.Format(App.DateTimePickerFormat());

	m_dtOutDateTime.Set();
	m_dtpOutDateTime.SetDateTime(m_dtOutDateTime);
	m_dtpOutDateTime.Format(App.DateTimePickerFormat());

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

bool CAddSessionDlg::OnOk()
{
	// Get session details form the controls
	m_dtInDateTime  = m_dtpInDateTime.GetDateTime();
	m_dtOutDateTime = m_dtpOutDateTime.GetDateTime();

	// Check clocking out later then clocking in.
	if (m_dtOutDateTime < m_dtInDateTime)
	{
		AlertMsg("You cannot clock out ealier than you clocked in.");
		return false;
	}

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
			if ( (m_dtInDateTime < pSession->Finish()) && (m_dtOutDateTime >= pSession->Start()) )
			{
				CString strDate    = pSession->Start().Date().ToString();
				CString strStart   = pSession->Start().Time().ToString(CTime::FMT_WIN_SHORT);
				CString strFinish  = pSession->Finish().Time().ToString(CTime::FMT_WIN_SHORT);
				CString strTask    = pSession->Task();
				CString strSession = CString::Fmt("%s  %s - %s  %s", strDate, strStart, strFinish, strTask);

				// Query user for action.
				if (QueryMsg("This session overlaps another:-\n\n%s\n\nDo you want to continue?", strSession) != IDYES)
					return false;
				else
					break;
			}
		}

	}

	// Get task and location.
	m_strTask = m_cbTask.Text();
	m_strLocn = m_cbLocn.Text();

	// Strip seconds.
	m_dtInDateTime  -= CDateTimeSpan(m_dtInDateTime.Time().Secs());
	m_dtOutDateTime -= CDateTimeSpan(m_dtOutDateTime.Time().Secs());

	return true;
}
