#pragma once
#include "control/control.h"

struct PDCCoefficients
{
    double kp;
    double kd;
    double kc;
};

class PDCControl : public Control
{
public:
    PDCControl(PDCCoefficients coefficients);
    double calculate(double error) override;

private:
    double _kp, _kd, _kc;

    double _prev_error = 0;
};
