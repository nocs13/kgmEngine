/* 
 * File:   kgmEditor.cpp
 * Author: goga
 * 
 * Created on January 7, 2012, 9:00 PM
 */

#include "kgmEditor.h"

kgmEditor::kgmEditor() {
  //lua = new kgmGameLua(this);

  //if(!luaRun("gui.lua")){
  //  printf("\nCan't load gui.lua");
  //}
    
  //if(!luaRun("main.lua")){
  //  printf("\nCan't run main.lua");
  //}
}

kgmEditor::kgmEditor(const kgmEditor& orig) {
}

kgmEditor::~kgmEditor() {
  //delete lua;
}

bool kgmEditor::luaRun(kgmString script)
{
  kgmMemory<char> m;
  m_resources->getFile(script, m);
  
  if(m.length() > 0){
    kgmString s(m.data(), m.length());
    lua->run(s);
    return true;
  }
  return false;
}
