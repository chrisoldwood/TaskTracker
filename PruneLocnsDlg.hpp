/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		PRLOCDLG.HPP
** COMPONENT:	The Application.
** DESCRIPTION:	The CPruneLocnsDlg class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef PRLOCDLG_HPP
#define PRLOCDLG_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include <WCL/CommonUI.hpp>
#include "LocnList.hpp"

/******************************************************************************
** 
** This dialog is used when the user prunes locations.
**
*******************************************************************************
*/

class CPruneLocnsDlg : public CDialog
{
public:
	//
	// Constructors/Destructor.
	//
	CPruneLocnsDlg();
	
	//
	// Members.
	//
	CLocnList	m_LocnList;

protected:
	//
	// Members.
	//
	CListBox	m_lbLocns;
	
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

#endif //PRLOCDLG_HPP
