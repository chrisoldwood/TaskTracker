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
** NB: This class used to derive from CPtrList.
**
*******************************************************************************
*/

class CSessionList : public std::list<CSession*>
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
	void Add(CSession* pSession);
	void Remove(CSession* pSession);
	void RemoveAll();

	int IndexOf(const CSession* pSession) const;

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
** Predicate class used to determine if a session is in the given date range.
**
*******************************************************************************
*/

class CIsSessionInRange : public std::unary_function<const CSession*, bool>
{
public:
	CIsSessionInRange(const CDateTime& dtStart, const CDateTime& dtEnd);

	bool operator()(const CSession* pSession);

private:
	//
	// Members.
	//
	CDateTime	m_dtStart;		// Start date.
	CDateTime	m_dtEnd;		// End date.
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

inline CIsSessionInRange::CIsSessionInRange(const CDateTime& dtStart, const CDateTime& dtEnd)
	: m_dtStart(dtStart)
	, m_dtEnd(dtEnd)
{
}

inline bool CIsSessionInRange::operator()(const CSession* pSession)
{
	ASSERT(pSession != NULL);

	return ( (pSession->Start() >= m_dtStart) && (pSession->Start() < m_dtEnd) );
}

#endif //SESSLIST_HPP
