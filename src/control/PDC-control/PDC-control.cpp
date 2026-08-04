#include "PDC-control.h"
#include "utils/math/math-utils.h"

PDCControl::PDCControl(PDCCoefficients coefficients) : _kp(coefficients.kp), _kd(coefficients.kd), _kc(coefficients.kc) {}

double PDCControl::calculate(double error)
{
    double u = _kp * error + _kd * (error - _prev_error) + _kc * math_utils::pow(error, 3);

    _prev_error = error;

    return u;
}
