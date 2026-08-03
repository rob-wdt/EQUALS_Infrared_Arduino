#include "PID-control.h"

PIDControl::PIDControl(double kp, double ki, double kd) : Control(), _kp(kp), _ki(ki), _kd(kd) {}

double PIDControl::calculate(double error)
{
    _sum_errors += error;
    double u = _kp * error + _ki * _sum_errors + _kd * (error - _prev_error);
    _prev_error = error;
    return u;
}
