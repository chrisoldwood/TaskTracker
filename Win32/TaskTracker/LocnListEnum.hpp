/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		LCNLSTEN.HPP
** COMPONENT:	The Application.
** DESCRIPTION:	The CLocnListEnum class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef LCNLSTEN_HPP
#define LCNLSTEN_HPP

/******************************************************************************
** 
** This is the enumerator for the task list.
**
*******************************************************************************
*/

class CLocnListEnum : public CPtrListIter
{
public:
	//
	// Constructors/Destructor.
	//
	CLocnListEnum(const CLocnList& List);

	//
	// Methods.
	//
	CString* Next();
	
protected:
	//
	// Members.
	//
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

inline CLocnListEnum::CLocnListEnum(const CLocnList& List)
	: CPtrListIter(List)
{
}

inline CString* CLocnListEnum::Next()
{
	return (CString*) CPtrListIter::Next();
}

#endif //LCNLSTEN_HPP
