/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		SESSION.CPP
** COMPONENT:	The Application.
** DESCRIPTION:	CSession class definition.
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

CSession::CSession()
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

CSession::~CSession()
{
}

/******************************************************************************
** Method:		Length()
**
** Description:	Get the length of the session in minutes.
**
** Parameters:	None.
**
** Returns:		The length or 0 if the start time is later then finish time.
**
*******************************************************************************
*/

ulong CSession::Length() const
{
	// Start later than finish?
	if (m_dtStart > m_dtFinish)
		return 0;

	// Return difference.
	return (m_dtFinish - m_dtStart).Mins();
}

/******************************************************************************
** Function:	Operator>>()
**
** Description:	Read a session from a stream.
**
** Parameters:	rStream		The stream to read from.
**				oList		The session to read into.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void operator >>(CStream& rStream, CSession& oSession)
{
	rStream >> oSession.m_dtStart;
	rStream >> oSession.m_dtFinish;
	rStream >> oSession.m_strTask;
	rStream >> oSession.m_strLocn;
}

/******************************************************************************
** Function:	Operator<<()
**
** Description:	Write a session to a stream.
**
** Parameters:	rStream		The stream to write to.
**				oSession	The session to write.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void operator <<(CStream& rStream, const CSession& oSession)
{
	rStream << oSession.m_dtStart;
	rStream << oSession.m_dtFinish;
	rStream << oSession.m_strTask;
	rStream << oSession.m_strLocn;
}
