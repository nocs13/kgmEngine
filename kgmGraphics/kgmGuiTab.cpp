#include "kgmGuiTab.h"

KGMOBJECT_IMPLEMENT(kgmGuiTab, kgmGui)

kgmGuiTab::kgmGuiTab(){
  m_index  = 0;
}

kgmGuiTab::kgmGuiTab(kgmGui *par, int x, int y, u32 w, u32 h)
:kgmGui(par, x, y, w, h){
  m_index  = 0;
}

kgmGuiTab::~kgmGuiTab(){
}


u32 kgmGuiTab::set(u32 k){
  if((k < 0) || (k >= m_childs.size()))
    return m_index;
  
  for(int i = 0; i < m_childs.size(); i++){
    if(i == k){
      m_index = i;
      m_childs[i]->show();
    }else{
      m_childs[i]->hide();
    }
  }
  return m_index;
}

u32 kgmGuiTab::next(){
  return set(m_index + 1);
}

u32 kgmGuiTab::previous(){
  return set(m_index - 1);
}

