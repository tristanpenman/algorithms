#pragma once

#include <string>

#include "Vector2D.h"

class Point2D
{
  std::string fId;
  Vector2D fPosition;

public:
  Point2D() = default;
  Point2D(const std::string& aId, double aX, double aY);

  Vector2D operator-(const Point2D& aRHS) const;
  bool operator<(const Point2D& aRHS) const;

  const std::string& getId() const;
  void setId(const std::string& aId);

  double getX() const;
  void setX(double aX);

  double getY() const;
  void setY(double aY);

  double directionTo(const Point2D& aOther) const;
  double magnitudeTo(const Point2D& aOther) const;

  bool clockwise(const Point2D& aP0, const Point2D& aP2) const;
  bool collinear(const Point2D& aP0, const Point2D& aP1) const;

  static constexpr double epsilon = 0.0001;
};

inline Point2D::Point2D(const std::string& aId, double aX, double aY)
  : fId(aId)
  , fPosition(aX, aY)
{
}

inline Vector2D Point2D::operator-(const Point2D& aRHS) const
{
  return fPosition - aRHS.fPosition;
}

inline const std::string& Point2D::getId() const
{
  return fId;
}

inline void Point2D::setId(const std::string& aId)
{
  fId = aId;
}

inline double Point2D::getX() const
{
  return fPosition.getX();
}

inline void Point2D::setX(double aX)
{
  fPosition.setX(aX);
}

inline double Point2D::getY() const
{
  return fPosition.getY();
}

inline void Point2D::setY(double aY)
{
  fPosition.setY(aY);
}

inline double Point2D::directionTo(const Point2D& aOther) const
{
  return (aOther - *this).direction();
}

inline double Point2D::magnitudeTo(const Point2D& aOther) const
{
  return (aOther - *this).magnitude();
}

inline bool Point2D::clockwise(const Point2D& aP0, const Point2D& aP2) const
{
  return (aP2 - aP0).cross(*this - aP0) > epsilon;
}

inline bool Point2D::collinear(const Point2D& aP0, const Point2D& aP2) const
{
  return std::abs((aP2 - aP0).cross(*this - aP0)) < epsilon;
}
