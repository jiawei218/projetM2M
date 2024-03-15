#ifndef composents_H
#define composents_H

#include <SPI.h>
#include <Ethernet2.h>
#include "Seeed_BME280.h"
#include <ChainableLED.h>

#define NUM_LEDS  1



// HTML Page stored in program memory
const char PROGMEM index_html[] = R"=====(
<!DOCTYPE html>
<html lang='en'>
<head>
    <meta charset='UTF-8'>
    <meta name='viewport' content='width=device-width, initial-scale=1.0'>
    <title>Arduino Web Server</title>

    <style>
        body {
            font-family: 'Arial', sans-serif;
            margin: 20px;
            background-color: #00000;
            text-align: center;
        }

        h1 {
            color: #333;
            margin-bottom: 20px;
        }

        table {
            width: 80%;
            margin: 0 auto;
            border-collapse: collapse;
            margin-top: 20px;
            background-color: #fff;
            box-shadow: 0 4px 8px rgba(0, 0, 0, 0.1);
        }

        th, td {
            border: 1px solid #ddd;
            padding: 8px;
            text-align: center;
        }

        th {
            background-color: #f2f2f2;
        }

        span {
            font-weight: bold;
        }

        button {
            background-color: #4CAF50;
            color: white;
            padding: 10px 15px;
            border: none;
            border-radius: 5px;
            cursor: pointer;
            font-size: 16px;
        }

        form {
            margin-top: 20px;
        }

        label {
            display: block;
            margin-bottom: 20px;
            color: #333;
            font-size: 18px;
        }

        input[type='color'] {
            margin-bottom: 10px;
        }

        #submit {
            background-color: #008CBA;
        }
    </style>
</head>
<body>

      <table >
        <h1>Affichage des données du capteur BME et Potentiomètre</h1>
        <thead>
         <tr>
            <th><h2>Température</h2></th>
                <th><h2>Taux d'humidite</h2></th>
                <th><h2>Pression</h2></th>
                <th><h2>Potentiomètre</h2></th>
        </tr>
        </thead>
          <tbody>
          <tr>
            <td><span id='temperature'></span> &#176C</td>
            <td><span id='humidite'></span> %</td>
            <td><span id='pression'></span> HPa</td>
            <td><span id='potentiometre'></span> </td>
          </tr>
          </tbody>
      </table><p>
          <div >
            <button class='button' onclick='refresh()'>Rafraichir</button>
          </div>



    <h1>LED Configuration</h1>
    <form id='led-control'>
        <label>Choisir la couleur de la LED :</label>
        <input type='color' id='colorInput'><br>
        <button id='submit' type='submit' >Envoyer</button>
    </form>
</body>

    <script>
         function hexToRgb(hex) {
            hex = hex.replace(/^#/, '');
            const res = [];
            // red green blue
            res[0] = parseInt(hex.slice(0, 2), 16);
            res[1] = parseInt(hex.slice(2, 4), 16);
            res[2] = parseInt(hex.slice(4, 6), 16);

            return res;
        }

        //bme
          let temperature = '0';
          let humidite = '0';
          let pression = '0';
          let potentiometre='0';
          document.getElementById('temperature').innerHTML = temperature;
          document.getElementById('humidite').innerHTML = humidite;
          document.getElementById('pression').innerHTML = pression;
          document.getElementById('potentiometre').innerHTML = potentiometre;

          function refresh(){
            var request = new XMLHttpRequest();
            request.open('GET', '/majCapteur', true);
            request.onload = function() {
              if (request.status == 200 && request.readyState == XMLHttpRequest.DONE) {
                var str = request.responseText.split("HTTP/1.1 200 OK\r\nContent-Type: application/json\r\n\r\n");
                if(str[1]){
                  var data = JSON.parse(str[1]);
                  document.getElementById('temperature').innerHTML = data.temperature;
                  document.getElementById('humidite').innerHTML = data.humidity;
                  document.getElementById('pression').innerHTML = data.pressure;
                  document.getElementById('potentiometre').innerHTML = data.potentiometre;
                }
              }
            };
            request.send();
          }

          document.addEventListener("DOMContentLoaded", function() {
          refresh();
        });

        //led
        const ledConf=document.getElementById('led-control');
        ledConf.addEventListener('submit',(event)=>{
            event.preventDefault();
            const color=document.getElementById('colorInput').value;
            colorRGB=hexToRgb(color);
            var request=new XMLHttpRequest();
            request.open('POST', '/ledconfig?red=' + colorRGB[0]+'&green='+colorRGB[1]+'&blue='+colorRGB[2], true);
            request.setRequestHeader("Content-Type", "application/x-www-form-urlencoded");
            request.send();
            request.onload=function(){
                if(request.status!=200)
                {
                    alert(`Error ${request.status}: ${request.statusText}`);
                }
            }
        } )
    </script>
</html>

)=====";

class composents
{
  private:
    // BME280 sensor object for temperature, humidity, and pressure
    BME280 bme280; // I2C

    //analog sensor value from Potentiomètre
    int sensorValue;

    // String to store the received HTTP request
    String requestRev;

    // Temporary character storage
    char mychar;

// Ethernet client
    EthernetClient client;

  public:
    composents();
    ~composents();
    int setup();
    void run();
    void loadPage();
    void collectRequest();
    void handleLEDRequest();
    void handleMajDataRequest();


};


#endif
