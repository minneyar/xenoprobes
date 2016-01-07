//
// Created by preed on 1/7/16.
//

#ifndef XENOPROBES_SEMAPHORE_H
#define XENOPROBES_SEMAPHORE_H


#include <mutex>
#include <condition_variable>

class semaphore {
public:
    using native_handle_type = std::condition_variable::native_handle_type;

    explicit semaphore(size_t n = 0);
    semaphore(const semaphore&) = delete;
    semaphore& operator=(const semaphore&) = delete;

    void notify();
    void wait();
    bool try_wait();
    template<class Rep, class Period>
    bool wait_for(const std::chrono::duration<Rep, Period>& d);
    template<class Clock, class Duration>
    bool wait_until(const std::chrono::time_point<Clock, Duration>& t);

    native_handle_type native_handle();

private:
    size_t                  count;
    std::mutex              mutex;
    std::condition_variable cv;
};


#endif //XENOPROBES_SEMAPHORE_H
