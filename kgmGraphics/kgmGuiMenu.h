#pragma once

#include "kgmGui.h"
#include "../kgmBase/kgmCallback.h"

class kgmGuiMenu: public kgmGui
{
  KGM_OBJECT(kgmGuiMenu)

public:

  static u32 ItemHeight;

  class Item
  {
  public:

    typedef kgmCallback<void, kgmObject*> ClickEventCallback;

    enum Type
    {
      TypeItem,
      TypeMenu,
      TypeSeparator
    };

    ClickEventCallback callback;

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
    :callback(null, null)
    {
      parent = par;

      id    = -1;
      title = text;

      type  = TypeMenu;

      popup = false;
      vertical = !horizontal;

      selected = -1;

      xscale = yscale = 1.0f;

      rect = iRect(0, 0, 0, 0);
      rect.w = 10 * title.length();
      rect.h = ItemHeight;

      callback = null;
    }

    Item(Item* par, u32 eid, kgmString text, ClickEventCallback fncall, bool horizontal = false)
    :callback(null, null)
    {
      parent = par;

      id    = eid;
      title = text;

      type  = TypeItem;

      popup = false;
      vertical = !horizontal;

      selected = -1;

      xscale = yscale = 1.0f;

      rect = iRect(0, 0, 0, 0);
      rect.w = 10 * title.length();
      rect.h = ItemHeight;

      callback = fncall;
    }

    ~Item()
    {
      for(int i = 0; i < items.length(); i++)
        delete items[i];

      items.clear();
    }

    kgmString getTitle() { return title; }

    iRect getRect() const { return rect; }

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
        if(item->rect.w > rect.w)
          rect.w = item->rect.w;

        rect.h += ItemHeight;
      }
      else
      {
        rect.w += item->rect.w;
      }

      items.add(item);

      return item;
    }

    Item* add(u32 id, kgmString title, ClickEventCallback fncall, kgmTexture* icon = null)
    {
      if(type != TypeMenu || title.length() < 1)
        return null;

      Item* item = new Item(this, id, title, fncall);

      if(vertical)
      {
        if(item->rect.w > rect.w)
          rect.w = item->rect.w;

        rect.h += ItemHeight;
      }
      else
      {
        rect.w += item->rect.w;
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
          rc.x += ((i==0) ? (0) : (items[i-1]->rect.w));
          rc.y = rect.y;
          rc.w = items[i]->rect.w;
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
