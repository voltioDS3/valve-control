#include "valve.h"
Valve::Valve(int number, int pin, bool is_on)
    :
    m_number{number}
    , m_pin{pin},
    m_is_on{is_on}{

    }

void Valve::toggle(){
    m_is_on = !m_is_on;
    digitalWrite(m_pin, m_is_on);
}
