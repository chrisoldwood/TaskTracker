/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		LOCNLIST.CPP
** COMPONENT:	The Application.
** DESCRIPTION:	CLocnList class definition.
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

CLocnList::CLocnList()
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

CLocnList::~CLocnList()
{
	RemoveAll();
}

/******************************************************************************
** Method:		Add()
**
** Description:	Add a location to the list if not already in it.
**
** Parameters:	pszLocn		The location.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CLocnList::Add(const char* pszLocn)
{
	// String already in list?.
	CString* pString = Find(pszLocn);
	if (pString)
		return;

	// Make a copy.
	pString = new CString(pszLocn);
	ASSERT(pString);

	// And add it.
	CPtrList::AddToTail(pString);
}

/******************************************************************************
** Method:		Remove()
**
** Description:	Remove the location from the list.
**
** Parameters:	pszLocn		The location.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CLocnList::Remove(const char* pszLocn)
{
	// Find the string.
	CString* pString = Find(pszLocn);
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

void CLocnList::RemoveAll()
{
	CLocnListEnum	Enum(*this);
	CString*		pString;
	
	// Free all tasks.
	while((pString = Enum.Next()) != NULL)
		delete pString;

	CPtrList::RemoveAll();
}

/******************************************************************************
** Method:		Find()
**
** Description:	Finds a location in the list.
**
** Parameters:	pszLocn		The location.
**
** Returns:		The string if found or NULL.
**
*******************************************************************************
*/

CString* CLocnList::Find(const char* pszLocn) const
{
	CLocnListEnum	Enum(*this);
	CString*		pString;
	
	// For all tasks.
	while((pString = Enum.Next()) != NULL)
	{
		// Match?
		if (*pString == pszLocn)
			return pString;
	}

	return NULL;
}

/******************************************************************************
** Method:		Operator<<()
**
** Description:	Read a location list from a stream.
**
** Parameters:	rStream		The stream.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CLocnList::operator <<(CStream& rStream)
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
** Description:	Write a location list to a stream.
**
** Parameters:	rStream		The stream.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CLocnList::operator >>(CStream& rStream) const
{
	CLocnListEnum	Enum(*this);
	CString*		pTask;
	int16			iCount = (int16) Length();
	
	// Write count.
	rStream << iCount;
	
	// Write tasks.
	while((pTask = Enum.Next()) != NULL)
		rStream << *pTask;
}
