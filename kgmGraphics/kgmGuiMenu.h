#include "kgmGui.h"

class kgmGuiMenu: public kgmGui
{
  KGM_OBJECT(kgmGuiMenu)

public:
 
 enum ItemType{
  ItemType_Item,
  ItemType_Menu,
  ItemType_Separator,
 };
  
 class Item{
 protected:
  ItemType   type;
  uint       id;
  kgmString  title;
  
  bool active;
 public:
     Item(kgmString t){
       type = ItemType_Item;             
       title = t;               
     }
     
     ItemType getType(){
         return type;
     }
     
     void activate(bool s){
          active = s;
     }
 };
 
 class Menu: public Item{
  kgmList<Item*> items;
 public:
     Menu(kgmString t)
     :Item(t){
     }
 };

 
 u32 id;
 kgmList<Item*> items;
 bool vertical;
 bool popup;
 
public:
 kgmGuiMenu(kgmGui* parent);
 ~kgmGuiMenu();
 
 private:
};
