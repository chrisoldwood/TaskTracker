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
	CAppCmds();
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

	// Help menu.
	void OnHelpAbout();

	//
	// UI handlers.
	//
	void OnUISessionClockIn();
	void OnUISessionSwitchTasks();
	void OnUISessionClockOut();

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
