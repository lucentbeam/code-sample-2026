#ifndef UTILITY_H
#define UTILITY_H

struct Vec2 {
    double x, y;

    // vector constructor
    constexpr Vec2(double x = 0, double y = 0) : x(x), y(y) {}

    // angle constructor
    Vec2(double angle_degrees);

    // operator overloads
    constexpr Vec2 operator+(const Vec2& other) const { return Vec2(x + other.x, y + other.y); }

    constexpr Vec2& operator+=(const Vec2& other) {
        x += other.x;
        y += other.y;
        return *this;
    }

    constexpr Vec2 operator-(const Vec2& other) const { return Vec2(x - other.x, y - other.y); }

    constexpr Vec2& operator-=(const Vec2& other) {
        x -= other.x;
        y -= other.y;
        return *this;
    }

    constexpr Vec2 operator*(const Vec2& other) const { return Vec2(x * other.x, y * other.y); }

    constexpr Vec2 operator*(double value) const { return Vec2(x * value, y * value); }

    constexpr Vec2 operator/(const Vec2& other) const { return Vec2(x / other.x, y / other.y); }

    constexpr Vec2 operator/(double value) const { return Vec2(x / value, y / value); }

    // vec2 helpers
    constexpr double dot(const Vec2& other) const { return x * other.x + y * other.y; }

    Vec2 rot(double angle_degrees) const;
    double length() const;
    Vec2 normalized() const;
    Vec2 projectedOnto(const Vec2& other) const;

    constexpr Vec2 right() const { return Vec2(-y, x); }
};

#endif // UTILITY_H
