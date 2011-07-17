/*
 * kgmNode.cpp
 *
 *  Created on: Jun 2, 2011
 *      Author: goga
 */

#include "kgmNode.h"

kgmNode::~kgmNode()
{
  for(int i = 0; i < nodes.size(); i++)
    delete nodes[i];
  nodes.clear();
}

static int depth = 0;
static void n_print(kgmNode* n)
{
  kgmString t = "None";
  if(!n)
    return;

  switch(n->type){
  case kgmNode::NodeNone:
    t = "None";
    break;
  case kgmNode::NodeObject:
    t = "Object";
    break;
  case kgmNode::NodeGui:
    t = "Gui";
    break;
  case kgmNode::NodeMesh:
    t = "Mesh";
    break;
  case kgmNode::NodeMaterial:
    t = "Material";
    break;
  case kgmNode::NodeLight:
    t = "Light";
    break;
  case kgmNode::NodeCamera:
    t = "Camera";
    break;
  case kgmNode::NodeMatrix:
    t = "Matrix";
    break;
  case kgmNode::NodeAnimation:
    t = "Animation";
    break;
  case kgmNode::NodeSkeleton:
    t = "Skeleton";
    break;
  case kgmNode::NodeString:
    t = "String";
    break;
  }
  for(int i = 0; i < depth; i++) fprintf(stderr, " ");
  fprintf(stderr, "%s: %i\n", t.data(), n->object);
  depth++;
  for(int i = 0; i < n->nodes.size(); i++) n_print(n->nodes[i]);
  depth--;
}

void kgmNode::print()
{
 n_print(this);
}

bool kgmNode::erase(kgmNode* node){
  if(!node)
    return false;
  for(int i = 0; i < nodes.size(); i++){
   if(nodes[i] == node){
    kgmNode*  n = nodes[i];
    nodes.erase(i);
    delete n;
    return true;
   }else if(nodes[i]->erase(node)){
     return true;
   }
  }
  return false;
}

kgmNode* kgmNode::find(uint type, void* o)
{
  if( (type == this->type) && (o == (void*)object))
    return this;
  kgmNode* f = 0;
  for(int i = 0; i < nodes.size(); i++)
    if((f = nodes[i]->find(type, o)) != 0)
      break;
  return f;
}
