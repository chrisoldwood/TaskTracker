////////////////////////////////////////////////////////////////////////////////
//! \author Chris Oldwood
//! \file   OPTIONSDLG.HPP
//! \brief  The COptionsDlg class declaration.

// Check for previous inclusion
#ifndef OPTIONSDLG_HPP
#define OPTIONSDLG_HPP

////////////////////////////////////////////////////////////////////////////////
//! The options dialog. This dialog class is used to configure the application
//! settings.

class COptionsDlg : public CDialog
{
public:
	//! Default constructor.
	COptionsDlg();
	
private:
	//
	// Members.
	//
	
	//
	// Controls.
	//
	CCheckBox	m_ckMinToTray;		//!< Minimise to system tray checkbox.
	CComboBox	m_cbLenFormat;		//!< Session length format combo.

	//
	// Message handlers.
	//

	//! Dialog initialisation handler.
	virtual void OnInitDialog();

	//! OK button handler.
	virtual bool OnOk();
};

#endif // OPTIONSDLG_HPP
