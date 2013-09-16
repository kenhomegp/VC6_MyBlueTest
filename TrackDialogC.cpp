#include "StdAfx.h"
#include "trackdialogc.h"
#include "resource.h"
#include "DIODlg.h"


TrackDialogC::TrackDialogC(CDIODlg& d)
:mainDlg(d)
{
}

TrackDialogC::~TrackDialogC(void)
{
}

void TrackDialogC::OnDialogClosed()
{
	mainDlg.OnUUTClosed();
}
