/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		TSKCOMBO.CPP
** COMPONENT:	The Application.
** DESCRIPTION:	CTaskCombo class definition.
**
*******************************************************************************
*/

#include "Common.hpp"
#include "TaskComboBox.hpp"
#include "TaskTracker.hpp"

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

CTaskCombo::CTaskCombo()
{
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

CTaskCombo::~CTaskCombo()
{
}

/******************************************************************************
** Method:		OnCreate()
**
** Description:	Initialise the control with the list of previous tasks.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CTaskCombo::OnCreate(const CRect&)
{
	// Template shorthands.
	typedef CTaskList::const_iterator CIter;

	// Add all tasks.
	for(CIter oIter = App.m_oTaskList.begin(); oIter != App.m_oTaskList.end(); ++oIter)
		Add(*oIter);
	
	// Select last task, if still valid.
	if (App.m_strLastTask != TXT(""))
	{
		int iItem = FindExact(App.m_strLastTask);

		if (iItem != CB_ERR)
			CurSel(iItem);
	}
}
