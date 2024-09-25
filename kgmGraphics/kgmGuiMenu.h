#pragma once

#include "kgmGui.h"

#include "../kgmBase/kgmCallback.h"

#define PIX_IN  2

class kgmGuiMenu: public kgmGui
{
  KGM_OBJECT(kgmGuiMenu)

public:
  static u32 ItemHeight;
  static u32 FontWidth;

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

    kgmTexture* icon;

    bool       popup;
    bool       vertical;

    bool       m_active;

    Item*      m_selected;

    s32        selected;
    f32        xscale, yscale;

    u32        swidth; //submenu max width

    kgmList<Item*> items;

  public:
    Item(Item* par, kgmString text, bool horizontal = false)
    {
      parent = par;

      id    = -1;
      title = text;

      type  = TypeMenu;

      popup = true;
      vertical = !horizontal;

      m_active = false;

      m_selected = null;

      selected = -1;

      xscale = yscale = 1.0f;

      rect = iRect(0, 0, 0, 0);
      rect.w = kgmGuiMenu::FontWidth * title.length();
      rect.h = kgmGuiMenu::ItemHeight;

      swidth = rect.w;
    }

    Item(Item* par, u32 eid, kgmString text, bool horizontal = false)
    {
      parent = par;

      id    = eid;
      title = text;

      type  = TypeItem;

      popup = false;
      vertical = !horizontal;

      m_active = false;

      m_selected = null;

      selected = -1;

      xscale = yscale = 1.0f;

      rect = iRect(0, 0, 0, 0);
      rect.w = kgmGuiMenu::FontWidth * title.length();
      rect.h = kgmGuiMenu::ItemHeight;

      swidth = rect.w;
    }

    ~Item()
    {
      for(int i = 0; i < items.length(); i++)
        delete items[i];

      items.clear();
    }

    kgmString& getTitle() { return title; }

    iRect getRect() const { return rect; }

    u32  getId() const { return id; }
    void setId(u32 i) { id = i; }
    s32 getType(){ return (s32)type; }
    s32 getSelected() { return selected; }
    s32 getItemsCount() { return items.length(); }

    Item* getRoot()
    {
      Item* res = parent;

      if (!res)
        return null;

      while(res->parent)
        res = res->parent;

      return res;
    }

    void select(bool s) {
      if (s)
        selected = id;
      else
        selected = -1;
    }

    bool active() const
    {
      return m_active;
    }

    void active(bool a)
    {
      m_active = a;
    }

    bool ispopup() const
    {
      return popup;
    }

    bool isSelected(Item* it)
    {
      return (it == m_selected);
    }

    Item* getItemSelected()
    {
      Item* item = null;

      if (m_selected && m_selected->active())
        item = m_selected;

      return item;
    }

    Item* add(kgmString title, kgmTexture* icon = null)
    {
      if(type != TypeMenu || title.length() < 1)
        return null;

      Item* item = new Item(this, title);

      item->icon = icon;

      if(vertical)
      {
        if((u32) item->rect.w > swidth)
        {
          swidth = item->rect.w;

          for(int i = 0; i < items.length(); i++)
            items[i]->rect.w = swidth;
        }

        item->rect.y += (ItemHeight * (1 + items.length()) - PIX_IN);
      }
      else
      {
        for(int i = 0; i < items.length(); i++)
          item->rect.x += (items[i]->rect.w - PIX_IN);
      }

      items.add(item);

      if ((this->parent == null) && (this->title.length() < 1))
      {
        rect.h = (kgmGuiMenu::ItemHeight < item->rect.h) ? (item->rect.h) : (kgmGuiMenu::ItemHeight);
        rect.w += item->rect.w;
      }

      return item;
    }

    Item* add(u32 id, kgmString title, kgmTexture* icon = null)
    {
      if(type != TypeMenu || title.length() < 1)
        return null;

      Item* item = new Item(this, id, title);

      item->icon = icon;

      if(vertical)
      {
        if((u32) item->rect.w > swidth)
        {
          swidth = item->rect.w;

          for(int i = 0; i < items.length(); i++)
            items[i]->rect.w = swidth;
        }
        else
        {
          item->rect.w = swidth;
        }

        item->rect.x = rect.x;
        item->rect.y += (ItemHeight * (1 + items.length()) - PIX_IN);
      }
      else
      {
        for(int i = 0; i < items.length(); i++)
          item->rect.x += (items[i]->rect.w - PIX_IN);
      }

      items.add(item);

      if ((this->parent == null) && (this->title.length() == 0))
      {
        rect.h = (kgmGuiMenu::ItemHeight < item->rect.h) ? (item->rect.h) : (kgmGuiMenu::ItemHeight);
        rect.w += item->rect.w;
      }

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

      return items[index]->rect;
    }

    iRect getSubRect()
    {
      iRect rc(0, 0, 0, 0);

      rc.x = rect.x;
      rc.y = rect.y;

      if(vertical)
      {
        rc.w = swidth;
        rc.h = ItemHeight * (1 + items.length());
      }
      else
      {
        for(int i = 0; i < items.length(); i++)
          rc.w += items[i]->rect.w;

        rc.h = ItemHeight;
      }

      return rc;
    }
    /*
    void movePointer(int x, int y)
    {
      for(s32 i = 0; i < items.length(); i++)
      {
        if(getRect(i).inside(x, y) && selected != i)
        {
          selected = i;

          //Item* ci = items[i];
          //Rect  rc = getRect(i);

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
    */
    bool havePointer(int x, int y);
  };

protected:
  Item* root;
  Item* choose;

public:
  Signal<u32> sigChoose;

public:
  kgmGuiMenu(kgmGui* parent);
  ~kgmGuiMenu();

  Item* add(kgmString title);
  Item* add(u32 id, kgmString title);

  void onMsMove(int k, int x, int y);
  void onMsLeftUp(int k, int x, int y);

  void showItems(int x, int y);


  Item* getItem() const
  {
    return root;
  }

  Item* getChoose() const
  {
    return choose;
  }

  bool isSelected(Item* m)
  {
    if (!m )
      return false;

    return root->isSelected(m);
  }

  static u32 fontWidth()
  {
    return 10;
  }
};
