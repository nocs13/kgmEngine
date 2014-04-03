#include "kFileDialog.h"
#include "../../kgmSystem/kgmSystem.h"
#include <dirent.h>

kgmString kFileDialog::DIRCON = "/";

kFileDialog::kFileDialog()
  :kgmGui(null, 100, 50, 200, 300)
{
  list = new kgmGuiList(this, 1, 1, 198, 200);
  button = new kgmGuiButton(this, 1, 201, 100, 35);

  list->addListener(this);
  button->addListener(this);

  pathFolder = "";

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
    }
  }
  else if(gui == button)
  {
    hide();
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
  }
}

void kFileDialog::forRead(kgmString dir)
{
  if(dir.length() < 1)
    pathFolder = "/";
  else
    pathFolder = dir;

  button->setText("Open");

  show();
  listFolder();
}
