////////////////////////////////////////////////////////////////////////////////
//! \author Chris Oldwood
//! \file   EXPORTDLG.HPP
//! \brief  The CExportDlg class declaration.

// Check for previous inclusion
#ifndef EXPORTDLG_HPP
#define EXPORTDLG_HPP

////////////////////////////////////////////////////////////////////////////////
//! The database export dialog.

class CExportDlg : public CDialog
{
public:
	//! Default constructor.
	CExportDlg();
	
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

#endif // EXPORTDLG_HPP
