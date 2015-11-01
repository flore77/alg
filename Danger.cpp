#include <queue>
#include <utility>
#include "Danger.h"

inline int min(int a, int b) {
	return a < b ? a : b;
}

Danger::Danger(Game& game) {0
	int i, j;
	for (i = 0; i < MaxDim; i++) {
		for (j = 0; j < MaxDim; j++) {
			_dangerMatrix[i][j] = 0;
		}
	}
	_game = game;
	_N = _game.getN(); _M = _game.getM();
}

Danger::~Danger() {

}

void Danger::calculateDanger() {
	int i, j, k;
	int flameDuration, explosionTime;
	for (i = 0; i < _N ; i++) { //i is column
		for (j = 0; j < _M; j++) { //j is line
			flameDuration = _game.getFlameDuration(j, i);
			explosionTime = _game.getExplosionTime(j, i);

			if (flameDuration > 1) {
				_dangerMatrix[j][i] = MaxDanger;
			}

			if (explosionTime == 1 || (flameDuration > 0 && explosionTime > 0)) {
			  for (k = 1; k <= 6; k++) {
					if (_game.isWall(j + k, i)) break;
					_dangerMatrix[j + k][i] = MaxDanger;
				}
				for (k = 1; k <= 6; k++) {
					if (_game.isWall(j, i + k)) break;
					_dangerMatrix[j][i + k] = MaxDanger;
				}
				for (k = -1; k >= -6; k--) {
					if (_game.isWall(j - k, i)) break;
					_dangerMatrix[j - k][i] = MaxDanger;
				}
				for (k = -1; k >= -6; k--) {
					if (_game.isWall(j, i - k)) break;
					_dangerMatrix[j][i - k] = MaxDanger;
				} 
			}
		}
	}
}

int Danger::getDanger(int x, int y) {
	return _dangerMatrix[y][x];
}

// complexitate mare!!
void Danger::calculateEstimatedExplosionTime() {
	for (int i = 0; i < _N; ++i) {
		for (int j = 0; j < _M; ++j) {
			_dangerMatrix[i][j] = _game.getExplosionTime(j, i);
		}
	}

	for (int val = 1; val <= 10; ++val) {
		for (int i = 0; i < _N; ++i) {
			for (int j = 0; j < _M; ++j) {
				if (_dangerMatrix[i][j] == val) {
					for (int dy = -6; dy <= 6; ++dy) {
						for (int dx = -6; dx <= 6; ++dx) {
							if (dx == 0 && dy == 0) continue;
			
							_dangerMatrix[i + dy][j + dx] = min(_dangerMatrix[i + dy][j + dx],
							                                    _dangerMatrix[i][j] + 1);
						}
					}
				}
			}
		}
	}
}

int Danger::getEstimatedExplosionTime(int x, int y) {
	return _dangerMatrix[y][x];
}
