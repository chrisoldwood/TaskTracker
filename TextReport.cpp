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
	m_rStream.Write("\0", 1);

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
	m_rStream.Write("\r\n", 2);
	
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

bool CTextReport::SendHeading(const char* pszText)
{
	int iLen = strlen(pszText);
	
	// Write string.
	m_rStream.Write(pszText, iLen);
	
	// Add CR/LF to string.
	SendLineBreak();
	
	// Underline string.
	for(int i = 0; i < iLen; i++)
		m_rStream.Write("=", 1);
	
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

bool CTextReport::SendText(const char* pszText)
{
	// Write string.
	m_rStream.Write(pszText, strlen(pszText));
	
	// Add CR/LF to string.
	SendLineBreak();
	
	return true;
}
