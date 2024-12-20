#include "dashboard.h"
WebServer server(80);
int valve_count = 4;


Valve* valve_array[4] = {&valve1,&valve2,&valve3,&valve4};

void renderDashboard(){
    server.send(200, "text/html", PAGE_MAIN);
}

void handleRemoveSchedule(){
    if (server.hasArg("valve") &&  server.hasArg("hour") && server.hasArg("minute")&& server.hasArg("day") && server.hasArg("isOn")) {

        int valveIndex = server.arg("valve").toInt();
        int is_on = server.arg("isOn").toInt();
        int hour = server.arg("hour").toInt();
        int minute = server.arg("minute").toInt();
        int day = server.arg("day").toInt();

        Preferences preferences;
        char valve_name[20]; 
        sprintf(valve_name, "valvula_%d", valveIndex+1);

        if (preferences.begin(valve_name, false)) {
            #ifdef DEBUG
            Serial.println("Preferences opened");
            #endif
        } else {
            #ifdef DEBUG
            Serial.println("Failed to open preferences");
            #endif
            return;
        }
        int schedule_count = preferences.getInt("schedule_count");
        
        char schedule_to_remove[20];
        sprintf(schedule_to_remove, "%d;%d;%d;%d", minute,hour,day,is_on);

        for (int i = 0; i < schedule_count; i++){
            char schedule_name[20];
            sprintf(schedule_name,"schedule_%d", i);
            String schedule_string = preferences.getString(schedule_name);
            #ifdef DEBUG
            Serial.print("el horario leido de la flash es: ");
            Serial.println(schedule_string);

            Serial.print("el horario a comparar es: ");
            Serial.println(schedule_to_remove);
            #endif
            if (schedule_string == schedule_to_remove){
                #ifdef DEBUG
                Serial.println("MATCH");
                #endif
            }

        }
        

   

        
        preferences.end();

    }
}

void handleValveStates(){
    StaticJsonDocument<200> doc;
    doc["valve_count"] = valve_count;
    JsonArray states = doc.createNestedArray("states");
    for (int i = 0; i < valve_count; i++) {
        #ifdef DEBUG
        Serial.print("valvula ");
        Serial.print(i);
        Serial.print(valves[i]->getState());
        #endif
        states.add(valves[i]->getState());
    }
    String jsonResponse;
    serializeJson(doc, jsonResponse);
    server.send(200, "application/json", jsonResponse);
}


void handleToggleValve() {
    if (server.hasArg("valve")) {
        int valveIndex = server.arg("valve").toInt();
        if (valveIndex >= 0 && valveIndex < valve_count) {
            valves[valveIndex]->toggle();
            #ifdef DEBUG
            Serial.println("abriendo ");
            #endif
            server.send(200, "text/plain", valves[valveIndex]->getState() ? "ON" : "OFF");
            return;
        }
    }
    server.send(400, "text/plain", "Invalid valve index");
}

void handleScheduleValve(){
    if (server.hasArg("valve") &&  server.hasArg("hour") && server.hasArg("minute")&& server.hasArg("day") && server.hasArg("action")) {
        int valveIndex = server.arg("valve").toInt();
        int is_on = server.arg("action").toInt();
        int hour = server.arg("hour").toInt();
        int minute = server.arg("minute").toInt();
        int day = server.arg("day").toInt();

        if (valveIndex >= 0 && valveIndex < valve_count) {
            valves[valveIndex]->addSchedule(day,hour, minute, is_on);
            #ifdef DEBUG
            Serial.print("schedulando valvula");
            Serial.println(valveIndex);
            #endif
            server.send(200, "text/plain", "bien");
            return;
        }
    }
    server.send(400, "text/plain", "Invalid valve index");
}

void handleSchedulesStates(){
    DynamicJsonDocument doc(200);
    for(int i = 0; i<VALVE_COUNT;i++){
        JsonArray valveSchedules= doc.createNestedArray(i);
        int schedule_count = valves[i]->getScheduleCount();
        Schedule* schedules = valves[i]->getSchedules();
        for(int j = 0; j<schedule_count;j++){
            JsonObject scheduleObject = valveSchedules.createNestedObject();
            scheduleObject["day"] = schedules[j].day;
            scheduleObject["hour"] = schedules[j].hour;
            scheduleObject["minute"] = schedules[j].minute;
            scheduleObject["is_on"] = schedules[j].is_on;
                        
        }
    }
    String jsonResponse;
    serializeJson(doc, jsonResponse);
    #ifdef DEBUG
    Serial.println(jsonResponse);
    #endif
    server.send(200, "application/json", jsonResponse);
    


}
void setupWebServer(){
    server.on("/", renderDashboard );
    server.on("/valveStates", handleValveStates);
    server.on("/toggleValve", handleToggleValve);
    server.on("/scheduleValve", handleScheduleValve);
    server.on("/schedulesStates", handleSchedulesStates);
    server.on("/removeSchedule", handleRemoveSchedule);
    server.begin();
    #ifdef DEBUG
    Serial.println("Server started");
    #endif
}


void runServer(){
    server.handleClient();
}

void toggleDashboard(){
    #ifdef DEBUG
    Serial.println("intento cambiar desde dashboard");
    #endif
    valve_array[0]->toggle();
    delay(5000);
}

