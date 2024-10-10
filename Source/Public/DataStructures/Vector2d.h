//
// Created by MrHaloka.
//

#ifndef VECTOR2D_H
#define VECTOR2D_H

#include <cmath>

struct Vector2d {
    float x;
    float y;

public:
    Vector2d() : x(0), y(0) {
    }

    Vector2d(float x, float y) : x(x), y(y) {
    }

    Vector2d(float value) : x(value), y(value) {
    }

    Vector2d(int x, int y) : x(x), y(y) {
    }

    Vector2d(int value) : x(value), y(value) {
    }

    Vector2d(uint32_t x, uint32_t y) : x(x), y(y) {
    };

    Vector2d operator*(const float multiplier) const {
        return {x * multiplier, y * multiplier};
    }

    Vector2d operator+(const Vector2d& other) const {
        return Vector2d(x + other.x, y + other.y);
    }

    Vector2d operator+(const uint16_t& other) const {
        return Vector2d(x + other, y + other);
    }

    double Distance(const Vector2d& vector2d) const{
        return std::sqrt((x - vector2d.x) * (x - vector2d.x) + (y - vector2d.y) * (y - vector2d.y));
    }

    int GetXInt() const {
        return x;
    }

    int GetYInt() const {
        return y;
    }
};


#endif //VECTOR2D_H
