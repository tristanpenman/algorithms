#pragma once

#include <cmath>

class Vector2D
{
  double fX;
  double fY;

public:
  Vector2D() = default;
  Vector2D(double aX, double aY);

  Vector2D operator+(const Vector2D& aRHS) const;
  Vector2D operator-(const Vector2D& aRHS) const;

  double getX() const;
  void setX(double aX);

  double getY() const;
  void setY(double aY);

  double cross(const Vector2D& aRHS) const;
  double dot(const Vector2D& aRHS) const;

  double angleBetween(const Vector2D& aRHS) const;
  double direction() const;
  double magnitude() const;
};

inline Vector2D::Vector2D(double aX, double aY)
  : fX(aX)
  , fY(aY)
{
}

inline Vector2D Vector2D::operator+(const Vector2D& aRHS) const
{
  return Vector2D(fX + aRHS.fX, fY + aRHS.fY);
}

inline Vector2D Vector2D::operator-(const Vector2D& aRHS) const
{
  return Vector2D(fX - aRHS.fX, fY - aRHS.fY);
}

inline double Vector2D::getX() const
{
  return fX;
}

inline void Vector2D::setX(double aX)
{
  fX = aX;
}

inline double Vector2D::getY() const
{
  return fY;
}

inline void Vector2D::setY(double aY)
{
  fY = aY;
}

inline double Vector2D::cross(const Vector2D& aRHS) const
{
  return fX * aRHS.fY - aRHS.fX * fY;
}

inline double Vector2D::dot(const Vector2D& aRHS) const
{
  return fX * aRHS.fX + fY * aRHS.fY;
}

inline double Vector2D::angleBetween(const Vector2D& aRHS) const
{
  return std::atan2(fY - aRHS.fY, fX - aRHS.fX);
}

inline double Vector2D::direction() const
{
  return std::atan2(fY, fX);
}

inline double Vector2D::magnitude() const
{
  return std::sqrt(fX * fX + fY * fY);
}