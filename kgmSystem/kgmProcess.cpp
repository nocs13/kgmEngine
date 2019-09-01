#include "kgmProcess.h"
#include "../kgmBase/kgmLog.h"

kgmProcess::kgmProcess()
{
  m_process = 0;
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

  pid_t pid = fork();
  if (pid == 0)
  {
    exit(exec(cmd.data(), args.data()));
  }
  else if (pid > 0)
  {
    m_process = pid;

    ret = true;
  }
  else
  {
    ret = false;
  }

  #endif

  return ret;
}

bool kgmProcess::wait()
{
  if (m_process == 0)
    return false;

  #ifdef WIN32

  WaitForSingleObject((HANDLE) m_process, INFINITE);
  CloseHandle((HANDLE)m_process);

  #else

  s32 status = 0;
  waitpid((pid_t) m_process, &status, NULL);

  #endif

  return true;
}
