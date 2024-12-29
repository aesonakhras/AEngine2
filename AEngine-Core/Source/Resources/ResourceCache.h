#pragma once
#include "Core/Types.h"
#include <unordered_map>
#include <string>
#include <memory>
//#include <atomic>
#include <shared_mutex>

//FIGURE OUT HWO TO HAVE IT SHOW TEMPLATES, NEED TO REMOVE THIS
#include "Graphics/Texture.h"

namespace AE::Core {
	template <typename T>
	class ResourceCache {
	private:
		struct CacheItem
		{
			std::shared_ptr<T> resource = nullptr;
			uint32 resourceSize = 0;
			bool isPersistent = false;
		};

		std::shared_mutex cacheMutex;

	public:
		std::unordered_map<std::string, CacheItem> cache;

		bool addItem(
			std::string id,
			std::shared_ptr<T> resource,
			uint32 resourceSize,
			bool isPersistent
		) {
			std::unique_lock<std::shared_mutex> lock(cacheMutex);

			if (cache.find(id) == cache.end()) {
				//cannot copy construct, since atomic does not allow it
				//that is why we use emplace
				cache.emplace(id, CacheItem{ resource, resourceSize, isPersistent });
				return true;
			}

			return false;
		}

		std::shared_ptr<T> GetItem(std::string id) {
			std::shared_lock<std::shared_mutex> lock(cacheMutex);
			if (cache.find(id) != cache.end()) {
				return cache[id].resource;
			}

			return nullptr;
		}

		void garbageCollect(uint32 clearToSize) {
			std::unique_lock<std::shared_mutex> lock(cacheMutex);

			for (auto it = cache.begin(); it != cache.end();) {
				//TODO: Most likely create a job to do this,
				//like delete the actual resource, remove from cache and move to a queue
				if ((it->second.isPersistent) && (it->second.resource.use_count() == 1)) {
					it = cache.erase(it);
				}
				else {
					it++;
				}
			}
		}
	};
};