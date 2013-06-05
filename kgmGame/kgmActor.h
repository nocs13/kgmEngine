#pragma once
#include "../kgmBase/kgmTab.h"
#include "../kgmBase/kgmTime.h"

#include "../kgmGraphics/kgmVisual.h"
#include "../kgmGraphics/kgmMesh.h"
#include "../kgmGraphics/kgmPolygon.h"
#include "../kgmGraphics/kgmTexture.h"
#include "../kgmGraphics/kgmShader.h"
#include "../kgmGraphics/kgmMaterial.h"

#include "../kgmGraphics/kgmAnimation.h"
#include "../kgmGraphics/kgmDummy.h"
#include "../kgmGraphics/kgmCamera.h"

#include "../kgmMedia/kgmSound.h"

#include "../kgmPhysics/kgmBody.h"

#include "kgmGameObject.h"
#include "kgmGamePhysics.h"

class kgmActor: public kgmGameObject
{
    KGM_OBJECT(kgmActor)
public:
    class State
    {
    public:
        kgmString id;
        kgmString type;
        kgmString switchto;

        u32       priopity;
        u32       time;

        kgmSound*     sound;
        kgmAnimation* animation;
        u32           fstart, fend;

        kgmTab<kgmString, kgmString> options;
    };

    struct Animation
    {
        kgmAnimation* animation;
        u32           start;
        u32           end;
    };

public:
    u32     m_uid;
    u32     m_type;
    u32     m_bearing;

    u32     m_group;
    u32     m_birth;
    u32     m_health;
    u32     m_attack;
    u32     m_defence;
    u32     m_evasion;
    u32     m_accuracy;
    u32     m_timeout;

    mtx4    m_transform;
    mtx4    m_dvisual;      //visual transform

    kgmString m_id;
    kgmString m_name;
    kgmString m_class;

    kgmList<State*>     m_states;
    kgmList<kgmDummy*>  m_dummies;

    kgmAnimation*       m_animation;
    kgmSkeleton*        m_skeleton;
    State*              m_state;
    bool                m_gameplayer;
public:
    kgmActor();
    virtual ~kgmActor();

    virtual void init();
    virtual void exit();
    virtual void update(u32);
    virtual void input(u32, int);

    void remove();

    void enable(bool s);
    void active(bool a);
    void visible(bool a);

    bool enabled();
    bool removed();

    void setPosition(vec3& v);
    void setRotation(vec3& r);
    void setRotation(quat& r);
    void setDirection(vec3& d);

    void setParent(kgmActor* a);
    void addChild(kgmActor* a);
    void delChild(kgmActor* a);

    void add(kgmDummy* m)
    {
        if(m)
            m_dummies.add(m);
    }

    void setAnimation(kgmAnimation* a)
    {
      if(a)
      {
        m_animation = a;
        m_visual->setAnimation(a);
      }
    }

    bool setState(kgmString s, bool force = false);
};

typedef kgmList<kgmActor*> kgmActors;
typedef kgmList<kgmActor*> kgmActorList;
