#include "HelpLayer.h"
#include "SceneManager.h"
#include "MySound.h"
#include "Basic.h"

static const char* s_helpText = 
"西瓜棋是中国民间传统儿童\n\
博戏棋类，两人开始有六颗\n\
棋子对阵，轮流移动自己的\n\
一颗棋子，可以沿线移动到\n\
相邻的没有其他棋子的交叉\n\
点上。如果将对方的一个或\n\
多个棋子围住，即可吃掉那\n\
些无法移动的棋子。最先吃\n\
掉对方四个或以上棋子的为\n\
为获胜方。";

bool HelpLayer::init()
{
	if (!CCLayer::init())  
    {  
        return false;  
    }
      
    CCSize s = CCDirector::sharedDirector()->getWinSize();

	//bg
	CCSprite* bg = CCSprite::create(PIC_MAIN_BACKGROUND);
	bg->setAnchorPoint(ccp(0,0));
	addChild(bg);

	//text
	CCLabelTTF* text = CCLabelTTF::create(a2u(s_helpText).c_str(), "Arial", 40.0f, CCSizeMake(500,700), kCCTextAlignmentLeft);
	text->setPosition(ccp(s.width*0.5, s.height*0.5));
	addChild(text);	
	
	text = CCLabelTTF::create(a2u("天普教育").c_str(), "Arial", 30.0f);
	text->setPosition(ccp(500, 250));
	addChild(text);	

	//quit menu
	CCMenuItemImage* quit = CCMenuItemImage::create(
            PIC_QUIT,
            PIC_QUIT_SELECTED,
            this,
            menu_selector(HelpLayer::quit));
	CCMenu* menu = CCMenu::create(quit, NULL);
	menu->setPosition(ccp(s.width*0.5, 150));
	this->addChild(menu);

	return true;

}

void HelpLayer::quit(CCObject* pSender)
{
	MySound::stopSound();
	SceneManager::pop();
}
