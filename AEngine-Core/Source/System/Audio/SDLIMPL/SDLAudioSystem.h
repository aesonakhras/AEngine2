#pragma once
#include <SDL.h>
#include <unordered_map>
#include "System/Audio/IAudioSystem.h"
#include "Core/Common.h"

namespace AE::System {
	class SDLAudioSystem :  public IAudioSystem {
		public:
			SDLAudioSystem();
			~SDLAudioSystem();

			virtual bool LoadClip(const std::string& filename, const std::string& clipName) override;
			virtual void PlayClip(const std::string& clipName) override;

		private:
			struct AudioClipData
			{
				SDL_AudioSpec wavSpec;
				uint8* wavBuffer;
				uint32 wavLength;
			};

			struct AudioClip {
				AudioClipData audioClipdata;
				uint32 currentTimeStep;
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