#include "kgmShader.h"
#include "kgmTexture.h"
#include "../kgmBase/kgmIGC.h"

KGMOBJECT_IMPLEMENT(kgmShader, kgmResource);

kgmShader::kgmShader(kgmIGC* g)
{
  m_gc     = g;
  m_shader = 0;

  m_input = IN_MTX4_PROJ | IN_MTX4_VIEW | IN_MTX4_TRAN | IN_VEC3_EYEPOS | IN_VEC3_EYEDIR | 
            IN_VEC4_LIGHT | IN_VEC1_TIME | IN_VEC1_RANDOM | 
            IN_MAP_COLOR | IN_MAP_NORMAL | IN_MAP_SPECULAR;
}

kgmShader::~kgmShader()
{
}

void kgmShader::start()
{
  m_gc->gcSetShader(m_shader);
}

void kgmShader::stop()
{
  m_gc->gcSetShader(0);
}

void kgmShader::set(const char* id, float& v, int count)
{
  m_gc->gcUniform(m_shader, gcunitype_float1, count, id, &v);
}

void kgmShader::set(const char* id, vec2& v, int count)
{
  m_gc->gcUniform(m_shader, gcunitype_float2, count, id, &v);
}

void kgmShader::set(const char* id, vec3& v, int count)
{
  m_gc->gcUniform(m_shader, gcunitype_float3, count, id, &v);
}

void kgmShader::set(const char* id, vec4& v, int count)
{
  m_gc->gcUniform(m_shader, gcunitype_float4, count, id, &v);
}

void kgmShader::set(const char* id, int v, int count)
{
  m_gc->gcUniform(m_shader, gcunitype_int1, count, id, &v);
}

void kgmShader::set(const char* id, mtx4& v, int count)
{
  m_gc->gcUniformMatrix(m_shader, gcunitype_mtx4, count, 0, id, v.m);
}

void kgmShader::set(const char* id, mtx3& v, int count)
{
  m_gc->gcUniformMatrix(m_shader, gcunitype_mtx3, count, 0, id, v.m);
}

void kgmShader::attr(int i, const char* a)
{
  m_gc->gcBindAttribute(m_shader, i, a);
}

const char *kgmShader::toString(Shader type)
{
#define s_text(x) if(x == type) return #x
  s_text(TypeNone);
  s_text(TypeNone);
  s_text(TypeBase);
  s_text(TypeSkin);
  s_text(TypePoor);
  s_text(TypeBlend);
  s_text(TypeWater);
  s_text(TypeRiver);
  s_text(TypeOcean);
  s_text(TypeGlass);
  s_text(TypeFlame);
  s_text(TypeMirror);
  s_text(TypeLights);
  s_text(TypeIce);
#undef s_text

  return "TypeNone";
}

kgmShader::Shader kgmShader::toType(const char* str)
{
#define s_type(x) if(strcmp(str, #x) == 0) return x
  s_type(TypeNone);
  s_type(TypeNone);
  s_type(TypeBase);
  s_type(TypeSkin);
  s_type(TypePoor);
  s_type(TypeBlend);
  s_type(TypeWater);
  s_type(TypeRiver);
  s_type(TypeOcean);
  s_type(TypeGlass);
  s_type(TypeFlame);
  s_type(TypeMirror);
  s_type(TypeLights);
  s_type(TypeIce);
#undef s_type

  return kgmShader::TypeNone;
}
