/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		EDTSNDLG.CPP
** COMPONENT:	The Application.
** DESCRIPTION:	CEditSessionDlg class definition.
**
*******************************************************************************
*/

#include "AppHeaders.hpp"
#include "EditSessionDlg.hpp"
#include "AddSessionDlg.hpp"
#include "ModifySessionDlg.hpp"

/******************************************************************************
** Method:		Default constructor.
**
** Description:	.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

CEditSessionDlg::CEditSessionDlg() : CDialog(IDD_EDIT_SESSIONS)
{
	DEFINE_CTRL_TABLE
		CTRL(IDC_SESSIONS,	&m_lbSessions)
		CTRL(IDC_ADD,		&m_bnAdd)
		CTRL(IDC_MODIFY,	&m_bnModify)
		CTRL(IDC_DELETE,	&m_bnDelete)
	END_CTRL_TABLE

	DEFINE_CTRLMSG_TABLE
		CMD_CTRLMSG(IDC_ADD, 		BN_CLICKED,		OnAdd)
		CMD_CTRLMSG(IDC_MODIFY,		BN_CLICKED,		OnModify)
		CMD_CTRLMSG(IDC_DELETE,		BN_CLICKED,		OnDelete)
		CMD_CTRLMSG(IDC_SESSIONS,	LBN_DBLCLK,		OnModify)
	END_CTRLMSG_TABLE
}

/******************************************************************************
** Method:		OnInitDialog()
**
** Description:	Initialise the dialog.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CEditSessionDlg::OnInitDialog()
{
	// Session list empty?
    if (!App.SessionList().Length())
    {
    	// Disable delete and modify buttons.
		m_bnModify.Enable(false);
		m_bnDelete.Enable(false);
    }
}

/******************************************************************************
** Method:		OnAdd()
**
** Description:	.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CEditSessionDlg::OnAdd()
{
	CAddSessionDlg Dlg;
	
	if (Dlg.RunModal(*this) == IDOK)
	{
		CSession*	pNewSession;
		
		// Create a new session.
		pNewSession = new CSession;
		ASSERT(pNewSession);
	
		// Initialise.
		pNewSession->Start(Dlg.m_dtInDateTime, Dlg.m_strTask);
		pNewSession->Finish(Dlg.m_dtOutDateTime, Dlg.m_strTask);
	
		// Add task to list if set.
		if (Dlg.m_strTask != "")
			App.TaskList().Add(Dlg.m_strTask);
	
		// Add to list.
		int i = App.SessionList().Add(pNewSession);
		
		// Refresh session list.
		m_lbSessions.Refresh();
		m_lbSessions.CurSel(i);

    	// Enable delete and modify buttons.
		m_bnModify.Enable();
		m_bnDelete.Enable();
		
		// Update dirty flag.
		App.Modified();
	}
}

/******************************************************************************
** Method:		OnModify()
**
** Description:	.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CEditSessionDlg::OnModify()
{
	CModifySessionDlg Dlg;

	// Get selected session.
	CSession* pSession = m_lbSessions.CurrSession();
	ASSERT(pSession);
	
	// Initialise dialog.
	Dlg.m_dtInDateTime  = pSession->Start();
	Dlg.m_dtOutDateTime = pSession->Finish();
	Dlg.m_strTask       = pSession->Task();
	
	// Show it.
	if (Dlg.RunModal(*this) == IDOK)
	{
		// Update session details.
		pSession->Start(Dlg.m_dtInDateTime, Dlg.m_strTask);
		pSession->Finish(Dlg.m_dtOutDateTime, Dlg.m_strTask);
	
		// Add task to list if set.
		if (Dlg.m_strTask != "")
			App.TaskList().Add(Dlg.m_strTask);
	
		// Refresh session list.
		m_lbSessions.Refresh();

		// Update dirty flag.
		App.Modified();
	}
}

/******************************************************************************
** Method:		OnDelete()
**
** Description:	.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CEditSessionDlg::OnDelete()
{
	// Get current selection.
	int iIdx = m_lbSessions.CurSel();
	ASSERT(iIdx != LB_ERR);

	// Get selected session.
	CSession* pSession = m_lbSessions.CurrSession();
	ASSERT(pSession);

	// Delete session.
	App.SessionList().Remove(pSession);
	delete pSession;
	
	// Update listbox.
	m_lbSessions.Delete(iIdx);

	// Change listbox selection.
	int iNumItems = App.SessionList().Length();
	
    if (iNumItems)
    {
    	// Select next item
    	if (iIdx == iNumItems)
    		m_lbSessions.CurSel(iIdx-1);
    	else
    		m_lbSessions.CurSel(iIdx);
    }
    else
    {
    	// Disable delete and modify buttons.
		m_bnModify.Enable(false);
		m_bnDelete.Enable(false);
    }
    
	// Update dirty flag.
	App.Modified();
}
