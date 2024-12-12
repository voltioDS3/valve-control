#include "wifi_utils.h"
static WiFiManager wm;
void setupWifi(){
    WiFi.mode(WIFI_STA);
    bool res;
    res = wm.autoConnect("AutoConnectAP", "password");
    if(!res){
        Serial.println("could not connect");
    }
    else{
        Serial.println("connected to: " + wm.getWiFiSSID());
        Serial.println("password: " + wm.getWiFiPass());
        
    }
    esp_wifi_set_ps(WIFI_PS_MIN_MODEM); // Alternativa: WIFI_PS_MAX_MODEM
    Serial.println("WiFi power-saving mode enabled");
    

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.print("IP address: "); Serial.println(WiFi.localIP());
}