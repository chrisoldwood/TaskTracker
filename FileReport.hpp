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
