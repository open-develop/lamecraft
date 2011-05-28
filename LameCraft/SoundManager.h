#ifndef SOUNDMANAGER_H_
#define SOUNDMANAGER_H_

extern "C"
{
	#include "sound_utils/oslib.h"
}

class SoundManager
{
public:
	SoundManager();
	~SoundManager();

	static SoundManager m_SoundManager;
	static SoundManager* Instance();

	void Init();

	void PlayMenuSound();
	void PlayPlopSound();
	void PlayWalkSound(int type);

	bool playerSounds;

private:

	int currentWalkSound;
	int lastWalkSound;

	//sounds
	OSL_SOUND *buttonSound;

	//plop sound
	OSL_SOUND *plopSound;

	//grass sounds
	OSL_SOUND *grassSounds[4];
	OSL_SOUND *grawelSounds[4];
	OSL_SOUND *stoneSounds[4];
	OSL_SOUND *woodSounds[4];
};

#endif

