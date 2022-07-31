//
// Created by knyaz on 7/2/2022.
//

#ifndef SABT_ANGLE_H
#define SABT_ANGLE_H

#include <cmath>
/*
 * Angle with automatic radians-degrees conversion and angle-save maths.
 */
class Angle {
protected:

    // Angle in radians, from -PI to PI
    double value{};

    // Called from specific ::deg or ::rad constructors
    explicit Angle(double radians);

public:

    // Some useful constants

    constexpr static double PI = 3.14159265359;
    constexpr static double TWO_PI = 2 * PI;
    constexpr static double HALF_PI = PI / 2.0;
    constexpr static double DEG_TO_RAD = PI / 180.0;
    constexpr static double RAD_TO_DEG = 180.0 / PI;

    // Custom constructors

    static Angle deg(double x);

    static Angle rad(double x);

    // Default constructor

    Angle() = default;

    // Maths

    Angle operator+(Angle other) const;

    Angle operator-(Angle other) const;

    Angle operator*(double factor) const;

    Angle operator/(double factor) const;

    Angle& operator=(Angle other);

    // Get the angle value

    [[nodiscard]] double deg() const;

    [[nodiscard]] double rad() const;

    // Trigonometry

    [[nodiscard]] double cos() const;

    [[nodiscard]] double sin() const;

    [[nodiscard]] double tan() const;


    // Makes the angle saved value range correct.
    // It is not always required, so it is just an option.
    void norm();
};


#endif //SABT_ANGLE_H
