/* 
 * File:   kgmGameGUI.cpp
 * Author: goga
 * 
 * Created on March 3, 2013, 3:49 PM
 */

#include "kgmGameGui.h"

kgmGameGui::kgmGameGui(kgmIGame* game, kgmXml& xml) {
  m_game = NULL;

  if (game == NULL){
      return;
  }else{
      m_game = game;
  }

  if (!xml.m_node)
    return;

  build(this, xml.m_node);
}

kgmGameGui::~kgmGameGui() {
}

void kgmGameGui::build(kgmGui* gui, kgmXml::Node* xnode){

}
