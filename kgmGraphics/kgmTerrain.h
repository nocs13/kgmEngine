// kgmLandscape.h: interface for the kgmLandscape class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

#include "kgmMesh.h"

#include "../kgmBase/kgmObject.h"
#include "../kgmBase/kgmString.h"
#include "../kgmBase/kgmList.h"
#include "../kgmMath/kgmBase.h"
#include "../kgmMath/kgmVector3d.h"
#include "../kgmMath/kgmPlane3d.h"
#include "../kgmMath/kgmTriangle3d.h"

class kgmPicture;
class kgmCamera;
class kgmMaterial;

class kgmTerrain: public kgmObject
{
  KGM_OBJECT(kgmTerrain);

public:

  struct Chunk
  {
    fRect area;

    vec3 normal;

    vec2 uv[3];
  };

  struct Sector
  {
    kgmList<Chunk*> chunks;

    Sector *parent, *child;
  };

  class Mesh: public kgmMesh
  {
    u32 count = 0;

    Vertex* v = null;

    kgmArray<Vertex_P_N_C_T> triangles;

  public:

    Mesh()
    {
       m_fvf = FVF_P_N_C_T;
       m_fff = 0;

       triangles.alloc(333);

       v = triangles.data();
    }

    ~Mesh()
    {
      //triangles.clear();
    }

    Vertex* vertices() const
    {
      return v;
    }

    u32 vcount() const
    {
      return (3 * count);
    }

    u32 fcount()
    {
      return triangles.length();
    }

    void reset()
    {
      count = 0;
    }

    void add(triangle3& tr)
    {
      if ((3 * count + 1) >= triangles.length())
      {
        triangles.realloc(triangles.length() + 300);

        v = triangles.data();
      }

      plane3 pn(tr.pt[0], tr.pt[1], tr.pt[2]);

      vec3 n = pn.normal();

      triangles[3 * count + 0].pos = tr.pt[0];
      triangles[3 * count + 1].pos = tr.pt[1];
      triangles[3 * count + 2].pos = tr.pt[2];

      triangles[3 * count + 0].col = 0xffffffff;
      triangles[3 * count + 1].col = 0xffffffff;
      triangles[3 * count + 2].col = 0xffffffff;

      triangles[3 * count + 0].nor = n;
      triangles[3 * count + 1].nor = n;
      triangles[3 * count + 2].nor = n;

      count ++;
    }
  };

  struct Heightmap
  {
    u16 width;
    u16 height;

    kgmArray<u16> map;
  };

  typedef kgmList<Mesh>::iterator MeshIt;

private:

  typedef kgmVector2d<u32> uint2;
  typedef kgmVector2d<f32> float2;

  f32 m_width;
  f32 m_length;
  f32 m_height;

  Heightmap m_heightmap;

  Mesh* m_mesh;

  kgmCamera* m_camera = nullptr;

  Sector* root = nullptr;

protected:
  ~kgmTerrain();

public:
  kgmTerrain();

  virtual void prepare(kgmCamera*);
  virtual MeshIt meshes();

  f32 width() const
  {
    return m_width;
  }

  void width(f32 w)
  {
    m_width = w;
  }

  f32 length() const
  {
    return m_length;
  }

  void length(f32 l)
  {
    m_length = l;
  }

  f32 height() const
  {
    return m_height;
  }

  void height(f32 h)
  {
    m_height = h;
  }

  bool heightmap(kgmPicture* map);

  kgmMesh* mesh();

private:
  void build();
  void update(kgmCamera* cam);
  void generate(vec3 points[4], u32 level);
  void generate(box2 rect, u32 level);

  float2 from_uint2(uint2 v);
  uint2  from_float2(float2 v);
  f32    get_height(uint2 v);
  f32    get_height(float2 v);
};

