#include "kgmObstacle.h"

KGMOBJECT_IMPLEMENT(kgmObstacle, kgmObject);

kgmObstacle::kgmObstacle()
{
  transform.identity();
}

kgmObstacle::~kgmObstacle()
{
  triangles.clear();
}

void kgmObstacle::fromBox(vec3 min, vec3 max)
{
  triangles.clear();

  triangle tr;

  tr.pt[0] = vec3(min.x, min.y, min.z);  tr.pt[1] = vec3(max.x, min.y, min.z);  tr.pt[2] = vec3(max.x, max.y, min.z);
  triangles.add(tr);

  tr.pt[0] = vec3(max.x, max.y, min.z);  tr.pt[1] = vec3(min.x, max.y, min.z);  tr.pt[2] = vec3(min.x, min.y, min.z);
  triangles.add(tr);

  tr.pt[0] = vec3(min.x, min.y, max.z);  tr.pt[1] = vec3(max.x, min.y, max.z);  tr.pt[2] = vec3(max.x, max.y, max.z);
  triangles.add(tr);

  tr.pt[0] = vec3(max.x, max.y, max.z);  tr.pt[1] = vec3(min.x, max.y, max.z);  tr.pt[2] = vec3(min.x, min.y, max.z);
  triangles.add(tr);

  tr.pt[0] = vec3(min.x, min.y, min.z);  tr.pt[1] = vec3(min.x, max.y, min.z);  tr.pt[2] = vec3(min.x, max.y, max.z);
  triangles.add(tr);

  tr.pt[0] = vec3(min.x, max.y, max.z);  tr.pt[1] = vec3(min.x, min.y, max.z);  tr.pt[2] = vec3(min.x, min.y, min.z);
  triangles.add(tr);

  tr.pt[0] = vec3(max.x, min.y, min.z);  tr.pt[1] = vec3(max.x, max.y, min.z);  tr.pt[2] = vec3(max.x, max.y, max.z);
  triangles.add(tr);

  tr.pt[0] = vec3(max.x, max.y, max.z);  tr.pt[1] = vec3(max.x, min.y, max.z);  tr.pt[2] = vec3(max.x, min.y, min.z);
  triangles.add(tr);

  tr.pt[0] = vec3(min.x, min.y, min.z);  tr.pt[1] = vec3(max.x, min.y, min.z);  tr.pt[2] = vec3(max.x, min.y, max.z);
  triangles.add(tr);

  tr.pt[0] = vec3(max.x, min.y, max.z);  tr.pt[1] = vec3(min.x, min.y, max.z);  tr.pt[2] = vec3(min.x, min.y, min.z);
  triangles.add(tr);

  tr.pt[0] = vec3(min.x, max.y, min.z);  tr.pt[1] = vec3(max.x, max.y, min.z);  tr.pt[2] = vec3(max.x, max.y, max.z);
  triangles.add(tr);

  tr.pt[0] = vec3(max.x, max.y, max.z);  tr.pt[1] = vec3(min.x, max.y, max.z);  tr.pt[2] = vec3(min.x, max.y, min.z);
  triangles.add(tr);
}

void kgmObstacle::fromRect(vec2 min, vec2 max)
{
  triangles.clear();

  triangle tr;

  tr.pt[0] = vec3(min.x, min.y, 0);
  tr.pt[1] = vec3(max.x, min.y, 0);
  tr.pt[2] = vec3(max.x, max.y, 0);
  triangles.add(tr);

  tr.pt[0] = vec3(max.x, max.y, 0);
  tr.pt[1] = vec3(min.x, max.y, 0);
  tr.pt[2] = vec3(min.x, min.y, 0);
  triangles.add(tr);
}
