////////////////////////////////////////////////////////////////////////////////
//! \author Chris Oldwood
//! \file   APPTYPES.HPP
//! \brief  The primitive application types.

// Check for previous inclusion
#ifndef APPTYPES_HPP
#define APPTYPES_HPP

#if _MSC_VER > 1000
#pragma once
#endif

////////////////////////////////////////////////////////////////////////////////
//! The reporting aggregation types.

typedef enum tagGrouping
{
	Ungrouped,
	ByWeek,
	ByMonth,
	ByTask
} Grouping;

////////////////////////////////////////////////////////////////////////////////
//! The reporting pre-defined periods.

typedef enum tagPeriod
{
	All,
	ThisWeek,
	ThisMonth,
	LastWeek,
	LastMonth,
	Custom
} Period;

////////////////////////////////////////////////////////////////////////////////
//! The formats used for converting 'n' minutes to a string.

typedef enum tagLenFormat
{
	HoursMins,
	HoursOnly,
} LenFormat;

////////////////////////////////////////////////////////////////////////////////
//! The day ordering within a week.

typedef enum tagWeekOrder
{
	SatToFri,
	SunToSat,
	MonToSun,
} WeekOrder;

////////////////////////////////////////////////////////////////////////////////
//! The type of file errors reported.

typedef enum tagFileErr
{
	OpenErr,
	CreateErr,
	ReadOnlyErr,
	ReadErr,
	WriteErr,
	FormatErr,
	VersionErr
} FileErr;

#endif // APPTYPES_HPP
