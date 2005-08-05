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

CEditSessionDlg::CEditSessionDlg()
	: CDialog(IDD_EDIT_SESSIONS)
{
	DEFINE_CTRL_TABLE
		CTRL(IDC_SESSIONS,	&m_lvSessions)
		CTRL(IDC_ADD,		&m_bnAdd)
		CTRL(IDC_MODIFY,	&m_bnModify)
		CTRL(IDC_DELETE,	&m_bnDelete)
	END_CTRL_TABLE

	DEFINE_CTRLMSG_TABLE
		CMD_CTRLMSG(IDC_ADD,      BN_CLICKED, OnAdd)
		CMD_CTRLMSG(IDC_MODIFY,   BN_CLICKED, OnModify)
		CMD_CTRLMSG(IDC_DELETE,   BN_CLICKED, OnDelete)
		NFY_CTRLMSG(IDC_SESSIONS, NM_DBLCLK,  OnGridDblClick)
	END_CTRLMSG_TABLE

	DEFINE_GRAVITY_TABLE
		CTRLGRAV(IDC_SESSIONS, LEFT_EDGE,  TOP_EDGE,    RIGHT_EDGE, BOTTOM_EDGE)
		CTRLGRAV(IDC_ADD,      LEFT_EDGE,  BOTTOM_EDGE, LEFT_EDGE,  BOTTOM_EDGE)
		CTRLGRAV(IDC_MODIFY,   LEFT_EDGE,  BOTTOM_EDGE, LEFT_EDGE,  BOTTOM_EDGE)
		CTRLGRAV(IDC_DELETE,   LEFT_EDGE,  BOTTOM_EDGE, LEFT_EDGE,  BOTTOM_EDGE)
		CTRLGRAV(IDOK,         RIGHT_EDGE, BOTTOM_EDGE, RIGHT_EDGE, BOTTOM_EDGE)
	END_GRAVITY_TABLE
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
    if (!App.SessionList().size())
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
		pNewSession->Start (Dlg.m_dtInDateTime,  Dlg.m_strTask, Dlg.m_strLocn);
		pNewSession->Finish(Dlg.m_dtOutDateTime, Dlg.m_strTask, Dlg.m_strLocn);
	
		// Add task to list if set.
		if (Dlg.m_strTask != "")
			App.TaskList().Add(Dlg.m_strTask);
	
		// Add Location to list if set.
		if (Dlg.m_strLocn != "")
			App.LocnList().Add(Dlg.m_strLocn);
	
		// Add to list.
		App.SessionList().Add(pNewSession);

		int i = App.SessionList().IndexOf(pNewSession);

		// Refresh session list.
		m_lvSessions.Refresh();
		m_lvSessions.Select(i);

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
	CSession* pSession = m_lvSessions.CurrSession();
	ASSERT(pSession);
	
	// Initialise dialog.
	Dlg.m_dtInDateTime  = pSession->Start();
	Dlg.m_dtOutDateTime = pSession->Finish();
	Dlg.m_strTask       = pSession->Task();
	Dlg.m_strLocn       = pSession->Location();
	
	// Show it.
	if (Dlg.RunModal(*this) == IDOK)
	{
		// Get current selection.
		int iIdx = m_lvSessions.Selection();

		// Remove session from list and view.
		App.SessionList().Remove(pSession);
		m_lvSessions.DeleteItem(iIdx);

		// Update session details.
		pSession->Start (Dlg.m_dtInDateTime,  Dlg.m_strTask, Dlg.m_strLocn);
		pSession->Finish(Dlg.m_dtOutDateTime, Dlg.m_strTask, Dlg.m_strLocn);
	
		// Add task to list if set.
		if (Dlg.m_strTask != "")
			App.TaskList().Add(Dlg.m_strTask);
	
		// Add Location to list if set.
		if (Dlg.m_strLocn != "")
			App.LocnList().Add(Dlg.m_strLocn);
	
		// Add to list.
		App.SessionList().Add(pSession);
		
		int i = App.SessionList().IndexOf(pSession);

		// Refresh session list.
		m_lvSessions.Refresh();
		m_lvSessions.Select(i);
		m_lvSessions.MakeItemVisible(i);

		// Update dirty flag.
		App.Modified();
	}
}

/******************************************************************************
** Method:		OnGridDblClick()
**
** Description:	The user double clicked the grid.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

LRESULT CEditSessionDlg::OnGridDblClick(NMHDR&)
{
	// If item selected, edit it.
	if (m_lvSessions.IsSelection())
		OnModify();

	return 0;
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
	int iIdx = m_lvSessions.Selection();
	ASSERT(iIdx != LB_ERR);

	// Get selected session.
	CSession* pSession = m_lvSessions.CurrSession();
	ASSERT(pSession);

	// Delete session.
	App.SessionList().Remove(pSession);
	delete pSession;
	
	// Update listbox.
	m_lvSessions.DeleteItem(iIdx);

	// Change listbox selection.
	int iNumItems = App.SessionList().size();
	
    if (iNumItems)
    {
    	// Select next item
    	if (iIdx == iNumItems)
    		m_lvSessions.Select(iIdx-1);
    	else
    		m_lvSessions.Select(iIdx);
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
