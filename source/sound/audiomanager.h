#pragma once

#include <string>
#include <map>
#include <fmod.hpp>
#include "carsounddata.h"


namespace troen
{
namespace sound
{
	/*! The AudioManager can load/play background songs and sounds for special effects (like collisions or the engine sound).*/
	class AudioManager {
	public:
		AudioManager();
		~AudioManager();
		void Update(float elapsed);
		void LoadSFX(const std::string& path);
		void LoadSong(const std::string& path);
		void LoadEngineSound();
		void PlaySFX(const std::string& path, float minVolume, float maxVolume, float minPitch, float maxPitch);
		void PlaySong(const std::string& path);
		void PlayEngineSound();
		void StopSFXs();
		void StopSongs();
		void SetMasterVolume(const float volume);
		float GetMasterVolume();
		void SetSFXsVolume(const float volume);
		void SetSongsVolume(const float volume);
		void detectBeat(const float tickCount);

		float getTimeSinceLastBeat();
		void setMotorSpeed(CarEngine * engine);
	private:
		typedef std::map<std::string, FMOD::Sound*> SoundMap;
		enum Category { CATEGORY_SFX, CATEGORY_SONG, CATEGORY_ENGINE, CATEGORY_COUNT };

		void Load(const Category type, const std::string& path);
		void auxiliaryCarSounds();
		FMOD::System* system;
		FMOD::ChannelGroup* master;
		FMOD::ChannelGroup* groups[CATEGORY_COUNT];
		SoundMap sounds[CATEGORY_COUNT];
		FMOD_MODE modes[CATEGORY_COUNT];

		float m_timeSinceLastBeat = 0.f;

		FMOD::Channel* currentSong;
		FMOD::Channel* engineChannel;
		FMOD::Channel* axleChannel;
		FMOD::DSP *m_dplowpass;

		std::shared_ptr<CarSoundData> m_engineSoundData;

		std::string currentSongPath;
		std::string nextSongPath;
		enum FadeState { FADE_NONE, FADE_IN, FADE_OUT };
		FadeState fade;

		FMOD::Channel* m_channel;
	};

}
}