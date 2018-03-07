//
// Created by Dmitrii on 21.11.2017.
//

#ifndef PROMISE_FUTURE_TUPLE_H
#define PROMISE_FUTURE_TUPLE_H

#include "Future.h"
#include "Promise.h"
#include <tuple>

template<typename T, typename Q = void>
struct nested_type_promise;

template<typename T>
struct nested_type_promise<T> {
    typedef T t_t;
};

template<typename T>
struct nested_type_promise<Future<T>> {
    typedef typename nested_type_promise<T>::t_t t_t;
};

template<typename ...Args>
struct nested_type_promise<std::tuple<Args...>> {
    typedef std::tuple<typename nested_type_promise<Args>::t_t...> t_t;
};


template<typename T>
T flatten_impl(const Future<T> &fut) {
    return std::move(fut.get());
}
/*
template<template<typename, typename...> typename C, typename T>
struct nested_type_promise<C<Future<T>>> {
    typedef C<T> t_t;
};
template<template<typename, typename...> typename C, typename T>
auto flatten_impl(C<Future<T>> const &fut_col) {
    C<T> out;
    for(auto it = fut_col.begin(); it != fut_col.end();it++) {
        out.insert(out.end(), std::move(*it.get()));
    }
    return out;
}*/

template<typename T>
auto flatten_impl(const Future<Future<T>> &fut) {
    return std::move(flatten_impl(std::move(fut.get())));
}

template<typename T>
auto flatten_impl(T e) {
    return e;
}

template<typename ...Args, std::size_t... I>
auto flatten_impl_tuple(const std::tuple<Args...> &tup, std::index_sequence<I...>) {
    return std::make_tuple(flatten_impl(std::get<I>(tup))...);
}

template<typename ...TParam, typename Indices = std::make_index_sequence<sizeof...(TParam)>>
auto flattenTuple(std::tuple<TParam...> tuple) {
    using name = typename nested_type_promise<std::tuple<TParam...>>::t_t;
    std::shared_ptr<Promise<name>> ptr(new Promise<name>);
    std::thread([ptr, &tuple]() {
        auto temp = flatten_impl_tuple(tuple, Indices{});
        ptr->set(temp);
    }).detach();
    return std::move(ptr->getFuture());
}

#endif //PROMISE_FUTURE_TUPLE_H
