#include "kgmTimer.h"

kgmTimer::Thread::Thread(kgmTimer* t)
{
  timer = t;

  valid = false;
}

void kgmTimer::Thread::stop()
{
  valid = false;
}

void kgmTimer::Thread::run()
{
  valid = true;

  while(valid)
  {
    lock();

    for(auto i = timer->list.begin(); !i.end(); i.next())
    {
      kgmTimer::Chunk c = (*i);

      if ((timer->time.getTicks() - c.start) >= c.delay)
      {
        c.fn(c.ob);

        c.start = timer->time.getTicks();
      }
    }

    unlock();

    kgmThread::sleep(0);
  }
}

kgmTimer::kgmTimer()
{
  thread = new Thread(this);

  time.update();
}

kgmTimer::~kgmTimer()
{
  if (thread)
  {
    thread->stop();
    thread->join();

    thread = null;
  }

  list.clear();
}

bool kgmTimer::add(Function fn, void *ob, u32 dy)
{
  if (!fn)
    return false;

  Chunk c;

  c.fn = fn;
  c.ob = ob;
  c.delay = dy;
  c.start = time.getTicks();

  if (list.size() < 1)
    thread->start();

  thread->lock();
  list.add(c);
  thread->unlock();

  return true;
}

void kgmTimer::remove(Function fn)
{
  for(auto i = list.begin(); !i.end(); i.next())
  {
    if ((*i).fn == fn)
    {
      thread->lock();
      list.erase(i);
      thread->unlock();

      break;
    }
  }
}
