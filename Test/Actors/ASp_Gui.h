#ifndef ASP_GUI_H
#define ASP_GUI_H

class ASp_Gui: public kgmGui
{
  KGM_OBJECT(ASp_Gui);

  class GuiHealth: public kgmGui
  {
    kgmIGame* game;

    kgmGui* gui_background;
    kgmGui* gui_midlground;
    kgmGui* gui_foreground;

    kgmActor* actor;
  public:
    GuiHealth(kgmIGame* g)
    {
      game = g;

      gui_midlground = new kgmGui();
      gui_foreground = new kgmGui();

      gui_midlground->setParent(this);
      gui_foreground->setParent(this);

      gui_midlground->setRect(0, 0, 200, 10);
      gui_foreground->setRect(0, 0, 200, 10);

      setRect(0, 0, 200, 10);
      show();

      actor = null;
    }

    void set(kgmActor* a)
    {
      actor = a;
    }

    void update()
    {

    }
  };

  class GuiMap: public kgmGui
  {
    struct GuiActor
    {
      kgmGui*    gui;
      kgmActor*  actor;
    };

    kgmIGame*           game;
    kgmGui*             gui_back;
    kgmList<GuiActor>   gui_actors;
  public:

    GuiMap(kgmIGame* g)
    {
      game     = g;
      gui_back = new kgmGui();

      gui_back->setParent(this);

      setRect(0, 100, 300, 300);
      show();
    }

    virtual ~GuiMap()
    {
      gui_actors.clear();
    }

    void add(kgmActor* a)
    {
      GuiActor ga;

      ga.actor = a;
      ga.gui   = new kgmGui();
      ga.gui->setParent(this);
      ga.gui->useStyle(false);

      if(a->m_gameplayer)
        ga.gui->setColor(0xffffffff);
      else
        ga.gui->setColor(0xff0000ff);

      ga.gui->show();
      gui_actors.add(ga);
    }

    void update()
    {
      return;
      for(kgmList<GuiActor>::iterator i = gui_actors.begin();
          i != gui_actors.end(); ++i)
      {
        if(!kgmObject::isValid((*i).actor) || !(*i).actor->valid())
        {
          (*i).gui->release();
          i = gui_actors.erase(i);

          continue;
        }
        else if((*i).actor->valid() && !(*i).actor->removed())
        {
          vec3 pos;

          if((*i).actor->getBody())
            pos = (*i).actor->getBody()->position();

          u32  x = pos.x / 35;
          u32  y = pos.y / 35;

          (*i).gui->setRect(150 + x, 150 - y, 5, 5);
        }
      }
    }
  };

  kgmIGame*  game;

  GuiHealth* gui_health;
  GuiMap*    gui_map;

public:
  ASp_Gui(kgmIGame* g)
  {
    game = g;

    gui_health = new GuiHealth(g);
    gui_map    = new GuiMap(g);

    gui_health->setParent(this);
    gui_map->setParent(this);

    setRect(0, 0, 1, 1);
    show();
  }

  void add(kgmActor* a)
  {
    gui_map->add(a);
  }

  void set(kgmActor* a)
  {

  }

  void update()
  {
    gui_health->update();
    gui_map->update();
  }
};

#endif // ASP_GUI_H
