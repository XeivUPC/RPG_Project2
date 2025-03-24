#include "Vector2Int.h"
#include "Vector2.h"


Vector2Int::Vector2Int() : x(0), y(0) {}
Vector2Int::Vector2Int(const Vector2Int& other) : x(other.x), y(other.y) {}
Vector2Int::Vector2Int(int x, int y) : x(x), y(y) {}

float Vector2Int::magnitude() const {
    return (float)sqrt(x * x + y * y);
}

int Vector2Int::dot(const Vector2Int& other) const {
    return (x * other.x + y * other.y);
}

int Vector2Int::cross(const Vector2Int& other) const {
    return (x * other.y - y * other.x);
}

Vector2Int Vector2Int::normalized() const {
    float mag = magnitude();
    if (mag == 0) {
        throw invalid_argument("Cannot normalize a zero vector");
    }
    return *this / mag;
}

void Vector2Int::normalize() {
    float mag = magnitude();
    if (mag == 0) {
        throw invalid_argument("Cannot normalize a zero vector");
    }
    x = (int)(x / mag);
    y = (int)(y / mag);
}

Vector2Int Vector2Int::Lerp(const Vector2Int& a, const Vector2Int& b, float t)
{
    return a + (b - a) * t;
}

Vector2Int Vector2Int::Direction(const Vector2Int& from, const Vector2Int& to)
{
    return (to - from).normalized();
}

bool Vector2Int::Approximately(const Vector2Int& a, const Vector2Int& b, float epsilon)
{
    return (std::abs(a.x - b.x) < epsilon) && (std::abs(a.y - b.y) < epsilon);
}

bool Vector2Int::operator==(const Vector2Int& other) const {
    return (x == other.x && y == other.y);
}

bool Vector2Int::operator!=(const Vector2Int& other) const {
    return !(*this == other);
}

bool Vector2Int::operator<(const Vector2Int& other) const {
    return this->magnitude() < other.magnitude();
}

bool Vector2Int::operator<=(const Vector2Int& other) const {
    return this->magnitude() <= other.magnitude();
}

bool Vector2Int::operator>(const Vector2Int& other) const {
    return this->magnitude() > other.magnitude();
}

bool Vector2Int::operator>=(const Vector2Int& other) const {
    return this->magnitude() >= other.magnitude();
}

Vector2Int Vector2Int::operator+(const Vector2Int& other) const {
    return Vector2Int(x + other.x, y + other.y);
}

Vector2Int Vector2Int::operator-(const Vector2Int& other) const {
    return Vector2Int(x - other.x, y - other.y);
}

Vector2Int Vector2Int::operator*(float scalar) const {
    return Vector2Int((int)(x * scalar), (int)(y * scalar));
}

Vector2Int Vector2Int::operator/(float scalar) const {
    if (scalar == 0) {
        throw invalid_argument("Division by zero");
    }
    return Vector2Int((int)(x / scalar), (int)(y / scalar));
}

Vector2Int& Vector2Int::operator+=(const Vector2Int& other) {
    x += other.x;
    y += other.y;
    return *this;
}


Vector2Int& Vector2Int::operator-=(const Vector2Int& other) {
    x -= other.x;
    y -= other.y;
    return *this;
}

Vector2Int& Vector2Int::operator*=(float scalar) {
    x *= scalar;
    y *= scalar;
    return *this;
}

Vector2Int& Vector2Int::operator/=(float scalar) {
    x /= scalar;
    y /= scalar;
    return *this;
}

void Vector2Int::operator=(const Vector2Int& other) {
    x = other.x;
    y = other.y;
}


Vector2Int::operator Vector2() const
{
    return { (float)x, (float)y };
}