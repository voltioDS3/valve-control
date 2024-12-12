#ifndef DASHBOARD_H
#define DASHBOAD_H
#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>
#include "pins.h"
#include "valve.h"

void setupWebServer();
void runServer();

void toggleDashboard();
const char PAGE_MAIN[] PROGMEM = R"=====(

<!DOCTYPE html>
<html lang="en" class="js-focus-visible">
<head>
    <title>Valvulas</title>
</head>
  <style>
    body {
        margin: 0;
        padding: 0;
        font-family: 'Arial', sans-serif; /* Cambia la tipografía */
        color: #FFFFFF; /* Texto claro */
    }
    header {
        width: 100%;
        height: 5vh;
        color: #131515; /* Texto oscuro */
        text-align: center;
        display: flex;
        flex-direction: row;
        align-items: center;
        justify-content: center;
        padding: 15px 0;
        font-size: 1.5rem;
        font-weight: bold;
    }
    .main {
        display: flex;
        flex-direction: column;
    }

    #valve-buttons-container{
        margin-top: 20px;
        
        display: flex;
        flex-direction: column;
        align-self: center;
        width: 80%; 
    }
    .button {
        
        background-color: #5c9a72;
        color: #131515;
        border: none;
        padding: 10px 20px;
        margin: 5px;
        font-size: 2rem;
        border-radius: 30px;
        cursor: pointer;
        transition: background-color 0.3s ease;
    }
    .button:hover {
        background-color: #5c9a72;
    }
    .button.on {
        background-color: #7CEA9C;
    }
  </style>

  <body style="background-color: #131515" onload="process()">
    <header style="background-color: #7CEA9C">
        <h1>CONTROL VALVULAS</h1>
    </header>

    <div class="main">
        <div id="valve-buttons-container">

        </div>
    </div>
    
  </body>

  <script type="text/javascript">
    const loadButtons = async () => {
            const response = await fetch("/valveStates");
            const data = await response.json();
            const valveContainer = document.getElementById("valve-buttons-container");

            valveContainer.innerHTML = "";

            for (let i = 0; i < data.valve_count; i++) {
                const button = document.createElement('button');
                button.className = `button ${data.states[i] ? "on" : ""}`;
                button.textContent = `Valvula ${i + 1}`;
                
                // Al hacer clic en el botón, cambiamos el estado de la válvula
                button.onclick = () => {
                    // Cambiar el estado (toggle)
                    data.states[i] = !data.states[i];

                    // Actualizar el estilo del botón
                    button.className = `button ${data.states[i] ? "on" : ""}`;

                    // Aquí podrías hacer una petición al servidor para cambiar el estado real
                    var xhr = new XMLHttpRequest();
                    xhr.open("GET", `/toggleValve?valve=${i}`);
                    xhr.send();
                    alert(`Valvula ${i + 1} esta ahora ${data.states[i] ? "ON" : "OFF"}`);
                };
                valveContainer.appendChild(button);
            }
        };

    window.onload = loadButtons;

  
  </script>

</html>


)=====";

#endif