#include "kgmShader.h"
#include "kgmTexture.h"
#include "../kgmBase/kgmIGC.h"

KGMOBJECT_IMPLEMENT(kgmShader, kgmResource);

kgmShader::kgmShader(kgmIGC* g)
{
  m_gc     = g;
  m_shader = 0;
  m_type   = TypeShader;

  m_maps  = MAP_COLOR | MAP_NORMAL | MAP_SPECULAR;
  m_input = IN_MTX4_PROJ | IN_MTX4_VIEW | IN_MTX4_TRAN | IN_VEC3_EYEPOS | IN_VEC3_EYEDIR
                | IN_VEC4_LIGHT | IN_VEC1_TIME | IN_VEC1_RANDOM;
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
