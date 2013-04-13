#include "GameOverLayer.h"
#include "SceneManager.h"
#include "MySound.h"
#include "Basic.h"
#include "cocos-ext.h"
USING_NS_CC_EXT;

GameOverLayer::~GameOverLayer(void)
{
}

bool GameOverLayer::initWithState(Game_Finish_State state)
{
	if (!CCLayerColor::initWithColor(ccc4(0,0,0,180)))  
	{  
		return false;  
	}  
	// elastic effect
    CCSize s = CCDirector::sharedDirector()->getWinSize();
	
    this->setTouchEnabled(true);
	
	//background...
	CCScale9Sprite* bg = CCScale9Sprite::create(PIC_ROUND);
	bg->setPreferredSize(CCSizeMake(480,640));
	bg->setColor(ccc3(153,51,250));
	bg->setOpacity(128);
	bg->setPosition(ccp(s.width*0.5, s.height*0.5));
	addChild(bg);
	
	//game info...
	char buf[50];
	if(state == GAME_FINISH_A_WIN)
		sprintf(buf, "Blue Win!");
	else if(state == GAME_FINISH_B_WIN)
		sprintf(buf, "Pink Win!");
	else
		sprintf(buf, "Draw~~");
	CCLabelTTF* label = CCLabelTTF::create(buf, "Marker Felt", 100.0/*, CCSizeMake(200, 50), kCCTextAlignmentLeft*/);
	label->setPosition(ccp(s.width*0.5, s.height*0.5));
	label->setColor(ccc3(0,0,0));
	addChild(label);

	//quit menu
	CCMenuItemImage* quit = CCMenuItemImage::create(
            PIC_QUIT,
            PIC_QUIT_SELECTED,
            this,
            menu_selector(GameOverLayer::quit));
	CCMenu* menu = CCMenu::create(quit, NULL);
	menu->setPosition(ccp(s.width*0.5, 0));
	menu->alignItemsVertically();
	this->addChild(menu);
	menu->runAction(CCMoveTo::create(0.5f, ccp(s.width*0.5, s.height*0.3)));

	return true;
}

void GameOverLayer::quit(CCObject* pSender)
{
	MySound::stopSound();
	this->removeFromParentAndCleanup(false);
	SceneManager::goMenu();  
}
