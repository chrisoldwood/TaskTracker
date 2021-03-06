/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		EDTSNDLG.HPP
** COMPONENT:	The Application.
** DESCRIPTION:	The CEditSessionDlg class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef EDTSNDLG_HPP
#define EDTSNDLG_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include <WCL/CommonUI.hpp>
#include "SessionListBox.hpp"

/******************************************************************************
** 
** This dialog is used to add, modify and delete sessions.
**
*******************************************************************************
*/

class CEditSessionDlg : public CDialog
{
public:
	//
	// Constructors/Destructor.
	//
	CEditSessionDlg();
	
	//
	// Members.
	//
	bool m_bModified;

protected:
	//
	// Members.
	//
	CSessionListView	m_lvSessions;
	CButton				m_bnAdd;
	CButton				m_bnModify;
	CButton				m_bnDelete;
	
	//
	// Message processors.
	//
	virtual void OnInitDialog();
	virtual void OnDestroy();
	     LRESULT OnGridDblClick(NMHDR& rMsgHdr);

	//
	// Control message processors.
	//
	void OnAdd();
	void OnModify();
	void OnDelete();
	void OnRename();
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

#endif //EDTSNDLG_HPP
