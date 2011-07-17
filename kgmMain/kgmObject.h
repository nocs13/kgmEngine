#pragma once
#include "kgmMain.h"
#include "kgmTypes.h"
#include <vector>
#include <stdlib.h>
#include <string.h>

class kgmApp;
class kgmObject;

struct kgmRuntime{
  const char* nClass; //class id
  uint        sClass; //size of class
  kgmRuntime* pClass; //parent class
};

#define KGM_OBJECT(o_class)                     \
 public:                                	\
  static  kgmRuntime   Class;			\
  static  o_class*     cast(kgmObject*);	\
  virtual kgmRuntime&  runtime();	
	
  

#define KGMOBJECT_IMPLEMENT(o_class, o_parent)	\
 kgmRuntime  o_class::Class = {  #o_class, sizeof(class o_class), &o_parent::Class} ;	\
 o_class*    o_class::cast(kgmObject* o) { return (o_class*)o; }	\
 kgmRuntime& o_class::runtime() { return o_class::Class; }	

//base class for kgame project
class kgmObject{
 KGM_OBJECT(kgmObject)
private:
 unsigned int m_references;

protected:
 ~kgmObject(){
 }
public:
 kgmObject(){
  m_references = 1;
 }
 bool instanceof(kgmObject& o){
  return !strcmp(runtime().nClass, o.runtime().nClass);
 }
 bool instanceof(kgmRuntime& o){
  return !strcmp(runtime().nClass, o.nClass);
 }
 bool instanceof(char* o){
  kgmRuntime* r = &Class; 
  while(r != 0){
   if(!strcmp(r->nClass, o))
	   return true;
   if(r == r->pClass)
	   break;
   r = r->pClass;
  }
  return false;
 }
 uint references(){
  return m_references;
 }
 void increment(){
  m_references += 1;
 }
 void release(){
  if(m_references <= 1)
   delete this;
  else
   m_references -= 1;
 }
 kgmObject* clone(){
  return 0;
 }

 void* operator new(unsigned int size);
 void  operator delete(void* p);

 friend class kgmApp;
//private:
 static void releaseObjects();
};
///////////////////




