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
** Method:		Operator<<()
**
** Description:	Read a session from a stream.
**
** Parameters:	rStream		The stream.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CSession::operator <<(CStream& rStream)
{
	rStream >> m_dtStart;
	rStream >> m_dtFinish;
	rStream >> m_strTask;
	rStream >> m_strLocn;
}

/******************************************************************************
** Method:		Operator>>()
**
** Description:	Write a session to a stream.
**
** Parameters:	rStream		The stream.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CSession::operator >>(CStream& rStream) const
{
	rStream << m_dtStart;
	rStream << m_dtFinish;
	rStream << m_strTask;
	rStream << m_strLocn;
}
