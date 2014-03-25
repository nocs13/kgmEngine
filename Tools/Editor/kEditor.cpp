#include "kEditor.h"

kEditor::kEditor()
{
  setMsAbsolute(true);
}

void kEditor::onKeyUp(int k)
{
  kgmGameBase::onKeyUp(k);
}

void kEditor::onKeyDown(int k)
{
  kgmGameBase::onKeyDown(k);
}

void kEditor::onMsLeftUp(int k, int x, int y)
{
  kgmGameBase::onMsLeftUp(k, x, y);
}

void kEditor::onMsLeftDown(int k, int x, int y)
{
  kgmGameBase::onMsLeftDown(k, x, y);
}

void kEditor::onMsRightUp(int k, int x, int y)
{
  kgmGameBase::onMsRightUp(k, x, y);
}

void kEditor::onMsRightDown(int k, int x, int y)
{
  kgmGameBase::onMsRightDown(k, x, y);
}

void kEditor::onMsMove(int k, int x, int y)
{
  kgmGameBase::onMsMove(k, x, y);
}

void kEditor::onMsWheel(int k, int x, int y, int z)
{
  kgmGameBase::onMsWheel(k, x, y, z);
}
