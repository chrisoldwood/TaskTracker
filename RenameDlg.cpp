////////////////////////////////////////////////////////////////////////////////
//! \file   RENAMEDLG.CPP
//! \brief  The CRenameDlg class definition.
//! \author Chris Oldwood

#include "Common.hpp"
#include "RenameDlg.hpp"

////////////////////////////////////////////////////////////////////////////////
//! Default constructor.

CRenameDlg::CRenameDlg()
	: CDialog(IDD_RENAME)
{
	DEFINE_CTRL_TABLE
		CTRL(IDC_OLD_TASK, &m_cbOldTask)
		CTRL(IDC_NEW_TASK, &m_cbNewTask)
	END_CTRL_TABLE

	DEFINE_CTRLMSG_TABLE
	END_CTRLMSG_TABLE
}

////////////////////////////////////////////////////////////////////////////////
//! Dialog initialisation handler.

void CRenameDlg::OnInitDialog()
{
	m_cbNewTask.RemoveSelection();
}

////////////////////////////////////////////////////////////////////////////////
//! OK button handler.

bool CRenameDlg::OnOk()
{
	// Get task and location.
	m_strOldTask = m_cbOldTask.Text();
	m_strNewTask = m_cbNewTask.Text();

	return true;
}
