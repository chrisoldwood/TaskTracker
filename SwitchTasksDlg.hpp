/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		SWTSKDLG.HPP
** COMPONENT:	The Application.
** DESCRIPTION:	The CSwitchTasksDlg class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef SWTSKDLG_HPP
#define SWTSKDLG_HPP

/******************************************************************************
** 
** This dialog is used when the user switches tasks.
**
*******************************************************************************
*/

class CSwitchTasksDlg : public CDialog
{
public:
	//
	// Constructors/Destructor.
	//
	CSwitchTasksDlg();
	
	//
	// Members.
	//
	CString		m_strPrevTask;
	CString		m_strNextTask;

protected:
	//
	// Members.
	//
	CTaskCombo	m_cbPrevTask;
	CTaskCombo	m_cbNextTask;
	
	//
	// Message processors.
	//
	virtual void OnInitDialog();
	virtual bool OnOk();

	//
	// Control message processors.
	//
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

#endif //SWTSKDLG_HPP
