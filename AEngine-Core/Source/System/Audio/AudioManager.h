#pragma once
#include <SDL.h>

#include <string>
#include <unordered_map>

#include <Core/Common.h>

namespace AE::System {
	class AudioManager {
		public:
			AudioManager();
			~AudioManager();

			bool LoadAudioClip(const std::string& filename, const std::string& clipName);
			void PlayAudio(const std::string& clipName);
		private:
			struct AudioClipData
			{
				SDL_AudioSpec wavSpec;
				AE::Core::uint8* wavBuffer;
				AE::Core::uint32 wavLength;
			};

			struct AudioClip {
				AudioClipData audioClipdata;
				AE::Core::uint32 currentTimeStep;
			};

			std::unordered_map<std::string, AudioClipData> audioClips;
			SDL_AudioDeviceID deviceID;
			
			void FreeClips();
			static void AudioCallback(void* userdata, Uint8* stream, int len);

			void HandleAudio(Uint8* stream, int len);

			bool isRunning;


			std::vector<AudioClip> activeAudioClips;
	};
}