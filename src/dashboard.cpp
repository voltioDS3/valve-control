#include "dashboard.h"
WebServer server(80);
int valve_count = 4;


Valve* valve_array[4] = {&valve1,&valve2,&valve3,&valve4};

void renderDashboard(){
    server.send(200, "text/html", PAGE_MAIN);
}

void handleValveStates(){
    StaticJsonDocument<200> doc;
    doc["valve_count"] = valve_count;
    JsonArray states = doc.createNestedArray("states");
    for (int i = 0; i < valve_count; i++) {
        Serial.print("valvula ");
        Serial.print(i);
        Serial.print(valves[i]->getState());
        states.add(valves[i]->getState());
    }
    String jsonResponse;
    serializeJson(doc, jsonResponse);
    server.send(200, "application/json", jsonResponse);
}

void handleValveSchedule(){
    
}
void handleToggleValve() {
    if (server.hasArg("valve")) {
        int valveIndex = server.arg("valve").toInt();
        if (valveIndex >= 0 && valveIndex < valve_count) {
            valves[valveIndex]->toggle();
            Serial.println("abriendo ");
            server.send(200, "text/plain", valves[valveIndex]->getState() ? "ON" : "OFF");
            return;
        }
    }
    server.send(400, "text/plain", "Invalid valve index");
}
void setupWebServer(){
    server.on("/", renderDashboard );
    server.on("/valveStates", handleValveStates);
    server.on("/toggleValve", handleToggleValve);
    server.begin();
    Serial.println("Server started");

}


void runServer(){
    server.handleClient();
}

void toggleDashboard(){
    Serial.println("intento cambiar desde dashboard");
    valve_array[0]->toggle();
    delay(5000);
}