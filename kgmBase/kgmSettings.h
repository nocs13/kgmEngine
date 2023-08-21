#pragma once

#include "kgmTab.h"
#include "kgmXml.h"
#include "kgmString.h"
#include "kgmObject.h"
#include "kgmISettings.h"

class kgmSettings: public kgmObject, public kgmISettings
{
  KGM_OBJECT(kgmSettings);

  kgmXml* m_xml;
  kgmString m_name;

public:
 short  m_gsid;
 bool   m_mouse_camera;

public:
 kgmSettings();
 kgmSettings(const char* path);
 virtual ~kgmSettings();

 void load();
 void save();

 kgmString get(char*);
 void      set(char*, char*);
};

