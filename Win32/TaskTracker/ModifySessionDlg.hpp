/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		MODSNDLG.HPP
** COMPONENT:	The Application.
** DESCRIPTION:	The CModifySessionDlg class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef MODSNDLG_HPP
#define MODSNDLG_HPP

/******************************************************************************
** 
** This dialog is used when the user adds a new session.
**
*******************************************************************************
*/

class CModifySessionDlg : public CDialog
{
public:
	//
	// Constructors/Destructor.
	//
	CModifySessionDlg();
	
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

#endif //MODSNDLG_HPP
