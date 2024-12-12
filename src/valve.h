#ifndef VALVE_H
#define VALVE_H
#include <Arduino.h>
class Valve{
    private:
        int m_number{};
        int m_pin{};
        bool m_is_on{};
    public:
        Valve(int number, int pin, bool is_on);
        void toggle();

};
#endif