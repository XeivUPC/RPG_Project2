#pragma once
#include <iostream>
#include <cmath>

using namespace std;

struct Vector2;

struct Vector2Int {
public:
    int x, y;

public:

    Vector2Int();
    Vector2Int(const Vector2Int& other);
    Vector2Int(int x, int y);

    float magnitude() const;

    int dot(const Vector2Int& other) const;

    int cross(const Vector2Int& other) const;

    Vector2Int normalized() const;

    void normalize();

    static Vector2Int Lerp(const Vector2Int& a, const Vector2Int& b, float t);

    static Vector2Int Direction(const Vector2Int& from, const Vector2Int& to);

    static bool Approximately(const Vector2Int& a, const Vector2Int& b, float epsilon = 1e-5f);

    bool operator==(const Vector2Int& other) const;

    bool operator!=(const Vector2Int& other) const;

    bool operator<(const Vector2Int& other) const;

    bool operator<=(const Vector2Int& other) const;

    bool operator>(const Vector2Int& other) const;

    bool operator>=(const Vector2Int& other) const;

    Vector2Int operator+(const Vector2Int& other) const;

    Vector2Int operator-(const Vector2Int& other) const;

    Vector2Int operator*(float scalar) const;

    Vector2Int operator/(float scalar) const;

    Vector2Int& operator+=(const Vector2Int& other);

    Vector2Int& operator-=(const Vector2Int& other);

    Vector2Int& operator*=(float scalar);

    Vector2Int& operator/=(float scalar);

    void operator=(const Vector2Int& other);

    operator Vector2() const;
};
