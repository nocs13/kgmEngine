#pragma once
//#include "kgmVector3d.h"
//#include "kgmVector4d.h"
//#include "kgmQuaternion.h"

//MATRIX4x4
template <class T> class kgmMatrix3d{
public:
 T m[16];
public:
 kgmMatrix3d(){
  identity();
 }
 kgmMatrix3d(T *f){
  memcpy(m, f, sizeof(T) * 16);
 }
 kgmMatrix3d(kgmVector3d<T>& v){
  identity();
  translate(v);
 }
 kgmMatrix3d(kgmQuaternion<T> &q){
  T x2 = q.x + q.x,
  y2 = q.y + q.y,
  z2 = q.z + q.z,
  xx = q.x * x2,
  yy = q.y * y2,
  zz = q.z * z2,
  xy = q.x * y2,
  yz = q.y * z2,
  zx = q.z * x2,
  wx = q.w * x2,
  wy = q.w * y2,
  wz = q.w * z2;
  m[0] = 1.0f - (yy + zz);
   m[1] = xy + wz;
    m[2] = zx - wy;
  m[4] = xy - wz;
   m[5] = 1.0f - (xx + zz);
    m[6] = yz + wx;
  m[8] = zx + wy;
   m[9] = yz - wx;
    m[10] = 1.0f - (xx + yy);
  m[3] = m[7] = m[11] =
  m[12] = m[13] = m[14] = 0.0f;
  m[15] = 1.0f;
 }
 kgmMatrix3d(kgmQuaternion<T> &q, kgmVector3d<T> &v){
  kgmMatrix3d mrt(q);
  kgmMatrix3d mtr(v);
  *this = mrt * mtr;
 }

 T& operator[](int i){
   return m[i];
 }
 T& operator()(int row, int col){
  return m[col * 4 + row];
 }
 kgmMatrix3d operator+(kgmMatrix3d& a){
  kgmMatrix3d r;
  r.m[0] = m[0]+a.m[0];
  r.m[1] = m[1]+a.m[1];
  r.m[2] = m[2]+a.m[2];
  r.m[3] = m[3]+a.m[3];

  r.m[4] = m[4]+a.m[4];
  r.m[5] = m[5]+a.m[5];
  r.m[6] = m[6]+a.m[6];
  r.m[7] = m[7]+a.m[7];

  r.m[8] = m[8]+a.m[8];
  r.m[9] = m[9]+a.m[9];
  r.m[10] = m[10]+a.m[10];
  r.m[11] = m[11]+a.m[11];

  r.m[12] = m[12]+a.m[12];
  r.m[13] = m[13]+a.m[13];
  r.m[14] = m[14]+a.m[14];
  r.m[15] = m[15]+a.m[15];
  return r;
 }
 kgmMatrix3d& operator*=(T f){
  m[0] += m[0] * f;
  m[1] += m[1] * f;
  m[2] += m[2] * f;
  m[3] += m[3] * f;
  m[4] += m[4] * f;
  m[5] += m[5] * f;
  m[6] += m[6] * f;
  m[7] += m[7] * f;
  m[8] += m[8] * f;
  m[9] += m[9] * f;
  m[10] += m[10] * f;
  m[11] += m[11] * f;
  m[12] += m[12] * f;
  m[13] += m[13] * f;
  m[14] += m[14] * f;
  m[15] += m[15] * f;
  return *this;
 }
 kgmMatrix3d operator*(T f){
  kgmMatrix3d r;
  r.m[0] = m[0] * f;
  r.m[1] = m[1] * f;
  r.m[2] = m[2] * f;
  r.m[3] = m[3] * f;
  r.m[4] = m[4] * f;
  r.m[5] = m[5] * f;
  r.m[6] = m[6] * f;
  r.m[7] = m[7] * f;
  r.m[8] = m[8] * f;
  r.m[9] = m[9] * f;
  r.m[10] = m[10] * f;
  r.m[11] = m[11] * f;
  r.m[12] = m[12] * f;
  r.m[13] = m[13] * f;
  r.m[14] = m[14] * f;
  r.m[15] = m[15] * f;
  return r;
 }
 kgmMatrix3d operator*(kgmMatrix3d& a){
  kgmMatrix3d r;
  r.m[0] = m[0]*a.m[0]+m[1]*a.m[4]+m[2]*a.m[8]+m[3]*a.m[12];
  r.m[1] = m[0]*a.m[1]+m[1]*a.m[5]+m[2]*a.m[9]+m[3]*a.m[13];
  r.m[2] = m[0]*a.m[2]+m[1]*a.m[6]+m[2]*a.m[10]+m[3]*a.m[14];
  r.m[3] = m[0]*a.m[3]+m[1]*a.m[7]+m[2]*a.m[11]+m[3]*a.m[15];

  r.m[4] = m[4]*a.m[0]+m[5]*a.m[4]+m[6]*a.m[8]+m[7]*a.m[12];
  r.m[5] = m[4]*a.m[1]+m[5]*a.m[5]+m[6]*a.m[9]+m[7]*a.m[13];
  r.m[6] = m[4]*a.m[2]+m[5]*a.m[6]+m[6]*a.m[10]+m[7]*a.m[14];
  r.m[7] = m[4]*a.m[3]+m[5]*a.m[7]+m[6]*a.m[11]+m[7]*a.m[15];

  r.m[8] = m[8]*a.m[0]+m[9]*a.m[4]+m[10]*a.m[8]+m[11]*a.m[12];
  r.m[9] = m[8]*a.m[1]+m[9]*a.m[5]+m[10]*a.m[9]+m[11]*a.m[13];
  r.m[10] = m[8]*a.m[2]+m[9]*a.m[6]+m[10]*a.m[10]+m[11]*a.m[14];
  r.m[11] = m[8]*a.m[3]+m[9]*a.m[7]+m[10]*a.m[11]+m[11]*a.m[15];

  r.m[12] = m[12]*a.m[0]+m[13]*a.m[4]+m[14]*a.m[8]+m[15]*a.m[12];
  r.m[13] = m[12]*a.m[1]+m[13]*a.m[5]+m[14]*a.m[9]+m[15]*a.m[13];
  r.m[14] = m[12]*a.m[2]+m[13]*a.m[6]+m[14]*a.m[10]+m[15]*a.m[14];
  r.m[15] = m[12]*a.m[3]+m[13]*a.m[7]+m[14]*a.m[11]+m[15]*a.m[15];
  return r;
 }
 kgmVector4d<T> operator*(kgmVector4d<T>& v){
  kgmVector4d<T> r;
  r.x = v.x*m[0] + v.y*m[4] + v.z*m[8] + v.w*m[12];
  r.y = v.x*m[1] + v.y*m[5] + v.z*m[9] + v.w*m[13];
  r.z = v.x*m[2] + v.y*m[6] + v.z*m[10] + v.w*m[14];
  r.w = v.x*m[3] + v.y*m[7] + v.z*m[11] + v.w*m[15];
  return r;
 }
 kgmVector3d<T> operator*(kgmVector3d<T>& v){
  kgmVector3d<T> r;
  r.x = v.x*m[0] + v.y*m[4] + v.z*m[8] + m[12];
  r.y = v.x*m[1] + v.y*m[5] + v.z*m[9] + m[13];
  r.z = v.x*m[2] + v.y*m[6] + v.z*m[10] + m[14];
  return r;
 }

 void identity(){
  m[1] = m[2] = m[3] = m[4] = m[6] = m[7] = m[8] =
  m[9] = m[11] = m[12] = m[13] = m[14] = 0;

  m[0] = m[5] = m[10] = m[15] = 1;
 }
 void angles(kgmVector3d<T>& r){
  double angle_x, angle_y, angle_z, c, tr_x, tr_y, tr_z;
  r.x = (T)asin(m[6]);
  r.y = (T)asin(m[8]);
  r.z = (T)asin(m[1]);

  angle_y = -asin(m[2]);
  c		 =  cos(angle_y);
  if(fabs(c) > 0.005){
   tr_x      =  m[10] / c;
   tr_y      = -m[6]  / c;
   angle_x  = atan2( tr_y, tr_x );
   tr_x      =  m[0] / c;
   tr_y      = -m[1] / c;
   angle_z  = atan2( tr_y, tr_x );
  }else{
   angle_x  = 0;
   tr_x      = m[5];
   tr_y      = m[4];
   angle_z  = atan2( tr_y, tr_x );
  }
/*
  if(angle_x < 0.0) angle_x = 0.0;
   if(angle_x > (2 * PI)) angle_x = (2 * PI);
  if(angle_y < 0.0) angle_y = 0.0;
   if(angle_y > (2 * PI)) angle_y = (2 * PI);
  if(angle_z < 0.0) angle_z = 0.0;
   if(angle_z > (2 * PI)) angle_z = (2 * PI);
*/
  r.x = (T)angle_x;
  r.y = (T)angle_y;
  r.z = (T)angle_z;
  /*
  angle_y = D = -asin( mat[2]);
    C           =  cos( angle_y );
    angle_y    *= RADIANS;
    if ( fabs( C ) > 0.005 )
    {
     trx      =  mat[10] / C;
     try      = -mat[6]  / C;
     angle_x  = atan2( try, trx ) * RADIANS;
     trx      =  mat[0] / C;
     try      = -mat[1] / C;
     angle_z  = atan2( try, trx ) * RADIANS;
    }else{
     angle_x  = 0;
     trx      = mat[5];
     try      = mat[4];
     angle_z  = atan2( try, trx ) * RADIANS;
    }
    angle_x = clamp( angle_x, 0, 360 );
    angle_y = clamp( angle_y, 0, 360 );
    angle_z = clamp( angle_z, 0, 360 );
  */
 }
 void translate(T x, T y, T z){
  m[12] = x,  m[13] = y,  m[14] = z;
 }
 void translate(kgmVector3d<T>& v){
  translate(v.x, v.y, v.z);
 }


 void rotate(T x, T y, T z){
  T cp = (T)cos(x);
  T sp = (T)sin(x);
  T cy = (T)cos(y);
  T sy = (T)sin(y);
  T cr = (T)cos(z);
  T sr = (T)sin(z);

  m[0] = cy*cr;
   m[1] = -sr*cy;
    m[2] = sy;

  m[4] = sp*sy*cr + sr*cp;
   m[5] = sp*sy*sr + cp*cr;
    m[6] = -sp*cy;

  m[8] = -sy*cp*cr + sp*sr;
   m[9] = sr*sy*cp + sr*sp;
    m[10] = cp*cy;
 }
 void rotate(kgmVector3d<T> &r){
  rotate(r.x, r.y, r.z);
 }

 void rotate(T angle, kgmVector3d<T> &axis) {
  T rad = angle;
  T c = cos(rad);
  T s = sin(rad);
  kgmVector3d<T> v = axis;
  v.normalize();
  float xy = v.x * v.y;
  float yz = v.y * v.z;
  float zx = v.z * v.x;
  float xs = v.x * s;
  float ys = v.y * s;
  float zs = v.z * s;
  m[0] = (1.0f - c) * v.x * v.x + c; m[4] = (1.0f - c) * xy - zs; m[8] = (1.0f - c) * zx + ys; m[12] = 0.0;
  m[1] = (1.0f - c) * xy + zs; m[5] = (1.0f - c) * v.y * v.y + c; m[9] = m[6] = (1.0f - c) * yz - xs; m[13] = 0.0;
  m[2] = (1.0f - c) * zx - ys; m[6] = (1.0f - c) * yz + xs; m[10] = (1.0f - c) * v.z * v.z + c; m[14] = 0.0;
  m[3] = 0.0; m[7] = 0.0; m[11] = 0.0; m[15] = 1.0;
 }

 void rotate(T angle, kgmVector3d<T> &axis, kgmVector3d<T> &point) {
  kgmVector3d<T> v;
  kgmMatrix3d<T> mtr, mrt, mts;
  mrt.rotate(angle, axis);
  v = mrt * point;
  mtr.translate(v.inverse());
  (*this) = mrt * mtr;
 }

 void scale(T x, T y, T z){
  m[0] = x;
  m[5] = y;
  m[10] = z;
 }
 void scale(kgmVector3d<T> &v){
  scale(v.x, v.y, v.z);
 }

 T determine(){
  return det(m[0], m[1], m[2], m[3], m[4], m[5], m[6], m[7],
           m[8], m[9], m[10], m[11], m[12], m[13], m[14], m[15]);
 }

 void invert(){
  kgmMatrix3d<T> &m = *this;
  kgmMatrix3d<T> out;

  T d = (m(0, 0) * m(1, 1) - m(1, 0) * m(0, 1)) * (m(2, 2) * m(3, 3) - m(3, 2) * m(2, 3))	- (m(0, 0) * m(2, 1) - m(2, 0) * m(0, 1)) * (m(1, 2) * m(3, 3) - m(3, 2) * m(1, 3))
      + (m(0, 0) * m(3, 1) - m(3, 0) * m(0, 1)) * (m(1, 2) * m(2, 3) - m(2, 2) * m(1, 3))	+ (m(1, 0) * m(2, 1) - m(2, 0) * m(1, 1)) * (m(0, 2) * m(3, 3) - m(3, 2) * m(0, 3))
    - (m(1, 0) * m(3, 1) - m(3, 0) * m(1, 1)) * (m(0, 2) * m(2, 3) - m(2, 2) * m(0, 3))	+ (m(2, 0) * m(3, 1) - m(3, 0) * m(2, 1)) * (m(0, 2) * m(1, 3) - m(1, 2) * m(0, 3));

  if (d == 0)
   return;

  d = 1 / d;

  out(0, 0) = d * (m(1, 1) * (m(2, 2) * m(3, 3) - m(3, 2) * m(2, 3)) + m(2, 1) * (m(3, 2) * m(1, 3) - m(1, 2) * m(3, 3)) + m(3, 1) * (m(1, 2) * m(2, 3) - m(2, 2) * m(1, 3)));
  out(1, 0) = d * (m(1, 2) * (m(2, 0) * m(3, 3) - m(3, 0) * m(2, 3)) + m(2, 2) * (m(3, 0) * m(1, 3) - m(1, 0) * m(3, 3)) + m(3, 2) * (m(1, 0) * m(2, 3) - m(2, 0) * m(1, 3)));
  out(2, 0) = d * (m(1, 3) * (m(2, 0) * m(3, 1) - m(3, 0) * m(2, 1)) + m(2, 3) * (m(3, 0) * m(1, 1) - m(1, 0) * m(3, 1)) + m(3, 3) * (m(1, 0) * m(2, 1) - m(2, 0) * m(1, 1)));
  out(3, 0) = d * (m(1, 0) * (m(3, 1) * m(2, 2) - m(2, 1) * m(3, 2)) + m(2, 0) * (m(1, 1) * m(3, 2) - m(3, 1) * m(1, 2)) + m(3, 0) * (m(2, 1) * m(1, 2) - m(1, 1) * m(2, 2)));
  out(0, 1) = d * (m(2, 1) * (m(0, 2) * m(3, 3) - m(3, 2) * m(0, 3)) + m(3, 1) * (m(2, 2) * m(0, 3) - m(0, 2) * m(2, 3)) + m(0, 1) * (m(3, 2) * m(2, 3) - m(2, 2) * m(3, 3)));
  out(1, 1) = d * (m(2, 2) * (m(0, 0) * m(3, 3) - m(3, 0) * m(0, 3)) + m(3, 2) * (m(2, 0) * m(0, 3) - m(0, 0) * m(2, 3)) + m(0, 2) * (m(3, 0) * m(2, 3) - m(2, 0) * m(3, 3)));
  out(2, 1) = d * (m(2, 3) * (m(0, 0) * m(3, 1) - m(3, 0) * m(0, 1)) + m(3, 3) * (m(2, 0) * m(0, 1) - m(0, 0) * m(2, 1)) + m(0, 3) * (m(3, 0) * m(2, 1) - m(2, 0) * m(3, 1)));
  out(3, 1) = d * (m(2, 0) * (m(3, 1) * m(0, 2) - m(0, 1) * m(3, 2)) + m(3, 0) * (m(0, 1) * m(2, 2) - m(2, 1) * m(0, 2)) + m(0, 0) * (m(2, 1) * m(3, 2) - m(3, 1) * m(2, 2)));
  out(0, 2) = d * (m(3, 1) * (m(0, 2) * m(1, 3) - m(1, 2) * m(0, 3)) + m(0, 1) * (m(1, 2) * m(3, 3) - m(3, 2) * m(1, 3)) + m(1, 1) * (m(3, 2) * m(0, 3) - m(0, 2) * m(3, 3)));
  out(1, 2) = d * (m(3, 2) * (m(0, 0) * m(1, 3) - m(1, 0) * m(0, 3)) + m(0, 2) * (m(1, 0) * m(3, 3) - m(3, 0) * m(1, 3)) + m(1, 2) * (m(3, 0) * m(0, 3) - m(0, 0) * m(3, 3)));
  out(2, 2) = d * (m(3, 3) * (m(0, 0) * m(1, 1) - m(1, 0) * m(0, 1)) + m(0, 3) * (m(1, 0) * m(3, 1) - m(3, 0) * m(1, 1)) + m(1, 3) * (m(3, 0) * m(0, 1) - m(0, 0) * m(3, 1)));
  out(3, 2) = d * (m(3, 0) * (m(1, 1) * m(0, 2) - m(0, 1) * m(1, 2)) + m(0, 0) * (m(3, 1) * m(1, 2) - m(1, 1) * m(3, 2)) + m(1, 0) * (m(0, 1) * m(3, 2) - m(3, 1) * m(0, 2)));
  out(0, 3) = d * (m(0, 1) * (m(2, 2) * m(1, 3) - m(1, 2) * m(2, 3)) + m(1, 1) * (m(0, 2) * m(2, 3) - m(2, 2) * m(0, 3)) + m(2, 1) * (m(1, 2) * m(0, 3) - m(0, 2) * m(1, 3)));
  out(1, 3) = d * (m(0, 2) * (m(2, 0) * m(1, 3) - m(1, 0) * m(2, 3)) + m(1, 2) * (m(0, 0) * m(2, 3) - m(2, 0) * m(0, 3)) + m(2, 2) * (m(1, 0) * m(0, 3) - m(0, 0) * m(1, 3)));
  out(2, 3) = d * (m(0, 3) * (m(2, 0) * m(1, 1) - m(1, 0) * m(2, 1)) + m(1, 3) * (m(0, 0) * m(2, 1) - m(2, 0) * m(0, 1)) + m(2, 3) * (m(1, 0) * m(0, 1) - m(0, 0) * m(1, 1)));
  out(3, 3) = d * (m(0, 0) * (m(1, 1) * m(2, 2) - m(2, 1) * m(1, 2)) + m(1, 0) * (m(2, 1) * m(0, 2) - m(0, 1) * m(2, 2)) + m(2, 0) * (m(0, 1) * m(1, 2) - m(1, 1) * m(0, 2)));
  *this = out;
 }
 void fast_invert(){
  kgmMatrix3d<T>  r;
  float idet = det(m[0], m[1], m[2],  m[4], m[5], m[6],  m[8], m[9], m[10]);
  float ndet = det();
  idet = 1.0f / idet;
  r.m[0] = (m[5]*m[10]-m[9]*m[6])*idet;
  r.m[1] = -(m[1]*m[10]-m[9]*m[2])*idet;
  r.m[2] = (m[1]*m[6]-m[5]*m[2])*idet;
  r.m[3] = 0.0f;
  r.m[4] = -(m[4]*m[10]-m[8]*m[6])*idet;
  r.m[5] = (m[0]*m[10]-m[8]*m[2])*idet;
  r.m[6] = -(m[0]*m[6]-m[4]*m[2])*idet;
  r.m[7] = 0.0f;
  r.m[8] = (m[4]*m[9]-m[8]*m[5])*idet;
  r.m[9] = -(m[0]*m[9]-m[8]*m[1])*idet;
  r.m[10] = (m[0]*m[5]-m[4]*m[1])*idet;
  r.m[11] = 0.0f;
  r.m[12] = -(m[12]*r.m[0]+m[13]*r.m[4]+m[14]*r.m[8]);
  r.m[13] = -(m[12]*r.m[1]+m[13]*r.m[5]+m[14]*r.m[9]);
  r.m[14] = -(m[12]*r.m[2]+m[13]*r.m[6]+m[14]*r.m[10]);
  r.m[15] = 1.0f;
  *this = r;
 }


 void quaternion(kgmVector4d<T> &q, kgmVector3d<T> &v){
  kgmMatrix3d<T> mtr, mrt;
  mrt.quaternion(q);
  mtr.translate(v);
  mul(*this, mrt, mtr);
 }
//matrix perspective
 void perspective(T fov, T asp, T znear, T zfar)
 {
   T y = (T)tan(fov),	 x = y * asp;
   m[0] =  1.0f/x;       m[1] =                                   m[2] = m[3] = 0.0f;
   m[4] =  0.0f;         m[5] = 1.0f / y;                         m[6] = m[7] = 0.0f;
   m[8] =  m[9] = 0.0f;  m[10] = -(zfar + znear)/(zfar-znear);    m[11] = -1.0f;
   m[12] = m[13] = 0.0f; m[14] = -(2.0f*zfar*znear)/(zfar-znear); m[15] = 0.0f;
 }

//matrix lookat for 3D
 void lookat(const kgmVector3d<T>& eye, const kgmVector3d<T>& dir, const kgmVector3d<T>& up)
 {
   kgmVector3d<T> x, y, z, ieye = eye;
   kgmMatrix3d<T> m0, m1;
   z = dir * (-1);
   z.normalize();
   x = up.cross(z);
   x.normalize();
   y = z.cross(x);
   y.normalize();
   m0.m[0] = x.x; m0.m[1] = y.x; m0.m[2] = z.x;  m0.m[3] = 0.0f;
   m0.m[4] = x.y; m0.m[5] = y.y; m0.m[6] = z.y;  m0.m[7] = 0.0f;
   m0.m[8] = x.z; m0.m[9] = y.z; m0.m[10] = z.z; m0.m[11] = 0.0f;
   m0.m[12] = 0.0f; m0.m[13] = 0.0f; m0.m[14] = 0.0f; m0.m[15] = 1.0f;
   ieye.invert();
   m1.identity();
   m1.translate(ieye.x, ieye.y, ieye.z);
   *this =  m1 * m0;
 }

//matrix lookat for 3D
 void lookat(T x, T y, T z,
              T lx, T ly, T lz,
              T ux, T uy, T uz)
 {
     kgmVector3d<T> dir(lx - x, ly - y, lz - z);
     kgmVector3d<T> pos(x, y, z);
     kgmVector3d<T> ups(ux, uy, uz);
     lookat(pos, dir, ups);
 }

 void shadow(kgmVector4d<T> light, kgmVector4d<T> plane){
  T  d;
  plane.normalize();
  d = plane.dot(light);
  m[0] = plane.x * light.x + d;
   m[4] = plane.y * light.x;
    m[8] = plane.z * light.x;
     m[12] = plane.w * light.x;
  m[1] = plane.x * light.y;
   m[5] = plane.y * light.y + d;
    m[9] = plane.z * light.y;
     m[13] = plane.w * light.y;
  m[2] = plane.x * light.z;
   m[6] = plane.y * light.z;
    m[10] = plane.z * light.z + d;
     m[14] = plane.w * light.z;
  m[3] = plane.x * light.w;
   m[7] = plane.y * light.w;
    m[11] = plane.z * light.w;
     m[15] = plane.w * light.w + d;
 }
// multiply vector 3d to matrix rotate component
 kgmVector3d<T> multiply(kgmVector3d<T>& v){
  kgmVector3d<T> r;
  r.x = v.x*m[0] + v.y*m[4] + v.z*m[8];
  r.y = v.x*m[1] + v.y*m[5] + v.z*m[9];
  r.z = v.x*m[2] + v.y*m[6] + v.z*m[10];
  return r;
 }
//Orthogonal projection
 void ortho(T l, T r, T b, T t, T n, T f){
  m[0] = (T)2.0 / (r - l),		m[1] = (T)0,				m[2] = (T)0,				m[3] = (T)0;
  m[4] = (T)0,					m[5] = (T)2.0 / (t - b),	m[6] = (T)0,				m[7] = (T)0;
  m[8] = (T)0,					m[9] = (T)0,				m[10] = (T)-2.0 / (f- n),	m[11] = (T)0;
  m[12] = - (r + l) / (r - l);  m[13] = - (t + b) / (t - b);m[14] = - (f + n) / (f - n);m[15] = (T)1.0;
 }
//transpose
 void transpose(){
  kgmMatrix3d<T> t = *this;
  m[0]  = t[0],  m[1]  = t[4], m[2]  = t[8],  m[3]  = t[12];
  m[4]  = t[1],  m[5]  = t[5], m[6]  = t[9],  m[7]  = t[13];
  m[8]  = t[2],  m[9]  = t[6], m[10] = t[10], m[11] = t[14];
  m[12] = t[3],  m[13] = t[7], m[14] = t[11], m[15] = t[15];
 }
protected:
inline T det(T& a00, T& a01, T& a10, T& a11){
 return (a00*a11 - a01*a10);
}

inline T det(T& a00, T& a01, T& a02,
         T& a10, T& a11, T& a12,
         T& a20, T& a21, T& a22){
 T mn0, mn1, mn2;
 mn0 = det(a11, a12, a21, a22);
 mn1 = det(a10, a12, a20, a22);
 mn2 = det(a10, a11, a20, a21);
 return (a00*mn0 - a01*mn1 + a02*mn2);
}

inline T det(T& a00, T& a01, T& a02, T& a03,
       T& a10, T& a11, T& a12, T& a13,
       T& a20, T& a21, T& a22, T& a23,
       T& a30, T& a31, T& a32, T& a33){
 T mn0, mn1, mn2, mn3;
 mn0 = det(a11, a12, a13, a21, a22, a23, a31, a32, a33);
 mn1 = det(a10, a12, a13, a20, a22, a23, a30, a32, a33);
 mn2 = det(a10, a11, a13, a20, a21, a23, a30, a31, a33);
 mn3 = det(a10, a11, a12, a20, a21, a22, a30, a31, a32);
 return (a00*mn0 - a01*mn1 + a02*mn2 - a03*mn3);
}

inline T det(){
 T det = m[0] * m[5] * m[10];
 det += m[4] * m[9] * m[2];
 det += m[8] * m[1] * m[6];
 det -= m[8] * m[5] * m[2];
 det -= m[4] * m[1] * m[10];
 det -= m[0] * m[9] * m[6];
 return det;
}
};

