#include "AudioEngine.h"

AudioEngine::AudioEngine()
{
	bgmVolume = 1.0f;
	effectVolume = 1.0f;
	bgmPlaying = false;
	isPlaylistSet = false;
	playlistLocation = 0;
	isMuted = false;
}


AudioEngine::~AudioEngine()
{
}

void AudioEngine::Update()
{
	bgmPlaying = CocosDenshion::SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying();
	if (bgmPlaying == false && isPlaylistSet == true)
	{
		BGMPlaylistNext();
	}
	VarLock();
}

void AudioEngine::SetBGMPlaylist(std::string fileLocations[5])
{
	itemLocation[0] = fileLocations[0];
	itemLocation[1] = fileLocations[1];
	itemLocation[2] = fileLocations[2];
	itemLocation[3] = fileLocations[3];
	itemLocation[4] = fileLocations[4];
	isPlaylistSet = true;
}

void AudioEngine::BGMPlaylistNext()
{
	StopBackgroundMusic();
	PlayBackgroundMusic(itemLocation[playlistLocation], false);
	playlistLocation++;
	bgmPlaying = true;
}

void AudioEngine::PlayBackgroundMusic(std::string fileLocation, bool loop)
{
	const char * c = fileLocation.c_str();

	CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic(
		c, true);
}

void AudioEngine::SetBackgroundVolume(float volume)
{
	bgmVolume = volume;
	CocosDenshion::SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(bgmVolume);
}

void AudioEngine::StopBackgroundMusic()
{
	CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
}

int AudioEngine::PlaySoundEffect(std::string fileLocation, bool loop)
{
	const char * c = fileLocation.c_str();

	int id =	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(c, loop, 1, 0, 1);
	return id;
}

void AudioEngine::SetSoundEffectsVolume(float volume)
{
	effectVolume = volume;
	CocosDenshion::SimpleAudioEngine::sharedEngine()->setEffectsVolume(effectVolume);
}

void AudioEngine::StopAllEffects()
{
	CocosDenshion::SimpleAudioEngine::sharedEngine()->stopAllEffects();	
}

void AudioEngine::StopSoundEffect(int soundID)
{
	CocosDenshion::SimpleAudioEngine::sharedEngine()->stopEffect(soundID);
}

float AudioEngine::GetBackgroundVolume()
{
	return bgmVolume;
}

float AudioEngine::GetEffectVolume()
{
	return effectVolume;
}

void AudioEngine::PauseBackgroundMusic()
{
	CocosDenshion::SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

void AudioEngine::PauseAllEffects()
{
	CocosDenshion::SimpleAudioEngine::sharedEngine()->pauseAllEffects();
}

void AudioEngine::PauseEffect(int soundID)
{
	CocosDenshion::SimpleAudioEngine::sharedEngine()->pauseEffect(soundID);
}

void AudioEngine::ResumeEffect(int soundID)
{
	CocosDenshion::SimpleAudioEngine::sharedEngine()->resumeEffect(soundID);
}

void AudioEngine::ResumeAllEffects()
{
	CocosDenshion::SimpleAudioEngine::sharedEngine()->resumeAllEffects();
}

void AudioEngine::ResumeBackgroundMusic()
{
	CocosDenshion::SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}


void AudioEngine::VarLock()
{
	if (playlistLocation >= 5)
	{
		playlistLocation = 0;
	}
}

void AudioEngine::MuteAllAudio()
{
	isMuted != isMuted;
	if (isMuted == true)
	{
		SetSoundEffectsVolume(0);
		SetBackgroundVolume(0);
	}
	if (isMuted == false)
	{
		SetSoundEffectsVolume(1);
		SetBackgroundVolume(1);
	}
}