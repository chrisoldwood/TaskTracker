/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		LOCNLIST.CPP
** COMPONENT:	The Application.
** DESCRIPTION:	CLocnList class definition.
**
*******************************************************************************
*/

#include "Common.hpp"
#include "LocnList.hpp"
#include <WCL/IInputStream.hpp>
#include <WCL/IOutputStream.hpp>
#include <algorithm>

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

void CLocnList::Add(const tchar* pszLocn)
{
	ASSERT(pszLocn != NULL);
	ASSERT(tstrlen(pszLocn) > 0);

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

void CLocnList::Remove(const tchar* pszLocn)
{
	ASSERT(pszLocn != NULL);
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
** Function:	Operator>>()
**
** Description:	Read a location list from a stream.
**
** Parameters:	rStream		The stream to read from.
**				oList		The list to read into.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void operator >>(WCL::IInputStream& rStream, CLocnList& oList)
{
	int16 iCount;
	
	// Read count.
	rStream >> iCount;

	// Read locations.
	while(iCount--)
	{
		CString strLocn;

		rStream >> strLocn;

		oList.push_back(strLocn);
	}
}

/******************************************************************************
** Function:	Operator<<()
**
** Description:	Write a location list to a stream.
**
** Parameters:	rStream		The stream to write to.
**				oList		The list to write.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void operator <<(WCL::IOutputStream& rStream, const CLocnList& oList)
{
	// Template shorthands.
	typedef CLocnList::const_iterator CIter;

	int16 iCount = (int16) oList.size();
	
	// Write count.
	rStream << iCount;
	
	// Write locations.
	for(CIter oIter = oList.begin(); oIter != oList.end(); ++oIter)
		rStream << (*oIter);
}
