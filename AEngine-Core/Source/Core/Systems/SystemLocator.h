#pragma once
#include "unordered_map"


namespace AE::Core {
    class SystemLocator {
    public:
        template<typename T>
        static void Register(T* service) {
            systems[typeid(T).hash_code()] = static_cast<void*>(service);
        }

        template<typename T>
        static T* Get() {
            auto it = systems.find(typeid(T).hash_code());
            return it != systems.end() ? static_cast<T*>(it->second) : nullptr;
        }

    private:
        static inline std::unordered_map<size_t, void*> systems;
    };
}