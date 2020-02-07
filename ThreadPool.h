#ifndef _THREADPOOL_
#define _THREADPOOL_

#include <queue>
#include <thread>
#include <functional>
#include <future>

using namespace std;

namespace SNY
{
class FunctionWrapper
{
    struct ImplBase
    {
        virtual ~ImplBase() {}
        virtual void Call() = 0;
    };

    template <typename F>
    struct ImplType : ImplBase
    {
        ImplType(F &&_f) : f(move(_f)) {}
        void Call() { f(); }

        F f;
    };

public:
    FunctionWrapper() = default;
    template <typename F>
    FunctionWrapper(F &&f) : impl(new ImplType<F>(move(f))) {}
    void operator()() { impl->Call(); }
    FunctionWrapper(FunctionWrapper &) = delete;
    FunctionWrapper(const FunctionWrapper &) = delete;
    FunctionWrapper(FunctionWrapper &&fr) : impl(move(fr.impl)) {}

    FunctionWrapper &operator=(const FunctionWrapper &) = delete;
    FunctionWrapper &operator=(FunctionWrapper &&fr)
    {
        impl = move(fr.impl);
        return *this;
    }

private:
    unique_ptr<ImplBase> impl;
};

template <typename T>
class ThreadSafeQueue
{
public:
    ThreadSafeQueue() {}
    ~ThreadSafeQueue() {}

    void Push(T value)
    {
        shared_ptr<T> p(make_shared<T>(move(value)));
        lock_guard<mutex> lk(mut);
        dataQueue.push(p);
        cv.notify_all();
    }

    void WaitAndPop(T &value)
    {
        unique_lock<mutex> lk(mut);
        cv.wait(lk, [this]() { return !dataQueue.empty(); });
        value = move(*dataQueue.front());
        dataQueue.pop();
    }

    bool TryPop(T &value)
    {
        lock_guard<mutex> lk(mut);
        if (dataQueue.empty())
        {
            return false;
        }
        value = move(*dataQueue.front());
        dataQueue.pop();
        return true;
    }

    shared_ptr<T> WaitAndPop()
    {
        unique_lock<mutex> lk(mut);
        cv.wait(lk, [this]() { return !dataQueue.empty(); });
        shared_ptr<T> res = dataQueue.front();
        dataQueue.pop();
        return res;
    }

    shared_ptr<T> TryPop()
    {
        lock_guard<mutex> lk(mut);
        if (dataQueue.empty())
        {
            return shared_ptr<T>();
        }
        shared_ptr<T> res = dataQueue.front();
        dataQueue.pop();
        return res;
    }

    bool Empty() const
    {
        lock_guard<mutex> lk(mut);
        return dataQueue.empty();
    }

private:
    mutable mutex mut;
    queue<shared_ptr<T>> dataQueue;
    condition_variable cv;
};

class JoinThreads
{
public:
    explicit JoinThreads(vector<thread> &t) : threads(t)
    {
        for (size_t i = 0; i < threads.size(); ++i)
        {
            if (threads[i].joinable())
            {
                threads[i].join();
            }
        }
    }
    ~JoinThreads()
    {
        for (size_t i = 0; i < threads.size(); ++i)
        {
            if (threads[i].joinable())
            {
                threads[i].join();
            }
        }
    }

    vector<thread> &threads;
};

class SimpleThreadPool
{
public:
    SimpleThreadPool(unsigned nTh) : done(false), joiner(threads)
    {
        unsigned threadCount = nTh > thread::hardware_concurrency() ? thread::hardware_concurrency() : nTh;
        try
        {
            for (unsigned i = 0; i < threadCount; ++i)
            {
                threads.push_back(thread(&SimpleThreadPool::Work, this));
            }
        }
        catch (...)
        {
            done = true;
            throw;
        }
    }

    ~SimpleThreadPool()
    {
        done = true;
    }

    template <typename FunctionType>
    void Submit(FunctionType f)
    {
        workQueue.Push(function<void()>(f));
    }

private:
    void Work()
    {
        while (!done)
        {
            function<void()> task;
            if (workQueue.TryPop(task))
            {
                task();
            }
            else
            {
                this_thread::yield();
            }
        }
    }

    atomic_bool done;
    vector<thread> threads;
    JoinThreads joiner;
    ThreadSafeQueue<function<void()>> workQueue;
};

class WaitableThreadPool
{
public:
public:
    WaitableThreadPool(unsigned nTh) : done(false), joiner(threads)
    {
        unsigned threadCount = nTh > thread::hardware_concurrency() ? thread::hardware_concurrency() : nTh;
        try
        {
            for (unsigned i = 0; i < threadCount; ++i)
            {
                threads.push_back(thread(&WaitableThreadPool::Work, this));
            }
        }
        catch (...)
        {
            done = true;
            throw;
        }
    }

    ~WaitableThreadPool()
    {
        done = true;
    }

    template <typename FT>
    future<typename result_of<FT()>::type> Submit(FT f)
    {
        typedef typename result_of<FT()>::type ResultType;

        packaged_task<ResultType()> task(move(f));
        future<ResultType> rs(task.get_future());
        workQueue.Push(move(task));
        return rs;
    }

private:
    void Work()
    {
        while (!done)
        {
            FunctionWrapper task;
            if (workQueue.TryPop(task))
            {
                task();
            }
            else
            {
                this_thread::yield();
            }
        }
    }

    atomic_bool done;
    vector<thread> threads;
    JoinThreads joiner;
    ThreadSafeQueue<FunctionWrapper> workQueue;
};
} // namespace SNY

#endif
