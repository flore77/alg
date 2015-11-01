#include <iostream>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include "Game.h"


Game::Game() {
	srand(time(NULL));
}

Game::Game(int N, int M) : _N(N), _M(M) { }

Game::~Game() { }

void Game::readId(int * buffer) {
	_myId = *buffer;
}

void Game::readHeader(int * bytestream) {
	int * iter = bytestream;

	_currentMove = *iter++;
	_aggresiveModeStart = *iter++;
	_maxMove = *iter++;
	_N = *iter++;
	_M = *iter++;
}

void Game::readMatrix(int * bytestream) {
	memcpy(_board, bytestream, _N * _M * sizeof(int));
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

bool Game::isValidMove(int x, int y) {
  return 0 <= x && x < _M &&
         0 <= y && y < _N &&
         !isWall(x, y);
}

std::pair<int, int> Game::nextMove() {
  std::queue<std::pair<int, int> > q;
  int mx[NR_MOVES] = MX;
  int my[NR_MOVES] = MY;
  int parents[_N * _M];
  bool visited[_N][_M];
  int x, y;

  for (int i = 0; i < _N; i++) {
    for (int j = 0; j < _M; j++) {
      visited[i][j] = false;
      parents[i * _M + j] = -1;
    }
  }

  q.push(_myPosition);
  visited[_myPosition.second][_myPosition.first] = true;

  while (!q.empty()) {
    std::pair<int, int> current = q.front();
    q.pop();

    if (current.first == _opPosition.first &&
        current.second == _opPosition.second) {

        int index = current.second * _M + current.first;

        while (parents[index] != _myPosition.second * _M + _myPosition.first) {
          index = parents[index];
        }

        return std::make_pair(index / _M, index % _M);
    }

    for (int i = 0; i < NR_MOVES; i++) {
      x = current.first + mx[i];
      y = current.second + my[i];

      if (isValidMove(x, y) && !visited[y][x]) {
        q.push(std::make_pair(x, y));

        visited[y][x] = true;
        parents[y * _M + x] = current.second * _M + current.first;
      }
    }
  }

  return std::make_pair(-1, -1);
}


void Game::makeMove(int * buffer) {
	findOpId();// SCHIMBAAAA
	findPositions();

  std::pair<int, int> p = nextMove();
  std::cout << "Next Move: "  << p.second << " " << p.first << '\n';

	*buffer = _moveCounter;
	++buffer;

	if (rand() % 10 < 3) {
		//*buffer = 1 << 31;
	}


	*buffer = (rand() % 4);

	++_moveCounter;
}

void Game::prettyPrint() {
	static int count = 0;
	std::cout << _currentMove << ' ' << _aggresiveModeStart << ' ' << _maxMove << std::endl;
	std::cout << _N << ' ' << _M << std::endl;
	count++;

	printf("myId = %d\n", _myId);

	if (count == 1) return;
	for (int i = 0; i < _N; ++i) {
		for (int j = 0; j < _M; ++j) {
			if (isWall(j, i)) {
				std::cout << 'W';
			}
		  else if ((_board[i * _M + j] & 0x0000FF) == 0) {
				std::cout << ' ';
			} else if ((_board[i * _M + j] & (1 << (_myId - 1))) != 0) {
				std::cout << 'E';
			} else {
				std::cout << _board[i * _M + j];
			}
		}
		std::cout << '\n';
	}

	std::cout << "myPosition(" << _myPosition.first << ", " << _myPosition.second << ")\n";
	std::cout << "opPosition(" << _opPosition.first << ", " << _opPosition.second << ")\n";
}

void Game::findPositions() {
	for (int i = 0; i < _N; ++i)
		for (int j = 0; j < _M; ++j)
			if (((_board[i * _M + j] & 0x000000ff) & (1 << (_myId - 1))) != 0)
				_myPosition = std::make_pair(j, i);
			else if ((_board[i * _M + j] & 0x000000ff) != 0)
				_opPosition = std::make_pair(j, i);
}

void Game::findOpId() {
	int mask = 0;
	for (int i = 0; i < _N; ++i)
		for (int j = 0; j < _M; ++j)
			mask = mask | (_board[i * _M + j]);

	mask &= 0x000000ff;
	mask ^= (1 << (_myId - 1));
	_opId = mask;
}

int Game::getN() {
	return _N;
}

int Game::getM() {
	return _M;
}
