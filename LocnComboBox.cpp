/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		LCNCOMBO.CPP
** COMPONENT:	The Application.
** DESCRIPTION:	CLocnCombo class definition.
**
*******************************************************************************
*/

#include "Common.hpp"
#include "LocnComboBox.hpp"
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

CLocnCombo::CLocnCombo()
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

CLocnCombo::~CLocnCombo()
{
}

/******************************************************************************
** Method:		OnCreate()
**
** Description:	Initialise the control with the list of previous locations.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CLocnCombo::OnCreate(const CRect&)
{
	// Template shorthands.
	typedef CLocnList::const_iterator CIter;

	// Add all locations.
	for(CIter oIter = App.m_oLocnList.begin(); oIter != App.m_oLocnList.end(); ++oIter)
		Add(*oIter);
	
	// Select last location, if still valid.
	if (App.m_strLastLocn != TXT(""))
	{
		int iItem = FindExact(App.m_strLastLocn);

		if (iItem != CB_ERR)
			CurSel(iItem);
	}
}
