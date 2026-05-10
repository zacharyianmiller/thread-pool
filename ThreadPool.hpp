#include <iostream>
#include <thread>
#include <algorithm>
#include <functional>
#include <queue>

namespace ASIO
{

class ThreadPool
{
public:
    ThreadPool();
    ~ThreadPool();

    void addJob(std::function<void()> job); 
    void start();
    void stop();
    bool isBusy();
private:
    void loop();

    std::mutex mutex;
    std::condition_variable cv;
    std::vector<std::thread> threads;
    std::queue<std::function<void()>> jobs;

    static std::string_view LoggerName;

    std::uint32_t numThreads_;
    bool terminate = false;
};

}; // namespace ASIO