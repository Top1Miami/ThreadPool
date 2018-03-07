//
// Created by Dima on 05.10.2017.
//

#ifndef SETTINGUPGOOGLETEST_PROMISE_H
#define SETTINGUPGOOGLETEST_PROMISE_H

#include "Shared_state.h"
#include "Future.h"


template<typename R>
class Promise {
private:
    std::shared_ptr<Shared_state<R>> st_ptr;
    //bool obtained = false;
public :
    Promise(Promise &&ptr) {
        st_ptr = std::move(ptr.st_ptr);
        st_ptr->promise_al = true;
    }

    Promise &operator=(Promise &&ptr) {
        if (this != &ptr) {
            st_ptr = std::move(ptr.st_ptr);
        }
        st_ptr->promise_al = true;
        return *this;
    }

    Promise(Promise const &) = delete;

    Promise &operator=(Promise const &) = delete;

    Promise() {
        st_ptr = std::make_shared<Shared_state<R>>();
        st_ptr->promise_al = true;
    }

    Future<R> getFuture() {
        std::unique_lock<std::mutex> lock(st_ptr->sh_mtx);
        /*if (obtained) {
            throw std::runtime_error("GetFuture was already called");
        }
        obtained = true;*/
        return Future<R>(st_ptr);
    }


    void set(const R &value) {
        std::unique_lock<std::mutex> lock(st_ptr->sh_mtx);
        if (st_ptr->sh_done) {
            throw std::runtime_error("Value is set");
        }
        st_ptr->sh_done = true;
        st_ptr->data = value;
        st_ptr->sh_cv.notify_one();
    }

    void set(R &&value) {
        std::unique_lock<std::mutex> lock(st_ptr->sh_mtx);
        if (st_ptr->sh_done) {
            throw std::runtime_error("Value is set");
        }
        st_ptr->sh_done = true;
        st_ptr->data = std::move(value);
        st_ptr->sh_cv.notify_one();
    }

    void setException(const std::exception_ptr &error) {
        std::unique_lock<std::mutex> lock(st_ptr->sh_mtx);
        if (st_ptr->sh_done) {
            throw std::runtime_error("Exception is set");
        }
        st_ptr->sh_done = true;
        st_ptr->sh_ex_ptr = error;
        st_ptr->sh_cv.notify_one();
    }

    void setPool(ThreadPool *pool) {
        std::unique_lock<std::mutex> lock(st_ptr->sh_mtx);
        st_ptr->pool = pool;
    }

    ~Promise() {
        if (st_ptr) {
            st_ptr->promise_al = false;
            st_ptr->sh_cv.notify_one();
        }
    }
};

template<typename R>
class Promise<R &> {
private:
    std::shared_ptr<Shared_state<R &>> st_ptr;
    bool obtained = false;
public:
    Promise(Promise &&ptr) {
        st_ptr = std::move(ptr.st_ptr);
    }

    Promise &operator=(Promise &&ptr) {
        if (this != &ptr) {
            st_ptr = std::move(ptr.st_ptr);
        }
        return *this;
    }

    Promise() {
        st_ptr = std::make_shared<Shared_state<R &>>();
        st_ptr->promise_al = true;
    }

    Future<R &> getFuture() {
        std::unique_lock<std::mutex> lock(st_ptr->sh_mtx);
        if (obtained) {
            throw std::runtime_error("GetFuture was already called");
        }
        obtained = true;
        return Future<R &>(st_ptr);
    }

    void set(R &value) {
        std::unique_lock<std::mutex> lock(st_ptr->sh_mtx);
        if (st_ptr->sh_done) {
            //setError(new std::runtime_error("Value is set"));
            throw std::runtime_error("Value is set");
        }
        st_ptr->sh_done = true;
        st_ptr->data = &value;
        st_ptr->sh_cv.notify_one();
    }

    void setException(const std::exception_ptr &error) {
        std::unique_lock<std::mutex> lock(st_ptr->sh_mtx);
        if (st_ptr->sh_done == true) {
            throw std::runtime_error("Exception is set");
        }
        st_ptr->sh_done = true;
        st_ptr->sh_ex_ptr = error;
        st_ptr->sh_cv.notify_one();

    }

    void setPool(ThreadPool *pool) {
        st_ptr->pool = pool;
    }

    ~Promise() {
        if (st_ptr) {
            st_ptr->promise_al = false;
            st_ptr->sh_cv.notify_one();
        }
    }
};

template<>
class Promise<void> {
private:
    std::shared_ptr<Shared_state<void>> st_ptr;
    bool obtained = false;
public:
    Promise(Promise &&ptr) {
        st_ptr = std::move(ptr.st_ptr);
    }

    Promise &operator=(Promise &&ptr) {
        if (this != &ptr) {
            st_ptr = std::move(ptr.st_ptr);
        }
        return *this;
    }

    Promise() {
        st_ptr = std::make_shared<Shared_state<void>>();
        st_ptr->promise_al = true;
    }

    Future<void> getFuture() {
        std::unique_lock<std::mutex> lock(st_ptr->sh_mtx);
        if (obtained) {
            throw std::runtime_error("GetFuture was already called");
        }
        obtained = true;
        return Future<void>(st_ptr);
    }

    void set() {
        std::unique_lock<std::mutex> lock(st_ptr->sh_mtx);
        if (st_ptr->sh_done) {
            //setError(new std::runtime_error("Value is set"));
            throw std::runtime_error("Value is set");
        }
        st_ptr->sh_done = true;
        st_ptr->sh_cv.notify_one();
    }

    void setException(const std::exception_ptr &error) {
        std::unique_lock<std::mutex> lock(st_ptr->sh_mtx);
        if (st_ptr->sh_done == true) {
            throw std::runtime_error("Exception is set");
        }
        st_ptr->sh_done = true;
        st_ptr->sh_ex_ptr = error;
        st_ptr->sh_cv.notify_one();

    }

    void setPool(ThreadPool *pool) {
        st_ptr->pool = pool;
    }

    ~Promise() {
        if (st_ptr) {
            st_ptr->promise_al = false;
            st_ptr->sh_cv.notify_one();
        }
    }
};

#endif //SETTINGUPGOOGLETEST_PROMISE_H
