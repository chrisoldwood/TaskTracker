////////////////////////////////////////////////////////////////////////////////
//! \author Chris Oldwood
//! \file   IMPORTDLG.HPP
//! \brief  The CImportDlg class declaration.

// Check for previous inclusion
#ifndef IMPORTDLG_HPP
#define IMPORTDLG_HPP

////////////////////////////////////////////////////////////////////////////////
//! The database import dialog.

class CImportDlg : public CDialog
{
public:
	//! Default constructor.
	CImportDlg();
	
	//
	// Members.
	//
	CPath	m_strFileName;			//!< The file name.

private:
	//
	// Members.
	//
	
	//
	// Controls.
	//
	CPathEditBox	m_ebFileName;	//!< The file name edit box.

	//
	// Message handlers.
	//

	//! Dialog initialisation handler.
	virtual void OnInitDialog();

	//! OK button handler.
	virtual bool OnOk();

	//! File Browse button handler.
	void OnBrowse();
};

#endif // IMPORTDLG_HPP
