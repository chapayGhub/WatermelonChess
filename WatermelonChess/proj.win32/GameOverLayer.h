#ifndef __GAME_OVER_H__
#define __GAME_OVER_H__   

#include "cocos2d.h" 
USING_NS_CC;
#include "PlayLayer.h"

class GameOverLayer : public CCLayerColor
{
public:
	~GameOverLayer();

	virtual bool initWithState(Game_Finish_State state);
	static GameOverLayer* create(Game_Finish_State state)
	{
		GameOverLayer* pRet = new GameOverLayer();
		if (pRet && pRet->initWithState(state))
		{
			pRet->autorelease();
			return pRet;
		}
		else
		{
			delete pRet;
			pRet = NULL;
			return NULL;
		}
	}
	virtual bool initWithString(const char* msg);
	static GameOverLayer* create(const char* msg)
	{
		GameOverLayer* pRet = new GameOverLayer();
		if (pRet && pRet->initWithString(msg))
		{
			pRet->autorelease();
			return pRet;
		}
		else
		{
			delete pRet;
			pRet = NULL;
			return NULL;
		}
	}

public:
	void quit(CCObject* pSender);
};

#endif