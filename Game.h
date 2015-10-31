#ifndef __GAME_H__
#define __GAME_H__

#define WALL_POSITION 15
#define MaxS (37*37*4)

class Game {
public:
	Game();
	Game(int N, int M);
	virtual ~Game();

	void readGameStateFromServerInput(int *);
	void prettyPrint();
//private:

	bool isWall(int x, int y);

	int getFlameDuration(int x, int y);

	int getExplosionTime(int x, int y);

	int _currentMove;
	int _aggresiveModeStart;
	int _maxMove;
	int _N, _M;
	int _board[MaxS];
};

#endif
