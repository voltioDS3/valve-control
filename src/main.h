#ifndef MAIN_H
#define MAIN_H

#include <Arduino.h>
#include <WiFi.h>
#include <ESP32Time.h>
#include <TaskScheduler.h>

#include <time.h>

#include "wifi_utils.h"
#include "dashboard.h"
#include "pins.h"
#include "valve.h"


const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = -10800;
const int   daylightOffset_sec = 3600;
unsigned long lastSyncTime = 0; // Última sincronización con NTP
const unsigned long syncInterval = 24 * 60 * 60 * 1000; // Cada 24 horas
const unsigned long valve_check_interval= 30*1000; // chechea cad 5 minutos
unsigned long last_valve_check=0;
#endif