/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		VIEWREPORTDLG.CPP
** COMPONENT:	The Application.
** DESCRIPTION:	CViewReportDlg class definition.
**
*******************************************************************************
*/

#include "AppHeaders.hpp"
#include "ViewReportDlg.hpp"

/******************************************************************************
** Method:		Default constructor.
**
** Description:	.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

CViewReportDlg::CViewReportDlg(CMemStream& rTxtStream) 
	: CDialog(IDD_VIEW_REPORT)
	, m_rTxtStream(rTxtStream)
	, m_oFont(ANSI_FIXED_FONT)
{
	DEFINE_CTRL_TABLE
		CTRL(IDC_REPORT, &m_ebReport)
	END_CTRL_TABLE

	DEFINE_GRAVITY_TABLE
		CTRLGRAV(IDC_REPORT, LEFT_EDGE,  TOP_EDGE,    RIGHT_EDGE, BOTTOM_EDGE)
		CTRLGRAV(IDC_FONT,   LEFT_EDGE,  BOTTOM_EDGE, LEFT_EDGE,  BOTTOM_EDGE)
		CTRLGRAV(IDOK,       RIGHT_EDGE, BOTTOM_EDGE, RIGHT_EDGE, BOTTOM_EDGE)
	END_GRAVITY_TABLE

	DEFINE_CTRLMSG_TABLE
		CMD_CTRLMSG(IDC_FONT, BN_CLICKED, OnFontClicked)
	END_CTRLMSG_TABLE
}

/******************************************************************************
** Method:		OnInitDialog()
**
** Description:	Initialise the dialog.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CViewReportDlg::OnInitDialog()
{
	CString	strReport;

	m_rTxtStream.Open();

	// Find the length of the text.
	ulong lLen = m_rTxtStream.Seek(0, FILE_END);
	m_rTxtStream.Seek(0, FILE_BEGIN);

	// Allocate a buffer big enough.
	strReport.BufferSize(lLen);

	// Copy the text to the string buffer.
	m_rTxtStream.Read((void*)(const char*)strReport, lLen);
	m_rTxtStream.Close();

	// Copy to the control.
	m_ebReport.Text(strReport);
	m_ebReport.Font(m_oFont);

	// Resize dialog to previous size.
	if (!App.m_rcReportDlg.Empty())
		Move(App.m_rcReportDlg);

	// Use a custom font, if configured.
	if (CFont::Parse(App.m_strRptDlgFont, m_oFont))
		m_ebReport.Font(m_oFont);
}

/******************************************************************************
** Method:		OnDestroy()
**
** Description:	The window is being destroyed.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CViewReportDlg::OnDestroy()
{
	// Remember its position.
	App.m_rcReportDlg = Placement();
}

/******************************************************************************
** Method:		OnFontClicked()
**
** Description:	Select a different font for the report.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CViewReportDlg::OnFontClicked()
{
	// Show common dialog.
	if (m_oFont.Select(*this))
	{
		// Update control.
		m_ebReport.Font(m_oFont);

		// Remeber font.
		App.m_strRptDlgFont = m_oFont.Format(CLogFont::FMT_SHORT);
	}
}
