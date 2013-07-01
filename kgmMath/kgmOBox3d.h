#pragma once

template <class T> class kgmOBox3d{
public:
  kgmVector3d<T> position,
                 direction,  // default should be x positive direction
                 dimension;  // space resolution of box(length, width, height)

public:
  kgmOBox3d()
  {
  }

  kgmOBox3d(const kgmVector3d<T> &pos, const kgmVector3d<T> &dir, const kgmVector3d<T> &dim)
  {
    position = pos, direction = dir.normal(), dimension = dim;
  }

  kgmOBox3d(T pos_x, T pos_y, T pos_z, T dir_x, T dir_y, T dir_z, T dim_x, T dim_y, T dim_z)
  {
    position.x = pos_x,
     position.y = pos_y,
      position.z = pos_z;
    direction.x = dir_x,
     direction.y = dir_y,
      direction.z = dir_z;
    dimension.x = dim_x,
     dimension.y = dim_y,
      dimension.z = dim_z;
  }

  void set(const kgmVector3d<T> &pos, const kgmVector3d<T> &dir, const kgmVector3d<T> &dim)
  {
    position = pos, direction = dir.normal(), dimension = dim;
  }

  void set(T pos_x, T pos_y, T pos_z, T dir_x, T dir_y, T dir_z, T dim_x, T dim_y, T dim_z)
  {
    position.x = pos_x,
     position.y = pos_y,
      position.z = pos_z;
    direction.x = dir_x,
     direction.y = dir_y,
      direction.z = dir_z;
    dimension.x = dim_x,
     dimension.y = dim_y,
      dimension.z = dim_z;
  }

  void points(kgmVector3d<T> v[])
  {
    T  z_angle = acos(direction.x);

    mtx4  m_rot;

    m_rot.rotate(0, 0, z_angle);
    //m_rot.rotate(0, direction);

    kgmVector3d<T> dim(0.5 * dimension.x,
                       0.5 * dimension.y,
                       0.5 * dimension.z);
    kgmVector3d<T> min(position.x - dim.x,
                       position.y - dim.y,
                       position.z - dim.z),
                   max(position.x + dim.x,
                       position.y + dim.y,
                       position.z + dim.z);
    kgmVector3d<T> d = max - min;

    v[0] = min;
    v[1] = kgmVector3d<T>(min.x + d.x, min.y,       min.z);
    v[2] = kgmVector3d<T>(min.x + d.x, min.y + d.y, min.z);
    v[3] = kgmVector3d<T>(min.x,       min.y + d.y, min.z);
    v[4] = kgmVector3d<T>(min.x,       min.y,       min.z + d.z);
    v[5] = kgmVector3d<T>(min.x + d.x, min.y,       min.z + d.z);
    v[6] = max;
    v[7] = kgmVector3d<T>(min.x,       min.y + d.y, min.z + d.z);

    for(int i = 0; i < 8; i++)
      v[i] = m_rot * v[i];
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

    plane[0] = kgmPlane3d<T>(c_points[0], c_points[2], c_points[1]);
    plane[1] = kgmPlane3d<T>(c_points[0], c_points[4], c_points[3]);
    plane[2] = kgmPlane3d<T>(c_points[0], c_points[1], c_points[5]);

    T dims[3] = {dimension.z, dimension.x, dimension.y};

    for(int I = 0; I < 3; I++)
    {
      T distance = plane[I].distance(b_sphere.center);

      if((distance > 0.0 && distance < b_sphere.radius) ||
         (distance < 0.0 && fabs(distance) < (b_sphere.radius + dims[I])))
      {
        int  sides = 0;
        bool cross = false;

        for(int i = 0; i < 8; i++)
        {
          distance = plane[I].distance(b_points[i]);

          if(distance < 0.0)
            sides--;
          else
            sides++;
          T too = fabs(distance);
          if((distance < 0.0) && (fabs(distance) < dims[I]))
          {
            cross = true;

            break;
          }
        }

        if(!cross && (abs(sides) == 8))
          return false;
      }
      else
      {
        return false;
      }
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
