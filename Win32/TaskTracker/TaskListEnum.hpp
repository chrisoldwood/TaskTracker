/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		TSKLSTEN.HPP
** COMPONENT:	The Application.
** DESCRIPTION:	The CTaskListEnum class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef TSKLSTEN_HPP
#define TSKLSTEN_HPP

/******************************************************************************
** 
** This is the enumerator for the task list.
**
*******************************************************************************
*/

class CTaskListEnum : public CPtrListIter
{
public:
	//
	// Constructors/Destructor.
	//
	CTaskListEnum(const CTaskList& List);

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

inline CTaskListEnum::CTaskListEnum(const CTaskList& List)
	: CPtrListIter(List)
{
}

inline CString* CTaskListEnum::Next()
{
	return (CString*) CPtrListIter::Next();
}

#endif //TSKLSTEN_HPP
