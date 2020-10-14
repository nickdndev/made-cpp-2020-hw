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

    bool operator==(const Point& point) const;

    bool operator!=(const Point& point) const;

    Point& operator+=(const Point& a);

    Point& operator-=(const Point& a);

    Point& operator*=(const Point& a);

    Point& operator*=(const double& number);

    Point operator+(const Point& a) const;

    Point operator-(const Point& a) const;

    Point operator*(const Point& a) const;

    Point operator*(const double& a) const;

    Point& operator=(const Point& a);

};


class Line {
    double c1, k;
    Point p1 = p2, p2 = p1;

public:
    double A, B, C;

    Line(Point p1, Point p2);

    Line(Point p1, double c1);

    Line(double k, double c1);

    bool operator==(const Point& point) const;

    bool operator!=(const Point& point) const;

};

class Shape {
public:
    virtual double perimeter() const = 0;

    virtual double area() const = 0;

    virtual void rotate(Point center, double angle) = 0;

    virtual void scale(Point center, double coefficient) = 0;
    // virtual bool operator==(const Shape& another);
};


class Polygon : public Shape {

protected:
    vector<Point> points;


public:
    Polygon(vector<Point> points);

    double perimeter() const override;

    double area() const override;

    void rotate(Point center, double angle);

    void scale(Point center, double coefficient) override;

    void reflex(const Line& line);

    vector<Point> getVertices();

    bool operator==(const Polygon& polygon) const;

    bool operator!=(const Polygon& polygon) const;

    Polygon& operator=(const Polygon& a);

};

class Triangle : public Polygon {
    using Polygon::Polygon;


public:

    Triangle(Point p1, Point p2, Point p3);

    double perimeter() const override;

    Circle inscribedCircle();

    Circle circumscribedCircle();

    Line EulerLine();

    Point centroid();


};

class Rectangle : public Polygon {

public:

    Rectangle(Point p1, Point p2, int k);

    double perimeter() const override;

    // double area() const override;


};


class Square : public Rectangle {

public:

    Square(Point p1, Point p2);


};

class Ellipse : public Shape {

protected:

    Point focus1;
    Point focus2;
    double a;


public:

    Ellipse(Point f1, Point f2, double c);

    double perimeter() const override;

    double area() const override;

    void rotate(Point center, double angle) override;

    void scale(Point center, double coefficient) override;

    double eccentricity() const;
};

class Circle : public Ellipse {

public:

    Circle(Point p1, double r);

    Point center();

    double radius();

    bool operator==(const Circle& circle) const;

};
