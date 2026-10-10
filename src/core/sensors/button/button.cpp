#include "button.h"

Button::Button(uint8_t pin) : _pin(pin) {}

void Button::init() { pinMode(_pin, INPUT); }

void Button::read() { _is_pressed = digitalRead(_pin); }

bool Button::is_pressed() { return !_is_pressed; }

void Button::reset() { _is_pressed = true; }

void Button::wait_click()
{
    reset();
    while (!is_pressed())
        read();
    while (is_pressed())
        read();
}

int Button::pin() { return _pin; }
