////////////////////////////////////////////////////////////////////////////////
//! \file   EXPORTDLG.CPP
//! \brief  The CExportDlg class definition.
//! \author Chris Oldwood

#include "Common.hpp"
#include "ExportDlg.hpp"
#include "TaskTracker.hpp"

////////////////////////////////////////////////////////////////////////////////
//! Default constructor.

CExportDlg::CExportDlg()
	: CDialog(IDD_EXPORT)
	, m_strFileName(App.m_strExportFile)
{
	DEFINE_CTRL_TABLE
		CTRL(IDC_FILENAME,	&m_ebFileName)
		CTRL(IDC_FROM_DATE,	&m_dtpFromDate)
		CTRL(IDC_TO_DATE, 	&m_dtpToDate)
	END_CTRL_TABLE

	DEFINE_CTRLMSG_TABLE
		CMD_CTRLMSG(IDC_BROWSE, BN_CLICKED, &CExportDlg::OnBrowse)
	END_CTRLMSG_TABLE
}

////////////////////////////////////////////////////////////////////////////////
//! Dialog initialisation handler.

void CExportDlg::OnInitDialog()
{
	ASSERT(!App.m_oSessionList.empty());

	// Initialise controls.
	if (!m_strFileName.Empty())
		m_ebFileName.Text(m_strFileName);

	m_dtpFromDate.Format(App.DatePickerFormat());
	m_dtpToDate.Format(App.DatePickerFormat());

	// Set dates to database limits.
	m_dtpFromDate.SetDate(App.m_oSessionList.front()->Start().Date());
	m_dtpToDate.SetDate(App.m_oSessionList.back()->Start().Date());
}

////////////////////////////////////////////////////////////////////////////////
//! OK button handler.

bool CExportDlg::OnOk()
{
	// Validate controls.
	if (m_ebFileName.TextLength() == 0)
	{
		AlertMsg(TXT("Please select a file name to export to."));
		m_ebFileName.Focus();
		return false;
	}

	if (m_dtpToDate.GetDate() < m_dtpFromDate.GetDate())
	{
		AlertMsg(TXT("The 'To' date must be later than the 'From' date."));
		m_dtpToDate.Focus();
		return false;
	}

	// Save control settings.
	m_strFileName = m_ebFileName.Text();
	m_dFromDate   = m_dtpFromDate.GetDate();
	m_dToDate     = m_dtpToDate.GetDate();

	return true;
}

////////////////////////////////////////////////////////////////////////////////
//! File Browse button handler. Shows the file selection common dialog.

void CExportDlg::OnBrowse()
{
	// File extensions.
	static tchar szExts[] = {	TXT("Data Files (*.CSV)\0*.CSV\0")
								TXT("All Files (*.*)\0*.*\0")
								TXT("\0\0")							};

	// If set, start in previous folder.
	CPath strDefFolder = CPath::ApplicationDir();

	if (!m_strFileName.Empty())
		strDefFolder = m_strFileName.Directory();

	// Select a filename into the edit box.
	if (m_strFileName.Select(*this, CPath::SaveFile, szExts, TXT("CSV"), strDefFolder))
		m_ebFileName.Text(m_strFileName);
}
