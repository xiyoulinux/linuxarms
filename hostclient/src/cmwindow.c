#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gtk/gtk.h>
#include "mwindow.h"
#include "debug.h"
#include "error.h"
#include "hsthread.h"
void cb_notebook_switch_page(GtkNotebook *notebook,
			     GtkNotebookPage *page, 
			     guint page_num, gpointer user_data)
{
	
	debug_print("notebook_switch_page, page num = %d\n",page_num);
}
