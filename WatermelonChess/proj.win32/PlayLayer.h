#ifndef __PLAY_LAYER_H__
#define __PLAY_LAYER_H__   
#include "cocos2d.h" 
USING_NS_CC;

#include "Chess.h" 

#define CHESS_BASE 100

enum Game_Finish_State
{
	GAME_FINISH_A_WIN,
	GAME_FINISH_B_WIN,
	GAME_FINISH_DRAW
};

class PlayLayer : public CCLayer  
{  
public:
	~PlayLayer();
	virtual bool init(); //CCLayer的儿子们都有这玩意儿。别忘了virtual关键字
	CREATE_FUNC(PlayLayer); //千万别忘了这个风情万种的宏

	//重写触屏相关函数----   
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
	bool m_turnA;
	bool m_movable;
	CCSprite* m_chessCircle;
	CCLabelTTF* m_labelTurnA;
	CCLabelTTF* m_labelTurnB;

	Chess m_chess;
	CCMenu* m_stopMenu;

};

#endif

