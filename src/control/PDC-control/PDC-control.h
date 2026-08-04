#pragma once
#include "control/control.h"

class PDCControl : public Control
{
public:
    PDCControl(double kp, double kd, double kc);
    double calculate(double error) override;

private:
    double _kp, _kd, _kc;

    double _prev_error = 0;
};
