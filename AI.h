#ifndef __AI_H_
#define __AI_H_

#define RADIUS 3

class AI {
public:
	AI(Game&, Danger&);
	virtual ~AI();

	std::pair<int, bool> getBestMove();
private:
	double getScore(int, int, bool);

	double area(int, int);
	double survival(int, int);

	Game _game;
	Danger _danger;
};

#endif
