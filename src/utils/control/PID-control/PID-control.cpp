#include "PID-control.h"

PIDControl::PIDControl(PIDCoefficients coefficients) : _kp(coefficients.kp), _ki(coefficients.ki), _kd(coefficients.kd) {}

double PIDControl::calculate(double error)
{
    _sum_errors += error;
    double u = _kp * error + _ki * _sum_errors + _kd * (error - _prev_error);
    _prev_error = error;
    return u;
}
