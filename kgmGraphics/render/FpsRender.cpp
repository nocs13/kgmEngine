#include "FpsRender.h"
#include "Draw.h"

#include "../kgmGraphics.h"
#include "../../kgmSystem/kgmTime.h"

FpsRender::FpsRender(kgmGraphics* gr)
  :BaseRender(gr)
{
  mi.identity();

  text.alloc(24);
  memset(text.data(), 0, text.length());
}

void FpsRender::render()
{
  static u32 fps_start_time = kgmTime::getTicks();
  static u32 fps_frames = 0;
  static s8  fps_text[256] = {0};

  if(kgmTime::getTicks() - fps_start_time > 1000)
  {
    memset(text.data(), 0, text.length());
    sprintf(text.data(), "FPS: %i", fps_frames);
    fps_frames = 0;
    fps_start_time = kgmTime::getTicks();
  }
  else
  {
    fps_frames++;
  }

  mtl.m_color = kgmMaterial::Color(0xffffffff);
  gr->set(&mtl);
  gr->set((kgmShader*)gr->m_shaders[kgmGraphics::ShaderGui]);
  gr->shaderSetGeneral();
  gr->shaderSetPrivate();

  gcDrawText(gr->gc, gr->font, 10, 15, 0xffffffff, kgmGui::Rect(gr->m_viewport.width() - 200, 1, 90, 20), text);
}
