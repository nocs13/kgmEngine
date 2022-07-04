#pragma once
#include "kgmGui.h"

class kgmGuiContainer: public kgmGui {
  KGM_OBJECT(kgmGuiContainer);

public:
    const u32 cols_per_page = 5;

    enum CellSize {
      CellSize_1 = 1,
      CellSize_2,
      CellSize_3,
      CellSize_4,
      CellSize_5,
    };

    enum Align {
      Align_left = 1,
      Align_center,
      Align_right,
    };

private:
    struct Cell {
      kgmGui* gui;
      CellSize width, height;
      Align align;
    };

    kgmList<Cell> _cells;

public:
    kgmGuiContainer();
    kgmGuiContainer(kgmGui *par, u32 x, u32 y, u32 w, u32 h);
    ~kgmGuiContainer();

    void add(kgmGui* g, CellSize cw, Align al);

private:
    void update();
};
