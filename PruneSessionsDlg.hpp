/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		PRSESDLG.HPP
** COMPONENT:	The Application.
** DESCRIPTION:	The CPruneSessionsDlg class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef PRSESDLG_HPP
#define PRSESDLG_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include <WCL/CommonUI.hpp>

/******************************************************************************
** 
** This dialog is used when the user prunes sessions.
**
*******************************************************************************
*/

class CPruneSessionsDlg : public CDialog
{
public:
	//
	// Constructors/Destructor.
	//
	CPruneSessionsDlg();
	
	//
	// Members.
	//
	CDate	m_dFromDate;		//!< The prune range start date.
	CDate	m_dToDate;			//!< The prune range end date.

protected:
	//
	// Members.
	//
	CRadioBtn		m_rbAll;		//!< The prune all radio button.
	CRadioBtn		m_rbBetween;	//!< The prune range radio button.
	CDateTimePicker	m_dtpFromDate;	//!< The start date picker.
	CDateTimePicker	m_dtpToDate;	//!< The end date picker.
	
	//
	// Message processors.
	//
	virtual void OnInitDialog();
	virtual bool OnOk();

	//
	// Control message processors.
	//
	void OnAll();
	void OnBetween();
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

#endif //PRSESDLG_HPP
