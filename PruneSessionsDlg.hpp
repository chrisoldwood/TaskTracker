/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		PRSESDLG.HPP
** COMPONENT:	The Application.
** DESCRIPTION:	The CPruneSessionsDlg class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef PRSESDLG_HPP
#define PRSESDLG_HPP

/******************************************************************************
** 
** This dialog is used when the user prunes sessions.
**
*******************************************************************************
*/

class CPruneSessionsDlg : public CDialog
{
public:
	//
	// Constructors/Destructor.
	//
	CPruneSessionsDlg();
	
	//
	// Members.
	//
	CDate	m_Date;

protected:
	//
	// Members.
	//
	CRadioBtn		m_rbAll;
	CRadioBtn		m_rbBefore;
	CDateTimePicker	m_dtpDate;
	
	//
	// Message processors.
	//
	virtual void OnInitDialog();
	virtual bool OnOk();

	//
	// Control message processors.
	//
	void OnAll();
	void OnBefore();
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

#endif //PRSESDLG_HPP
