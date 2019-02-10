/*
 * Terrain.h
 *
 *  Created on: Feb 10, 2019
 *      Author: Goga
 */

#ifndef KGMGRAPHICS_RENDER_TERRAIN_H_
#define KGMGRAPHICS_RENDER_TERRAIN_H_

#include "BaseRender.h"

class kgmGraphics;

class kgmShader;

namespace Render
{

class Terrain: public BaseRender
{
  kgmShader* m_shader;
  
public:
  Terrain(kgmGraphics* gr);
  ~Terrain();

  void render();
};

} /* namespace Render */

#endif /* KGMGRAPHICS_RENDER_TERRAIN_H_ */
