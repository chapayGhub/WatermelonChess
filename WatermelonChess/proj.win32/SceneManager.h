#ifndef __SCENE_MANAGER_H__
#define __SCENE_MANAGER_H__

#include "cocos2d.h"
USING_NS_CC;

class SceneManager : public CCObject
{
public:
	SceneManager(void);
	~SceneManager(void);
	
    static void initLoading();
    static void goMenu(int transType = 0);
	static void goPlay();
	static void goPlay2();
	static void goHelp();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	static void goPlayNet();
	static void goLeaderboard(int transType = 0);
	static void goAchievement(int transType = 0);
	static void goWeibo(int transType = 0);
#endif
	static void goStop();
    static void go(CCLayer* layer, int transType = 0);
    static void push(CCLayer* layer);  
	static void pop();
    static CCScene* wrap(CCLayer* layer);      
	
    //������̬���   
    static CCTransitionScene* createTransition(CCScene* s, int index = 0);  
    //�������� CCTransitionScene ���󣬾��忴.cpp�е�ʵ��
public:
	static bool music;
	static bool sound;
};

#endif