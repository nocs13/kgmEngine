#include "kFileDialog.h"
#include "../../kgmBase/kgmObject.h"
#include "../../kgmSystem/kgmSystem.h"
#include "../../kgmGraphics/kgmGui.h"
#include <dirent.h>

using namespace kgmGameEditor;

kgmString kFileDialog::DIRCON = "/";

kFileDialog::kFileDialog()
  :kgmGui(null, 100, 50, 200, 300)
{
  list = new kgmGuiList(this, 1, 1, 198, 200);
  text = new kgmGuiText(this, 1, 237, 198, 30);
  btnCmd = new kgmGuiButton(this, 1, 201, 99, 35);
  btnFail = new kgmGuiButton(this, 101, 201, 99, 35);

  btnFail->setText("Close");

  pathFolder = "";

  modeSave = false;
  localable = true;
  allsee = true;

  fnCallback = null;
  fnArg = null;

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

void kFileDialog::onAction(kgmGui *gui, u32 id)
{
  if(gui == list)
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
  else if(gui == btnCmd)
  {
    hide();

    if(modeSave)
    {
      if(fnCallback)
      {
        fnCallback(fnArg);
        fnCallback = null;
        fnArg = null;
      }
      else
      {
        onSave();
      }
    }
    else
    {
      if(fnCallback)
      {
        fnCallback(fnArg);
        fnCallback = null;
        fnArg = null;
      }
      else
      {
        onOpen();
      }
    }
  }
  else if(gui == btnFail)
  {
    hide();

    onFail();
    fnCallback = null;
    fnArg = null;
  }
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

void kFileDialog::forOpen(kgmString dir, ClickEventCallback fn_call, void* arg)
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

  filePath = "";
  modeSave = false;
  text->setEditable(false);

  show();
  listFolder();

  fnCallback = fn_call;
  fnArg = arg;
}

void kFileDialog::forSave(kgmString dir, ClickEventCallback fn_call = null, void* arg = null)
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

  filePath = "";
  modeSave = true;
  text->setEditable(true);

  show();
  listFolder();

  fnCallback = fn_call;
  fnArg = arg;
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
