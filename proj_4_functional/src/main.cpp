#include "function.h"
#include <iostream>
#include <list>

#include "compose.h"
#include <cassert>

int foo() { return 1; }

int bar() { return 2; }

double tap() { return 10; }

int main() {
  {
    function<int(void)> f(foo);
    assert(f() == 1);
  }

  {
    function<int(void)> b1(bar);
    function<int(void)> second(b1);
    assert(second() == 2);
  }
  {
    function<int(void)> b2(bar);
    function<int(void)> second2(std::move(b2));
    assert(second2() == 2);
  }

  {
    function<int(void)> b(bar);
    function<int(void)> second = b;
    assert(second() == 2);
  }

  {
    int a = 10;
    function<int(int)> l = [a](int x) { return a + x; };
    assert(l(5) == 15);
  }

  {
    double a = 2;
    function<double(double)> l = [a](double x) { return a * x; };
    assert(l(11.1) == 22.2);
  }

  {

    function<int()> f(foo);
    function<int()> b(bar);
    assert(f() == 1);
    assert(b() == 2);

    f.swap(b);

    assert(f() == 2);
    assert(b() == 1);
  }
  {
    function<int()> f = foo;
    assert(f() == 1);
    f = tap;
    assert(tap() == 10);
  }
  {
    function<int(void)> f(nullptr);
    assert(!f);
    f = foo;
    assert(f);
  }
  {

    function<double(int)> f = [](auto i) { return i + 3; };
    function<int(double)> g = [](auto i) { return i * 2; };
    function<int(int)> h = [](auto i) { return i + 1; };

    assert(9 == compose(f, g, f)(0));
    assert(5 == compose(f, g, h)(0));
  }

  {

    const function<bool(int)> a = [](int x) -> bool {
      std::cout << "a" << std::endl;
      return x > 10;
    };

    const function<int(float)> b = [](float y) -> int {
      std::cout << "b" << std::endl;
      return int(y * y);
    };

    const function<float(bool)> c = [](bool z) -> float {
      std::cout << "c" << std::endl;
      return z ? 3.1f : 3.34f;
    };

    auto d = compose(a, b, c, a, b, c);
    std::cout << d(true) << std::endl;
    assert(d(true));
  }
}