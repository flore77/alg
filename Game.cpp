#include <iostream>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include "Game.h"

int _dangerMatrix[37][37];

Game::Game() {
	srand(time(NULL));
}

Game::Game(int N, int M) : _N(N), _M(M) {}

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
	return ((_board[y * _M + x] & (1 << WALL_POSITION)) != 0) || (getExplosionTime(x, y) != 0);
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
         !isWall(x, y) && getDanger(x, y) != MaxDanger;
}

std::pair<int, std::pair<int, int> > Game::BFS() {
  std::queue<std::pair<int, int> > q;
  int mx[NR_MOVES] = MX;
  int my[NR_MOVES] = MY;
  int parents[_N * _M];
  int distances[_N * _M];
  bool visited[_N][_M];
  int x, y, prod;

  if (_myPosition == _opPosition) {
    return std::make_pair(0, _myPosition);
  }

  for (int i = 0; i < _N; i++) {
    for (int j = 0; j < _M; j++) {
      visited[i][j] = false;
      parents[i * _M + j] = -1;
      distances[i * _M + j] = MAX_DISTANCE;
    }
  }

  q.push(_myPosition);

  visited[_myPosition.second][_myPosition.first] = true;
  distances[_myPosition.second * _M + _myPosition.first] = 0;

  while (!q.empty()) {
    std::pair<int, int> current = q.front();
    q.pop();

    int prod = current.second * _M + current.first;

    if (current.first == _opPosition.first &&
        current.second == _opPosition.second) {

        int index = current.second * _M + current.first;

        while (parents[index] != _myPosition.second * _M + _myPosition.first) {
          index = parents[index];
        }

        return std::make_pair(distances[prod],
                                std::make_pair(index % _M, index / _M));
    }

    for (int i = 0; i < NR_MOVES; i++) {
      x = current.first + mx[i];
      y = current.second + my[i];

      if (!visited[y][x] && isValidMove(x, y)) {
        q.push(std::make_pair(x, y));

        visited[y][x] = true;
        distances[y *_M + x] = distances[prod] + 1;
        parents[y * _M + x] = prod;
      }
    }
  }

  return std::make_pair(-1, std::make_pair(-1, -1));
}


