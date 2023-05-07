#include "kWindow.h"

#include <gdk/gdkx.h>

kWindow::kWindow(GtkWidget* w)
{
  m_gw = w;

  GdkDisplay* dpy = gdk_window_get_display(gtk_widget_get_window(w));
  m_dpy = gdk_x11_display_get_xdisplay (dpy);
  m_wnd = gdk_x11_window_get_xid(gtk_widget_get_window(w));
}