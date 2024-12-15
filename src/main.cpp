#include "main.h"


Valve valve1(1,19 , false);
Valve valve2(2,18 , false);
Valve valve3(3,5 , false);
Valve valve4(4,17 , false);
Valve* valves[4] = {&valve1, &valve2, &valve3, &valve4};


void syncRTC();

void setup() {
  Serial.begin(9600);
  for (int i = 0; i< VALVE_COUNT;i++){
    valves[i]->updateFromFlash();
  }
  setupWifi();  
  setupWebServer();
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  syncRTC();

  pinMode(VALVE1_PIN,OUTPUT);
  pinMode(VALVE2_PIN,OUTPUT);
  pinMode(VALVE3_PIN,OUTPUT);
  pinMode(VALVE4_PIN,OUTPUT);
  


}

void syncRTC(){
  Serial.println("Sincronizando tiempo desde NTP...");
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  struct tm timeinfo;
  
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Falló la sincronización con NTP");
    return;
  }
  
  // Configura el RTC interno con el tiempo sincronizado
  time_t now;
  time(&now); // Obtiene la hora actual en formato epoch
  struct timeval tv = { now, 0 }; // Hora en segundos y microsegundos
  settimeofday(&tv, nullptr); // Configura el RTC interno del ESP32
  
  Serial.println("RTC sincronizado con NTP");
}


void printLocalTime(){
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
  Serial.print("Day of week: ");
  Serial.println(&timeinfo, "%A");
  Serial.print("Month: ");
  Serial.println(&timeinfo, "%B");
  Serial.print("Day of Month: ");
  Serial.println(&timeinfo, "%d");
  Serial.print("Year: ");
  Serial.println(&timeinfo, "%Y");
  Serial.print("Hour: ");
  Serial.println(&timeinfo, "%H");
  Serial.print("Hour (12 hour format): ");
  Serial.println(&timeinfo, "%I");
  Serial.print("Minute: ");
  Serial.println(&timeinfo, "%M");
  Serial.print("Second: ");
  Serial.println(&timeinfo, "%S");

  Serial.println("Time variables");
  char timeHour[3];
  strftime(timeHour,3, "%H", &timeinfo);
  Serial.println(timeHour);
  char timeWeekDay[10];
  strftime(timeWeekDay,10, "%A", &timeinfo);
  Serial.println(timeWeekDay);
  Serial.println();
}

void loop() {
  runServer();
  // put your main code here, to run repeatedly:
  if (millis() - lastSyncTime > syncInterval) {
    syncRTC();
    lastSyncTime = millis();
  }

  if(millis() - last_valve_check > valve_check_interval){
    printLocalTime();
      for(int i=0; i<VALVE_COUNT;i++){
        valves[i]->checkSchedules();
      }
    last_valve_check = millis();
    //comment
  }
  

}

