#include "kgmGui.h"

class kgmGuiMenu: public kgmGui
{
public:
 enum ItemType{
  ItemType_Menu,
  ItemType_Title,
  ItemType_Separator,
 };
private:
 typedef struct{
  ItemType	 type;
  uint		 id;
  kgmString  title;
 } Item;

public:
 kgmGuiMenu(kgmGui* parent, uint type){}
 ~kgmGuiMenu(){}
};