#ifndef KGMIGRAPHICS_H
#define KGMIGRAPHICS_H

class kgmIcon;
class kgmLight;
class kgmVisual;
class kgmCamera;

class kgmIGraphics
{
public:
  virtual void add(kgmLight*)  = 0;
  virtual void add(kgmVisual*) = 0;

  virtual kgmCamera& camera() = 0;
};

#endif // KGMIGRAPHICS_H
