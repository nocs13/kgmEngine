#include "kgmShader.h"
#include "kgmTexture.h"
#include "../kgmBase/kgmIGraphics.h"

KGMOBJECT_IMPLEMENT(kgmShader, kgmResource);

kgmShader::kgmShader(kgmIGraphics* g)
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

void kgmShader::set(const char* id, float& v)
{
  m_gc->gcUniform(m_shader, 0, 1, id, &v);
}

void kgmShader::set(const char* id, vec2& v)
{
  m_gc->gcUniform(m_shader, 0, 2, id, &v);
}

void kgmShader::set(const char* id, vec3& v)
{
  m_gc->gcUniform(m_shader, 0, 3, id, &v);
}

void kgmShader::set(const char* id, vec4& v)
{
  m_gc->gcUniform(m_shader, 0, 4, id, &v);
}

void kgmShader::set(const char* id, int v)
{
  m_gc->gcUniform(m_shader, 1, 1, id, &v);
}

void kgmShader::set(const char* id, mtx4& v)
{
  m_gc->gcUniformMatrix(m_shader, 4, 0, id, v.m);
}

void kgmShader::set(const char* id, mtx3& v)
{
  m_gc->gcUniformMatrix(m_shader, 3, 0, id, v.m);
}

void kgmShader::set(const char* id, void* t)
{
  m_gc->gcUniformSampler(m_shader, id, t);
}

void kgmShader::attr(int i, const char* a)
{
  m_gc->gcBindAttribute(m_shader, i, a);
}
