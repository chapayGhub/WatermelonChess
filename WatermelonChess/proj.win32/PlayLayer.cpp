#include "PlayLayer.h"
#include "SceneManager.h"
#include "StopLayer.h"
#include "MySound.h"
#include "Basic.h"
#include "GameOverLayer.h"
#include <pthread.h>
//#pragma comment(lib, "pthreadVC2.lib")  //必须加上这句

#include "cocos-ext.h"
USING_NS_CC_EXT;

bool PlayLayer::init()
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
	m_movable = true;
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
	m_labelTurnA = CCLabelTTF::create("Blue turns", "Marker Felt", 50);
	m_labelTurnA->setAnchorPoint(ccp(0.5,0.5));
	m_labelTurnA->setPosition(ccp(s.width*0.5,100));
	m_labelTurnA->setVisible(false);
	addChild(m_labelTurnA);
	m_labelTurnB = CCLabelTTF::create("Pink turns", "Marker Felt", 50);
	m_labelTurnB->setAnchorPoint(ccp(0.5,0.5));
	m_labelTurnB->setPosition(ccp(s.width*0.5,s.height-100));
	m_labelTurnB->setVisible(false);
	addChild(m_labelTurnB);

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
	
	m_chess.nextTurn();
	nextTurn();

	//MySound::playSound(SOUND_START);
	return true;
}

void PlayLayer::setLevel(int a, int b)
{
	m_levelA = a;
	m_levelB = b;
	if(a>0)
		m_labelTurnA->setString("Blue Thinking...");
	if(b>0)
		m_labelTurnB->setString("Pink Thinking...");
}

void PlayLayer::back(cocos2d::CCObject *pSender)  
{  
	SceneManager::goMenu();  
	//back就是回主菜单，这里调用了人见人爱 花见花开的静态函数 goMenu  
}

PlayLayer::~PlayLayer()
{

}

void PlayLayer::initBoard()
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

void PlayLayer::realMove()
{
	m_chess.move(m_touchChessPos, m_moveToChessPos);
	m_movable = false;

	CCSprite* chessman = (CCSprite*)getChildByTag(100+m_touchChessPos);
	chessman->setTag(CHESS_BASE+m_moveToChessPos);
	CCPoint pos = getChessPoint(m_moveToChessPos);
	CCSprite* chessSelect = m_chess.isTurnA()? m_chess1Select : m_chess2Select;
	chessSelect->runAction(CCMoveTo::create(0.2f, pos));
	chessman->runAction(CCSequence::create(CCMoveTo::create(0.2f, pos), 
		CCCallFunc::create(this, callfunc_selector(PlayLayer::callbackMoveDone)),
		NULL));
}

void PlayLayer::callbackMoveDone()
{
	CCSprite* chessSelect = m_chess.isTurnA()? m_chess1Select : m_chess2Select;
	chessSelect->setVisible(false);
	if(m_hideChess)
		m_hideChess->setVisible(true);
	MySound::playSound(SOUND_MOVEDONE_CHESS);
	check();
}

void PlayLayer::callbackCheckDone(CCNode* node)
{
	node->removeFromParentAndCleanup(true);
	if(!m_movable)
		m_movable = true;
}

void PlayLayer::check()
{
	if(m_chess.check())
	{
		const vector<int>& deads = m_chess.getDeadChessmen();
		for(size_t i = 0; i < deads.size(); i ++)
		{
			//CCLog("dead: %d", deads[i]);
			CCSprite* chessman = (CCSprite*)getChildByTag(100+deads[i]);
			chessman->runAction(CCSequence::create(CCBlink::create(0.5f, 2),
				CCCallFuncN::create(this, callfuncN_selector(PlayLayer::callbackCheckDone)),
				NULL));
		}
		runAction(CCSequence::create(CCDelayTime::create(0.5f),
				CCCallFunc::create(this, callfunc_selector(PlayLayer::checkFinish)),
				NULL));
	}
	else
	{
		nextTurn();
	}
}

