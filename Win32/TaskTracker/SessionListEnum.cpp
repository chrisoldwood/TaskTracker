/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		SESSENUM.CPP
** COMPONENT:	The Application.
** DESCRIPTION:	CSessionEnum class definition.
**
*******************************************************************************
*/

#include "AppHeaders.hpp"

/******************************************************************************
** Method:		Constructor.
**
** Description:	Construct an enumerator for the whole of the list.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

CSessionEnum::CSessionEnum(const CSessionList& List)
	: CPtrListIter(List)
{
	// Set start and end days to min/max.
	m_dtStart = CDateTime::Min();
	m_dtEnd   = CDateTime::Max();
}

/******************************************************************************
** Method:		Constructor.
**
** Description:	Construct an enumerator for the part of the list that falls
**				between the two dates.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

CSessionEnum::CSessionEnum(const CSessionList& List, const CDateTime dtStart, const CDateTime dtEnd)
	: CPtrListIter(List)
{
	// Copy start and end dates.
	m_dtStart = dtStart;
	m_dtEnd   = dtEnd;
}

/******************************************************************************
** Method:		Next()
**
** Description:	Returns the next session in the enumeration.
**
** Parameters:	None.
**
** Returns:		The session or NULL.
**
*******************************************************************************
*/

CSession* CSessionEnum::Next()
{
	// Get next session.
	CSession*	pSession = (CSession*) CPtrListIter::Next();

	// Skip sessions not in limits.
	while ( (pSession) 
		 && ( (pSession->Start() <  m_dtStart) 
		   || (pSession->Start() >= m_dtEnd) ) )
	{
		pSession = (CSession*) CPtrListIter::Next();
	}
	
	return pSession;
}
