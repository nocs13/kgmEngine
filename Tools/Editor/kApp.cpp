#include "kApp.h"
#include "../../kgmBase/kgmLog.h"
#include "../../kgmSystem/kgmSystem.h"

#include <gtk/gtk.h>

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
    u32 w, h;
    kgmSystem::getDesktopDimension(w, h);

    editor = new kEditor();

    m_game = static_cast<kgmIGame*> (editor);

    setMainWindow(editor);

    if (editor && !editor->gInit())
    {
      failed = true;
    }

    editor->init();
}

void kApp::gameLoop()
{
    if(editor != null && failed != true)
    {
      editor->loop();
    }
}

void kApp::gameFree()
{
    if(editor)
    {
      editor->release();

      m_game = editor = null;
    }
}

void kApp::main()
{
  kgm_log() << "Start\n";

  editor = new kEditor();
  m_game = editor;

  editor->loop();
}

static void activate(GtkApplication* app)
{
  GtkWidget* w;

  u32 dw, dh;

  kgmSystem::getDesktopDimension(dw, dh);

  w = gtk_application_window_new(app);
  gtk_window_set_title (GTK_WINDOW (w), "kEditor");
  gtk_window_set_default_size (GTK_WINDOW (w), dw, dh);

  gtk_window_present (GTK_WINDOW (w));

  kEditor* e = new kEditor(w);
}

int main(int argc, char** argv)
{
  //kApp app;
  //int res = app.exec(argc, argv);

  GtkApplication *app;
  int status;

  app = gtk_application_new ("org.gtk.example", G_APPLICATION_FLAGS_NONE);
  g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
  status = g_application_run (G_APPLICATION (app), argc, argv);
  g_object_unref (app);

  //gtk_init(&argc, &argv);

  //printf("GTK+ version: %d.%d.%d\n", gtk_major_version,
  //          gtk_minor_version, gtk_micro_version);
  //printf("Glib version: %d.%d.%d\n", glib_major_version,
  //          glib_minor_version, glib_micro_version);

  return 0;
}

