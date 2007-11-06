/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		EDTSNDLG.CPP
** COMPONENT:	The Application.
** DESCRIPTION:	CEditSessionDlg class definition.
**
*******************************************************************************
*/

#include "Common.hpp"
#include "EditSessionDlg.hpp"
#include "AddSessionDlg.hpp"
#include "ModifySessionDlg.hpp"
#include "RenameDlg.hpp"
#include "TaskTracker.hpp"

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
	, m_bModified(false)
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
		CMD_CTRLMSG(IDC_RENAME,   BN_CLICKED, OnRename)
		NFY_CTRLMSG(IDC_SESSIONS, NM_DBLCLK,  OnGridDblClick)
	END_CTRLMSG_TABLE

	DEFINE_GRAVITY_TABLE
		CTRLGRAV(IDC_SESSIONS, LEFT_EDGE,  TOP_EDGE,    RIGHT_EDGE, BOTTOM_EDGE)
		CTRLGRAV(IDC_ADD,      LEFT_EDGE,  BOTTOM_EDGE, LEFT_EDGE,  BOTTOM_EDGE)
		CTRLGRAV(IDC_MODIFY,   LEFT_EDGE,  BOTTOM_EDGE, LEFT_EDGE,  BOTTOM_EDGE)
		CTRLGRAV(IDC_DELETE,   LEFT_EDGE,  BOTTOM_EDGE, LEFT_EDGE,  BOTTOM_EDGE)
		CTRLGRAV(IDC_RENAME,   LEFT_EDGE,  BOTTOM_EDGE, LEFT_EDGE,  BOTTOM_EDGE)
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
    if (!App.m_oSessionList.size())
    {
    	// Disable delete and modify buttons.
		m_bnModify.Enable(false);
		m_bnDelete.Enable(false);
    }

	// Resize dialog to previous size.
	if (!App.m_rcEditDlg.Empty())
		Move(App.m_rcEditDlg);
}

/******************************************************************************
** Method:		OnDestroy()
**
** Description:	The window is being destroyed.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CEditSessionDlg::OnDestroy()
{
	// Remember its position.
	App.m_rcEditDlg = Placement();
}

/******************************************************************************
** Method:		OnAdd()
**
** Description:	Add a new session to the database.
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
		// Type shorthands.
		typedef CSessionList::iterator Iter;

		// Create a new session.
		CSessionPtr pNewSession(new CSession);
		
		// Initialise.
		pNewSession->Start (Dlg.m_dtInDateTime,  Dlg.m_strTask, Dlg.m_strLocn);
		pNewSession->Finish(Dlg.m_dtOutDateTime, Dlg.m_strTask, Dlg.m_strLocn);
	
		// Add task to list if set.
		if (Dlg.m_strTask != "")
			App.m_oTaskList.Add(Dlg.m_strTask);
	
		// Add Location to list if set.
		if (Dlg.m_strLocn != "")
			App.m_oLocnList.Add(Dlg.m_strLocn);
	
		// Remember task and location used.
		App.m_strLastTask = Dlg.m_strTask;
		App.m_strLastLocn = Dlg.m_strLocn;

		// Add to sessionlist and view.
		uint nPos = App.m_oSessionList.Add(pNewSession);

		m_lvSessions.AddSession(nPos, pNewSession);
		m_lvSessions.Select(nPos);
		m_lvSessions.MakeItemVisible(nPos);

    	// Enable delete and modify buttons.
		m_bnModify.Enable();
		m_bnDelete.Enable();
		
		// Update dirty flag.
		m_bModified = true;
	}
}

/******************************************************************************
** Method:		OnModify()
**
** Description:	Edit the currently selected session.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CEditSessionDlg::OnModify()
{
	ASSERT(m_lvSessions.Selection() != LB_ERR);

	CModifySessionDlg Dlg;

	// Get selected session.
	CSessionPtr pSession = m_lvSessions.SelSession();

	ASSERT(pSession.Get() != nullptr);
	
	// Initialise dialog.
	Dlg.m_pSession      = pSession;
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
		App.m_oSessionList.Remove(iIdx);
		m_lvSessions.DeleteItem(iIdx);

		// Update session details.
		pSession->Start (Dlg.m_dtInDateTime,  Dlg.m_strTask, Dlg.m_strLocn);
		pSession->Finish(Dlg.m_dtOutDateTime, Dlg.m_strTask, Dlg.m_strLocn);
	
		// Add task to list if set.
		if (Dlg.m_strTask != "")
			App.m_oTaskList.Add(Dlg.m_strTask);
	
		// Add Location to list if set.
		if (Dlg.m_strLocn != "")
			App.m_oLocnList.Add(Dlg.m_strLocn);
	
		// Re-add to sessionlist and view.
		uint nPos = App.m_oSessionList.Add(pSession);

		m_lvSessions.AddSession(nPos, pSession);
		m_lvSessions.Select(nPos);
		m_lvSessions.MakeItemVisible(nPos);

		// Update dirty flag.
		m_bModified = true;
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
** Description:	Delete the selected session.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CEditSessionDlg::OnDelete()
{
	ASSERT(m_lvSessions.Selection() != LB_ERR);

	// Get current selection.
	int         iIdx     = m_lvSessions.Selection();
	CSessionPtr pSession = m_lvSessions.SelSession();

	ASSERT(pSession.Get() != nullptr);

	// Remove from collection and view.
	App.m_oSessionList.Remove(iIdx);
	m_lvSessions.DeleteItem(iIdx);

	ASSERT(App.m_oSessionList.size() == (size_t)m_lvSessions.ItemCount());

	// Change listbox selection.
	int iNumItems = App.m_oSessionList.size();
	
    if (iNumItems > 0)
    {
    	// Select nearest item
    	if (iIdx == iNumItems)
    		--iIdx;

    	m_lvSessions.Select(iIdx);
    }
    else
    {
    	// Disable delete and modify buttons.
		m_bnModify.Enable(false);
		m_bnDelete.Enable(false);
    }
    
	// Update dirty flag.
	m_bModified = true;
}

/******************************************************************************
** Method:		OnRename()
**
** Description:	Rename a task.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CEditSessionDlg::OnRename()
{
	CRenameDlg Dlg;

	// Query the user for the task to rename.
	if (Dlg.RunModal(*this) == IDOK)
	{
		typedef CSessionList::iterator Iter;

		// For all sessions...
		for (Iter pIter = App.m_oSessionList.begin(); pIter != App.m_oSessionList.end(); ++pIter)
		{
			CSessionPtr pSession = *pIter;

			// Rename task?
			if (pSession->Task() == Dlg.m_strOldTask)
				pSession->Task(Dlg.m_strNewTask);
		}

		// Add task to list if set.
		if (Dlg.m_strNewTask != "")
			App.m_oTaskList.Add(Dlg.m_strNewTask);

		// Update UI.
		m_lvSessions.RefreshSessions();

		// Update dirty flag.
		m_bModified = true;
	}
}
