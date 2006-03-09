////////////////////////////////////////////////////////////////////////////////
//! \author Chris Oldwood
//! \file   RENAMEDLG.CPP
//! \brief  The CRenameDlg class definition.

#include "AppHeaders.hpp"
#include "RenameDlg.hpp"

#ifdef _DEBUG
// For memory leak detection.
#define new DBGCRT_NEW
#endif

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
	// Remove selection from new task name combo.
	m_cbNewTask.CurSel(CB_ERR);
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
