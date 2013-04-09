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

    this->setTouchEnabled(true);//���ô���
      
    CCSize s = CCDirector::sharedDirector()->getWinSize();

	//background...
	CCScale9Sprite* bg = CCScale9Sprite::create(PIC_ROUND);
	bg->setPreferredSize(CCSizeMake(600, 600));
	bg->setAnchorPoint(ccp(0.5,0.5));
	//bg->setColor(ccc3(50,30,50));
	bg->setPosition(ccp(s.width*0.5, s.height*0.5));
	addChild(bg);
	
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
	musicMenu->setPosition(ccp(s.width-40,s.height-40));
	this->addChild(musicMenu);

	// ����һ��CCMenuItemFont    
    CCMenuItemFont *item1 = CCMenuItemFont::create("Play", this, menu_selector(MainMenuLayer::menuCallbackPlay) );  
	item1->setFontSizeObj(100);
    //item2->setFontName("Marker Felt");  
    CCMenuItemFont *item2 = CCMenuItemFont::create("Vs Computer", this, menu_selector(MainMenuLayer::menuCallbackPlay2) );  
	item2->setFontSizeObj(100);
    CCMenuItemFont *item3 = CCMenuItemFont::create("Recording", this, menu_selector(MainMenuLayer::menuCallbackRecording) );  
    item3->setFontSizeObj(100);  


	//����CCMenu�˵�����������Ϊ�ǲ˵���   
    CCMenu* menu = CCMenu::create( item1, item2, item3, NULL);  
    menu->alignItemsVertically();//�������Ϊ���óɴ�ֱ����   
    //menu->alignItemsHorizontally();//ˮƽ����   

	addChild(menu);
    menu->setPosition(ccp(s.width/2, s.height/2));  
	
	loadImage();
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
	MySound::playSound(SOUND_SELECT_PROP);
	
	SceneManager::goPlay();
}

void MainMenuLayer::menuCallbackPlay2(CCObject* sender) 
{
	MySound::playSound(SOUND_SELECT_PROP);
	
	SceneManager::goPlay2();
}

void MainMenuLayer::menuCallbackRecording(CCObject* sender) 
{
	MySound::playSound(SOUND_SELECT_PROP);
	SceneManager::goRecording();
}

void MainMenuLayer::music(CCObject *pSender)
{
	MySound::playSound(SOUND_SELECT_PROP);
	if(SceneManager::music)
	{
		SceneManager::music = false;
		MySound::stopMusic();
	}
	else
	{
		SceneManager::music = true;
		MySound::playBackGround(2);
	}
}
