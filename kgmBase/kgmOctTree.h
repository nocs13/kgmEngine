// kgmOctTree.h: interface for the kgmOctTree class.
//
//////////////////////////////////////////////////////////////////////
#pragma once
#include "../kgmMath/kgmMath.h"
#include "kgmList.h"


template <class T> class kgmOctTree{
class Node{
 public:
  box3 box;
  u32 index;
  Node *parent;
  Node *childs[8];
  bool active;
  kgmList<T> objects;
 public:
  Node(){
   index   = 0; 
   active = false;
   parent  = 0;
   memset(childs, 0, sizeof(childs));
  }
  ~Node(){
   objects.clear();
   for(int i = 0; i < 8; i++)
	if(childs[i]) 
		delete childs[i];
  }
 };
////////////////////////
 Node *root;
 u32 depth;
 box3 box;
private:
	bool insert(Node* node, T obj, box3& box){
     if(!node->box.isin(box))
		 return false;
     for(int i = 0; i < 8; i++)
		 if(node->childs[i] && 
			insert(node->childs[i], obj, box))
			return true;
     node->objects.add(obj);
	 return true;
	}
	void divide(Node* node, u32 dep){
	 if(!node || !dep)
		 return;
	 kgmVector3d<float> v = node->box.max - node->box.min;
     node->childs[0] = new Node();
	  node->childs[0]->box.min = node->box.min + vec3(0.0, 0.0, 0.0);
	  node->childs[0]->box.max = node->childs[0]->box.min + v * 0.5;
	  divide(node->childs[0], dep - 1);
     node->childs[1] = new Node();
	  node->childs[1]->box.min = node->box.min + vec3(0.5 * v.x, 0.0, 0.0);
	  node->childs[1]->box.max = node->childs[1]->box.min + v * 0.5;
	  divide(node->childs[1], dep - 1);
     node->childs[2] = new Node();
	  node->childs[2]->box.min = node->box.min + vec3(0.0, 0.5 * v.y, 0.0);
	  node->childs[2]->box.max = node->childs[2]->box.min + v * 0.5;
	  divide(node->childs[2], dep - 1);
     node->childs[3] = new Node();
	  node->childs[3]->box.min = node->box.min + vec3(0.5 * v.x, 0.5 * v.y, 0.0);
	  node->childs[3]->box.max = node->childs[3]->box.min + v * 0.5;
	  divide(node->childs[3], dep - 1);
     node->childs[4] = new Node();
	  node->childs[4]->box.min = node->box.min + vec3(0.0, 0.0, 0.5 * v.z);
	  node->childs[4]->box.max = node->childs[4]->box.min + v * 0.5;
	  divide(node->childs[4], dep - 1);
     node->childs[5] = new Node();
	  node->childs[5]->box.min = node->box.min + vec3(0.5 * v.x, 0.0, 0.5 * v.z);
	  node->childs[5]->box.max = node->childs[5]->box.min + v * 0.5;
	  divide(node->childs[5], dep - 1);
     node->childs[6] = new Node();
	  node->childs[6]->box.min = node->box.min + vec3(0.0, 0.5 * v.y, 0.5 * v.z);
	  node->childs[6]->box.max = node->childs[6]->box.min + v * 0.5;
	  divide(node->childs[6], dep - 1);
     node->childs[7] = new Node();
	  node->childs[7]->box.min = node->box.min + vec3(0.5 * v.x, 0.5 * v.y, 0.5 * v.z);
	  node->childs[7]->box.max = node->childs[4]->box.min + v * 0.5;
	  divide(node->childs[7], dep - 1);
	}
public:
	kgmOctTree(){
		root = 0;
		depth = 0;
	}
	~kgmOctTree(){
		clear();
	}

	void clear(){
		if(root) delete root;
		root = 0;
		depth = 0;
	}
	bool build(box3& b, u32 dep){
		if(root)
			clear();
		root = new Node();
		depth = dep;
		root->box   = b;
		divide(root, dep);
		return true;
	}
	bool add(T o, box3& box){
		if(!insert(root, o, box))
			root->objects.add(o);
		return true;
	}
};

 