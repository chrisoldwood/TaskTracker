////////////////////////////////////////////////////////////////////////////////
//! \file   EXPORTDLG.HPP
//! \brief  The CExportDlg class declaration.
//! \author Chris Oldwood

// Check for previous inclusion
#ifndef EXPORTDLG_HPP
#define EXPORTDLG_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include <WCL/CommonUI.hpp>
#include <WCL/Path.hpp>
#include <WCL/PathEditBox.hpp>

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
	CDate	m_dFromDate;			//!< The export range start date.
	CDate	m_dToDate;				//!< The export range end date.

private:
	//
	// Members.
	//
	
	//
	// Controls.
	//
	CPathEditBox	m_ebFileName;	//!< The file name edit box.
	CDateTimePicker	m_dtpFromDate;	//!< The start date picker.
	CDateTimePicker	m_dtpToDate;	//!< The end date picker.

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
