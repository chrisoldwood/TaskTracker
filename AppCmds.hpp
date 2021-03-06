/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		APPCMDS.HPP
** COMPONENT:	The Application.
** DESCRIPTION:	The CAppCmds class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef APPCMDS_HPP
#define APPCMDS_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include <WCL/CmdCtrl.hpp>

// Forward declarations.
class CAppWnd;

/******************************************************************************
** 
** Application command controller.
**
*******************************************************************************
*/

class CAppCmds : public CCmdControl
{
public:
	//
	// Constructors/Destructor.
	//
	CAppCmds(CAppWnd& appWnd);
	~CAppCmds();

	//
	// Commands.
	//

	// File menu.
	void OnFileExport();
	void OnFileImport();
	void OnFileExit();

	// Session menu.
	void OnSessionClockIn();
	void OnSessionSwitchTasks();
	void OnSessionClockOut();
	void OnSessionEdit();

	// Report menu.
	void OnReportWindow();
	void OnReportClipboard();
	void OnReportPrint();
	void OnReportFile();

	// Prune menu.
	void OnPruneSessions();
	void OnPruneTasks();
	void OnPruneLocations();

	// Tools menu.
	void OnToolsOptions();

	// Help menu.
	void OnHelpContents();
	void OnHelpAbout();

	//
	// UI handlers.
	//
	void OnUISessionClockIn();
	void OnUISessionSwitchTasks();
	void OnUISessionClockOut();
	void OnUIPruneSessions();
	void OnUIPruneTasks();
	void OnUIPruneLocations();

protected:
	//
	// Members.
	//
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

#endif //APPCMDS_HPP
