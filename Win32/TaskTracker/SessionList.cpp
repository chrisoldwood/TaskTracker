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
}

/******************************************************************************
** Method:		Add()
**
** Description:	Add a session to the list which is sorted by time.
**
** Parameters:	pNewSession		The new session.
**
** Returns:		The position of insertion.
**
*******************************************************************************
*/

uint CSessionList::Add(CSessionPtr& pNewSession)
{
	// Template shorthands.
	typedef iterator CIter;

	// For all sessions...
	for(CIter itSession = begin(); itSession != end(); ++itSession)
	{
		CSessionPtr pSession = *itSession;

		// New session starts earlier?
		if (pNewSession->Start() < pSession->Start())
		{
			CIter itPos = insert(itSession, pNewSession);

			return std::distance(begin(), itPos);
		}
    }
    
	// List is empty or session belongs at end.
	push_back(pNewSession);

	return size()-1;
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

void CSessionList::Remove(int nIndex)
{
	erase(begin()+nIndex);
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

	oList.reserve(iCount);

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
