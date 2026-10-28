#pragma once

class MathFunction
{
};

// linear function: y = kx + b
class LinearFunction : public MathFunction
{
public:
    LinearFunction(float k_coef, float b_koef) : _k_coef(k_coef), _b_koef(b_koef) {}
    float k_coef() { return _k_coef; }
    float b_koef() { return _b_koef; }

private:
    float _k_coef;
    float _b_koef;
};
