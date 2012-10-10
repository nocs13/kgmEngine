#include "../kgmSystem/kgmApp.h"
#include "../kgmGame/kgmGameBase.h"
#include "kgmEditor.h"


class kgmBuilder: public kgmApp{
public:
 kgmBuilder(){
 }
 
 ~kgmBuilder(){
 }

 void main(){
   kgmEditor* editor = new kgmEditor();  
   if(editor){
     editor->loop();
     editor->release();
   }
   
   kgmObject::releaseObjects();
   printf("leave\n");
 } 
};

//main object
kgmBuilder theApp;
//////////////
