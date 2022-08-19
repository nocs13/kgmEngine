#include "BaseRender.h"
#include "../../kgmGraphics/kgmMesh.h"
#include "../../kgmGraphics/kgmIcon.h"
#include "../../kgmGraphics/kgmText.h"
#include "../../kgmGraphics/kgmLight.h"
#include "../../kgmGraphics/kgmCamera.h"
#include "../../kgmGraphics/kgmVisual.h"
#include "../../kgmGraphics/kgmMaterial.h"
#include "../../kgmGraphics/kgmGraphics.h"

BaseRender::BaseRender(kgmGraphics* g)
{
  gr = g;
  gc = g->gc;

  m_sh_base = gr->m_shaders[kgmMaterial::TypeBase];
}

void BaseRender::render(kgmCamera* cam, kgmIGraphics::INode* nod)
{
  if (!cam || !nod || nod->getNodeType() != kgmIGraphics::NodeMesh)
    return;

  box3 bound = nod->getNodeBound();

  vec3  l = bound.max - bound.min;
  vec3  v = (bound.min + bound.max) * 0.5;

  kgmShader*   s = m_sh_base;
  kgmMesh*     msh = (kgmMesh*) nod->getNodeObject();
  kgmMaterial* mtl = (nod->getNodeMaterial()) ? (nod->getNodeMaterial()) : (gr->m_def_material);

  box3    bbound = nod->getNodeBound();
  sphere3 sbound;

  bbound.min = nod->getNodeTransform() * bbound.min;
  bbound.max = nod->getNodeTransform() * bbound.max;

  sbound.center = bbound.center();
  sbound.radius = 0.5f * bbound.dimension().length();

  mtx4 m        = nod->getNodeTransform();
  float shine   = mtl->shininess();
  vec4 color    = mtl->m_color.get();

  gchandle tcolor = null;

  if(mtl->hasTexColor())
  {
    gc->gcSetTexture(0, mtl->getTexColor()->texture());
    tcolor = mtl->getTexColor()->texture();
  }
  else
  {
    gc->gcSetTexture(0, gr->m_tex_white->texture());
    tcolor = gr->m_tex_white->texture();
  }

  s->start();

  s->set("g_fShine",          mtl->shininess());
  s->set("g_mProj",           cam->mProj);
  s->set("g_mView",           cam->mView);
  s->set("g_mTran",           m);
  s->set("g_vColor",          color);
  s->set("g_vEye",            cam->mPos);
  s->set("g_vLook",           cam->mDir);

  if (m_clip_planes[0].on)
  {
    s->set("g_iClipping", 1);
    s->set("g_vClipPlane", m_clip_planes[0].plane);
  }
  else
  {
    s->set("g_iClipping", 0);
  }

  s->set("g_txColor", 0);

  draw(msh);

  s->stop();

  gc->gcSetTexture(0, 0);
  gc->gcSetTexture(1, 0);
  gc->gcSetTexture(2, 0);
  gc->gcSetTexture(3, 0);

}

void BaseRender::draw(kgmVisual* visual)
{
  if(!visual)
    return;

  switch(visual->type())
  {
  case kgmVisual::TypeNone:
  case kgmVisual::TypeMesh:
  {
    kgmMesh* msh = visual->getMesh();

    u32  pmt;

    if(!msh)
      return;

    switch(msh->rtype())
    {
    case kgmMesh::RT_LINE:
      pmt = gcpmt_lines;
      break;
    case kgmMesh::RT_POINT:
      pmt = gcpmt_points;
      break;
    default:
      pmt = gcpmt_triangles;
    };

    gc->gcDraw(pmt, msh->fvf(),
               msh->vsize(), msh->vcount(), msh->vertices(),
               2, 3 * msh->fcount(), msh->faces());

  }
    break;
  case kgmVisual::TypeText:
  {
    kgmText* text = visual->getText();
    kgmGui::Rect rc(text->m_rect.x, text->m_rect.y,
                    text->m_rect.w, text->m_rect.h);
    gr->gcDrawText(gr->font, text->m_size / 2, text->m_size, text->m_color, rc, text->m_text);
  }
    break;
  }
}

