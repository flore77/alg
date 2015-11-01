#include "Danger.h"

Danger::Danger(Game& game) {
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

void Danger::calculate() {
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
