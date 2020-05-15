#pragma once
#include "kgmFrustum.h"

class kgmCamera: public kgmFrustum
{
  KGM_OBJECT(kgmCamera);

public:
  enum
  {
    cst_third_person,
    cst_first_person,
    cst_strategy
  };

  struct Ortho
  {
    bool  on;

    float scale;
    float width;
    float height;
  };

public:
  vec3  mPos, mDir, mUp;
  float mFov, mAspect, mNear, mFar;
  mtx4  mProj, mView, mVwPj;
  
  vec3 mRot;

  Ortho ortho;

public:
  kgmCamera();
  ~kgmCamera();

  void set(float fov, float asp, float zn, float zf, vec3 v, vec3 d,  vec3 u);
  void move(float l);

  void move(float x, float y, float z);
  void look(float x, float y, float z);
  
  void rotate(float pitch, float yaw);
  void update();
  void viewport(float width, float height);
  void scale(float scale);
  void setOrthogonal(bool on);

  bool isOrthogonal() const;
  fRect getOrthoView() const;

  vec3 position()  const;
  vec3 direction() const;
  vec3 upside()    const;

protected:
  void perspective(float fov, float asp, float zn, float zf);
  void lookat(const vec3& pos, const vec3& dir, const vec3& up);
};
