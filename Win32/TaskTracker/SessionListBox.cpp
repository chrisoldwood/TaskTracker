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
{    40,     75,   50,    50,       65,    100,         80 };

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
	// Template shorthands.
	typedef CSessionList::const_iterator CIter;

	// Save current selection.
	int iSel = Selection();

	// Empty contents.
	DeleteAllItems();

	// For all sessions...
	for(CIter oIter = App.m_oSessionList.begin(); oIter != App.m_oSessionList.end(); ++oIter)
	{
		CSessionPtr pSession = *oIter;

		CString strStartDay  = pSession->Start().Date().DayOfWeekStr(false);
		CString	strStartDate = pSession->Start().Date().ToString(CDate::FMT_WIN_SHORT);
		CString	strStartTime = pSession->Start().Time().ToString(CTime::FMT_WIN_SHORT);
		CString	strEndTime   = pSession->Finish().Time().ToString(CTime::FMT_WIN_SHORT);
		CString strTask      = pSession->Task();
		CString strLocation  = pSession->Location();
		CString strLength    = App.MinsToStr(pSession->Length());

		// Add flag, if clocked-out on a different day.
		if (pSession->Start().Date() != pSession->Finish().Date())
			strEndTime += '*';

		// Add the item.
		int i = AppendItem("");

		ItemText(i, 0, strStartDay);
		ItemText(i, 1, strStartDate);
		ItemText(i, 2, strStartTime);
		ItemText(i, 3, strEndTime);
		ItemText(i, 4, strLength);
		ItemText(i, 5, strTask);
		ItemText(i, 6, strLocation);
		ItemPtr (i,    &(*oIter));
    }

	RestoreSel(iSel);
}

/******************************************************************************
** Method:		SelSession.
**
** Description:	Gets the currently selected session.
**
** Parameters:	None.
**
** Returns:		The currently selected session or nullptr, if none.
**
*******************************************************************************
*/

CSessionPtr CSessionListView::SelSession()
{
	CSessionPtr pSession;

	// Is a selected session?
	if (IsSelection())
		pSession = *static_cast<CSessionPtr*>(ItemPtr(Selection()));

	return pSession;
}
