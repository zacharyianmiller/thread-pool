#include "ThreadPool.hpp"

namespace ASIO
{

std::string_view ThreadPool::LoggerName = "[ThreadPool]";

ThreadPool::ThreadPool()
{
    std::println("{} Logical CPU cores available for concurrent work: {}", LoggerName, std::thread::hardware_concurrency());
}

ThreadPool::~ThreadPool()
{
    std::println("{} Work complete. Process terminated.", LoggerName);

    stop();
}

void ThreadPool::start()
{
    const auto availableThreads = std::thread::hardware_concurrency();

    for (std::uint32_t i = 0; i < availableThreads; ++i)
    {
        // Assign thread job to this->ThreadPool::loop()
        threads.emplace_back(
            std::thread(
                &ThreadPool::loop,
                this
            )
        );
    }
}

void ThreadPool::stop()
{
    {
        std::unique_lock<std::mutex> lock(mutex);
        terminate = true;
    }

    cv.notify_all();
    for (auto& thread : threads)
    {
        // If joinable, synchronize threads when work is done
        if (thread.joinable()) thread.join();
    }

    threads.clear();
}

bool ThreadPool::isBusy()
{
    bool working;
    {
        std::unique_lock<std::mutex> lock(mutex);
        working = !jobs.empty();
    }

    return working;
}

void ThreadPool::addJob(std::function<void()> job)
{
    {
        std::unique_lock<std::mutex> lock(mutex);
        jobs.push(job);
    }

    cv.notify_one();
}

void ThreadPool::loop()
{
    while (true)
    {
        // Declare target job object 
        std::function<void()> job;
        {
            std::unique_lock<std::mutex> lock(mutex);

            // Wait until job is available or the process is killed
            cv.wait(lock, [this] {
                return !jobs.empty() || terminate;
            });

            if (terminate) return;

            // Assign new job, remove from queue
            job = jobs.front();
            jobs.pop();
        }
        
        // Do work, log current thread
        job(); 

        auto id = std::hash<std::thread::id>()(std::this_thread::get_id());
        std::println("{} Thread #{} started work.", LoggerName, id);
    }
}

}; // namespace ASIO