//
// Created by knyaz on 7/2/2022.
//

#include "Angle.h"

constexpr Angle::Angle(const double radians) {
    value = radians;
}

constexpr Angle Angle::operator+(Angle other) const {
    return Angle::rad(value + other.rad());
}

constexpr Angle Angle::operator-(Angle other) const {
    return Angle::rad(value - other.rad());
}

constexpr Angle Angle::operator*(double factor) const {
    return Angle::rad(value * factor);
}

constexpr Angle Angle::operator/(double factor) const {
    return Angle::rad(value / factor);
}

constexpr double Angle::deg() const {
    return value * DEG_TO_RAD;
}

constexpr double Angle::rad() const {
    return value;
}

constexpr double Angle::cos() const {
    return ::cos(value);
}

constexpr double Angle::sin() const {
    return ::sin(value);
}

constexpr double Angle::tan() const {
    return ::tan(value);
}

constexpr Angle Angle::deg(const double x) {
    return Angle(x * DEG_TO_RAD);
}

constexpr Angle Angle::rad(const double x) {
    return Angle(x);
}

constexpr void Angle::norm() {
    while (value > PI) {
        value -= TWO_PI;
    }

    while (value <= -PI) {
        value += TWO_PI;
    }
}
