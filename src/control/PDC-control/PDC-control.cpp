#include "PDC-control.h"
#include "utils/math/math-utils.h"

PDCControl::PDCControl(double kp, double kd, double kc) : Control(), _kp(kp), _kd(kd), _kc(kc) {}

double PDCControl::calculate(double error)
{
    double u = _kp * error + _kd * (error - _prev_error) + _kc * math_utils::pow(error, 3);

    _prev_error = error;

    return u;
}
