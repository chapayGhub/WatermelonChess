//
//  NetPlayLayer.h
//  WatermelonChess
//
//  Created by 王奕凯 on 4/13/13.
//
//

#ifndef __WatermelonChess__NetPlayLayer__
#define __WatermelonChess__NetPlayLayer__

#include "PlayLayer.h"
#include "Chess.h"
#include "cocos2d.h"
USING_NS_CC;

#define NET_CHESS_BASE 100

/*
            20
        17		19
            18
 
    14		15		  16
 
 7	8	9	10	11	12	13
 
    4		5		  6
 
            2
        1		3
            0
 */


enum Net_Game_Finish_State
{
	NET_GAME_FINISH_A_WIN,
	NET_GAME_FINISH_B_WIN,
	NET_GAME_FINISH_DRAW
};

enum GameState{
    kGameStateWaitingForMatch =0,
    kGameStateWaitingForRandomNumber,
    kGameStateWaitingForStart,
    kGameStateActive,
    kGameStateDone
};

enum EndReason{
    kEndReasonWin,
    kEndReasonLose,
    kEndReasonDisconnect
};

enum MessageType{
    kMessageTypeRandomNumber = 0,
    kMessageTypeGameBegin,
    kMessageTypeMove,
    kMessageTypeGameOver
};

struct Message{
    MessageType messageType;
};

struct MessageRandomNumber{
    Message message;
    int randomNumber;
};

struct MessageGameBegin{
    Message message;
};

struct MessageMove{
    Message message;
    int from;
    int to;
};

struct MessageGameOver{
    Message message;
    BOOL player1Won;
};

class NetPlayLayer;

#import "GCHelper.h"
@interface NetPlayMatch : NSObject<GCHelperDelegate>
{
    NetPlayLayer* m_playLayer;
}
- (NetPlayMatch*)initWithLayer:(NetPlayLayer*) playLayer;
-(void)matchCancel;
- (void)matchStarted;
- (void)matchEnded;
- (void)match:(GKMatch *)match didReceiveData:(NSData *)data
   fromPlayer:(NSString *)playerID;
- (void)inviteReceived;
@end

class NetPlayLayer : public CCLayer//, public GCHelperDelegate
{
public:
    ~NetPlayLayer();
	virtual bool init(); //CCLayerµƒ∂˘◊”√«∂º”–’‚ÕÊ“‚∂˘°£±Õ¸¡Àvirtualπÿº¸◊÷
	CREATE_FUNC(NetPlayLayer); //«ßÕÚ±Õ¸¡À’‚∏ˆ∑Á«ÈÕÚ÷÷µƒ∫Í
    
	//÷ÿ–¥¥•∆¡œ‡πÿ∫Ø ˝----
	virtual void onEnter();
	virtual void onExit();
	virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
	virtual void ccTouchMoved(CCTouch* touch, CCEvent* event);
	virtual void ccTouchEnded(CCTouch* touch, CCEvent* event);
    
public:
	void back (CCObject* pSender);
	void menuStopCallback(CCObject* pSender);
	
	void setStopVisible(bool b);
	void initBoard();
    
public:
	bool getTouchChessPos(const CCPoint& point, int& chessPos);
	void selectChessman(int chessPos);
	void unselectChessman();
	void moveChessman();
    
private:
	CCPoint getChessPoint(int chessPos) { return ccp(s_chessmenPosition[chessPos].x, s_chessmenPosition[chessPos].y+160); }
	void realMove();
	void callbackMoveDone();
	void check();
	void callbackCheckDone(CCNode* node);
	void checkFinish();
	void nextTurn();
    
private:
	int m_levelA;
	int m_levelB;
	int m_touchChessPos;
	int m_moveToChessPos;
	bool m_selected;
	bool m_movable;
	CCLabelTTF* m_labelTurnA;
	CCLabelTTF* m_labelTurnB;
	CCSprite* m_chess1Select;
	CCSprite* m_chess2Select;
	CCSprite* m_hideChess;
    
	Chess m_chess;
	CCMenu* m_stopMenu;
    NetPlayMatch* m_match;
    
public:
    void matchStarted();
    void matchEnded();
    void matchCancel();
    void matchData(NSData* data, NSString* playerID);
    void restartGame();
    void commitScore();
    int winScore;
    int loseScore;
    
private:
    void setGameState(GameState state);
    void sendData(NSData *data);
    void sendRandomNumber();
    void sendGameBegin();
    void sendMove(int from, int to);
    void sendGameOver(BOOL player1Won);
    void tryStartGame();
    void setNickName(NSString* otherName);
    GameState m_state;
    int ourRandom;
    bool receivedRandom;
    bool isPlayer1;
    NSString* otherPlayerID;
    std::string myNickName;
    std::string otherNickName;
    bool isCancel;
    CCLabelTTF* m_waitLabel;
};

#endif /* defined(__WatermelonChess__NetPlayLayer__) */
