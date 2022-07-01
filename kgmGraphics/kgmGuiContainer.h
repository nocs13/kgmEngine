#include "kgmGui.h"

/* This container will made similar as bootstrap 's-12' layout but
   Instead 12 columns will be used 5 only, as 20% of full space. */

class kgmGuiContainer: public kgmGui {
    KGM_OBJECT(kgmGuiContainer);

    struct Cell {
      u32 width, height;
      u32 align;
      kgmGui* gui;
    };

    kgmList<Cell> _cells;
public:
    enum {
      Cell_1 = 1,
      Cell_2,
      Cell_3,
      Cell_4,
      Cell_5,
    };
public:
    kgmGuiContainer();
    kgmGuiContainer(kgmGui *par, u32 x, u32 y, u32 w, u32 h);
    ~kgmGuiContainer();
};
