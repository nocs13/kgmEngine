#include "../kgmSystem/kgmApp.h"
#include "../kgmGame/kgmGameBase.h"
//#include "../kgmGame/kgmGameLogic.h"

#include "../kgmBase/kgmXml.h"

#include "kGui.h"

class kGame: public kgmGameBase{
  kGui* gui;

public:
  kGame(){
      gui = new kGui(this);
  }

  ~kGame(){
  }

};


class kApp: public kgmApp{
 kGame*	m_game;	 
public:
 kApp(){
 }
 
 ~kApp(){
 }

 void print_xml(kgmXml::Node* n){
  if(!n)
   return;
  kgmString s, d;
  n->id(s);
  n->data(d);
  printf("\n id=%s", (char*)s);
  printf("\n data=%s", (char*)d);
  
  for(int i = 0; i < n->attributes(); i++){
    kgmString s, d;
    n->attribute(i, s, d);
    printf("\n   attr: %s=%s", (char*)s, (char*)d);
  }
  
  for(int i = 0; i < n->nodes(); i++)
   print_xml(n->node(i));
 }
 
  void printf_xml(char* path){
   kgmString s(path, strlen(path));
   kgmFile f;
   f.open(s, kgmFile::Read);
   kgmMemory<char> m;
   m.alloc(f.length());
   f.read(m.data(), f.length());
   f.close();
   kgmXml xml(m);
   print_xml(xml.m_node);
  }

 void main(){
  //printf_xml("map001.kgm");
  m_game = new kGame();
  m_game->loop();
  delete m_game;
  //kgmOGLWindow* w = new kgmOGLWindow(0, "", 0, 0, 100, 100, 16, false); 
  //w->loop();
  //delete w;
   printf("hello\n");
 } 
};

//main object
kApp theApp;
//////////////
