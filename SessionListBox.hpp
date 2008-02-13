/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		SESSNLB.HPP
** COMPONENT:	The Application.
** DESCRIPTION:	The CSessionListView class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef SESSNLB_HPP
#define SESSNLB_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include <WCL/ListView.hpp>
#include "Session.hpp"

/******************************************************************************
** 
** This is a list view used to display the list of sessions.
** NB: This class used to be based on a CListBox.
**
*******************************************************************************
*/

class CSessionListView : public CListView
{
public:
	//
	// Constructors/Destructor.
	//
	CSessionListView();
	~CSessionListView();
	
	//
	// Methods.
	//
	void AddSession(size_t nItem, const CSessionPtr& pSession);
	void RemoveSession(size_t nItem);
	void RefreshSessions();

	CSessionPtr SelSession();

protected:
	// Column names.
	enum
	{
		DAY_COLUMN	= 0,
		DATE_COLUMN	= 1,
		IN_COLUMN	= 2,
		OUT_COLUMN	= 3,
		LEN_COLUMN	= 4,
		TASK_COLUMN	= 5,
		LOCN_COLUMN	= 6,

		NUM_COLUMNS = 7,
	};

	// Column data.
	static const tchar*	apszLabels[];
	static const uint	aiWidths[];

	//
	// Message processors.
	//
	virtual void OnCreate(const CRect& rcClient);

	//
	// Internal methods.
	//
	void RefreshSession(size_t nItem, const CSession* pSession);
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

#endif //SESSNLB_HPP
