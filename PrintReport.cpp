/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		PRINTREPORT.CPP
** COMPONENT:	The Application.
** DESCRIPTION:	CPrintReport class definition.
**
*******************************************************************************
*/

#include "AppHeaders.hpp"

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

CPrintReport::CPrintReport(CPrinterDC& rPrinterDC)
	: m_rPrinterDC(rPrinterDC)
	, m_iLineHeight(0)
	, m_iPageOffset(0)
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

CPrintReport::~CPrintReport()
{
}

/******************************************************************************
** Method:		Init()
**
** Description:	Initialise the printer by creating a DC.
**
** Parameters:	None.
**
** Returns:		true or false.
**
*******************************************************************************
*/

bool CPrintReport::Init()
{
	HFONT hFont = nullptr;

	// Initialise page settings.
	m_rcArea      = m_rPrinterDC.PrintableArea();
	m_iPageOffset = m_rcArea.top;
	
	// Create body text font.
	hFont = CreateFont(0, 0, 0, 0, FW_DONTCARE, 0, 0, 0, ANSI_CHARSET,
						OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
						DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
						nullptr);
	
	m_NormalFont.Create(hFont, true);

	// Create heading text font.
	hFont = CreateFont(0, 0, 0, 0, FW_DONTCARE, 0, TRUE, 0, ANSI_CHARSET,
						OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
						DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
						nullptr);
	
	m_HeadingFont.Create(hFont, true);

	CSize	m_NormalSize;
	CSize	m_HeadingSize;

	// Get font metrics.
	m_rPrinterDC.Select(m_HeadingFont);
	m_HeadingSize = m_rPrinterDC.TextExtents("Ly");
	
	m_rPrinterDC.Select(m_NormalFont);
	m_NormalSize = m_rPrinterDC.TextExtents("Ly");
	
	// Get line height.
	if (m_HeadingSize.cy > m_NormalSize.cy)
		m_iLineHeight = m_HeadingSize.cy;
	else
		m_iLineHeight = m_NormalSize.cy;
	
	// Start the document.
	if (!m_rPrinterDC.Start("TaskTracker Report"))
	{
		App.m_AppWnd.AlertMsg("Failed to start the document");
		return false;
	}
	
	// Start the first page.
	if (!m_rPrinterDC.StartPage())
	{
		App.m_AppWnd.AlertMsg("Failed to start a new page");
		return false;
	}
	
	return true;
}

/******************************************************************************
** Method:		Term()
**
** Description:	Cleanup the printer DC.
**
** Parameters:	None.
**
** Returns:		true or false.
**
*******************************************************************************
*/

bool CPrintReport::Term()
{
	// Finish printing.
	m_rPrinterDC.EndPage();
	m_rPrinterDC.End();
	
	return true;
}

/******************************************************************************
** Method:		SendLineBreak()
**
** Description:	Outputs a line break. If this overflows the current page, then
**				start a new one.
**
** Parameters:	None.
**
** Returns:		true or false.
**
*******************************************************************************
*/

bool CPrintReport::SendLineBreak()
{
	// Move down to next line.
	m_iPageOffset += m_iLineHeight;

	// Outside current page?
	if ((m_iPageOffset+m_iLineHeight) > m_rcArea.bottom)
	{
		// Finish the current page.
		if (!m_rPrinterDC.EndPage())
		{
			App.m_AppWnd.AlertMsg("Failed to send the last page");
			return false;
		}
			
		// Start a new page.
		if (!m_rPrinterDC.StartPage())
		{
			App.m_AppWnd.AlertMsg("Failed to start a new page");
			return false;
		}
		
		// Reset cursor to page top.
		m_iPageOffset = m_rcArea.top;
	}

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

bool CPrintReport::SendHeading(const char* pszText)
{
	// Select the font
	m_rPrinterDC.Select(m_HeadingFont);

	// Print the heading
	m_rPrinterDC.TextOut(0, m_iPageOffset, pszText);

	return SendLineBreak();
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

bool CPrintReport::SendText(const char* pszText)
{
	// Select the font.
	m_rPrinterDC.Select(m_NormalFont);

	// Print the heading.
	m_rPrinterDC.TextOut(0, m_iPageOffset, pszText);
	
	return SendLineBreak();
}
