#ifndef FLATTEN
#define FLATTEN

#include <thread>
#include "Promise.h"
#include "Future.h"
#include <memory>
#include <iostream>

template<typename T>
struct nested_type_promise;

template<typename T>
struct nested_type_promise<Future<T>> {
    typedef T t_t;
};

template<typename T>
struct nested_type_promise<Future<Future<T>>> {
    typedef typename nested_type_promise<Future<T>>::t_t t_t;
};

template<typename T>
auto flatten(const Future<T> &fut) {
    std::shared_ptr<Promise<T>> p(new Promise<T>());
    std::thread([&fut, p]() {
        p->set(std::move(fut.get()));
    }).detach();
    return std::move(p->getFuture());
}

template<typename T>
auto flatten_impl(const Future<Future<T>> &fut) {
    return std::move(flatten_impl(std::move(fut.get())));
}

template<typename T>
auto flatten(const Future<Future<T>> &fut) {
    std::shared_ptr<Promise<typename nested_type_promise<Future<T>>::t_t>> p(
            new Promise<typename nested_type_promise<Future<T>>::t_t>());
    std::thread([&fut, p]() {
        p->set(std::move(flatten_impl(fut)));
    }).detach();
    return std::move(p->getFuture());
}

template<typename T>
auto flatten_impl(const Future<T> &fut) {
    return (std::move(fut.get()));
}

template<template<typename, typename...> typename C, typename T>
Future<C<T>> flatten(C<Future<T>> const &fut_col) {
    std::shared_ptr<Promise<C<T>>> p(new Promise<C<T>>());
    std::thread([&fut_col, p]() {
        C<T> out = C<T>();
        for (const Future<T> &fut : fut_col) {
            out.insert(std::end(out), fut.get());
        }
        p->set(out);
    }).detach();
    return std::move(p->getFuture());
}

#endif //FLATTEN