void BaseRender::draw(kgmMesh *m)
{
  if(!m)
    return;

  size_t t = (size_t) m;

  kgmMesh* mesh = (kgmMesh*) t;

  u32  pmt;
  u32  ibs;

  u32  typ = mesh->rtype();

  switch(typ)
  {
  case kgmMesh::RT_LINE:
    pmt = gcpmt_lines;
    ibs = 2;
    break;
  case kgmMesh::RT_POINT:
    pmt = gcpmt_points;
    ibs = 1;
    break;
  default:
    pmt = gcpmt_triangles;
    ibs = 3;
  };

  u32 fvf    = mesh->fvf();
  u32 vsize  = mesh->vsize();
  u32 vcount = mesh->vcount();

  //if (m->gpu())
  //{
  //  gc->gcDrawVertexBuffer(m->vertices(), pmt, m->fvf(), m->vsize(), m->vcount(),
  //                         m->fsize() / ibs, ibs * m->fcount(), 0);
  //}
  //else
  //{
    gc->gcDraw(pmt, mesh->fvf(), mesh->vsize(), mesh->vcount(), mesh->vertices(),
               mesh->fsize() / ibs, ibs * mesh->fcount(), mesh->faces());
  //}
}

void BaseRender::draw(kgmParticles* particles)
{
  if(!particles || !particles->getMesh())
    return;

  draw(particles->getMesh());
}

void BaseRender::draw(kgmIcon* icon, kgmCamera* c)
{
  vec3    rv, uv;

  if (c)
    return;

  rv = vec3(c->mView.m[0], c->mView.m[2], c->mView.m[1]);
  rv.normalize();
  uv = rv.cross(c->mDir);
  uv.normalize();

  vec3 pos   = icon->getPosition();
  vec3 crv = rv * icon->getWidth(),
       cuv = uv * icon->getHeight();

  kgmMesh::Vertex_P_C_T points[6];

  points[0].pos = (pos - crv + cuv);
  points[0].uv = vec2(0, 1);
  points[1].pos = (pos - crv - cuv);
  points[1].uv = vec2(0, 0);
  points[2].pos = (pos + crv + cuv);
  points[2].uv = vec2(1, 1);

  points[3].pos = (pos + crv - cuv);
  points[3].uv = vec2(1, 0);
  points[4].pos = (pos + crv + cuv);
  points[4].uv = vec2(1, 1);
  points[5].pos = (pos - crv - cuv);
  points[5].uv = vec2(0, 0);

  points[0].col = points[1].col =
  points[2].col = points[3].col =
  points[4].col = points[5].col = kgmColor::toVector(0xffffffff);

  gc->gcDraw(gcpmt_triangles, gcv_xyz|gcv_col|gcv_uv0, sizeof(kgmMesh::Vertex_P_C_T), 6, points, 0, 0, 0);
}

