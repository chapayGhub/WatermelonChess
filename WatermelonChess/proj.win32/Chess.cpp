#include "Chess.h"
#include <time.h>
#include "cocos2d.h"
USING_NS_CC;


vector<int> Chess::m_around[21];

static const int around0[] = {1,2,3};
static const int around1[] = {0,2,4};
static const int around2[] = {0,1,3,5};
static const int around3[] = {0,2,6};
static const int around4[] = {1,7,8};
static const int around5[] = {2,9,10,11};
static const int around6[] = {3,12,13};
static const int around7[] = {4,8,14};
static const int around8[] = {4,7,9,14};
static const int around9[] = {5,8,10,15};
static const int around10[] = {5,9,11,15};
static const int around11[] = {5,10,12,15};
static const int around12[] = {6,11,13,16};
static const int around13[] = {6,12,16};
static const int around14[] = {7,8,17};
static const int around15[] = {9,10,11,18};
static const int around16[] = {12,13,19};
static const int around17[] = {14,18,20};
static const int around18[] = {15,17,19,20};
static const int around19[] = {16,18,20};
static const int around20[] = {17,18,19};

static void setAroundContent(vector<int>& around, const int* content, int num)
{
	around.clear();
	for(int i = 0; i < num; i++)
	{
		around.push_back(content[i]);
	}
}

void Chess::createAroundMap()
{
	setAroundContent(m_around[0], around0, 3);
	setAroundContent(m_around[1], around1, 3);
	setAroundContent(m_around[2], around2, 4);
	setAroundContent(m_around[3], around3, 3);
	setAroundContent(m_around[4], around4, 3);
	setAroundContent(m_around[5], around5, 4);
	setAroundContent(m_around[6], around6, 3);
	setAroundContent(m_around[7], around7, 3);
	setAroundContent(m_around[8], around8, 4);
	setAroundContent(m_around[9], around9, 4);
	setAroundContent(m_around[10], around10, 4);
	setAroundContent(m_around[11], around11, 4);
	setAroundContent(m_around[12], around12, 4);
	setAroundContent(m_around[13], around13, 3);
	setAroundContent(m_around[14], around14, 3);
	setAroundContent(m_around[15], around15, 4);
	setAroundContent(m_around[16], around16, 3);
	setAroundContent(m_around[17], around17, 3);
	setAroundContent(m_around[18], around18, 4);
	setAroundContent(m_around[19], around19, 3);
	setAroundContent(m_around[20], around20, 3);
}


bool Chess::isConnect(int pos1, int pos2)
{
	if(pos1 < 0 || pos1 > 20 || pos2 < 0 || pos2 > 20)
		return false;
	return findIntInVector(m_around[pos1], pos2);
}

Chess::Chess(void)
{
	Chess::createAroundMap();
	m_turnA = true;
	for(int i = 7; i < 14; i++)
	{
		m_chessboard[i] = CHESS_POINT_NONE;
	}
	m_chessboard[0] = CHESS_POINT_A;
	m_chessboard[1] = CHESS_POINT_A;
	m_chessboard[2] = CHESS_POINT_A;
	m_chessboard[3] = CHESS_POINT_A;
	m_chessboard[4] = CHESS_POINT_A;
	m_chessboard[5] = CHESS_POINT_NONE;
	m_chessboard[6] = CHESS_POINT_A;
	m_chessboard[14] = CHESS_POINT_B;
	m_chessboard[15] = CHESS_POINT_NONE;
	m_chessboard[16] = CHESS_POINT_B;
	m_chessboard[17] = CHESS_POINT_B;
	m_chessboard[18] = CHESS_POINT_B;
	m_chessboard[19] = CHESS_POINT_B;
	m_chessboard[20] = CHESS_POINT_B;
	m_chessmen_A.insert(0);
	m_chessmen_A.insert(1);
	m_chessmen_A.insert(2);
	m_chessmen_A.insert(3);
	m_chessmen_A.insert(4);
	m_chessmen_A.insert(6);
	m_chessmen_B.insert(14);
	m_chessmen_B.insert(16);
	m_chessmen_B.insert(17);
	m_chessmen_B.insert(18);
	m_chessmen_B.insert(19);
	m_chessmen_B.insert(20);
}

Chess::Chess(const Chess& chess)
{
	for(int i = 0; i < 21; i++)
	{
		m_chessboard[i] = chess.m_chessboard[i];
	}
	m_chessmen_A.clear();
	m_chessmen_B.clear();
	for(set<int>::iterator iter = chess.m_chessmen_A.begin();
		iter != chess.m_chessmen_A.end();
		iter++)
	{
		m_chessmen_A.insert(*iter);
	}
	for(set<int>::iterator iter = chess.m_chessmen_B.begin();
		iter != chess.m_chessmen_B.end();
		iter++)
	{
		m_chessmen_B.insert(*iter);
	}
	m_turnA = chess.m_turnA;
}

