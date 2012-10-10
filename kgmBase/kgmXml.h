// kgmXml.h: interface for the kgmXml class.
//
//////////////////////////////////////////////////////////////////////
#pragma once
#include "kgmList.h"
#include "kgmString.h"
#include "kgmMemory.h"
#include <vector>
#include <string>

typedef unsigned int uint;

class kgmXml  
{
public:
 struct Attribute{
   kgmString m_name;
   kgmString m_data;
 };

 class Node{
  Node* m_parent;

 public:
  kgmString m_name;
  kgmString m_data;

  kgmList<Node*>	m_nodes;
  kgmList<Attribute*>	m_attributes;
 public:
  Node(Node*);
  ~Node();
  void  add(Node*);
  void  add(Attribute*);
  Node* parent();
  int   nodes();
  Node* node(int i);
  Node* node(kgmString id);
  bool  id(kgmString& id);
  bool  data(kgmString& data);
  int   attributes();
  bool  attribute(int i, kgmString& id, kgmString& data);
  bool  attribute(kgmString id, kgmString& value);
 };

public:
 Node*	m_node;

public:
 kgmXml();
 kgmXml(void* mem, int size);
 kgmXml(kgmMemory<char>& m);
 kgmXml(kgmString& s);
 virtual ~kgmXml();
 void print(Node*);
protected:
 Node* parse(void* mem, int size);

 unsigned char* toSyms(unsigned char* m, char* to);
 unsigned char* toStr(unsigned char* m, char* to);
 unsigned char* exeptSyms(unsigned char* m, char* to);
};

