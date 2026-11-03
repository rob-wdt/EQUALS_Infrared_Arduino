#pragma once

// Uses range recieved from distance sensor to decide wether the robot is in rear out or not
class OutsHandlerDistanceSensor
{
public:
    void handle(int rear_range, int threshold);
    bool in_out();

private:
    bool _in_out;
};
