//
//  NetPlayLayer.cpp
//  WatermelonChess
//
//  Created by 王奕凯 on 4/13/13.
//
//

#import"AppController.h"
#import"RootViewController.h"
#import "NetPlayLayer.h"
#include "SceneManager.h"
#include "StopLayer.h"
#include "MySound.h"
#include "Basic.h"
#include "GameOverLayer.h"
#include <pthread.h>
//#pragma comment(lib, "pthreadVC2.lib")  //±ÿ–Îº”…œ’‚æ‰

#include "cocos-ext.h"
USING_NS_CC_EXT;


@implementation NetPlayMatch

-(NetPlayMatch*) initWithLayer :(NetPlayLayer*) playLayer
{
    self = [super init];
    if(self)
    {
        m_playLayer = playLayer;
    }
    return self;
}

- (void)matchCancel
{
    m_playLayer->matchCancel();
}

- (void)matchStarted
{
    m_playLayer->matchStarted();
}
- (void)matchEnded
{
    m_playLayer->matchEnded();
}
- (void)match:(GKMatch *)match didReceiveData:(NSData *)data
fromPlayer:(NSString *)playerID
{
    m_playLayer->matchData(data, playerID);
}
- (void)inviteReceived
{
    m_playLayer->restartGame();
}
@end

void NetPlayLayer::matchStarted()
{
    CCLog("matchStart...");
    if (receivedRandom)
    {
        setGameState(kGameStateWaitingForStart);
    }
    else
    {
        setGameState(kGameStateWaitingForRandomNumber);
    }
    sendRandomNumber();
    tryStartGame();
}

void NetPlayLayer::matchEnded()
{
    CCLog("matchEnd...");
    [[GCHelper sharedInstance].match disconnect];
    [GCHelper sharedInstance].match = nil;
    CCLog("end disconnect...");
	m_movable = false;
    char buf[200];
    sprintf(buf, "%s\n离开了游戏", otherNickName.c_str());
    GameOverLayer* gameOver = (GameOverLayer*)getChildByTag(444);
    if(!gameOver)
    {
        GameOverLayer* gameOver = GameOverLayer::create(buf);
        addChild(gameOver, 1000, 444);
        setStopVisible(false);
        m_labelTurnA->setVisible(false);
        m_labelTurnB->setVisible(false);
    }
    else
    {
        
        CCLabelTTF* label = CCLabelTTF::create("连接已断开...", "Marker Felt", 30.0f);
        label->setPosition(ccp(320, 600));
        gameOver->addChild(label);
    }
}

void NetPlayLayer::matchCancel()
{
    isCancel = true;
    if(m_waitLabel)
        m_waitLabel->setVisible(false);
    SceneManager::goMenu();
}

void NetPlayLayer::matchData(NSData* data, NSString* playerID)
{
    CCLog("matchData...");
    // Store away other player ID for later
    if (otherPlayerID == nil)
    {
        otherPlayerID = [playerID retain];
    }
    
    Message *message = (Message *) [data bytes];
    if (message->messageType == kMessageTypeRandomNumber)
    {
        MessageRandomNumber * messageInit = (MessageRandomNumber *) [data bytes];
        CCLog("Received random number: %d, ours %d", messageInit->randomNumber, ourRandom);
        bool tie = false;
        
        if (messageInit->randomNumber == ourRandom)
        {
            CCLog("TIE!");
            tie =true;
            ourRandom = random()%100;
            sendRandomNumber();
        }
        else if (ourRandom > messageInit->randomNumber)
        {
            CCLog("We are player 1");
            isPlayer1 = true;
        }
        else
        {
            CCLog("We are player 2");
            isPlayer1 = false;
            m_chess.nextTurn();
        }
        
        if (!tie)
        {
            receivedRandom = YES;
            if (m_state == kGameStateWaitingForRandomNumber)
            {
                setGameState(kGameStateWaitingForStart);
            }
            tryStartGame();
        }
        
    }
    else if (message->messageType == kMessageTypeGameBegin)
    {
        setNickName(otherPlayerID);
        setGameState(kGameStateActive);
        
    }
    else if (message->messageType == kMessageTypeMove)
    {
        MessageMove * messageMove = (MessageMove *) [data bytes];
        
        m_touchChessPos = 20 - messageMove->from;
        m_moveToChessPos = 20 - messageMove->to;
        realMove();
        /*
        CCLog("Received move");
        if (isPlayer1)
        {
            CCLog("2 move from %d to %d", messageMove->from, messageMove->to);
        }
        else
        {
            CCLog("1 move from %d to %d", messageMove->from, messageMove->to);
        }*/
    }
    else if (message->messageType == kMessageTypeGameOver)
    {
        MessageGameOver * messageGameOver = (MessageGameOver *) [data bytes];
        CCLog("Received game over with player 1 won: %d", messageGameOver->player1Won);
        
        if (messageGameOver->player1Won)
        {
        }
        else
        {
        }
        
    }
}