Chess& Chess::operator= (const Chess& other)
{
	if(this == &other)
		return *this;
	
	for(int i = 0; i < 21; i++)
	{
		m_chessboard[i] = other.m_chessboard[i];
	}
	m_chessmen_A.clear();
	m_chessmen_B.clear();
	for(set<int>::iterator iter = other.m_chessmen_A.begin();
		iter != other.m_chessmen_A.end();
		iter++)
	{
		m_chessmen_A.insert(*iter);
	}
	for(set<int>::iterator iter = other.m_chessmen_B.begin();
		iter != other.m_chessmen_B.end();
		iter++)
	{
		m_chessmen_B.insert(*iter);
	}
	m_turnA = other.m_turnA;

	return *this;
}

Chess::~Chess(void)
{
}

void Chess::move(int pos1, int pos2)
{
	if(pos1 < 0 || pos1 > 20 || pos2 < 0 || pos2 > 20)
		return;
	if(m_chessboard[pos1] == CHESS_POINT_NONE ||
		m_chessboard[pos2] != CHESS_POINT_NONE)
		return;
	ChessPointState type = m_chessboard[pos1];
	if(type == CHESS_POINT_A)
	{
		set<int>::iterator iter = m_chessmen_A.find(pos1);
		m_chessmen_A.erase(iter);
		m_chessmen_A.insert(pos2);
	}
	else if(type == CHESS_POINT_B)
	{
		set<int>::iterator iter = m_chessmen_B.find(pos1);
		m_chessmen_B.erase(iter);
		m_chessmen_B.insert(pos2);
	}
	m_chessboard[pos2] = m_chessboard[pos1];
	m_chessboard[pos1] = CHESS_POINT_NONE;
}

void Chess::revive(const vector<int>& deads, bool isDead_A)
{
	if(isDead_A)
	{
		for(size_t i = 0; i < deads.size(); i++)
		{
			m_chessmen_A.insert(deads[i]);
			m_chessboard[deads[i]] = CHESS_POINT_A;
		}
	}
	else
	{
		for(size_t i = 0; i < deads.size(); i++)
		{
			m_chessmen_B.insert(deads[i]);
			m_chessboard[deads[i]] = CHESS_POINT_B;
		}
	}
}

void Chess::revive()
{
	if(m_deadA)
	{
		for(size_t i = 0; i < m_deadChessmen.size(); i++)
		{
			m_chessmen_A.insert(m_deadChessmen[i]);
			m_chessboard[m_deadChessmen[i]] = CHESS_POINT_A;
		}
	}
	else
	{
		for(size_t i = 0; i < m_deadChessmen.size(); i++)
		{
			m_chessmen_B.insert(m_deadChessmen[i]);
			m_chessboard[m_deadChessmen[i]] = CHESS_POINT_B;
		}
	}
	m_deadChessmen.clear();
}

bool findIntInVector(vector<int>& chessmen, int chessman)
{
	vector<int>::iterator result = find(chessmen.begin(), chessmen.end(), chessman);
	return result != chessmen.end();
}

bool Chess::hasBreath(int chessman, vector<int>& together)
{
	ChessPointState type = m_chessboard[chessman];
	bool breath = false;
	for(size_t i = 0; i < m_around[chessman].size(); i++)
	{
		ChessPointState aroundType = m_chessboard[m_around[chessman][i]];
		if(aroundType == CHESS_POINT_NONE)
			return true;
		else if(aroundType == type)
		{
			if(findIntInVector(together, m_around[chessman][i]))
				continue;
			together.push_back(m_around[chessman][i]);
			if(hasBreath(m_around[chessman][i], together))
				return true;
		}
	}
	return false;
}

bool Chess::check()
{
	m_deadChessmen.clear();
	if(m_turnA)
	{
		for(set<int>::iterator iter = m_chessmen_B.begin(); iter!=m_chessmen_B.end(); iter++)
		{
			if(findIntInVector(m_deadChessmen, *iter))
				continue;

			vector<int> chessmen;
			chessmen.push_back(*iter);
			if(!hasBreath(*iter, chessmen))
			{
				for(size_t j = 0; j <chessmen.size(); j++)
					m_deadChessmen.push_back(chessmen[j]);
			}
		}
	}
	else
	{
		for(set<int>::iterator iter = m_chessmen_A.begin(); iter!=m_chessmen_A.end(); iter++)
		{
			if(findIntInVector(m_deadChessmen, *iter))
				continue;

			vector<int> chessmen;
			chessmen.push_back(*iter);
			if(!hasBreath(*iter, chessmen))
			{
				for(size_t j = 0; j <chessmen.size(); j++)
					m_deadChessmen.push_back(chessmen[j]);
			}
		}
	}
	if(m_deadChessmen.size() > 0)
	{
		for(size_t i = 0; i < m_deadChessmen.size(); i++)
		{
			m_chessboard[m_deadChessmen[i]] = CHESS_POINT_NONE;
			if(m_turnA)
			{
				set<int>::iterator iter = m_chessmen_B.find(m_deadChessmen[i]);
				m_chessmen_B.erase(iter);
			}
			else
			{
				set<int>::iterator iter = m_chessmen_A.find(m_deadChessmen[i]);
				m_chessmen_A.erase(iter);
			}
		}
		m_deadA = !m_turnA;
		return true;
	}
	return false;
}

