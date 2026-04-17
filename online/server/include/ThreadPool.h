#ifndef __THREADPOOL_H__
#define __THREADPOOL_H__

#include <vector>
#include <thread>
#include <functional>

using std::vector;
using std::thread;
using std::function;

using Task = function<void()>;

class ThreadPool
{
public:
    ThreadPool(size_t num, size_t capa);
    ~ThreadPool();

    void start();
    void stop();

private:
    Task getTask();
    void doTask(); // 线程入口函数

private:
    size_t _num;
    size_t _capacity;
    vector<thread> _threads;
    bool _isExit;
};

#endif
