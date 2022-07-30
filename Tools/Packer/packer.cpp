#include <stdio.h>
#include "../../kgmBase/kgmArchive.h"

void usage()
{
  printf("packer [cmd] args\n");
  printf("packer list arch_file.   list data from archive file\n");
  printf("packer create arch_file.   create empty archive file\n");
  printf("packer remove arch_file file1, file2, ...  remove file from archive file\n");
  printf("packer append arch_file file1, file2, ...  append file to archive file\n");
  printf("packer extract arch_file path  extract archive in path\n");
}

int main(int argc, char **argv)
{
  kgmArchive arch;

  if(argc < 3)
  {
    usage();

    return 1;
  }

  if(!strncmp(argv[1], "list", 6))
  {
    if(!arch.open(argv[2]))
      return 1;

    for(int i = 0; i < arch.count(); i++)
    {
      kgmString file;
      u32       size, offset;
      arch.info(i, file, size, offset);

      printf("%s size: %i offset: %x\n", (char*)file, size, offset);
    }
  }
  else if(!strncmp(argv[1], "create", 6))
  {
    if(!arch.create(argv[2]))
      return 1;
  }
  else if(!strncmp(argv[1], "remove", 6))
  {
    if(!arch.open(argv[2]))
      return 1;

    for(int i = 3; i < argc; i++)
    {
      arch.erase(argv[i]);
    }
  }
  else if(!strncmp(argv[1], "append", 6))
  {
    if(!arch.open(argv[2]))
      return 1;

    for(int i = 3; i < argc; i++)
    {
      arch.append(argv[i]);
    }
  }
  else if(argc == 4 && !strncmp(argv[1], "extract", 7))
  {
    if(!arch.open(argv[2]))
      return 1;

    for(int i = 0; i < arch.count(); i++)
    {
      kgmString     fname;
      kgmArray<u8> data;
      u32           size, offset;

      arch.info(i, fname, size, offset);
      arch.copy((u32)i, data);

      if(fname.length() > 0)
      {
        kgmFile file;
        kgmString path = kgmString(argv[3]) + "/" + fname;

        if(file.open(path, kgmFile::Create | kgmFile::Write))
        {
          file.write(data.data(), data.length());
          file.close();
        }
      }
    }
  }
  else
  {
    usage();

    return 1;
  }

  return 0;
}
