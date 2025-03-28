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

Vector2 Vector2::Lerp(const Vector2& a, const Vector2& b, float t)
{
    return a + (b - a) * t;
}

Vector2 Vector2::Direction(const Vector2& from, const Vector2& to)
{
    return (to - from).normalized();
}

bool Vector2::Approximately(const Vector2& a, const Vector2& b, float epsilon)
{
    return (std::abs(a.x - b.x) < epsilon) && (std::abs(a.y - b.y) < epsilon);
}

float Vector2::Distance(const Vector2& a, const Vector2& b) {
    float dx = a.x - b.x;
    float dy = a.y - b.y;
    return sqrt(dx * dx + dy * dy);
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

Vector2& Vector2::operator+=(const Vector2& other) {
    x += other.x;
    y += other.y;
    return *this;
}

Vector2& Vector2::operator-=(const Vector2& other) {
    x -= other.x;
    y -= other.y;
    return *this;
}
Vector2& Vector2::operator*=(float scalar) {
    x *= scalar;
    y *= scalar;
    return *this;
}

Vector2& Vector2::operator/=(float scalar) {
    x /= scalar;
    y /= scalar;
    return *this;
}

void Vector2::operator=(const Vector2& other) {
    x = other.x;
    y = other.y;
}

Vector2::operator Vector2Int() const
{
    return { (int)x, (int)y };
}