void NetPlayLayer::sendData(NSData *data)
{
    NSError *error;
    BOOL success = [[GCHelper sharedInstance].match sendDataToAllPlayers:data withDataMode:GKMatchSendDataReliable error:&error];
    if (!success)
    {
        CCLOG("Error sending init packet");
        matchEnded();
    }
}

void NetPlayLayer::setGameState(GameState state)
{
    m_state = state;
    switch(state)
    {
        case kGameStateWaitingForMatch:
            CCLog("Waiting for match");
            break;
        case kGameStateWaitingForRandomNumber:
            CCLog("Waiting for rand #");
            break;
        case kGameStateWaitingForStart:
            CCLog("Waiting for start");
            break;
        case kGameStateActive:
        {
            m_waitLabel->setVisible((false));            
            if(isPlayer1)
            {
                m_labelTurnA->setVisible(true);
                m_movable = true;
            }
            else
            {
                m_labelTurnB->setVisible(true);
            }
            CCLog("Active");
            break;
        }
        case kGameStateDone:
            CCLog("Done");
            break;
    }    
}
void NetPlayLayer::sendRandomNumber()
{
    MessageRandomNumber message;
    message.message.messageType = kMessageTypeRandomNumber;
    message.randomNumber = ourRandom;
    NSData *data = [NSData dataWithBytes:&message length:sizeof(MessageRandomNumber)];
    sendData(data);
}

void NetPlayLayer::sendGameBegin()
{
    MessageGameBegin message;
    message.message.messageType = kMessageTypeGameBegin;
    NSData *data = [NSData dataWithBytes:&message length:sizeof(MessageGameBegin)];
    sendData(data);
    
}

void NetPlayLayer::sendMove(int from, int to)
{    
    MessageMove message;
    message.message.messageType = kMessageTypeMove;
    message.from = from;
    message.to = to;
    CCLog("send %d, %d", from, to);
    NSData *data = [NSData dataWithBytes:&message length:sizeof(MessageMove)];
    sendData(data);
    
}

void NetPlayLayer::sendGameOver(BOOL player1Won)
{
    MessageGameOver message;
    message.message.messageType = kMessageTypeGameOver;
    message.player1Won = player1Won;
    NSData *data = [NSData dataWithBytes:&message length:sizeof(MessageGameOver)];
    sendData(data);    
}

void NetPlayLayer::tryStartGame()
{
    if (isPlayer1 && m_state == kGameStateWaitingForStart)
    {
        setNickName(otherPlayerID);
        setGameState(kGameStateActive);
        sendGameBegin();
    }    
}


void NetPlayLayer::setNickName(NSString* playerID)
{
    char buf[50];
    sprintf(buf, "轮到%s", [GKLocalPlayer localPlayer].alias.UTF8String);
    m_labelTurnA->setString(buf);
    myNickName = [GKLocalPlayer localPlayer].alias.UTF8String;
    GKPlayer *player = [[GCHelper sharedInstance].playersDict objectForKey:playerID];
    sprintf(buf, "轮到%s", player.alias.UTF8String);
    m_labelTurnB->setString(buf);
    otherNickName = player.alias.UTF8String;
}

