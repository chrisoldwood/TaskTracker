/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		CLKOTDLG.HPP
** COMPONENT:	The Application.
** DESCRIPTION:	The CClockOutDlg class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef CLKOTDLG_HPP
#define CLKOTDLG_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include <WCL/CommonUI.hpp>
#include <WCL/DateTime.hpp>
#include "TaskComboBox.hpp"
#include "LocnComboBox.hpp"

/******************************************************************************
** 
** This dialog is used when the user clocks out.
**
*******************************************************************************
*/

class CClockOutDlg : public CDialog
{
public:
	//
	// Constructors/Destructor.
	//
	CClockOutDlg();
	
	//
	// Members.
	//
	CDateTime	m_dtDateTime;
	CString		m_strTask;
	CString		m_strLocn;

protected:
	//
	// Members.
	//
	CRadioBtn		m_rbNow;
	CRadioBtn		m_rbOnAt;
	CDateTimePicker	m_dtpDateTime;
	CTaskCombo		m_cbTask;
	CLocnCombo		m_cbLocn;
	
	//
	// Message processors.
	//
	virtual void OnInitDialog();
	virtual bool OnOk();

	//
	// Control message processors.
	//
	void OnNow();
	void OnOnAt();
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

#endif //CLKOTDLG_HPP
