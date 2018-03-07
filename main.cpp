//
// Created by eugene on 28.9.2017
//
#ifdef _GTEST

#include <gtest/gtest.h>


#include <type_traits>

#endif
#include "FlattenTuple.h"
#include "Promise.h"
#include <vector>
#include "Future.h"
#include <chrono>
#include "ThreadPool.h"
using namespace std;



template<class Ch, class Tr, class Tuple, std::size_t... Is>
void print_tuple_impl(std::basic_ostream<Ch,Tr>& os,
                      const Tuple & t,
                      std::index_sequence<Is...>)
{
    ((os << (Is == 0? "" : ", ") << std::get<Is>(t)), ...);
}

template<class Ch, class Tr, typename  T>
decltype(auto) operator<<(std::basic_ostream<Ch, Tr>& os,
                          const vector<T>& t)
{
    os << "[ ";
    for(auto &i: t){

        os << i ;
        if(i != t.back())
            os << ", ";
    }
    return os << "]";
}

template<class Ch, class Tr, class... Args>
decltype(auto) operator<<(std::basic_ostream<Ch, Tr>& os,
                          const std::tuple<Args...>& t)
{
    os << "(";
    print_tuple_impl(os, t, std::index_sequence_for<Args...>{});
    return os << ")";
}
int main(int argc, char *argv[]) {

    /*std::vector<int> b {1 , 2 , 3};
    auto f = [](int &a){
        a += 2;
    };

    ThreadPool t(3);
    t.parallel(b.begin(), b.end(), f);

    std::this_thread::sleep_for(1s);

    for(int i = 0; i < b.size();i++) {
        std::cout << b[i] << " ";
    }*/

    return 0;/*
#ifdef _GTEST
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
#endif

    return 0;*/
}