#pragma once
#include "../kgmBase/kgmList.h"
#include "../kgmBase/kgmResource.h"
#include "../kgmMath/kgmMath.h"

// TIME IN MILLISECONDS
//animation for animated groups
class kgmAnimation: public kgmResource{
  KGM_OBJECT(kgmAnimation)

public:
  struct Frame{
    float time;
    vec3  pos;
    quat  rot;
  };

  class Animation{
  public:
    kgmString      m_name;
    kgmList<Frame> m_frames;

  public:
    Animation();
    virtual ~Animation();

    void addFrame(float time, vec3 pos, quat rot);
    bool getFrame(float time, vec3& pos, quat& rot);
    bool getFrame(float time, mtx4& m);
    bool getFrameSlerp(float time, mtx4& m);
  };
 
public:
 u32   m_start, m_end, m_fps;
 kgmList<Animation*> m_nodes;

 kgmAnimation():kgmResource(){
  m_start = m_end = 0, m_fps = 1;
 }
 virtual ~kgmAnimation(){
  clear();
 }

 void clear(){
  for(int i = 0; i < m_nodes.length(); i++)
   delete m_nodes[i];
  m_nodes.clear();
 }

 u32 count(){
  return m_nodes.length();
 }

 u32 getFullTime(){
  return m_fps * (m_end - m_start);
 }

 bool addNode(Animation* node){
  if(!node)
   return false;
  m_nodes.add(node);
  return true;
 }

 Animation* getNode(u32 id){
   if(id >= m_nodes.length())
     return 0;
   return m_nodes[id];
 }

 Animation* getNode(char* id){
  for(u32 i = 0; i < m_nodes.length(); i++)
   if(!strcmp(id, m_nodes[i]->m_name))
    return m_nodes[i];
  return 0;
 }

 Animation* getNode(kgmString id){
  for(u32 i = 0; i < m_nodes.length(); i++)
   if(id == m_nodes[i]->m_name)
    return m_nodes[i];
  return 0;
 }

 bool getFrames(mtx4 *pmtx, u32 mtx_cnt, u32 time)
 {
  u32 n = (m_nodes.length() < mtx_cnt)?(m_nodes.length()):(mtx_cnt);
  if(!pmtx)
	 return false;
  for(u32 i = 0; i < n; i++){
   m_nodes[i]->getFrame(time, pmtx[i]);
  }
  return true;
 }
};

typedef kgmList<kgmAnimation*> kgmAnimationList;
