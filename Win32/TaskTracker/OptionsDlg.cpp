////////////////////////////////////////////////////////////////////////////////
//! \author Chris Oldwood
//! \file   OPTIONSDLG.CPP
//! \brief  The COptionsDlg class definition.

#include "AppHeaders.hpp"
#include "OptionsDlg.hpp"

#ifdef _DEBUG
// For memory leak detection.
#define new DBGCRT_NEW
#endif

////////////////////////////////////////////////////////////////////////////////
//! Default constructor.

COptionsDlg::COptionsDlg()
	: CDialog(IDD_OPTIONS)
{
	DEFINE_CTRL_TABLE
		CTRL(IDC_MIN_TO_TRAY, &m_ckMinToTray)
		CTRL(IDC_LEN_FORMAT,  &m_cbLenFormat)
	END_CTRL_TABLE
}

////////////////////////////////////////////////////////////////////////////////
//! Dialog initialisation handler.

void COptionsDlg::OnInitDialog()
{
	// Populate length formats combo.
	m_cbLenFormat.Add("Hours & Minutes");
	m_cbLenFormat.Add("Hours Only");

	// Initialise controls.
	m_ckMinToTray.Check(App.m_bMinToTray);
	m_cbLenFormat.CurSel(App.m_eLenFormat);
}

////////////////////////////////////////////////////////////////////////////////
//! OK button handler.

bool COptionsDlg::OnOk()
{
	// Save new settings.
	App.m_bMinToTray = m_ckMinToTray.IsChecked();
	App.m_eLenFormat = static_cast<LenFormat>(m_cbLenFormat.CurSel());

	return true;
}
