#include "AudioManager.h"
#include "Core/Debug.h"

using namespace AE::System;


//NOTE:  We only support 44100hz and 32bit depth
//otherwise the audio will not play

//TODO: abstract sdl audio layer
AudioManager::AudioManager() : isRunning(false) {
	if (SDL_InitSubSystem(SDL_INIT_AUDIO)) {
		AE::Core::Debug::LogError("Failed to initialize SDL audio: " + std::string(SDL_GetError()));
	}

	SDL_AudioSpec spec {};
	spec.freq = 44100;
	spec.format = AUDIO_F32;
	spec.channels = 2;
	spec.samples = 4096;
	spec.callback = AudioManager::AudioCallback;
	spec.userdata = this;

	SDL_AudioSpec givenSpec{};

	deviceID = SDL_OpenAudioDevice(nullptr, 0, &spec, &givenSpec, 0);

	if (deviceID == 0) {
		AE::Core::Debug::LogError("Failed to initialize SDL audio: " + std::string(SDL_GetError()));
	}

	SDL_PauseAudioDevice(deviceID, 0);
}

AudioManager::~AudioManager() {
	if (isRunning) {
		isRunning = false;
	}

	FreeClips();

	SDL_CloseAudioDevice(deviceID);
	SDL_Quit();
};

bool AudioManager::LoadAudioClip(const std::string& filename, const std::string& clipName) {
	//determine if clip exists already
	if (audioClips.contains(clipName)) {
		AE::Core::Debug::LogWarning("Audio Clip already exists, choose another name");
		return false;
	}
	
	AudioClipData clip;

	if (SDL_LoadWAV(filename.c_str(), &clip.wavSpec, &clip.wavBuffer, &clip.wavLength) == nullptr) {
		AE::Core::Debug::LogError(std::string("Failed to load audio clip") + std::string(SDL_GetError()));
		return false;
	}

	if (clip.wavBuffer == nullptr || clip.wavLength == 0) {
		std::cerr << "Audio buffer is invalid or empty after loading." << std::endl;
		return false;
	}

	audioClips[clipName] = clip;

	return true;
}

void AudioManager::PlayAudio(const std::string& clipName) {
	if (!audioClips.contains(clipName)) {
		AE::Core::Debug::LogWarning("unable to find audio clip to play");
		return;
	}

	const AudioClipData& clip = audioClips[clipName];

	AudioClip activeAudioClip = { clip, 0 };
	activeAudioClips.push_back(activeAudioClip);
}

void AudioManager::AudioCallback(void* userdata, Uint8* stream, int len) {
	AudioManager* manager = static_cast<AudioManager*>(userdata);

	// Call the non-static member function to fill the buffer
	manager->HandleAudio(stream, len);
}

void AudioManager::HandleAudio(Uint8* stream, int len) {
	if (activeAudioClips.size() == 0) {
		isRunning = false;
		return;
	}

	int perClipVolume = SDL_MIX_MAXVOLUME / activeAudioClips.size();

	SDL_memset(stream, 0, len);

	for (auto& clip : activeAudioClips)
	{
		Uint32 remainingLength = clip.audioClipdata.wavLength - clip.currentTimeStep;
		Uint32 bytesToMix = std::min((Uint32)len, remainingLength);

		SDL_MixAudioFormat(stream, clip.audioClipdata.wavBuffer + clip.currentTimeStep, clip.audioClipdata.wavSpec.format, bytesToMix, SDL_MIX_MAXVOLUME);

		clip.currentTimeStep += bytesToMix;
	}

	activeAudioClips.erase(
		std::remove_if(activeAudioClips.begin(), activeAudioClips.end(),
		[](const AudioClip& clip) {
			return clip.currentTimeStep >= clip.audioClipdata.wavLength;
		}),
		activeAudioClips.end()
	);
}

void AudioManager::FreeClips() {
	for (auto& pair : audioClips) {
		SDL_FreeWAV(pair.second.wavBuffer);
	}
	audioClips.clear();
}