void NetPlayLayer::restartGame()
{
    // Reload the current scene
    SceneManager::goPlayNet();
}

bool NetPlayLayer::init()
{
	if (!CCLayer::init())
	{
		return false;
	}
    CCSize s = CCDirector::sharedDirector()->getWinSize();
	//init
	
	m_touchChessPos = -1;
	m_moveToChessPos = -1;
	m_selected = false;
	m_movable = false;
	m_levelA = 0;
	m_levelB = 0;
    
	//background
	CCSprite* bg = CCSprite::create(PIC_MAIN_BACKGROUND);
	bg->setPosition(ccp(s.width*0.5, s.height*0.5));
	addChild(bg);
    
	//board
	CCSprite* chessboard = CCSprite::create(PIC_CHESSBOARD1);
	chessboard->setPosition(ccp(s.width*0.5, s.height*0.5));
	addChild(chessboard);
	//chessmen
	initBoard();
    
	//select and hide chess...
	m_hideChess = NULL;
	m_chess1Select = CCSprite::create(PIC_CHESSMAN1S);
	m_chess2Select = CCSprite::create(PIC_CHESSMAN2S);
	m_chess1Select->setVisible(false);
	m_chess2Select->setVisible(false);
	addChild(m_chess1Select,2);
	addChild(m_chess2Select,2);
    
	//turns label...
	m_labelTurnA = CCLabelTTF::create("Your turns", "Marker Felt", 50);
	m_labelTurnA->setAnchorPoint(ccp(0.5,0.5));
	m_labelTurnA->setPosition(ccp(s.width*0.5,100));
	m_labelTurnA->setVisible(false);
	addChild(m_labelTurnA);
	m_labelTurnB = CCLabelTTF::create("Other turns", "Marker Felt", 50);
	m_labelTurnB->setAnchorPoint(ccp(0.5,0.5));
	m_labelTurnB->setPosition(ccp(s.width*0.5,s.height-100));
	m_labelTurnB->setVisible(false);
	addChild(m_labelTurnB);
    m_waitLabel = CCLabelTTF::create("等待对手…", "Marker Felt", 50);
	m_waitLabel->setAnchorPoint(ccp(0.5,0.5));
	m_waitLabel->setPosition(ccp(s.width*0.5,s.height-100));
	addChild(m_waitLabel);
    
	//stop button...
	CCMenuItemImage* stopItem = CCMenuItemImage::create(
                                                        PIC_STOPBUTTON,
                                                        PIC_STOPBUTTON_SELECTED,
                                                        this,
                                                        menu_selector(PlayLayer::menuStopCallback));
	stopItem->setPosition(ccp(s.width - 40, s.height - 40));
	// Create a menu with the "close" menu item, it's an auto release object.
	m_stopMenu = CCMenu::create(stopItem, NULL);
	m_stopMenu->setPosition(CCPointZero);
	this->addChild(m_stopMenu, 15);
	
	//m_chess.nextTurn();
	//nextTurn();
    
    isCancel = false;
    NetPlayMatch* m_match = [[NetPlayMatch alloc] initWithLayer:this];
    AppController* delegate= (AppController *) [UIApplication sharedApplication].delegate;
    [[GCHelper sharedInstance] findMatchWithMinPlayers:2 maxPlayers:2 viewController:delegate.viewController delegate:m_match];
    
    ourRandom = rand()%100;
    setGameState(kGameStateWaitingForMatch);
    
	return true;
}

NetPlayLayer::~NetPlayLayer()
{
}

void NetPlayLayer::back(cocos2d::CCObject *pSender)
{
	SceneManager::goMenu();
	//backæÕ «ªÿ÷˜≤Àµ•£¨’‚¿Ôµ˜”√¡À»Àº˚»À∞Æ ª®º˚ª®ø™µƒæ≤Ã¨∫Ø ˝ goMenu
}

