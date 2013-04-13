//
//  GCHelper.h
//  WatermelonChess
//
//  Created by 王奕凯 on 4/13/13.
//
//

#import <UIKit/UIKit.h>
#import<Foundation/Foundation.h>
#import<GameKit/GameKit.h>

@protocol GCHelperDelegate
- (void)matchStarted;
- (void)matchEnded;
- (void)match:(GKMatch *)match didReceiveData:(NSData *)data
   fromPlayer:(NSString *)playerID;
@end
/*
class GCHelperDelegate
{
    void matchStarted();
    void matchEnded();
    GKMatch* didReceiveDataFromPlay(NSData* data, NSString* playerID);
};
 
 */
@interface GCHelper : NSObject <GKLeaderboardViewControllerDelegate, GKAchievementViewControllerDelegate, GKMatchmakerViewControllerDelegate, GKMatchDelegate>{
    BOOL gameCenterAvailable;
    BOOL userAuthenticated;
    
    UIViewController *presentingViewController;
    GKMatch *match;
    BOOL matchStarted;
    id<GCHelperDelegate>delegate;
    NSMutableDictionary *playersDict;
}

@property (assign, readonly) BOOL gameCenterAvailable;
@property (retain) UIViewController *presentingViewController;
@property (retain) GKMatch *match;
@property (assign) id<GCHelperDelegate>delegate;
@property (retain) NSMutableDictionary *playersDict;

+ (GCHelper *)sharedInstance;
- (void)authenticateLocalUser;
- (void) showLeaderboard;
- (void) showAchivement;
- (void)leaderboardViewControllerDidFinish:(GKLeaderboardViewController *)viewController;
- (void)achievementViewControllerDidFinish:(GKAchievementViewController *)viewController;

- (void)findMatchWithMinPlayers:(int)minPlayers maxPlayers:(int)maxPlayers
                       viewController:(UIViewController *)viewController
                       delegate:(id<GCHelperDelegate>)theDelegate;
@end


