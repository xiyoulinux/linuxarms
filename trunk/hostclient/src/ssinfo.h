#ifndef _SSINFO_H
#define _SSINFO_H
#include <gtk/gtk.h>
#include "linuxarms.h"
#include "hnet.h"

struct hssinfo_struct {
	struct hnet_struct *socket;
};

boolean hssinfo_init(struct hssinfo_struct *hssinfo, struct hnet_struct *socket);
GtkWidget *create_page_ssinfo(struct linuxarms_struct *linuxarms);

#endif
