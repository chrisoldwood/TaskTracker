////////////////////////////////////////////////////////////////////////////////
//! \author Chris Oldwood
//! \file   REPORTOPTIONS.HPP
//! \brief  The CReportOptions class declaration.

// Check for previous inclusion
#ifndef REPORTOPTIONS_HPP
#define REPORTOPTIONS_HPP

#if _MSC_VER > 1000
#pragma once
#endif

////////////////////////////////////////////////////////////////////////////////
//! The data class used to pass the options when generating a report.

class CReportOptions
{
public:
	//! Default constructor.
	CReportOptions();

	//
	// Members.
	//
	CReportPtr	m_pDevice;			//!< The device to write to.
	Grouping	m_eGrouping;		//!< The aggregation type.
	bool		m_bShowSessions;	//!< Report each session?
	bool		m_bShowIfEmpty;		//!< Report aggregations that are empty?
	bool		m_bShowTotal;		//!< Report the overall total?
};

////////////////////////////////////////////////////////////////////////////////
//! Default constructor.

inline CReportOptions::CReportOptions()
	: m_eGrouping(Ungrouped)
	, m_bShowSessions(true)
	, m_bShowIfEmpty(true)
	, m_bShowTotal(true)
{
}

#endif // REPORTOPTIONS_HPP