void Game::makeMove(int * buffer) {
  findOpId();
  findPositions();

  /*std::cout << "Next Move: "  << p.first << " " << p.second << '\n';
  std::cout << "Distance: " << distance << "\n";

  std::cout << "myPosition(" << _myPosition.first << ", " << _myPosition.second << ")\n";
  std::cout << "opPosition(" << _opPosition.first << ", " << _opPosition.second << ")\n";*/

  *buffer = _moveCounter;
  ++buffer;

	calculateEstimatedExplosionTime();
  calculateDanger();

  std::pair<int, std::pair<int, int> > bfsResult = BFS();
	std::cout << "BFS out: dist =" << bfsResult.first << "pair(" << bfsResult.second.first << ", " <<bfsResult.first<<")" << std::endl;
  _distOp = bfsResult.first;
  std::pair<int, int> p = bfsResult.second;

  if (_distOp > BARRIER) {
    if (_myPosition.first == p.first) {
      if (_myPosition.second < p.second) {
        *buffer = DOWN;
      }
      else {
        *buffer = UP;
      }
    }
    else {
      if (_myPosition.first < p.first) {
        *buffer = RIGHT;
      }
      else {
        *buffer = LEFT;
      }
    }

    std::cout << "Miscare fara bomba: " << *buffer << std::endl;
  }
  else {
    std::pair<int, bool> pr = getBestMove();
    std::cout << "Miscare cu bomba: " << pr.first << std::endl;
    *buffer = pr.first | (1 << 31);
  }



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
			if (getFlameDuration(j, i) > 0) {
        std::cout << '#';
      } else if (getExplosionTime(j, i) > 0) {
        std::cout << 'B';
      }
      else if (isWall(j, i)) {
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
}

void Game::findPositions() {
	for (int i = 0; i < _N; ++i) {
		for (int j = 0; j < _M; ++j) {
      if ((_board[i * _M + j] & 0x000000ff) != 0) {

        if (((_board[i * _M + j] & 0x000000ff) & (1 << (_myId - 1))) != 0)
				  _myPosition = std::make_pair(j, i);

        if (((_board[i * _M + j] & 0x000000ff) ^ (1 << (_myId - 1))) != 0)
				  _opPosition = std::make_pair(j, i);
      }
    }
  }
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

void Game::calculateDanger() {
  int i, j, k;
  int flameDuration, explosionTime;
  for (i = 0; i < _N ; i++) { //i is column
    for (j = 0; j < _M; j++) { //j is line
      flameDuration = getFlameDuration(j, i);
      explosionTime = getExplosionTime(j, i);

      if (flameDuration > 1) {
        _dangerMatrix[j][i] = MaxDanger;
      }

      if (explosionTime == 1 || (flameDuration > 0 && explosionTime > 0)) {
        for (k = 1; k <= 6; k++) {
          if (isWall(j + k, i)) break;
          _dangerMatrix[j + k][i] = MaxDanger;
        }
        for (k = 1; k <= 6; k++) {
          if (isWall(j, i + k)) break;
          _dangerMatrix[j][i + k] = MaxDanger;
        }
        for (k = -1; k >= -6; k--) {
          if (isWall(j - k, i)) break;
          _dangerMatrix[j - k][i] = MaxDanger;
        }
        for (k = -1; k >= -6; k--) {
          if (isWall(j, i - k)) break;
          _dangerMatrix[j][i - k] = MaxDanger;
        }
      }
    }
  }
}

int Game::getDanger(int x, int y) {
  return _dangerMatrix[y][x];
}

// complexitate mare!!
void Game::calculateEstimatedExplosionTime() {
  for (int i = 0; i < _N; ++i) {
    for (int j = 0; j < _M; ++j) {
      _dangerMatrix[i][j] = getExplosionTime(j, i);
    }
  }

  for (int val = 1; val <= 10; ++val) {
    for (int i = 0; i < _N; ++i) {
      for (int j = 0; j < _M; ++j) {
        if (_dangerMatrix[i][j] == val) {
          for (int dy = -6; dy <= 6; ++dy) {
            for (int dx = -6; dx <= 6; ++dx) {
              if (dx == 0 && dy == 0) continue;

              _dangerMatrix[i + dy][j + dx] = std::min(_dangerMatrix[i + dy][j + dx],
                                                  _dangerMatrix[i][j] + 1);
            }
          }
        }
      }
    }
  }
}

int Game::getEstimatedExplosionTime(int x, int y) {
  return _dangerMatrix[y][x];
}

std::pair<int, bool> Game::getBestMove() {
  double score;
  double bestScore = 0;
  std::pair<int, bool> returnValue;

  int column = _myPosition.first;
  int line = _myPosition.second;

	int dx[] = { 0, -1, 0,  1 , 0};
	int dy[] = { 1,  0, -1, 0, 0};
	int moves[] = { DOWN, LEFT, UP, RIGHT, STAY };

	for (int k = 0; k < 5; ++k) {
		if (0 <=   line + dy[k] &&   line + dy[k] < _N &&
		    0 <= column + dx[k] && column + dx[k] < _M &&
				!isWall(column + dx[k], line + dy[k])      &&
			  getDanger(column + dx[k], line + dy[k]) != MaxDanger) {
				
					score = getScore(column + dx[k], line + dy[k], false);
					if (score > bestScore) {
						bestScore = score;
						returnValue.first = moves[k];
						returnValue.second = false;
					}

					if (!isWall(column + dx[k], line + dy[k]))
						score = getScore(column + dx[k], line + dy[k], true);
						
					if (score > bestScore) {
						bestScore = score;
						returnValue.first = moves[k];
						returnValue.second = true;
					}
				}
	}

  return returnValue;
}

double Game::getScore(int x, int y, bool bomb) {
  return area(x, y) * 0.3 + 1 / survival(x, y);
}

double Game::area(int x, int y) {
  int width = _M / 4;
  int height = _N / 4;

  int ox = _opPosition.first;
  int oy = _opPosition.second;

  int countMyBlocks = 0, countOpBlocks = 0;

  for (int dy = -height; dy <= height; ++dy)
    for (int dx = -width; dx <= width; ++dx)
      if (0 <= x + dx && x + dx < _M &&
          0 <= y + dy && y + dy < _N)
          ++countMyBlocks;

  for (int dy = -height; dy <= height; ++dy)
    for (int dx = -width; dx <= width; ++dx)
      if (0 <= ox + dx && ox + dx < _M &&
          0 <= oy + dy && oy + dy < _N)
          ++countOpBlocks;

    return (double)countMyBlocks / countOpBlocks;
}

double Game::survival(int x, int y) {
  int xStart = x - RADIUS + 1;
  int yStart = y - RADIUS + 1;
  int sum = 0;

  calculateEstimatedExplosionTime();

  for (int i = yStart; i < y + RADIUS; i++) {
    for (int j = xStart; j < x + RADIUS; j++) {
      if (i < 0 || j < 0 || i >= _N || j >= _M) {
        continue;
      }

      sum += 11 - getEstimatedExplosionTime(j, i);
    }
  }

  return (double)sum / RADIUS * RADIUS;
}
