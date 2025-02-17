#include "Vector2Int.h"


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

void Vector2Int::operator=(const Vector2Int& other) {
    x = other.x;
    y = other.y;
}