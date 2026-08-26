#include "math-utils.h"

namespace math_utils
{
    double pow(double base, int exp)
    {
        double res = 1;

        if (exp >= 0)
        {
            for (int i = 0; i < exp; i++)
            {
                res *= base;
            }
        }
        else
        {
            for (int i = 0; i > exp; i--)
            {
                res /= base;
            }
        }

        return res;
    }
};