void NetPlayLayer::initBoard()
{
#if 0
	for(int i = 0; i < 21; i++)
	{
		char buf[3];
		sprintf(buf, "%d", i);
		CCLabelTTF* num = CCLabelTTF::create(buf, "Marker Felt",30);
		num->setPosition(getChessPoint(i));
		num->setColor(ccc3(255,0,0));
		addChild(num, 2);
	}
#endif
    
	int initChessmemA[6] = {0,1,2,3,4,6};
	int initChessmemB[6] = {14,16,17,18,19,20};
	for(size_t i = 0; i < 6; i++)
	{
		CCSprite* chessman = CCSprite::create(PIC_CHESSMAN1);
		int chessPos = initChessmemA[i];
		chessman->setPosition(getChessPoint(chessPos));
		addChild(chessman, 1, 100+chessPos);
	}
	for(size_t i = 0; i < 6; i++)
	{
		CCSprite* chessman = CCSprite::create(PIC_CHESSMAN2);
		int chessPos = initChessmemB[i];
		chessman->setPosition(getChessPoint(chessPos));
		addChild(chessman, 1, 100+chessPos);
	}
}

void NetPlayLayer::realMove()
{
	m_chess.move(m_touchChessPos, m_moveToChessPos);
    if(m_chess.isTurnA())
        sendMove(m_touchChessPos, m_moveToChessPos);
	m_movable = false;
    
	CCSprite* chessman = (CCSprite*)getChildByTag(100+m_touchChessPos);
	chessman->setTag(NET_CHESS_BASE+m_moveToChessPos);
	CCPoint pos = getChessPoint(m_moveToChessPos);
	CCSprite* chessSelect = m_chess.isTurnA()? m_chess1Select : m_chess2Select;
	chessSelect->runAction(CCMoveTo::create(0.2f, pos));
	chessman->runAction(CCSequence::create(CCMoveTo::create(0.2f, pos),
                                           CCCallFunc::create(this, callfunc_selector(NetPlayLayer::callbackMoveDone)),
                                           NULL));
}

void NetPlayLayer::callbackMoveDone()
{
	CCSprite* chessSelect = m_chess.isTurnA()? m_chess1Select : m_chess2Select;
	chessSelect->setVisible(false);
	if(m_hideChess)
		m_hideChess->setVisible(true);
	MySound::playSound(SOUND_MOVEDONE_CHESS);
	check();
}

void NetPlayLayer::callbackCheckDone(CCNode* node)
{
	node->removeFromParentAndCleanup(true);
	if(!m_movable)
		m_movable = true;
}

void NetPlayLayer::check()
{
	if(m_chess.check())
	{
		const vector<int>& deads = m_chess.getDeadChessmen();
		for(size_t i = 0; i < deads.size(); i ++)
		{
			//CCLog("dead: %d", deads[i]);
			CCSprite* chessman = (CCSprite*)getChildByTag(100+deads[i]);
			chessman->runAction(CCSequence::create(CCBlink::create(0.5f, 2),
                                                   CCCallFuncN::create(this, callfuncN_selector(NetPlayLayer::callbackCheckDone)),
                                                   NULL));
		}
		runAction(CCSequence::create(CCDelayTime::create(0.5f),
                                     CCCallFunc::create(this, callfunc_selector(NetPlayLayer::checkFinish)),
                                     NULL));
	}
	else
	{
		nextTurn();
	}
}

