/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		PRINTREPORT.HPP
** COMPONENT:	The Application.
** DESCRIPTION:	The CPrintReport class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef PRINTREPORT_HPP
#define PRINTREPORT_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include "Report.hpp"
#include <WCL/PrinterDC.hpp>

/******************************************************************************
** 
** This is a CReport derived class, used to report to a printer.
**
*******************************************************************************
*/

class CPrintReport : public CReport
{
public:
	//
	// Constructors/Destructor.
	//
	CPrintReport(CPrinterDC& rPrinterDC);
	~CPrintReport();

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
	CPrinterDC&	m_rPrinterDC;
	CRect		m_rcArea;
	CFont		m_NormalFont;
	CFont		m_HeadingFont;
	int			m_iLineHeight;
	int			m_iPageOffset;
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

#endif //PRINTREPORT_HPP
