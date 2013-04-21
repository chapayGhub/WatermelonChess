#include "MainMenuLayer.h"
#include "SceneManager.h"
#include "MySound.h"
#include "Basic.h"

	
bool MainMenuLayer::init()
{
	if (!CCLayer::init())  
    {  
        return false;  
    }

	loadImage();

    this->setTouchEnabled(true);//���ô���
      
    CCSize s = CCDirector::sharedDirector()->getWinSize();

	//background...
	/*CCScale9Sprite* bg = CCScale9Sprite::create(PIC_ROUND);
	bg->setPreferredSize(CCSizeMake(600, 600));
	bg->setAnchorPoint(ccp(0.5,0.5));
	//bg->setColor(ccc3(50,30,50));
	bg->setPosition(ccp(s.width*0.5, s.height*0.5));
	addChild(bg);*/
	
	CCSprite* bg = CCSprite::create(PIC_MAIN_BACKGROUND);
	bg->setAnchorPoint(ccp(0,0));
	addChild(bg);
	CCSprite* bgTitle = CCSprite::create(PIC_MAIN_TITLE);
	bgTitle->setPosition(ccp(s.width*0.5,830));
	addChild(bgTitle);
	
	//music button...
	CCSprite* musicOn = CCSprite::create(PIC_MUSIC_ON);
	CCSprite* musicOff = CCSprite::create(PIC_MUSIC_OFF);
	CCMenuItemSprite* music1 = CCMenuItemSprite::create(musicOn, musicOn);
	CCMenuItemSprite* music2 = CCMenuItemSprite::create(musicOff, musicOff);
	if(!SceneManager::music)
	{
		CCMenuItemSprite* tmp = music1;
		music1 = music2;
		music2 = tmp;
	}
	CCMenuItemToggle *music_toggle = CCMenuItemToggle::createWithTarget(this, menu_selector(MainMenuLayer::music), music1, music2, NULL);
	CCMenu *musicMenu = CCMenu::create(music_toggle,NULL);
	musicMenu->setPosition(ccp(s.width-54,s.height-52));
	this->addChild(musicMenu);

	// ����һ��CCMenuItemFont    
	CCMenuItemImage* item1 = CCMenuItemImage::create(PIC_BTN_2PLAYER1, PIC_BTN_2PLAYER2, this, menu_selector(MainMenuLayer::menuCallbackPlay) ); 
	CCMenuItemImage* item2 = CCMenuItemImage::create(PIC_BTN_COMPUTER1, PIC_BTN_COMPUTER2, this, menu_selector(MainMenuLayer::menuCallbackPlay2) ); 
	CCMenuItemImage* item3 = CCMenuItemImage::create(PIC_BTN_ONLINE1, PIC_BTN_ONLINE2, this, menu_selector(MainMenuLayer::menuCallbackPlayNet) ); 
	CCMenuItemImage* item4 = CCMenuItemImage::create(PIC_BTN_HELP1, PIC_BTN_HELP2, this, menu_selector(MainMenuLayer::menuCallbackHelp) ); 
	//CCMenuItemImage* item5 = CCMenuItemImage::create(PIC_BTN_LEADBOARD1, PIC_BTN_LEADBOARD2, this, menu_selector(MainMenuLayer::menuCallbackRecording) ); 
		
    /*CCMenuItemFont *item1 = CCMenuItemFont::create("2 Players", this, menu_selector(MainMenuLayer::menuCallbackPlay) );  
	item1->setFontSizeObj(100);
    //item2->setFontName("Marker Felt");  
    CCMenuItemFont *item2 = CCMenuItemFont::create("Vs Computer", this, menu_selector(MainMenuLayer::menuCallbackPlay2) );  
	item2->setFontSizeObj(100);
    CCMenuItemFont *item3 = CCMenuItemFont::create("Play online", this, menu_selector(MainMenuLayer::menuCallbackPlayNet) );  
    item3->setFontSizeObj(100);
    CCMenuItemFont *item4 = CCMenuItemFont::create("Recording", this, menu_selector(MainMenuLayer::menuCallbackRecording) );  
    item4->setFontSizeObj(100);*/


	//����CCMenu�˵�����������Ϊ�ǲ˵���   
    CCMenu* menu = CCMenu::create( item1, item2, item3, item4, NULL);  
    menu->alignItemsVertically();//�������Ϊ���óɴ�ֱ����   
    //menu->alignItemsHorizontally();//ˮƽ����   
    menu->setPosition(ccp(s.width/2, s.height/2));
	addChild(menu);	
	
	//leaderboard, achievement, weibo...
	item1 = CCMenuItemImage::create(PIC_BTN_LEADBOARD, PIC_BTN_LEADBOARD, this, menu_selector(MainMenuLayer::menuCallbackLeaderboard) ); 
	item2 = CCMenuItemImage::create(PIC_BTN_ACHIEVEMENT, PIC_BTN_ACHIEVEMENT, this, menu_selector(MainMenuLayer::menuCallbackAchievement) ); 
	item3 = CCMenuItemImage::create(PIC_BTN_WEIBO, PIC_BTN_WEIBO, this, menu_selector(MainMenuLayer::menuCallbackWeibo) ); 
	/*
	menu = CCMenu::create(item1,NULL);
	menu->setAnchorPoint(ccp(0.5,0.5));
	menu->setPosition(ccp(150,120));
	addChild(menu);
	menu = CCMenu::create(item2,NULL);
	menu->setAnchorPoint(ccp(0.5,0.5));
	menu->setPosition(ccp(320,120));
	addChild(menu);
	menu = CCMenu::create(item3,NULL);
	menu->setAnchorPoint(ccp(0.5,0.5));
	menu->setPosition(ccp(490,120));
	addChild(menu);*/
	
	menu = CCMenu::create(item1, item2, item3, NULL);
    menu->alignItemsHorizontally();//ˮƽ����menu
    menu->setPosition(ccp(480, 80));
	addChild(menu);
	
	CCLabelTTF* text = CCLabelTTF::create(a2u("����").c_str(), "Arial", 20.0f);
	text->setPosition(ccp(395,25));
	addChild(text);
	text = CCLabelTTF::create(a2u("�ɾ�").c_str(), "Arial", 20.0f);
	text->setPosition(ccp(480,25));
	addChild(text);
	text = CCLabelTTF::create(a2u("����").c_str(), "Arial", 20.0f);
	text->setPosition(ccp(565,25));
	addChild(text);

	return true;
}

