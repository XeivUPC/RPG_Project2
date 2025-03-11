#include "Vector2.h"
#include "Vector2Int.h"
#include <iostream>
#include <cmath>

Vector2::Vector2() : x(0), y(0) {}
Vector2::Vector2(const Vector2& other) : x(other.x), y(other.y) {}
Vector2::Vector2(float x, float y) : x(x), y(y) {}

float Vector2::magnitude() const {
    return (float)sqrt(x * x + y * y);
}

float Vector2::dot(const Vector2& other) const {
    return (x * other.x + y * other.y);
}

float Vector2::cross(const Vector2& other) const {
    return (x * other.y - y * other.x);
}

Vector2 Vector2::normalized() const {
    float mag = magnitude();
    if (mag == 0) {
        throw invalid_argument("Cannot normalize a zero vector");
    }
    return *this / mag;
}

void Vector2::normalize() {
    float mag = magnitude();
    if (mag == 0) {
        throw invalid_argument("Cannot normalize a zero vector");
    }
    x /= mag;
    y /= mag;
}

bool Vector2::operator==(const Vector2& other) const {
    return (x == other.x && y == other.y);
}

bool Vector2::operator!=(const Vector2& other) const {
    return !(*this == other);
}

bool Vector2::operator<(const Vector2& other) const {
    return this->magnitude() < other.magnitude();
}

bool Vector2::operator<=(const Vector2& other) const {
    return this->magnitude() <= other.magnitude();
}

bool Vector2::operator>(const Vector2& other) const {
    return this->magnitude() > other.magnitude();
}

bool Vector2::operator>=(const Vector2& other) const {
    return this->magnitude() >= other.magnitude();
}

Vector2 Vector2::operator+(const Vector2& other) const {
    return Vector2(x + other.x, y + other.y);
}

Vector2 Vector2::operator-(const Vector2& other) const {
    return Vector2(x - other.x, y - other.y);
}

Vector2 Vector2::operator*(float scalar) const {
    return Vector2(x * scalar, y * scalar);
}

Vector2 Vector2::operator/(float scalar) const {
    if (scalar == 0) {
        throw invalid_argument("Division by zero");
    }
    return Vector2(x / scalar, y / scalar);
}

void Vector2::operator=(const Vector2& other) {
    x = other.x;
    y = other.y;
}

Vector2::operator Vector2Int() const
{
    return { (int)x, (int)y };
}
