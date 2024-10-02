#pragma once

#include <functional>
#include <queue>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <iostream>

#include <iostream>

namespace AE::Core {
    using JobFunction = std::function<void()>;

    class JobSystem {
    public:
        JobSystem(size_t numThreads) : shutdown(false) {
            for (size_t i = 0; i < numThreads; ++i) {
                workerThreads.emplace_back([this]() { WorkerThread(); });
            }
        }

        ~JobSystem() {
            {
                std::lock_guard<std::mutex> lock(queueMutex);
                shutdown = true;
            }
            cv.notify_all();

            for (auto& thread : workerThreads) {
                if (thread.joinable()) {
                    thread.join();
                }
            }
        }

        void SubmitJob(JobFunction job) {
            {
                std::lock_guard<std::mutex> lock(queueMutex);
                jobQueue.push(job);
            }
            cv.notify_one();
        }

        void WaitForCompletion() {
            std::unique_lock<std::mutex> lock(queueMutex);
            cv.wait(lock, [this]() { return jobQueue.empty() && activeJobs == 0; });
        }

    private:
        std::vector<std::thread> workerThreads;
        std::queue<JobFunction> jobQueue;
        std::mutex queueMutex;
        std::condition_variable cv;
        bool shutdown = false;
        int activeJobs = 0;

        void WorkerThread() {
            while (true) {
                JobFunction job;

                {
                    std::unique_lock<std::mutex> lock(queueMutex);
                    
                    //wait for job or shut down
                    cv.wait(lock, [this]() { return !jobQueue.empty() || shutdown; });

                    if (shutdown && jobQueue.empty()) {
                        break;
                    }

                    if (!jobQueue.empty()) {
                        job = jobQueue.front();
                        jobQueue.pop();
                        activeJobs++;
                    }
                }

                if (job) {
                    job();
                }

                {
                    std::lock_guard<std::mutex> lock(queueMutex);
                    activeJobs--;

                    if (jobQueue.empty() && activeJobs == 0) {
                        cv.notify_all();
                    }
                }
            }
        }
    };
}