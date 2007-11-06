/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		PRLOCDLG.CPP
** COMPONENT:	The Application.
** DESCRIPTION:	CPruneLocnsDlg class definition.
**
*******************************************************************************
*/

#include "Common.hpp"
#include "PruneLocnsDlg.hpp"
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

CPruneLocnsDlg::CPruneLocnsDlg()
	: CDialog(IDD_PRUNE_LOCNS)
{
	DEFINE_CTRL_TABLE
		CTRL(IDC_LOCNS, &m_lbLocns)
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

void CPruneLocnsDlg::OnInitDialog()
{
	// Template shorthands.
	typedef CLocnList::const_iterator CLocnIter;
	typedef CSessionList::const_iterator CSessIter;

	// For all locations...
	for(CLocnIter oLocnIter = App.m_oLocnList.begin(); oLocnIter != App.m_oLocnList.end(); ++oLocnIter)
	{
		bool bInUse = false;

		// For all sessions.
		for(CSessIter oSessIter = App.m_oSessionList.begin(); oSessIter != App.m_oSessionList.end(); ++oSessIter)
        {
        	// Location in use?
        	if ((*oLocnIter) == (*oSessIter)->Location())
        	{
        		bInUse = true;
        		break;
        	}
        }

        // Location in use by current session?
        if ( (App.m_pCurrSession.Get()       != nullptr)
		  && (App.m_pCurrSession->Location() == (*oLocnIter)) )
		{
        	bInUse = true;
		}

        // Location in use as "Last Location"?
        if ((*oLocnIter) == App.m_strLastLocn)
        	bInUse = true;
        
		// Add location if not in use?
		if (!bInUse)
			m_lbLocns.Add(*oLocnIter);
	}

	// Nothing to delete?
	if (m_lbLocns.Count() == 0)
	{
		NotifyMsg("There are no locations that can be deleted.");
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

bool CPruneLocnsDlg::OnOk()
{
	int iNumLocns = m_lbLocns.Count();

	// For all locations.
	for (int i = 0; i < iNumLocns; i++)
	{
		// Add to list if selected.
		if (m_lbLocns.IsSel(i))
			m_LocnList.Add(m_lbLocns.Text(i));
	}
	
	return true;
}
