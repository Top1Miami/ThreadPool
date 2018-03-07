//
// Created by Dmitrii on 07.11.2017.
//
#include "ThreadPool.h"
ThreadPool::ThreadPool(size_t num_threads) {
    term = false;
    num_th = num_threads;
    th_proc.reserve(num_th);

    for (int i = 0; i < num_th; i++) {
        th_proc.emplace_back(std::thread([&] {
            curPool = this;
            while (!term) {
                std::function<void()> bar;
                std::unique_lock<std::mutex> lock_q(op_mutex);
                op_var.wait(lock_q, [=] { return !q_op.empty() || term; });
                if(term) {
                    return;
                }
                bar = std::move(q_op.front());
                q_op.pop();
                lock_q.unlock();
                bar();
            }
        }));
    }
}

ThreadPool::~ThreadPool() {
    std::unique_lock<std::mutex> lock_q(op_mutex);
    term = true;
    lock_q.unlock();
    op_var.notify_all();
    for (int i = 0; i < num_th; i++) {
        th_proc[i].join();
    }
}

void ThreadPool::execute(std::function<void()> const & foo)
{
    std::unique_lock<std::mutex> lock_q(op_mutex);
    q_op.push(std::function<void()>(foo));
    op_var.notify_one();
}

thread_local ThreadPool *ThreadPool::curPool = nullptr;
