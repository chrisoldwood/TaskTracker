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
	CTextReport(CStream& rStream);
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
	CStream&	m_rStream;	// The stream to write data to.
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

#endif //TEXTREPORT_HPP