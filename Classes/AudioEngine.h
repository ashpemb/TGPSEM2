#pragma once
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include <cstdlib>
#include <ctime>
#include "SimpleAudioEngine.h"  
#include <string>
class AudioEngine
{
public:
	AudioEngine();
	void Update();
	void PlayBackgroundMusic(std::string fileLocation,bool loop);
	void StopBackgroundMusic();
	int PlaySoundEffect(std::string fileLocation,bool loop);
	void StopSoundEffect(int soundID);
	void StopAllEffects();
	void SetBackgroundVolume(float volume);
	void SetSoundEffectsVolume(float volume);
	void PauseBackgroundMusic();
	void PauseAllEffects();
	void PauseEffect(int soundID);
	float GetBackgroundVolume();
	float GetEffectVolume();
	void BGMPlaylistNext();
	void SetBGMPlaylist(std::string fileLocations[5]);
	void MuteAllAudio(bool mute);
	void ResumeEffect(int soundID);
	void ResumeAllEffects();
	void ResumeBackgroundMusic();
	~AudioEngine();
private:
	float effectVolume;
	float bgmVolume;
	bool bgmPlaying;
	bool isPlaylistSet;
	std::string itemLocation[5];
	int playlistLocation;
	void VarLock();
	CocosDenshion::SimpleAudioEngine* audioManager;
	bool isMuted;
};

