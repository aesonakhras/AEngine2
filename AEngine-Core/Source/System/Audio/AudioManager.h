#pragma once


#include <string>
#include <unordered_map>
#include <memory>

#include "Core/Common.h"

#include "Utils/Singleton.h"

namespace AE::System {
	//forward declares
	class IAudioSystem;

	class AudioManager : public AE::Utils::Singleton<AudioManager> {
		public:
			bool LoadAudioClip(const std::string& filename, const std::string& clipName);
			void PlayAudio(const std::string& clipName);

		private:
			AudioManager() = default;
			~AudioManager() = default;

			friend class AE::Utils::Singleton<AudioManager>;
			void initialize();
			void shutdown();

			std::shared_ptr<IAudioSystem> audioSystem;
	};
}