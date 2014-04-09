#include "kFileDialog.h"
#include "../../kgmSystem/kgmSystem.h"
#include <dirent.h>

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
        char* a = strrchr(tmp.data(), '/');

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
      onSave();
    else
      onOpen();
  }
  else if(gui == btnFail)
  {
    hide();

    onFail();
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
      list->addItem(ent->d_name);
    }

    closedir (folder);
    list->sort();
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

  filePath = "";
  modeSave = true;
  text->setEditable(true);

  show();
  listFolder();
}

kgmString kFileDialog::getPath()
{
  return filePath;
}

kgmString kFileDialog::getFolder()
{
  return pathFolder;
}
