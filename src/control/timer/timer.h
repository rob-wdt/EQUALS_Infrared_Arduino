#pragma once

class Timer
{
public:
    Timer();
    void set_time_ms(long time_ms);
    bool time_is_up();
    void reset();

private:
    unsigned long _set_time_ms;
    unsigned long _mark;
};
