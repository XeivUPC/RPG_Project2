#pragma once
#include <iostream>
#include <cmath>

using namespace std;

struct Vector2Int;

struct Vector2 {
public:
    float x, y;
    
public:

    Vector2();
    Vector2(const Vector2& other);
    Vector2(float x, float y);

    float magnitude() const;

    float dot(const Vector2& other) const;

    float cross(const Vector2& other) const;

    Vector2 normalized() const;

    void normalize();

    bool operator==(const Vector2& other) const;

    bool operator!=(const Vector2& other) const;

    bool operator<(const Vector2& other) const;

    bool operator<=(const Vector2& other) const;

    bool operator>(const Vector2& other) const;

    bool operator>=(const Vector2& other) const;

    Vector2 operator+(const Vector2& other) const;

    Vector2 operator-(const Vector2& other) const;

    Vector2 operator*(float scalar) const;

    Vector2 operator/(float scalar) const;

    void operator=(const Vector2& other);

    operator Vector2Int() const;
};
