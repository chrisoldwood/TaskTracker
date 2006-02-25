/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		LCNCOMBO.CPP
** COMPONENT:	The Application.
** DESCRIPTION:	CLocnCombo class definition.
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
	for(CIter oIter = App.LocnList().begin(); oIter != App.LocnList().end(); ++oIter)
		Add(*oIter);
	
	// Select last location, if one
	if (App.LastLocn() != "")
	{
		int iItem = FindExact(App.LastLocn());
		ASSERT(iItem != CB_ERR);
		
		CurSel(iItem);
	}
}