/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		PRTSKDLG.HPP
** COMPONENT:	The Application.
** DESCRIPTION:	The CPruneTasksDlg class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef PRTSKDLG_HPP
#define PRTSKDLG_HPP

/******************************************************************************
** 
** This dialog is used when the user prunes tasks.
**
*******************************************************************************
*/

class CPruneTasksDlg : public CDialog
{
public:
	//
	// Constructors/Destructor.
	//
	CPruneTasksDlg();
	
	//
	// Members.
	//
	CTaskList	m_TaskList;

protected:
	//
	// Members.
	//
	CListBox	m_lbTasks;
	
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

#endif //PRTSKDLG_HPP
