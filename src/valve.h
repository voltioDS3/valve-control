#ifndef VALVE_H
#define VALVE_H
#include <Arduino.h>
#include <vector>
#include "schedule.h"
#include <time.h>
class Valve{
    private:
        int m_number;
        int m_pin;
        bool m_is_on;
        int m_schedule_count;
        Schedule m_schedules[14] = {-1,-1,-1,-1};
        
    public:
        Valve(int number, int pin, bool is_on);
        void addSchedule(int day, int hour, int minute, bool is_on);
        void toggle();
        bool getState();
        void checkSchedules();
        int getScheduleCount();
        Schedule* getSchedules();

};
#endif