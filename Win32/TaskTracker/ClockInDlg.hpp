/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		CLKINDLG.HPP
** COMPONENT:	The Application.
** DESCRIPTION:	The CClockInDlg class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef CLKINDLG_HPP
#define CLKINDLG_HPP

/******************************************************************************
** 
** This dialog is used when the user clocks in.
**
*******************************************************************************
*/

class CClockInDlg : public CDialog
{
public:
	//
	// Constructors/Destructor.
	//
	CClockInDlg();
	
	//
	// Members.
	//
	CDateTime	m_dtDateTime;
	CString		m_strTask;
	CString		m_strLocn;

protected:
	//
	// Members.
	//
	CRadioBtn		m_rbNow;
	CRadioBtn		m_rbOnAt;
	CDateTimePicker	m_dtpDateTime;
	CTaskCombo		m_cbTask;
	CLocnCombo		m_cbLocn;
	
	//
	// Message processors.
	//
	virtual void OnInitDialog();
	virtual bool OnOk();

	//
	// Control message processors.
	//
	void OnNow();
	void OnOnAt();
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

#endif //CLKINDLG_HPP
