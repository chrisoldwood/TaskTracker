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
{
	DEFINE_CTRL_TABLE
		CTRL(IDC_REPORT, &m_ebReport)
	END_CTRL_TABLE

	DEFINE_GRAVITY_TABLE
		CTRLGRAV(IDC_REPORT, LEFT_EDGE,  TOP_EDGE,    RIGHT_EDGE, BOTTOM_EDGE)
		CTRLGRAV(IDOK,       RIGHT_EDGE, BOTTOM_EDGE, RIGHT_EDGE, BOTTOM_EDGE)
	END_GRAVITY_TABLE
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
	m_ebReport.Font(CFont(ANSI_FIXED_FONT));

	// Resize dialog to previous size.
	if (!App.m_rcReportDlg.Empty())
		Move(App.m_rcReportDlg);
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
