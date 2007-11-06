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

#include <Core/Common.hpp>		// Core library common headers.
#include <WCL/wcl.hpp>			// Windows C++ library.

/******************************************************************************
**
** Application specific headers.
**
*******************************************************************************
*/

#include "Resource.h"

// Core data classes.
#include "AppTypes.hpp"
#include "Session.hpp"

// Collection classes.
#include "SessionList.hpp"
#include "TaskList.hpp"
#include "LocnList.hpp"

// GUI classes.
#include "SessionListBox.hpp"
#include "TaskComboBox.hpp"
#include "LocnComboBox.hpp"

// Report classes.
#include "Report.hpp"
#include "PrintReport.hpp"
#include "FileReport.hpp"
#include "TextReport.hpp"
#include "ReportOptions.hpp"

// Main app classes.
#include "AppCmds.hpp"
#include "AppToolBar.hpp"
#include "AppDlg.hpp"
#include "AppWnd.hpp"
#include "TaskTracker.hpp"

#endif //APPHEADERS_HPP
