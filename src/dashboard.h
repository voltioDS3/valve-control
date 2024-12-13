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
        <div class="scheduler-container">
            <div class="valve-scheduler">
                <div class="scheduler-container">
                    <h2>Programar Riego</h2>
                    <label for="valve">Valvula:</label>
                    <select id="valve">
                        
                    </select>
                    <label for="days">Día:</label>
                    <select id="days">
                        <option value="7">Todos los dias</option>
                        <option value="1">Lunes</option>
                        <option value="2">Martes</option>
                        <option value="3">Miércoles</option>
                        <option value="4">Jueves</option>
                        <option value="5">Viernes</option>
                        <option value="6">Sábado</option>
                        <option value="0">Domingo</option>
                    </select>
        
                    <label for="hour">Hora:</label>
                    <input type="number" id="hour" min="0" max="23" placeholder="Hora (0-23)">
        
                    <label for="minute">Minuto:</label>
                    <input type="number" id="minute" min="0" max="59" placeholder="Minuto (0-59)">
                    
                    <label for="action">Accion:</label>
                    <select id="action">
                        <option value="1">Prender</option>
                        <option value="0">Apagar</option>
                    </select>
                    <button class="submit-schedule" onclick="scheduleValve()">Programar</button>
                </div>
            </div>
        </div>
    </div>
    
  </body>

  <script type="text/javascript">
    const loadButtons = async () => {
            const response = await fetch("/valveStates");
            const data = await response.json();
            const valveContainer = document.getElementById("valve-buttons-container");
            const valve_selection = document.getElementById("valve");
            valveContainer.innerHTML = "";
            valve_selection.innerHTML = "";
            for (let i = 0; i < data.valve_count; i++) {
                const button = document.createElement('button');
                button.className = `button ${data.states[i] ? "on" : ""}`;
                button.textContent = `Valvula ${i + 1}`;
                
                const option = document.createElement("option")
                option.value = `${i}`; // Asignar el valor
                option.textContent = `Valvula ${i+1}`; 
                valve_selection.appendChild(option);
                
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

        const scheduleValve = () =>{
        const day = document.getElementById("days").value;
        const valve = document.getElementById("valve").value;
        const hour = document.getElementById("hour").value;
        const min = document.getElementById("minute").value;
        const action = document.getElementById("action").value;
        if (hour === "" || minute === "") {
            alert("Por favor, ingresa una hora y un minuto válidos.");
            return;
        }

        const xhr = new XMLHttpRequest();
        xhr.open("GET", `/scheduleValve?day=${day}&hour=${hour}&minute=${minute}&valve=${valve}&action=${action}`);
        xhr.send();
        alert(`Riego programado !`);
    }

    window.onload = loadButtons;

  
  </script>

</html>


)=====";

#endif