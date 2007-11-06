////////////////////////////////////////////////////////////////////////////////
//! \file   IMPORTDLG.HPP
//! \brief  The CImportDlg class declaration.
//! \author Chris Oldwood

// Check for previous inclusion
#ifndef IMPORTDLG_HPP
#define IMPORTDLG_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include <WCL/CommonUI.hpp>
#include <WCL/Path.hpp>
#include <WCL/PathEditBox.hpp>

////////////////////////////////////////////////////////////////////////////////
//! The database import dialog.

class CImportDlg : public CDialog
{
public:
	//! Default constructor.
	CImportDlg();

	//! Import action.
	enum Action
	{
		REPLACE,					//! Replace existing database.
		MERGE,						//! Merge into database.
	};

	//
	// Members.
	//
	CPath	m_strFileName;			//!< The file name.
	Action	m_eAction;				//!< Import action.
	bool	m_bNoDuplicates;		//!< Don't import duplicates.

private:
	//
	// Members.
	//
	
	//
	// Controls.
	//
	CPathEditBox	m_ebFileName;		//!< The file name edit box.
	CRadioBtn		m_rbReplace;		//!< Replace database radio button.
	CRadioBtn		m_rbMerge;			//!< Merge database radio button.
	CCheckBox		m_ckNoDuplicates;	//!< No duplicates check box.

	//
	// Message handlers.
	//

	//! Dialog initialisation handler.
	virtual void OnInitDialog();

	//! OK button handler.
	virtual bool OnOk();

	//! File Browse button handler.
	void OnBrowseClicked();

	//! Replace button state handler.
	void OnReplaceClicked();

	//! Merge button state handler.
	void OnMergeClicked();
};

#endif // IMPORTDLG_HPP
