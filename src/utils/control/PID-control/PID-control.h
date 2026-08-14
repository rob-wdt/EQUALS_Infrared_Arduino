#pragma once
#include "utils/control/control.h"

struct PIDCoefficients
{
    double kp;
    double ki;
    double kd;
};

class PIDControl : public Control
{
public:
    PIDControl(PIDCoefficients coefficients);
    double calculate(double error) override;

private:
    double _kp, _ki, _kd;

    double _sum_errors = 0;
    double _prev_error = 0;
};
