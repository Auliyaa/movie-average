#pragma once

#include <atomic>

struct spin_lock
{
  std::atomic_flag flag = ATOMIC_FLAG_INIT;

  void lock()
  {
    while (flag.test_and_set(std::memory_order_acquire))
    {
      // Provides a hint to the implementation to reschedule the execution of threads, allowing other threads to run.
      std::this_thread::yield();
    }
  }

  void unlock()
  {
    flag.clear(std::memory_order_release);
  }
};
