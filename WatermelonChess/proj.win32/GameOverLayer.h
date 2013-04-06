#ifndef __GAME_OVER_H__
#define __GAME_OVER_H__   

#include "cocos2d.h" 
USING_NS_CC;
#include "PlayLayer.h"

class GameOverLayer : public CCLayerColor
{
public:
	~GameOverLayer();

	virtual bool initWithState(Game_Finish_State state); //CCLayer的儿子们都有这玩意儿。别忘了virtual关键字   
	
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
public:
	void quit(CCObject* pSender);
	void menuLottery(CCObject* pSender);
	void callBackLottery(CCNode* node);
	void callBackRemove(CCNode* node);
};

#endif