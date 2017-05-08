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
    kgmVisual::Mesh* msh = visual->getMesh();

    u32  pmt;

    if(!msh)
      return;

    switch(msh->getRenderType())
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

    gc->gcDraw(pmt, msh->getFvf(),
               msh->getVsize(), msh->getVcount(), msh->getVertices(),
               2, 3 * msh->getFcount(), msh->getFaces());

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

  u32  pmt;

  switch(m->m_rtype)
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

  gc->gcDraw(pmt, m->fvf(), m->vsize(),
             m->vcount(), m->vertices(),
             2, 3 * m->fcount(), m->faces());
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
  points[4].col = points[5].col = 0xffffffff;

  gc->gcDraw(gcpmt_triangles, gcv_xyz|gcv_col|gcv_uv0, sizeof(kgmMesh::Vertex_P_C_T), 6, points, 0, 0, 0);
}
