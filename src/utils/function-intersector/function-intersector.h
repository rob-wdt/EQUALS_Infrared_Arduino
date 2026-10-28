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
};
