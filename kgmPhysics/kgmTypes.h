#ifndef KGMTYPES_H
#define KGMTYPES_H

namespace kgmPhysicTypes
{
  class Triangle
  {
  public:
    vec3   v[3];
    plane3 plane;

  public:
    Triangle()
    {
      v[0] = vec3(0, 0, 0);
      v[1] = vec3(0, 0, 0);
      v[2] = vec3(0, 0, 0);

      plane = plane3(v[0], v[1], v[2]);
    }

    Triangle(vec3 a, vec3 b, vec3 c)
    {
      v[0] = a;
      v[1] = b;
      v[2] = c;

      plane = plane3(a, b, c);
    }
  };
};

#endif // KGMTYPES_H
