#include "kgmGuiTab.h"

kgmGuiTab::kgmGuiTab(){
  m_index  = 0;
}

kgmGuiTab::kgmGuiTab(kgmGui *par, int x, int y, uint w, uint h)
:kgmGui(par, x, y, w, h){
  m_index  = 0;
}

kgmGuiTab::~kgmGuiTab(){
}


uint kgmGuiTab::set(uint k){
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

uint kgmGuiTab::next(){
  return set(m_index + 1);
}

uint kgmGuiTab::previous(){
  return set(m_index - 1);
}

