#include "valve.h"
Valve::Valve(int number, int pin, bool is_on){
    m_is_on = is_on;
    m_number = number;
    m_pin = pin;
}
   

void Valve::toggle(){
    m_is_on = !m_is_on;
    digitalWrite(m_pin, m_is_on);
}

bool Valve::getState(){
    return m_is_on;
}