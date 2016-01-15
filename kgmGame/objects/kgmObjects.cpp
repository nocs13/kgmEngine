#include "kgmObjects.h"
KGMOBJECT_IMPLEMENT(kgmResult, kgmUnit);
KGMOBJECT_IMPLEMENT(kgmParticlesObject, kgmEffect);
KGMOBJECT_IMPLEMENT(kgmFlame, kgmParticlesObject);
KGMOBJECT_IMPLEMENT(kgmSmoke, kgmParticlesObject);
KGMOBJECT_IMPLEMENT(kgmExplode, kgmParticlesObject);
KGMOBJECT_IMPLEMENT(kgmLaser, kgmEffect);
KGMOBJECT_IMPLEMENT(kgmIonNozzle, kgmEffect);

kgmResult::kgmResult(kgmIGame* g, u32 time, s32 res, kgmString txt)
  :kgmUnit(g)
{
  text = txt;
  result = res;
  timeout(time);

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
  timeout(life);

  particles = new kgmParticles();
  m_visual  = new kgmVisual();

  material = new kgmMaterial();
  material->depth(false);
  material->blend(true);
  material->m_srcblend     = gcblend_srcalpha;
  material->m_dstblend     = gcblend_one;
  material->m_type         = "simple";
  material->setShader(g->getResources()->getShader("blend.glsl"));
  material->setTexColor(g->getResources()->getTexture(tid));

  particles->direction = dir;
  particles->volume    = vol;
  particles->m_speed   = speed;
  particles->m_count   = count;
  particles->m_life    = life;
  particles->m_loop    = loop;
  particles->st_size   = size_start;
  particles->en_size   = size_end;
  particles->div_life  = div_life;

  m_visual->set(material);
  m_visual->set(particles);

  setPosition(pos);

  kgmVariable var;

  var = kgmVariable("Count",  0,     &particles->m_count);
  m_variables.add(var);
  var = kgmVariable("Speed",  0.0f,  &particles->m_speed);
  m_variables.add(var);
  var = kgmVariable("dSpeed", 0.0f,  &particles->div_speed);
  m_variables.add(var);
  var = kgmVariable("Life",   0.0f,  &particles->m_life);
  m_variables.add(var);
  var = kgmVariable("dLife",  0.0f,  &particles->div_life);
  m_variables.add(var);
  var = kgmVariable("Loop",   false, &particles->m_loop);
  m_variables.add(var);
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
  material->m_srcblend     = gcblend_srcalpha;
  material->m_dstblend     = gcblend_one;
  material->m_type         = "simple";
  material->setShader(g->getResources()->getShader("blend.glsl"));
}

kgmExplode::kgmExplode(kgmIGame* g, vec3 pos, vec3 vol, vec3 dir,
           float speed, float div_speed, float life, float div_life,
           float size_start, float size_end, u32 count, bool loop, kgmString tid)
  :kgmParticlesObject(g, pos, vol, dir, speed, div_speed, life, div_life,
                      size_start, size_end, count, tid, loop)
{
  timeout(life);

  particles->div_life = 0.8;

  particles->build();

  m_visual->set(material);
  m_visual->set(particles);

  setPosition(pos);
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
  timeout(time);
  this->setId("laser");

  m_visual  = new kgmVisual();

  material = new kgmMaterial();
  material->cull(false);
  material->depth(false);
  material->blend(true);
  material->m_srcblend = gcblend_one;
  material->m_dstblend = gcblend_one;
  material->setTexColor(g->getResources()->getTexture((char*)"point_redd.tga"));
  material->setShader(g->getResources()->getShader("blend.glsl"));

  mesh = new kgmMesh();
  Vertex* v = (Vertex*)mesh->vAlloc(18, kgmMesh::FVF_P_C_T);

  v[0]  = { {0, -hwid, -hwid}, 0xffffffff, {0, 1}};
  v[1]  = { {0, -hwid,  hwid}, 0xffffffff, {0, 0}};
  v[2]  = { {0,  hwid,  hwid}, 0xffffffff, {1, 0}};
  v[3]  = { {0,  hwid,  hwid}, 0xffffffff, {1, 0}};
  v[4]  = { {0,  hwid, -hwid}, 0xffffffff, {1, 1}};
  v[5]  = { {0, -hwid, -hwid}, 0xffffffff, {0, 1}};

  v[6]  = { {hlen,  -hwid, 0}, 0xffffffff, {0, 1}};
  v[7]  = { {hlen,   hwid, 0}, 0xffffffff, {0, 0}};
  v[8]  = { {-hlen,  hwid, 0}, 0xffffffff, {1, 0}};
  v[9]  = { {-hlen,  hwid, 0}, 0xffffffff, {1, 0}};
  v[10] = { {-hlen, -hwid, 0}, 0xffffffff, {1, 1}};
  v[11] = { {hlen,  -hwid, 0}, 0xffffffff, {0, 1}};

  v[12] = { {hlen,  0, -hwid}, 0xffffffff, {0, 1}};
  v[13] = { {hlen,  0, hwid},  0xffffffff, {0, 0}};
  v[14] = { {-hlen, 0, hwid},  0xffffffff, {1, 0}};
  v[15] = { {-hlen, 0, hwid},  0xffffffff, {1, 0}};
  v[16] = { {-hlen, 0, -hwid}, 0xffffffff, {1, 1}};
  v[17] = { {hlen,  0, -hwid}, 0xffffffff, {0, 1}};

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
