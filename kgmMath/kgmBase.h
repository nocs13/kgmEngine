#pragma once
#include <math.h>
#include <float.h>
#include "../kgmBase/kgmTypes.h"

#define PI          3.1415926535f
#define SQR(x)      ((x) * (x))
#define DEGTORAD(x) ((x) * PI / 180.0)
#define RADTODEG(x) (x*180.0/PI)
#define MIN(x,y)    (x<y)?(x):(y)
#define MAX(x,y)    (x>y)?(x):(y)


template <class T> class kgmVector2d;
template <class T> class kgmVector3d;
template <class T> class kgmVector4d;
template <class T> class kgmMatrix3d;
template <class T> class kgmQuaternion;
template <class T> class kgmBox3d;
template <class T> class kgmBox2d;
template <class T> class kgmLine3d;
template <class T> class kgmRay3d;
template <class T> class kgmPlane3d;
template <class T> class kgmPolygon3d;
template <class T> class kgmTriangle3d;
template <class T> class kgmSphere3d;
template <class T> class kgmCylinder3d;
template <class T> class kgmFrustum3d;

typedef kgmQuaternion<f32> quat;

typedef kgmVector3d<f32>   vec3;
typedef kgmVector3d<f32>   float3;
typedef kgmVector3d<u32>    u323;
typedef kgmVector3d<u8>   uchar3;
typedef kgmVector3d<u16>  u163;

typedef kgmVector2d<f32> float2;
typedef kgmVector2d<f32> vec2;
typedef kgmVector2d<u32>  u322;
typedef kgmVector2d<s32>   int2;
typedef kgmVector2d<s8>  char2;

typedef kgmVector4d<s32>   vec4;
typedef kgmVector4d<f32>   float4;
typedef kgmVector4d<u32>    u324;
typedef kgmVector4d<u8>   uchar4;
typedef kgmVector4d<u16>  u164;

typedef kgmMatrix3d<f32> mtx4;
typedef kgmMatrix3d<f32> kgmTransform;

typedef kgmPlane3d<f32> plane;
typedef kgmPlane3d<f32> plane3;
typedef kgmPolygon3d<f32>  polygon;
typedef kgmPolygon3d<f32>  polygon3;
typedef kgmTriangle3d<f32> triangle;
typedef kgmTriangle3d<f32> triangle3;
typedef kgmCylinder3d<f32> cylinder;
typedef kgmCylinder3d<f32> cylinder3;
typedef kgmBox2d<f32> box2;
typedef kgmBox3d<f32> box3;
typedef kgmBox3d<f32> box;
typedef kgmLine3d<f32> line;
typedef kgmLine3d<f32> line3;
typedef kgmSphere3d<f32> sphere;
typedef kgmSphere3d<f32> sphere3;

typedef kgmBox2d<f32> box2;
typedef kgmBox2d<s32>   iRect;
typedef kgmBox2d<u32>  uRect;
typedef kgmBox2d<f32> fRect;



inline f32 fround(f32 n, u32 d){
 return (f32)(floor(n * powf(10., d) + 0.5) / powf(10., d));
}

inline f32 fround(f64 n, u32 d){
 return floor(n * powf(10., d) + 0.5) / powf(10., d);
}
