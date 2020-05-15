#include  "kgmCamera.h"

kgmCamera::kgmCamera()
{
  memset(&ortho, 0, sizeof(Ortho));

  ortho.scale = 1.0;

  set(0.16 * PI, 1.0, .01, 100000.0, vec3(0, 0, 1), vec3(1, 0, 0), vec3(0, 0, 1));
}

kgmCamera::~kgmCamera()
{
}

inline void kgmCamera::set(float fov, float asp, float zn, float zf, vec3 v, vec3 d,  vec3 u)
{
  perspective(fov, asp, zn, zf);
  lookat(v, d, u);
  update();
}

inline void kgmCamera::move(float l)
{
  mPos = mPos + mDir * l;

  update();
}

void kgmCamera::move(float x, float y, float z)
{
  mPos = mPos + vec3(x, y, z);

  update();
}

void kgmCamera::look(float x, float y, float z)
{
  mDir.set(x, y, z);

  update();
}

inline void kgmCamera::rotate(float pitch, float yaw)
{
  mRot.z += pitch;
  //mRot.y += yaw;
  //mRot.x += x;

  //if((mRot.z >= 2.0f * PI) || (mRot.z <= - 2.0f * PI))  mRot.z = 0.0;

  //if(mRot.x <= -PI) mRot.x = -PI;
  //if(mRot.x >=  PI) mRot.x =  PI;

  //if(mRot.y <=  (-0.5 * PI)) mRot.y = -0.5 * PI;
  //if(mRot.y >=  ( 0.5 * PI)) mRot.y =  0.5 * PI;

  vec3 dz(cos(mRot.z), sin(mRot.z), 0);
  vec3 dy(cos(mRot.y), 0, sin(mRot.y));

  dz.normalize();
  dy.normalize();

  mDir = dz;

  mtx4 mr;
  //mr.rotate(mRot);
  //mDir = mr * mRot;
  mDir.normalize();
  update();
}

inline void kgmCamera::update()
{
  if(ortho.on)
  {
    mProj.identity();
    mProj.ortho(-ortho.scale, ortho.scale, -ortho.scale, ortho.scale, 0, 100000);
    //mProj.ortho(0.0, ortho.width, 0.0, ortho.height, 0, 100000);
    //mProj.ortho(-ortho.scale * ortho.width/ortho.height,
    //            ortho.scale * ortho.width/ortho.height, -ortho.scale, ortho.scale,
    //            0.0, 1000000);

    mView.identity();
    //mView.ortho(0.0, ortho.width, ortho.height, 0.0, 1, -1);
    mView.lookat(mPos, mDir, mUp);
  }
  else
  {
    mProj.perspective(mFov, mAspect, mNear, mFar);
    mView.lookat(mPos, mDir, mUp);
  }

  mVwPj = mView * mProj;

  frustum(mVwPj);
}

void kgmCamera::viewport(float width, float height)
{
  ortho.width = width;
  ortho.height = height;
}

inline void kgmCamera::scale(float scale)
{
  if(scale < 1)
    return;

  ortho.scale = scale;
}

inline void kgmCamera::perspective(float fov, float asp, float zn, float zf)
{
  mFov = fov, mAspect = asp, mNear = zn, mFar = zf;
}

inline void kgmCamera::lookat(const vec3& pos, const vec3& dir, const vec3& up)
{
  mPos = pos, mDir = dir, mUp = up;
}

inline void kgmCamera::setOrthogonal(bool on)
{
  ortho.on = on;
}

inline bool kgmCamera::isOrthogonal() const
{
  return ortho.on;
}

inline fRect kgmCamera::getOrthoView() const
{
  return fRect(-ortho.scale * ortho.width/ortho.height,
               ortho.scale * ortho.width/ortho.height,
               -ortho.scale, ortho.scale);
}

inline vec3 kgmCamera::position() const
{
  return mPos;
}

inline vec3 kgmCamera::direction() const
{
  return mDir;
}

inline vec3 kgmCamera::upside() const
{
  return mUp;
}
