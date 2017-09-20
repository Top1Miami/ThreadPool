// Thr1.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include <iostream>
#include <thread>
#include <queue>
#include <vector>
#include <memory>
#include <mutex>
#include <condition_variable>
#include <atomic>
using namespace std;

class ThreadPool {
private:
	vector<thread> th_proc;
	queue < function<void()>> q_op;
	size_t num_th;
	mutex op_mutex;
	condition_variable op_var;
	atomic<bool> term;

public:	
	ThreadPool(size_t num_threads)
	{
		term = false;
		num_th = num_threads;
		th_proc.reserve(num_th);
		for (int i = 0; i < num_th; i++)
		{
			th_proc.emplace_back(thread([&] {
				while (!term)
				{
					unique_lock<mutex> lock_q(op_mutex);
					op_var.wait(lock_q, [=] { return !q_op.empty() && !term; });
					//if (!q_op.empty())
					//{
					function<void()> bar = q_op.front();
					q_op.pop();
					lock_q.unlock();
					bar();
					//}
					lock_q.lock();
				}
			}));
		}
	}
	~ThreadPool()
	{
		cout << "nestruktor";
		unique_lock<mutex> lock_q(op_mutex);
		term = true;
		op_var.notify_all();
		for (int i = 0; i < num_th; i++)
		{
			th_proc[i].join();
		}
	}
	void execute(function<void()> const & foo)
	{
		unique_lock<mutex> lock_q(op_mutex);
		q_op.push(function<void()>(foo));
		op_var.notify_one();
	}
};


void thread1()
{
	cout << "thread 1\n";
	cout << "thread 1\n";
	this_thread::sleep_for(chrono::milliseconds(100));
	cout << "thread 1\n";
	cout << "thread 1\n";
}
/*
void thread2()
{
cout << "thread 2\n";
this_thread::sleep_for(chrono::milliseconds(100));
cout << "thread 2\n";
}*/

void thread3()
{
	int a = 3;
	int b = 3;
	int c = a + b;
	cout << c;
}

int main() {
	ThreadPool pool(3);
	{
		pool.execute(thread1);
		//pool.execute(thread2);
		pool.execute([]() {
			cout << "lambda thread\n";
		});
		pool.execute(thread3);
		this_thread::sleep_for(chrono::milliseconds(1337));
	}
	this_thread::sleep_for(chrono::milliseconds(1337));

	return 0;
}
