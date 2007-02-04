/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		MODSNDLG.CPP
** COMPONENT:	The Application.
** DESCRIPTION:	CModifySessionDlg class definition.
**
*******************************************************************************
*/

#include "AppHeaders.hpp"
#include "ModifySessionDlg.hpp"

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

CModifySessionDlg::CModifySessionDlg() : CDialog(IDD_MODIFY_SESSION)
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

void CModifySessionDlg::OnInitDialog()
{
	// Initialise date and time fields.
	m_dtpInDateTime.SetDateTime(m_dtInDateTime);
	m_dtpInDateTime.Format(App.DateTimePickerFormat());

	m_dtpOutDateTime.SetDateTime(m_dtOutDateTime);
	m_dtpOutDateTime.Format(App.DateTimePickerFormat());

	int nTaskIdx = -1;
	int nLocnIdx = -1;

	// Initialise task.
	if (m_strTask != "")
	{
		nTaskIdx = m_cbTask.FindExact(m_strTask);

		ASSERT(nTaskIdx != CB_ERR);
	}

	// Initialise task.
	if (m_strLocn != "")
	{
		nLocnIdx = m_cbLocn.FindExact(m_strLocn);

		ASSERT(nLocnIdx != CB_ERR);
	}

	m_cbTask.CurSel(nTaskIdx);
	m_cbLocn.CurSel(nLocnIdx);
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

bool CModifySessionDlg::OnOk()
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

			// Ignore if the session we're editing.
			if (pSession == m_pSession)
				continue;

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

	// Strip seconds as we only store HH:MM:00
	m_dtInDateTime  -= CDateTimeSpan(m_dtInDateTime.Time().Secs());
	m_dtOutDateTime -= CDateTimeSpan(m_dtOutDateTime.Time().Secs());

	return true;
}
