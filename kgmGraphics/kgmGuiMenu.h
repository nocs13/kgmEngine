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
    u32        id;
    Type       type;
    kgmString  title;

    kgmGui::Rect rect;

    bool       expand;
    bool       vertical;

    s32        selected;

    f32        xscale, yscale;

    kgmList<Item*> items;

  public:
    Item(kgmString text, bool horizontal = false)
    {
      id    = -1;
      title = text;

      type  = TypeMenu;

      expand = false;
      vertical = !horizontal;

      selected = -1;

      xscale = yscale = 1.0f;

      rect = iRect(0, 0, 10 * title.length(), ItemHeight);
    }

    Item(u32 eid, kgmString text, bool horizontal = false)
    {
      id    = eid;
      title = text;

      type  = TypeItem;

      expand = false;
      vertical = !horizontal;

      selected = -1;

      xscale = yscale = 1.0f;

      rect = iRect(0, 0, 10 * title.length(), ItemHeight);
    }

    ~Item()
    {
      for(int i = 0; i < items.length(); i++)
        delete items[i];

      items.clear();
    }

    kgmString getTitle() { return title; }

    void setExpand(bool e) { expand = e; }
    bool getExpand() { return expand; }

    iRect getRect() { return rect; }

    s32 getType(){ return (s32)type; }
    s32 getSelected() { return selected; }
    s32 getItemsCount() { return items.length(); }

    Item* add(kgmString title, kgmTexture* icon = null)
    {
      if(type != TypeMenu || title.length() < 1)
        return null;

      Item* item = new Item(title);
      items.add(item);

      if(vertical)
      {
        rect.h += item->getRect().height();

        item->setPosition(rect.x, rect.y + rect.h);
      }
      else
      {
        rect.w += item->getRect().width();

        item->setPosition(rect.x + rect.w, rect.y);
      }

      return item;
    }

    Item* add(u32 id, kgmString title, kgmTexture* icon = null)
    {
      if(type != TypeMenu || title.length() < 1)
        return null;

      Item* item = new Item(id, title);
      items.add(item);

      if(vertical)
      {
        item->setPosition(rect.x, rect.y + rect.h);
        rect.h += item->getRect().height();
      }
      else
      {
        item->setPosition(rect.x + rect.w, rect.y);
        rect.w += item->getRect().width();
      }

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
      if(rect.inside(x, y))
      {
        for(u32 i = 0; i < items.length(); i++)
        {
          if(items[i]->getRect().inside(x, y) && selected != i)
          {
            if(selected != -1 && selected < items.length())
              items[selected]->setExpand(false);

            selected = i;

            break;
          }
        }

        if(selected != -1)
        {
          if(selected < items.length() && items[selected]->getItemsCount() > 0)
            items[selected]->setExpand(true);
        }
      }
      else if(selected != -1 && selected < items.length() && items[selected]->getExpand())
      {
        items[selected]->movePointer(x, y);
      }
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
