#pragma once
#ifndef ASP_GUI_H
#define ASP_GUI_H

class ASp_Gui: public kgmGui
{
  KGM_OBJECT(ASp_Gui);

  class GuiHealth: public kgmGui
  {
    kgmIGame* game;

    kgmGui* gui_foreground;

    kgmActor* actor;
  public:
    GuiHealth(kgmIGame* g)
    {
      game = g;

      gui_foreground = new kgmGui();
      gui_foreground->setParent(this);
      gui_foreground->setRect(0, 0, 200, 10);
      gui_foreground->setColor(0xff0000bb);
      gui_foreground->useStyle(false);
      gui_foreground->show();

      setText("main health");
      setRect(0, 0, 200, 10);
      show();

      actor = null;
    }

    ~GuiHealth()
    {
      delChild(gui_foreground);

      gui_foreground->release();
    }

    void set(kgmActor* a)
    {
      actor = a;
    }

    void update()
    {
      if(actor && kgmObject::isValid(actor))
      {
        gui_foreground->setRect(0, 0, 2 * actor->m_health, 10);
      }
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
    kgmList<GuiActor>   gui_actors;
  public:

    GuiMap(kgmIGame* g)
    {
      game     = g;
      setText("main map");
      setRect(0, 20, 200, 200);
      show();
    }

    virtual ~GuiMap()
    {
      for(kgmList<GuiActor>::iterator i = gui_actors.begin();
          i != gui_actors.end(); i.next())
      {
        delChild((*i).gui);
        (*i).gui->release();
      }

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
      else if(!strcmp("ASp_SpaceshipA", a->runtime().nClass))
        ga.gui->setColor(0xff0000ff);
      else if(!strcmp("ASp_SpacerA", a->runtime().nClass))
        ga.gui->setColor(0xff4169ff);

      ga.gui->show();
      gui_actors.add(ga);
    }

    void update()
    {
      for(kgmList<GuiActor>::iterator i = gui_actors.begin();
          i != gui_actors.end(); ++i)
      {
        if(!kgmObject::isValid((*i).actor) || !(*i).actor->valid())
        {
          delChild((*i).gui);
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

          (*i).gui->setRect(100 + x, 100 - y, 4, 4);
        }
      }
    }
  };

  class GuiInput: public kgmGui
  {
    kgmIGame*  game;
    kgmGuiButton *btnLeft, *btnRight, *btnUp, *btnShoot;

  public:
    GuiInput(  kgmIGame*  g)
    {
      game = g;
      setRect(200, 200, 300, 100);

      btnUp = new kgmGuiButton(this, 0, 0, 300, 40);
      btnLeft = new kgmGuiButton(this, 0, 40, 100, 60);
      btnRight = new kgmGuiButton(this, 100, 40, 100, 60);
      btnShoot = new kgmGuiButton(this, 200, 40, 100, 60);

      show();
    }

    virtual ~GuiInput()
    {
      delChild(btnUp);
      delChild(btnLeft);
      delChild(btnRight);
      delChild(btnShoot);

      btnUp->release();
      btnLeft->release();
      btnRight->release();
      btnShoot->release();
    }
  };

  kgmIGame*  game;

  GuiInput*  gui_input;
  GuiHealth* gui_health;
  GuiMap*    gui_map;

public:
  ASp_Gui(kgmIGame* g)
  {
    game = g;

    gui_health = new GuiHealth(g);
    gui_map    = new GuiMap(g);
    gui_input  = new GuiInput(g);

    gui_health->setParent(this);
    gui_map->setParent(this);
    gui_input->setParent(this);

    setText("main gui");
    setRect(0, 0, 1, 1);
    show();
  }

  ~ASp_Gui()
  {
    delChild(gui_health);
    delChild(gui_map);
    delChild(gui_input);

    gui_health->release();
    gui_map->release();
    gui_input->release();
  }

  void add(kgmActor* a)
  {
    gui_map->add(a);
  }

  void set(kgmActor* a)
  {
    gui_health->set(a);
  }

  void update()
  {
    gui_health->update();
    gui_map->update();
  }
};

#endif // ASP_GUI_H
