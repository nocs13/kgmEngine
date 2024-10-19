#include "kgmGuiFileDialog.h"
#include "../kgmBase/kgmObject.h"
#include "../kgmSystem/kgmSystem.h"
#include "kgmGui.h"
#include <dirent.h>

// This in Singleton type object class.
// So need to protect to made more objects.

kgmString kgmGuiFileDialog::DIRCON = "/";

kgmGuiFileDialog* kgmGuiFileDialog::single = null;

kgmGuiFileDialog::kgmGuiFileDialog()
  :kgmGuiFrame(null, 100, 50, 200, 300)
{
  Rect rc;
  this->getClient()->getRect(rc);

  list   = new kgmGuiList(this->getClient(),   1, 1, rc.width() - 2, rc.height() - 77);
  text   = new kgmGuiText(this->getClient(),   1, rc.height() - 31, rc.width() - 2, 30);
  btnCmd = new kgmGuiButton(this->getClient(), 1, rc.height() - 75, rc.width() - 2, 43);

  kgmSystem::getCurrentDirectory(pathFolder);

  modeSave = false;
  localable = true;
  allsee = true;

  slotListSelect.connect(this, (Slot<kgmGuiFileDialog, u32>::FN) &kgmGuiFileDialog::onListChange, &list->sigSelect);

#ifdef WIN32
  DIRCON = "\\";
#endif

  m_text = m_sid = "guiFileDialog";
}

kgmGuiFileDialog::~kgmGuiFileDialog()
{
  kgmGuiFileDialog::single = null;
}

void kgmGuiFileDialog::onMsLeftUp(int k, int x, int y)
{
  kgmGuiFrame::onMsLeftUp(k, x, y);
}

void kgmGuiFileDialog::onMsMove(int k, int x, int y)
{
  kgmGuiFrame::onMsMove(k, x, y);
}

void kgmGuiFileDialog::onFileSelect()
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

void kgmGuiFileDialog::onOpenSelect(int i)
{
  sigSelect(this);

  onOpen();

  close();
}

void kgmGuiFileDialog::onSaveSelect(int i)
{
  sigSelect(this);

  onSave();

  close();
}

void kgmGuiFileDialog::onFailSelect()
{
  sigFail(this);

  onFail();

  close();
}

void kgmGuiFileDialog::onListChange(u32 i)
{
  text->setText(list->getItem(i));
}

void kgmGuiFileDialog::onEditFile(kgmString s)
{

  filePath = pathFolder + DIRCON + s;
}

void kgmGuiFileDialog::listFolder()
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
        if(filter.length() <= (s32) strlen(ent->d_name))
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

void kgmGuiFileDialog::forOpen(kgmString dir)
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
  slotSelect.connect(this, (Slot<kgmGuiFileDialog, int>::FN) &kgmGuiFileDialog::onOpenSelect, &btnCmd->sigClick);

  filePath = "";
  modeSave = false;
  text->setEditable(false);

  show();
  listFolder();
}

void kgmGuiFileDialog::forSave(kgmString dir)
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
  slotSelect.connect(this, (Slot<kgmGuiFileDialog, int>::FN) &kgmGuiFileDialog::onOpenSelect, &btnCmd->sigClick);

  filePath = "";
  modeSave = true;
  text->setEditable(true);

  show();
  listFolder();
}

void kgmGuiFileDialog::setFilter(kgmString flt)
{
  filter = flt;
}

kgmString kgmGuiFileDialog::getFile()
{
  return text->getText();
}

kgmString kgmGuiFileDialog::getPath()
{
  if(!filePath.length())
    filePath = getFolder() + DIRCON + getFile();

  return filePath;
}

kgmString kgmGuiFileDialog::getFolder()
{
  return pathFolder;
}

void kgmGuiFileDialog::changeLocation(bool s)
{
  localable = s;
}

void kgmGuiFileDialog::showHidden(bool s)
{
  allsee = s;
}

kgmGuiFileDialog* kgmGuiFileDialog::getDialog()
{
  if (kgmGuiFileDialog::single)
    return null;

  return (kgmGuiFileDialog::single = new kgmGuiFileDialog());
}
