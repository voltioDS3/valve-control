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
    Preferences preferences;
    char valve_name[20]; 
    sprintf(valve_name, "valvula_%d", m_number);
    if (preferences.begin(valve_name, false)) {
        Serial.println("Preferences opened");
    } else {
        Serial.println("Failed to open preferences");
        return;
    }

    char schedule_name[20];
    sprintf(schedule_name, "schedule_%d", m_schedule_count);

    char schedule_to_flash[20];
    sprintf(schedule_to_flash, "%d;%d;%d;%d", minute, hour,day,is_on);

    preferences.putString(schedule_name, schedule_to_flash);

    m_schedule_count ++;
    preferences.putInt("schedule_count", m_schedule_count);
    preferences.end();
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

void Valve::updateFromFlash(){
    Preferences preferences;
    char valve_name[20]; 
    sprintf(valve_name, "valvula_%d", m_number);
    Serial.println(valve_name);
    if (preferences.begin(valve_name, false)) {
        Serial.println("Preferences opened");
    } else {
        Serial.println("Failed to open preferences");
        return;
    }

    m_schedule_count = preferences.getInt("schedule_count");

    for (int i = 0; i < m_schedule_count;i++){
        char schedule_name[20];
        sprintf(schedule_name, "schedule_%d", i);
        String schedule_string = preferences.getString(schedule_name);
        Serial.println(schedule_name);
        Serial.println(schedule_string);
        int minuto;
        int dia;
        int hora;
        bool is_on;
        // Dividir la cadena en partes
        int firstIndex = schedule_string.indexOf(';');
        int secondIndex = schedule_string.indexOf(';', firstIndex + 1);
        int thirdIndex = schedule_string.indexOf(';', secondIndex + 1);

        // Convertir cada parte a los tipos de datos correspondientes
        if (firstIndex > 0 && secondIndex > 0 && thirdIndex > 0) {
            minuto = schedule_string.substring(0, firstIndex).toInt();
            hora = schedule_string.substring(firstIndex + 1, secondIndex).toInt();
            dia = schedule_string.substring(secondIndex + 1, thirdIndex).toInt();
            is_on = schedule_string.substring(thirdIndex + 1).toInt() == 1; // 1 = true, 0 = false
        }
        m_schedules[i] = {dia,hora,minuto,is_on};
        Serial.println("update correcto");

    }
    preferences.end();
}