#include "kgmShader.h"
#include "kgmTexture.h"
#include "../kgmBase/kgmIGC.h"

kgmShader::kgmShader(kgmIGC* g)
{
  m_gc     = g;
  m_shader = 0;

  m_input = IN_MTX4_PROJ | IN_MTX4_VIEW | IN_MTX4_TRAN | IN_VEC3_EYEPOS | IN_VEC3_EYEDIR | 
            IN_VEC4_LIGHT | IN_VEC1_TIME | IN_VEC1_RANDOM | 
            IN_MAP_COLOR | IN_MAP_NORMAL | IN_MAP_SPECULAR;
}

kgmShader::kgmShader(const kgmShader& s)
{
  m_gc     = s.m_gc;
  m_input  = s.m_input;
  m_shader = s.m_shader;
}

kgmShader::~kgmShader()
{
  m_values.clear();
}

void kgmShader::start()
{
  m_gc->gcSetShader(m_shader);
}

void kgmShader::stop()
{
  m_gc->gcSetShader(0);
}

void kgmShader::setValue(const char* id, kgmShader::TypeValue type, void* data, u32 count)
{
  if (!id)
    return;

  Value val = {id, type, data, count};

  m_values.set(id, val);
}

void kgmShader::delValue(const char* id)
{
  if (!id)
    return;

  kgmTab<const char*, Value>::iterator i = m_values[id];

  m_values.erase(i);
}

void kgmShader::useValues()
{
  kgmTab<const char*, Value>::iterator i;

  for(i = m_values.begin(); i.valid() && !i.isEnd(); i.next())
  {
    Value val = (*i);

    switch(val.type)
    {
    case Val_Float:
      m_gc->gcUniform(m_shader, gcunitype_float1, val.count, val.id, val.data);
      break;
    case Val_Vec2:
      m_gc->gcUniform(m_shader, gcunitype_float2, val.count, val.id, val.data);
      break;
    case Val_Vec3:
      m_gc->gcUniform(m_shader, gcunitype_float3, val.count, val.id, val.data);
      break;
    case Val_Vec4:
      m_gc->gcUniform(m_shader, gcunitype_float4, val.count, val.id, val.data);
      break;
    case Val_Mtx3:
      m_gc->gcUniformMatrix(m_shader, gcunitype_mtx3, val.count, 0, val.id, val.data);
      break;
    case Val_Mtx4:
      m_gc->gcUniformMatrix(m_shader, gcunitype_mtx4, val.count, 0, val.id, val.data);
      break;
    case Val_Int:
      m_gc->gcUniform(m_shader, gcunitype_int1, val.count, val.id, val.data);
      break;
    default:
      break;
    }
  }
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

void kgmShader::set(const char* id, int v)
{
  m_gc->gcUniform(m_shader, gcunitype_int1, 1, id, &v);
}

void kgmShader::set(const char* id, int *v, int count)
{
  m_gc->gcUniform(m_shader, gcunitype_int1, count, id, v);
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
