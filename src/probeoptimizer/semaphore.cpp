//
// Created by preed on 1/7/16.
//

#include "probeoptimizer/semaphore.h"


semaphore::semaphore(size_t n) : count{n} {}

void semaphore::notify() {
  std::lock_guard<std::mutex> lock{mutex};
  ++count;
  cv.notify_one();
}

void semaphore::wait() {
  std::unique_lock<std::mutex> lock{mutex};
  cv.wait(lock, [&]{ return count > 0; });
  --count;
}

bool semaphore::try_wait() {
  std::lock_guard<std::mutex> lock{mutex};

  if (count > 0) {
    --count;
    return true;
  }

  return false;
}

template<class Rep, class Period>
bool semaphore::wait_for(const std::chrono::duration<Rep, Period>& d) {
  std::unique_lock<std::mutex> lock{mutex};
  auto finished = cv.wait_for(lock, d, [&]{ return count > 0; });

  if (finished)
    --count;

  return finished;
}

template<class Clock, class Duration>
bool semaphore::wait_until(const std::chrono::time_point<Clock, Duration>& t) {
  std::unique_lock<std::mutex> lock{mutex};
  auto finished = cv.wait_until(lock, t, [&]{ return count > 0; });

  if (finished)
    --count;

  return finished;
}

semaphore::native_handle_type semaphore::native_handle() {
  return cv.native_handle();
}
