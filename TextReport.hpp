/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		TEXTREPORT.HPP
** COMPONENT:	The Application.
** DESCRIPTION:	The CTextReport class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef TEXTREPORT_HPP
#define TEXTREPORT_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include "Report.hpp"

/******************************************************************************
** 
** This is a CReport derived class, used to report to a text stream.
**
*******************************************************************************
*/

class CTextReport : public CReport
{
public:
	//
	// Constructors/Destructor.
	//
	CTextReport(WCL::IOutputStream& rStream);
	~CTextReport();

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
	WCL::IOutputStream&	m_rStream;	// The stream to write data to.
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

#endif //TEXTREPORT_HPP
