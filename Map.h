//
// Created by Dmitrii on 07.11.2017.
//
#pragma once
#ifndef PROMISE_FUTURE_MAP_H
#define PROMISE_FUTURE_MAP_H

#include <thread>
#include <iostream>
#include <memory>
#include <functional>
#include "Future.h"
#include "Promise.h"
#include "ThreadPool.h"

template<typename T, typename F>
Future<typename std::result_of<F(T)>::type> map(ThreadPool *pool, const F &func, Future<T> &&f) {
    using D = typename std::result_of<F(T)>::type;
    std::shared_ptr<Promise<D> > p = std::shared_ptr<Promise<D>>(new Promise<D>());
    if (pool != nullptr) {
        pool->execute([&f, &func, p] {
            p->set(std::move(func(f.get())));
        });
    } else {
        std::thread([&f, &func, p] {
            p->set(std::move(func(f.get())));
        }).detach();
    }
    return std::move(p->getFuture());
};


template<typename T, typename F>
Future<typename std::result_of<F(T)>::type> map(Future<T> &&f, const F &func) {
    ThreadPool *pool;
    if (f.getPool() != nullptr) {
        //fut pool
        pool = f.getPool();
    } else if (ThreadPool::curPool != nullptr){
        //map pool
        pool = ThreadPool::curPool;
    } else {
        //new thr
        pool = nullptr;
    }
    return std::move(map(pool, func, std::move(f)));
};


#endif //PROMISE_FUTURE_MAP_H
