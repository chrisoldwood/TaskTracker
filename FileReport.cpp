/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		FILEREPORT.CPP
** COMPONENT:	The Application.
** DESCRIPTION:	CFileReport class definition.
**
*******************************************************************************
*/

#include "Common.hpp"
#include "FileReport.hpp"
#include <stdio.h>
#include <WCL/FileException.hpp>
#include "TaskTracker.hpp"
#include <Core/AnsiWide.hpp>

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
	catch(const CFileException& rException)
	{
		// Notify user.
		App.m_AppWnd.AlertMsg(TXT("%s"), rException.twhat());
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
	m_File.Write(TXT("\r\n"), 2);
	
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

bool CFileReport::SendHeading(const tchar* pszText)
{
	size_t nChars = tstrlen(pszText);
	
	// Write string.
	m_File.Write(T2A(pszText), nChars);
	
	// Add CR/LF to string.
	if (!SendLineBreak())
		return false;
	
	// Underline string.
	for (size_t i = 0; i < nChars; i++)
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

bool CFileReport::SendText(const tchar* pszText)
{
	// Write string.
	m_File.Write(T2A(pszText), tstrlen(pszText));
	
	// Add CR/LF to string.
	if (!SendLineBreak())
		return false;
	
	return true;
}
