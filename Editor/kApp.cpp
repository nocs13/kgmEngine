#include "kApp.h"
#include "../../kgmBase/kgmLog.h"
#include "../../kgmSystem/kgmSystem.h"

#include <gtk/gtk.h>
#include <gdk/gdkx.h>

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Gl_Window.h>
#include <FL/Fl_Menu_Button.H>
#include <FL/gl.h>
#include <FL/x.H>

kApp::kApp()
{
  m_game = null;
  editor = null;
}

kApp::~kApp()
{
}

void kApp::gameInit()
{
}

void kApp::gameLoop()
{
}

void kApp::gameFree()
{
}

void kApp::main()
{
}

static gint timer = 0;
static kEditor *e = null;

static gint update(gpointer data)
{
  if (e)
    e->onIdle();

  // fprintf(stderr, "[%d/%p] update per 50ms\n", getpid(), pthread_self());

  return 1; // NOTE: Needs to be non-zero otherwise timer seems to be stopped.
}

static void activate(GtkApplication *app)
{
  /*
  GtkWidget* wtk;

  u32 dw, dh;

  kgmSystem::getDesktopDimension(dw, dh);

  wtk = gtk_application_window_new(app);
  gtk_window_set_title (GTK_WINDOW (wtk), "kEditor");
  gtk_window_set_default_size (GTK_WINDOW (wtk), dw, dh);

  gtk_window_present (GTK_WINDOW (wtk));

  GdkDisplay* dpy = gdk_window_get_display(gtk_widget_get_window(wtk));

  kWindow* w = new kWindow(gdk_x11_display_get_xdisplay(dpy), gdk_x11_window_get_xid(gtk_widget_get_window(wtk)));

  e = new kEditor(w);

  timer = g_timeout_add (50, update, NULL);
  fprintf(stderr, "[%d/%p] generic timer %d\n", getpid(), pthread_self(), timer);
  */
}

static void destroy()
{
  // g_source_remove(timer);
}

int v_widht = 1024;
int v_height = 768;

class GL_Window : public Fl_Gl_Window
{
  kEditor *e = null;
  kWindow *w = null;

  Fl_Menu_Button *popup = null;

  static GL_Window* wnd;
public:
  GL_Window(int X, int Y, int W, int H, const char *L)
      : Fl_Gl_Window(X, Y, W, H, L)
      {
        GL_Window::wnd = this;
      }
  virtual ~GL_Window()
  {
    if (e)
      e->release();
    if (w)
      w->release();
  }
  void resize(int x, int y, int w, int h)
  {
    kgm_log() << "Income event resize.\n";

    if (e)
    {
      e->resize(w - x, h - y);
    }

    damage();
  }
  // void 	show (){}
  // void 	hide (){}
  // void 	flush (){}

  int handle(int evt)
  {
    kgm_log() << "Income event " << evt << ".\n";

    if (evt == FL_SHOW)
    {
      kgm_log() << "Income event show.\n";

      if (e == null && context() != nullptr)
      {
        w = new kWindow(fl_display, fl_xid(this));
        e = new kEditor(w, context());

        e->resize(v_widht, v_height);
      }

      if (popup == null)
      {
        popup = new Fl_Menu_Button(30, 30, 100, 300);
        popup->type(Fl_Menu_Button::POPUP3);
        popup->user_data(this);
        popup->add("Map/New", "",  menu_event, (void*) kEditor::ME_MAP_NEW, 0);
        popup->add("Map/Open", "", menu_event, (void*) kEditor::ME_MAP_OPEN, 0);
        popup->add("Map/Save", "", menu_event, (void*) kEditor::ME_MAP_SAVE, 0);
        popup->add("Map/Quit", "", menu_event, (void*) kEditor::ME_QUIT, 0);
        popup->add("Edit/Clone", "", menu_event, (void*) kEditor::ME_EDIT_CLONE, 0);
        popup->add("Edit/Remove", "", menu_event, (void*) kEditor::ME_EDIT_REMOVE, 0);
        popup->add("Edit/Options", "", menu_event, (void*) kEditor::ME_EDIT_OPTIONS, 0);
        popup->add("Help/About", "", menu_event, (void*) kEditor::ME_HELP_ABOUT, 0);
      }
    }
    else if (evt == FL_ACTIVATE)
    {
      kgm_log() << "Income event activate.\n";

      if (Fl::event_buttons() == FL_BUTTON3)
      {
        popup->popup();
      }
    }
    else if (evt == FL_CLOSE)
    {
      kgm_log() << "Income event close.\n";

      if (e)
        e->release();
      if (w)
        w->release();

      e = null;
      w = null;
    }
    else if (evt == FL_FOCUS)
    {
      kgm_log() << "Income event focus.\n";

      if (e == null && context() != nullptr)
      {
        w = new kWindow(fl_display, fl_xid(this));
        e = new kEditor(w, context());

        e->resize(v_widht, v_height);
      }
    }
    else if (evt == FL_PUSH)
    {
      kgm_log() << "Income event push.\n";

      if (Fl::event_buttons() == FL_BUTTON3)
      {
        popup->popup();
      }
    }

    return 1;
  }

  void draw()
  {
    kgm_log() << "Income event draw.\n";

    if (e)
      e->onIdle();
  }

public:
  static void menu_event(Fl_Widget *w, void *arg)
  {
    kEditor* e = ((GL_Window*) GL_Window::wnd)->e;

    if (e)
      e->onMenu((u32) (size_t) arg);
  }
};

GL_Window* GL_Window::wnd = null;

int main(int argc, char **argv)
{
  /*
  GtkApplication *app;
  int status;

  app = gtk_application_new ("org.gtk.example", G_APPLICATION_FLAGS_NONE);
  g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
  g_signal_connect (app, "destroy", G_CALLBACK (destroy), NULL);
  status = g_application_run (G_APPLICATION (app), argc, argv);
  g_object_unref (app);
  */
  // Fl_Window *window = new Fl_Gl_Window(340,180);
  GL_Window window(0, 0, v_widht, v_height, "kEditor");
  window.resizable(&window);
  window.end();
  window.show(argc, argv);

  return Fl::run();
}
