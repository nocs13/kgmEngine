#include "kgmProcess.h"
#include "../kgmBase/kgmLog.h"

#ifdef WIN32
#else
  #include <sys/wait.h>
#endif

kgmProcess::kgmProcess()
{
  m_process = 0;
  m_out.clear();
}

bool kgmProcess::run(kgmString cmd, kgmString args)
{
  bool ret = false;

  #ifdef WIN32

  STARTUPINFO         si;
  PROCESS_INFORMATION pi;

  ZeroMemory(&si, sizeof(si));
  ZeroMemory(&pi, sizeof(pi));

  si.cb = sizeof(si);

  BOOL result = CreateProcess(cmd.data(), args.data(), NULL, NULL, FALSE,
                              0, NULL, NULL, &si, &pi);

  if (result == TRUE)
  {
    m_process = (size_t) pi.hProcess;
    ret = true;
  }
  else
  {
    DWORD error = GetLastError();

    kgm_log() << "Cannot run process, error is " << (u32) error << "\n";
  }

  #else

  int fd[2];

  pipe(fd);

  const char* proc = cmd.data();
  const char* parg[128] = {nullptr};

  kgmString cargs = args;
    
  bool isstr = false;
  char srsym = 0;

  char* p = cargs.data();

  char* a = nullptr;

  int iarg = 0;

  parg[iarg++] = cmd.data();

    while(*p) 
    {
      if (*p == ' ' || *p == '\t') 
      {
        if (!isstr && a)
        {
          parg[iarg++] = a;

          a = nullptr;
        }

        p++;

        continue;
      } 
      else if (*p == '\'' || *p == '"')
      {
        if (isstr && (srsym == *p)) 
        {
          isstr = false;

          *p = '\0';

          parg[iarg++] = a;

          a = nullptr;

          p++;

          continue;
        } 
        else if ((a == nullptr) && !isstr)
        {
          a = p++;

          isstr = true;
        }
      }
      else if (a == nullptr)
      {
        a = p;
      }

      p++;
    }

    if (a)
    {
      parg[iarg++] = a;
    }

    parg[iarg] = nullptr;

  pid_t pid = fork();

  if (pid == 0)
  {
    dup2(fd[1], STDOUT_FILENO);

    close(fd[0]);
    close(fd[1]);

    execv(proc, (char* const*) parg);
  }
  else if (pid > 0)
  {
    int stat = 0;
    int nbytes = 0;

    m_process = pid;

    char *buf = (char*) kgm_alloc(1024);

    m_out.clear();

    close(fd[1]);
    
    while(0 != (nbytes = read(fd[0], buf, 1023))) 
    {
      buf[nbytes] = '\0';
      m_out += buf;
    }

    kgm_free(buf);    

    close(fd[0]);
    
    while(pid != waitpid(pid, &stat, 0))
    {
      
    }

    ret = true;
  }
  else
  {
    ret = false;
  }

  #endif

  return ret;
}

kgmString kgmProcess::out()
{
  return m_out;
}

/*
bool kgmProcess::wait()
{
  if (m_process == 0)
    return false;

  #ifdef WIN32

  WaitForSingleObject((HANDLE) m_process, INFINITE);
  CloseHandle((HANDLE)m_process);

  #else

  s32 status = 0;

  while(::wait(&status) != (int) m_process);

  #endif

  return true;
}
*/