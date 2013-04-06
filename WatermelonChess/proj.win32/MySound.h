#ifndef __MYSOUND_H__
#define __MYSOUND_H__

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
using namespace CocosDenshion;
#define PLAY_EFFECT(A) SimpleAudioEngine::sharedEngine()->playEffect(A)

enum Sound_Type
{
	SOUND_INIT,
	SOUND_OPEN,
	SOUND_MONEY,
	SOUND_UPGRADE,
	SOUND_FIND_PROP,
	SOUND_SELECT_PROP,
	SOUND_DROP_PROP,
	SOUND_GAMEOVER,
	SOUND_BOMB,
	SOUND_DEAD,
	SOUND_LEVEL_COMPLETE,
	SOUND_PROP_FULL,
	SOUND_PROP_DO,
	SOUND_SKILL_DEFENSE,

	SOUND_COUNT
};

const char* const sound_effects[SOUND_COUNT] = {
	"sound_init.wav",
	"sound_open.wav",
	"sound_money.mp3",
	"sound_upgrade.mp3",
	"sound_findProp.mp3",
	"sound_selectProp.mp3",
	"sound_propDown.mp3",
	"sound_gameOver.mp3",
	"sound_bomb.wav",
	"sound_dead.wav",
	"sound_levelComplete.mp3",
	"sound_propFull.mp3",
	"sound_propDo.wav",
	"sound_defense.wav",
};

class MySound
{
public:
	MySound() {}
public:
	static void loadSounds();

	static void playBackGround(int index);
	static void playSound(Sound_Type type);

	static void stopMusic();
	static void stopSound();
	static void pauseMusic();
	static void resumeMusic();
public:
	static int music_index;
};

#endif