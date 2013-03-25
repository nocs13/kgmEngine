#pragma once
#include "../kgmBase/kgmTab.h"
#include "../kgmBase/kgmString.h"

class kgmGameSettings{
public:
 short  m_gsid;
// int	m_width;
// int	m_height;
// int	m_bpp;
// bool	m_fullscreen;

 kgmString m_name;

 kgmTab<kgmString, kgmString> m_parameters;
public:
 kgmGameSettings();
 kgmGameSettings(const char* path);
 virtual ~kgmGameSettings();

 void load();
 void save();

 kgmString get(char*);
};

