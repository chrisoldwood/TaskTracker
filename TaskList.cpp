/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		TASKLIST.CPP
** COMPONENT:	The Application.
** DESCRIPTION:	CTaskList class definition.
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
	// String already in list?.
	CString* pString = Find(pszTask);
	if (pString)
		return;

	// Make a copy.
	pString = new CString(pszTask);
	ASSERT(pString);

	// And add it.
	CPtrList::AddToTail(pString);
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
	// Find the string.
	CString* pString = Find(pszTask);
	ASSERT(pString);

	// And delete.
	CPtrList::Remove(pString);
	delete pString;
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
	CTaskListEnum	Enum(*this);
	CString*		pString;
	
	// Free all tasks.
	while((pString = Enum.Next()) != NULL)
		delete pString;

	CPtrList::RemoveAll();
}

/******************************************************************************
** Method:		Find()
**
** Description:	Finds a task in the list.
**
** Parameters:	pszTask		The task.
**
** Returns:		The string if found or NULL.
**
*******************************************************************************
*/

CString* CTaskList::Find(const char* pszTask) const
{
	CTaskListEnum	Enum(*this);
	CString*		pString;
	
	// For all tasks.
	while((pString = Enum.Next()) != NULL)
	{
		// Match?
		if (*pString == pszTask)
			return pString;
	}

	return NULL;
}

/******************************************************************************
** Method:		Operator<<()
**
** Description:	Read a task list from a stream.
**
** Parameters:	rStream		The stream.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CTaskList::operator <<(CStream& rStream)
{
	CString*	pTask;
	int16 		iCount;
	
	// Read count.
	rStream >> iCount;

	// Read tasks.
	while(iCount--)
	{
		pTask = new CString;
		ASSERT(pTask != NULL);

		rStream >> *pTask;

		CPtrList::AddToTail(pTask);
	}
}

/******************************************************************************
** Method:		Operator>>()
**
** Description:	Write a task list to a stream.
**
** Parameters:	rStream		The stream.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CTaskList::operator >>(CStream& rStream) const
{
	CTaskListEnum	Enum(*this);
	CString*		pTask;
	int16			iCount = (int16) Length();
	
	// Write count.
	rStream << iCount;
	
	// Write tasks.
	while((pTask = Enum.Next()) != NULL)
		rStream << *pTask;
}