void NetPlayLayer::checkFinish()
{
	if(!m_chess.checkFinish())
	{
		nextTurn();
		return;
	}
    
    //sendGameOver(m_chess.isTurnA() ? YES : NO);
	m_movable = false;
	Game_Finish_State state = GAME_FINISH_A_WIN;
	if(!m_chess.isTurnA())
		state = GAME_FINISH_B_WIN;
	MySound::stopMusic();
    loseScore = CCGetInt("loseScore", 0);
    winScore = CCGetInt("winScore", 0);
	if(!m_chess.isTurnA())
    {
        loseScore++;
        CCSetInt("loseScore", loseScore);
		MySound::playSound(SOUND_LOSE);
    }
	else
    {
        winScore++;
        CCSetInt("winScore", winScore);
		MySound::playSound(SOUND_WIN);
    }
    commitScore();
    const char* msg = m_chess.isTurnA() ? "你赢了!" : "你输了~";
	GameOverLayer* gameOver = GameOverLayer::create(msg);
	addChild(gameOver, 1000, 444);
	setStopVisible(false);
    m_labelTurnA->setVisible(false);
    m_labelTurnB->setVisible(false);
}

void NetPlayLayer::commitScore()
{
    CCLog("winScore:%d",winScore);
    [[GCHelper sharedInstance] commitScore:@"connectWin" value:winScore];
    [[GCHelper sharedInstance] commitAchievement:@"winConnect50" value:2];
    [[GCHelper sharedInstance] commitAchievement:@"winConnect10" value:10];
    [[GCHelper sharedInstance] commitAchievement:@"winConnect1" value:100];
    /*
    if(winScore >= 50)
    {
        [[GCHelper sharedInstance] commitAchievement:@"winConnect50" value:100];
    }
    else if(winScore >= 10)
    {
        [[GCHelper sharedInstance] commitAchievement:@"winConnect10" value:100];
    }
    else if(winScore >= 1)
    {
        [[GCHelper sharedInstance] commitAchievement:@"winConnect1" value:100];
    }*/
}

void NetPlayLayer::nextTurn()
{
    m_touchChessPos = -1;
    m_moveToChessPos = -1;
	m_chess.nextTurn();
	m_labelTurnA->setVisible(m_chess.isTurnA());
	m_labelTurnB->setVisible(!m_chess.isTurnA());
    m_movable = true;
}

void NetPlayLayer::selectChessman(int chessPos)
{
	CCSprite* chessSelect = m_chess.isTurnA()? m_chess1Select : m_chess2Select;
	ChessPointState moveToState = m_chess.getChessPosState(chessPos);
	if( (m_chess.isTurnA() && moveToState == CHESS_POINT_A) ||
       (!m_chess.isTurnA() && moveToState == CHESS_POINT_B) )
	{
		if(m_touchChessPos!=-1)
		{
			CCSprite* chessman = (CCSprite*)getChildByTag(100+m_touchChessPos);
			chessman->setVisible(true);
		}
		m_hideChess = (CCSprite*)getChildByTag(100+chessPos);
		m_hideChess->setVisible(false);
        
		//CCLog("select chess: %d",chessPos);
		m_touchChessPos = chessPos;
		chessSelect->setPosition(getChessPoint(m_touchChessPos));
		chessSelect->setVisible(true);
		m_selected = true;
		MySound::playSound(SOUND_SELECT_CHESS);
	}
	else
	{
		if(m_touchChessPos!=-1)
		{
			m_hideChess = (CCSprite*)getChildByTag(100+m_touchChessPos);
			m_hideChess->setVisible(true);
		}
		//CCLog("cant select");
		m_touchChessPos = -1;
		chessSelect->setVisible(false);
        
		m_selected = false;
	}
}

void NetPlayLayer::unselectChessman()
{
	CCSprite* chessSelect = m_chess.isTurnA()? m_chess1Select : m_chess2Select;
	//CCLog("unselect chess: %d",m_touchChessPos);
	if(m_touchChessPos != -1)
	{
		if(m_hideChess)
			m_hideChess->setVisible(true);
		MySound::playSound(SOUND_SELECT_CHESS);
	}
	m_touchChessPos = -1;
	chessSelect->setVisible(false);
    
	m_selected = false;
}

