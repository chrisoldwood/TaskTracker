/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		REPORT.HPP
** COMPONENT:	The Application.
** DESCRIPTION:	The CReport class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef REPORT_HPP
#define REPORT_HPP

/******************************************************************************
** 
** This is an abstract class used for the device to report to.
**
*******************************************************************************
*/

class CReport
{
public:
	//
	// Constructors/Destructor.
	//
	virtual bool Init() = 0;
	virtual bool Term() = 0;
	
	//
	// Methods.
	//
	virtual bool SendLineBreak() = 0;
	virtual bool SendHeading(const char* pszText) = 0;
	virtual bool SendText(const char* pszText) = 0;

protected:
	//
	// Members.
	//
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

#endif //REPORT_HPP
