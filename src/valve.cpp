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
Schedule nearest_schedule = m_schedules[0];
struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }

  for(int i = 0; i<m_schedule_count;i++){
    Schedule schedule = m_schedules[i];
    Serial.print("comparando  valvula: ");
    Serial.print(m_number);
    Serial.print("\n");
    Serial.println(schedule.day);
    Serial.println(timeinfo.tm_wday);
    if (schedule.day == timeinfo.tm_wday || schedule.day == 7){
        int nearest_day_minutes = nearest_schedule.hour*60 + nearest_schedule.minute;
        int schedule_day_minutes = schedule.hour*60 + schedule.minute;
        int time_day_minutes = timeinfo.tm_hour*60 + timeinfo.tm_min;
        int minute_difference = time_day_minutes-schedule_day_minutes;
        
        Serial.println("son el mismo dia");
        if (minute_difference < 0){
            continue;
        } else{
            int nearest_schedule_minute_difference = time_day_minutes-nearest_day_minutes;
            if (abs(nearest_schedule_minute_difference) > abs(minute_difference) ){
                Serial.println("cambio nearest schedule");
                Serial.println(abs(nearest_schedule_minute_difference));
                 Serial.println(abs(minute_difference));
                nearest_schedule = schedule;
            }
        }
        
    }else{
        continue;
    }
  }
    if(nearest_schedule.day != -1){
        m_is_on = nearest_schedule.is_on;
        digitalWrite(m_pin, nearest_schedule.is_on);

    }
  
}

int Valve::getScheduleCount(){
    return m_schedule_count;
}

Schedule* Valve::getSchedules(){
    return m_schedules;
}