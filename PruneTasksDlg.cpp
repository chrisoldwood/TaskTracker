/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		PRTSKDLG.CPP
** COMPONENT:	The Application.
** DESCRIPTION:	CPruneTasksDlg class definition.
**
*******************************************************************************
*/

#include "Common.hpp"
#include "PruneTasksDlg.hpp"
#include "TaskTracker.hpp"

/******************************************************************************
** Method:		Default constructor.
**
** Description:	Override any default settings.
**
** Parameters:	pParent		The dialogs' parent window.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

CPruneTasksDlg::CPruneTasksDlg() : CDialog(IDD_PRUNE_TASKS)
{
	DEFINE_CTRL_TABLE
		CTRL(IDC_TASKS, &m_lbTasks)
	END_CTRL_TABLE
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

void CPruneTasksDlg::OnInitDialog()
{
	// Template shorthands.
	typedef CTaskList::const_iterator CTaskIter;
	typedef CSessionList::const_iterator CSessIter;

	// For all tasks...
	for(CTaskIter oTaskIter = App.m_oTaskList.begin(); oTaskIter != App.m_oTaskList.end(); ++oTaskIter)
	{
		bool bInUse = false;

		// For all sessions.
		for(CSessIter oSessIter = App.m_oSessionList.begin(); oSessIter != App.m_oSessionList.end(); ++oSessIter)
        {
        	// Task in use?
        	if ((*oTaskIter) == (*oSessIter)->Task())
        	{
        		bInUse = true;
        		break;
        	}
        }
        
        // Task in use by current session?
        if ( (App.m_pCurrSession.Get()   != nullptr)
		  && (App.m_pCurrSession->Task() == (*oTaskIter)) )
		{
        	bInUse = true;
		}

        // Task in use as "Last Task"?
        if ((*oTaskIter) == App.m_strLastTask)
        	bInUse = true;
        
		// Add task if not in use?
		if (!bInUse)
			m_lbTasks.Add(*oTaskIter);
	}

	// Nothing to delete?
	if (m_lbTasks.Count() == 0)
	{
		NotifyMsg(TXT("There are no tasks that can be deleted."));
		EndDialog();
	}
}

/******************************************************************************
** Method:		OnOk()
**
** Description:	Check if the user can okay the dialog.
**
** Parameters:	None.
**
** Returns:		true or false.
**
*******************************************************************************
*/

bool CPruneTasksDlg::OnOk()
{
	int iNumTasks = m_lbTasks.Count();

	// For all tasks.
	for (int i = 0; i < iNumTasks; i++)
	{
		// Add to list if selected.
		if (m_lbTasks.IsSel(i))
			m_TaskList.Add(m_lbTasks.Text(i));
	}
	
	return true;
}
