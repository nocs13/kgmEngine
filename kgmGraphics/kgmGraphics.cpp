#include "kgmGraphics.h"
#include "kgmGui.h"
#include "kgmMesh.h"
#include "kgmMaterial.h"
#include "kgmTexture.h"
#include "kgmShader.h"
#include "kgmAnimation.h"
#include "kgmSkeleton.h"
#include "kgmFrustum.h"
#include "kgmCamera.h"

#include "../kgmMath/kgmBase.h"
#include "../kgmMain/kgmTime.h"

KGMOBJECT_IMPLEMENT(kgmFrustum,		kgmObject);
KGMOBJECT_IMPLEMENT(kgmCamera,		kgmFrustum);

kgmGraphics::kgmGraphics(kgmWindow* w)
{
  gc = w->getGC();
  wnd = w;
  node = new kgmNode((kgmObject*)0);

  fwidth = 800;
  fheight = 600;
  zfar = 100000.0;
  znear = 1.0;

  camera = 0;
}

kgmGraphics::~kgmGraphics()
{
  if(node){
    delete node;
  }
  guis.clear();
}

void kgmGraphics::optimize()
{

}

void kgmGraphics::render()
{
  static float alpha = 0.0;
  static float m_time[4];
  vec3 v[2];
  mtx4 m;
  vec4 myvar;
  int i = 0;

   mtx4 mvw, mpr;
   int  rc[4];


   //mpr.perspective(0.25 * PI, (float)rc[2]/(float)rc[3], 0.1, 10000000.0);
   //mvw.lookat(vec3(0, 0, 1), vec3(1, 0, 0), vec3(0, 0, 1));
   //gc->gcSetMatrix(gcmtx_view, mvw.m);
   //gc->gcSetMatrix(gcmtx_proj, mpr.m);

   //wnd->getRect(rect[0], rect[1], rect[2], rect[3]);
   //gc->gcSetViewport(0, 0, fwidth, fheight, 1.0, 1000.0);
   gc->gcClear(gcflag_color | gcflag_depth, 0xff0000ff, 1, 0);

   //mpr.perspective(0.25 * PI, (float)fwidth/(float)fheight, 0.1, 10000000.0);
   //mvw.lookat(vec3(0, 0, 1), vec3(1, 0, 0), vec3(0, 0, 1));
   //gc->gcSetMatrix(gcmtx_view, mvw.m);
   //gc->gcSetMatrix(gcmtx_proj, mpr.m);

   gc->gcBegin();
   render(node);
   gc->gc2DMode();
   //For last step draw gui
   for(int i = 0; i < guis.size(); i++)
     guis[i]->repaint(gc);
   gc->gc3DMode();
   gc->gcRender();
   //clear&reset
}

void kgmGraphics::render(kgmNode* n)
{
  if(!n)
    return;
  switch(n->type){
  case kgmNode::NodeCamera:
    if(n->camera){
      gc->gcSetMatrix(gcmtx_proj, n->camera->mProj.m);
      gc->gcSetMatrix(gcmtx_view, n->camera->mView.m);
      camera = n->camera;
    }else{
      mtx4 mvw, mpr;
      mvw.identity();
      mpr.identity();
      gc->gcSetMatrix(gcmtx_view, mvw.m);
      gc->gcSetMatrix(gcmtx_proj, mpr.m);
    }
    break;
  case kgmNode::NodeMatrix:
    gc->gcSetMatrix(gcmtx_view, n->transform->m);
    mtransform = n->transform;
    break;
  case kgmNode::NodeMaterial:
    if(!n->material){
      gc->gcSetShader(0);
      gc->gcSetTexture(0, 0);
      gc->gcSetTexture(1, 0);
      gc->gcSetTexture(2, 0);
      gc->gcSetTexture(3, 0);
      return;
    }

    if(n->material->m_tex_diffuse){
      gc->gcSetTexture(0, n->material->m_tex_diffuse->m_texture);
      tdiffuse = n->material->m_tex_diffuse->m_texture;
    }
    if(n->material->m_tex_normal){
      gc->gcSetTexture(1, n->material->m_tex_normal->m_texture);
      tnormal = n->material->m_tex_normal->m_texture;
    }
    if(n->material->m_tex_specular){
      gc->gcSetTexture(1, n->material->m_tex_specular->m_texture);
      tspecular = n->material->m_tex_specular->m_texture;
    }

    if(n->material->m_shader){
        n->material->m_shader->start();
        shader = n->material->m_shader->m_shader;
        for(int i = 0; i < n->material->m_shader->m_input.length(); i++){
          kgmString        id;
          kgmShader::Type  type;
          n->material->m_shader->m_input.get(i, type, id);
          switch(type){
          case kgmShader::fTime:
            n->material->m_shader->set(id.data(), kgmTime::getTime());
            break;
          case kgmShader::fRandom:
            n->material->m_shader->set(id.data(), (float)rand()/(float)RAND_MAX);
            break;
          case kgmShader::mView:
            n->material->m_shader->set(id.data(), camera->mView);
            break;
          case kgmShader::mProjection:
            n->material->m_shader->set(id.data(), camera->mProj);
            break;
          case kgmShader::mViewProjection:
            n->material->m_shader->set(id.data(), camera->mVwPj);
            break;
          case kgmShader::vEyePosition:
            n->material->m_shader->set(id.data(), camera->mPos);
            break;
          case kgmShader::vEyeDirection:
            n->material->m_shader->set(id.data(), camera->mDir);
            break;
          }
        }
    }
    break;
  case kgmNode::NodeMesh:
    gc->gcDraw(gcpmt_triangles, n->mesh->fvf(), n->mesh->vsize(), n->mesh->vcount(), n->mesh->vertices(),
               2, 3 * n->mesh->fcount(),  n->mesh->faces());
    break;
  default:
    break;
  }

  for(int i = 0; i < n->nodes.size(); i++)
    render(n->nodes[i]);
}

void kgmGraphics::resize(int w, int h)
{
  mtx4 mvw, mpr;

  fwidth = w;
  fheight = h;
  mpr.perspective(0.25 * PI, (float)w/(float)h, 0.1, 10000000.0);
  mvw.lookat(vec3(0, 0, 1), vec3(1, 0, 0), vec3(0, 0, 1));

  gc->gcSetViewport(0, 0, w, h, znear, zfar);
  gc->gcSetMatrix(gcmtx_view, mvw.m);
  gc->gcSetMatrix(gcmtx_proj, mpr.m);
}


void kgmGraphics::add(kgmNode* n)
{
 if(!n)
   return;
 node->add(n);
}

void kgmGraphics::add(kgmGui* g)
{
  guis.add(g);
}

void kgmGraphics::erase(kgmNode* n)
{
 node->erase(n);
}

void kgmGraphics::erase(kgmGui* g)
{
  for(int i = (guis.size() - 1); i >= 0; i--)
    if(guis[i] == g)
      guis.erase(i);
}
