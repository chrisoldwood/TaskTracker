/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		SWTSKDLG.CPP
** COMPONENT:	The Application.
** DESCRIPTION:	CSwitchTasksDlg class definition.
**
*******************************************************************************
*/

#include "AppHeaders.hpp"
#include "SwitchTasksDlg.hpp"

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

CSwitchTasksDlg::CSwitchTasksDlg() : CDialog(IDD_SWITCH_TASKS)
{
	DEFINE_CTRL_TABLE
		CTRL(IDC_PREV_TASK,	&m_cbPrevTask)
		CTRL(IDC_NEXT_TASK,	&m_cbNextTask)
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

void CSwitchTasksDlg::OnInitDialog()
{
	// Initialise tasks.
	m_strPrevTask = "";
	m_strNextTask = "";
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

bool CSwitchTasksDlg::OnOk()
{
	// Get tasks.
	m_strPrevTask = m_cbPrevTask.Text();
	m_strNextTask = m_cbNextTask.Text();

	return true;
}
