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

class CSessionList : public std::list<CSessionPtr>
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
	void Add(CSessionPtr& pSession);
	void Remove(CSessionPtr& pSession);
	void RemoveAll();

	int IndexOf(const CSessionPtr& pSession) const;

private:
	//
	// File I/O.
	//
	friend void operator >>(CStream& rStream, CSessionList& oList);
	friend void operator <<(CStream& rStream, const CSessionList& oList);
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

	bool operator()(const CSessionPtr& pSession);

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

inline bool CIsSessionInRange::operator()(const CSessionPtr& pSession)
{
	ASSERT(pSession.Get() != nullptr);

	return ( (pSession->Start() >= m_dtStart) && (pSession->Start() < m_dtEnd) );
}

#endif //SESSLIST_HPP