/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		PRTSKDLG.CPP
** COMPONENT:	The Application.
** DESCRIPTION:	CPruneTasksDlg class definition.
**
*******************************************************************************
*/

#include "AppHeaders.hpp"
#include "PruneTasksDlg.hpp"

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
	CTaskListEnum	Enum(App.TaskList());
	CString*		pString;
	
	// For all tasks.
	while((pString = Enum.Next()) != NULL)
	{
		CSessionEnum	Enum(App.SessionList());
		CSession*		pSession;
		bool			bInUse = false;

		// For all sessions.
		while((pSession = Enum.Next()) != NULL)
        {
        	// Task in use?
        	if (*pString == pSession->Task())
        	{
        		bInUse = true;
        		break;
        	}
        }
        
        // Task in use by current session?
        if ( (App.CurrentSession()) && (*pString == App.CurrentSession()->Task()) )
        	bInUse = true;

        // Task in use as "Last Task"?
        if (*pString == App.LastTask())
        	bInUse = true;
        
		// Add task if not in use?
		if (!bInUse)
			m_lbTasks.Add(*pString);
	}

	// Nothing to delete?
	if (m_lbTasks.Count() == 0)
	{
		NotifyMsg("There are no tasks that can be deleted.");
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
