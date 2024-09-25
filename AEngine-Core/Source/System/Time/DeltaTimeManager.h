#pragma once
#include <chrono>
#include <thread>

#include <SDL_audio.h>

#include "Core/Common.h"
#include "Utils/Singleton.h"


namespace AE::System {
	class DeltaTimeManager : public AE::Utils::Singleton<DeltaTimeManager> {
	public:
		DeltaTimeManager() : m_targetFrameTime(0.0f) {}

		void StartFrame() {
			auto currentTime = std::chrono::high_resolution_clock::now();
			std::chrono::duration<AE::Core::float32> deltaTime = currentTime - m_lastTime;
			m_lastTime = currentTime;
			m_deltaTime = deltaTime.count();
		}

		AE::Core::float32 GetDeltaTime() {
			return m_deltaTime;
		}

		void LimitFrameRate() {
			auto currentTime = std::chrono::high_resolution_clock::now();
			std::chrono::duration<AE::Core::float32> frameTime = currentTime - m_lastTime;

			if (frameTime.count() < m_targetFrameTime) {
				AE::Core::float32 remainingTime = m_targetFrameTime - frameTime.count();
				std::this_thread::sleep_for(std::chrono::duration<AE::Core::float32>(remainingTime));
			}
		}


	private:
		AE::Core::float32 m_targetFrameTime;
		AE::Core::float32 m_deltaTime = 0.0f;
		std::chrono::high_resolution_clock::time_point m_lastTime;

		friend class AE::Utils::Singleton<DeltaTimeManager>;
		void initialize(AE::Core::float32 targetFrameRate) {
			m_targetFrameTime = (1.0f / targetFrameRate);
		}

		void shutdown() {

		}
	};
}