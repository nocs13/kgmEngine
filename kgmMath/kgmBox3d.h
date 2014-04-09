#pragma once

template <class T> class kgmBox3d
{
public:
  kgmVector3d<T> min, max;

public:
  kgmBox3d()
  {
  }

  kgmBox3d(const kgmVector3d<T> &a, const kgmVector3d<T> &b)
  {
    min = a, max = b;
  }

  kgmBox3d(const kgmVector3d<T> v[], int count)
  {
    min = max = v[0];
    for(int i = 0; i < count; i++){
      min.x = MIN(min.x, v[i].x);
      min.y = MIN(min.y, v[i].y);
      min.z = MIN(min.z, v[i].z);
      max.x = MAX(max.x, v[i].x);
      max.y = MAX(max.y, v[i].y);
      max.z = MAX(max.z, v[i].z);
    }
  }
  void set(kgmVector3d<T> &a, kgmVector3d<T> &b)
  {
    min = a, max = b;
  }
  void set(T mnx, T mny, T mnz, T mxx, T mxy, T mxz)
  {
    min.x = mnx, min.y = mny, min.z = mnz;
    max.x = mxx, max.y = mxy, max.z = mxz;
  }

  void dimension(kgmVector3d<T> &dim)
  {
    dim = max - min;
  }

  kgmVector3d<T> dimension()
  {
    return  (max - min);
  }

  void center(kgmVector3d<T> &dim)
  {
    dim = max - min;

    dim.x = min.x + 0.5 * dim.x;
    dim.y = min.y + 0.5 * dim.y;
    dim.z = min.z + 0.5 * dim.z;
  }

  kgmVector3d<T> center()
  {
    kgmVector3d<T> dim = max - min;

    dim.x = min.x + 0.5 * dim.x;
    dim.y = min.y + 0.5 * dim.y;
    dim.z = min.z + 0.5 * dim.z;

    return  dim;
  }

  void points(kgmVector3d<T> v[]){
    kgmVector3d<T> d = max - min;

    v[0] = min;
    v[1] = kgmVector3d<T>(min.x + d.x, min.y,       min.z);
    v[2] = kgmVector3d<T>(min.x + d.x, min.y + d.y, min.z);
    v[3] = kgmVector3d<T>(min.x,       min.y + d.y, min.z);
    v[4] = kgmVector3d<T>(min.x,       min.y,       min.z + d.z);
    v[5] = kgmVector3d<T>(min.x + d.x, min.y,       min.z + d.z);
    v[6] = max;
    v[7] = kgmVector3d<T>(min.x,       min.y + d.y, min.z + d.z);
  }

  void planes(kgmPlane3d<T>  plane[])
  {
    kgmVector3d<T> pts[8];

    points(pts);
    plane[0] = kgmPlane3d<T>(pts[0], pts[2], pts[1]);
    plane[1] = kgmPlane3d<T>(pts[0], pts[4], pts[3]);
    plane[2] = kgmPlane3d<T>(pts[1], pts[2], pts[5]);
    plane[3] = kgmPlane3d<T>(pts[0], pts[1], pts[5]);
    plane[4] = kgmPlane3d<T>(pts[2], pts[3], pts[7]);
    plane[5] = kgmPlane3d<T>(pts[4], pts[5], pts[6]);
  }

  bool intersect(kgmBox3d<T>& b)
  {
    return !( (min.x > b.max.x) || (min.y > b.max.y) || (min.z > b.max.z) ||
              (max.x < b.min.x) || (max.y < b.min.y) || (max.z < b.min.z));
  }

  bool intersect(kgmVector3d<T> center, T radius)
  {
    kgmBox3d<T>  b(center - kgmVector3d<T>(radius, radius, radius),
                   center + kgmVector3d<T>(radius, radius, radius));

    return intersect(b);
  }

  bool intersect(kgmRay3d<T> &r, kgmVector3d<T> &cp){
    kgmVector3d<T> rect[6][4];
    kgmPlane3d<T>  plane;
    kgmBox3d<T>    box;

    rect[0][0] = kgmVector3d<T>(min.x, min.y, min.z);
    rect[0][1] = kgmVector3d<T>(min.x, max.y, min.z);
    rect[0][2] = kgmVector3d<T>(max.x, max.y, min.z);
    rect[0][3] = kgmVector3d<T>(max.x, min.y, min.z);
    plane = kgmPlane3d<T>(rect[0][0], rect[0][1], rect[0][2]);
    box = kgmBox3d(rect[0], 4);

    if(plane.intersect(r, cp) && box.isin(cp))
      return true;

    rect[1][0] = kgmVector3d<T>(max.x, max.y, min.z);
    rect[1][1] = kgmVector3d<T>(max.x, max.y, max.z);
    rect[1][2] = kgmVector3d<T>(max.x, min.y, max.z);
    rect[1][3] = kgmVector3d<T>(max.x, min.y, min.z);
    plane = kgmPlane3d<T>(rect[1][0], rect[1][1], rect[1][2]);
    box = kgmBox3d(rect[1], 4);

    if(plane.intersect(r, cp) && box.isin(cp))
      return true;

    rect[2][0] = kgmVector3d<T>(min.x, max.y, min.z);
    rect[2][1] = kgmVector3d<T>(min.x, max.y, max.z);
    rect[2][2] = kgmVector3d<T>(max.x, max.y, max.z);
    rect[2][3] = kgmVector3d<T>(max.x, max.y, min.z);
    plane = kgmPlane3d<T>(rect[2][0], rect[2][1], rect[2][2]);
    box = kgmBox3d(rect[2], 4);

    if(plane.intersect(r, cp) && box.isin(cp))
      return true;

    rect[3][0] = kgmVector3d<T>(min.x, min.y, min.z);
    rect[3][1] = kgmVector3d<T>(min.x, min.y, max.z);
    rect[3][2] = kgmVector3d<T>(min.x, max.y, max.z);
    rect[3][3] = kgmVector3d<T>(min.x, max.y, min.z);
    plane = kgmPlane3d<T>(rect[3][0], rect[3][1], rect[3][2]);
    box = kgmBox3d(rect[3], 4);

    if(plane.intersect(r, cp) && box.isin(cp))
      return true;

    rect[4][0] = kgmVector3d<T>(max.x, min.y, min.z);
    rect[4][1] = kgmVector3d<T>(max.x, min.y, max.z);
    rect[4][2] = kgmVector3d<T>(min.x, min.y, max.z);
    rect[4][3] = kgmVector3d<T>(min.x, min.y, min.z);
    plane = kgmPlane3d<T>(rect[4][0], rect[4][1], rect[4][2]);
    box = kgmBox3d(rect[4], 4);

    if(plane.intersect(r, cp) && box.isin(cp))
      return true;

    rect[5][0] = kgmVector3d<T>(min.x, min.y, max.z);
    rect[5][1] = kgmVector3d<T>(min.x, max.y, max.z);
    rect[5][2] = kgmVector3d<T>(max.x, max.y, max.z);
    rect[5][3] = kgmVector3d<T>(max.x, min.y, max.z);
    plane = kgmPlane3d<T>(rect[5][0], rect[5][1], rect[5][2]);
    box = kgmBox3d(rect[5], 4);

    if(plane.intersect(r, cp) && box.isin(cp))
      return true;

    return false;
  }

  bool isin(kgmVector3d<T> &v){
    if((v.x >= min.x) && (v.y >= min.y) && (v.z >= min.z) &&
       (v.x <= max.x) && (v.y <= max.y) && (v.z <= max.z))
      return true;
    return false;
  }

  bool isin(kgmVector3d<T> v[], kgmVector3d<T> &p){
    kgmVector3d<T> v0, v1, v2, v3;
    T an = 0;
    v0 = v[0] - p; v0.normalize();
    v1 = v[1] - p; v1.normalize();
    v2 = v[2] - p; v2.normalize();
    v3 = v[3] - p; v3.normalize();
    an =  v0.angle(v1);
    an += v1.angle(v2);
    an += v2.angle(v3);
    an += v3.angle(v0);

    if(fabs(an - (2*PI)) < 0.01)
      return true;

    return false;
  }

  bool isin(kgmBox3d<T> &b){
    if((b.min.x > min.x) && (b.min.y > min.y) && (b.min.z > min.z) &&
       (b.max.x < max.x) && (b.max.y < max.y) && (b.max.z < max.z))
      return true;

    return false;
  }

  void extend(kgmBox3d<T> &b){
    if(b.min.x < min.x) min.x = b.min.x;
    if(b.min.y < min.y) min.y = b.min.y;
    if(b.min.z < min.z) min.z = b.min.z;
    if(b.max.x > max.x) max.x = b.max.x;
    if(b.max.y > max.y) max.y = b.max.y;
    if(b.max.z > max.z) max.z = b.max.z;
  }
};

