/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		SESSNLB.CPP
** COMPONENT:	The Application.
** DESCRIPTION:	CSessionListView class definition.
**
*******************************************************************************
*/

#include "AppHeaders.hpp"
#include <stdio.h>

/******************************************************************************
** Column definitions.
*/

const char*	CSessionListView::apszLabels[NUM_COLUMNS] =
{ "Day", "Date", "In", "Out", "Length", "Task", "Location" };

const int	CSessionListView::aiWidths[NUM_COLUMNS] = 
{    40,     60,   50,    50,       50,    125,         75 };

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

CSessionListView::CSessionListView()
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

CSessionListView::~CSessionListView()
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

void CSessionListView::OnCreate(const CRect&)
{
	// Change style.
	FullRowSelect(true);
	GridLines(true);

	// Add the columns.
	for (int i = 0; i < NUM_COLUMNS; i++)
		InsertColumn(i, apszLabels[i], aiWidths[i]);

	// Fill listbox.
	Refresh();
	
	// Select 1st item, if one.
	if (ItemCount())
		Select(0);
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

void CSessionListView::Refresh()
{
	// Save current selection.
	int iSel = Selected();

	// Empty contents.
	DeleteAllItems();

	CSessionEnum	Enum(App.SessionList());
	CSession*		pSession;
	
	// For all sessions within the period.
	while((pSession = Enum.Next()) != NULL)
	{
		// Get length in minutes.
		int nLength = pSession->Length();

		CString strStartDay  = pSession->Start().Date().ToString(CDate::SD);
		CString	strStartDate = pSession->Start().Date().ToString(CDate::DD_MM_YY);
		CString	strStartTime = pSession->Start().Time().ToString(CTime::HH_MM);
		CString	strEndTime   = pSession->Finish().Time().ToString(CTime::HH_MM);
		CString strTask      = pSession->Task();
		CString strLocation  = pSession->Location();
		CString strLength;

		// Create length string.
		strLength.Format("%02d:%02d", nLength / 60,  nLength % 60);

		// Add the item.
		int i = AppendItem("", pSession);

		ItemText(i, 0, strStartDay);
		ItemText(i, 1, strStartDate);
		ItemText(i, 2, strStartTime);
		ItemText(i, 3, strEndTime);
		ItemText(i, 4, strLength);
		ItemText(i, 5, strTask);
		ItemText(i, 6, strLocation);
    }

	RestoreSel(iSel);
}
