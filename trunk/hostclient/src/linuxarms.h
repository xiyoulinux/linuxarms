#ifndef _LINUXARMS_H
#define _LINUXARMS_H

#define _HOST_CLIENT_

#if (TRUE == 1) && (FALSE == 0)
#include <gtk/gtk.h>
typedef gboolean boolean;
#else
typedef enum _boolean {
	FALSE = 0,
	TRUE
} boolean;
#endif
struct mwindow_struct;
struct hmthread_struct;
struct hfthread_struct;
struct hsthread_struct;
struct hcthread_struct;

struct linuxarms_struct {
	struct mwindow_struct  *mwindow;
	struct hmthread_struct *hmthread;
	struct hfthread_struct *hfthread;
	struct hsthread_struct *hsthread;
	struct hcthread_struct *hcthread;
};
#endif
