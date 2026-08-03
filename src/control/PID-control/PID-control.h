#pragma once
#include "control/control.h"

class PIDControl : public Control
{
public:
    PIDControl(double kp, double ki, double kd);
    double calculate(double error) override;

private:
    double _kp;
    double _ki;
    double _kd;

    double _sum_errors = 0;
    double _prev_error = 0;
};
