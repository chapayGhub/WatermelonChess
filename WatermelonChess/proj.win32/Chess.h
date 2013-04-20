#ifndef __CHESS_H__
#define __CHESS_H__

#include <set>
#include <vector>
#include <algorithm>
using namespace std;

struct Chess_Point
{
	double x;
	double y;
};

/*
			20
		17		19
			18

  14		15		  16
	
7	8	9	10	11	12	13
					
  4			5		  6

			2
		1		3
			0	
*/
const Chess_Point s_chessmenPosition[21] = {
	{320, 34.5},
	{175.5, 70.5}, {320, 119.5}, {464.5, 70.5},
	{72.5, 175.5}, {320, 201.5}, {567.5, 175.5},
	{33.5,320}, {119.5, 320}, {203.5, 320}, {320, 320}, {436.5, 320}, {520.5, 320}, {606.5, 320},
	{72.5, 464.5}, {320, 438.5}, {567.5, 464.5},
	{175.5, 569.5}, {320, 525.2}, {464.5, 569.5},
	{320, 605.5}
};

enum ChessPointState
{
	CHESS_POINT_NONE,
	CHESS_POINT_A,
	CHESS_POINT_B
};

class Chess
{
public:
	Chess(void);
	~Chess(void);
	Chess(const Chess& chess);
	Chess& operator= (const Chess& other);

public:
	const set<int>& getChessmen_A() { return m_chessmen_A; }
	const set<int>& getChessmen_B() { return m_chessmen_B; }
	const vector<int>& getDeadChessmen() { return m_deadChessmen; }
	void revive();
	void revive(const vector<int>& deads, bool isDead_A);
	ChessPointState getChessPosState(int chessPos) { return m_chessboard[chessPos]; }
	bool isConnect(int pos1, int pos2);
	bool isTurnA() { return m_turnA; }
	void nextTurn() { m_turnA = !m_turnA; }
	bool check();
	bool checkFinish();
	void move(int pos1, int pos2);
private:
	bool hasBreath(int chessman, vector<int>& together);
private:
	static void createAroundMap();
    static vector<int> m_around[21];
private:
	bool m_turnA;
	vector<int> m_deadChessmen;
	bool m_deadA;
	ChessPointState m_chessboard[21];
	set<int> m_chessmen_A;
	set<int> m_chessmen_B;

public:
	//AI part...
	void getAllMovable(vector<pair<int,int> >& allMovable);
	bool getAImove(int level, int& fromPos, int& toPos);
	int getBoardValue() {return getBoardValue(m_chessboard, m_chessmen_A, m_chessmen_B, m_turnA); }
	static int getBoardValue(ChessPointState chessboard[21],
		const set<int>& chessmenA, const set<int>& chessmenB, bool turnA);
};

class ChessAI
{
public:
	ChessAI(Chess* chess, int level);
	~ChessAI();

public:
	void getAImove(int& fromPos, int& toPos);
	int alpha_beta(int depth, int& alpha, int& beta);

private:
	const int m_level;
	int m_fromPos;
	int m_toPos;
	Chess* m_board;
	//vector<Chess*> m_boards;
};

extern bool findIntInVector(vector<int>& chessmen, int chessman);

#endif