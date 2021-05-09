#pragma once

struct Point {
  Point(int x, int y);
  Point(const Point& rhs);
  Point& operator=(const Point& rhs);
  Point operator+(const Point& rhs) const;
  Point operator/(float) const;

  //
  // Allow points to be sorted first along the X axis,
  // then along the Y axis
  //
  bool operator<=(const Point& rhs) const;

  int x;
  int y;
};

inline Point::Point(int x, int y)
: x(x)
, y(y) { }

inline Point::Point(const Point& rhs)
: x(rhs.x)
, y(rhs.y) { }

inline Point& Point::operator=(const Point& rhs) {
  x = rhs.x;
  y = rhs.y;
  return *this;
}

inline Point Point::operator+(const Point& rhs) const {
  return Point(x + rhs.x, y + rhs.y);
}

inline Point Point::operator/(float f) const {
  return Point(x / f, y / f);
}

//
// Allow points to be sorted first along the X axis,
// then along the Y axis
//
inline bool Point::operator<=(const Point& rhs) const {
  return (x == rhs.x && y <= rhs.y) || x <= rhs.x;
}
