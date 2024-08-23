#pragma once
#include <memory>
#include "../Debug.h"

namespace AE::Utils {
	template <typename T>
	class Singleton {
		public:
			//do not allow copy construction or copy assignment
			Singleton(const Singleton& singleton) = delete;
			Singleton& operator=(const Singleton& singleton) = delete;


			static T& GetInstance() {
				static T instance;
				return instance;
			}

			static void Initialize() {
				GetInstance().OnInitialize();
			}

			static void ShutDown() {
				GetInstance().ShutDown();
			}

		protected:

			Singleton() = default;
			virtual ~Singleton() = default;

			virtual void OnInitialize() {}
			virtual void OnShutdown() {}
	};
}