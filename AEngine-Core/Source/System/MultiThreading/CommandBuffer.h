#pragma once

#include <queue>
#include <mutex>
#include <functional>


namespace AE::Core {
    class CommandBuffer {
    public:

        void Submit(std::function<void()> command) {
            std::lock_guard<std::mutex> lock(bufferMutex);
            commands.push(std::move(command));
        }

        void Execute() {
            std::queue<std::function<void()>> commandsToExecute;

            {
                std::lock_guard<std::mutex> lock(bufferMutex);
                std::swap(commands, commandsToExecute);
            }

            while (!commandsToExecute.empty()) {
                commandsToExecute.front()();
                commandsToExecute.pop();
            }
        }

    private:
        std::queue<std::function<void()>> commands;
        std::mutex bufferMutex;
    };
}