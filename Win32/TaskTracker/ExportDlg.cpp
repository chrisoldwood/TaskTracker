////////////////////////////////////////////////////////////////////////////////
//! \author Chris Oldwood
//! \file   EXPORTDLG.CPP
//! \brief  The CExportDlg class definition.

#include "AppHeaders.hpp"
#include "ExportDlg.hpp"

#ifdef _DEBUG
// For memory leak detection.
#define new DBGCRT_NEW
#endif

////////////////////////////////////////////////////////////////////////////////
//! Default constructor.

CExportDlg::CExportDlg()
	: CDialog(IDD_EXPORT)
	, m_strFileName(App.m_strExportFile)
{
	DEFINE_CTRL_TABLE
		CTRL(IDC_FILENAME, &m_ebFileName)
	END_CTRL_TABLE

	DEFINE_CTRLMSG_TABLE
		CMD_CTRLMSG(IDC_BROWSE, BN_CLICKED, OnBrowse)
	END_CTRLMSG_TABLE
}

////////////////////////////////////////////////////////////////////////////////
//! Dialog initialisation handler.

void CExportDlg::OnInitDialog()
{
	// Initialise controls.
	if (!m_strFileName.Empty())
		m_ebFileName.Text(m_strFileName);
}

////////////////////////////////////////////////////////////////////////////////
//! OK button handler.

bool CExportDlg::OnOk()
{
	// Validate controls.
	if (m_ebFileName.TextLength() == 0)
	{
		AlertMsg("Please select a file name to export to.");
		m_ebFileName.Focus();
		return false;
	}

	// Save control settings.
	m_strFileName = m_ebFileName.Text();

	return true;
}

////////////////////////////////////////////////////////////////////////////////
//! File Browse button handler. Shows the file selection common dialog.

void CExportDlg::OnBrowse()
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
	if (m_strFileName.Select(*this, CPath::SaveFile, szExts, "CSV", strDefFolder))
		m_ebFileName.Text(m_strFileName);
}
