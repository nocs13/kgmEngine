#include "IconRender.h"
#include "../kgmGraphics.h"

IconRender::IconRender(kgmGraphics* g)
  :BaseRender(g)
{
  gr = g;
}

void IconRender::render()
{
  mtx4 mtx;
  mtx.identity();
  gr->setWorldMatrix(mtx);

  for(int i = gr->m_icons.size(); i > 0; i--)
  {
    kgmIcon* icon = gr->m_icons[i - 1];

    if(icon == null)
    {
      gr->m_icons.erase(i - 1);
    }
    else
    {
      kgmMaterial mtl;

      gr->gc->gcBlend(true, 0, gcblend_srcalpha, gcblend_srcialpha);
      mtl.setTexColor(icon->getIcon());
      gr->set(&mtl);
      gr->set(gr->m_shaders[kgmMaterial::TypeBase]);
      gr->draw(icon);
      gr->set((kgmShader*)null);
      gr->set((kgmMaterial*)null);
      gr->gc->gcBlend(false, 0, gcblend_srcalpha, gcblend_srcialpha);
    }
  }

  if (gr->m_show_lights && gr->m_icon_light) {
    kgmIcon *li = gr->m_icon_light;

    for(int i = gr->m_a_light_count; i > 0; i--)
    {
      kgmIGraphics::INode* nl = gr->m_a_lights[i - 1];
      li->setPosition(nl->getNodePosition());
      render(li);
    }
  }
}

void IconRender::render(kgmList<kgmLight*>& lights)
{
  for(kgmList<kgmLight*>::iterator i = lights.begin(); !i.end(); ++i) {
  }
}

void IconRender::render(kgmIcon* icon)
{
  mtx4    mtr = gr->getViewMatrix();
  vec3    rv, uv;

  rv = vec3(mtr.m[0], mtr.m[2], mtr.m[1]);
  rv.normalize();
  uv = rv.cross(gr->m_camera->mDir);
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

  gr->gc->gcDraw(gcpmt_triangles, gcv_xyz|gcv_col|gcv_uv0, sizeof(kgmMesh::Vertex_P_C_T), 6, points, 0, 0, 0);
}
