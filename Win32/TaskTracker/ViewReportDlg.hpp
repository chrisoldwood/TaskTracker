/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		VIEWREPORTDLG.HPP
** COMPONENT:	The Application.
** DESCRIPTION:	The CViewReportDlg class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef VIEWREPORTDLG_HPP
#define VIEWREPORTDLG_HPP

/******************************************************************************
** 
** This is the dialog that display the results of a report.
**
*******************************************************************************
*/

class CViewReportDlg : public CDialog
{
public:
	//
	// Constructors/Destructor.
	//
	CViewReportDlg(CMemStream& rTxtStream);

protected:
	//
	// Members.
	//
	CMemStream&	m_rTxtStream;
	CEditBox	m_ebReport;
	CFont		m_oFont;
	
	//
	// Message processors.
	//
	virtual void OnInitDialog();
	virtual void OnDestroy();

	//
	// Control message processors.
	//
	void OnFontClicked();
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

#endif //VIEWREPORTDLG_HPP
