#pragma once 
#include "kgmObject.h"
#include "malloc.h"

typedef unsigned int uint;
////////// Hash Tab

template<class Key, class Value> 
class kgmTab{
private:
 struct _Node{
  Key    m_key;
  Value  m_value;
  _Node* m_next;
 };
 _Node* m_nodes;
public:
 kgmTab(){
  m_nodes = 0;
 }

 ~kgmTab(){
 }

 Value& operator[](Key key){
  _Node* node = _find(key);
  return node->m_value;
 }

 void clear(){
  for(int i = length(); i > 0; i--){ 
   _Node* node = _index(i-1);
   delete node;
  }
 }

 int length(){
  int len = 0;
  _Node* node = m_nodes;
  while(node){
   len++;
   node = node->m_next;
  }
  return len;
 }

 bool get(uint index, Key& key, Value& value){
  _Node* node = _index(index);
  if(node == 0)
    return false;
  key = node->m_key;
  value = node->m_value;
  return true;
 }
 
 bool get(Key key, Value& v){
  _Node* node = _find(key);
  if(node == 0)
   return false;
  v = node->m_value;
  return true;
 }

 void add(Key key, Value value){ 
  _Node* node = new _Node;
  node->m_key = key;
  node->m_value = value;
  node->m_next = 0;
  if(!m_nodes)
   m_nodes = node;
  else
   _last()->m_next = node;
 }

private:
 _Node* _last(){
  _Node* node = m_nodes;
  if(node == 0) return 0;
  while(node->m_next != 0)
	  node = node->m_next;
  return node;
 }

 _Node* _index(uint i){
  _Node* node = m_nodes;
  uint index = 0;
  while(node){
   if(i == index)
    return node;
   node = node->m_next;
   index++;
  }
  return 0;
 }

 _Node* _find(Key& key){
  _Node* node = m_nodes;
  while(node){
    if(node->m_key == key) 
     return node;
   node = node->m_next;
  }
  return 0;
 }
};
