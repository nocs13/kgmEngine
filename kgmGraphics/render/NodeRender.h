/*
 * NodeRender.h
 *
 *  Created on: May 17, 2018
 *      Author: guram
 */

#ifndef KGMGRAPHICS_RENDER_NODERENDER_H_
#define KGMGRAPHICS_RENDER_NODERENDER_H_

#include "../../kgmBase/kgmIGC.h"
#include "../kgmGui.h"

#include "BaseRender.h"

class kgmGraphics;

class NodeRender: public BaseRender
{

public:
	NodeRender(kgmGraphics*);
	~NodeRender();
};

#endif /* KGMGRAPHICS_RENDER_NODERENDER_H_ */
