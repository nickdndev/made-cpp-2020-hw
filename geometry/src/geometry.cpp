#include "geometry.h"
#include <algorithm>
#include <cmath>
#include <utility>
#include <vector>

using namespace std;

const double EPS = 1e-6;
const double INF_OVER = 1 + 1e-8;

double normVector(const Point &pointA, const Point &pointB) {
  return sqrt(pow((pointB.y - pointA.y), 2) + pow((pointB.x - pointA.x), 2));
}

template <typename Base, typename T> inline bool instanceof (const T &ob) {
  return typeid(Base) == typeid(ob) || is_base_of<Base, T>::value;
}

Point::Point(double x_, double y_) : x(x_), y(y_) {}

Point::Point() : x(0.), y(0.) {}

bool Point::operator==(const Point &point) const {
  return abs(x - point.x) <= EPS && abs(y - point.y) <= EPS;
}

bool Point::operator!=(const Point &point) const { return !(*this == point); }

Point &Point::operator+=(const Point &a) {
  x += a.x;
  y += a.y;
  return *this;
}

Point &Point::operator-=(const Point &a) {
  x -= a.x;
  y -= a.y;
  return *this;
}

Point &Point::operator*=(const Point &a) {
  x *= a.x;
  y *= a.y;
  return *this;
}

Point &Point::operator*=(const double &number) {
  x *= number;
  y *= number;
  return *this;
}

Point Point::operator+(const Point &a) const {
  Point m = *this;
  return m += a;
}

Point Point::operator-(const Point &a) const {
  Point m = *this;
  return m -= a;
}

Point Point::operator*(const Point &a) const {
  Point m = *this;
  return m *= a;
}

Point Point::operator*(const double &a) const {
  Point m = *this;
  return m *= a;
}

Point &Point::operator=(const Point &a) {
  if (this == &a)
    return *this;
  x = a.x;
  y = a.y;
  return *this;
}

Line::Line(Point p1_, Point p2_) : point_a(p1_), point_b(p2_) {
  B = point_a.y - point_b.y;
  A = point_b.x - point_a.x;
  C = point_a.x * point_b.y - point_b.x * point_a.y;
}

Line::Line(Point p1, double c1_) : point_a(p1), C(c1_) {
  point_b = Point(c1_, c1_);
}

Line::Line(double b_, double c_) : A(1), B(b_), C(c_) {}

bool Line::operator==(const Line &line) const {
  return abs(B / A - line.B / line.A) <= EPS &&
      abs(C / A - line.C / line.A) <= EPS;
}

bool Line::operator!=(const Line &line) const { return !(*this == line); }

Polygon::Polygon(vector<Point> points_) : vertices(std::move(points_)) {}

double Polygon::perimeter() const {
  double perimeter = 0.;
  size_t j = vertices.size() - 1;
  for (size_t i = 0; i < vertices.size(); i++) {
    perimeter += normVector(vertices[j], vertices[i]);
    j = i;
  }

  return perimeter;
}

double Polygon::area() const {

  double area = 0.;

  size_t j = vertices.size() - 1;
  for (size_t i = 0; i < vertices.size(); i++) {
    area += (vertices[j].x + vertices[i].x) * (vertices[j].y - vertices[i].y);
    j = i;
  }

  return abs(area / 2.0);
}

void rotatePoint(Point &point, Point center, double angle) {
  double s = sin(angle);
  double c = cos(angle);

  point -= center;

  double xNew = point.x * c - point.y * s;
  double yNew = point.x * s + point.y * c;

  point.x = xNew + center.x;
  point.y = yNew + center.y;
}

void Polygon::rotate(const Point &center, double angle) {
  for (std::size_t i = 0; i < vertices.size(); i++) {
    rotatePoint(vertices[i], center, angle);
  }
}

void scalePoint(Point &point, const Point &center, double coefficient) {
  point.x = coefficient * (point.x - center.x) + center.x;
  point.y = coefficient * (point.y - center.y) + center.y;
}

void Polygon::scale(const Point &center, double coefficient) {
  for (std::size_t i = 0; i < vertices.size(); i++) {
    scalePoint(vertices[i], center, coefficient);
  }
}

vector<Point> Polygon::getVertices() { return vertices; }

void reflectPoint(Point &point, const Line &line) {
  double newX = (point.x * (pow(line.A, 2) - pow(line.B, 2)) -
      2 * line.B * (line.A * point.y + line.C)) /
      (pow(line.A, 2) + pow(line.B, 2));

  double newY = (point.y * (pow(line.B, 2) - pow(line.A, 2)) -
      2 * line.A * (line.B * point.x + line.C)) /
      (pow(line.A, 2) + pow(line.B, 2));
  point.x = newX;
  point.y = newY;
}

void reflectPoint(Point &point, const Point &reflectPoint) {
  point.x = reflectPoint.x + (reflectPoint.x - point.x);
  point.y = reflectPoint.y + (reflectPoint.y - point.y);
}

