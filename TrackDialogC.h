#pragma once
#include "modelessdialogtracker.h"

class CDIODlg;

class TrackDialogC :
	public ModelessDialogTracker
{
public:
	TrackDialogC(CDIODlg& d);
	virtual ~TrackDialogC(void);

	virtual void OnDialogClosed();	// override if you want to do something when closed.
private:
	CDIODlg& mainDlg;

};
