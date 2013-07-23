#ifndef ASP_GUI_H
#define ASP_GUI_H

class ASp_Gui: public kgmGui
{
  KGM_OBJECT(ASp_Gui);

  class GuiHealth: public kgmGui
  {
    kgmGui* gui_background;
    kgmGui* gui_midlground;
    kgmGui* gui_foreground;

    s32     health;
  public:
    GuiHealth()
    {
      gui_midlground = new kgmGui();
      gui_foreground = new kgmGui();

      gui_midlground->setParent(this);
      gui_foreground->setParent(this);

      gui_midlground->setRect(0, 0, 200, 10);
      gui_foreground->setRect(0, 0, 200, 10);

      setRect(0, 0, 200, 10);
      show();

      health = 100;
    }

    void setHealth(s32 h)
    {
      health = h;
    }
  };

  class GuiMap: public kgmGui
  {
    struct GuiActor
    {
      kgmGui*    gui;
      kgmActor*  actor;
    };

    kgmGui*             gui_back;
    kgmList<GuiActor*>  gui_actors;
  public:
    GuiMap()
    {
      gui_back = new kgmGui();

      gui_back->setParent(this);

      setRect(0, 100, 300, 300);
      show();
    }

    void add(kgmActor* a)
    {
      GuiActor ga;

      ga.actor = a;
    }
  };

  kgmIGame*  game;

  GuiHealth* gui_health;
  GuiMap*    gui_map;

public:
  ASp_Gui(kgmIGame* g)
  {
    game = g;

    gui_health = new GuiHealth();
    gui_map    = new GuiMap();

    gui_health->setParent(this);
    gui_map->setParent(this);

    setRect(0, 0, 1, 1);
    show();
  }

  void add(kgmActor* a)
  {
    gui_map->add(a);
  }

  void update()
  {

  }
};

#endif // ASP_GUI_H
