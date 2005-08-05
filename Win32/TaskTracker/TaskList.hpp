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
** NB: This class used to derive from CPtrList.
**
*******************************************************************************
*/

class CTaskList : public std::list<CString>
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
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

#endif //TASKLIST_HPP
