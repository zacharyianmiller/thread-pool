#include "ThreadPool.hpp"

static constexpr unsigned int NumJobs = 12;

int main()
{
    ASIO::ThreadPool pool;

    for (auto i = 0; i < NumJobs; ++i)
    {
        pool.addJob([] {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        });
    }

    pool.start();

    std::this_thread::sleep_for(std::chrono::milliseconds(2000));

    return 0;
}