#include "main.h"

char XML[2048];

IPAddress Actual_IP;
Valve valve1(1,VALVE1_PIN , false);
Valve valve2(2,VALVE2_PIN , false);
Valve valve3(3,VALVE3_PIN , false);
Valve valve4(4,VALVE4_PIN , false);
Valve valve_list[] = {valve1,valve2,valve3,valve4};

Scheduler ts;
void setup() {
  Serial.begin(115200);
  setupWifi();  
  setupWebServer();
  
  pinMode(VALVE1_PIN,OUTPUT);
  pinMode(VALVE2_PIN,OUTPUT);
  pinMode(VALVE3_PIN,OUTPUT);
  pinMode(VALVE4_PIN,OUTPUT);
  

}


void loop() {
  runServer();
  // put your main code here, to run repeatedly:
}

