/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		TSKCOMBO.CPP
** COMPONENT:	The Application.
** DESCRIPTION:	CTaskCombo class definition.
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
	
	// Select last task, if one
	if (App.m_strLastTask != "")
	{
		int iItem = FindExact(App.m_strLastTask);
		ASSERT(iItem != CB_ERR);
		
		CurSel(iItem);
	}
}
