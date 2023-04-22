#include "kWindow.h"

#include <gdk/gdkx.h>

kWindow::kWindow(GtkWidget* w)
{
  m_gw = w;

  m_wnd = gdk_x11_window_get_xid(gtk_widget_get_window(w));
}