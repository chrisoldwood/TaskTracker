////////////////////////////////////////////////////////////////////////////////
//! \author Chris Oldwood
//! \file   RENAMEDLG.HPP
//! \brief  The CRenameDlg class declaration.

// Check for previous inclusion
#ifndef RENAMEDLG_HPP
#define RENAMEDLG_HPP

////////////////////////////////////////////////////////////////////////////////
//!

class CRenameDlg : public CDialog
{
public:
	//! Default constructor.
	CRenameDlg();
	
	//
	// Members.
	//
	CString	m_strOldTask;		//!< Task to rename.
	CString	m_strNewTask;		//!< New Task name.

private:
	//
	// Members.
	//
	
	//
	// Controls.
	//
	CTaskCombo	m_cbOldTask;	//!< Task to rename combo.
	CTaskCombo	m_cbNewTask;	//!< New Task name combo.

	//
	// Message handlers.
	//

	//! Dialog initialisation handler.
	virtual void OnInitDialog();

	//! OK button handler.
	virtual bool OnOk();
};

#endif // RENAMEDLG_HPP
