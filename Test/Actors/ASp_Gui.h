#ifndef ASP_GUI_H
#define ASP_GUI_H

class ASp_Gui: public kgmGui
{
  KGM_OBJECT(ASp_Gui);

  int health;

public:
  ASp_Gui()
  :kgmGui()
  {

  }
};

#endif // ASP_GUI_H
