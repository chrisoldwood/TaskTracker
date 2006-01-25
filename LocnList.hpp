/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		LOCNLIST.HPP
** COMPONENT:	The Application.
** DESCRIPTION:	The CLocnList class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef LOCNLIST_HPP
#define LOCNLIST_HPP

/******************************************************************************
** 
** This class is used to hold a list of locations previously used.
**
** NB: This class used to derive from CPtrList.
**
*******************************************************************************
*/

class CLocnList : public std::list<CString>
{
public:
	//
	// Constructors/Destructor.
	//
	CLocnList();
	~CLocnList();
	
	//
	// Methods.
	//
	void Add(const char* pszLocn);
	void Remove(const char* pszLocn);
	void RemoveAll();

private:
	//
	// File I/O.
	//
	friend void operator >>(CStream& rStream, CLocnList& oList);
	friend void operator <<(CStream& rStream, const CLocnList& oList);
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

#endif //LOCNLIST_HPP
