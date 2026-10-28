#pragma once

#include "utils/math-function/math-function.h"
#include "utils/point/point.h"

// Calculates the intersection point of two math functions
class FunctionIntersector
{
public:
    // returns (0, 0) if there's no intersection or the functions' graphs equal
    point intersect(LinearFunction f1, LinearFunction f2)
    {
        point res = {0, 0};

        if (f1.k_coef() == f2.k_coef())
            return res;

        res.x = (f2.b_koef() - f1.b_koef()) / (f1.k_coef() - f2.k_coef()); // k1 * x + b1 = k2 * x + b2
        res.y = f1.k_coef() * res.x + f1.b_koef(); // y(x_res)

        return res;
    }

    // A vertical line always crosses a non-vertical one: at its x
    point intersect(LinearFunction f, VerticalLine v)
    {
        point res = {v.x(), f.k_coef() * v.x() + f.b_koef()}; // y(x = const)
        return res;
    }

    point intersect(VerticalLine v, LinearFunction f) { return intersect(f, v); }

    // returns (0, 0): two vertical lines are parallel or the same
    point intersect(VerticalLine v1, VerticalLine v2)
    {
        point res = {0, 0};
        return res;
    }
};
