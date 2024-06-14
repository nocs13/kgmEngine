#include "kWindow.h"

/*
kWindow::kWindow(GtkWidget* w)
{
  m_gw = w;
}
*/

kWindow::kWindow(Display* dpy, Window wnd)
{
  m_dpy = dpy;
  m_wnd = wnd;
}