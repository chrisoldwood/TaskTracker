/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		PRINTREPORTDLG.HPP
** COMPONENT:	The Application.
** DESCRIPTION:	The CPrintReportDlg class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef PRINTREPORTDLG_HPP
#define PRINTREPORTDLG_HPP

/******************************************************************************
** 
** This dialog is used when the user reports to a printer.
**
*******************************************************************************
*/

class CPrintReportDlg : public CDialog
{
public:
	//
	// Constructors/Destructor.
	//
	CPrintReportDlg();
	
	//
	// Members.
	//
	Grouping	m_eGrouping;
	CDate		m_FromDate;
	CDate		m_ToDate;

protected:
	//
	// Members.
	//
	CLabel			m_txtPrinter;
	CLabel			m_txtPort;
	CRadioBtn		m_rbGroupByNone;
	CRadioBtn		m_rbGroupByWeek;
	CRadioBtn		m_rbGroupByMonth;
	CRadioBtn		m_rbGroupByTask;
	CComboBox		m_cbPeriod;
	CDateTimePicker	m_dtpFromDate;
	CDateTimePicker	m_dtpToDate;
	
	//
	// Message processors.
	//
	virtual void OnInitDialog();
	virtual bool OnOk();

	//
	// Control message processors.
	//
	void OnSelect();
	void OnPeriodChange();
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

#endif //PRINTREPORTDLG_HPP
