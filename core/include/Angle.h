//
// Created by knyaz on 7/2/2022.
//

#ifndef SABT_ANGLE_H
#define SABT_ANGLE_H

#include <cmath>

class Angle {
private:

    // Angle in radians, from -PI to PI
    double value{};

    // Called from specific ::deg or ::rad constructors
    constexpr explicit Angle(double radians);

public:

    // Some useful constants

    constexpr static double PI = 3.14159265359;
    constexpr static double TWO_PI = 2 * PI;
    constexpr static double HALF_PI = PI / 2.0;
    constexpr static double DEG_TO_RAD = PI / 180.0;
    constexpr static double RAD_TO_DEG = 180.0 / PI;

    // Custom constructors

    constexpr static Angle deg(double x);

    constexpr static Angle rad(double x);

    // Default constructor

    Angle() = default;

    // Maths

    constexpr Angle operator+(Angle other) const;

    constexpr Angle operator-(Angle other) const;

    constexpr Angle operator*(double factor) const;

    constexpr Angle operator/(double factor) const;

    // Get the angle value

    [[nodiscard]] constexpr double deg() const;

    [[nodiscard]] constexpr double rad() const;

    // Trigonometry

    [[nodiscard]] constexpr double cos() const;

    [[nodiscard]] constexpr double sin() const;

    [[nodiscard]] constexpr double tan() const;


    // Makes the angle saved value range correct.
    // It is not always required, so it is just an option.
    constexpr void norm();
};


#endif //SABT_ANGLE_H
