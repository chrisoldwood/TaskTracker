/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		ADDSNDLG.HPP
** COMPONENT:	The Application.
** DESCRIPTION:	The CAddSessionDlg class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef ADDSNDLG_HPP
#define ADDSNDLG_HPP

/******************************************************************************
** 
** This dialog is used when the user adds a new session.
**
*******************************************************************************
*/

class CAddSessionDlg : public CDialog
{
public:
	//
	// Constructors/Destructor.
	//
	CAddSessionDlg();
	
	//
	// Members.
	//
	CDateTime	m_dtInDateTime;
	CDateTime	m_dtOutDateTime;
	CString		m_strTask;
	CString		m_strLocn;

protected:
	//
	// Members.
	//
	CDateTimePicker	m_dtpInDateTime;
	CDateTimePicker	m_dtpOutDateTime;
	CTaskCombo		m_cbTask;
	CLocnCombo		m_cbLocn;
	
	//
	// Message processors.
	//
	virtual void OnInitDialog();
	virtual bool OnOk();
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

#endif //ADDSNDLG_HPP