bool Chess::checkFinish()
{
	if(m_turnA)
	{
		if(m_chessmen_B.size() <=2)
			return true;
	}
	else
	{
		if(m_chessmen_A.size() <=2)
			return true;
	}
	return false;
}

//=========================AI part================================

bool Chess::getAImove(int level, int& fromPos, int& toPos)
{
	ChessAI chessAI(this, level);
	chessAI.getAImove(fromPos, toPos);
	if(fromPos < 0 || toPos < 0)
		return false;
	return true;
}

int Chess::getBoardValue(ChessPointState chessboard[21], const set<int>& chessmenA, const set<int>& chessmenB, bool turnA)
{
	if(chessmenB.size()<=2)
		return 3000;
	else if(chessmenA.size()<=2)
		return -3000;
	int k = 1;//turnA ? 1 : -1;
	int value = ((int)chessmenA.size() - (int)chessmenB.size()) * 200 * k; //every chessman 200
	if(value>0)
		value += rand()%50;
	else if(value < 0)
		value -= rand()%50;
	else
		value = rand()%50 - 25;
	return value;

	//A breaths...
	set<int> breathsA;
	for(set<int>::iterator iter = chessmenA.begin(); iter != chessmenA.end(); iter++)
	{
		for(size_t i = 0; i < m_around[*iter].size(); i++)
		{
			if(chessboard[m_around[*iter][i]] == CHESS_POINT_NONE)
			{
				if(breathsA.find(m_around[*iter][i]) == breathsA.end())
					breathsA.insert(m_around[*iter][i]);
			}
		}
	}
	//B breaths...
	set<int> breathsB;
	for(set<int>::iterator iter = chessmenB.begin(); iter != chessmenB.end(); iter++)
	{
		for(size_t i = 0; i < m_around[*iter].size(); i++)
		{
			if(chessboard[m_around[*iter][i]] == CHESS_POINT_NONE)
			{
				if(breathsB.find(m_around[*iter][i]) == breathsB.end())
					breathsB.insert(m_around[*iter][i]);
			}
		}
	}
	value += ((int)breathsA.size()- (int)breathsB.size()) * 50; //every breath 50
	return value;
}

void Chess::getAllMovable(vector<pair<int,int> >& allMovable)
{	
	if(m_turnA)
	{
		for(set<int>::iterator iter = m_chessmen_A.begin(); iter!=m_chessmen_A.end(); iter++)
		{
			for(size_t i = 0; i < m_around[*iter].size(); i++)
			{
				if(m_chessboard[m_around[*iter][i]] == CHESS_POINT_NONE)
				{
					allMovable.push_back(make_pair(*iter, m_around[*iter][i]));
				}
			}
		}
	}
	else
	{
		for(set<int>::iterator iter = m_chessmen_B.begin(); iter!=m_chessmen_B.end(); iter++)
		{
			for(size_t i = 0; i < m_around[*iter].size(); i++)
			{
				if(m_chessboard[m_around[*iter][i]] == CHESS_POINT_NONE)
				{
					allMovable.push_back(make_pair(*iter, m_around[*iter][i]));
				}
			}
		}
	}
}

//====================Chess AI Class=====================
static int c = 0;
clock_t t1;
clock_t t2;
clock_t t3;
clock_t t4;
ChessAI::ChessAI(Chess* chess, int level)
	:m_level(level), m_fromPos(-1), m_toPos(-1)
{
	m_board = new Chess(*chess);
	//Chess* board = new Chess(*chess);
	//m_boards.push_back(board);
	int alpha = -10000;
	int beta = 10000;
	c = 0;
	
	/*
    for(int i = 0; i < m_level; i++)
	{
		Chess* board = new Chess();
		m_boards.push_back(board);
	}*/
	
    //DWORD  dw = GetTickCount();//timeGetTime(); time(NULL);
	t1 = 0;
	t2 = 0;
	t3 = 0;
	t4 = 0;
	clock_t cc = clock();

	alpha_beta(0, alpha, beta);
	
	CCLog("calulator times: %d", c);
	cc = clock() - cc;
	CCLog("all time: %f", (float)cc/1000);
	if(t1!=0) CCLog("t1: %f", (float)t1/1000);
	if(t2!=0) CCLog("t2: %f", (float)t2/1000);
	if(t3!=0) CCLog("t3: %f", (float)t3/1000);
	if(t4!=0) CCLog("t4: %f", (float)t4/1000);
}

