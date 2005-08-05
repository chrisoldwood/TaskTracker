/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		SESSLIST.CPP
** COMPONENT:	The Application.
** DESCRIPTION:	CSessionList class definition.
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

CSessionList::CSessionList()
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

CSessionList::~CSessionList()
{
	RemoveAll();
}

/******************************************************************************
** Method:		Add()
**
** Description:	Add a session to the list sorted by time.
**
** Parameters:	pNewSession		The new session.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CSessionList::Add(CSession* pNewSession)
{
	// Template shorthands.
	typedef iterator CIter;

	// For all sessions...
	for(CIter oIter = begin(); oIter != end(); ++oIter)
	{
		CSession* pSession = *oIter;

		// New session starts earlier?
		if (pNewSession->Start() < pSession->Start())
		{
			insert(oIter, pNewSession);
			return;
		}
    }
    
	// List is empty or session belongs at end.
	push_back(pNewSession);
}

/******************************************************************************
** Method:		Remove()
**
** Description:	Remove a session from the list.
**
** Parameters:	pSession	The session to remove.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CSessionList::Remove(CSession* pSession)
{
	remove(pSession);
}

/******************************************************************************
** Method:		RemoveAll()
**
** Description:	Remove all sessions from the list.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CSessionList::RemoveAll()
{
	// Template shorthands.
	typedef const_iterator CIter;

	// Free all sessions.
	for(CIter oIter = begin(); oIter != end(); ++oIter)
		delete *oIter;

	clear();
}

/******************************************************************************
** Method:		IndexOf()
**
** Description:	Gets the index of the session in the list.
**
** Parameters:	pSession	The session to find.
**
** Returns:		The position.
**
*******************************************************************************
*/

int CSessionList::IndexOf(const CSession* pSession) const
{
	// Template shorthands.
	typedef const_iterator CIter;

	CIter oIter = std::find(begin(), end(), pSession);

	ASSERT(oIter != end());

	return std::distance(begin(), oIter);
}

/******************************************************************************
** Method:		Operator<<()
**
** Description:	Read a session list from a stream.
**
** Parameters:	rStream		The stream.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CSessionList::operator <<(CStream& rStream)
{
	int16 iCount;
	
	// Read count.
	rStream >> iCount;

	// Read sessions.
	while(iCount--)
	{
		CSession* pSession = new CSession;

		*pSession << rStream;
			
		push_back(pSession);
	}
}

/******************************************************************************
** Method:		Operator>>()
**
** Description:	Write a session list to a stream.
**
** Parameters:	rStream		The stream.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CSessionList::operator >>(CStream& rStream) const
{
	// Template shorthands.
	typedef const_iterator CIter;

	int16 iCount = (int16) size();
	
	// Write count.
	rStream << iCount;
	
	// Write sessions.
	for(CIter oIter = begin(); oIter != end(); ++oIter)
		*(*oIter) >> rStream;
}
