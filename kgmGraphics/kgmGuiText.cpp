#include "kgmGuiText.h"

KGMOBJECT_IMPLEMENT(kgmGuiText, kgmGui);

kgmGuiText::kgmGuiText()
{
}

kgmGuiText::kgmGuiText(kgmGui *par, u32 x, u32 y, u32 w, u32 h)
 :kgmGui(par, x, y, w, h){
}

kgmGuiText::~kgmGuiText(){
}
