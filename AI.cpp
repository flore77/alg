#include "AI.h"

AI::AI(Game& game, Danger& danger) : _game(game), _danger(danger) {

}

AI::~AI() {

}

std::pair<int, bool> getBestMove() {
	double score;
	double bestScore = 0;
	std::pair<int, bool> returnValue;

	int column = _game._myPosition.first;
	int line = _game._myPosition.second;

	if (line + 1 < _game.getN() && _danger.getDanger(line + 1, column) != MaxDanger) {
		score = getScore(line + 1, column, false);
		if (score > bestScore) {
			bestScore = score;
			returnValue.first = RIGHT; returnValue.second = false;
		}
		score = getScore(line + 1, column, true);
 		if (score > bestScore) {
			bestScore = score;
			returnValue.first = RIGHT; returnValue.second = true;
		}
	}
	if (line - 1 >= 0 && _danger.getDanger(line - 1, column) != MaxDanger) {
		score = getScore(line - 1, column, false);
    if (score > bestScore) {
			bestScore = score;
			returnValue.first = LEFT; returnValue.second = false;
		}
		score = getScore(line - 1, column, true);
    if (score > bestScore) {
			bestScore = score;
			returnValue.first = LEFT; returnValue.second = true;
		}
	}
	if (column + 1 < _game.getM() && _danger.getDanger(line, column + 1) != MaxDanger)  {
		score = getScore(line, column + 1, false);
    if (score > bestScore) {
			bestScore = score;
			returnValue.first = DOWN; returnValue.second = false;
		}
		score = getScore(line, column + 1, true);
    if (score > bestScore) {
			bestScore = score;
			returnValue.first = DOWN; returnValue.second = true;
		}
	}
	if (column - 1 >= 0 && _danger.getDanger(line, column - 1) != MaxDanger) {
		score = getScore(line, column - 1, false);
    if (score > bestScore) {
			bestScore = score;
			returnValue.first = UP; returnValue.second = false;
		}
		score = getScore(line, column - 1, true);
    if (score > bestScore) {
			bestScore = score;
			returnValue.first = UP; returnValue.second = true;
		}
	}

	if (_danger.getDanger(line, column) != MaxDanger) {
		score = getScore(line, column, false);
		if (score > bestScore) {
			bestScore = score;
			returnValue.first = STAY; returnValue.second = false;
		}
		score = getScore(line, column, true);
		if (score > bestScore) {
			bestScore = score;
			returnValue.first = STAY; returnValue.second = true;
		}
	}

	return returnValue;
}
