//#pragma once
#include <algorithm>
#include <utility>
#include <vector>
#include <complex>
#include "geometry.h"
#include <cmath>

using namespace std;

Point::Point(double x_, double y_) : x(x_), y(y_) {

}

bool Point::operator==(const Point& point) const {
    return x == point.x && y == point.y;
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

bool Line::operator==(const Point& point) const {
    return false;
}

bool Line::operator!=(const Point& point) const {
    return false;
}


Polygon::Polygon(vector<Point> points_) : points(std::move(points_)) {

}

bool Polygon::operator==(const Polygon& polygon) const {
    if (polygon.points.size() != points.size())
        return false;

    for (size_t i = 0; i < points.size(); i++) {

        Point pointLeft = points[i];
        bool exist = false;
        for (size_t j = 0; j < points.size(); j++) {
            Point pointRight = polygon.points[j];
            if (pointRight == pointLeft)
                exist = true;
        }

        if (!exist)
            return false;

    }
    return true;
}

bool Polygon::operator!=(const Polygon& polygon) const {
    return !(*this == polygon);;
}

double Polygon::perimeter() const {
    return 0;
}

double Polygon::area() const {

    double area = 0.0;

    size_t j = points.size() - 1;
    for (size_t i = 0; i < points.size(); i++) {
        area += (points[j].x + points[i].x) * (points[j].y - points[i].y);
        j = i;
    }

    area = abs(area / 2.0);


    return area;
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

void Polygon::rotate(Point center, double angle) {
    for (std::size_t i = 0; i < points.size(); i++) {
        rotatePoint(points[i], center, angle);
    }
}

void scalePoint(Point& point, Point& center, double coefficient) {
    point.x = coefficient * (point.x - center.x) + center.x;
    point.y = coefficient * (point.y - center.y) + center.y;
}


void Polygon::scale(Point center, double coefficient) {
    for (std::size_t i = 0; i < points.size(); i++) {
        scalePoint(points[i], center, coefficient);
    }
}

vector<Point> Polygon::getVertices() {
    vector<Point> vertices;
    for (const auto& point : points) {

        vertices.push_back(point);
    }

    return vertices;
}

void reflectPoint(Point& point, const Line& line) {
    double newX = (point.x * (pow(line.A, 2) - pow(line.B, 2)) - 2 * line.B * (line.A * point.y + line.C)) /
                  (pow(line.A, 2) + pow(line.B, 2));

    double newY = (point.y * (pow(line.B, 2) - pow(line.A, 2)) - 2 * line.A * (line.B * point.x + line.C)) /
                  (pow(line.A, 2) + pow(line.B, 2));
    point.x = newX;
    point.y = newY;

}

void Polygon::reflex(const Line& line) {
    for (auto& point : points) {
        reflectPoint(point, line);
    }
}

Polygon& Polygon::operator=(const Polygon& a) {
    if (this == &a)
        return *this;

    vector<Point> newPoints;
    for (const auto& point : a.points) {
        newPoints.push_back(point);
    }

    points = newPoints;

    return *this;
}

double Triangle::perimeter() const {
    return 0;
}


Triangle::Triangle(Point p1_, Point p2_, Point p3_) : Polygon({p1_, p2_, p3_}) {

}

Circle Triangle::inscribedCircle() {

    Point pointA = points[0];
    Point pointB = points[1];
    Point pointC = points[2];

    double c = sqrt(pow((pointB.y - pointA.y), 2) + pow((pointB.x - pointA.x), 2));
    double a = sqrt(pow((pointC.y - pointB.y), 2) + pow((pointC.x - pointB.x), 2));
    double b = sqrt(pow((pointA.y - pointC.y), 2) + pow((pointA.x - pointC.x), 2));

    double s = (a + b + c) / 2;

    double radius = sqrt((s - a) * (s - b) * (s - c) / s);

    double x = (a * pointA.x + b * pointB.x + c * pointC.x) / (a + b + c);
    double y = (a * pointA.y + b * pointB.y + c * pointC.y) / (a + b + c);
    Point centerPoint(x, y);
    return Circle(centerPoint, radius);
}

Circle Triangle::circumscribedCircle() {
    Point pointA = points[0];
    Point pointB = points[1];
    Point pointC = points[2];

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

Point Triangle::centroid() {
    Point pointA = points[0];
    Point pointB = points[1];
    Point pointC = points[2];

    double x = (pointA.x + pointB.x + pointC.x) / 3;
    double y = (pointA.y + pointB.y + pointC.y) / 3;

    return Point(x, y);
}

Line Triangle::EulerLine() {
    Point centroidPoint = centroid();
    Point circlePoint = circumscribedCircle().center();

    return Line(centroidPoint, circlePoint);
}


Rectangle::Rectangle(Point p1, Point p3, int k = 1) : Polygon({}) {
    Point p2(p1.x, p3.y);
    Point p4(p3.x, p1.y);

    points.push_back(p1);
    points.push_back(p2);
    points.push_back(p3);
    points.push_back(p4);

}

double Rectangle::perimeter() const {
    return Polygon::perimeter();
}

Square::Square(Point p1, Point p2) : Rectangle(p1, p2) {

}

Ellipse::Ellipse(Point f1_, Point f2_, double c_) : focus1(f1_), focus2(f2_), a(c_ / 2.) {

}

double Ellipse::eccentricity() const {
    double c = sqrt(pow((focus2.y - focus1.y), 2) + pow((focus2.x - focus1.x), 2)) / 2;
    double e = c / a;
    return e;
}

double Ellipse::perimeter() const {
    double perimeter = 4 * a * std::comp_ellint_2(eccentricity());
    return perimeter;
}

double Ellipse::area() const {

    double c = sqrt(pow((focus2.y - focus1.y), 2) + pow((focus2.x - focus1.x), 2)) / 2;
    double e = c / a;

    double b = a * sqrt(1 - e * e);

    return M_PI * a * b;
}

void Ellipse::rotate(Point center, double angle) {

}

void Ellipse::scale(Point center, double coefficient) {

}


Circle::Circle(Point p1, double r) : Ellipse(p1, p1, r * 2.) {

}

Point Circle::center() {
    return focus1;
}

double Circle::radius() {
    return a;
}

bool Circle::operator==(const Circle& circle) const {
    return a == circle.a;
}
