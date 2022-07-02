//
// Created by knyaz on 7/2/2022.
//

#include "Angle.h"

Angle::Angle(const double radians) {
    value = radians;
}

Angle Angle::operator+(Angle other) const {
    return Angle::rad(value + other.rad());
}

Angle Angle::operator-(Angle other) const {
    return Angle::rad(value - other.rad());
}

Angle Angle::operator*(double factor) const {
    return Angle::rad(value * factor);
}

Angle Angle::operator/(double factor) const {
    return Angle::rad(value / factor);
}

double Angle::deg() const {
    return value * DEG_TO_RAD;
}

double Angle::rad() const {
    return value;
}

Angle Angle::deg(const double x) {
    return Angle(x * DEG_TO_RAD);
}

Angle Angle::rad(const double x) {
    return Angle(x);
}

double Angle::cos() const {
    return ::cos(value);
}

double Angle::sin() const {
    return ::sin(value);
}

double Angle::tan() const {
    return ::tan(value);
}

void Angle::norm() {
    while (value > PI) {
        value -= TWO_PI;
    }

    while (value <= -PI) {
        value += TWO_PI;
    }
}

Angle& Angle::operator=(Angle other) {
    value = other.rad();

    return *this;
}
