#pragma once
#include "../kgmBase/kgmTab.h"
#include "../kgmBase/kgmXml.h"
#include "../kgmBase/kgmString.h"
#include "../kgmBase/kgmObject.h"
#include "../kgmBase/kgmSettings.h"

class kgmGameSettings: public kgmSettings
{
  kgmXml* m_xml;

public:
 short  m_gsid;
 bool   m_mouse_camera;

 static kgmString m_name;
public:
 kgmGameSettings();
 kgmGameSettings(const char* path);
 virtual ~kgmGameSettings();

 void load();
 void save();

 kgmString get(char*);
 void      set(char*, char*);
};

