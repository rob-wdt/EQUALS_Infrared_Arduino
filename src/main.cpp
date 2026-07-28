#include "config.h"
#include "core/sensors/phototransistor/phototransistor.h"
#include "core/sensors/button/button.h"

#if DEBUG_MODE == DEBUG_MODE_NONE
void setup()
{
}

void loop()
{
}

#elif DEBUG_MODE == DEBUG_MODE_PHOTOTRANSISTOR
Phototransistor pt(Pin::PIN_PHOTOTRANSISTOR);

void setup()
{
    pt.init();
    Serial.begin(115200);
}

void loop()
{
    pt.read();
    Serial.print("Phototr. data: ");
    Serial.println(pt.data());
}

#elif DEBUG_MODE == DEBUG_MODE_BUTTON
Button button(Pin::PIN_BTN_1);

void setup()
{
    button.init();
}

void loop()
{
    button.read();
    Serial.print("Button data: ");
    Serial.println(button.is_pressed());
}

#endif
