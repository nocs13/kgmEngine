// kgmXml.cpp: implementation of the kgmXml class.
//
//////////////////////////////////////////////////////////////////////

#include "kgmXml.h"
#include <stdlib.h>
#include <string.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

kgmXml::Node::Node(kgmXml::Node* parent){
 m_parent = parent;
 if(parent)
  parent->add(this);
}

kgmXml::Node::~Node(){
 int i;
 for(i = 0; i < m_nodes.size(); i++)
	 delete m_nodes[i];
 for(i = 0; i < m_attributes.size(); i++)
	 delete m_attributes[i];
 m_nodes.clear();
 m_attributes.clear();
}

void kgmXml::Node::add(kgmXml::Node* n){
 m_nodes.push_back(n);
}

void kgmXml::Node::add(kgmXml::Attribute* a){
 m_attributes.push_back(a);
}

kgmXml::Node* kgmXml::Node::parent(){
 return m_parent;
}

int   kgmXml::Node::nodes(){
 m_nodes.size();
}

kgmXml::Node* kgmXml::Node::node(int i){
 if(i >= m_nodes.size())
  return 0;
 return m_nodes[i];
}

kgmXml::Node* kgmXml::Node::node(kgmString id){
 for( int i = 0; i < nodes(); i++){
   kgmString nid;
   node(i)->id(nid);
   if(id == nid)
     return node(i);
 }
 return 0;
}

bool kgmXml::Node::id(kgmString& id){
 id = m_name;
 return true;
}

bool kgmXml::Node::data(kgmString& data){
 data = m_data;
 return true;
}

int   kgmXml::Node::attributes(){
 return	m_attributes.size();
}

bool  kgmXml::Node::attribute(int i, kgmString& id, kgmString& data){
 if(i >= m_attributes.size())
  return false;
 id = m_attributes[i]->m_name;
 data = m_attributes[i]->m_data;
 return true;
}

bool  kgmXml::Node::attribute(kgmString id, kgmString& value){
 for( int i = 0; i < attributes(); i++){
  kgmString a, v;
  attribute(i, a, v);
  if(a == id){
   value = (const kgmString)v;
   return true;
  }
 }
 return false;
}

kgmXml::kgmXml()
{
 m_node = 0;
}

kgmXml::kgmXml(void* mem, int size){
 m_node = parse(mem, size);
}

kgmXml::kgmXml(kgmMemory<char>& m){
 m_node = parse(m.data(), m.length());
}

kgmXml::kgmXml(kgmString& s){
 m_node = parse(s.data(), s.length());
}

kgmXml::~kgmXml()
{
 if(m_node)
  delete m_node;
}

kgmXml::Node* kgmXml::parse(void* mem, int size){
 int pos = 0;
 int cpos = 0;      //select new probably position
 char a = 0;
 char charString = 0; //string started with char ' or "
 bool isTag = false;
 bool isTab = false;
 bool isNode = false;
 bool isString = false;
 bool isNodeName = false;
 Node* node = 0;
 Node* base = 0;
 Attribute* attr = 0;
 unsigned char* p = (unsigned char*)mem;

 void* m_name_ref = 0;
 void* m_data_ref = 0;
 int   m_name_len = 0;
 int   m_data_len = 0;

 while((p - (unsigned char*)mem) < size){
  if(*p == '<'){
   p++;
   if(*p == '?' || *p == '!'){
    unsigned char schar = *p;
    p = toSyms(p, ">\0");
    continue;
   }else if(*p == '/'){
    unsigned char* cp;
    unsigned char* cn = p - 1;
    p++;
    p  = exeptSyms(p, " \t\r\n");
    cp = p;
    p  = toSyms(p, " \t\r\n>");
    if(node){
      if(!memcmp(m_name_ref, cp, m_name_len))
	isNode = false;
      if(node->m_nodes.size())
	m_data_len = 0;
      else
	node->m_data.alloc((char*)m_data_ref, ((int)cn) - (int)m_data_ref);
      node = node->parent();
    }
   }else{
    Node* n = new Node(node);

    m_name_ref = 0;
    m_data_ref = 0;
    m_name_len = 0;
    m_data_len = 0;

    if(!base)  base = n;
    isNode = true;
    node = n;

    p = exeptSyms(p, " \t\r\n");
    m_name_ref = p;
    p = toSyms(p, " \t/>");
    m_name_len = (int)p - (int)m_name_ref;
    n->m_name.alloc((char*)m_name_ref, m_name_len);

    while(*p != '>'){
     if(!isNode){
      p = toSyms(p, ">\0");
      break;
     }else if(isNode && (*p == '/')){
      if(!memcmp(p, "/>", 2)){
       isNode = false;
       node = node->parent();
      }
     }else if(isNode && (*p == ' ' || *p == '\t')){
      p = exeptSyms(p, " \t");
      if(*p != '/' && *p != '>'){
       Attribute* a = new Attribute;
       void* m_name_ref = 0;
       void* m_data_ref = 0;
       int   m_name_len = 0;
       int   m_data_len = 0;

       node->add(a);
       m_name_ref = p;
       p = toSyms(p, " \t=");
       m_name_len = (int)p - (int)m_name_ref;
       p = toSyms(p, "=");
       p = toSyms(p, "'\"");
       char csyms[2] = {*p, '\0'};
       m_data_ref = ++p;
       p = toSyms(p, csyms);
       m_data_len = (int)p - (int)m_data_ref;

       a->m_name.alloc((char*)m_name_ref, m_name_len);
       a->m_data.alloc((char*)m_data_ref, m_data_len);
      }
     }else{
       p++;
     }
    }
    if(*p == '>' && isNode){
      m_data_ref = p + 1;
    }
   }
  }

  p++;
 }
 return base;
}

unsigned char* kgmXml::toSyms(unsigned char* m, char* to){
 bool loop = true;
 while(loop){
  char* c = to;
  for(int i = 0; i < strlen(to); i++, c++){
   if(*m == *c){
    loop = false;
   }
  }
  if(loop)
   m++;
 }
 return m;
}

unsigned char* kgmXml::toStr(unsigned char* m, char* to){
 bool loop = true;
 while(loop){
  if(!memcmp(m, to, strlen(to)))
   break;
  else
   m++;
 }
 return m;
}

unsigned char* kgmXml::exeptSyms(unsigned char* m, char* to){
 bool loop = true;
 while(loop){
  char* c = to;
  loop = false;
  for(int i = 0; i < strlen(to); i++){
   if(*m == *c){
    loop = true;
    break;
   }
  }
  if(loop)
   m++;
  else
   break;
 }
 return m;
}

void kgmXml::print(Node* n)
{
  if(!n)
   return;
  kgmString s, d;
  n->id(s);
  n->data(d);
  printf("\n id=%s data=%s", (char*)s, (char*)d);

  for(int i = 0; i < n->attributes(); i++){
    kgmString s, d;
    n->attribute(i, s, d);
    printf("\n   attr: %s=%s", (char*)s, (char*)d);
  }

  for(int i = 0; i < n->nodes(); i++)
   print(n->node(i));
}
