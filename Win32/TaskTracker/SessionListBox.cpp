/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		SESSNLB.CPP
** COMPONENT:	The Application.
** DESCRIPTION:	CSessionListBox class definition.
**
*******************************************************************************
*/

#include "AppHeaders.hpp"
#include <stdio.h>

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

CSessionListBox::CSessionListBox()
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

CSessionListBox::~CSessionListBox()
{
}

/******************************************************************************
** Method:		OnCreate()
**
** Description:	Initialise the control with the list of sessions.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CSessionListBox::OnCreate(const CRect&)
{
	// Set the tab stops.
	enum { NUM_TABSTOPS = 3 };
	int aiTabStops[NUM_TABSTOPS] = { 40, 70, 100 };

	SetTabStops(NUM_TABSTOPS, aiTabStops);

	// Fill listbox.
	Refresh();
	
	// Select 1st item, if one.
	if (App.SessionList().Length())
		CurSel(0);
}

/******************************************************************************
** Method:		Refresh()
**
** Description:	Refresh the list of sessions.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CSessionListBox::Refresh() const
{
	// Save current selection.
	int iSel = CurSel();

	// Empty listbox.
	Reset();

	CSessionEnum	Enum(App.SessionList());
	CSession*		pSession;
	CString			strSession;
	
	// For all sessions within the period.
	while((pSession = Enum.Next()) != NULL)
	{
		CString	strStartDate = pSession->Start().Date().ToString(CDate::DD_MM_YY);
		CString	strStartTime = pSession->Start().Time().ToString(CTime::HH_MM);
		CString	strEndTime   = pSession->Finish().Time().ToString(CTime::HH_MM);
		CString strTask      = pSession->Task();
	
		// Convert to a string and add.
		strSession.Format("%s\t%s\t%s\t%s", strStartDate, strStartTime, strEndTime, strTask);
		int iIdx = Add(strSession);
		
		// Add session as item data.
		ItemData(iIdx, (LPARAM)pSession);
    }
    
    // Was current selection?
    if (iSel != LB_ERR)
    	CurSel(iSel);
    else
    	CurSel(0);
}
