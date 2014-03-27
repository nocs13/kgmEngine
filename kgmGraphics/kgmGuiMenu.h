#include "kgmGui.h"

class kgmGuiMenu: public kgmGui
{
  KGM_OBJECT(kgmGuiMenu)

public:

  class Item
  {
  protected:
    u32        id;
    kgmString  title;

    kgmGui::Rect rect;

    bool       expand;
    bool       vertical;

    s32        selected;

    kgmList<Item*> items;

  public:
    Item(u32 i, kgmString t, bool vert = true)
    {
      id    = i;
      title = t;

      expand = false;
      vertical = vert;

      selected = -1;

      rect = iRect(0, 0, 0, 20);
      rect.h = 20;
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

    s32 getSelected() { return selected; }
    s32 getItemsCount() { return items.length(); }

    Item* add(u32 id, kgmString title, kgmTexture* icon = null)
    {
      if(title.length() < 1)
        return null;

      Item* item = new Item(id, title);
      items.add(item);

      if(vertical)
        rect.h += 21;
      else
        rect.w += 71;

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
        selected = rect.height() / 21;

        if(selected < items.length() && items[selected]->getItemsCount() > 0)
        {
          items[selected]->setExpand(true);
        }
      }
      else if(selected < items.length() && items[selected]->getExpand())
      {
        items[selected]->movePointer(x, y);
      }
    }
  };


  Item* item;

public:
  kgmGuiMenu(kgmGui* parent, kgmString title = "Menu");
  ~kgmGuiMenu();

  iRect getMenuRect();
  void setPosition(int x, int y);

  void onMsMove(int k, int x, int y);
  void onMsLeftUp(int k, int x, int y);
};
