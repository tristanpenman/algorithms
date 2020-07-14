#include "ClosestPair.h"
#include "Util.h"

int main(int argc, char* argv[]) {
  Point points[] = {
    Point(0, 0),
    Point(2, 2),
    Point(3, 4),
    Point(6, 8),
    Point(8, 16),
    Point(10, 32),
    Point(12, 64)
  };

  std::cout << "Input array: " << Util::streamArray(points, 0, 6) << std::endl;
  const std::pair<int, int> closest = ClosestPair::findClosestPair(points, 0, 6);
  std::cout << "Closest pair: " << points[closest.first] << " and " << points[closest.second] << std::endl;

  return 0;
}
