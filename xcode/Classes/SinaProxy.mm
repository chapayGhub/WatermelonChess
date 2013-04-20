//
//  SinaProxy.cpp
//  WatermelonChess
//
//  Created by 王奕凯 on 4/20/13.
//
//


#import "SinaProxy.h"


@implementation SinaProxy

@synthesize sinaweibo;

#pragma mark Initialization

static SinaProxy *sharedProxy = nil;

+ (SinaProxy *) sharedInstance {
    if (!sharedProxy) {
        sharedProxy = [[SinaProxy alloc] initWithKey:kAppKey
                                              secret:kAppSecret
                                                redi:kAppRedirectURI];
    }
    return sharedProxy;
}

-(id) initWithKey:(NSString *)appKey secret:(NSString *)appSecret redi:(NSString *)redirectUri
{    
    sinaweibo = [[SinaWeibo alloc] initWithAppKey:appKey appSecret:appSecret appRedirectURI:redirectUri andDelegate:self];
    NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
    NSDictionary *sinaweiboInfo = [defaults objectForKey:@"SinaWeiboAuthData"];
    
    if ([sinaweiboInfo objectForKey:@"AccessTokenKey"] && [sinaweiboInfo objectForKey:@"ExpirationDateKey"]
        && [sinaweiboInfo objectForKey:@"UserIDKey"]){
        sinaweibo.accessToken = [sinaweiboInfo objectForKey:@"AccessTokenKey"];
        sinaweibo.expirationDate = [sinaweiboInfo objectForKey:@"ExpirationDateKey"];
        sinaweibo.userID = [sinaweiboInfo objectForKey:@"UserIDKey"];
    }
    
    return self;
}

-(void)likeWatermelonChess
{
    type = WEIBO_LIKE;
    [sinaweibo logIn];
}


-(void)sinaweiboDidLogIn:(SinaWeibo *)sinaweibo
{
    //将获取的信息打印 log。
    NSLog(@"sinaweiboDidLogIn userID = %@ accesstoken = %@ expirationDate = %@ refresh_token = %@", sinaweibo.userID, sinaweibo.accessToken, sinaweibo.expirationDate,sinaweibo.refreshToken);
    if(type == WEIBO_LIKE)
    {
        [sinaweibo requestWithURL:@"statuses/upload.json"
                           params:[NSMutableDictionary dictionaryWithObjectsAndKeys:
                                   @"西瓜棋有人听说过吗？小时候玩的，下了一个，感觉找回了童年的记忆……（北京天普教育）https://itunes.apple.com/us/app/xi-gua-qi/id634355640?ls=1&mt=8", @"status",
                                   //@"hello world", @"status",
                                   [UIImage imageNamed:@"Icon@2x.png"], @"pic", nil]
                       httpMethod:@"POST"
                         delegate:self];    }
}

- (void)request:(SinaWeiboRequest *)request didFailWithError:(NSError *)error
{
    NSLog(@"requet error...");
}

@end