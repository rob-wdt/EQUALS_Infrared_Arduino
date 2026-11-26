#include "button.h"

Button::Button(uint8_t pin) : _pin(pin) {}

void Button::init()
{
    pinMode(_pin, INPUT);
    reset();
}

void Button::read()
{
    bool pressed = digitalRead(_pin) == LOW;
    _was_pressed = pressed && !_is_pressed;
    _is_pressed = pressed;
}

bool Button::is_pressed() { return _is_pressed; }

bool Button::was_pressed() { return _was_pressed; }

void Button::reset()
{
    _is_pressed = false;
    _was_pressed = false;
}

void Button::wait_click()
{
    reset();
    while (!is_pressed())
        read();
    while (is_pressed())
        read();
}

int Button::pin() { return _pin; }
