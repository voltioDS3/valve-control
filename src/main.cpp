#include "main.h"

char XML[2048];
Valve valve1(1,19 , false);
Valve valve2(2,18 , false);
Valve valve3(3,5 , false);
Valve valve4(4,17 , false);
Valve* valves[4] = {&valve1, &valve2, &valve3, &valve4};

IPAddress Actual_IP;
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
  Serial.println(valve1.getState());
}

