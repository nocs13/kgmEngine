#include <stdio.h>
#include "../../kgmBase/kgmArchive.h"

void usage()
{
  printf("packer [cmd] args\n");
  printf("packer create arch_file.   create empty archive file\n");
  printf("packer remove arch_file file1, file2, ...  remove file from archive file\n");
  printf("packer append arch_file file1, file2, ...  append file to archive file\n");
}

int main(int argc, char **argv)
{
  kgmArchive arch;

  if(argc < 3)
  {
    usage();

    return 1;
  }

  if(!strncmp(argv[1], "create", 6))
  {
    if(!arch.create(argv[2]))
      return 1;

    arch.close();
  }
  else if(!strncmp(argv[1], "remove", 6))
  {
    if(!arch.open(argv[2]))
      return 1;

    for(int i = 3; i < argc; i++)
    {
      arch.erase(argv[i]);
    }

    arch.close();
  }
  else if(!strncmp(argv[1], "append", 6))
  {
    if(!arch.open(argv[2]))
      return 1;

    for(int i = 3; i < argc; i++)
    {
      arch.append(argv[i]);
    }

    arch.close();
  }
  else
  {
    usage();

    return 1;
  }

  return 0;
}
