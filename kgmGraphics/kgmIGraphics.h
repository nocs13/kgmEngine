#ifndef KGMIGRAPHICS_H
#define KGMIGRAPHICS_H

class kgmIcon;
class kgmLight;
class kgmVisual;
class kgmCamera;
class kgmResource;

class kgmIGraphics
{
public:
  virtual void clean()  = 0;
  virtual void render() = 0;

  virtual void render_3d() = 0;
  virtual void render_2d() = 0;

  virtual void add(kgmLight*)  = 0;
  virtual void add(kgmVisual*) = 0;

  virtual bool resource(kgmResource*)      = 0;
  virtual kgmResource* resource(kgmString) = 0;

  virtual kgmCamera& camera() = 0;
};

#endif // KGMIGRAPHICS_H
