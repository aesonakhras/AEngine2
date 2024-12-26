#pragma once
#include <iostream>
#include <unordered_map>
#include <string>
#include "Core/Containers/AEHashMap.h"



namespace AE::System {
    struct MemInfo{
        std::string fileName;
        int Line;
        std::string StackTrace;
    };

    class MemoryAllocator {
        public:
            static void* Alloc(size_t size) {
                return std::malloc(size);
            }

            static void Free(void* ptr) {
                std::free(ptr);
            }

            static void* DebugAlloc(size_t size, const char* file, int line) {
                void* mem = Alloc(size);
                
                if (mem != nullptr) {
                    memoryAllocations[mem] = { file, line, ""};
                    return mem;
                }
                
                //uh oh
                return nullptr;
            }

            static void DebugFree(void* ptr) {
                if (memoryAllocations.contains(ptr)) {
                    memoryAllocations.erase(ptr);
                }
                
                std::free(ptr);
            }

            static void DonaldDUMP() {

                for (const auto& allocation : memoryAllocations) {
                    std::cout << "Leak at adress: " << allocation.first 
                        << "on line: " << allocation.second.Line << "in file: " << allocation.second.fileName << std::endl;
                }
            }

        private:
            static AE::Core::AEHashMap<void*, MemInfo, 1024> memoryAllocations;
    };
}

AE::Core::AEHashMap<void*, AE::System::MemInfo, 1024> AE::System::MemoryAllocator::memoryAllocations;