/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		SESSION.HPP
** COMPONENT:	The Application.
** DESCRIPTION:	The CSession class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef SESSION_HPP
#define SESSION_HPP

/******************************************************************************
** 
** This class represents a period of time spent on a task.
**
*******************************************************************************
*/

class CSession
{
public:
	//
	// Constructors/Destructor.
	//
	CSession();
	~CSession();

	//
	// Member access.
	//
	const CDateTime& Start() const;
	const CDateTime& Finish() const;
	const CString&   Task() const;
	const CString&   Location() const;
	ulong Length() const;

	//
	// Member setting.
	//
	void Start (const CDateTime& dtStart,  const char* pszTask, const char* pszLocn);
	void Finish(const CDateTime& dtFinish, const char* pszTask, const char* pszLocn);
	
protected:
	//
	// Members.
	//
	CDateTime	m_dtStart;		// Clock In time.
	CDateTime	m_dtFinish;		// Clock Out time.
	CString		m_strTask;		// Task.
	CString		m_strLocn;		// Location.

	//
	// File I/O.
	//
	friend void operator >>(CStream& rStream, CSession& oSession);
	friend void operator <<(CStream& rStream, const CSession& oSession);
};

// Smart pointer type.
typedef Core::SharedPtr<CSession> CSessionPtr;

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

inline const CDateTime& CSession::Start() const
{
	return m_dtStart;
}

inline const CDateTime& CSession::Finish() const
{
	return m_dtFinish;
}

inline const CString& CSession::Task() const
{
	return m_strTask;
}

inline const CString& CSession::Location() const
{
	return m_strLocn;
}

inline void CSession::Start(const CDateTime& dtStart, const char* pszTask, const char* pszLocn)
{
	m_dtStart = dtStart;
	m_strTask = pszTask;
	m_strLocn  = pszLocn;
}

inline void CSession::Finish(const CDateTime& dtFinish, const char* pszTask, const char* pszLocn)
{
	m_dtFinish = dtFinish;
	m_strTask  = pszTask;
	m_strLocn  = pszLocn;
}

#endif //SESSION_HPP