ChessAI::~ChessAI()
{
	delete m_board;
	/*
	for(size_t i = 0; i < m_boards.size(); i++)
	{
		delete m_boards[i];
	}*/
}

void ChessAI::getAImove(int& fromPos, int& toPos)
{
	fromPos = m_fromPos;
	toPos = m_toPos;
}

int ChessAI::alpha_beta(int depth, int& alpha, int& beta)
{
	if(depth == m_level)
	{
		c++;
		//m_boards[depth].nextTurn();
		//int value = m_boards[depth]->getBoardValue();
		m_board->nextTurn();
		int value = m_board->getBoardValue();
		m_board->nextTurn();
		return value;
	}
	vector<pair<int,int> > allMovable;
	allMovable.clear();
	m_board->getAllMovable(allMovable);
	//m_boards[depth]->getAllMovable(allMovable);
	//if(m_boards[depth]->isTurnA())
	if(m_board->isTurnA())
	{
		//CCLog("A depth:%d", depth);
		for(size_t i = 0; i < allMovable.size(); i++)
		{
			clock_t c3 = clock();
			//*m_boards[depth+1] = *m_boards[depth];
			c3 = clock() - c3;
			t3+=c3;
			clock_t c4 = clock();
			pair<int,int> move = allMovable[i];
			//m_boards[depth+1]->move(move.first, move.second);
			//m_boards[depth+1]->check();
			m_board->move(move.first, move.second);
			m_board->check();
			vector<int> deads = m_board->getDeadChessmen();
			c4 = clock() - c4;
			t4+=c4;
			//if(depth == 0 && m_boards[depth+1]->checkFinish())
			if(depth == 0 && m_board->checkFinish())
			{
				m_fromPos = move.first;
				m_toPos = move.second;
				return 3000;
			}
			//m_boards[depth+1]->nextTurn();
			m_board->nextTurn();
			int a = alpha;
			int b = beta;
			int value = alpha_beta(depth+1, alpha, beta);
			alpha = a;
			beta = b;
			m_board->move(move.second, move.first);
			m_board->revive(deads, false);
			m_board->nextTurn();
			//CCLog("A move:%d->%d, value:%d", move.first,move.second, value);
			if(value > alpha)
			{
				//CCLog("A biggerbiggerbigger alpha: %d", value);
				//CCLog("A better move:%d->%d", move.first,move.second);
				alpha = value;
				if(depth == 0)
				{
					m_fromPos = move.first;
					m_toPos = move.second;
				}
			}
			if(alpha >= beta || value == 3000)
			{
				//CCLog("alpha>=beta, %d > %d", alpha, beta);
				break;
			}
		}
		//CCLog("A return alpha: %d", alpha);
		return alpha;
	}
	else
	{
		//CCLog("B depth:%d", depth);
		for(size_t i = 0; i < allMovable.size(); i++)
		{
			clock_t c3 = clock();
			//*m_boards[depth+1] = *m_boards[depth];
			c3 = clock() - c3;
			t3+=c3;
			clock_t c4 = clock();
			pair<int,int> move = allMovable[i];
			//m_boards[depth+1]->move(move.first, move.second);
			//m_boards[depth+1]->check();
			m_board->move(move.first, move.second);
			m_board->check();
			vector<int> deads = m_board->getDeadChessmen();
			c4 = clock() - c4;
			t4+=c4;
			//if(depth == 0 && m_boards[depth+1]->checkFinish())
			if(depth == 0 && m_board->checkFinish())
			{
				m_fromPos = move.first;
				m_toPos = move.second;
				return -3000;
			}
			//m_boards[depth+1]->nextTurn();
			m_board->nextTurn();
			int a = alpha;
			int b = beta;
			int value = alpha_beta(depth+1, alpha, beta);
			alpha = a;
			beta = b;
			m_board->move(move.second, move.first);
			m_board->revive(deads, true);
			m_board->nextTurn();
			//CCLog("B move:%d->%d, value:%d", move.first,move.second, value);
			if(value < beta)
			{
				//CCLog("B smallersmallersmaller beta: %d", value);
				//CCLog("B better move:%d->%d", move.first,move.second);
				beta = value;
				if(depth == 0)
				{
					m_fromPos = move.first;
					m_toPos = move.second;
				}
			}
			if(alpha >= beta || value == -3000)
			{
				//CCLog("alpha>=beta, %d > %d", alpha, beta);
				break;
			}
		}
		//CCLog("B return beta: %d", beta);
		return beta;
	}
}