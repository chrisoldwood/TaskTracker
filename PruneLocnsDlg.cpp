/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		PRLOCDLG.CPP
** COMPONENT:	The Application.
** DESCRIPTION:	CPruneLocnsDlg class definition.
**
*******************************************************************************
*/

#include "AppHeaders.hpp"
#include "PruneLocnsDlg.hpp"

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
	CLocnListEnum	Enum(App.LocnList());
	CString*		pString;
	
	// For all locations.
	while((pString = Enum.Next()) != NULL)
	{
		CSessionEnum	Enum(App.SessionList());
		CSession*		pSession;
		bool			bInUse = false;

		// For all sessions.
		while((pSession = Enum.Next()) != NULL)
        {
        	// Location in use?
        	if (*pString == pSession->Location())
        	{
        		bInUse = true;
        		break;
        	}
        }
        
        // Location in use by current session?
        if ( (App.CurrentSession()) && (*pString == App.CurrentSession()->Location()) )
        	bInUse = true;

        // Location in use as "Last Location"?
        if (*pString == App.LastLocn())
        	bInUse = true;
        
		// Add location if not in use?
		if (!bInUse)
			m_lbLocns.Add(*pString);
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
