/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		FILEREPORTDLG.HPP
** COMPONENT:	The Application.
** DESCRIPTION:	The CFileReportDlg class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef FILEREPORTDLG_HPP
#define FILEREPORTDLG_HPP

/******************************************************************************
** 
** This dialog is used when the user reports data to a file.
**
*******************************************************************************
*/

class CFileReportDlg : public CDialog
{
public:
	//
	// Constructors/Destructor.
	//
	CFileReportDlg();
	
	//
	// Members.
	//
	CPath		m_strFileName;
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
	CPathEditBox	m_ebFileName;
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
	void OnBrowse();
	void OnPeriodChange();
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

#endif //FILEREPORTDLG_HPP
