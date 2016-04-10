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
	void BGMPlaylistNext();
	void SetBGMPlaylist(std::string fileLocations[5]);
	void MuteAllAudio(bool mute);
	void ResumeEffect(int soundID);
	void ResumeAllEffects();
	void ResumeBackgroundMusic();
	~AudioEngine();
	void SetEffectVolume(float _newEffectVolume) { effectVolume = _newEffectVolume; }
	float GetEffectVolume() { return effectVolume; }
	void SetBGMVolume(float _newBGMVolume) { bgmVolume = _newBGMVolume; }
	float GetBGMVolume() { return bgmVolume; }
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

