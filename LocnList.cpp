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
	// Ignore, if string already in list.
	if (std::find(begin(), end(), pszLocn) != end())
		return;

	push_back(pszLocn);
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
	ASSERT(std::find(begin(), end(), pszLocn) != end());

	remove(pszLocn);
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
	clear();
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
	int16 iCount;
	
	// Read count.
	rStream >> iCount;

	// Read locations.
	while(iCount--)
	{
		CString strLocn;

		rStream >> strLocn;

		push_back(strLocn);
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
	// Template shorthands.
	typedef const_iterator CIter;

	int16 iCount = (int16) size();
	
	// Write count.
	rStream << iCount;
	
	// Write locations.
	for(CIter oIter = begin(); oIter != end(); ++oIter)
		rStream << (*oIter);
}
