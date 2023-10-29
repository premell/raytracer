#pragma once

#include <cmath>
// vector 2

struct Vec2 {
  float x, y;
};

struct Vec3 {
  float x, y, z;
};

// never used this for the union
// };
// float E[3];

// inline Vec3 vec3(float x, float y, float z) {
//   Vec3 Result;
//   Result.x = x;
//   Result.y = y;
//   Result.z = z;
//
//   return Result;
// }

inline Vec3 operator*(float b, Vec3 a) {
  a.x = b * a.x;
  a.y = b * a.y;
  a.z = b * a.z;
  return a;
}

inline Vec3 operator*(Vec3 b, float a) { return a * b; }

inline Vec3 operator*(Vec3 a, Vec3 b) {
  a.x = b.x * a.x;
  a.y = b.y * a.y;
  a.z = b.z * a.z;
  return a;
}

inline Vec3 operator+(Vec3 a, Vec3 b) {
  a.x = a.x + b.x;
  a.y = a.y + b.y;
  a.z = a.z + b.z;
  return a;
}

inline Vec3 operator-(Vec3 a, Vec3 b) {
  a.x = a.x - b.x;
  a.y = a.y - b.y;
  a.z = a.z - b.z;
  return a;
}

inline Vec3 operator-(Vec3 a) {
  a.x = -a.x;
  a.y = -a.y;
  a.y = -a.z;
  return a;
}

inline Vec3 operator*=(Vec3 &a, float b) {
  a = a * b;
  return a;
}

inline Vec3 operator/(Vec3 a, float b) {
  a.x = a.x / b;
  a.y = a.y / b;
  a.z = a.z / b;
  return a;
}

inline Vec3 operator+=(Vec3 &a, Vec3 b) {
  a = a + b;
  return a;
}

inline Vec3 operator-=(Vec3 &a, Vec3 b) {
  a = a - b;
  return a;
}

inline bool operator==(Vec3 a, Vec3 b) {
  return a.x == b.x && a.y == b.y && a.z == b.z;
}

inline bool operator!=(Vec3 a, Vec3 b) {
  return a.x != b.x || a.y != b.y || a.z != b.z;
}

inline float square(float a) { return a * a; }

inline float dot(Vec3 a, Vec3 b) { return a.x * b.x + a.y * b.y + a.z * b.z; }

inline Vec3 max(Vec3 a, Vec3 b) {
  a.x = std::max(a.x, b.x);
  a.y = std::max(a.y, b.y);
  a.z = std::max(a.z, b.z);
  return a;
}
inline Vec3 min(Vec3 a, Vec3 b) {
  a.x = std::min(a.x, b.x);
  a.y = std::min(a.y, b.y);
  a.z = std::min(a.z, b.z);
  return a;
}
inline Vec3 max(float b, Vec3 a) { return max({b, b, b}, a); }
inline Vec3 min(float b, Vec3 a) { return min({b, b, b}, a); }

inline Vec3 floor(Vec3 a) {
  a.x = std::floor(a.x);
  a.y = std::floor(a.y);
  a.z = std::floor(a.z);
  return a;
}

inline Vec3 ceil(Vec3 a) {
  a.x = std::ceil(a.x);
  a.y = std::ceil(a.y);
  a.z = std::ceil(a.z);
  return a;
}

inline Vec3 decimals(Vec3 a) {
  a.x = a.x - static_cast<int>(a.x);
  a.y = a.y - static_cast<int>(a.y);
  a.z = a.z - static_cast<int>(a.z);
  return a;
}

inline Vec3 sign(Vec3 a) {
  a.x = a.x > 0 ? 1 : -1;
  a.y = a.y > 0 ? 1 : -1;
  a.z = a.z > 0 ? 1 : -1;
  return a;
}

inline Vec3 abs(Vec3 a) {
  a.x = std::abs(a.x);
  a.y = std::abs(a.y);
  a.z = std::abs(a.z);
  return a;
}

inline float vec_length(Vec3 a) {
  return std::sqrt(a.x * a.x + a.y * a.y + a.z * a.z);
}

inline float vec_length_squared(Vec3 a) {
  return a.x * a.x + a.y * a.y + a.z * a.z;
}

inline Vec3 normalize(Vec3 a) {
  float length = vec_length(a);

  float tolerance = 0.01;
  if (length < tolerance) {
    return Vec3{0, 0, 0};
  }

  a.x = a.x / length;
  a.y = a.y / length;
  a.z = a.z / length;

  return a;
}

inline float random_double() {
  // Returns a random real in [0,1).
  return rand() / (RAND_MAX + 1.0);
}

inline float random_double(double min, double max) {
  // Returns a random real in [min,max).
  return min + (max - min) * random_double();
}

inline Vec3 random_vec() {
  return Vec3{random_double(), random_double(), random_double()};
}

inline Vec3 random_vec(double min, double max) {
  return Vec3{random_double(min, max), random_double(min, max),
              random_double(min, max)};
}

inline Vec3 random_vec_in_unit_sphere() {
  while (true) {
    auto p = random_vec(-1, 1);
    if (vec_length_squared(p) < 1)
      return p;
  }
}

inline Vec3 random_vec_on_unit_sphere() {
  return normalize(random_vec_in_unit_sphere());
}

// requires to be in the same direction as input vector
inline Vec3 random_vec_on_unit_hemosphere(Vec3 vector) {

  auto random_vec = normalize(random_vec_in_unit_sphere());

  if (dot(random_vec, vector) >= 0) {
    return random_vec;
  } else {
    return -random_vec;
  }
}


// rays
struct Ray {
  Vec3 origin;
  Vec3 direction;
};

inline Vec3 rayAt(Ray ray, float t) { return ray.origin + t * ray.direction; }

const double pi = 3.1415926535897932385;
inline double degrees_to_radians(double degrees) {
  return degrees * pi / 180.0;
}
