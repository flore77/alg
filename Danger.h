#ifndef __DANGER_H_
#define __DANGER_H_

#include "Game.h"

#define MaxDim 37
#define MaxDanger 1000

class Danger {
public:
	Danger(Game&);
	virtual ~Danger();

	void calculateDanger();
	int getDanger(int, int);

	void calculateEstimatedExplosionTime();
	int getEstimatedExplosionTime(int, int);
private:
  Game _game;
	int _N, _M;
	int _dangerMatrix[MaxDim][MaxDim];
};

#endif
