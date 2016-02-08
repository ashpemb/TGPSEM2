#include "AudioEngine.h"

AudioEngine::AudioEngine()
{
	bgmVolume = 1.0f;
	effectVolume = 1.0f;
	bgmPlaying = false;
	isPlaylistSet = false;
	playlistLocation = 0;
	isMuted = false;
	audioManager = CocosDenshion::SimpleAudioEngine::getInstance();

}


AudioEngine::~AudioEngine()
{
}

void AudioEngine::Update()
{
	bgmPlaying = audioManager->getInstance()->isBackgroundMusicPlaying();
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
	if (isMuted == false)
	{
		const char * c = fileLocation.c_str();

		audioManager->getInstance()->playBackgroundMusic(
			c, true);
	}
}

void AudioEngine::SetBackgroundVolume(float volume)
{
	bgmVolume = volume;
	audioManager->getInstance()->setBackgroundMusicVolume(volume);
}

void AudioEngine::StopBackgroundMusic()
{
	audioManager->getInstance()->stopBackgroundMusic();
}

int AudioEngine::PlaySoundEffect(std::string fileLocation, bool loop)
{
	if (isMuted == false)
	{
		const char * c = fileLocation.c_str();

		int id = audioManager->getInstance()->playEffect(c, loop, 1, 0, 1);
		return id;
	}
}

void AudioEngine::SetSoundEffectsVolume(float volume)
{
	effectVolume = volume;
	audioManager->getInstance()->setEffectsVolume(effectVolume);
}

void AudioEngine::StopAllEffects()
{
	audioManager->getInstance()->stopAllEffects();	
}

void AudioEngine::StopSoundEffect(int soundID)
{
	audioManager->getInstance()->stopEffect(soundID);
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
	audioManager->getInstance()->pauseBackgroundMusic();
}

void AudioEngine::PauseAllEffects()
{
	audioManager->getInstance()->pauseAllEffects();
}

void AudioEngine::PauseEffect(int soundID)
{
	audioManager->getInstance()->pauseEffect(soundID);
}

void AudioEngine::ResumeEffect(int soundID)
{
	audioManager->getInstance()->resumeEffect(soundID);
}

void AudioEngine::ResumeAllEffects()
{
	audioManager->getInstance()->resumeAllEffects();
}

void AudioEngine::ResumeBackgroundMusic()
{
	audioManager->getInstance()->resumeBackgroundMusic();
}


void AudioEngine::VarLock()
{
	if (playlistLocation >= 5)
	{
		playlistLocation = 0;
	}
}

void AudioEngine::MuteAllAudio(bool mute)
{
	isMuted = mute;
	if (isMuted == false)
	{
		ResumeAllEffects();
		ResumeBackgroundMusic();
	}
	else if (isMuted == true)
	{
		PauseAllEffects();
		PauseBackgroundMusic();
	}
}