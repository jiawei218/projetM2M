#include "composents.h"

//LED Configuration: Pin7
ChainableLED leds(7, 8, NUM_LEDS);
// Ethernet server(port:80)
EthernetServer server(80);
//address ip of the board(server)
IPAddress ip(192, 168, 1, 7);
// address mac of the board 
    byte mac[] ={ 0xA8, 0x61, 0x0A, 0xAE, 0x70, 0x1D };

/**
* @brief Constructor for the 'server' class.
*/
composents::composents()
{
  sensorValue=0;


}

/**
 * @brief Destructor for the 'server' class.
 */
composents::~composents()
{
}


/**
 * @brief Initializes components, including BME280 sensor, LED, and Ethernet server.
 * @return 0 on success, 1 on failure.
 */
int composents::setup()
{
  // Initialize BME280 sensor
  if(!bme280.init()){
      Serial.println("Device error!");
    }

  // Initialize LED
  leds.init();
  leds.setColorRGB(0, 0, 0, 0);

  // Start Ethernet connection and server
  Serial.println("Ethernet WebServer Example");
  // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip);
  // start the server
  server.begin();
  return 0;
}


/**
 * @brief Main function to run the components, including reading sensor values and handling client requests.
 */
void composents::run(){
  // Read analog signal from the potentiometer
  sensorValue = analogRead(A0);

  // Check for incoming client requests
  client = server.available();
  if (client) {
    Serial.println("new client");
    // An HTTP request ends with a blank line
    collectRequest();

    // Check the type of request and handle accordingly
    if(requestRev.indexOf("ledconfig") >-1)
    {
        handleLEDRequest();
    }
    else if(requestRev.indexOf("majCapteur") >-1)
    {
        handleMajDataRequest();
    }

    // Close the client connection
    client.stop();
    Serial.println("client disconnected");
    // Clear the request buffer
    requestRev="";
  }
}


/**
 * @brief Function to load the HTML page for the client.
 */
void composents::loadPage()
{
    client.println("HTTP/1.1 200 OK");
    client.println("Content-type:text/html");
    client.println("Connection: close");
    client.println();

    // Send the HTML page stored in program memory
    for(int k=0;k<strlen_P(index_html);k++)
    {
      mychar=pgm_read_byte_near(index_html+k);
      client.print(mychar);
    }
}

/**
 * @brief Function to collect the HTTP request from the client.
 */
void composents::collectRequest()
{
    bool currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        requestRev+=c;
        if (c == '\n' && currentLineIsBlank) {
          // If a blank line is encountered, load the HTML page
          loadPage();
          break;
        }
        if (c == '\n') {
          // Starting a new line
          currentLineIsBlank = true;
        } else if (c != '\r') {
          // Character on the current line
          currentLineIsBlank = false;
        }
      }
    }
}

/**
 * @brief Function to handle LED configuration request from the client.
 */
void composents::handleLEDRequest()
{
  //Extract RGB values from the request
      int red=requestRev.substring(requestRev.indexOf("red")+4,requestRev.indexOf("green")-1).toInt();
      int green=requestRev.substring(requestRev.indexOf("green")+6,requestRev.indexOf("blue")-1).toInt();
      int blue=requestRev.substring(requestRev.indexOf("blue")+5,requestRev.indexOf("blue")+8).toInt();
      // Set LED color
      leds.setColorRGB(0, red, green, blue);
}

/**
 * @brief Function to handle sensor data request from the client.
 */
void composents::handleMajDataRequest()
{
  String response="";
  response.concat("HTTP/1.1 200 OK\r\nContent-Type: application/json\r\n\r\n");

  // Construct JSON response with sensor data
  response += "{ \"temperature\" :";
  response.concat(bme280.getTemperature());
  response +=  ", \"humidity\": ";
  response.concat(bme280.getHumidity());
  response += ", \"pressure\": ";
  response.concat(bme280.getPressure()/100);
  response +=", \"potentiometre\": ";
  response.concat(sensorValue);
  response += " }" ;

  Serial.println(response);
  client.print(response);
}
