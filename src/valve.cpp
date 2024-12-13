#include "valve.h"
Valve::Valve(int number, int pin, bool is_on){
    m_is_on = is_on;
    m_number = number;
    m_pin = pin;
    m_schedule_count = 0;
}
   

void Valve::toggle(){
    m_is_on = !m_is_on;
    digitalWrite(m_pin, m_is_on);
}

bool Valve::getState(){
    return m_is_on;
}

void Valve::addSchedule(int day, int hour, int minute, bool is_on){
    m_schedules[m_schedule_count] = {day, hour, minute, is_on};
    m_schedule_count ++;
}   

void Valve::checkSchedules(){
struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  for(int i = 0; i<=m_schedule_count;i++){
    Schedule schedule = m_schedules[i];
    if (schedule.day == timeinfo.tm_wday){
        if(schedule.hour>= timeinfo.tm_hour){
            if(schedule.minute >= timeinfo.tm_min){
                m_is_on = schedule.is_on;
                digitalWrite(m_pin, m_is_on);
            }
        }else{
            break;
        }
    }else{
        break;
    }
  }
  
}

int Valve::getScheduleCount(){
    return m_schedule_count;
}

Schedule* Valve::getSchedules(){
    return m_schedules;
}