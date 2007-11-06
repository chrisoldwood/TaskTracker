/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		APPDLG.HPP
** COMPONENT:	The Application.
** DESCRIPTION:	The CAppDlg class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef APPDLG_HPP
#define APPDLG_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include <WCL/MainDlg.hpp>
#include <WCL/DateTime.hpp>
#include <WCL/Label.hpp>

/******************************************************************************
** 
** This is the main application dialog. It displays a summary of the tasks
** to date.
**
*******************************************************************************
*/

class CAppDlg : public CMainDlg
{
public:
	//
	// Constructors/Destructor.
	//
	CAppDlg();
	
	//
	// Methods.
	//
	void Update();

protected:
	//
	// Members.
	//
	CDateTime	m_dtCurrent;
	CFont		m_oHeaderFont;
	DWORD		m_dwLastBackup;

	//
	// Controls.
	//
	CLabel		m_txtCurrDate;
	CLabel		m_txtCurrTime;
	CLabel		m_txtSessionDate;
	CLabel		m_txtSessionTime;
	CLabel		m_txtSessionLen;
	CLabel		m_txtSessionTask;
	CLabel		m_txtTotalToday;
	CLabel		m_txtTotalWeek;
	CLabel		m_txtTotalMonth;
	CLabel		m_txtTotalTotal;

	//
	// Message processors.
	//
	virtual void OnInitDialog();
	virtual void OnDestroy();
	virtual void OnTimer(uint iTimerID);
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

#endif //APPDLG_HPP
