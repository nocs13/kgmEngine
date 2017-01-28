#include "kgmObjects.h"

kgmResult::kgmResult(kgmIGame* g, u32 time, s32 res, kgmString txt)
  :kgmUnit(g)
{
  text = txt;
  result = res;
  setLiving(time);

  int cx, cy, cw, ch, tw, tl;
  //game->getWindow()->getRect(cx, cy, cw, ch);

  tl = txt.length();

  if(tl < 1)
    tl = 1;

  tw = 300 / tl;

  m_visual  = new kgmVisual();
  m_text = new kgmText();
  m_text->m_rect  = uRect(10, ch - 2 * tw, 300, 2 * tw);
  m_text->m_text  = txt;
  m_text->m_size  = tw;

  m_visual->set(m_text);
}

kgmResult::~kgmResult()
{
  delete m_text;
}

s32 kgmResult::getResult()
{
  return result;
}

void kgmResult::update(u32 t)
{
  kgmUnit::update(t);
}

void kgmResult::init()
{
}

void kgmResult::exit()
{
}

kgmParticlesObject::kgmParticlesObject(kgmIGame* g,  vec3 pos, vec3 vol, vec3 dir,
                                       float speed, float div_speed,
                                       float life,  float div_life,
                                       float size_start, float size_end,
                                       u32 count,  kgmString tid, bool loop)
  :kgmEffect(g)
{
  setLiving(life);

  particles = new kgmParticles();
  m_visual  = new kgmVisual();

  material = new kgmMaterial();
  material->depth(false);
  material->blend(true);
  material->srcblend(gcblend_srcalpha);
  material->dstblend(gcblend_one);
  material->setTexColor(g->getResources()->getTexture(tid));

  particles->speed(speed);
  particles->count(count);
  particles->life(life);
  particles->size(size_start);
  particles->esize(size_end);
  particles->loop(loop);

  m_visual->set(material);
  m_visual->set(particles);

  position(pos);

  kgmVariable var;

  idTex = tid;
  var = kgmVariable("tId",   kgmString(""),     &idTex);
  m_variables.add(var);
}

kgmParticlesObject::~kgmParticlesObject()
{
  delete material;
  delete particles;
}

void kgmParticlesObject::setTexture(kgmTexture* t)
{
  material->setTexColor(t);
}

void kgmParticlesObject::setSlideFrames(u32 rows, u32 cols)
{
  particles->tex_slide_rows = rows;
  particles->tex_slide_cols = cols;
}

void kgmParticlesObject::init()
{
  particles->build();
}

void kgmParticlesObject::update(u32 ms)
{
  particles->update(ms);
}

#ifdef EDITOR
void kgmParticlesObject::eupdate()
{
  material->setTexColor(game()->getResources()->getTexture(idTex));
  particles->build();
}
#endif

kgmFlame::kgmFlame(kgmIGame* g, vec3 pos, vec3 vol, vec3 dir,
                   float speed, float div_speed, float life,  float div_life,
                   float size_start, float size_end, u32 count,
                   kgmString tid, bool loop)
  :kgmParticlesObject(g, pos, vol, dir, speed, div_speed, life, div_life,
                      size_start, size_end, count, tid, loop)
{
}

kgmSmoke::kgmSmoke(kgmIGame* g, vec3 pos, vec3 vol, vec3 dir,
                   float speed, float div_speed, float life,  float div_life,
                   float size_start, float size_end, u32 count,
                   kgmString tid, bool loop)
  :kgmParticlesObject(g, pos, vol, dir, speed, div_speed, life, div_life,
                      size_start, size_end, count, tid, loop)
{
  material->blend(true);
  material->srcblend(gcblend_srcalpha);
  material->dstblend(gcblend_one);
}

kgmExplode::kgmExplode(kgmIGame* g, vec3 pos, vec3 vol, vec3 dir,
           float speed, float div_speed, float life, float div_life,
           float size_start, float size_end, u32 count, bool loop, kgmString tid)
  :kgmParticlesObject(g, pos, vol, dir, speed, div_speed, life, div_life,
                      size_start, size_end, count, tid, loop)
{
  setLiving(life);

  particles->build();

  m_visual->set(material);
  m_visual->set(particles);

  position(pos);
}

kgmLaser::kgmLaser(kgmIGame* g)
  :kgmEffect(g)
{
}

kgmLaser::kgmLaser(kgmIGame* g, u32 time, vec3 pos, vec3 rot,
                   float speed, float hlen, float hwid)
  :kgmEffect(g)
{
  power = 1;
  setLiving(time);
  this->setName("laser");

  m_visual  = new kgmVisual();

  material = new kgmMaterial();
  material->cull(false);
  material->depth(false);
  material->blend(true);
  material->srcblend(gcblend_one);
  material->dstblend(gcblend_one);
  material->setTexColor(g->getResources()->getTexture((char*)"point_redd.tga"));
  material->setShader(g->getResources()->getShader("blend.glsl"));

  mesh = new kgmMesh();
  Vertex* v = (Vertex*)mesh->vAlloc(18, kgmMesh::FVF_P_C_T);

  v[0]  = { {0, -hwid, -hwid}, 0xffffffff, {0, 1} };
  v[1]  = { {0, -hwid,  hwid}, 0xffffffff, {0, 0} };
  v[2]  = { {0,  hwid,  hwid}, 0xffffffff, {1, 0} };
  v[3]  = { {0,  hwid,  hwid}, 0xffffffff, {1, 0} };
  v[4]  = { {0,  hwid, -hwid}, 0xffffffff, {1, 1} };
  v[5]  = { {0, -hwid, -hwid}, 0xffffffff, {0, 1} };

  v[6]  = { {hlen,  -hwid, 0}, 0xffffffff, {0, 1} };
  v[7]  = { {hlen,   hwid, 0}, 0xffffffff, {0, 0} };
  v[8]  = { {-hlen,  hwid, 0}, 0xffffffff, {1, 0} };
  v[9]  = { {-hlen,  hwid, 0}, 0xffffffff, {1, 0} };
  v[10] = { {-hlen, -hwid, 0}, 0xffffffff, {1, 1} };
  v[11] = { {hlen,  -hwid, 0}, 0xffffffff, {0, 1} };

  v[12] = { {hlen,  0, -hwid}, 0xffffffff, {0, 1} };
  v[13] = { {hlen,  0, hwid},  0xffffffff, {0, 0} };
  v[14] = { {-hlen, 0, hwid},  0xffffffff, {1, 0} };
  v[15] = { {-hlen, 0, hwid},  0xffffffff, {1, 0} };
  v[16] = { {-hlen, 0, -hwid}, 0xffffffff, {1, 1} };
  v[17] = { {hlen,  0, -hwid}, 0xffffffff, {0, 1} };

  m_visual->set(mesh);
  m_visual->set(material);

  m_body = new kgmBody();

  m_body->translate(pos);
  m_body->rotate(rot);
  m_body->m_velocity  = speed;
  m_body->m_gravity   = false;
  m_body->m_udata     = this;
  m_body->m_mass      = 0.0f;
  m_body->m_bound.min = vec3(-hlen, -hwid, -hwid);
  m_body->m_bound.max = vec3( hlen,  hwid,  hwid);
}

kgmLaser::~kgmLaser()
{
  delete mesh;
  delete material;
}
