#include "kgmGui.h"

class kgmGuiMenu: public kgmGui
{
  KGM_OBJECT(kgmGuiMenu)

public:

  static u32 ItemHeight;

  class Item
  {
  public:
    enum Type
    {
      TypeItem,
      TypeMenu,
      TypeSeparator
    };

  protected:
    Item*      parent;
    u32        id;
    Type       type;
    kgmString  title;

    kgmGui::Rect rect;

    bool       popup;
    bool       vertical;

    s32        selected;
    f32        xscale, yscale;

    kgmList<Item*> items;

  public:
    Item(Item* par, kgmString text, bool horizontal = false)
    {
      parent = par;

      id    = -1;
      title = text;

      type  = TypeMenu;

      popup = false;
      vertical = !horizontal;

      selected = -1;

      xscale = yscale = 1.0f;

      rect = iRect(0, 0, 10 * title.length(), ItemHeight);

      if(parent)
      {
        if(parent->vertical)
        {
          setPosition(parent->rect.x, parent->rect.y + ItemHeight * parent->items.length());
        }
        else
        {
          setPosition(parent->rect.x + 40 * parent->items.length(), parent->rect.y);
        }
      }
    }

    Item(Item* par, u32 eid, kgmString text, bool horizontal = false)
    {
      parent = par;

      id    = eid;
      title = text;

      type  = TypeItem;

      popup = false;
      vertical = !horizontal;

      selected = -1;

      xscale = yscale = 1.0f;

      rect = iRect(0, 0, 10 * title.length(), ItemHeight);

      if(parent)
      {
        if(parent->vertical)
        {
          setPosition(parent->rect.x, parent->rect.y + ItemHeight * parent->items.length());
        }
        else
        {
          setPosition(parent->rect.x + 40 * parent->items.length(), parent->rect.y);
        }
      }
    }

    ~Item()
    {
      for(int i = 0; i < items.length(); i++)
        delete items[i];

      items.clear();
    }

    kgmString getTitle() { return title; }

    iRect getRect() { return rect; }

    s32 getType(){ return (s32)type; }
    s32 getSelected() { return selected; }
    s32 getItemsCount() { return items.length(); }

    iRect rectContent()
    {
      if(type == TypeItem)
        return rect;

      s32 w = 0, h = 0;

      for(int i = 0; i < items.length(); i++)
      {
        s32 iw = items[i]->getRect().width();
        s32 ih = items[i]->getRect().height();

        if(vertical)
        {
          h += ih;
          w = (iw > w) ? (iw) : (w);
        }
        else
        {
          w += iw;
          h = ih;
        }
      }

      return iRect(rect.x, rect.y, w, h);
    }

    Item* add(kgmString title, kgmTexture* icon = null)
    {
      if(type != TypeMenu || title.length() < 1)
        return null;

      Item* item = new Item(this, title);
      items.add(item);

      return item;
    }

    Item* add(u32 id, kgmString title, kgmTexture* icon = null)
    {
      if(type != TypeMenu || title.length() < 1)
        return null;

      Item* item = new Item(this, id, title);
      items.add(item);

      return item;
    }

    Item* getItem(int i)
    {
      if(i >= 0 && i < items.length())
        return items[i];

      return null;
    }

    void setPosition(int x, int y)
    {
      rect.x = x;
      rect.y = y;
    }

    void movePointer(int x, int y)
    {
      for(u32 i = 0; i < items.length(); i++)
      {
        if(items[i]->getRect().inside(x, y) && selected != i)
        {
          selected = i;

          break;
        }
      }

      if(selected != -1 && selected < items.length())
      {
        items[selected]->movePointer(x, y);
      }
    }

    Item* clickPointer(int x, int y)
    {
      selected = -1;

      if(rect.inside(x, y))
        return this;

      if(selected != -1 && selected < items.length())
      {
        return items[selected]->clickPointer(x, y);
      }

      return null;
    }
  };

protected:
  Item* item;

public:
  kgmGuiMenu(kgmGui* parent);
  ~kgmGuiMenu();

  Item* add(kgmString title);
  Item* add(u32 id, kgmString title);

  void onMsMove(int k, int x, int y);
  void onMsLeftUp(int k, int x, int y);

  Item* getItem() { return item; }
};
