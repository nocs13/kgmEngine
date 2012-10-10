#pragma once
#include "kgmFrustum.h"

class kgmCamera: public kgmFrustum{
  KGM_OBJECT(kgmCamera)

public:
  enum{
    cst_third_person,
    cst_first_person,
    cst_strategy
  };

 public:
  vec3  mPos, mDir, mUp;
  float mFov, mAspect, mNear, mFar;
  mtx4  mProj, mView, mVwPj;
  
  vec3 mRot;

 public:
  kgmCamera(){
    set(0.16 * PI, 1.0, .01, 100000.0, vec3(0, 0, 1), vec3(1, 0, 0), vec3(0, 0, 1));
  }

  virtual ~kgmCamera(){
  }

  void set(float fov, float asp, float zn, float zf, vec3 v, vec3 d,  vec3 u)
  {
    perspective(fov, asp, zn, zf);
    lookat(v, d, u);
    update();
 }
  
  void move(float l){
    mPos = mPos + mDir * l;
    update();
  }
  
  void move(float x, float y, float z){
    mPos = mPos + vec3(x, y, z);
    update();
  }
  
  void rotate(float pitch, float yaw){
    mRot.z += pitch;
    mRot.y += yaw;
    //mRot.x += x;
    
    //if((mRot.z >= 2.0f * PI) || (mRot.z <= - 2.0f * PI))  mRot.z = 0.0;
    
    //    if(mRot.x <= -PI) mRot.x = -PI;
    //    if(mRot.x >=  PI) mRot.x =  PI;
    
    //if(mRot.y <=  (-0.5 * PI)) mRot.y = -0.5 * PI;
    //if(mRot.y >=  ( 0.5 * PI)) mRot.y =  0.5 * PI;
    
    vec3 dz(cos(mRot.z), sin(mRot.z), 0);
    vec3 dy(cos(mRot.y), 0, sin(mRot.y));
    mDir = dz + dy;

    mtx4 mr;
    //mr.rotate(mRot);
    //mDir = mr * mRot;
    mDir.normalize();
    update();
  }

  void update(){
    mProj.perspective(mFov, mAspect, mNear, mFar);
    mView.lookat(mPos, mDir, mUp);
    mVwPj = mView * mProj;
    Frustum(mVwPj);
  }
  
protected:
  void perspective(float fov, float asp, float zn, float zf){
    mFov = fov, mAspect = asp, mNear = zn, mFar = zf;
  }
  void lookat(const vec3& pos, const vec3& dir, const vec3& up){
    mPos = pos, mDir = dir, mUp = up;
  }
};
