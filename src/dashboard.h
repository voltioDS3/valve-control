#ifndef DASHBOARD_H
#define DASHBOAD_H
#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>
#include "pins.h"
#include "valve.h"
#include "schedule.h"
void setupWebServer();
void runServer();

void toggleDashboard();
const char PAGE_MAIN[] PROGMEM = R"=====(

<!DOCTYPE html>
<html lang="en" class="js-focus-visible">
<head>
    <title>Valvulas</title>
    <meta name="viewport" content="width=device-width, initial-scale=1.0, user-scalable=no">
</head>
<style>
    body {
        margin: 0;
        padding: 0;
        font-family: 'Arial', sans-serif;
        color: #FFFFFF;
        text-align: center;
    }
    header {
        width: 100%;
        color: #131515;
        padding: 10px 0;
        font-size: 1.8rem;
        font-weight: bold;
        background-color: #7CEA9C;
    }
    .main {
        display: flex;
        flex-direction: column;
        align-items: center;
        justify-content: center;
        padding: 10px;
    }
    #valve-buttons-container, .scheduler-container, #valve-schedules-container {
        width: 100%;
        max-width: 400px;
        margin: 20px 0;
    }
    .button {
        background-color: #5c9a72;
        color: #131515;
        border: none;
        padding: 15px;
        margin: 10px 0;
        font-size: 1.8rem;
        border-radius: 20px;
        cursor: pointer;
        width: 100%;
        text-align: center;
        transition: background-color 0.3s ease;
    }
    .button:hover {
        background-color: #7CEA9C;
    }
    .button.on {
        background-color: #7CEA9C;
    }
    .button.off{
        background-color: #5c9a72;
    }
    select, input[type="number"], .submit-schedule {
        font-size: 1.5rem;
        padding: 10px;
        margin: 10px 0;
        border-radius: 10px;
        border: 1px solid #ccc;
        width: 100%;
        box-sizing: border-box;
    }
    label {
        font-size: 1.2rem;
        margin-top: 10px;
        display: block;
    }
    h2, h3 {
        font-size: 1.8rem;
        margin-bottom: 10px;
    }
</style>

<body style="background-color: #131515">
    <header>
        <h1>CONTROL VALVULAS</h1>
    </header>

    <div class="main">
        <div id="valve-buttons-container"></div>
        
        <div class="scheduler-container">
            <h2>Programar Riego</h2>
            <label for="valve">Valvula:</label>
            <select id="valve"></select>

            <label for="days">Dia:</label>
            <select id="days">
                <option value="7">Todos los dias</option>
                <option value="1">Lunes</option>
                <option value="2">Martes</option>
                <option value="3">Miercoles</option>
                <option value="4">Jueves</option>
                <option value="5">Viernes</option>
                <option value="6">Sabado</option>
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

        <div id="valve-schedules-container"></div>
    </div>
</body>

  <script type="text/javascript">
    const day_dictionary={
        7:"Todos los dias",
        0:"Domingo",
        1:"Lunes",
        2:"Martes",
        3:"Miercoles",
        4:"Jueves",
        5:"Viernes",
        6:"Sabado",
    }
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

            const option = document.createElement("option");
            option.value = `${i}`; // Asignar el valor
            option.textContent = `Valvula ${i + 1}`;
            valve_selection.appendChild(option);

            // Al hacer clic en el botón, cambiamos el estado de la válvula
            button.onclick = async () => {
                try {
                    // Realizar la petición al servidor para alternar el estado
                    const toggleResponse = await fetch(`/toggleValve?valve=${i}`);
                    if (toggleResponse.ok) {
                        // Alternar el estado en el cliente
                        data.states[i] = !data.states[i];

                        // Actualizar el estilo del botón
                        button.className = `button ${data.states[i] ? "on" : "off"}`;
                        alert(`Valvula ${i + 1} esta ahora ${data.states[i] ? "ON" : "OFF"}`);
                    } else {
                        alert("Error al intentar cambiar el estado de la valvula.");
                    }
                } catch (error) {
                    console.error("Error al realizar la peticion:", error);
                    alert("No se pudo comunicar con el servidor.");
                }
            };

            valveContainer.appendChild(button);
        }
    };
        const loadSchedules = async () =>{
            const response = await fetch("/schedulesStates");
            const data = await response.json();
            const scheduleContainer = document.getElementById("valve-schedules-container");
            const valveKeys = Object.keys(data); // Obtén las claves del objeto
            const valveCount = valveKeys.length; // Número de válvulas

            console.log(`Numero de válvulas: ${valveCount}`);
          
            for(let i = 0; i<= valveCount; i++){
                 const valveName = valveKeys[i]; // Nombre de la válvula actual
            const schedules = data[valveName]; // Contenido de la válvula (array de horarios)

            console.log(`Horarios de ${valveName}:`, schedules);

            // Ejemplo: Mostrar horarios en el DOM
            const valveElement = document.createElement("div");
            valveElement.innerHTML = `<h3> Valvula ${Number(valveName)+1}</h3><ul></ul>`;
            const scheduleList = valveElement.querySelector("ul");

            schedules.forEach(schedule => {
                const scheduleItem = document.createElement("li");
                scheduleItem.textContent = `Dia: ${day_dictionary[schedule.day]}, Hora: ${schedule.hour}, Minuto: ${schedule.minute}, accion: ${schedule.is_on  ? "Prender" : "Apagar"}`;
                scheduleList.appendChild(scheduleItem);
            });

            scheduleContainer.appendChild(valveElement);
            }

        }

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
        xhr.open("GET", `/scheduleValve?day=${day}&hour=${hour}&minute=${min}&valve=${valve}&action=${action}`);
        xhr.send();
        alert(`Riego programado !`);
    }
        document.addEventListener("DOMContentLoaded", () => {
        loadSchedules();
    });
    window.onload = loadButtons;

  
  </script>

</html>


)=====";

#endif