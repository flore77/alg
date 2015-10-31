#include <iostream>
#include <string.h>
#include "Game.h"

Game::Game() { }

Game::Game(int N, int M) : _N(N), _M(M) { }

Game::~Game() { }

void Game::readGameStateFromServerInput(int * bytestream) {
	int * iter = bytestream;

	_currentMove = *iter++;
	_aggresiveModeStart = *iter++;
	_maxMove = *iter++;
	_N = *iter++;
	_M = *iter++;
	memcpy(_board, iter, _N * _M * sizeof(int));
}

bool Game::isWall(int x, int y) {
	return (_board[y * _M + x] & (1 << WALL_POSITION)) != 0;
}

int Game::getFlameDuration(int x, int y) {
	return (_board[y * _M + x] >> 16) & 0x000000FF;
}

int Game::getExplosionTime(int x, int y) {
	return (_board[y * _M + x] >> 24) & 0x000000FF;
}

void Game::prettyPrint() {
	std::cout << _currentMove << ' ' << _aggresiveModeStart << ' ' << _maxMove << std::endl;
	std::cout << _N << ' ' << _M << std::endl;

	for (int i = 0; i < _N; ++i) {
		for (int j = 0; j < _M; ++j) {
			if (isWall(j, i)) {
				std::cout << 'W';
			} else {
				std::cout << ' ';
			}
			std::cout << ' ';
		}
		std::cout << '\n';
	}
}
