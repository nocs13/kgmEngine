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
      setAlpha(true);
      setColor(0x55966804);
      useStyle(false);
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
    class Button: public kgmGuiButton
    {
    public:
      Button(kgmGui* p, int x, int y, int w, int h)
      :kgmGuiButton(p, x, y, w, h)
      {
        useStyle(false);
        setAlpha(true);
        setColor(0xFF966804);
      }

      ~Button()
      {
        if(m_image)
        {
          m_image->release();
        }
      }

      void onMsLeftUp(int key, int x, int y)
      {
        setColor(0xFF966804);

        if(m_parent)
          m_parent->onAction(this, 0);
      }

      void onMsLeftDown(int key, int x, int y)
      {
        setColor(0xff0000ff);

        if(m_parent)
          m_parent->onAction(this, 1);
      }
    };

    kgmIGame*  game;
    Button *btnLeft, *btnRight, *btnUp, *btnShoot;

  public:
    GuiInput(  kgmIGame*  g)
    {
      int cx, cy, cw, ch;

      game = g;

      game->getWindow()->getRect(cx, cy, cw, ch);

      if(ch > cw)
      {
       setRect(cw / 2 - 150, ch - 100, 300, 100);
      }
      else
      {
        setRect(cw - 300, ch - 100, 300, 100);
      }

      btnUp = new Button(this, 0, 0, 300, 40);
      btnLeft = new Button(this, 0, 40, 100, 60);
      btnShoot = new Button(this, 100, 40, 100, 60);
      btnRight = new Button(this, 200, 40, 100, 60);

      btnUp->setImage(game->getResources()->getTexture("ui_up.tga"));
      btnLeft->setImage(game->getResources()->getTexture("ui_left.tga"));
      btnRight->setImage(game->getResources()->getTexture("ui_right.tga"));
      btnShoot->setImage(game->getResources()->getTexture("ui_shoot.tga"));

      //btnUp->hide();
      //btnLeft->hide();
      //btnRight->hide();
      //btnShoot->hide();

      setColor(0x00000000);
      useStyle(false);
      setAlpha(true);
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

    void onAction(kgmGui* src, u32 type)
    {
      kgmEvent::Event evt;

      if(src == btnUp)
      {
        if(type)
        {
          evt.event = evtKeyDown;
          evt.key = KEY_UP;
          game->getWindow()->onEvent(&evt);
        }
        else
        {
          evt.event = evtKeyUp;
          evt.key = KEY_UP;
          game->getWindow()->onEvent(&evt);
        }
      }
      else if(src == btnLeft)
      {
        if(type)
        {
          evt.event = evtKeyDown;
          evt.key = KEY_LEFT;
          game->getWindow()->onEvent(&evt);
        }
        else
        {
          evt.event = evtKeyUp;
          evt.key = KEY_LEFT;
          game->getWindow()->onEvent(&evt);
        }
      }
      else if(src == btnRight)
      {
        if(type)
        {
          evt.event = evtKeyDown;
          evt.key = KEY_RIGHT;
          game->getWindow()->onEvent(&evt);
        }
        else
        {
          evt.event = evtKeyUp;
          evt.key = KEY_RIGHT;
          game->getWindow()->onEvent(&evt);
        }
      }
      else if(src == btnShoot)
      {
        if(type)
        {
          evt.event = evtKeyDown;
          evt.key = KEY_X;
          game->getWindow()->onEvent(&evt);
        }
        else
        {
          evt.event = evtKeyUp;
          evt.key = KEY_X;
          game->getWindow()->onEvent(&evt);
        }
      }
    }

    void update()
    {

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
    gui_input->update();
  }
};

#endif // ASP_GUI_H
