////////////////////////////////////////////////////////////////////////////////
//! \file   OPTIONSDLG.CPP
//! \brief  The COptionsDlg class definition.
//! \author Chris Oldwood

#include "Common.hpp"
#include "OptionsDlg.hpp"
#include "TaskTracker.hpp"

////////////////////////////////////////////////////////////////////////////////
//! Default constructor.

COptionsDlg::COptionsDlg()
	: CDialog(IDD_OPTIONS)
{
	DEFINE_CTRL_TABLE
		CTRL(IDC_MIN_TO_TRAY, &m_ckMinToTray)
		CTRL(IDC_OVERLAP,     &m_ckOverlap)
		CTRL(IDC_LEN_FORMAT,  &m_cbLenFormat)
		CTRL(IDC_WEEK_ORDER,  &m_cbWeekOrder)
	END_CTRL_TABLE
}

////////////////////////////////////////////////////////////////////////////////
//! Dialog initialisation handler.

void COptionsDlg::OnInitDialog()
{
	// Populate length formats combo.
	m_cbLenFormat.Add(TXT("Hours & Minutes"));
	m_cbLenFormat.Add(TXT("Hours Only"));

	// Populate week order combo.
	m_cbWeekOrder.Add(CDate::DayOfWeekName(5) + TXT(" - ") + CDate::DayOfWeekName(4));
	m_cbWeekOrder.Add(CDate::DayOfWeekName(6) + TXT(" - ") + CDate::DayOfWeekName(5));
	m_cbWeekOrder.Add(CDate::DayOfWeekName(0) + TXT(" - ") + CDate::DayOfWeekName(6));

	// Initialise controls.
	m_ckMinToTray.Check(App.m_bMinToTray);
	m_ckOverlap.Check(App.m_bCheckOverlap);
	m_cbLenFormat.CurSel(App.m_eLenFormat);
	m_cbWeekOrder.CurSel(App.m_eWeekOrder);
}

////////////////////////////////////////////////////////////////////////////////
//! OK button handler.

bool COptionsDlg::OnOk()
{
	// Save new settings.
	App.m_bMinToTray    = m_ckMinToTray.IsChecked();
	App.m_bCheckOverlap = m_ckOverlap.IsChecked();
	App.m_eLenFormat    = static_cast<LenFormat>(m_cbLenFormat.CurSel());
	App.m_eWeekOrder    = static_cast<WeekOrder>(m_cbWeekOrder.CurSel());

	return true;
}