void Polygon::reflex(const Point &refPoint) {
  for (auto &point : vertices) {
    reflectPoint(point, refPoint);
  }
}

void Polygon::reflex(const Line &line) {
  for (auto &point : vertices) {
    reflectPoint(point, line);
  }
}

Polygon &Polygon::operator=(const Polygon &a) {
  if (this == &a)
    return *this;

  vector<Point> newPoints;
  newPoints.reserve(a.vertices.size());

  for (const auto &point : a.vertices) {
    newPoints.push_back(point);
  }

  vertices = newPoints;

  return *this;
}

bool Polygon::operator==(const Shape &another) {

  const auto another_polygon = dynamic_cast<const Polygon *>(&another);

  if (another_polygon == nullptr) {
    return false;
  }

  if ((*another_polygon).vertices.size() != vertices.size()) {
    return false;
  }

  for (size_t i = 0; i < vertices.size(); i++) {
    bool exist = false;
    for (size_t j = 0; j < vertices.size(); j++) {
      if ((*another_polygon).vertices[j] == vertices[i])
        exist = true;
    }

    if (!exist)
      return false;
  }

  return true;
}

bool Polygon::operator!=(const Shape &another) { return !(*this == another); }

size_t Polygon::verticesCount() const { return vertices.size(); }

Triangle::Triangle(Point p1_, Point p2_, Point p3_)
    : Polygon({p1_, p2_, p3_}) {}

Circle Triangle::inscribedCircle() const {

  Point pointA = vertices[0];
  Point pointB = vertices[1];
  Point pointC = vertices[2];

  double c = normVector(pointA, pointB);
  double a = normVector(pointC, pointB);
  double b = normVector(pointC, pointA);

  double s = (a + b + c) / 2;

  double radius = sqrt((s - a) * (s - b) * (s - c) / s);

  double x = (a * pointA.x + b * pointB.x + c * pointC.x) / (a + b + c);
  double y = (a * pointA.y + b * pointB.y + c * pointC.y) / (a + b + c);

  return Circle(Point(x, y), radius);
}

Point findCenterCircumscribedCircle(const Point &pointA, const Point &pointB,
                                    const Point &pointC) {

  double numeratorX =
      (pointA.y - pointB.y) * ((pointC.x - pointB.x) * (pointC.x + pointB.x) +
          (pointC.y - pointB.y) * (pointC.y + pointB.y)) -
          (pointC.y - pointB.y) * ((pointA.x - pointB.x) * (pointA.x + pointB.x) +
              (pointA.y - pointB.y) * (pointA.y + pointB.y));

  double denominatorX = (2 * (pointC.y - pointB.y) * (pointB.x - pointA.x) -
      2 * (pointB.x - pointC.x) * (pointA.y - pointB.y));

  double x = numeratorX / denominatorX;

  double numeratorY =
      (pointB.x - pointC.x) * ((pointA.x - pointB.x) * (pointA.x + pointB.x) +
          (pointA.y - pointB.y) * (pointA.y + pointB.y)) -
          (pointB.x - pointA.x) * ((pointC.x - pointB.x) * (pointC.x + pointB.x) +
              (pointC.y - pointB.y) * (pointC.y + pointB.y));

  double denominatorY = (2 * (pointB.y - pointC.y) * (pointB.x - pointA.x) -
      2 * (pointB.x - pointC.x) * (pointB.y - pointA.y));

  double y = numeratorY / denominatorY;

  return Point(x, y);
}

Circle Triangle::circumscribedCircle() const {
  Point pointA = vertices[0];
  Point pointB = vertices[1];
  Point pointC = vertices[2];

  Point centerPoint = findCenterCircumscribedCircle(pointA, pointB, pointC);

  double radius = normVector(pointA, centerPoint);

  return Circle(centerPoint, radius);
}

Point Triangle::centroid() const {
  Point pointA = vertices[0];
  Point pointB = vertices[1];
  Point pointC = vertices[2];

  double x = (pointA.x + pointB.x + pointC.x) / 3;
  double y = (pointA.y + pointB.y + pointC.y) / 3;

  return Point(x, y);
}

Line Triangle::EulerLine() const {
  return Line(centroid(), circumscribedCircle().center());
}

Circle Triangle::ninePointsCircle() const {

  Point pointA = vertices[0];
  Point pointB = vertices[1];
  Point pointC = vertices[2];

  Point pointMidA((pointA.x + pointB.x) / 2, (pointA.y + pointB.y) / 2);
  Point pointMidB((pointB.x + pointC.x) / 2, (pointB.y + pointC.y) / 2);
  Point pointMidC((pointC.x + pointA.x) / 2, (pointC.y + pointA.y) / 2);

  Point centerPoint =
      findCenterCircumscribedCircle(pointMidA, pointMidB, pointMidC);

  double radius = normVector(pointMidA, centerPoint);

  return Circle(centerPoint, radius);
}

