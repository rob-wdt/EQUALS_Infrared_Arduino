#pragma once

#include <Arduino.h>
#include "utils/point/point.h"

// Angles are field headings like everywhere on the robot: 0 = +y (towards the opponent goal), 90 = +x (to the right)
class MathVector
{
public:
    MathVector(point origin, point end) : _origin(origin), _end(end) {}
    MathVector(point origin, float x_projection, float y_projection) : _origin(origin)
    {
        _end.x = _origin.x + x_projection;
        _end.y = _origin.y + y_projection;
    }
    // Not a constructor: MathVector(origin, 45, 10) would be ambiguous with the projections one
    static MathVector from_angle(point origin, float angle, float length)
    {
        return MathVector(origin, length * sin(radians(angle)), length * cos(radians(angle)));
    }

    point origin() const { return _origin; }
    point end() const { return _end; }
    float length() const { return sqrt(x_projection() * x_projection() + y_projection() * y_projection()); }
    float x_projection() const { return _end.x - _origin.x; }
    float y_projection() const { return _end.y - _origin.y; }
    float angle() const { return degrees(atan2(x_projection(), y_projection())); }
    // The same direction with length 1, a zero vector stays zero
    MathVector unit() const
    {
        float l = length();
        return l > 0 ? MathVector(_origin, x_projection() / l, y_projection() / l) : *this;
    }

private:
    point _origin;
    point _end;
};

inline MathVector operator+(const MathVector &v1, const MathVector &v2) { return MathVector(v1.origin(), v1.x_projection() + v2.x_projection(), v1.y_projection() + v2.y_projection()); }

inline MathVector operator-(const MathVector &v1, const MathVector &v2) { return MathVector(v1.origin(), v1.x_projection() - v2.x_projection(), v1.y_projection() - v2.y_projection()); }

inline MathVector operator*(const MathVector &vector, float scalar) { return MathVector(vector.origin(), vector.x_projection() * scalar, vector.y_projection() * scalar); }

inline MathVector operator/(const MathVector &vector, float scalar) { return MathVector(vector.origin(), vector.x_projection() / scalar, vector.y_projection() / scalar); }

// > 0 when the vectors point roughly the same way, < 0 when roughly opposite
inline float dot(const MathVector &v1, const MathVector &v2) { return v1.x_projection() * v2.x_projection() + v1.y_projection() * v2.y_projection(); }

// > 0 when v2 is turned counterclockwise from v1 (on the field: to the left), < 0 clockwise
inline float cross(const MathVector &v1, const MathVector &v2) { return v1.x_projection() * v2.y_projection() - v1.y_projection() * v2.x_projection(); }

inline MathVector parallel_transition(MathVector vector, point new_origin) { return MathVector(new_origin, vector.x_projection(), vector.y_projection()); }
