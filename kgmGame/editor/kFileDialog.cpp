#include "kFileDialog.h"
#include "../../kgmBase/kgmObject.h"
#include "../../kgmSystem/kgmSystem.h"
#include "../../kgmGraphics/kgmGui.h"
#include <dirent.h>

using namespace kgmGameEditor;

kgmString kFileDialog::DIRCON = "/";

kFileDialog::kFileDialog()
  :kgmGuiFrame(null, 100, 50, 200, 300)
{
  list = new kgmGuiList(this->getClient(), 1, 1, 198, 200);
  text = new kgmGuiText(this->getClient(), 1, 237, 198, 30);
  //text->setChangeEventCallback(kgmGuiText::ChangeEventCallback(this, (kgmGuiText::ChangeEventCallback::Function)&kFileDialog::onEditFile));
  btnCmd = new kgmGuiButton(this->getClient(), 1, 201, 99, 35);
  btnFail = new kgmGuiButton(this->getClient(), 101, 201, 99, 35);

  //list->setSelectCallback(kgmGuiList::SelectEventCallback(this, (kgmGuiButton::ClickEventCallback::Function)&kFileDialog::onFileSelect));

  btnFail->setText("Close");
  slotFail.connect(this, (Slot<kFileDialog, int>::FN) &kFileDialog::onFailSelect, &btnFail->sigClick);
  //btnFail->setClickCallback(kgmGuiButton::ClickEventCallback(this, (kgmGuiButton::ClickEventCallback::Function)&kFileDialog::onFailSelect));

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
  kgmGuiFrame::onMsLeftUp(k, x, y);
}

void kFileDialog::onMsMove(int k, int x, int y)
{
  kgmGuiFrame::onMsMove(k, x, y);
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
      filePath = "";
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

void kFileDialog::onOpenSelect(int)
{
  sigSelect(this);

  close();
}

void kFileDialog::onSaveSelect(int)
{
  sigSelect(this);

  close();
}

void kFileDialog::onFailSelect()
{
  sigFail(this);

  close();
}

void kFileDialog::onEditFile(kgmString s)
{

  filePath = pathFolder + DIRCON + s;
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

void kFileDialog::forOpen(kgmString dir)
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
  slotSelect.connect(this, (Slot<kFileDialog, int>::FN) &kFileDialog::onOpenSelect, &btnCmd->sigClick);

  filePath = "";
  modeSave = false;
  text->setEditable(false);

  show();
  listFolder();
}

void kFileDialog::forSave(kgmString dir)
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
  slotSelect.connect(this, (Slot<kFileDialog, int>::FN) &kFileDialog::onOpenSelect, &btnCmd->sigClick);

  filePath = "";
  modeSave = true;
  text->setEditable(true);

  show();
  listFolder();
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
