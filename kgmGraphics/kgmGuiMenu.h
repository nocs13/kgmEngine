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
    u32          width;

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

      //rect = iRect(0, 0, 10 * title.length(), ItemHeight);
      width = 10 * title.length();
      rect = iRect(0, 0, 0, 0);
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

      //rect = iRect(0, 0, 10 * title.length(), ItemHeight);
      width = 10 * title.length();
      rect = iRect(0, 0, 0, 0);
    }

    ~Item()
    {
      for(int i = 0; i < items.length(); i++)
        delete items[i];

      items.clear();
    }

    kgmString getTitle() { return title; }

    iRect getRect() { return rect; }

    u32 getId(){ return id; }
    s32 getType(){ return (s32)type; }
    s32 getSelected() { return selected; }
    s32 getItemsCount() { return items.length(); }

    Item* add(kgmString title, kgmTexture* icon = null)
    {
      if(type != TypeMenu || title.length() < 1)
        return null;

      Item* item = new Item(this, title);

      if(vertical)
      {
        if(item->width > rect.w)
          rect.w = item->width;

        rect.h += ItemHeight;
      }
      else
      {
        rect.w += item->width;
      }

      items.add(item);

      return item;
    }

    Item* add(u32 id, kgmString title, kgmTexture* icon = null)
    {
      if(type != TypeMenu || title.length() < 1)
        return null;

      Item* item = new Item(this, id, title);

      if(vertical)
      {
        if(item->width > rect.w)
          rect.w = item->width;

        rect.h += ItemHeight;
      }
      else
      {
        rect.w += item->width;
      }

      items.add(item);

      return item;
    }

    Item* getItem(int i)
    {
      if(i >= 0 && i < items.length())
        return items[i];

      return null;
    }

    iRect getRect(s32 index)
    {
      iRect rc(0, 0, 0, 0);

      if(index >= items.length())
        return rc;

      for(int i = 0; i <= index; i++)
      {
        if(vertical)
        {
          rc.x = rect.x;
          rc.y += ItemHeight;
          rc.w = rect.w;
          rc.h = ItemHeight;
        }
        else
        {
          rc.x += ((i==0)?(0):(items[i-1]->width));
          rc.y = rect.y;
          rc.w = items[i]->width;
          rc.h = ItemHeight;
        }
      }

      return rc;
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
        if(getRect(i).inside(x, y) && selected != i)
        {
          selected = i;

          Item* ci = items[i];
          Rect  rc = getRect(i);

          if(vertical)
          {
            if(ci->vertical)
              ci->setPosition(rc.x + rc.w, rc.y);
          }
          else
          {
            if(ci->vertical)
              ci->setPosition(rc.x, rc.y + rc.h);
          }

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
      if(selected != -1 && getRect(selected).inside(x, y))
      {
        Item* sel = items[selected];

        selected = -1;

        return sel;
      }

      if(selected != -1 && items[selected]->type == TypeMenu)
      {
        Item* sel = items[selected]->clickPointer(x, y);

        selected = -1;

        return sel;
      }

      selected = -1;

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
