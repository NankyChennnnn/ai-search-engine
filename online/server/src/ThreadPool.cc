#include "ThreadPool.h"
#include <iostream>

using std::cout;
using std::endl;

ThreadPool::ThreadPool(size_t num, size_t capa)
: _num(num)
, _capacity(capa)
, _threads()
, _isExit(false)
{

}

ThreadPool::~ThreadPool()
{
}

void ThreadPool::start()
{
    // 创建并启动线程，最后放到vector中
    for (size_t idx = 0; idx != _num; ++idx)
    {
        thread th(&ThreadPool::doTask, this);
        _threads.push_back(std::move(th));
    }

}

void ThreadPool::stop()
{

}

Task ThreadPool::getTask()
{
    // taskQue.pop_back()
    cout << "[ERROR] nullptr" << endl;
}

void ThreadPool::doTask()
{
    // 只要线程池不退出，就一直执行任务
    // while(1) ❌
    // while(!_taskQue.empty()) ❌
    while (!_isExit)
    {
        // 获取任务与执行任务
        Task task = getTask();
        if (task)
        {
            /* ptask->process(); */
            task();
        }
        else
        {
            cout << "nullptr" << endl;
        }
    }
}
