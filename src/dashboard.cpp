#include "dashboard.h"
WebServer server(80);
int valve_count = 4;
bool valve_states[] = {false,false,true,false};
int valve_pins[] = {VALVE1_PIN,VALVE2_PIN,VALVE3_PIN, VALVE4_PIN};

void renderDashboard(){
    server.send(200, "text/html", PAGE_MAIN);
}

void handleValveStates(){
    StaticJsonDocument<200> doc;
    doc["valve_count"] = valve_count;
    JsonArray states = doc.createNestedArray("states");
    for (int i = 0; i < valve_count; i++) {
        states.add(valve_states[i]);
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
            valve_states[valveIndex] = !valve_states[valveIndex]; // Cambiar estado
            digitalWrite(valve_pins[valveIndex] , valve_states[valveIndex]);
            Serial.println("abriendo ");
            server.send(200, "text/plain", valve_states[valveIndex] ? "ON" : "OFF");
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