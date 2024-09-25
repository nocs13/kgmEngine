#include "lua_main.h"
#include "../kgmLuaScript.h"
#include "../../kgmGraphics/kgmGuiMenu.h"

namespace kgmLibScript
{
  class luaGuiMenu: public kgmGuiMenu
  {
    //KGM_OBJECT(luaGuiMenu)

    lua_State* lua = null;
    const s8* oname = null;

  public:
    luaGuiMenu(kgmGui* g, const s8* id, const s8* name, lua_State* l)
    :kgmGuiMenu(g)
    {
      setSid(id);

      lua = l;

      oname = name;
    }

    void onMsLeftUp(int key, int x, int y)
    {
      if(!m_view || m_freeze)
        return;

      Item* selected = root->getItemSelected();

      if(selected && !selected->ispopup() && selected->getId() > 0)
      {
        kgmLuaScript ls(lua);

        ls.ccall(oname, "onMenu", "is", selected->getId(), selected->getTitle().data());
      }
    }
  };

  s32 kgmGuiMenuCreate(void* lh)
  {
    if (!lh)
      return 0;

    kgmLuaScript ls((lua_State*) lh);

    kgmGui* gui = null;
    const s8* sid = null;
    const s8* name = null;

    ls.args("pss", &gui, &sid, &name);

    luaGuiMenu* menu = new luaGuiMenu(gui, sid, name, (lua_State*) lh);

    if (menu)
    {
      menu->setRect(0, 0, 300, 20);

      ls.resl("p", menu);

      return 1;
    }

    return 0;
  }

  s32 kgmGuiMenuItemCreate(void* lh)
  {
    if (!lh)
      return 0;

    kgmLuaScript ls((lua_State*) lh);

    kgmGuiMenu* menu = null;
    const s8* sid = null;

    ls.args("ps", &menu, &sid);

    if (menu && sid)
    {
      kgmGuiMenu::Item *item = menu->add(sid);

      ls.resl("p", item);

      return 1;
    }

    return 0;
  }

  s32 kgmGuiMenuItemAdd(void* lh)
  {
    if (!lh)
      return 0;

    kgmLuaScript ls((lua_State*) lh);

    kgmGuiMenu::Item* item = null;
    const s8* sid = null;
    s32 id = 0;

    ls.args("psi", &item, &sid, &id);

    if (item && id)
    {
      item->add(id, sid);

      ls.resl("i", 1);

      return 1;
    }

    return 0;
  }

  s32 kgmGuiShow(void* lh)
  {
    if (!lh)
      return 0;

    kgmLuaScript ls((lua_State*) lh);

    void* p = null;
    kgmGui* g = null;
    s32 s = 0;

    ls.args("pi", &p, &s);

    g = static_cast<kgmGui*>(p);

    if (g)
    {
      if (s)
        g->show();
      else
        g->hide();

      ls.resl("i", 1);

      return 1;
    }

    return 0;
  }
}