Point Triangle::orthocenter() const {

  Point pointA = vertices[0];
  Point pointB = vertices[1];
  Point pointC = vertices[2];

  double cfA =
      (-1 / ((pointB.y * INF_OVER - pointC.y) / (pointB.x - pointC.x)));
  double cfC =
      (-1 / ((pointA.y * INF_OVER - pointB.y) / (pointA.x - pointB.x)));

  double dA = pointA.y - (cfA * (pointA.x));
  double bC = pointC.y - (cfC * (pointC.x));

  double x = (bC - dA) / (cfA - cfC);
  double y = (cfA * bC - cfC * dA) / (cfA - cfC);

  return Point(x, y);
}

Rectangle::Rectangle(Point p1, Point p3, int k = 1) : Polygon({}) {

  double diagonal = normVector(p1, p3);

  double ad_side = sqrt((diagonal * diagonal) / (1 + k * k));
  double dc_side = ad_side / k;

  double a = (ad_side * ad_side - dc_side * dc_side + diagonal * diagonal) /
      (2 * diagonal);
  double h = sqrt(ad_side * ad_side - a * a);

  Point splitter_diagonal(p1.x + (a / diagonal) * (p3.x - p1.x),
                          (p1.y + (a / diagonal) * (p3.y - p1.y)));

  Point new_point_candidate_D(
      splitter_diagonal.x + (h / diagonal) * (p3.x - p1.x),
      splitter_diagonal.y - (h / diagonal) * (p3.y - p1.y));

  Point new_point_candidate_B(
      splitter_diagonal.x - (h / diagonal) * (p3.x - p1.x),
      splitter_diagonal.y + (h / diagonal) * (p3.y - p1.y));

  vertices.push_back(p1);
  vertices.push_back(new_point_candidate_B);
  vertices.push_back(p3);
  vertices.push_back(new_point_candidate_D);
}

pair<Line, Line> Rectangle::diagonals() const {

  Line diagonalAC(Point(vertices[0].x, vertices[0].y),
                  Point(vertices[2].x, vertices[2].y));
  Line diagonalBD(Point(vertices[1].x, vertices[1].y),
                  Point(vertices[3].x, vertices[3].y));

  return make_pair(diagonalAC, diagonalBD);
}

Square::Square(Point p1, Point p2) : Rectangle(p1, p2) {}

Circle Square::circumscribedCircle() const {

  double radius = normVector(vertices[0], vertices[3]);

  Point center((vertices[0].x + vertices[1].x) / 2,
               (vertices[0].y + vertices[1].y) / 2);

  return Circle(center, radius);
}

Circle Square::inscribedCircle() const {

  double radius = normVector(vertices[0], vertices[1]);

  Point center((vertices[0].x + vertices[1].x) / 2,
               (vertices[0].y + vertices[1].y) / 2);

  return Circle(center, radius);
}

Ellipse::Ellipse(Point f1_, Point f2_, double c_)
    : focus_a(f1_), focus_b(f2_), a(c_ / 2.) {}

double Ellipse::eccentricity() const {
  double c = normVector(focus_a, focus_b) / 2;
  double e = c / a;
  return e;
}

double Ellipse::perimeter() const {
  return 4 * a * std::comp_ellint_2(eccentricity());
}

double Ellipse::area() const {

  double e = eccentricity();
  double b = a * sqrt(1 - e * e);

  return M_PI * a * b;
}

void Ellipse::rotate(const Point &center, double angle) {
  rotatePoint(focus_a, center, angle);
  rotatePoint(focus_b, center, angle);
}

void Ellipse::scale(const Point &center, double coefficient) {
  Point pointRad((focus_a.x + focus_b.x) / 2 + a, focus_a.y);

  scalePoint(focus_a, center, coefficient);
  scalePoint(focus_b, center, coefficient);
  scalePoint(pointRad, center, coefficient);

  a = sqrt(pow(((focus_a.x + focus_b.x) / 2 - pointRad.x), 2) +
      pow(((focus_a.y + focus_b.y) / 2 - pointRad.y), 2));
}

bool Ellipse::operator==(const Shape &another) {

  const auto another_ellipse = dynamic_cast<const Ellipse *>(&another);

  if (another_ellipse == nullptr) {
    return false;
  }

  double nornFocusesAnother =
      normVector((*another_ellipse).focus_a, (*another_ellipse).focus_b);
  double nornFocusesThis = normVector(focus_a, focus_b);

  return (abs(nornFocusesAnother - nornFocusesThis) <= EPS) &&
      (((*another_ellipse).a - a) <= EPS);
}

bool Ellipse::operator!=(const Shape &another) { return !(*this == another); }

void Ellipse::reflex(const Line &line) {
  reflectPoint(focus_a, line);
  reflectPoint(focus_b, line);
}

void Ellipse::reflex(const Point &point) {
  reflectPoint(focus_a, point);
  reflectPoint(focus_b, point);
}
pair<Point, Point> Ellipse::focuses() const { return {focus_a, focus_b}; }

Circle::Circle(Point p1, double r) : Ellipse(p1, p1, r * 2.) {}

Point Circle::center() const { return focus_a; }

double Circle::radius() const { return a; }
