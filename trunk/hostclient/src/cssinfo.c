#include <gtk/gtk.h>
#include "linuxarms.h"
#include "ssinfo.h"

boolean hssinfo_init(struct hssinfo_struct *hssinfo, struct hnet_struct *socket)
{
	LINUXARMS_POINTER(hssinfo);
	hssinfo->socket = socket;
	return TRUE;
}
