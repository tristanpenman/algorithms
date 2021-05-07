#include <fstream>
#include <iostream>

#include "Point2DSet.h"

using namespace std;

// write a Point2D to an output stream
ostream& operator<<(ostream& aOutStream, const Point2D& aObject)
{
  aOutStream << aObject.getId() << ": (" << aObject.getX() << "," << aObject.getY() << ")";

  return aOutStream;
}

// read a Point2D from an input stream
istream& operator>>(istream& aInpStream, Point2D& aObject)
{
  string id;
  double x;
  double y;
  aInpStream >> id >> x >> y;

  if (aInpStream.good()) {
    aObject.setId(id);
    aObject.setX(x);
    aObject.setY(y);
  }

  return aInpStream;
}

// write a Point2DSet to an output stream
ostream& operator<<(ostream& aOutStream, const Point2DSet& aObject)
{
  for (const Point2D& p : aObject) {
    aOutStream << p << endl;
  }

  return aOutStream;
}

// read a Point2DSet from an input stream
istream& operator>>(istream& aInpStream, Point2DSet& aObject)
{
  int count;
  aInpStream >> count;
  while (count--) {
    Point2D p;
    aInpStream >> p;
    aObject.add(move(p));
  }

  return aInpStream;
}

int main()
{
  Point2DSet lSet;

  ifstream ifs("points.txt");
  ifs >> lSet;

  cout << "Points:" << endl;
  cout << lSet;

  Point2DSet lConvexHull;
  lSet.buildConvexHull(lConvexHull);

  cout << "Convex hull:" << endl;
  size_t lSize = lConvexHull.size();
  for (auto i = 0; i < lSize; i++) {
    cout
      << lConvexHull[i].getId() << " - "
      << lConvexHull[(i + 1) % lSize].getId() << endl;
  }

  return 0;
}
