/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		TASKLIST.CPP
** COMPONENT:	The Application.
** DESCRIPTION:	CTaskList class definition.
**
*******************************************************************************
*/

#include "Common.hpp"
#include "TaskList.hpp"
#include <algorithm>
#include <WCL/IInputStream.hpp>
#include <WCL/IOutputStream.hpp>

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

CTaskList::CTaskList()
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

CTaskList::~CTaskList()
{
	RemoveAll();
}

/******************************************************************************
** Method:		Add()
**
** Description:	Add a task to the list if not already in it.
**
** Parameters:	pszTask		The task.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CTaskList::Add(const char* pszTask)
{
	ASSERT(pszTask != NULL);
	ASSERT(strlen(pszTask) > 0);

	// Ignore, if string already in list.
	if (std::find(begin(), end(), pszTask) != end())
		return;

	push_back(pszTask);
}

/******************************************************************************
** Method:		Remove()
**
** Description:	Remove the task from the list.
**
** Parameters:	pszTask		The task.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CTaskList::Remove(const char* pszTask)
{
	ASSERT(pszTask != NULL);
	ASSERT(std::find(begin(), end(), pszTask) != end());

	remove(pszTask);
}

/******************************************************************************
** Method:		RemoveAll()
**
** Description:	Remove all tasks from the list.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CTaskList::RemoveAll()
{
	clear();
}

/******************************************************************************
** Function:	Operator>>()
**
** Description:	Read a task list from a stream.
**
** Parameters:	rStream		The stream to read from.
**				oList		The list to read into.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void operator >>(WCL::IInputStream& rStream, CTaskList& oList)
{
	int16 iCount;
	
	// Read count.
	rStream >> iCount;

	// Read tasks.
	while(iCount--)
	{
		CString strTask;

		rStream >> strTask;

		oList.push_back(strTask);
	}
}

/******************************************************************************
** Function:	Operator<<()
**
** Description:	Write a task list to a stream.
**
** Parameters:	rStream		The stream to write to.
**				oList		The list to write.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void operator <<(WCL::IOutputStream& rStream, const CTaskList& oList)
{
	// Template shorthands.
	typedef CTaskList::const_iterator CIter;

	int16 iCount = (int16) oList.size();
	
	// Write count.
	rStream << iCount;
	
	// Write tasks.
	for(CIter oIter = oList.begin(); oIter != oList.end(); ++oIter)
		rStream << (*oIter);
}
