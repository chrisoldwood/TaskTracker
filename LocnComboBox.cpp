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
	CLocnListEnum	Enum(App.LocnList());
	CString*		pString;
	
	// Add all locations.
	while((pString = Enum.Next()) != NULL)
		Add(*pString);
	
	// Select last location, if one
	if (App.LastLocn() != "")
	{
		int iItem = FindExact(App.LastLocn());
		ASSERT(iItem != CB_ERR);
		
		CurSel(iItem);
	}
}
