/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		TSKCOMBO.HPP
** COMPONENT:	The Application.
** DESCRIPTION:	The CTaskCombo class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef TSKCOMBO_HPP
#define TSKCOMBO_HPP

/******************************************************************************
** 
** This is a combo box used to display the list of previous tasks.
**
*******************************************************************************
*/

class CTaskCombo : public CComboBox
{
public:
	//
	// Constructors/Destructor.
	//
	CTaskCombo();
	~CTaskCombo();
	
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

#endif //TSKCOMBO_HPP
