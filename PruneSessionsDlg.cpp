/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		PRSESDLG.CPP
** COMPONENT:	The Application.
** DESCRIPTION:	CPruneSessionsDlg class definition.
**
*******************************************************************************
*/

#include "Common.hpp"
#include "PruneSessionsDlg.hpp"
#include "TaskTracker.hpp"

/******************************************************************************
** Method:		Default constructor.
**
** Description:	Override any default settings.
**
** Parameters:	pParent		The dialogs' parent window.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

CPruneSessionsDlg::CPruneSessionsDlg()
	: CDialog(IDD_PRUNE_SESSIONS)
{
	DEFINE_CTRL_TABLE
		CTRL(IDC_ALL,       &m_rbAll)
		CTRL(IDC_BETWEEN,   &m_rbBetween)
		CTRL(IDC_FROM_DATE,	&m_dtpFromDate)
		CTRL(IDC_TO_DATE, 	&m_dtpToDate)
	END_CTRL_TABLE

	DEFINE_CTRLMSG_TABLE
		CMD_CTRLMSG(IDC_ALL,     BN_CLICKED, &CPruneSessionsDlg::OnAll)
		CMD_CTRLMSG(IDC_BETWEEN, BN_CLICKED, &CPruneSessionsDlg::OnBetween)
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

void CPruneSessionsDlg::OnInitDialog()
{
	ASSERT(!App.m_oSessionList.empty());

	// Initialise controls.
	m_dtpFromDate.Format(App.DatePickerFormat());
	m_dtpToDate.Format(App.DatePickerFormat());

	// Set dates to database limits.
	m_dtpFromDate.SetDate(App.m_oSessionList.front()->Start().Date());
	m_dtpToDate.SetDate(App.m_oSessionList.back()->Start().Date());

	// Initalise prune selection.
	m_rbAll.Check(true);
	m_rbBetween.Check(false);
	OnAll();
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

bool CPruneSessionsDlg::OnOk()
{
	// Validate controls.
	if ( (m_rbBetween.IsChecked()) && (m_dtpToDate.GetDate() < m_dtpFromDate.GetDate()) )
	{
		AlertMsg("The 'End' date must be later than the 'Start' date.");
		m_dtpToDate.Focus();
		return false;
	}

	// Delete all data?
	if (m_rbAll.IsChecked())
	{
		m_dFromDate = CDate::Min();
		m_dToDate   = CDate::Max();
	}
	// Delete range.
	else
	{
		m_dFromDate = m_dtpFromDate.GetDate();
		m_dToDate   = m_dtpToDate.GetDate();
	}

	return true;
}

/******************************************************************************
** Method:		OnAll()
**
** Description:	Disable the date fields.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CPruneSessionsDlg::OnAll()
{
	m_dtpToDate.Enable(false);
	m_dtpFromDate.Enable(false);
}

/******************************************************************************
** Method:		OnBetween()
**
** Description:	Enable the date fields.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CPruneSessionsDlg::OnBetween()
{
	m_dtpToDate.Enable(true);
	m_dtpFromDate.Enable(true);
}
