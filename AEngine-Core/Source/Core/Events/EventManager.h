#pragma once

#include <iostream>

#include <unordered_map>
#include <vector>
#include <functional>
#include <typeindex>


namespace AE::Core {
    class EventManager {
    public:
        template<typename EventType>
        static void Subscribe(std::function<void(const EventType&)> listener) {
            GetListeners<EventType>().push_back(listener);
        }

        template<typename EventType>
        static void Emit(const EventType& event) {
            for (const auto& listener : GetListeners<EventType>()) {
                std::cout << "Emmiting ending event" << std::endl;
                listener(event);
            }
        }

    private:
        template<typename EventType>
        static std::vector<std::function<void(const EventType&)>>& GetListeners() {
            static std::vector<std::function<void(const EventType&)>> listeners;
            return listeners;
        }
    };
}