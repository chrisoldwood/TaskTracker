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
		AddSession(ItemCount(), *oIter);
	
	// Select 1st item, if one.
	if (ItemCount())
		Select(0);
}

/******************************************************************************
** Method:		AddSession()
**
** Description:	Adds the specified session to the view.
**
** Parameters:	nItem	The index of the item to add.
**				oIter	An SessionList iterator to the session.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CSessionListView::AddSession(int nItem, const CSessionPtr& pSession)
{
	int i = InsertItem(nItem, "");

	RefreshSession(i, pSession.Get());

	ItemPtr(i, pSession.Get());
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
** Method:		RefreshSessions()
**
** Description:	Refreshes the details of all sessions. This assumes that the
**				item positions haven't changed.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CSessionListView::RefreshSessions()
{
	int nCount = ItemCount();

	for (int i = 0; i < nCount; ++i)
		RefreshSession(i, static_cast<const CSession*>(ItemPtr(i)));
}

/******************************************************************************
** Method:		RefreshSession()
**
** Description:	Refreshes the details of a single session.
**
** Parameters:	nItem		The ListView index.
**				pSessions	The session.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CSessionListView::RefreshSession(int nItem, const CSession* pSession)
{
	ASSERT(pSession != nullptr);

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
	ItemText(nItem, DAY_COLUMN,  strStartDay);
	ItemText(nItem, DATE_COLUMN, strStartDate);
	ItemText(nItem, IN_COLUMN,   strStartTime);
	ItemText(nItem, OUT_COLUMN,  strEndTime);
	ItemText(nItem, LEN_COLUMN,  strLength);
	ItemText(nItem, TASK_COLUMN, strTask);
	ItemText(nItem, LOCN_COLUMN, strLocation);
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
