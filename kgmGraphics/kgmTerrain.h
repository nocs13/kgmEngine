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
class kgmTexture;

class kgmTerrain: public kgmObject
{
  KGM_OBJECT(kgmTerrain);

public:

  struct Chunk
  {
    box2 rect;
    u32  details;
    s32  id[2];

    Chunk* sis[2] = {null};

    static bool compare(Chunk& a, Chunk& b)
    {
      return (a.details < b.details);
    }

    bool operator<(Chunk& a)
    {
      return (details < a.details);
    }
  };

  class Mesh: public kgmMesh
  {
    u32 count = 0;
    u32 limit = 0;

    Vertex* v = null;

    kgmArray<Vertex_P_N_C_T> triangles;

  public:

    Mesh()
    {
       m_fvf = FVF_P_N_C_T;
       m_fff = 0;

       triangles.alloc(333);

       v = triangles.data();

       limit = 100000;
    }

    ~Mesh()
    {
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
      return count;
    }

    void reset()
    {
      count = 0;
    }

    u32 getLimit()
    {
      return limit;
    }

    void setLimit(u32 l)
    {
      limit = l;
    }

    bool atLimit()
    {
      if (count >= limit)
        return true;

      return false;
    }

    void add(triangle3& tr, triangle3& uv, u32 col = 0xff999999, vec3 n = vec3(0, 0, 1))
    {
      if (count > limit)
        return;

      if ((3 * count + 1) >= triangles.length())
      {
        triangles.realloc(triangles.length() + 300);

        v = triangles.data();
      }

      /*
      plane3 pn(tr.pt[0], tr.pt[1], tr.pt[2]);

      vec3 n = pn.normal();
      */
      triangles[3 * count + 0].pos = tr.pt[0];
      triangles[3 * count + 1].pos = tr.pt[1];
      triangles[3 * count + 2].pos = tr.pt[2];

      triangles[3 * count + 0].uv = vec2(uv.pt[0].x, uv.pt[0].y);
      triangles[3 * count + 1].uv = vec2(uv.pt[1].x, uv.pt[1].y);
      triangles[3 * count + 2].uv = vec2(uv.pt[2].x, uv.pt[2].y);

      //triangles[3 * count + 0].uv[1] = vec2(uv2.pt[0].x, uv2.pt[0].y);
      //triangles[3 * count + 1].uv[1] = vec2(uv2.pt[1].x, uv2.pt[1].y);
      //triangles[3 * count + 2].uv[1] = vec2(uv2.pt[2].x, uv2.pt[2].y);

      triangles[3 * count + 0].col = col;
      triangles[3 * count + 1].col = col;
      triangles[3 * count + 2].col = col;

      triangles[3 * count + 0].nor = n;
      triangles[3 * count + 1].nor = n;
      triangles[3 * count + 2].nor = n;

      count ++;
    }
  };

  class MLines: public kgmMesh
  {
    u32 count = 0;

    Vertex* v = null;

    kgmArray<Vertex_P_C> lines;

  public:

    MLines()
    {
       m_fvf = FVF_P_C;
       m_fff = 0;

       lines.alloc(222);

       v = lines.data();
    }

    ~MLines()
    {
    }

    Vertex* vertices() const
    {
      return v;
    }

    u32 vcount() const
    {
      return (2 * count);
    }

    u32 fcount()
    {
      return count;
    }

    void reset()
    {
      count = 0;
    }

    void add(vec3& a, vec3& b, u32 col = 0xff999999)
    {
      if ((2 * count + 1) >= lines.length())
      {
        lines.realloc(lines.length() + 300);

        v = lines.data();
      }


      lines[2 * count + 0].pos = a;
      lines[2 * count + 1].pos = b;

      lines[2 * count + 0].col = col;
      lines[2 * count + 1].col = col;

      count ++;
    }
  };

  struct Heightmap
  {
    u16 width;
    u16 height;

    kgmArray<u16>  map;
    kgmArray<vec3> nor;
  };

  typedef kgmList<Mesh>::iterator MeshIt;

private:

  typedef kgmVector2d<u32> uint2;
  typedef kgmVector2d<f32> float2;

  f32 m_width;
  f32 m_length;
  f32 m_height;

  f32 m_chunk;



  Heightmap m_heightmap;

  Mesh*   m_mesh;
  Mesh*   m_fill;
  MLines* m_lines;

  kgmCamera* m_camera = nullptr;

  kgmTexture* m_tex_color[4];
  kgmTexture* m_tex_blend;

  vec2 m_uv_scale;

protected:
  ~kgmTerrain();

public:
  kgmTerrain();

  virtual void prepare(kgmCamera*);

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
  bool normalmap(kgmPicture* map);
  bool texColor(u32 id, kgmTexture* tex);
  bool texBlend(kgmTexture* tex);

  void uvScale(f32 u, f32 v);

  kgmMesh* mesh();
  kgmMesh* fill();
  kgmMesh* lines();

  kgmTexture* texColor(u32);
  kgmTexture* texBlend();

  vec2 uvScale();

private:
  void build();
  void update(kgmCamera* cam);
  void generate(vec3 points[4], u32 level);
  void generate(box2 rect, u32 level);
  void fillx(Chunk* c, Chunk* n);
  void filly(Chunk* c, Chunk* n);
  void normals();

  float2 from_uint2(uint2 v);
  uint2  from_float2(float2 v);
  f32    get_height(uint2 v);
  f32    get_height(float2 v);
  f32    len_from_details(u32 details);
  vec3   get_normal(uint2  v);
  vec3   get_normal(float2 v);
};

