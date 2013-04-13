#include "cocos2d.h"
#include "CCEGLView.h"
#include "AppDelegate.h"
#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "SceneManager.h"
#include "MainMenuLayer.h"
#include "NetPlayLayer.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#import"GCHelper.h"
#endif

using namespace CocosDenshion;
USING_NS_CC;

AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate()
{
    SimpleAudioEngine::end();
}

bool AppDelegate::applicationDidFinishLaunching()
{
    // initialize director
    CCDirector *pDirector = CCDirector::sharedDirector();
    pDirector->setOpenGLView(CCEGLView::sharedOpenGLView());
    
	CCSize screenSize = CCEGLView::sharedOpenGLView()->getFrameSize();
	CCSize designSize = CCSizeMake(640, 960);
    pDirector->setContentScaleFactor(1);
	CCEGLView::sharedOpenGLView()->setDesignResolutionSize(designSize.width,designSize.height, kResolutionNoBorder);
    
    // turn on display FPS
    pDirector->setDisplayStats(false);
    
    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 / 60);
	
	srand((unsigned)time(0));
	
	SceneManager::goMenu();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    // At the end of applicationDidFinishLaunching, right before
    // the last line that calls runWithScene:
    [[GCHelper sharedInstance] authenticateLocalUser];
#else
    CCLog("not ios platform...");
#endif
    
    return true;
}

void SceneManager::goPlayNet()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	NetPlayLayer* layer = NetPlayLayer::create();
	SceneManager::go(layer);
#else
    CCLog("not ios platform...");
#endif
}

void SceneManager::goRecording(int transType)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    //[[GCHelper sharedInstance] showLeaderboard];
    [[GCHelper sharedInstance] showAchivement];
#else
    CCLog("not ios platform...");
#endif
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground()
{
    CCDirector::sharedDirector()->stopAnimation();
    
    SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
    CCDirector::sharedDirector()->startAnimation();
    
    SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}
