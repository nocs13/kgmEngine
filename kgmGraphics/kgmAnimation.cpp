#include "kgmAnimation.h"

KGMOBJECT_IMPLEMENT(kgmAnimation, kgmResource);

kgmAnimation::Animation::Animation(){

}

kgmAnimation::Animation::~Animation(){
 m_frames.clear();
}

void kgmAnimation::Animation::addFrame(float time, vec3 pos, quat rot){
 Frame frame;
 frame.time = time,
 frame.pos = pos,
 frame.rot = rot;
 m_frames.add(frame);
}

bool kgmAnimation::Animation::getFrame(float time, mtx4& m)
{
 Frame frm;
 float an_time = 0;
 float c_time = 0;
 int   c_frame = 0;
 int   i = 0;

 if(!m_frames.size())
  return false;

 an_time = m_frames[m_frames.size() - 1].time - m_frames[0].time;
 if(time < m_frames[0].time)
  return false;

 if(time <= an_time)
  c_time = time;
 else
  c_time = (int)time % (int)an_time;

 for(i = 0; i < m_frames.size() - 1; i++){
  if(c_time == m_frames[i].time){
   c_frame = i;
	 break;
  }
  if(c_time == m_frames[i + 1].time){
   c_frame = i + 1;
	 break;
  }
  if(c_time > m_frames[i].time && c_time < m_frames[i + 1].time){
   c_frame = i;
	 break;
  }
 }

 m = mtx4(m_frames[i].rot, m_frames[i].pos);

 return true;
}

bool kgmAnimation::Animation::getFrame(float time, vec3& pos, quat& rot){
 Frame frm;
 u32 an_time = m_frames[m_frames.size() - 1].time - m_frames[0].time;
 u32 c_time = 0;
 int  c_frame = 0;
 int  i = 0;

 if(time < m_frames[0].time)
	 return false;
 if(time <= an_time)
  c_time = time;
 else
  c_time = (int)time % (int)an_time;

 for(i = 0; i < m_frames.size() - 1; i++){
  if(c_time == m_frames[i].time){
   c_frame = i;
	 break;
  }
  if(c_time == m_frames[i + 1].time){
   c_frame = i + 1;
	 break;
  }
  if(c_time > m_frames[i].time && c_time < m_frames[i + 1].time){
   c_frame = i;
	 break;
  }
 }
 rot = m_frames[i].rot;
 pos = m_frames[i].pos;
 return true;
}


bool kgmAnimation::Animation::getFrameSlerp(float time, mtx4& m)
{
  Frame frm;
  u32 keys = m_frames.size();
  u32 utime = time;
  u32 vtime = (u32)(time) % (u32)(m_frames[m_frames.size() - 1].time - m_frames[0].time);
  if(!keys)
    return false;

  if(time < m_frames[0].time){
    m = mtx4(m_frames[0].rot, m_frames[0].pos);
    return true;
  }

  if(time > m_frames[keys - 1].time){
    m = mtx4(m_frames[keys - 1].rot, m_frames[keys - 1].pos);
    return true;
  }

  for(u32 i = 0; i < (keys); i++)
  {
    if(time == m_frames[i].time){
      m = mtx4(m_frames[i].rot, m_frames[i].pos);
      return true;
    }

    if((time > m_frames[i].time) && (time < m_frames[i + 1].time)){
      u32 dt = m_frames[i+1].time - m_frames[i].time;
      u32 dtm = time - m_frames[i].time;
      float f_tm = dtm * (1.0f / dt);
      frm.pos = m_frames[i].pos.slerp(m_frames[i+1].pos, f_tm);
      frm.rot = m_frames[i].rot.slerp(m_frames[i+1].rot, f_tm);

      m = mtx4(frm.rot, frm.pos);
      return true;
    }
  }
  return true;
}

