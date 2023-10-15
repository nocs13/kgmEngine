// kgmXml.cpp: implementation of the kgmXml class.
//
//////////////////////////////////////////////////////////////////////

#include "kgmXml.h"
#include "kgmLog.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

kgmXml::Node::Node(kgmXml::Node* parent)
{
  m_parent = parent;

  if(parent)
    parent->add(this);
}

kgmXml::Node::~Node()
{
  int i;

  for(i = 0; i < m_nodes.size(); i++)
    delete m_nodes[i];

  for(i = 0; i < m_attributes.size(); i++)
    delete m_attributes[i];

  m_nodes.clear();
  m_attributes.clear();
}

void kgmXml::Node::add(kgmXml::Node* n)
{
  m_nodes.push_back(n);
}

void kgmXml::Node::add(kgmXml::Attribute* a)
{
  m_attributes.push_back(a);
}

kgmXml::Node* kgmXml::Node::parent(){
  return m_parent;
}

int kgmXml::Node::nodes()
{
  return m_nodes.size();
}

kgmXml::Node* kgmXml::Node::node(int i){
  if(i >= m_nodes.size())
    return 0;

  return m_nodes[i];
}

kgmXml::Node* kgmXml::Node::node(kgmString id){
  for( int i = 0; i < nodes(); i++) {
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

bool kgmXml::Node::hasattr(kgmString id)
{
  for(int i = 0; i < m_attributes.length(); i++) {
    Attribute *a = m_attributes[i];

    if(a->m_name == id) {
      return true;
    }
  }

  return false;
}

kgmXml::kgmXml()
{
  m_node = 0;
  m_spaces = 0;
  m_position = null;
}

kgmXml::kgmXml(void* mem, int size){
  m_node = 0;
  m_spaces = 0;
  m_position = null;
  m_node = parse(mem, size);
}

kgmXml::kgmXml(kgmArray<u8>& m){
  m_node = 0;
  m_spaces = 0;
  m_position = null;
  m_node = parse(m.data(), m.length());
}

kgmXml::kgmXml(kgmString& s){
  m_node = 0;
  m_spaces = 0;
  m_position = null;
  m_node = parse(s.data(), s.length());
}

kgmXml::~kgmXml()
{
  if(m_node)
    delete m_node;
}

kgmXml::Node* kgmXml::parse(void* mem, int size)
{
  //int pos = 0;
  //int cpos = 0;      //select new probably position
  //char a = 0;
  //char charString = 0; //string started with char ' or "
  //bool isTag = false;
  //bool isTab = false;
  bool isNode = false;
  //bool isString = false;
  //bool isNodeName = false;
  Node* node = 0;
  Node* base = 0;
  //Attribute* attr = 0;
  unsigned char* p = (unsigned char*)mem;

  void* m_name_ref = 0;
  void* m_data_ref = 0;
  int   m_name_len = 0;
  //int   m_data_len = 0;

  while((p - (unsigned char*)mem) < size){
    if(*p == '<'){
      p++;
      if(*p == '?'){
        p = toSyms(p, (char*)"?>\0");
        p++;
        continue;
      }else if(*p == '!'){
        p++;

        if(strncmp((const char*)p, "[CDATA[", 7))
        {
          char*     cdata = (char*)p;
          kgmString sdata;

          p = toSyms(p, (char*)"]]>\0");
          sdata.alloc(cdata, (u32)((size_t)p - (size_t)cdata));

          if(isNode && node)
          {
            node->m_data += sdata;
          }

          p += 2;
          continue;
        }
        else if(strncmp((const char*)p, "--", 2))
        {
          p = toSyms(p, (char*)"-->\0");
          p += 2;
          continue;
        }
        else
        {
          p = toSyms(p, (char*)">\0");
          continue;
        }
      }else if(*p == '/'){
        unsigned char* cp;
        unsigned char* cn = p - 1;
        p++;
        p  = exeptSyms(p, (char*)" \t\r\n");
        cp = p;
        p  = toSyms(p, (char*)" \t\r\n>");
        if(node){
          if(!memcmp(m_name_ref, cp, m_name_len))
            isNode = false;
          if(node->m_nodes.size())
          {
            //m_data_len = 0;
          }
          else
          {
            kgmString s;
            s.alloc((char*)m_data_ref, ((size_t)cn) - (size_t)m_data_ref);
            node->m_data += s;
          }
          node = node->parent();
        }
      }else{
        Node* n = new Node(node);

        m_name_ref = 0;
        m_data_ref = 0;
        m_name_len = 0;
        //m_data_len = 0;

        if(!base)  base = n;
        isNode = true;
        node = n;

        p = exeptSyms(p, (char*)" \t\r\n");
        m_name_ref = p;
        p = toSyms(p, (char*)" \t/>");
        m_name_len = (size_t)p - (size_t)m_name_ref;
        n->m_name.alloc((char*)m_name_ref, m_name_len);

        while(*p != '>'){
          if(!isNode){
            p = toSyms(p, (char*)">\0");
            break;
          }else if(isNode && (*p == '/')){
            if(!memcmp(p, "/>", 2)){
              isNode = false;
              node = node->parent();
            }
          }else if(isNode && (*p == ' ' || *p == '\t')){
            p = exeptSyms(p, (char*)" \t");
            if(*p != '/' && *p != '>'){
              Attribute* a = new Attribute;
              void* m_name_ref = 0;
              void* m_data_ref = 0;
              int   m_name_len = 0;
              int   m_data_len = 0;

              node->add(a);
              m_name_ref = p;
              p = toSyms(p, (char*)" \t=");
              m_name_len = (size_t)p - (size_t)m_name_ref;
              p = toSyms(p, (char*)"=");
              p = toSyms(p, (char*)"'\"");
              char csyms[2] = {(char) *p, '\0'};
              m_data_ref = ++p;
              p = toSyms(p, csyms);
              m_data_len = (size_t)p - (size_t)m_data_ref;

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

kgmXml::XmlState kgmXml::open(kgmArray<u8>& m)
{
  m_position = null;

  if(m_xmlString.length())
    close();

  if(m.length() < 1)
    return XML_ERROR;

  m_xmlString.alloc((char*)m.data(), m.length());
  m_position = m_xmlString.data();

  if(*m_position != '<' && *(++m_position) != '?')
  {
    close();

    return XML_ERROR;
  }
  else
  {
    m_position = (char*)toSyms((unsigned char*)m_position, (char*)">", m.length());

    if(!m_position)
    {
      close();

      return XML_ERROR;
    }

    char* c = m_position - 1;

    if(*c != '?')
    {
      close();

      return XML_ERROR;
    }
  }

  if(*m_position == '>')
    m_position++;

  return XML_NONE;
}

kgmXml::XmlState kgmXml::next()
{
  m_tagData.clear();
  m_tagName.clear();

  for (kgmList<Attribute*>::iterator i = m_attributes.begin(); i != m_attributes.end(); i.next())
  {
    delete (*i);
  }

  m_attributes.clear();

  if(!m_position)
  {
    close();

    return XML_ERROR;
  }

  char* pt = m_position;

  int size = (int)(pt - m_xmlString.data());

  if(size >= m_xmlString.length())
  {
    close();

    return XML_FINISH;
  }

  //Find tag start symbol
  pt = (char*)exeptSyms((unsigned char*)pt, (char*)" \t\r\n");

  if(!pt)
  {
    close();

    return XML_FINISH;
  }

  //Check if before was some ascii, it will be identified as data
  if(*pt != '<')
  {
    pt = (char*)toSyms((unsigned char*)pt, (char*)"<");

    if(!pt)
    {
      close();

      return XML_ERROR;
    }

    m_tagData.alloc(m_position, (s32)(pt - m_position));

    m_position = pt;

    return XML_TAG_DATA;
  }

  //got tag start position
  m_position = ++pt;

  if(*pt == '>' || *pt == ' ' || *pt == '\t')
  {
    close();

    return XML_ERROR;
  }
  else if(*pt == '/')
  {
    m_position = ++pt;

    pt = (char*)toSyms((unsigned char*)pt, (char*)" \t/>");

    if(*pt != '>' || (pt == m_position))
    {
      close();

      return XML_ERROR;
    }
    else
    {
      m_tagName.alloc(m_position, (int)(pt - m_position));
      m_position = ++pt;

      return XML_TAG_CLOSE;
    }
  }

  pt = (char*)toSyms((unsigned char*)pt, (char*)" \t/>");

  m_tagName.alloc(m_position, (int)(pt - m_position));
  Attribute* attr = null;

  while((pt = (char*) exeptSyms((u8*) pt, (char*) " \t")))
  {
    if(*pt == '0')
    {
      close();

      return XML_FINISH;
    }

    if(*pt == '>')
    {
      m_position = ++pt;

      return XML_TAG_OPEN;
    }
    else if(*pt == '/')
    {
      pt++;

      if(*pt == '>')
      {
        m_position = ++pt;

        return XML_TAG_CLOSE;
      }
      else
      {
        close();

        return XML_FINISH;
      }
    }
    else if(*pt == '=')
    {
      if(!attr)
      {
        close();

        return XML_ERROR;
      }

      pt = (char*)exeptSyms((unsigned char*)(++pt), (char*)" \t");

      if(*pt == '/' || *pt == '>' )
      {
        close();

        return XML_ERROR;
      }
      else if(*pt == '\'' || *pt == '"')
      {
        char* sym = (char*)((*pt == '"') ? ("\"") : ("'"));

        m_position = ++pt;
        pt = (char*)toSyms((unsigned char*)pt, sym);

        if(!pt)
        {
          close();

          return XML_ERROR;
        }

        attr->m_data.alloc(m_position, (u32)(pt - m_position));
        m_attributes.append(attr);
        attr = null;
        m_position = ++pt;
      }
      else
      {
        pt = (char*)toSyms((unsigned char*)pt, (char*)" \t/>");

        if(!pt)
        {
          close();

          return XML_ERROR;
        }

        attr->m_data.alloc(m_position, (u32)(pt - m_position));
        m_attributes.append(attr);
        attr = null;
      }
    }
    else if((*pt >= 'a' && *pt <= 'z') || (*pt >= 'A' && *pt <= 'Z'))
    {
      m_position = pt;
      pt = (char*)toSyms((unsigned char*)pt, (char*)" \t=/>");

      if(!pt || attr)
      {
        close();

        return XML_ERROR;
      }

      attr = new Attribute();
      attr->m_name.alloc(m_position, (u32)(pt - m_position));
    }
  }

  if(attr)
  {
    delete attr;
  }

  close();

  return XML_ERROR;
}

kgmXml::XmlState kgmXml::close()
{
  m_position = null;
  m_tagData.clear();
  m_tagName.clear();

  for (kgmList<Attribute*>::iterator i = m_attributes.begin(); i != m_attributes.end(); i.next())
  {
    delete (*i);
  }

  m_attributes.clear();

  if(m_xmlString.length())
    m_xmlString.clear();

  return XML_NONE;
}

int kgmXml::attributes()
{
  return m_attributes.length();
}

bool kgmXml::attribute(int i, kgmString& key, kgmString& value)
{
  if(i >= m_attributes.length())
    return false;

  Attribute* attr = m_attributes[i];
  key = attr->m_name;
  value = attr->m_data;

  return true;
}

bool kgmXml::attribute(kgmString key, kgmString& value)
{
  for(kgmList<Attribute*>::iterator i = m_attributes.begin(); i != m_attributes.end(); i++)
  {

    if(key == (*i)->m_name)
    {
      value = (*i)->m_data;

      return true;
    }
  }

  return false;
}

bool kgmXml::hasattr(kgmString id)
{
  for(kgmList<Attribute*>::iterator i = m_attributes.begin(); i != m_attributes.end(); i++)
  {
    if(id == (*i)->m_name)
    {
      return true;
    }
  }

  return false;
}

unsigned char* kgmXml::toSyms(unsigned char* m, char* to, int max){

  if(max < 0)
  {
    max = 1 * 1024 * 1024 * 1024;
  }

  while(--max)
  {
    char* c = to;

    for(u32 i = 0; i < strlen(to); i++, c++)
    {
      if(*m == *c)
      {
        return m;
      }
    }

    m++;
  }

  return null;
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

  while(loop)
  {
    char* c = to;
    loop = false;

    for(u32 i = 0; i < strlen(to); i++)
    {
      if(*m == c[i])
      {
        loop = true;
        break;
      }
    }

    if(loop)
    {
      m++;

      if(*m == '\0')
        return null;
    }
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

void kgmXml::addToString(kgmString& str, kgmXml::Node* node)
{
  if(!node)
    return;

  m_spaces ++;

  str += kgmString("\n");

  for(int i = 0; i < m_spaces; i++)
    str += " ";

  str += kgmString("<") + node->m_name;

  for(int i = 0; i < node->m_attributes.length(); i++)
  {
    str += " ";
    str += node->m_attributes[i]->m_name;
    str += "='";
    str += node->m_attributes[i]->m_data;
    str += "'";
  }

  bool subs = false;

  if(node->m_data.length() > 0 ||  node->m_nodes.length() > 0)
    subs = true;

  if(!subs)
    str += "/>";
  else
    str += ">";

  if(node->m_data.length() > 0)
  {
    str += node->m_data;
  }
  else if(node->m_nodes.length() > 0)
  {
    for(int i = 0; i < node->m_nodes.length(); ++i)
      addToString(str, node->m_nodes[i]);
  }

  if(subs)
  {
    str += "\n";

    for(int i = 0; i < m_spaces; i++)
      str += " ";

    str += "</";
    str += node->m_name;
    str += ">";
  }

  m_spaces --;
}

u32 kgmXml::toString(kgmString& str)
{
  str = "<?xml version='1.0'?>";

  m_spaces = -1;

  addToString(str, m_node);

  return str.length();
}
