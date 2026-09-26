#include "core/sensors/button/button.h"
#include "config.h"

class ButtonTest
{
public:
    ButtonTest(int pin) : _button(pin) {}
    void setup()
    {
        _button.init();
        Serial.begin(115200);
    }
    void loop()
    {
        _button.reset();
        _button.read();
        Serial.print("Button data: ");
        Serial.println(_button.is_pressed());
    }

private:
    Button _button;
};

ButtonTest test(Pin::PIN_BTN_1);

void setup()
{
    test.setup();
}

void loop()
{
    test.loop();
}