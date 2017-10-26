#pragma once

class kgmInterface
{
public:
  virtual unsigned int vSize()  = 0;
  virtual const char*  vClass() = 0;

  virtual void         clear() {}
};
