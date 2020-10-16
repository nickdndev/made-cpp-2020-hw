#pragma once

#include <cfloat>
#include <cmath>
#include <iostream>
#include <vector>

using namespace std;

namespace task {

const double ERROR_PRECISION = 1e-6;

vector<double> operator+(const vector<double> &vec1,
                         const vector<double> &vec2) {
  vector<double> vec3;
  vec3.reserve(vec1.size());

  for (size_t i = 0; i < vec1.size(); i++)
    vec3.push_back(vec1[i] + vec2[i]);

  return vec3;
}

vector<double> operator-(const vector<double> &vec1,
                         const vector<double> &vec2) {
  vector<double> vec3;
  vec3.reserve(vec1.size());

  for (size_t i = 0; i < vec1.size(); i++)
    vec3.push_back(vec1[i] - vec2[i]);

  return vec3;
}

vector<double> operator+(const vector<double> &vec) {
  vector<double> vec2;
  vec2.reserve(vec2.size());

  for (size_t i = 0; i < vec.size(); i++)
    vec2.push_back(vec[i]);

  return vec2;
}

vector<double> operator-(const vector<double> &vec) {
  vector<double> vec2;
  vec2.reserve(vec2.size());

  for (size_t i = 0; i < vec.size(); i++)
    vec2.push_back(-1 * vec[i]);

  return vec2;
}

double operator*(const vector<double> &vector1, const vector<double> &vector2) {
  double dot_product = 0;

  for (size_t i = 0; i < vector1.size(); i++)
    dot_product += vector1[i] * vector2[i];

  return dot_product;
}

vector<double> operator%(const vector<double> &vec1,
                         const vector<double> &vec2) {
  vector<double> vec3(3, 0);

  vec3[0] = vec1[1] * vec2[2] - vec1[2] * vec2[1];
  vec3[1] = -1 * (vec1[0] * vec2[2] - vec1[2] * vec2[0]);
  vec3[2] = vec1[0] * vec2[1] - vec1[1] * vec2[0];

  return vec3;
}

double normVector(const vector<double> &vec1) {
  double norm = 0;

  for (double item : vec1) {
    norm += item * item;
  }

  return norm;
}

bool operator||(const vector<double> &vec1, const vector<double> &vec2) {

  double normVec1 = normVector(vec1);
  double normVec2 = normVector(vec2);

  if (normVec1 <= 0 || normVec2 <= 0)
    return true;

  double res = abs((vec1 * vec2) / (sqrt(normVec1) * sqrt(normVec2)));
  return abs(res - 1) < ERROR_PRECISION;
}

bool operator&&(const vector<double> &vec1, const vector<double> &vec2) {

  double normVec1 = normVector(vec1);
  double normVec2 = normVector(vec2);

  if (normVec1 <= 0 || normVec2 <= 0)
    return true;

  double scal_result = vec1 * vec2;
  double res = (scal_result) / (sqrt(normVec1) * sqrt(normVec2));
  return abs(res - 1) < ERROR_PRECISION && scal_result > 0;
}

void reverse(vector<double> &vec1) {
  for (size_t i = 0; i < vec1.size() / 2; i++)
    swap(vec1[i], vec1[vec1.size() - 1 - i]);
}

vector<int> operator|(const vector<int> &vec1, const vector<int> &vec2) {
  vector<int> vec3;
  vec3.reserve(vec1.size());

  for (size_t i = 0; i < vec1.size(); i++)
    vec3.push_back(vec1[i] | vec2[i]);

  return vec3;
}

vector<int> operator&(const vector<int> &vec1, const vector<int> &vec2) {
  vector<int> vec3;
  vec3.reserve(vec1.size());

  for (size_t i = 0; i < vec1.size(); i++)
    vec3.push_back(vec1[i] & vec2[i]);

  return vec3;
}

stringstream &operator>>(stringstream &stream, vector<double> &vec) {
  int size;
  double item;

  stream >> size;

  vec.resize(size);

  for (size_t i = 0; i < size; i++) {
    stream >> item;
    vec[i] = item;
  }

  return stream;
}

stringstream &operator<<(stringstream &stream, const vector<double> &vec) {
  for (double i : vec) {
    stream << i << ' ';
  }

  stream << '\n';

  return stream;
}
} // namespace task
