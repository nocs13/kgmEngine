#include "kgmGui.h"

class kgmGuiContainer: public kgmGui {
    KGM_OBJECT(kgmGuiContainer);
public:
    kgmGuiContainer();
    kgmGuiContainer(kgmGui *par, u32 x, u32 y, u32 w, u32 h);
    ~kgmGuiContainer();
};
