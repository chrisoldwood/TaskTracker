////////////////////////////////////////////////////////////////////////////////
//! \file   IMPORTDLG.CPP
//! \brief  The CImportDlg class definition.
//! \author Chris Oldwood

#include "AppHeaders.hpp"
#include "ImportDlg.hpp"

#ifdef _DEBUG
// For memory leak detection.
#define new DBGCRT_NEW
#endif

////////////////////////////////////////////////////////////////////////////////
//! Default constructor.

CImportDlg::CImportDlg()
	: CDialog(IDD_IMPORT)
	, m_strFileName(App.m_strImportFile)
	, m_eAction(REPLACE)
	, m_bNoDuplicates(true)
{
	DEFINE_CTRL_TABLE
		CTRL(IDC_FILENAME,      &m_ebFileName)
		CTRL(IDC_REPLACE,       &m_rbReplace)
		CTRL(IDC_MERGE,         &m_rbMerge)
		CTRL(IDC_NO_DUPLICATES, &m_ckNoDuplicates)
	END_CTRL_TABLE

	DEFINE_CTRLMSG_TABLE
		CMD_CTRLMSG(IDC_BROWSE,  BN_CLICKED, OnBrowseClicked)
		CMD_CTRLMSG(IDC_REPLACE, BN_CLICKED, OnReplaceClicked)
		CMD_CTRLMSG(IDC_MERGE,   BN_CLICKED, OnMergeClicked)
	END_CTRLMSG_TABLE
}

////////////////////////////////////////////////////////////////////////////////
//! Dialog initialisation handler.

void CImportDlg::OnInitDialog()
{
	// Initialise controls.
	if (!m_strFileName.Empty())
		m_ebFileName.Text(m_strFileName);

	m_rbReplace.Check(m_eAction == REPLACE);
	m_rbMerge.Check(m_eAction == MERGE);
	m_ckNoDuplicates.Check(m_bNoDuplicates);

	// Initialise dependent button states.
	OnReplaceClicked();
}

////////////////////////////////////////////////////////////////////////////////
//! OK button handler.

bool CImportDlg::OnOk()
{
	ASSERT(m_rbReplace.IsChecked() || m_rbMerge.IsChecked());

	// Validate controls.
	if (m_ebFileName.TextLength() == 0)
	{
		AlertMsg("Please select a file to import from.");
		m_ebFileName.Focus();
		return false;
	}

	// Save control settings.
	m_strFileName   = m_ebFileName.Text();
	m_eAction       = m_rbReplace.IsChecked() ? REPLACE : MERGE;
	m_bNoDuplicates = m_ckNoDuplicates.IsChecked();

	return true;
}

////////////////////////////////////////////////////////////////////////////////
//! File Browse button handler. Shows the file selection common dialog.

void CImportDlg::OnBrowseClicked()
{
	// File extensions.
	static char szExts[] = {	"Data Files (*.CSV)\0*.CSV\0"
								"All Files (*.*)\0*.*\0"
								"\0\0"							};

	// If set, start in previous folder.
	CPath strDefFolder = CPath::ApplicationDir();

	if (!m_strFileName.Empty())
		strDefFolder = m_strFileName.Directory();

	// Select a filename into the edit box.
	if (m_strFileName.Select(*this, CPath::OpenFile, szExts, "CSV", strDefFolder))
		m_ebFileName.Text(m_strFileName);
}

////////////////////////////////////////////////////////////////////////////////
//! Replace button state handler. Enable/Disable dependent controls.

void CImportDlg::OnReplaceClicked()
{
	m_ckNoDuplicates.Enable(false);
}

////////////////////////////////////////////////////////////////////////////////
//! Merge button state handler. Enable/Disable dependent controls.

void CImportDlg::OnMergeClicked()
{
	m_ckNoDuplicates.Enable(true);
}
