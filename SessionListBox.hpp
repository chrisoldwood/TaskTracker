/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		SESSNLB.HPP
** COMPONENT:	The Application.
** DESCRIPTION:	The CSessionListBox class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef SESSNLB_HPP
#define SESSNLB_HPP

/******************************************************************************
** 
** This is a list box used to display the list of sessions.
**
*******************************************************************************
*/

class CSessionListBox : public CListBox
{
public:
	//
	// Constructors/Destructor.
	//
	CSessionListBox();
	~CSessionListBox();
	
	//
	// Methods.
	//
	void Refresh() const;
	CSession* CurrSession() const;

protected:
	//
	// Members.
	//

	//
	// Message processors.
	//
	virtual void OnCreate(const CRect& rcClient);
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

inline CSession* CSessionListBox::CurrSession() const
{
	return (CSession*) ItemData(CurSel());
}

#endif //SESSNLB_HPP
