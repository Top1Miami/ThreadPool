//
// Created by Dima on 05.10.2017.
//

#ifndef SETTINGUPGOOGLETEST_SHARED_STATE_H
#define SETTINGUPGOOGLETEST_SHARED_STATE_H

#include <mutex>
#include <condition_variable>
#include <atomic>
#include <exception>
#include <stdexcept>
#include "ThreadPool.h"

template<typename R>
class Shared_state {
public:
    std::condition_variable sh_cv;
    std::mutex sh_mtx;
    std::atomic<bool> sh_done;
    R data;
    std::exception_ptr sh_ex_ptr;
    std::atomic<bool> promise_al;
    ThreadPool *pool = nullptr;

    Shared_state() {
        sh_done = false;
        //promise_al = true;
    }
};

template<typename R>
class Shared_state<R &> {
public:
    std::condition_variable sh_cv;
    std::mutex sh_mtx;
    std::atomic<bool> sh_done;
    R *data;
    std::exception_ptr sh_ex_ptr;
    std::atomic<bool> promise_al;
    ThreadPool *pool = nullptr;

    Shared_state() {
        sh_done = false;
        promise_al = true;
    }
};

template<>
class Shared_state<void> {
public:
    std::condition_variable sh_cv;
    std::mutex sh_mtx;
    std::atomic<bool> sh_done;
    std::exception_ptr sh_ex_ptr;
    std::atomic<bool> promise_al;
    ThreadPool *pool = nullptr;

    Shared_state() {
        sh_done = false;
        promise_al = true;
    }
};

#endif //SETTINGUPGOOGLETEST_SHARED_STATE_H
