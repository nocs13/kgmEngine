#include "SpriteRender.h"
#include "Draw.h"

#include "../kgmGraphics.h"

SpriteRender::SpriteRender(kgmGraphics* gr)
{
  this->gr = gr;

  mi.identity();
}

void SpriteRender::render()
{
  gr->gc->gcSetShader(null);
  gr->gc->gcDepth(false, 0, 0);
  gr->gc2DMode();

  gr->setWorldMatrix(mi);

  gr->render(gr->m_shaders[kgmGraphics::kgmShader_TypeGui]);

  for(int i = 0; gr->m_visible_sprites[i] != null; i++)
  {
  }

  gr->render((kgmShader*)null);

  gr->gc3DMode();
}

void SpriteRender::render(kgmVisual* visual)
{
  if(!visual)
    return;

  switch(visual->type())
  {
  case kgmVisual::TypeText:
  {
    kgmText* text = visual->getText();
    kgmGui::Rect rc(text->m_rect.x, text->m_rect.y,
                    text->m_rect.w, text->m_rect.h);

    gcDrawText(gr->gc, gr->font, text->m_size / 2, text->m_size, text->m_color, rc, text->m_text);
  }
    break;
  case kgmVisual::TypeSprite:
  {
    render(visual->getSprite());
  }
    break;
  }
}

void SpriteRender::render(kgmSprite* sprite)
{
  struct Point{ vec3 pos; u32 col; vec2 uv; };
  Point v[4];

  v[0].pos = vec3(sprite->m_rect.x, sprite->m_rect.y, 0);
  v[0].uv.x = 0.0f, v[0].uv.y = 0.0f;
  v[1].pos = vec3(sprite->m_rect.x, sprite->m_rect.y + sprite->m_rect.h, 0);
  v[1].uv.x = 0.0f, v[1].uv.y = 1.0f;
  v[2].pos = vec3(sprite->m_rect.x + sprite->m_rect.w, sprite->m_rect.y, 0);
  v[2].uv.x = 1.0f, v[2].uv.y = 0.0f;
  v[3].pos = vec3(sprite->m_rect.x + sprite->m_rect.w, sprite->m_rect.y + sprite->m_rect.h, 0);
  v[3].uv.x = 1.0f, v[3].uv.y = 1.0f;

  v[0].col = v[1].col = v[2].col = v[3].col = sprite->m_color.color;

  gr->gc->gcDraw(gcpmt_trianglestrip, gcv_xyz|gcv_col|gcv_uv0, sizeof(Point), 4, v, 0, 0, 0);
}
