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
** Returns:		The position of insertion.
**
*******************************************************************************
*/

int CSessionList::Add(CSession* pNewSession)
{
	CSessionEnum	Enum(*this);
	CSession*		pSession;
	
	// For all sessions.
	while((pSession = Enum.Next()) != NULL)
	{
		/* New session starts earlier? */
		if (pNewSession->Start() < pSession->Start())
			return CPtrList::InsertBefore(pSession, pNewSession);
    }
    
	// List is empty or session belongs at end.
	return CPtrList::AddToTail(pNewSession);
}

/******************************************************************************
** Method:		Remove()
**
** Description:	Remove a session from the list.
**
** Parameters:	pSession	The session to remove.
**
** Returns:		The position of deletion.
**
*******************************************************************************
*/

int CSessionList::Remove(CSession* pSession)
{
	return CPtrList::Remove(pSession);
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
	CSessionEnum	Enum(*this);
	CSession*		pSession;
	
	// Free all sessions.
	while((pSession = Enum.Next()) != NULL)
		delete pSession;

	CPtrList::RemoveAll();
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
	CSession*	pSession;
	int16 		iCount;
	
	// Read count.
	rStream >> iCount;

	// Read sessions.
	while(iCount--)
	{
		pSession = new CSession;
		ASSERT(pSession != NULL);

		*pSession << rStream;
			
		Add(pSession);
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
	CSessionEnum	Enum(*this);
	CSession*		pSession;
	int16			iCount = (int16) Length();
	
	// Write count.
	rStream << iCount;
	
	// Write sessions.
	while((pSession = Enum.Next()) != NULL)
		*pSession >> rStream;
}
