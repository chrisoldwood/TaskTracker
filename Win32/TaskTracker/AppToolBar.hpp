/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		APPTOOLBAR.HPP
** COMPONENT:	The Application.
** DESCRIPTION:	The CAppToolBar class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef APPTOOLBAR_HPP
#define APPTOOLBAR_HPP

/******************************************************************************
** 
** This is the applications toolbar.
**
*******************************************************************************
*/

class CAppToolBar : public CToolBar
{
public:
	//
	// Constructors/Destructor.
	//
	CAppToolBar();
	~CAppToolBar();
	
	//
	// Members.
	//
	CCmdButton	m_ExportBtn;
	CCmdButton	m_ImportBtn;
	CCmdButton	m_ClockInBtn;
	CCmdButton	m_SwitchTasksBtn;
	CCmdButton	m_ClockOutBtn;
	CCmdButton	m_EditSessionsBtn;
	CCmdButton	m_WindowReportBtn;
	CCmdButton	m_ClpbrdReportBtn;
	CCmdButton	m_PrintReportBtn;
	CCmdButton	m_FileReportBtn;
	CCmdButton	m_HelpBtn;
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

#endif //APPTOOLBAR_HPP
