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

#ifdef _DEBUG
// For memory leak detection.
#define new DBGCRT_NEW
#endif

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

void CSessionList::Add(CSessionPtr& pNewSession)
{
	// Template shorthands.
	typedef iterator CIter;

	// For all sessions...
	for(CIter oIter = begin(); oIter != end(); ++oIter)
	{
		CSessionPtr pSession = *oIter;

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

void CSessionList::Remove(CSessionPtr& pSession)
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

int CSessionList::IndexOf(const CSessionPtr& pSession) const
{
	// Template shorthands.
	typedef const_iterator CIter;

	CIter oIter = std::find(begin(), end(), pSession);

	ASSERT(oIter != end());

	return std::distance(begin(), oIter);
}

/******************************************************************************
** Function:	Operator>>()
**
** Description:	Read a session list from a stream.
**
** Parameters:	rStream		The stream to read from.
**				oList		The list to read into.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void operator >>(CStream& rStream, CSessionList& oList)
{
	int16 iCount;
	
	// Read count.
	rStream >> iCount;

	// Read sessions.
	while(iCount--)
	{
		CSession* pSession = new CSession;

		rStream >> *pSession;
			
		oList.push_back(pSession);
	}
}

/******************************************************************************
** Function:	Operator<<()
**
** Description:	Write a session list to a stream.
**
** Parameters:	rStream		The stream to write to.
**				oList		The list to write.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void operator <<(CStream& rStream, const CSessionList& oList)
{
	// Template shorthands.
	typedef CSessionList::const_iterator CIter;

	int16 iCount = (int16) oList.size();
	
	// Write count.
	rStream << iCount;
	
	// Write sessions.
	for(CIter oIter = oList.begin(); oIter != oList.end(); ++oIter)
		rStream << *(*oIter);
}
