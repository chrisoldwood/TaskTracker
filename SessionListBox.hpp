/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		SESSNLB.HPP
** COMPONENT:	The Application.
** DESCRIPTION:	The CSessionListView class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef SESSNLB_HPP
#define SESSNLB_HPP

/******************************************************************************
** 
** This is a list view used to display the list of sessions.
** NB: This class used to be based on a CListBox.
**
*******************************************************************************
*/

class CSessionListView : public CListView
{
public:
	//
	// Constructors/Destructor.
	//
	CSessionListView();
	~CSessionListView();
	
	//
	// Methods.
	//
	void Refresh();
	CSession* CurrSession();

protected:
	// Constants.
	enum { NUM_COLUMNS = 7 };

	// Column data.
	static const char*	apszLabels[];
	static const int	aiWidths[];

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

inline CSession* CSessionListView::CurrSession()
{
	return (CSession*) ItemData(Selected());
}

#endif //SESSNLB_HPP
