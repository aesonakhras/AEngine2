#pragma once
#include <string>

namespace AE::System {
	class IAudioSystem {
		public:
			virtual bool LoadClip(const std::string& filename, const std::string& clipName) = 0;
			virtual void PlayClip(const std::string& clipName) = 0;
	};
}