#pragma once
#include <memory>
#include "Core/Debug.h"

namespace AE::Utils {
	template <typename T>
	class Singleton {
		public:
			//do not allow copy construction or copy assignment
			Singleton(const Singleton& singleton) = delete;
			Singleton& operator=(const Singleton& singleton) = delete;

			template <typename... Args>
			static void Initialize(Args&&... args) {
				if (!initialized) {
					instance.initialize(std::forward<Args>(args)...);
					initialized = true;
				}
			}

			static T& GetInstance() {
				if (!initialized)
				{
					AE::Core::Debug::LogError("Singleton called before being initialized, call Initialize first.");
				}

				return instance;
			}

			static void ShutDown() {
				if (initialized) {
					instance.shutdown();
					initialized = false;
				}
			}

		protected:
			Singleton() = default;
			~Singleton() = default;
		private:
			static bool initialized;
			static T instance;
	};
	template <typename T>
	bool Singleton<T>::initialized = false;

	template <typename T>
	T Singleton<T>::instance;
}