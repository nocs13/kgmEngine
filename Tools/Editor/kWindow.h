#include "../../kgmSystem/kgmWindow.h"
#include <gtk/gtk.h>

class kWindow: public kgmWindow
{
  GtkWidget* m_gw;

  public:
  kWindow(GtkWidget*);
};