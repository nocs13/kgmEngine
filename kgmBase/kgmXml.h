// kgmXml.h: interface for the kgmXml class.
//
//////////////////////////////////////////////////////////////////////
#pragma once
#include "kgmList.h"
#include "kgmString.h"
#include "kgmMemory.h"

typedef unsigned int u32;

class kgmXml  
{
public:
 enum XmlState
 {
     XML_NONE,
     XML_ERROR,
     XML_FINISH,
     XML_TAG_OPEN,
     XML_TAG_CLOSE,
     XML_TAG_DATA
 };

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

private:
 char*               m_position;
 kgmString           m_xmlString;
 kgmString           m_tagName;
 kgmString           m_tagData;
 kgmList<Attribute*> m_attributes;

public:
 kgmXml();
 kgmXml(void* mem, int size);
 kgmXml(kgmMemory<char>& m);
 kgmXml(kgmString& s);
 virtual ~kgmXml();
 void print(Node*);

 XmlState open(kgmMemory<char>& m);
 XmlState next();
 XmlState close();

protected:
 Node* parse(void* mem, int size);

 unsigned char* toSyms(unsigned char* m, char* to, int max = -1);
 unsigned char* toStr(unsigned char* m, char* to);
 unsigned char* exeptSyms(unsigned char* m, char* to);
};