void BaseRender::material(kgmMaterial* m)
{
  if(!m)
  {
    gc->gcSetTexture(0, 0);
    gc->gcSetTexture(1, 0);
    gc->gcSetTexture(2, 0);
    gc->gcSetTexture(3, 0);

    gc->gcCull(1);
    gc->gcBlend(false, 0, 0, 0);
    gc->gcDepth(true, true, gccmp_lequal);

    return;
  }

  if(m->blend())
  {
    switch(m->blend())
    {
    case kgmMaterial::Blend_Add:
      //gc->gcBlend(true, 0, gcblend_srcalpha, gcblend_one);
      gc->gcBlend(true, 0, gcblend_one, gcblend_one);
      break;
    case kgmMaterial::Blend_Mul:
      gc->gcBlend(true, 0, gcblend_dstcol, gcblend_zero);
      break;
    case kgmMaterial::Blend_Sub:
      gc->gcBlend(true, gcblend_eqsub, gcblend_dstcol, gcblend_zero);
      break;
    case kgmMaterial::Blend_Inter:
      gc->gcBlend(true, 0, gcblend_srcalpha, gcblend_srcialpha);
      break;
    case kgmMaterial::Blend_CBurn:
      gc->gcBlend(true, 0, gcblend_one, gcblend_srcicol);
      break;
    case kgmMaterial::Blend_LBurn:
      gc->gcBlend(true, 0, gcblend_one, gcblend_one);
      break;
    case kgmMaterial::Blend_CDodge:
      gc->gcBlend(true, 0, gcblend_dstcol, gcblend_zero);
      break;
    case kgmMaterial::Blend_LDodge:
      gc->gcBlend(true, 0, gcblend_srcalpha, gcblend_one);
      break;
    case kgmMaterial::Blend_Screen:
      gc->gcBlend(true, 0, gcblend_one, gcblend_srcicol);
      break;
    case kgmMaterial::Blend_Darken:
      gc->gcBlend(true, gcblend_eqmin, gcblend_one, gcblend_one);
      break;
    case kgmMaterial::Blend_Lighten:
      gc->gcBlend(true, gcblend_eqmax, gcblend_one, gcblend_one);
      break;
    }
  }
  else if(m->transparency() > 0.0f)
  {
    gc->gcBlend(true, 0, gcblend_srcalpha, gcblend_srcialpha);
  }

  if(!m->depth())
  {
    gc->gcDepth(false, true, gccmp_less);
  }

  if(!m->cull())
  {
    gc->gcCull(0);
  }

  if(m->hasTexColor())
  {
    gc->gcSetTexture(0, m->getTexColor()->texture());
  }
  else
  {
    gc->gcSetTexture(0, gr->m_tex_white->texture());
  }

  if(m->hasTexNormal())
  {
    gc->gcSetTexture(1, m->getTexNormal()->texture());
  }
  else
  {
    gc->gcSetTexture(1, gr->m_tex_gray->texture());
  }

  if(m->hasTexSpecular())
  {
    gc->gcSetTexture(2, m->getTexSpecular()->texture());
  }
  else
  {
    gc->gcSetTexture(2, gr->m_tex_black->texture());
  }
}

void BaseRender::shader(kgmShader* shd, kgmCamera* cam, kgmMaterial* mtl, kgmIGraphics::INode* nod)
{
  if(!shd)
  {
    gc->gcSetShader(null);

    return;
  }

  //send default parameters
  vec4 v_light(0, 0, 0, 10);
  vec4 v_light_color(1, 1, 1, 1);
  vec4 v_light_direction(0, 0, 1, 0);

  float random    = (float)rand() / (float)RAND_MAX;
  mtx4  transform = nod->getNodeTransform();
  vec4 color      = mtl->m_color.get();
  f32  shininess  = mtl->shininess();
  f32  time       = kgmTime::getTime();

  shd->start();
  shd->set("g_fTime",           time);
  shd->set("g_fRandom",         random);
  shd->set("g_fShine",          shininess);
  shd->set("g_mProj",           cam->mProj);
  shd->set("g_mView",           cam->mView);
  shd->set("g_mTran",           transform);
  shd->set("g_vColor",          color);
  shd->set("g_vLight",          v_light);
  shd->set("g_vLightColor",     v_light_color);
  shd->set("g_vLightDirection", v_light_direction);
  shd->set("g_vUp",             cam->mUp);
  shd->set("g_vEye",            cam->mPos);
  shd->set("g_vLook",           cam->mDir);
  shd->set("g_iClipping",       0);

  shd->set("g_txColor", 0);
  shd->set("g_txNormal", 1);
  shd->set("g_txSpecular", 2);

  kgmString lcolor = "g_vLights[*].color";
  kgmString lposition = "g_vLights[*].position";
  kgmString ldirection = "g_vLights[*].direction";

  /* for(u32 i = 0; i < 1; i++)
  {
    char c = '0' + (char) i;
    lcolor[10] = c;
    lposition[10] = c;
    ldirection[10] = c;

     s->set(lcolor, lights[i].color);
     s->set(lposition, lights[i].position);
     s->set(ldirection, lights[i].direction);
   } */
}
