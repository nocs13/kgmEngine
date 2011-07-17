#pragma once

#include "kgmIGame.h"
//#include "kgmActor.h"
#include "../kgmGraphics/kgmMesh.h"
#include "../kgmGraphics/kgmLight.h"
#include "../kgmGraphics/kgmCamera.h"
//#include "../kgmScript/kgmScript.h"

class kgmGameLogic{
 public:
  class Sensor;
  class Controller;
  class Actuator;

  enum SensorType{
    SensorNone,
    SensorInput,
    SensorCollision,
  };

  enum ControllerType{
    ControllerNo,
    ControllerAll,
    ControllerAnd,
    ControllerOr,
    ControllerXor,
  };

  enum ActuatorType{
    ActuatorNone,
    ActuatorMotion,
  };

  class Actuator{
  public:
    ActuatorType type;
    Actuator(){
    }
    ~Actuator(){
    }
    virtual bool action(kgmObject* o, int x = 0){
      return false;
    }
  };

  class Controller{
  public:
    ControllerType type;
    kgmList<Actuator*> acts;
    int ctl;
    Controller(ControllerType t, int c = 1){
      type = t;
      ctl = c;
    }
    ~Controller(){
    }
    void add(Actuator* a){
      if(a)
	acts.add(a);
    }
    virtual bool control(int k, kgmObject* obj){
      int r = 0;
      switch(type){
      case ControllerNo:
	r = 0;
	break;
      case ControllerAll:
	r = 1;
	break;
      case ControllerAnd:
	r = k & ctl;
	break;
      case ControllerOr:
	r = k | ctl;
	break;
      case ControllerXor:
	r = k ^ ctl;
	break;
      default:
	return false;
      }
      if(!r)
	return false;
      for(int i = 0; i < acts.size(); i++)
	acts[i]->action(obj, k);
      return true;
    }
  };

  class ActuatorGame:public Actuator{
    enum{Quit, LoadMap, AddScene, };
  };

  class ActuatorMotion:public Actuator{
    enum{Normal, Fly, Script, };
  };

  class ActuatorSound:public Actuator{
    enum{Play, Stop, };
  };

  class Sensor{
  public:
    SensorType type;
    kgmList<kgmGameLogic::Controller*> ctls;
    kgmObject*                         obj;

    Sensor(kgmObject* o){
      obj = o;
      if(obj) obj->increment();
    }
    ~Sensor(){
      if(obj) obj->release();
      for(int i = 0; i < ctls.size(); i++)
	delete ctls[i];
      ctls.clear();
    }
    virtual bool sense(void* a){}
  };

  class SensorInput:public Sensor{
  public:
    bool sense(void* p){
      char* c = (char*)p;
      if(!p)
	return false;
      for(int i = 0; i < ctls.size(); i++)
	ctls[i]->control(c[0], obj);
      return true;
    }
  };

  class SensorMessage:public Sensor{
  public:
  };

  class SensorCollision:public Sensor{
  };

  class SensorRadar:public Sensor{
  };

  class SensorDelay:public Sensor{
  };


protected:
  kgmList<Sensor*> m_sensors;
public:
  kgmGameLogic();
  ~kgmGameLogic();

  void clear(){
    for(int i = 0; i < m_sensors.size(); i++)
	delete m_sensors[i];
    m_sensors.clear();
  }
  void add(Sensor* s){ 
    if(s) m_sensors.add(s); 
  }

  virtual bool sense(SensorType type, void* x){ 
    for(int i = 0; i < m_sensors.size(); i++)
      if(m_sensors[i]->type == type)
    	  m_sensors[i]->sense(x);
    return true;
  }
};
