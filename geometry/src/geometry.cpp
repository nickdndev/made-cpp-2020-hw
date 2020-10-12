//#pragma once
#include <algorithm>
#include <utility>
#include <vector>
#include <complex>
#include "geometry.h"

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

Line::Line(Point p1_, Point p2_) : p1(p1_), p2(p2_) {

}

Line::Line(Point p1, double c1_) : p1(p1), c1(c1_) {

    p2 = Point(c1_, c1_);
}

Line::Line(double k_, double c1_) : k(k_), c1(c1_) {

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

unsigned int Polygon::perimeter() const {
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

unsigned int Triangle::perimeter() const {
    return 0;
}


Triangle::Triangle(Point p1_, Point p2_, Point p3_) : Polygon({p1_, p2_, p3_}) {

}


