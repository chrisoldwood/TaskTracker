/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		PRSESDLG.CPP
** COMPONENT:	The Application.
** DESCRIPTION:	CPruneSessionsDlg class definition.
**
*******************************************************************************
*/

#include "AppHeaders.hpp"
#include "PruneSessionsDlg.hpp"

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

CPruneSessionsDlg::CPruneSessionsDlg() : CDialog(IDD_PRUNE_SESSIONS)
{
	DEFINE_CTRL_TABLE
		CTRL(IDC_ALL, 		&m_rbAll)
		CTRL(IDC_BEFORE,	&m_rbBefore)
		CTRL(IDC_DATE, 		&m_dtpDate)
	END_CTRL_TABLE

	DEFINE_CTRLMSG_TABLE
		CMD_CTRLMSG(IDC_ALL, 	BN_CLICKED,	OnAll)
		CMD_CTRLMSG(IDC_BEFORE,	BN_CLICKED,	OnBefore)
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
	// Initialise date and time fields.
	m_Date.Set();
	m_dtpDate.SetDate(m_Date);
	m_dtpDate.Format(App.DatePickerFormat());

	// Initalise prune selection.
	m_rbAll.Check(true);
	m_rbBefore.Check(false);
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
	// Delete all data?
	if (m_rbAll.IsChecked())
		m_Date = CDate::Max();
	else
		m_Date = m_dtpDate.GetDate();

	return true;
}

/******************************************************************************
** Method:		OnAll()
**
** Description:	Disable the date field.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CPruneSessionsDlg::OnAll()
{
	m_dtpDate.Enable(false);
}

/******************************************************************************
** Method:		OnBefore()
**
** Description:	Enable the date field.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CPruneSessionsDlg::OnBefore()
{
	m_dtpDate.Enable(true);
}
