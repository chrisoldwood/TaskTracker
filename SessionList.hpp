/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		SESSLIST.HPP
** COMPONENT:	The Application.
** DESCRIPTION:	The CSessionList class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef SESSLIST_HPP
#define SESSLIST_HPP

/******************************************************************************
** 
** This class is used to hold the list of sessions.
**
*******************************************************************************
*/

class CSessionList : public CPtrList
{
public:
	//
	// Constructors/Destructor.
	//
	CSessionList();
	~CSessionList();
	
	//
	// Methods.
	//
	int  Add(CSession* pSession);
	int  Remove(CSession* pSession);
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
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

#endif //SESSLIST_HPP
