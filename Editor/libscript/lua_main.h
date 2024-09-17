#include "../../kgmBase/kgmTypes.h"

namespace Editor
{
  s32 kCreateEditor(void* lh);
  s32 kCloseEditor(void* lh);

  s32 kEditorDelHandler(void* lh);
  s32 kEditorAddHandler(void* lh);

  s32 kgmNewEvent(void *lh);
  s32 kgmEventDelete(void *lh);

  void lua_init(void* lh);
};