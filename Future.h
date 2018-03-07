//
// Created by Dima on nullptr5.1nullptr.2nullptr17.
//

#ifndef SETTINGUPGOOGLETEST_FUTURE_H
#define SETTINGUPGOOGLETEST_FUTURE_H

#include <iostream>
#include "Shared_state.h"
#include "ThreadPool.h"

template<typename R>
class Promise;

template<typename R>
class Promise<R &>;

template<>
class Promise<void>;

template<typename R>
class Future {
private :
    friend class Promise<R>;

    std::shared_ptr<Shared_state<R>> st_ptr = nullptr;
    //bool obtained = false;

    explicit Future(std::shared_ptr<Shared_state<R>> ptr) {
        st_ptr = ptr;
    }

public :

    Future(Future const &) = delete;

    Future(Future &&ptr) {
        st_ptr = std::move(ptr.st_ptr);
    }

    Future &operator=(Future &&ptr) {
        if (this != &ptr) {
            st_ptr = std::move(ptr.st_ptr);
        }
        return *this;
    }

    Future &operator=(Future const &) = delete;

    Future() = default;

    R get() const {
        wait();
        /*
        if (st_ptr == nullptr){
            throw std::runtime_error("Promise wasn't created");
        }*/
        if (st_ptr->sh_ex_ptr) {
            std::rethrow_exception(st_ptr->sh_ex_ptr);
        }/*
        if (obtained) {
            throw std::runtime_error("Get was already called");
        }
        obtained = true;*/
        return std::move(st_ptr->data);
    }

    bool isReady() const {
        return st_ptr->sh_done;
    }

    void wait() const {
        /*
        if (st_ptr == nullptr){
            throw std::runtime_error("Promise wasn't created");
        }*/
        std::unique_lock<std::mutex> lock(st_ptr->sh_mtx);
        if (isReady()) {
            return;
        }
        while (!st_ptr->sh_done && st_ptr->promise_al) {
            st_ptr->sh_cv.wait(lock);
        }

        if (!st_ptr->promise_al && !st_ptr->sh_done) {
            throw std::runtime_error("Value wasn't set");
        }
    }

    ThreadPool *getPool() {
        return st_ptr->pool;
    }
};

template<typename R>
class Future<R &> {
private:
    friend class Promise<R &>;

    std::shared_ptr<Shared_state<R &>> st_ptr = nullptr;
    //bool obtained = false;

    explicit Future(std::shared_ptr<Shared_state<R &>> ptr) {
        st_ptr = ptr;
    }

public:

    Future(Future &&ptr) {
        st_ptr = std::move(ptr.st_ptr);
    }

    Future &operator=(Future &&ptr) {
        if (this != &ptr) {
            st_ptr = std::move(ptr.st_ptr);
        }
        return *this;
    }

    Future(Future const &) = delete;

    Future &operator=(Future const &) = delete;

    Future() = default;

    R &get() {
        /*
        if (st_ptr == nullptr){
            throw std::runtime_error("Promise wasn't created");
        }*/
        if (st_ptr->sh_ex_ptr) {
            std::rethrow_exception(st_ptr->sh_ex_ptr);
        }
        /*if (obtained) {
            throw std::runtime_error("Get was already called");
        }
        obtained = true;*/
        wait();
        return *st_ptr->data;
    }

    bool isReady() const {
        return st_ptr->sh_done;
    }

    void wait() const {
        /*
        if (st_ptr == nullptr){
            throw std::runtime_error("Promise wasn't created");
        }*/
        std::unique_lock<std::mutex> lock(st_ptr->sh_mtx);
        if (isReady()) {
            return;
        }

        while (!st_ptr->sh_done && st_ptr->promise_al) {
            st_ptr->sh_cv.wait(lock);
        }

        if (!st_ptr->promise_al && !st_ptr->sh_done) {
            throw std::runtime_error("Value wasn't set");
        }
    }

    ThreadPool *getPool() {
        return st_ptr->pool;
    }
};


template<>
class Future<void> {
private:
    friend class Promise<void>;

    std::shared_ptr<Shared_state<void>> st_ptr = nullptr;

    //bool obtained = false;
    explicit Future(std::shared_ptr<Shared_state<void>> ptr) {
        st_ptr = ptr;
    }

public:
    Future(Future &&ptr) {
        st_ptr = std::move(ptr.st_ptr);
    }

    Future &operator=(Future &&ptr) {
        if (this != &ptr) {
            st_ptr = std::move(ptr.st_ptr);
        }
        return *this;
    }

    Future(Future const &) = delete;

    Future &operator=(Future const &) = delete;

    Future() = default;

    void get() {
        /*if (st_ptr == nullptr){
            throw std::runtime_error("Promise wasn't created");
        }*/
        if (st_ptr->sh_ex_ptr) {
            std::rethrow_exception(st_ptr->sh_ex_ptr);
        }/*
        if (obtained) {
            throw std::runtime_error("Get was already called");
        }
        obtained = true;*/
        wait();
    }

    bool isReady() const {
        return st_ptr->sh_done;
    }

    void wait() const {
        /*if (st_ptr == nullptr){
            throw std::runtime_error("Promise wasn't created");
        }*/
        std::unique_lock<std::mutex> lock(st_ptr->sh_mtx);
        if (isReady()) {
            return;
        }

        while (!st_ptr->sh_done && st_ptr->promise_al) {
            st_ptr->sh_cv.wait(lock);
        }

        if (!st_ptr->promise_al && !st_ptr->sh_done) {
            throw std::runtime_error("Value wasn't set");
        }
    }

    ThreadPool *getPool() {
        return st_ptr->pool;
    }
};

#endif //SETTINGUPGOOGLETEST_FUTURE_H
