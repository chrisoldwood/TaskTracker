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
	// Template shorthands.
	typedef CSessionList::iterator Iter;

	// Change style.
	FullRowSelect(true);
	GridLines(true);

	// Add the columns.
	for (int i = 0; i < NUM_COLUMNS; i++)
		InsertColumn(i, apszLabels[i], aiWidths[i]);

	// Populate the listview...
	for(Iter oIter = App.m_oSessionList.begin(); oIter != App.m_oSessionList.end(); ++oIter)
		AddSesion(ItemCount(), *oIter);
	
	// Select 1st item, if one.
	if (ItemCount())
		Select(0);
}

/******************************************************************************
** Method:		AddSesion()
**
** Description:	Adds the specified session to the view.
**				NB: We store a pointer to the SharedPtr wrapper as the item
**				data, so it must not be a temporary.
**
** Parameters:	nItem	The index of the item to add.
**				oIter	An SessionList iterator to the session.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CSessionListView::AddSesion(int nItem, CSessionPtr& pSession)
{
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
	int i = InsertItem(nItem, "");

	ItemText(i, 0, strStartDay);
	ItemText(i, 1, strStartDate);
	ItemText(i, 2, strStartTime);
	ItemText(i, 3, strEndTime);
	ItemText(i, 4, strLength);
	ItemText(i, 5, strTask);
	ItemText(i, 6, strLocation);
	ItemPtr (i,    &pSession);
}

/******************************************************************************
** Method:		RemoveSession()
**
** Description:	Removes the specified item from the view.
**
** Parameters:	nItem	The index of the item to remove.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CSessionListView::RemoveSession(int nItem)
{
	DeleteItem(nItem);

	// Move selection.
	Select((nItem < ItemCount()) ? nItem : nItem-1);
}

/******************************************************************************
** Method:		SelSession()
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
	int         nIndex = Selection();

	// Is a selected session?
	if (nIndex != LB_ERR)
		pSession = App.m_oSessionList[nIndex];

	return pSession;
}
