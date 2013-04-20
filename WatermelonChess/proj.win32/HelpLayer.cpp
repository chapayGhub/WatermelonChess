#include "HelpLayer.h"
#include "SceneManager.h"
#include "MySound.h"
#include "Basic.h"

static const char* s_helpText = 
"���������й���䴫ͳ��ͯ\n\
��Ϸ���࣬���˿�ʼ������\n\
���Ӷ��������ƶ��Լ���\n\
һ�����ӣ����������ƶ���\n\
���ڵ�û���������ӵĽ���\n\
���ϡ�������Է���һ����\n\
�������Χס�����ɳԵ���\n\
Щ�޷��ƶ������ӡ����ȳ�\n\
���Է��ĸ����������ӵ�Ϊ\n\
Ϊ��ʤ����";

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
	
	text = CCLabelTTF::create(a2u("���ս���").c_str(), "Arial", 30.0f);
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