void NetPlayLayer::moveChessman()
{
	ChessPointState moveToState = m_chess.getChessPosState(m_moveToChessPos);
	if(moveToState == CHESS_POINT_NONE)
	{
		if(m_chess.isConnect(m_touchChessPos, m_moveToChessPos))
		{
			//CCLog("move chess: %d to %d", m_touchChessPos, m_moveToChessPos);
			realMove();
			m_touchChessPos = -1;
			m_moveToChessPos = -1;
			m_selected = false;
		}
		else
		{
			//CCLog("can't move");
			return;
		}
	}
	else if(moveToState == CHESS_POINT_A)
	{
		if(m_chess.isTurnA())
			selectChessman(m_moveToChessPos);
		else
		{
			//CCLog("cant choose B");
		}
	}
	else// if(moveToState == CHESS_POINT_A)
	{
		if(!m_chess.isTurnA())
			selectChessman(m_moveToChessPos);
		else
		{
			//CCLog("cant choose A");
		}
	}
}

bool NetPlayLayer::getTouchChessPos(const CCPoint& point, int& chessPos)
{
	for(int i = 0; i < 21; i++)
	{
		CCPoint chessPoint = getChessPoint(i);
		if(point.x - chessPoint.x > -40 && point.x - chessPoint.x < 40 &&
           point.y - chessPoint.y > -40 && point.y - chessPoint.y < 40)
		{
			chessPos = i;
			return true;
		}
	}
	return false;
}

//---SpriteClassModel ∞¥º¸-----------------
// ±Õ¸º«º”»Î TouchDispatcher  ,TouchDispatcher“ª∞„∑≈‘⁄onenter ∞Û∂®£¨onexit …æ≥˝
bool NetPlayLayer::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
    if (m_state != kGameStateActive)
        return false;
	if(!m_movable)
		return false;
	if(!m_chess.isTurnA())
		return false;
	CCPoint touchPoint = touch->getLocation();
	int chessPos = 0;
	if(getTouchChessPos(touchPoint, chessPos))
	{
		if(m_selected)
		{
			if(chessPos == m_touchChessPos)
			{
				unselectChessman();
			}
			else
			{
				m_moveToChessPos = chessPos;
				moveChessman();
			}
			return false;
		}
		else
		{
			selectChessman(chessPos);
			return true;
		}
	}
	else
		unselectChessman();
	return false;
}

void NetPlayLayer::ccTouchMoved(CCTouch* touch, CCEvent* event)
{
	//Õœ∂Ø
	if(m_touchChessPos == -1)
		return;
	CCPoint touchPoint = touch->getLocation();
	int chessPos = 0;
	if(getTouchChessPos(touchPoint, chessPos))
	{
		m_moveToChessPos = chessPos;
	}
	else
	{
		m_moveToChessPos = -1;
	}
}

void NetPlayLayer::ccTouchEnded(CCTouch* touch, CCEvent* event)
{
	//Õœ∂Ø
	if(m_touchChessPos == -1)
		return;
	CCPoint touchPoint = touch->getLocation();
	int chessPos = 0;
	if(getTouchChessPos(touchPoint, chessPos))
	{
		m_moveToChessPos = chessPos;
		if(m_moveToChessPos == m_touchChessPos)
		{
			//selectChessman(m_touchChessPos);
		}
		else
		{
			moveChessman();
		}
	}
	else
		unselectChessman();
}

void NetPlayLayer::onEnter()
{
	//JigsawSound::resumeMusic();
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, false);
	CCLayer::onEnter();
}

void NetPlayLayer::onExit()
{
    if(!isCancel)
        matchEnded();
    [m_match release];
    m_match = nil;
    [otherPlayerID release];
    otherPlayerID = nil;
	//MySound::stopAll();
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);  
	CCLayer::onExit();
}

void NetPlayLayer::menuStopCallback(CCObject* pSender)
{
	MySound::playSound(SOUND_SELECT_CHESS);
	StopLayer* stop = StopLayer::create();
	stop->initWithColor(ccc4(0,0,0,125));
	this->addChild(stop, 100);
	setStopVisible(false);
	//SceneManager::goStop(); 
}

void NetPlayLayer::setStopVisible(bool b)
{
	m_stopMenu->setVisible(b);
}

