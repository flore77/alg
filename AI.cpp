

double survival(int x, int y) {
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
