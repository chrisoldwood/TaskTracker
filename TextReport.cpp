/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		TEXTREPORT.CPP
** COMPONENT:	The Application.
** DESCRIPTION:	CTextReport class definition.
**
*******************************************************************************
*/

#include "Common.hpp"
#include "TextReport.hpp"
#include <WCL/IOutputStream.hpp>

/******************************************************************************
** Method:		Constructor.
**
** Description:	.
**
** Parameters:	rStream		The stream to write to.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

CTextReport::CTextReport(WCL::IOutputStream& rStream)
	: m_rStream(rStream)
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

CTextReport::~CTextReport()
{
}

/******************************************************************************
** Method:		Init()
**
** Description:	Open the stream.
**
** Parameters:	None.
**
** Returns:		true or false.
**
*******************************************************************************
*/

bool CTextReport::Init()
{
	return true;
}

/******************************************************************************
** Method:		Term()
**
** Description:	Close the stream.
**
** Parameters:	None.
**
** Returns:		true or false.
**
*******************************************************************************
*/

bool CTextReport::Term()
{
	// Write terminator.
	m_rStream.Write(TXT("\0"), Core::NumBytes<tchar>(1));

	return true;
}

/******************************************************************************
** Method:		SendLineBreak()
**
** Description:	Outputs a line break.
**
** Parameters:	None.
**
** Returns:		true or false.
**
*******************************************************************************
*/

bool CTextReport::SendLineBreak()
{
	// Write CR/LF.
	m_rStream.Write(TXT("\r\n"), Core::NumBytes<tchar>(2));
	
	return true;
}

/******************************************************************************
** Method:		SendHeading()
**
** Description:	Outputs the heading for a period.
**
** Parameters:	pszText		The string to output.
**
** Returns:		true or false.
**
*******************************************************************************
*/

bool CTextReport::SendHeading(const tchar* pszText)
{
	size_t nChars = tstrlen(pszText);
	
	// Write string.
	m_rStream.Write(pszText, Core::NumBytes<tchar>(nChars));
	
	// Add CR/LF to string.
	SendLineBreak();
	
	// Underline string.
	for(size_t i = 0; i < nChars; i++)
		m_rStream.Write(TXT("="), Core::NumBytes<tchar>(1));
	
	// Add CR/LF to underline.
	SendLineBreak();

	return true;
}

/******************************************************************************
** Method:		SendText()
**
** Description:	Outputs a line of text.
**
** Parameters:	pszText		The string to output.
**
** Returns:		true or false.
**
*******************************************************************************
*/

bool CTextReport::SendText(const tchar* pszText)
{
	size_t nChars = tstrlen(pszText);

	// Write string.
	m_rStream.Write(pszText, Core::NumBytes<tchar>(nChars));
	
	// Add CR/LF to string.
	SendLineBreak();
	
	return true;
}
