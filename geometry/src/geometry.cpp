#include <algorithm>
#include <utility>
#include <vector>
#include "geometry.h"
#include <cmath>

using namespace std;

const double EPS = 1e-6;
const double INF_OVER = 1 + 1e-8;


double calculateLengthByPoints(Point pointA, Point pointB) {
    return sqrt(pow((pointB.y - pointA.y), 2) + pow((pointB.x - pointA.x), 2));
}

Point::Point(double x_, double y_) : x(x_), y(y_) {

}

Point::Point() : x(0.), y(0.) {

}

bool Point::operator==(const Point& point) const {
    return abs(x - point.y) <= EPS && abs(y - point.y) <= EPS;
}

bool Point::operator!=(const Point& point) const {
    return !(*this == point);
}

Point& Point::operator+=(const Point& a) {
    x += a.x;
    y += a.y;
    return *this;
}

Point& Point::operator-=(const Point& a) {
    x -= a.x;
    y -= a.y;
    return *this;
}

Point& Point::operator*=(const Point& a) {
    x *= a.x;
    y *= a.y;
    return *this;
}

Point& Point::operator*=(const double& number) {
    x *= number;
    y *= number;
    return *this;
}

Point Point::operator+(const Point& a) const {
    Point m = *this;
    return m += a;
}

Point Point::operator-(const Point& a) const {
    Point m = *this;
    return m -= a;
}

Point Point::operator*(const Point& a) const {
    Point m = *this;
    return m *= a;
}

Point Point::operator*(const double& a) const {
    Point m = *this;
    return m *= a;
}

Point& Point::operator=(const Point& a) {
    if (this == &a)
        return *this;
    x = a.x;
    y = a.y;
    return *this;
}


Line::Line(Point p1_, Point p2_) : p1(p1_), p2(p2_) {
    B = p1.y - p2.y;
    A = p2.x - p1.x;
    C = p1.x * p2.y - p2.x * p1.y;
}

Line::Line(Point p1, double c1_) : p1(p1), c1(c1_) {

    p2 = Point(c1_, c1_);
}

Line::Line(double k_, double c1_) : k(k_), c1(c1_) {
    A = 1;
    B = k;
    C = c1;
}


bool Line::operator==(const Line& line) const {
    return abs(B / A - line.B / line.A) <= EPS && abs(C / A - line.C / line.A) <= EPS;
}

bool Line::operator!=(const Line& line) const {
    return !(*this == line);
}

bool Shape::operator==(const Shape& another) {
    if (typeid(*this) == typeid(another)) {

        double a1 = area();
        double a2 = another.area();
        return abs(area() - another.area()) <= EPS;
    }

    return false;
}

bool Shape::operator!=(const Shape& another) {

    return !(*this == another);
}


Polygon::Polygon(vector<Point> points_) : vertices(std::move(points_)) {

}

