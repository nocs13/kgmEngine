#pragma once
#include "../kgmBase/kgmTab.h"
#include "../kgmBase/kgmString.h"
#include "../kgmBase/kgmEnvironment.h"

class kgmGameSettings: public kgmEnvironment
{
public:
 short  m_gsid;
 bool   m_mouse_camera;

 kgmString m_name;

 kgmTab<kgmString, kgmString> m_parameters;
public:
 kgmGameSettings();
 kgmGameSettings(const char* path);
 virtual ~kgmGameSettings();

 void load();
 void save();

 kgmString get(char*);
 void      set(char*, char*);
};