void PlayLayer::checkFinish()
{
	if(!m_chess.checkFinish())
	{
		nextTurn();
		return;
	}

	m_movable = false;
	Game_Finish_State state = GAME_FINISH_A_WIN;
	if(!m_chess.isTurnA())
		state = GAME_FINISH_B_WIN;
	MySound::stopMusic();
	if(!m_chess.isTurnA() && m_levelB > 0)
		MySound::playSound(SOUND_LOSE);
	else
		MySound::playSound(SOUND_WIN);
	GameOverLayer* gameOver = GameOverLayer::create(state);
	addChild(gameOver, 1000);
	setStopVisible(false);
}

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
void* ThreadFunction(void* arg)
{
    pthread_mutex_lock(&mutex);
	srand((unsigned)time(0));
    PlayLayer* playLayer = (PlayLayer*)arg;
	playLayer->thinkAndMove();
    pthread_mutex_unlock(&mutex);
    return NULL;
}

void CreateThinkThread(PlayLayer* playLayer)
{
	//void *status = NULL;
    pthread_t thread;
    pthread_create(&thread, NULL, &ThreadFunction, playLayer);
	//pthread_join(thread, &status);
}

void PlayLayer::thinkAndMove()
{	
	if(m_chess.isTurnA() && m_levelA > 0)
	{
		//CCLog("auto A...");
		if(m_chess.getAImove(m_levelA, m_touchChessPos, m_moveToChessPos))
		{
			selectChessman(m_touchChessPos);
			moveChessman();
		}
	}
	else if(!m_chess.isTurnA() && m_levelB > 0)
	{
		//CCLog("auto B...");
		if(m_chess.getAImove(m_levelB, m_touchChessPos, m_moveToChessPos))
		{			
			CCLog("from %d, to %d", m_touchChessPos, m_moveToChessPos);
			selectChessman(m_touchChessPos);
			moveChessman();
		}
	}
}

void PlayLayer::nextTurn()
{
	m_chess.nextTurn();
	m_labelTurnA->setVisible(m_chess.isTurnA());
	m_labelTurnB->setVisible(!m_chess.isTurnA());

	if((m_chess.isTurnA() && m_levelA > 0) ||
		(!m_chess.isTurnA() && m_levelB > 0) )
	{
		CreateThinkThread(this);
	}
	else
		m_movable = true;
}

void PlayLayer::selectChessman(int chessPos)
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

void PlayLayer::unselectChessman()
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

void PlayLayer::moveChessman()
{
	CCSprite* chessSelect = m_chess.isTurnA()? m_chess1Select : m_chess2Select;
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

bool PlayLayer::getTouchChessPos(const CCPoint& point, int& chessPos)
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

//---SpriteClassModel 按键-----------------
// 别忘记加入 TouchDispatcher  ,TouchDispatcher一般放在onenter 绑定，onexit 删除
bool PlayLayer::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
	if(!m_movable)
		return false;
	if(m_chess.isTurnA() && m_levelA > 0)
		return false;
	else if(!m_chess.isTurnA() && m_levelB > 0)
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

void PlayLayer::ccTouchMoved(CCTouch* touch, CCEvent* event)
{   
	//拖动
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

void PlayLayer::ccTouchEnded(CCTouch* touch, CCEvent* event)
{	
	//拖动
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

void PlayLayer::onEnter()
{   
	//JigsawSound::resumeMusic();
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, false);
	CCLayer::onEnter();
}

void PlayLayer::onExit()
{   
	//MySound::stopAll();
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);  
	CCLayer::onExit();
}

void PlayLayer::menuStopCallback(CCObject* pSender)
{
	MySound::playSound(SOUND_MENU);
	StopLayer* stop = StopLayer::create();
	stop->initWithColor(ccc4(0,0,0,125));
	this->addChild(stop, 100);
	setStopVisible(false);
	//SceneManager::goStop(); 
}

void PlayLayer::setStopVisible(bool b)
{
	m_stopMenu->setVisible(b);
}
