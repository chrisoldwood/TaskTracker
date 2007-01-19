/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		APPTOOLBAR.CPP
** COMPONENT:	The Application.
** DESCRIPTION:	CAppToolBar class definition.
**
*******************************************************************************
*/

#include "AppHeaders.hpp"

/******************************************************************************
** Method:		Constructor.
**
** Description:	.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

CAppToolBar::CAppToolBar()
{
	// Define the toolbar.
	DEFINE_TOOLBAR
		TBCTL(&m_ExportBtn,			ID_FILE_EXPORT)
		TBCTL(&m_ImportBtn,			ID_FILE_IMPORT)
		TBGAP()
		TBCTL(&m_ClockInBtn,		ID_SESSION_CLOCK_IN)
		TBCTL(&m_SwitchTasksBtn,	ID_SESSION_SWITCH_TASKS)
		TBCTL(&m_ClockOutBtn,		ID_SESSION_CLOCK_OUT)
		TBCTL(&m_EditSessionsBtn,	ID_SESSION_EDIT)
		TBGAP()
		TBCTL(&m_WindowReportBtn,	ID_REPORT_WINDOW)
		TBCTL(&m_ClpbrdReportBtn,	ID_REPORT_CLIPBOARD)
		TBCTL(&m_PrintReportBtn,	ID_REPORT_PRINT)
		TBCTL(&m_FileReportBtn,		ID_REPORT_FILE)
		TBGAP()
		TBCTL(&m_HelpBtn,			ID_HELP_CONTENTS)
	END_TOOLBAR
}

/******************************************************************************
** Method:		Destructor.
**
** Description:	.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

CAppToolBar::~CAppToolBar()
{
}
