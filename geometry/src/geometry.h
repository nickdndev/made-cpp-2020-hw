#pragma once

#include <vector>

using namespace std;


struct Point {
    double x, y;

    Point(double x, double y);

    bool operator==(const Point& point) const;

    bool operator!=(const Point& point) const;

    Point &operator+=(const Point &a);

    Point &operator-=(const Point &a);

    Point &operator*=(const Point &a);

    Point &operator*=(const double &number);

    Point operator+(const Point &a) const;

    Point operator-(const Point &a) const;

    Point operator*(const Point &a) const;

    Point operator*(const double &a) const;

};


class Line {
    double c1, k;
    Point p1 = p2, p2 = p1;

public:
    Line(Point p1, Point p2);

    Line(Point p1, double c1);

    Line(double k, double c1);

    bool operator==(const Point& point) const;

    bool operator!=(const Point& point) const;

};

class Shape {
public:
    virtual unsigned perimeter() const = 0;

    virtual double area() const = 0;

    virtual void rotate(Point center, double angle) = 0;
    // virtual bool operator==(const Shape& another);
};


class Polygon : public Shape {

protected:
    vector<Point> points;


public:
    Polygon(vector<Point> points);

    unsigned int perimeter() const override;

    double area() const override;

    void rotate(Point center, double angle);

    void scale(Point center, double coefficient);



    bool operator==(const Polygon& polygon) const;

    bool operator!=(const Polygon& polygon) const;

};

class Triangle : public Polygon {
    using Polygon::Polygon;


public:

    Triangle(Point p1, Point p2, Point p3);

    unsigned int perimeter() const override;

    // double area() const override;


};