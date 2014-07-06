#include "kFileDialog.h"
#include "../../kgmBase/kgmObject.h"
#include "../../kgmSystem/kgmSystem.h"
#include "../../kgmGraphics/kgmGui.h"
#include <dirent.h>

using namespace kgmGameEditor;

kgmString kFileDialog::DIRCON = "/";

kFileDialog::kFileDialog()
  :kgmGui(null, 100, 50, 200, 300), callback(null, null), fail_callback(null, null)
{
  list = new kgmGuiList(this, 1, 1, 198, 200);
  text = new kgmGuiText(this, 1, 237, 198, 30);
  btnCmd = new kgmGuiButton(this, 1, 201, 99, 35);
  btnFail = new kgmGuiButton(this, 101, 201, 99, 35);

  list->setSelectCallback(kgmGuiList::SelectEventCallback(this, (kgmGuiButton::ClickEventCallback::Function)&kFileDialog::onFileSelect));

  btnFail->setText("Close");
  btnFail->setClickCallback(kgmGuiButton::ClickEventCallback(this, (kgmGuiButton::ClickEventCallback::Function)&kFileDialog::onFailSelect));

  pathFolder = "";

  modeSave = false;
  localable = true;
  allsee = true;

#ifdef WIN32
  DIRCON = "\\";
#endif
}

void kFileDialog::onMsLeftUp(int k, int x, int y)
{
  kgmGui::onMsLeftUp(k, x, y);
}

void kFileDialog::onMsMove(int k, int x, int y)
{
  kgmGui::onMsMove(k, x, y);
}

void kFileDialog::onFileSelect()
{
  int i = list->getSel();

  kgmString item = list->getItem(i);

  if(item == ".")
  {

  }

  if(item == "..")
  {
    kgmString tmp = pathFolder;

    if(tmp != "/")
    {
      char* a = null;

      #ifdef WIN32
      a = strrchr(tmp.data(), '\\');
      #else
      a = strrchr(tmp.data(), '/');
      #endif

      if(a)
      {
        tmp = kgmString(tmp.data(), (int)(a - tmp.data()));
      }

      pathFolder = tmp;

      listFolder();
    }
  }
  else
  {
    kgmString tmp = pathFolder + DIRCON + kgmString(item);

    if(kgmSystem::isDirectory(tmp))
    {
      pathFolder = tmp;
      listFolder();
    }
    else
    {
      text->setText(item);
      text->dropCursor();

      filePath = tmp;
    }
  }
}

void kFileDialog::onOpenSelect()
{
  hide();

  if(callback.hasObject() && callback.hasFunction())
    callback(callback.getObject(), getPath());
}

void kFileDialog::onSaveSelect()
{
  hide();

  if(callback.hasObject() && callback.hasFunction())
    callback(callback.getObject(), getPath());
}

void kFileDialog::onFailSelect()
{
  hide();

  if(fail_callback.hasObject() && fail_callback.hasFunction())
    fail_callback(fail_callback.getObject());
}

void kFileDialog::setFailCallback(kgmGuiButton::ClickEventCallback call)
{
  fail_callback = call;
}

void kFileDialog::listFolder()
{
  if(pathFolder.length() < 1)
    return;

  list->clear();

  DIR *folder = null;
  struct dirent *ent = null;

  if((folder = opendir (pathFolder.data())) != NULL)
  {
    while((ent = readdir (folder)) != NULL)
    {
      if(strlen(ent->d_name) == 1 && strncmp(ent->d_name, ".", 1) == 0)
        continue;

      if(!localable)
      {
        if(strlen(ent->d_name) == 2 && strncmp(ent->d_name, "..", 2) == 0)
          continue;
      }

      if(!allsee)
      {
        if(strlen(ent->d_name) > 1 && ent->d_name[0] == '.')
        {
          if(strcmp(ent->d_name, "..") != 0)
            continue;
        }
      }

      if(filter.length() > 0)
      {
        if(filter.length() <= strlen(ent->d_name))
        {
          if(memcmp(filter.data(),
                    ent->d_name + strlen(ent->d_name) - filter.length(),
                    filter.length()) == 0)
          {
            list->addItem(ent->d_name);
          }
        }
      }
      else
      {
        list->addItem(ent->d_name);
      }
    }

    closedir (folder);
    list->sort();
    list->setSel(0);
  }
}

void kFileDialog::forOpen(kgmString dir, ClickEventCallback fn_call)
{
  if(dir.length() < 1)
  {
    if(!pathFolder.length())
      pathFolder = "/";
  }
  else
  {
    pathFolder = dir;
  }

  btnCmd->setText("Open");
  btnCmd->setClickCallback(kgmGuiButton::ClickEventCallback(this, (kgmGuiButton::ClickEventCallback::Function)&kFileDialog::onOpenSelect));

  filePath = "";
  modeSave = false;
  text->setEditable(false);

  show();
  listFolder();

  callback = fn_call;
}

void kFileDialog::forSave(kgmString dir, ClickEventCallback fn_call)
{
  if(dir.length() < 1)
  {
    if(!pathFolder.length())
      pathFolder = "/";
  }
  else
  {
    pathFolder = dir;
  }

  btnCmd->setText("Save");
  btnCmd->setClickCallback(kgmGuiButton::ClickEventCallback(this, (kgmGuiButton::ClickEventCallback::Function)&kFileDialog::onSaveSelect));

  filePath = "";
  modeSave = true;
  text->setEditable(true);

  show();
  listFolder();

  callback = fn_call;
}

void kFileDialog::setFilter(kgmString flt)
{
  filter = flt;
}

kgmString kFileDialog::getFile()
{
  return text->m_text;
}

kgmString kFileDialog::getPath()
{
  if(!filePath.length())
    filePath = getFolder() + DIRCON + getFile();

  return filePath;
}

kgmString kFileDialog::getFolder()
{
  return pathFolder;
}

void kFileDialog::changeLocation(bool s)
{
  localable = s;
}

void kFileDialog::showHidden(bool s)
{
  allsee = s;
}
