#pragma once

#include <vector>

using namespace std;

class Ellipse;

class Circle;

class Line;

class Polygon;

class Triangle;

class Square;

struct Point {
  double x, y;

  Point(double x, double y);

  Point();

  bool operator==(const Point &point) const;

  bool operator!=(const Point &point) const;

  Point &operator+=(const Point &a);

  Point &operator-=(const Point &a);

  Point &operator*=(const Point &a);

  Point &operator*=(const double &number);

  Point operator+(const Point &a) const;

  Point operator-(const Point &a) const;

  Point operator*(const Point &a) const;

  Point operator*(const double &a) const;

  Point &operator=(const Point &a);
};

class Line {
  Point point_a, point_b;

public:
  double A, B, C;

  Line(Point point_a, Point point_b);

  Line(Point point_a, double c);

  Line(double b, double c);

  bool operator==(const Line &line) const;

  bool operator!=(const Line &line) const;
};

class Shape {
public:
  virtual double perimeter() const = 0;

  virtual double area() const = 0;

  virtual void rotate(const Point &center, double angle) = 0;

  virtual void scale(const Point &center, double coefficient) = 0;

  virtual void reflex(const Line &line) = 0;

  virtual void reflex(const Point &point) = 0;

  virtual bool operator==(const Shape &another) = 0;

  virtual bool operator!=(const Shape &another) = 0;
};

class Polygon : public Shape {

protected:
  vector<Point> vertices;

public:
  Polygon(vector<Point> points);

  double perimeter() const override;

  double area() const override;

  void rotate(const Point &center, double angle) override;

  void scale(const Point &center, double coefficient) override;

  void reflex(const Line &line) override;

  void reflex(const Point &point) override;

  vector<Point> getVertices();

  size_t verticesCount() const;

  Polygon &operator=(const Polygon &a);

  bool operator==(const Shape &another) override;

  bool operator!=(const Shape &another) override;
};

class Triangle : public Polygon {
  using Polygon::Polygon;

public:
  Triangle(Point p1, Point p2, Point p3);

  Circle inscribedCircle() const;

  Circle circumscribedCircle() const;

  Circle ninePointsCircle() const;

  Point orthocenter() const;

  Line EulerLine() const;

  Point centroid() const;
};

class Rectangle : public Polygon {

public:
  Rectangle(Point p1, Point p2, int k);

  pair<Line, Line> diagonals() const;
};

class Square : public Rectangle {

public:
  Square(Point p1, Point p2);

  Circle circumscribedCircle() const;

  Circle inscribedCircle() const;
};

class Ellipse : public Shape {

protected:
  Point focus_a;
  Point focus_b;
  double a;

public:
  Ellipse(Point f1, Point f2, double c);

  double perimeter() const override;

  double area() const override;

  void rotate(const Point &center, double angle) override;

  void scale(const Point &center, double coefficient) override;

  void reflex(const Line &line) override;

  void reflex(const Point &line) override;

  double eccentricity() const;

  pair<Point, Point> focuses() const;

  bool operator==(const Shape &another) override;

  bool operator!=(const Shape &another) override;
};

class Circle : public Ellipse {

public:
  Circle(Point p1, double r);

  Point center() const;

  double radius() const;
};
