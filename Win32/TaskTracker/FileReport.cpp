/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		FILEREPORT.CPP
** COMPONENT:	The Application.
** DESCRIPTION:	CFileReport class definition.
**
*******************************************************************************
*/

#include "AppHeaders.hpp"
#include <stdio.h>

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

CFileReport::CFileReport(const CPath& rFileName)
{
	// Initialse filename.
	m_Path = rFileName;
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

CFileReport::~CFileReport()
{
}

/******************************************************************************
** Method:		Init()
**
** Description:	Initialise the file by opening it.
**
** Parameters:	None.
**
** Returns:		true or false.
**
*******************************************************************************
*/

bool CFileReport::Init()
{
	try
	{
		m_File.Create(m_Path);
	}
	catch(CFileException& rException)
	{
		// Notify user.
		App.m_AppWnd.AlertMsg(rException.ErrorText());
		return false;
	}

	return true;
}

/******************************************************************************
** Method:		Term()
**
** Description:	Close the file.
**
** Parameters:	None.
**
** Returns:		true or false.
**
*******************************************************************************
*/

bool CFileReport::Term()
{
	m_File.Close();
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

bool CFileReport::SendLineBreak()
{
	// Write CR/LF.
	m_File.Write("\r\n", 2);
	
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

bool CFileReport::SendHeading(const char* pszText)
{
	int iLen = strlen(pszText);
	
	// Write string.
	m_File.Write(pszText, iLen);
	
	// Add CR/LF to string.
	if (!SendLineBreak())
		return false;
	
	// Underline string.
	for(int i = 0; i < iLen; i++)
		m_File.Write("=", 1);
	
	// Add CR/LF to underline.
	if (!SendLineBreak())
		return false;

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

bool CFileReport::SendText(const char* pszText)
{
	// Write string.
	m_File.Write(pszText, strlen(pszText));
	
	// Add CR/LF to string.
	if (!SendLineBreak())
		return false;
	
	return true;
}
