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
*******************************************************************************
*/

class CLocnList : public CPtrList
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
	CString* Find(const char* pszLocn) const;
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

#endif //LOCNLIST_HPP
