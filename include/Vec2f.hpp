/*
  Vec2f.hpp
  2D vector construct to manage and use 2D numbers. This class
  defines operations and conversions to provide the basic unit
  structure of a 2D game.
*/

#pragma once

#include <string>
#include <SFML/System/Vector2.hpp>

class Vec2f {
 public:
  float x = 0;
  float y = 0;

  Vec2f();
  Vec2f(float xin, float yin);

  bool operator==(const Vec2f &other) const;
  bool operator!=(const Vec2f &other) const;
  Vec2f operator+(const Vec2f &other) const;
  Vec2f operator-(const Vec2f &other) const;
  Vec2f operator*(const float value) const;
  Vec2f operator/(const float value) const;

  void operator+=(const Vec2f &other);
  void operator-=(const Vec2f &other);
  void operator*=(const float value);
  void operator/=(const float value);

  // Scalar distance (magnitude only)
  float scalarDist(const Vec2f & other) const;

  // Returns a new vector that is normal (no changes)
  Vec2f normal() const;

  // Normalizes our vector (changes values)
  void normalize();

  // Returns the angle in degress of the vector
  float angle() const;

  // Basically the magnitude of the vector
  float length() const;

  // Returns distance vector between 2 points
  Vec2f dist(const Vec2f & other) const;

  // Returns a string representation of the vector,
  // mostly for logging.
  const std::string str() const;

  // Utility conversions to make SFML happy
  sf::Vector2f toVector2f() const;
  sf::Vector2i toVector2i() const;
};
