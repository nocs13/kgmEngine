#include "../kgmBase/kgmObject.h"

#include "../kgmGraphics/kgmGui.h"
#include "../kgmGraphics/kgmGuiStyle.h"

#include "../kgmGraphics/kgmMesh.h"
#include "../kgmGraphics/kgmLight.h"
#include "../kgmGraphics/kgmCamera.h"
#include "../kgmGraphics/kgmMaterial.h"
#include "../kgmGraphics/kgmGraphics.h"

#include "kgmActor.h"
#include "kgmEffect.h"

class kgmGameGraphics: public kgmObject
{
  struct VBMesh
  {
    int   id;
    void* vb;
    int v_fmt, v_size, i_size, i_count;
     box3 box;
  };

  struct Mesh
  {
    kgmMesh* mesh;
    kgmMaterial* material;
    box3 box;
  };

  struct Light{
    kgmLight* light;
  };

  class GMesh{
    kgmMaterial* material;
    kgmList<Mesh> meshes;
    kgmOctTree<Mesh*> tree;
  };

  kgmIGraphics* gc;
  kgmIResources* rc;

  kgmFont* font;

  kgmCamera m_camera;

  kgmList<Mesh> m_meshes;
  kgmList<kgmMaterial*> m_materials;
  kgmList<Light> m_lights;
  kgmList<kgmActor*> m_actors;
  kgmList<kgmGui*> m_guis, m_tguis;

  kgmGuiStyle* gui_style;

  void* tdiffuse;
  void* tspecular;
  void* tnormal;
  void* tdepth;
  void* shader;

  kgmTab<u16, kgmShader*> shaders;
  kgmTab<u16, kgmTexture*> textures;

public:
  kgmGameGraphics(kgmIGraphics*, kgmIResources*);
  ~kgmGameGraphics();

  void clean();
  void build();
  void render();
  void resize(float, float);
  void setDefaultFont(kgmFont*);
  void setGuiTheme(kgmString);
  void update();

private:
  void render(Mesh*);
  void render(kgmGui*);
  void render(kgmMesh*);
  void render(kgmActor*);
  void render(kgmShader*, u32);
  void render(kgmMaterial*);
  void gcDrawText(kgmFont*, u32, u32, u32, kgmGui::Rect, kgmString&);
  void gcDrawRect(kgmGui::Rect, u32, void*);

  void trash();
public:
  void add(kgmMesh* mesh, kgmMaterial* material){
    if(mesh)
      mesh->increment();
    if(material)
      material->increment();

    Mesh m;
    m.mesh = mesh;
    m.material = material;
    m.box = mesh->bound();
    m_meshes.add(m);
  }

  void add(kgmMaterial* mtl){
    if(mtl){
      m_materials.add(mtl);
    }
  }

  void add(kgmLight* lgt){
    if(lgt)
      lgt->increment();

    Light light;
    light.light = lgt;
    m_lights.add(light);
  }

  void add(kgmActor* a){
    if(a)
        a->increment();
    m_actors.add(a);
  }

  void add(kgmGui* gui, bool tmp = false){
    if(gui){
      gui->increment();
      if(tmp)
       m_tguis.add(gui);
      else
        m_guis.add(gui);
    }
  }

  void setTo(kgmMesh* mesh, kgmMaterial* mtl){
    if(!mesh || !mtl)
      return;

    for(int i = 0; i < m_meshes.size(); i++){
      if(mesh == m_meshes[i].mesh){
        m_meshes[i].material = mtl;
        break;
      }
    }
  }

  kgmCamera& camera(){
    return m_camera;
  }
};
