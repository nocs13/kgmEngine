#pragma once
#include "kgmObject.h"
#include "kgmString.h"

class kgmResource: public kgmObject{
 KGM_OBJECT(kgmResource)
public:
 enum  Type{
  TypeNone,
  TypeFont,	
  TypeTexture,
  TypeShader,
  TypeMesh,
  TypeSound,
  TypeAnimation,
  TypeSkeleton,
 };
public:
 kgmString  m_id;
 uint	    m_uid;
 uint       m_type;

protected:
 ~kgmResource();
public:
 kgmResource();
 void setId(kgmString id){ m_id = id; }
};

