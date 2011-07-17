#pragma once
#include <math.h>
#include <float.h>

#define PI          3.1415926535f
#define SQR(x)      ((x) * (x))
#define DEGTORAD(x) ((x) * PI / 180.0)
#define RADTODEG(x) (x*180.0/PI)
#define MIN(x,y)    (x<y)?(x):(y)
#define MAX(x,y)    (x>y)?(x):(y)


typedef unsigned int     uint;
typedef unsigned char    uchar;
typedef unsigned short   ushort;


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

typedef kgmQuaternion<float> quat;

typedef kgmVector3d<float>   vec3;
typedef kgmVector3d<float>   float3;
typedef kgmVector3d<uint>    uint3;
typedef kgmVector3d<uchar>   uchar3;
typedef kgmVector3d<ushort>  ushort3;

typedef kgmVector2d<float> float2;
typedef kgmVector2d<float> vec2;
typedef kgmVector2d<uint>  uint2;
typedef kgmVector2d<int>   int2;
typedef kgmVector2d<long>  long2;
typedef kgmVector2d<char>  char2;

typedef kgmVector4d<float>   vec4;
typedef kgmVector4d<float>   float4;
typedef kgmVector4d<uint>    uint4;
typedef kgmVector4d<uchar>   uchar4;
typedef kgmVector4d<ushort>  ushort4;

typedef kgmMatrix3d<float> mtx4;

typedef kgmPlane3d<float> plane;
typedef kgmPlane3d<float> plane3;
typedef kgmPolygon3d<float>  polygon;
typedef kgmPolygon3d<float>  polygon3;
typedef kgmTriangle3d<float> triangle;
typedef kgmTriangle3d<float> triangle3;
typedef kgmCylinder3d<float> cylinder;
typedef kgmCylinder3d<float> cylinder3;
typedef kgmBox2d<float> box2;
typedef kgmBox3d<float> box3;
typedef kgmBox3d<float> box;
typedef kgmLine3d<float> line;
typedef kgmLine3d<float> line3;
typedef kgmSphere3d<float> sphere;
typedef kgmSphere3d<float> sphere3;

typedef kgmBox2d<float> box2;
typedef kgmBox2d<int>   iRect;
typedef kgmBox2d<uint>  uRect;
typedef kgmBox2d<long>  lRect;
typedef kgmBox2d<float> fRect;



inline float fround(float n, unsigned d){
 return (float)(floor(n * powf(10., d) + 0.5) / powf(10., d));
}
inline double fround(double n, unsigned d){
 return floor(n * powf(10., d) + 0.5) / powf(10., d);
}