#include "utility.h"

#include <cmath>
#include <limits>

namespace {
    constexpr double deg_to_rad = 3.1416926 / 180;
}

Vec2::Vec2(double angle_degrees) :
    x(std::cos(angle_degrees * deg_to_rad)),
    y(std::sin(angle_degrees * deg_to_rad))
{

}

Vec2 Vec2::rot(double angle_degrees) const
{
    angle_degrees *= deg_to_rad;
    double sin = std::sin(angle_degrees);
    double cos = std::cos(angle_degrees);
    return Vec2(x * cos - y * sin, x * sin + y * cos);
}

double Vec2::length() const
{
    return std::sqrt(x * x + y * y);
}

Vec2 Vec2::normalized() const
{
    double l = length();
    if (std::fabs(l) < std::numeric_limits<double>::epsilon()) return Vec2();
    return *this / l;
}

Vec2 Vec2::projectedOnto(const Vec2 &other) const
{
    return other.normalized() * dot(other);
}
