#include "AI.h"

AI::AI(Game& game, Danger& danger) : _game(game), _danger(danger) {}

AI::~AI() {}

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

double AI::area(int x, int y) {
	int width = _game.getM() / 4;
	int height = _game.getN() / 4;

	int ox = _opPosition.first;
	int oy = _opPosition.second;

	int countMyBlocks = 0, countOpBlocks = 0;

	for (int dy = height; dy <= height; ++dy)
		for (int dx = width; dx <= width; ++dx)
			if (0 <= x + dx && x + dx < _game.getM() &&
				  0 <= y + dy && y + dy < _game.getN())
					++countMyBlocks;

	for (int dy = height; dy <= height; ++dy)
		for (int dx = width; dx <= width; ++dx)
			if (0 <= ox + dx && ox + dx < _game.getM() &&
				  0 <= oy + dy && oy + dy < _game.getN())
					++countOpBlocks;

		return (double)countMyBlocks / countOpBlocks;
}

double AI::survival(int x, int y) {
  int xStart = x - RADIUS + 1;
  int yStart = y - RADIUS + 1;
  int sum = 0;

  _danger.calculateEstimatedExplosionTime();

  for (int i = ystart; i < y + RADIUS; i++) {
    for (int j = xStart; j < x + RADIUS; j++) {
      if (i < 0 || j < 0 || i >= _game._N || j >= _game._M) {
        continue;
      }

      sum += 11 - _danger.getEstimatedExplosionTime(j, i);
    }
  }

  return (double)sums / RADIUS * RADIUS;
}
