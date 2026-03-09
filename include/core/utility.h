#ifndef UTILITY_H
#define UTILITY_H

struct Vec2 {
    double x, y;

    // default constructor
    constexpr Vec2() : x(0), y(0) {}

    // angle constructor
    Vec2(double angle_degrees);

    // vector constructor
    constexpr Vec2(double x, double y) : x(x), y(y) {}

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

    constexpr Vec2& operator*=(double value) {
        x *= value;
        y *= value;
        return *this;
    }

    constexpr Vec2 operator*(double value) const { return Vec2(x * value, y * value); }

    constexpr Vec2 operator/(const Vec2& other) const { return Vec2(x / other.x, y / other.y); }

    constexpr Vec2 operator/=(const Vec2& other) {
        x /= other.x;
        y /= other.y;
        return *this;
    }

    constexpr Vec2& operator/=(double value) {
        // TODO: evaluate whether this should check for divide by zero errors
        x /= value;
        y /= value;
        return *this;
    }

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
