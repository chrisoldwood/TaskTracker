/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		TASKLIST.HPP
** COMPONENT:	The Application.
** DESCRIPTION:	The CTaskList class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef TASKLIST_HPP
#define TASKLIST_HPP

/******************************************************************************
** 
** This class is used to hold a list of tasks previously used.
**
*******************************************************************************
*/

class CTaskList : public CPtrList
{
public:
	//
	// Constructors/Destructor.
	//
	CTaskList();
	~CTaskList();
	
	//
	// Methods.
	//
	void Add(const char* pszTask);
	void Remove(const char* pszTask);
	void RemoveAll();

	//
	// File I/O.
	//
	void operator <<(CStream& rStream);
	void operator >>(CStream& rStream) const;

protected:
	//
	// Members.
	//

	//
	// Methods.
	//
	CString* Find(const char* pszTask) const;
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

#endif //TASKLIST_HPP
