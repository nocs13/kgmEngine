#include <qdatastream.h>
#include <qtextstream.h>
#include <qmetatype.h>

#include <QApplication>
#include <QtWidgets>
#include <QOpenGLWindow>

#include <X11/Xlib.h>

/*
#include "kApp.h"
#include "../kgmBase/kgmLog.h"
#include "../kgmSystem/kgmSystem.h"
#include "../kgmScript/kgmLuaScript.h"
#include "../kgmScript/libscript/lua_main.h"
#include "../kgmGame/kgmGameResources.h"
#include "libscript/lua_main.h"


kApp::kApp()
{
  editor = null;
}

kApp::~kApp()
{
}

s32 kApp::exec(s32, s8**)
{
  fprintf(stderr, "XXX.\n");
  fflush(stderr);

  kgmLuaScript* script = new kgmLuaScript();

  kgmLibScript::kgm_lua_init(script->getCore());
  Editor::lua_init(script->getCore());


  kgmIResources* res = kgmApp::application()->getResources();

  kgmCString path = "scripts/main.lua";

  kgmArray<u8> m;

  if (res->getFile(path, m))
  {
    kgmCString sc((char*) m.data(), m.length());

    //fprintf(stderr, "{[%s]}", sc.data());

    if (script->load(sc))
    {
      script->call("main", "");
    }
    else
    {
      kgm_log() << "Error: unable load " << (char*) path << " script.\n";
    }
  }

  script->release();

  return 0;
}
*/
class kWindow: public QOpenGLWindow
{
  //Q_OBJECT
public:
  kWindow()
  {
    fprintf(stderr, "XXX ccc\n");

    void* w = (void*) winId();

    fprintf(stderr, "XXX ccc %p\n", w);
  }
  virtual ~kWindow()
  {

  }
};

int main(int argc, char** argv)
{
  //kApp app;

  //return  app.exec(argc, argv);

  QApplication a(argc, argv);

  kWindow window;
  window.resize(320, 240);
  window.show();

  return a.exec();
}
