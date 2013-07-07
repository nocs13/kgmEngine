#pragma once

template <class T> class kgmOBox3d{
public:
  kgmVector3d<T> position,
                 rotation,
                 dimension;  // space resolution of box(length, width, height)

public:
  kgmOBox3d()
  {
  }

  kgmOBox3d(const kgmVector3d<T> &pos, const kgmVector3d<T> &rot, const kgmVector3d<T> &dim)
  {
    position = pos, rotation = rot, dimension = dim;
  }

  kgmOBox3d(T pos_x, T pos_y, T pos_z, T rot_x, T rot_y, T rot_z, T dim_x, T dim_y, T dim_z)
  {
    position.x = pos_x,
     position.y = pos_y,
      position.z = pos_z;
    rotation.x = rot_x,
     rotation.y = rot_y,
      rotation.z = rot_z;
    dimension.x = dim_x,
     dimension.y = dim_y,
      dimension.z = dim_z;
  }

  void set(const kgmVector3d<T> &pos, const kgmVector3d<T> &rot, const kgmVector3d<T> &dim)
  {
    position = pos, rotation = rot, dimension = dim;
  }

  void set(T pos_x, T pos_y, T pos_z, T rot_x, T rot_y, T rot_z, T dim_x, T dim_y, T dim_z)
  {
    position.x = pos_x,
     position.y = pos_y,
      position.z = pos_z;
    rotation.x = rot_x,
     rotation.y = rot_y,
      rotation.z = rot_z;
    dimension.x = dim_x,
     dimension.y = dim_y,
      dimension.z = dim_z;
  }

  void points(kgmVector3d<T> v[])
  {
    quat q;
    q.euler(rotation);

    mtx4  mrt(q);
    mtx4  mtl(position);
    mtx4  mtr = mrt * mtl;

    kgmVector3d<T> dim(0.5 * dimension.x,
                       0.5 * dimension.y,
                       0.5 * dimension.z);
    kgmVector3d<T> min_(-dim.x, -dim.y, -dim.z);
    kgmVector3d<T> max_( dim.x,  dim.y,  dim.z);

    v[0] = min_;
    v[1] = kgmVector3d<T>(min_.x + dimension.x, min_.y,               min_.z);
    v[2] = kgmVector3d<T>(min_.x + dimension.x, min_.y + dimension.y, min_.z);
    v[3] = kgmVector3d<T>(min_.x,               min_.y + dimension.y, min_.z);
    v[4] = kgmVector3d<T>(min_.x,               min_.y,               min_.z + dimension.z);
    v[5] = kgmVector3d<T>(min_.x + dimension.x, min_.y,               min_.z + dimension.z);
    v[6] = max_;
    v[7] = kgmVector3d<T>(min_.x,               min_.y + dimension.y, min_.z + dimension.z);

    for(int i = 0; i < 8; i++)
      v[i] = mtr * v[i];
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

  bool intersect(kgmRay3d<T> &r, kgmVector3d<T> &cp)
  {
    return false;
  }

  bool intersect(kgmOBox3d<T> &box)
  {
    kgmPlane3d<T>  plane[3];
    kgmVector3d<T> b_points[8];
    kgmVector3d<T> c_points[8];

    points(c_points);
    box.points(b_points);

    kgmSphere3d<T> b_sphere(b_points, 8);
    kgmSphere3d<T> c_sphere(c_points, 8);

    plane[0] = kgmPlane3d<T>(c_points[0], c_points[2], c_points[1]);
    plane[1] = kgmPlane3d<T>(c_points[0], c_points[4], c_points[3]);
    plane[2] = kgmPlane3d<T>(c_points[0], c_points[1], c_points[5]);

    T    dims[3] = {dimension.z, dimension.x, dimension.y};
    int  sides = 0;
    bool cross = false;

    for(int I = 0; I < 3; I++)
    {
      T distance = plane[I].distance(b_sphere.center);

      //if((distance > 0.0 && distance < b_sphere.radius) ||
      //   (distance < 0.0 && fabs(distance) < (b_sphere.radius + dims[I])))
      {
        sides = 0;
        cross = false;

        for(int i = 0; i < 8; i++)
        {
          distance = plane[I].distance(b_points[i]);

          if(distance < 0.0)
            sides--;
          else
            sides++;

          if((distance < 0.0) && (fabs(distance) <= dims[I]))
          {
            cross = true;

            break;
          }
        }

        if(!cross && (abs(sides) == 8))
          return false;
      }
      //else
      //{
      //  return false;
      //}
    }

    return true;
  }

  bool isin(kgmVector3d<T> &v)
  {
    return false;
  }

  bool isin(kgmVector3d<T> v[], kgmVector3d<T> &p)
  {
    return false;
  }
};
