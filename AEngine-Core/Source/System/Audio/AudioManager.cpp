#include "AudioManager.h"
#include "Core/Debug.h"

#include "SDLIMPL/SDLAudioSystem.h"

using namespace AE::System;

//NOTE:  We only support 44100hz and 32bit depth
//otherwise the audio will not play
void AudioManager::initialize() {
	audioSystem = std::make_shared<SDLAudioSystem>();
}

void AudioManager::shutdown() {
	
}

bool AudioManager::LoadAudioClip(const std::string& filename, const std::string& clipName) {
	return audioSystem->LoadClip(filename, clipName);
}

void AudioManager::PlayAudio(const std::string& clipName) {
	audioSystem->PlayClip(clipName);
}