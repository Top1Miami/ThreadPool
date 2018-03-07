//
// Created by Dmitrii on 07.11.2017.
//

#ifndef PROMISE_FUTURE_THREADPOOL_H
#define PROMISE_FUTURE_THREADPOOL_H

#include <iostream>
#include <thread>
#include <queue>
#include <vector>
#include <memory>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <functional>

class ThreadPool {
private:
    std::vector<std::thread> th_proc;
    std::queue<std::function<void()>> q_op;
    size_t num_th;
    std::mutex op_mutex;
    std::condition_variable op_var;
    std::atomic<bool> term;
public:
    ThreadPool(size_t num_threads);

    ~ThreadPool();

    void execute(std::function<void()> const & foo);

    static thread_local ThreadPool *curPool;
    /*
    template<typename F>
    void parallel(auto a,auto b, F f){
        auto c = a;
        std::vector<Future<void>> task_thr;
        while(c != b) {
            if(c + 1 == b) {
                *c = f(*c);
                //Task for Parallel's thread
            } else {
                std::shared_ptr<Promise<void>> p(new Promise<void>);
                task_thr.push_back(std::move(p->getFuture()));
                execute([c, &f, p](){
                    *c = f(*c);
                    p->set();
                });
                //Other tasks
            }
            c++;
        }
        for(size_t i = 0; i < task_thr.size(); i++) {
            task_thr[i].wait();
        }
    }*/

};

#endif //PROMISE_FUTURE_THREADPOOL_H
