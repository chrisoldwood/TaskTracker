/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		APPHEADERS.HPP
** COMPONENT:	The Application.
** DESCRIPTION:	Wrapper to include all the main application headers.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef APPHEADERS_HPP
#define APPHEADERS_HPP

/******************************************************************************
**
** System headers.
**
*******************************************************************************
*/

#include "wcl.hpp"		// Windows C++ library and core headers.

/******************************************************************************
**
** Application specific headers.
**
*******************************************************************************
*/

#include "Resource.h"

// Core data classes.
#include "Session.hpp"

// Collection classes.
#include "SessionList.hpp"
#include "SessionListEnum.hpp"
#include "TaskList.hpp"
#include "TaskListEnum.hpp"
#include "LocnList.hpp"
#include "LocnListEnum.hpp"

// GUI classes.
#include "SessionListBox.hpp"
#include "TaskComboBox.hpp"
#include "LocnComboBox.hpp"

// Report classes.
#include "Report.hpp"
#include "PrintReport.hpp"
#include "FileReport.hpp"
#include "TextReport.hpp"

// Main app classes.
#include "AppCmds.hpp"
#include "AppToolBar.hpp"
#include "AppDlg.hpp"
#include "AppWnd.hpp"
#include "TaskTracker.hpp"

#endif //APPHEADERS_HPP
