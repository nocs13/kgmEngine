#pragma once
#include "../kgmMain/kgmString.h"

class kgmGameSettings{
public:
 short  m_gsid;
 int	m_width;
 int	m_height;
 int	m_bpp;
 bool	m_fullscreen;

 kgmString m_name;
public:
 kgmGameSettings();
 virtual ~kgmGameSettings();

 void load();
 void save();
};

