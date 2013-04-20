//
//  WatermelonChessAppController.mm
//  WatermelonChess
//
//  Created by 王奕凯 on 4/8/13.
//  Copyright __MyCompanyName__ 2013. All rights reserved.
//
#import <UIKit/UIKit.h>
#import "AppController.h"
#import "cocos2d.h"
#import "EAGLView.h"
#import "AppDelegate.h"

#import "RootViewController.h"
#define USE_AD 0
#if USE_AD
#import "YouMiConfig.h"
#import "YouMiView.h"
#endif

@implementation AppController

@synthesize window;
@synthesize viewController;

#pragma mark -
#pragma mark Application lifecycle

// cocos2d application instance
static AppDelegate s_sharedApplication;

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {

    // Override point for customization after application launch.

    // Add the view controller's view to the window and display.
    window = [[UIWindow alloc] initWithFrame: [[UIScreen mainScreen] bounds]];
    EAGLView *__glView = [EAGLView viewWithFrame: [window bounds]
                                     pixelFormat: kEAGLColorFormatRGBA8
                                     depthFormat: GL_DEPTH_COMPONENT16
                              preserveBackbuffer: NO
                                      sharegroup: nil
                                   multiSampling: NO
                                 numberOfSamples:0 ];

    // Use RootViewController manage EAGLView
    viewController = [[RootViewController alloc] initWithNibName:nil bundle:nil];
    viewController.wantsFullScreenLayout = YES;
    viewController.view = __glView;

    // Set RootViewController to window
    if ( [[UIDevice currentDevice].systemVersion floatValue] < 6.0)
    {
        // warning: addSubView doesn't work on iOS6
        [window addSubview: viewController.view];
    }
    else
    {
        // use this method on ios6
        [window setRootViewController:viewController];
    }
    
    [window makeKeyAndVisible];
    
#if USE_AD
    ///////////////////////////////////////////////////////////////
    [YouMiConfig setShouldGetLocation:NO]; // 可选,控制 是否使⽤用地理位置
    [YouMiConfig launchWithAppID:@"e4c4302945b5b5c5" appSecret:@"352ce5887be1b5fb"];
    
    // 创建⼲⼴广告条
    YouMiView* adView = [[YouMiView alloc] initWithContentSizeIdentifier:YouMiBannerContentSizeIdentifier320x50 delegate:nil];
    
    //设置您应⽤用的推开渠道号 [可选]
    //adView.channelID = 1;
    
    //可以设置委托[可选]
    //adView.delegate = viewController;
    
    //设置⽂文字⼲⼴广告的属性[可选]
    adView.indicateBorder         = NO;
    adView.indicateTranslucency  = YES;
    //adView.indicateRounded        = NO;
    //adView.indicateBackgroundColor  	= [UIColor purpleColor];
    //adView.textColor                   	= [UIColor whiteColor];
    //adView.subTextColor               	= [UIColor yellowColor];
    
    //添加对应的关键词 [可选]
    [adView addKeyword:@"游戏"];
    [adView setFrame:(CGRectMake(0,0,320,50))];
    
    // 开始请求⼲⼴广告
    [adView start];
    
    // 把⼲⼴广告条添加到特定的view中 [例如]
    [__glView addSubview:adView];
    ///////////////////////////////////////////////////////////////
#endif

    [[UIApplication sharedApplication] setStatusBarHidden: YES];

    cocos2d::CCApplication::sharedApplication()->run();
    return YES;
}


- (void)applicationWillResignActive:(UIApplication *)application {
    /*
     Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
     Use this method to pause ongoing tasks, disable timers, and throttle down OpenGL ES frame rates. Games should use this method to pause the game.
     */
    cocos2d::CCDirector::sharedDirector()->pause();
}

- (void)applicationDidBecomeActive:(UIApplication *)application {
    /*
     Restart any tasks that were paused (or not yet started) while the application was inactive. If the application was previously in the background, optionally refresh the user interface.
     */
    cocos2d::CCDirector::sharedDirector()->resume();
}

- (void)applicationDidEnterBackground:(UIApplication *)application {
    /*
     Use this method to release shared resources, save user data, invalidate timers, and store enough application state information to restore your application to its current state in case it is terminated later.
     If your application supports background execution, called instead of applicationWillTerminate: when the user quits.
     */
    cocos2d::CCApplication::sharedApplication()->applicationDidEnterBackground();
}

- (void)applicationWillEnterForeground:(UIApplication *)application {
    /*
     Called as part of  transition from the background to the inactive state: here you can undo many of the changes made on entering the background.
     */
    cocos2d::CCApplication::sharedApplication()->applicationWillEnterForeground();
}

- (void)applicationWillTerminate:(UIApplication *)application {
    /*
     Called when the application is about to terminate.
     See also applicationDidEnterBackground:.
     */
}

/*
- (NSUInteger)application:(UIApplication*)application supportedInterfaceOrientationsForWindow:(UIWindow*)window
{
    return UIInterfaceOrientationMaskAllButUpsideDown;
}*/


#pragma mark -
#pragma mark Memory management

- (void)applicationDidReceiveMemoryWarning:(UIApplication *)application {
    /*
     Free up as much memory as possible by purging cached data objects that can be recreated (or reloaded from disk) later.
     */
     cocos2d::CCDirector::sharedDirector()->purgeCachedData();
}


- (void)dealloc {
    [super dealloc];
}


@end

