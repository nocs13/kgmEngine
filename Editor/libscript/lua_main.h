#include "../../kgmBase/kgmTypes.h"

namespace Editor
{
  s32 kCreateEditor(void* lh);
  s32 kCloseEditor(void* lh);

  s32 kEditorDelHandler(void* lh);
  s32 kEditorAddHandler(void* lh);

  s32 kEditorGuiAdd(void* lh);

  void lua_init(void* lh);
};