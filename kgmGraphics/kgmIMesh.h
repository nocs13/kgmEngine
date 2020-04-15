#ifndef KGMIMESH_H
#define KGMIMESH_H

#include "../kgmBase/kgmTypes.h"
#include "../kgmMath/kgmMath.h"

class kgmIMesh
{
public:
  enum RenderType
  {
    RT_TRIANGLE,
    RT_POINT,
    RT_LINE,
    RT_TRIANGLESTRIP
  };

  enum FVF
  {
    FVF_P,                  // = (gcv_xyz) ,
    FVF_P_C,                // = (gcv_xyz|gcv_col) ,
    FVF_P_N,                // = (gcv_xyz|gcv_nor) ,
    FVF_P_T,                // = (gcv_xyz|gcv_uv0) ,
    FVF_P_N_T,              // = (gcv_xyz|gcv_nor|gcv_uv0) ,
    FVF_P_C_T,              // = (gcv_xyz|gcv_col|gcv_uv0) ,
    FVF_P_N_T2,             // = (gcv_xyz|gcv_nor|gcv_col|gcv_uv0|gcv_uv1) ,
    FVF_P_N_T_BW_BI,        // = (gcv_xyz|gcv_nor|gcv_col|gcv_uv0|gcv_uv1|gcv_uv2|gcv_uv3|gcv_uv_f4)
    FVF_P_N_T2_BW_BI,       // = (gcv_xyz|gcv_nor|gcv_col|gcv_uv0|gcv_uv1|gcv_uv2|gcv_uv3|gcv_uv_f4)
  };

  enum FFF
  {
    FFF_16,
    FFF_32
  };

  struct Vertex
  {
    vec3 pos;
  };

  struct Vertex_P_C: public Vertex
  {
    vec4 col;
  };

  struct Vertex_P_T: public Vertex
  {
    vec2 uv;
  };

  struct Vertex_P_N: public Vertex
  {
    vec3 nor;
  };

  struct Vertex_P_N_T: public Vertex_P_N
  {
    vec2 uv;
  };

  struct Vertex_P_N_T2: public Vertex_P_N
  {
    vec2 uv[2];
  };

  struct Vertex_P_C_T: public Vertex_P_C
  {
    vec2 uv;
  };

  struct Vertex_P_N_T_BW_BI: public Vertex_P_N_T
  {
    float bw[4];
    int   bi[4];
  };

  struct Vertex_P_N_T2_BW_BI: public Vertex_P_N_T2
  {
    float bw[4];
    int   bi[4];
  };

  struct Face
  {
  };

  struct Face_16: public Face
  {
    union
    {
      struct{ u16 a, b, c; };
      u16 f[3];
    };
  };

  struct Face_32: public Face
  {
    union
    {
      struct{ u32 a, b, c; };
      u32 f[3];
    };
  };

public:
  virtual Vertex* vertices() = 0;
  virtual Face*   faces()    = 0;

  virtual u32 vsize() = 0;
  virtual u32 fsize() = 0;
  virtual u32 fvf()   = 0;
  virtual u32 fff()   = 0;
  virtual u32 vcount() const = 0;
  virtual u32 fcount() const = 0;

  virtual RenderType rtype() const = 0;

  virtual box3 bound() const = 0;

  virtual bool gpu() const = 0;
};

#endif // KGMIMESH_H