double Polygon::perimeter() const {
    double perimeter = 0.;
    size_t j = vertices.size() - 1;
    for (size_t i = 0; i < vertices.size(); i++) {
        perimeter += sqrt(pow((vertices[j].x - vertices[i].x), 2) + pow((vertices[j].y - vertices[i].y), 2));
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


void rotatePoint(Point& point, Point center, double angle) {
    double s = sin(angle);
    double c = cos(angle);

    point -= center;

    double xNew = point.x * c - point.y * s;
    double yNew = point.x * s + point.y * c;

    point.x = xNew + center.x;
    point.y = yNew + center.y;
}

void Polygon::rotate(const Point& center, double angle) {
    for (std::size_t i = 0; i < vertices.size(); i++) {
        rotatePoint(vertices[i], center, angle);
    }
}

void scalePoint(Point& point, const Point& center, double coefficient) {
    point.x = coefficient * (point.x - center.x) + center.x;
    point.y = coefficient * (point.y - center.y) + center.y;
}


void Polygon::scale(const Point& center, double coefficient) {
    for (std::size_t i = 0; i < vertices.size(); i++) {
        scalePoint(vertices[i], center, coefficient);
    }
}

vector<Point> Polygon::getVertices() {
    vector<Point> verticesNew;
    for (const auto& point : vertices) {
        verticesNew.push_back(point);
    }

    return verticesNew;
}

void reflectPoint(Point& point, const Line& line) {
    double newX = (point.x * (pow(line.A, 2) - pow(line.B, 2)) - 2 * line.B * (line.A * point.y + line.C)) /
                  (pow(line.A, 2) + pow(line.B, 2));

    double newY = (point.y * (pow(line.B, 2) - pow(line.A, 2)) - 2 * line.A * (line.B * point.x + line.C)) /
                  (pow(line.A, 2) + pow(line.B, 2));
    point.x = newX;
    point.y = newY;

}

void reflectPoint(Point& point, const Point& reflectPoint) {
    point.x = reflectPoint.x + (reflectPoint.x - point.x);
    point.y = reflectPoint.y + (reflectPoint.y - point.y);

}

void Polygon::reflex(const Point& refPoint) {
    for (auto& point : vertices) {
        reflectPoint(point, refPoint);
    }
}

void Polygon::reflex(const Line& line) {
    for (auto& point : vertices) {
        reflectPoint(point, line);
    }
}

Polygon& Polygon::operator=(const Polygon& a) {
    if (this == &a)
        return *this;

    vector<Point> newPoints;
    for (const auto& point : a.vertices) {
        newPoints.push_back(point);
    }

    vertices = newPoints;

    return *this;
}

bool Polygon::operator==(const Shape& another) {
    return abs(area() - another.area()) <= EPS;
}

bool Polygon::operator!=(const Shape& another) {
    return !(*this == another);
}


Triangle::Triangle(Point p1_, Point p2_, Point p3_) : Polygon({p1_, p2_, p3_}) {

}


Circle Triangle::inscribedCircle() const {

    Point pointA = vertices[0];
    Point pointB = vertices[1];
    Point pointC = vertices[2];

    double c = calculateLengthByPoints(pointA, pointB);
    double a = calculateLengthByPoints(pointC, pointB);
    double b = calculateLengthByPoints(pointC, pointA);

    double s = (a + b + c) / 2;

    double radius = sqrt((s - a) * (s - b) * (s - c) / s);

    double x = (a * pointA.x + b * pointB.x + c * pointC.x) / (a + b + c);
    double y = (a * pointA.y + b * pointB.y + c * pointC.y) / (a + b + c);
    Point centerPoint(x, y);
    return Circle(centerPoint, radius);
}

Circle Triangle::circumscribedCircle() const {
    Point pointA = vertices[0];
    Point pointB = vertices[1];
    Point pointC = vertices[2];

    double res1 = (pointA.y - pointB.y)
                  * ((pointC.x - pointB.x) * (pointC.x + pointB.x) + (pointC.y - pointB.y) * (pointC.y + pointB.y))
                  - (pointC.y - pointB.y)
                    * ((pointA.x - pointB.x) * (pointA.x + pointB.x) + (pointA.y - pointB.y) * (pointA.y + pointB.y));

    double res2 = (2 * (pointC.y - pointB.y) * (pointB.x - pointA.x) -
                   2 * (pointB.x - pointC.x) * (pointA.y - pointB.y));


    double x = res1 / res2;


    double res3 = (pointB.x - pointC.x)
                  * ((pointA.x - pointB.x) * (pointA.x + pointB.x) + (pointA.y - pointB.y) * (pointA.y + pointB.y))
                  - (pointB.x - pointA.x)
                    * ((pointC.x - pointB.x) * (pointC.x + pointB.x) + (pointC.y - pointB.y) * (pointC.y + pointB.y));

    double res4 = (2 * (pointB.y - pointC.y) * (pointB.x - pointA.x) -
                   2 * (pointB.x - pointC.x) * (pointB.y - pointA.y));


    double y = res3 / res4;


    double radius = sqrt(pow((pointA.x - x), 2) + pow((pointA.y - y), 2));


    Point centerPoint(x, y);
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

    double Dx = pointA.x / 2 + pointB.x / 2;
    double Dy = pointA.y / 2 + pointB.y / 2;

    double Ex = pointB.x / 2 + pointC.x / 2;
    double Ey = pointB.y / 2 + pointC.y / 2;

    double Fx = pointC.x / 2 + pointA.x / 2;
    double Fy = pointC.y / 2 + pointA.y / 2;


    double res1 = (Dy - Ey)
                  * ((Fx - Ex) * (Fx + Ex) + (Fy - Ey) * (Fy + Ey))
                  - (Fy - Ey)
                    * ((Dx - Ex) * (Dx + Ex) + (Dy - Ey) * (Dy + Ey));

    double res2 = (2 * (Fy - Ey) * (Ex - Dx) -
                   2 * (Ex - Fx) * (Dy - Ey));


    double x = res1 / res2;


    double res3 = (Ex - Fx)
                  * ((Dx - Ex) * (Dx + Ex) + (Dy - Ey) * (Dy + Ey))
                  - (Ex - Dx)
                    * ((Fx - Ex) * (Fx + Ex) + (Fy - Ey) * (Fy + Ey));

    double res4 = (2 * (Ey - Fy) * (Ex - Dx) -
                   2 * (Ex - Fx) * (Ey - Dy));


    double y = res3 / res4;
    double radius = sqrt(pow((Dx - x), 2) + pow((Dy - y), 2));

    Point centerPoint(x, y);
    return Circle(centerPoint, radius);
}

Point Triangle::orthocenter() const {

    Point pointA = vertices[0];
    Point pointB = vertices[1];
    Point pointC = vertices[2];

    double cfA = (-1 / ((pointB.y * INF_OVER - pointC.y) / (pointB.x - pointC.x)));
    double cfC = (-1 / ((pointA.y * INF_OVER - pointB.y) / (pointA.x - pointB.x)));

    double dA = pointA.y - (cfA * (pointA.x));
    double bC = pointC.y - (cfC * (pointC.x));

    double x = (bC - dA) / (cfA - cfC);
    double y = (cfA * bC - cfC * dA) / (cfA - cfC);

    return Point(x, y);
}


Rectangle::Rectangle(Point p1, Point p3, int k = 1) : Polygon({}) {
    Point p2(p1.x, p3.y);
    Point p4(p3.x, p1.y);

    vertices.push_back(p1);
    vertices.push_back(p2);
    vertices.push_back(p3);
    vertices.push_back(p4);

}

pair<Line, Line> Rectangle::diagonals() {

    Line diagonalAC(Point(vertices[0].x, vertices[0].y), Point(vertices[2].x, vertices[2].y));
    Line diagonalBD(Point(vertices[1].x, vertices[1].y), Point(vertices[3].x, vertices[3].y));

    return make_pair(diagonalAC, diagonalBD);
}

Square::Square(Point p1, Point p2) : Rectangle(p1, p2) {

}

Circle Square::circumscribedCircle() const {

    double radius = calculateLengthByPoints(vertices[0], vertices[3]);

    Point center((vertices[0].x + vertices[1].x) / 2, (vertices[0].y + vertices[1].y) / 2);

    return Circle(center, radius);
}

Circle Square::inscribedCircle() const {

    double radius = calculateLengthByPoints(vertices[0], vertices[1]);

    Point center((vertices[0].x + vertices[1].x) / 2, (vertices[0].y + vertices[1].y) / 2);

    return Circle(center, radius);
}

Ellipse::Ellipse(Point f1_, Point f2_, double c_) : focus1(f1_), focus2(f2_), a(c_ / 2.) {

}

double Ellipse::eccentricity() const {
    double c = calculateLengthByPoints(focus1, focus2) / 2;
    double e = c / a;
    return e;
}

double Ellipse::perimeter() const {
    return 4 * a * std::comp_ellint_2(eccentricity());;
}

double Ellipse::area() const {

    double e = eccentricity();

    double b = a * sqrt(1 - e * e);

    return M_PI * a * b;
}

void Ellipse::rotate(const Point& center, double angle) {
    rotatePoint(focus1, center, angle);
    rotatePoint(focus2, center, angle);
}

void Ellipse::scale(const Point& center, double coefficient) {
    Point pointRad((focus1.x + focus2.x) / 2 + a, focus1.y);

    scalePoint(focus1, center, coefficient);
    scalePoint(focus2, center, coefficient);
    scalePoint(pointRad, center, coefficient);

    a = sqrt(pow(((focus1.x + focus2.x) / 2 - pointRad.x), 2) + pow(((focus1.y + focus2.y) / 2 - pointRad.y), 2));
}

bool Ellipse::operator==(const Shape& another) {
    return abs(area() - another.area()) <= EPS;
}

bool Ellipse::operator!=(const Shape& another) {
    return !(*this == another);
}

void Ellipse::reflex(const Line& line) {
    reflectPoint(focus1, line);
    reflectPoint(focus2, line);
}

void Ellipse::reflex(const Point& point) {
    reflectPoint(focus1, point);
    reflectPoint(focus2, point);
}


Circle::Circle(Point p1, double r) : Ellipse(p1, p1, r * 2.) {

}

Point Circle::center() {
    return focus1;
}

double Circle::radius() {
    return a;
}
