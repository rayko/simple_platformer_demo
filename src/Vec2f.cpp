#include "Vec2f.hpp"
#include <cmath>

Vec2f::Vec2f() {}
Vec2f::Vec2f(float xin, float yin) : x(xin), y(yin) {}

bool Vec2f::operator==(const Vec2f &other) const {
  return (x == other.x && y == other.y);
}

bool Vec2f::operator!=(const Vec2f &other) const {
  return !(x == other.x && y == other.y);
}

Vec2f Vec2f::operator+(const Vec2f &other) const {
  return Vec2f(x + other.x, y + other.y);
}

Vec2f Vec2f::operator-(const Vec2f &other) const {
  return Vec2f(x - other.x, y - other.y);
}

Vec2f Vec2f::operator*(const float value) const {
  return Vec2f(x * value, y * value);
}

Vec2f Vec2f::operator/(const float value) const {
  return Vec2f(x / value, y / value);
}

void Vec2f::operator+=(const Vec2f &other) {
  x += other.x;
  y += other.y;
}

void Vec2f::operator-=(const Vec2f &other) {
  x -= other.x;
  y -= other.y;
}

void Vec2f::operator*=(const float value) {
  x *= value;
  y *= value;
}

void Vec2f::operator/=(const float value) {
  x /= value;
  y /= value;
}

float Vec2f::scalarDist(const Vec2f & other) const {
  return sqrt(pow(other.x - x, 2) + pow(other.y - y, 2));
}

float Vec2f::length() const {
  return sqrt((x*x) + (y*y));
}

void Vec2f::normalize() {
  float myLenth = length();
  x /= myLenth;
  y /= myLenth;
}

Vec2f Vec2f::normal() const {
  float myLenth = length();
  return Vec2f(x/myLenth, y/myLenth);
}

float Vec2f::angle() const {
  float pi = std::numbers::pi_v<float>;
  float rads = atan2(y, x);
  float degs = rads * (180.0f / pi);
  if (degs < 0)
    degs += 360;
  return degs;
}

Vec2f Vec2f::dist(const Vec2f & other) const {
  return Vec2f(other.x - x, other.y -y);
}

const std::string Vec2f::str() const {
  return "(" + std::to_string(x) + "," + std::to_string(y) + ")";
}