void MainMenuLayer::loadImage()
{
	for(int i = 0; i < s_pics_num; i++)
	{
		CCTextureCache::sharedTextureCache()->addImage(s_pics[i].c_str());
	}
}

void MainMenuLayer::menuCallbackPlay(CCObject* sender) 
{
	MySound::playSound(SOUND_MENU);
	
	SceneManager::goPlay();
}

void MainMenuLayer::menuCallbackPlay2(CCObject* sender) 
{
	MySound::playSound(SOUND_MENU);
	
	SceneManager::goPlay2();
}

void MainMenuLayer::menuCallbackPlayNet(CCObject* sender) 
{
	MySound::playSound(SOUND_MENU);
	
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	SceneManager::goPlayNet();
#endif
}

void MainMenuLayer::menuCallbackHelp(CCObject* sender) 
{
	MySound::playSound(SOUND_MENU);
	SceneManager::goHelp();
}

void MainMenuLayer::menuCallbackLeaderboard(CCObject* sender) 
{
	MySound::playSound(SOUND_MENU);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	SceneManager::goLeaderboard();
#endif
}

void MainMenuLayer::menuCallbackAchievement(CCObject* sender) 
{
	MySound::playSound(SOUND_MENU);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	SceneManager::goAchievement();
#endif
}
	
void MainMenuLayer::menuCallbackWeibo(CCObject* sender) 
{
	MySound::playSound(SOUND_MENU);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	SceneManager::goWeibo();
#endif
}

void MainMenuLayer::music(CCObject *pSender)
{
	MySound::playSound(SOUND_MENU);
	if(SceneManager::music)
	{
		SceneManager::music = false;
		SceneManager::sound = false;
		MySound::stopMusic();
	}
	else
	{
		SceneManager::music = true;
		SceneManager::sound = true;
		MySound::playBackGround(1);
	}
}
