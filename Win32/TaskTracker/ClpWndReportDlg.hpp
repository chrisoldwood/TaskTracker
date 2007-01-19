/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		CLPWNDREPORTDLG.HPP
** COMPONENT:	The Application.
** DESCRIPTION:	The CClpWndReportDlg class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef CLPWNDREPORTDLG_HPP
#define CLPWNDREPORTDLG_HPP

/******************************************************************************
** 
** This dialog is used when the user reports data to a file.
**
*******************************************************************************
*/

class CClpWndReportDlg : public CDialog
{
public:
	//
	// Constructors/Destructor.
	//
	CClpWndReportDlg();
	
	//
	// Members.
	//
	CString		m_strTitle;
	Grouping	m_eGrouping;
	CDate		m_FromDate;
	CDate		m_ToDate;
	bool		m_bShowSessions;
	bool		m_bShowIfEmpty;
	bool		m_bShowTotal;

protected:
	//
	// Controls.
	//
	CRadioBtn		m_rbGroupByNone;
	CRadioBtn		m_rbGroupByWeek;
	CRadioBtn		m_rbGroupByMonth;
	CRadioBtn		m_rbGroupByTask;
	CComboBox		m_cbPeriod;
	CDateTimePicker	m_dtpFromDate;
	CDateTimePicker	m_dtpToDate;
	CCheckBox		m_ckShowSessions;
	CCheckBox		m_ckShowIfEmpty;
	CCheckBox		m_ckShowTotal;
	CCheckBox		m_ckRemember;
	
	//
	// Message processors.
	//
	virtual void OnInitDialog();
	virtual bool OnOk();

	//
	// Control message processors.
	//
	void OnPeriodChange();
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

#endif //CLPWNDREPORTDLG_HPP
