/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		FILEREPORT.HPP
** COMPONENT:	The Application.
** DESCRIPTION:	The CFileReport class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef FILEREPORT_HPP
#define FILEREPORT_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include "Report.hpp"
#include <WCL/Path.hpp>
#include <WCL/File.hpp>

/******************************************************************************
** 
** This is a CReport derived class, used to report to a file.
**
*******************************************************************************
*/

class CFileReport : public CReport
{
public:
	//
	// Constructors/Destructor.
	//
	CFileReport(const CPath& rFileName);
	~CFileReport();

	virtual bool Init();
	virtual bool Term();
	
	//
	// Methods.
	//
	virtual bool SendLineBreak();
	virtual bool SendHeading(const char* pszText);
	virtual bool SendText(const char* pszText);

protected:
	//
	// Members.
	//
	CPath	m_Path;
	CFile	m_File;
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

#endif //FILEREPORT_HPP
