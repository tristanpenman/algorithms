#pragma once

#include <vector>

#include "Point2D.h"

class Point2DSet
{
  using PointVector = std::vector<Point2D>;

  PointVector fPoints;

  // helper function to check if a new point forms a right turn with the existing points
  // in a set, relative to a particular origin point
  bool doesNotTurnLeft(const Point2D& aOrigin, const Point2D& aPoint) const;

public:
  using Iterator = std::vector<Point2D>::const_iterator;

  const Point2D& operator[](size_t aIndex) const;

  // make this iterable
  Iterator begin() const;
  Iterator end() const;
  size_t size() const;

  void add(const Point2D& aPoint);
  void add(Point2D&& aPoint);
  void clear();
  void removeLast();

  void buildConvexHull(Point2DSet& aConvexHull);
};

inline bool Point2DSet::doesNotTurnLeft(const Point2D& aOrigin, const Point2D& aPoint) const
{
  const Point2D& lastPoint = fPoints[size() - 1];
  const Point2D& secondLastPoint = fPoints[size() - 2];

  return lastPoint.clockwise(secondLastPoint, aPoint);
}

inline const Point2D& Point2DSet::operator[](size_t aIndex) const
{
  return fPoints[aIndex];
}

inline Point2DSet::Iterator Point2DSet::begin() const
{
  return fPoints.begin();
}

inline Point2DSet::Iterator Point2DSet::end() const
{
  return fPoints.end();
}

inline size_t Point2DSet::size() const
{
  return fPoints.size();
}

inline void Point2DSet::add(const Point2D& aPoint)
{
  fPoints.push_back(aPoint);
}

inline void Point2DSet::add(Point2D&& aPoint)
{
  fPoints.push_back(aPoint);
}

inline void Point2DSet::clear()
{
  fPoints.clear();
}

inline void Point2DSet::removeLast()
{
  fPoints.pop_back();
}

inline void Point2DSet::buildConvexHull(Point2DSet& aConvexHull)
{
  aConvexHull.clear();

  // smallest point will be used as origin for magnitude and direction calculations
  const Point2D lOrigin = *std::min_element(fPoints.begin(), fPoints.end(),
    [](const Point2D& a, const Point2D& b) {
      if (a.getY() - a.getX() < Point2D::epsilon) {
        return a.getX() < b.getX();
      } else {
        return a.getY() < b.getY();
      }
    }
  );

  // sort points by direction relative to origin, ties broken by magnitude
  std::stable_sort(fPoints.begin(), fPoints.end(),
    [&lOrigin](const Point2D& a, const Point2D& b) {
      if (a.collinear(lOrigin, b)) {
        return a.magnitudeTo(lOrigin) < b.magnitudeTo(lOrigin);
      } else {
        return a.directionTo(lOrigin) < b.directionTo(lOrigin);
      }
    }
  );

  for (const auto& fPoint : fPoints) {
    // remove existing points that are contained by new point
    while (aConvexHull.size() > 1 && aConvexHull.doesNotTurnLeft(lOrigin, fPoint)) {
      aConvexHull.removeLast();
    }

    // add new point to the convex hull
    aConvexHull.add(fPoint);
  }
}
