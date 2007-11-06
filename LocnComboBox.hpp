/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		LCNCOMBO.HPP
** COMPONENT:	The Application.
** DESCRIPTION:	The CLocnCombo class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef LCNCOMBO_HPP
#define LCNCOMBO_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include <WCL/ComboBox.hpp>

/******************************************************************************
** 
** This is a combo box used to display the list of previous locations.
**
*******************************************************************************
*/

class CLocnCombo : public CComboBox
{
public:
	//
	// Constructors/Destructor.
	//
	CLocnCombo();
	~CLocnCombo();
	
protected:
	//
	// Members.
	//

	//
	// Message processors.
	//
	virtual void OnCreate(const CRect& rcClient);
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

#endif //LCNCOMBO_HPP
