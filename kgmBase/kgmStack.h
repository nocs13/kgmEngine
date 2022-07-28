#pragma once
#include "kgmMemory.h"

template <class T> 
class kgmStack{
 struct _Node{
  T data;
  _Node *prev;

  _Node(){
   memset(this, 0, sizeof(_Node));
  }
 };

 _Node *_Last;
 int    _length;
public:
 kgmStack(){
  _Last = 0;
  _length = 0;
 }
 ~kgmStack(){
 }

 void clear(){
 }

 void push(T data){
  _Node *node = (_Node*) kgm_alloc(sizeof(_Node));
  node->data = data;
  node->prev = _Last;
  _Last = node;
  _length++;
 }

 bool pop(T &data){
  if(!_Last)
	  return false;
  data = _Last->data;
  _Node* node = _Last;
  _Last = _Last->prev;
  kgm_free(node);
  _length--;
  return true;
 }

 T &top(){
  return _Last->data;
 }

 int length(){
  return _length;
 }
};
