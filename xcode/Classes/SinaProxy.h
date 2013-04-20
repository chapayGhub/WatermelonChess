//
//  SinaProxy.h
//  WatermelonChess
//
//  Created by 王奕凯 on 4/20/13.
//
//

#ifndef __WatermelonChess__SinaProxy__
#define __WatermelonChess__SinaProxy__

#import "SinaWeibo.h"

#define kAppKey @"949830220"
#define kAppSecret @"594bb37263cf3cc3b0eb9a3f32f3d051"
#define kAppRedirectURI @"http://weibo.com/u/1421220553"

enum weiboType
{
    WEIBO_LIKE,
};

@interface SinaProxy : NSObject<SinaWeiboDelegate, SinaWeiboRequestDelegate>
{    
    UIViewController *presentingViewController;
    SinaWeibo *sinaweibo;
    weiboType type;
}

@property (readonly, nonatomic) SinaWeibo *sinaweibo;

+ (SinaProxy *)sharedInstance;
-(id) initWithKey:(NSString *)appKey_ secret:(NSString *)appSecret_ redi:(NSString *)redirectUri_;
-(void) likeWatermelonChess;
-(void)sinaweiboDidLogIn:(SinaWeibo *)sinaweibo;


- (void)request:(SinaWeiboRequest *)request didFailWithError:(NSError *)error;
@end



#endif /* defined(__WatermelonChess__SinaProxy__) */
